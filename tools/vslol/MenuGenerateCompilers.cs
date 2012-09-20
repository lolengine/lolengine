using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel.Design;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Runtime.InteropServices;
using System.Text;

using EnvDTE;

using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;

using VSConstants = Microsoft.VisualStudio.VSConstants;

namespace Lol.VisualStudio.Plugin
{
    internal class MenuGenerateCompilers : OleMenuCommand
    {
        public MenuGenerateCompilers(ServiceProvider sp, CommandID id) :
            base(new EventHandler(ClickCallback), id, VsLol.ResourceManager.GetString("GenerateCompilersText"))
        {
            this.sp = sp;
            this.projects = new List<Project>();
            this.BeforeQueryStatus += new EventHandler(OnBeforeQueryStatus);
        }

        private void OnBeforeQueryStatus(object sender, EventArgs e)
        {
            projects.Clear();

            var cmd = sender as OleMenuCommand;
            if (cmd == null)
                return;

            IVsMonitorSelection monitorSelection = sp.GetService(typeof(IVsMonitorSelection)) as IVsMonitorSelection;
            if (monitorSelection == null)
                return;

            IntPtr hier = IntPtr.Zero;
            UInt32 itemid;
            IVsMultiItemSelect multiitem = null;
            IntPtr container = IntPtr.Zero;

            try
            {
                monitorSelection.GetCurrentSelection(out hier, out itemid, out multiitem, out container);

                /* Bail out if nothing is selected */
                if (itemid != VSConstants.VSITEMID_SELECTION && itemid != VSConstants.VSITEMID_NIL)
                {
                    if (hier == IntPtr.Zero)
                    {
                        /* FIXME: parse the whole solution */
                    }
                    else
                    {
                        object project = null;

                        IVsHierarchy hierarchy = (IVsHierarchy)Marshal.GetObjectForIUnknown(hier);
                        hierarchy.GetProperty(VSConstants.VSITEMID_ROOT, (int)__VSHPROPID.VSHPROPID_ExtObject, out project);
                        projects.Add(project as Project);
                    }
                }
            }
            finally
            {
                if (hier != IntPtr.Zero)
                    Marshal.Release(hier);

                if (container != IntPtr.Zero)
                    Marshal.Release(container);
            }

            // If there are .l or .y files in this project, display the context menu
            Visible = false;
            foreach (Project project in projects)
                foreach (ProjectItem item in ParseProjectItems(project))
                {
                    if (item.Name.EndsWith("-scanner.l")
                         || item.Name.EndsWith("-parser.y"))
                        Visible = true;
                }
        }

        private static void ClickCallback(object sender, EventArgs args)
        {
            MenuGenerateCompilers cmd = sender as MenuGenerateCompilers;
            if (cmd == null)
                return;

            cmd.ClearOutputPane();
            cmd.WriteToOutputPane("------ Build started: Generating Compilers ------\n");

            int scanner_count = 0, parser_count = 0, error_count = 0;

            foreach (Project project in cmd.projects)
            {
                cmd.WriteToOutputPane("Project " + project.Name + "\n");

                string project_path = Path.GetDirectoryName(project.FullName);

                /* FIXME: find this using the solution globals! */
                string contrib_path = project_path;
                for (int i = 0; i < 10; ++i)
                {
                    contrib_path += "\\..";
                    if (Directory.Exists(contrib_path + "\\contrib"))
                        break;
                }

                /* FIXME: do not hardcode shit! */
                string flex_path = contrib_path + "\\contrib\\flex-2.5.35";
                string bison_path = contrib_path + "\\contrib\\bison-2.4.2";

                // Run flex on all the .l files
                foreach (ProjectItem item in ParseProjectItems(project))
                {
                    string filename = item.get_FileNames(0);

                    if (filename.StartsWith(project_path + "\\"))
                    {
                        filename = filename.Substring(project_path.Length + 1);
                        filename = filename.Replace("\\", "/");
                    }

                    if (item.Name.EndsWith("-scanner.l"))
                    {
                        cmd.WriteToOutputPane("flex.exe " + filename + "\n");

                        string basename = Path.GetFileName(filename.Substring(0, filename.LastIndexOf("-scanner.l")));
                        if (!cmd.Run(project_path,
                                     flex_path + "\\bin\\flex.exe",
                                     "-v -o "
                                      + "generated/" + basename + "-scanner.cpp "
                                      + filename,
                                     ""))
                            ++error_count;

                        ++scanner_count;
                    }

                    if (item.Name.EndsWith("-parser.y"))
                    {
                        cmd.WriteToOutputPane("bison.exe " + filename + "\n");

                        string basename = Path.GetFileName(filename.Substring(0, filename.LastIndexOf("-parser.y")));
                        if (!cmd.Run(project_path,
                                     bison_path + "\\bin\\bison.exe",
                                     "-v -o "
                                      + "generated/" + basename + "-parser.cpp "
                                      + "--defines=generated/" + basename + "-parser.h "
                                      + "-d "
                                      + "-b "
                                      + "generated/" + basename + " "
                                      + filename,
                                     "BISON_PKGDATADIR=" + bison_path + "\\share\\bison"))
                            ++error_count;

                        ++parser_count;
                    }
                }
            }

            cmd.WriteToOutputPane(string.Format("========== Done: {0} scanner(s), {1} parser(s), {2} error(s) ==========\n",
                                  scanner_count, parser_count, error_count));
        }

        bool Run(string directory, string executable, string arguments, string env)
        {
            System.Diagnostics.Process p = new System.Diagnostics.Process();
            p.StartInfo.FileName = executable;
            p.StartInfo.Arguments = arguments;
            foreach (string s in env.Split(new char[]{'\n'}, StringSplitOptions.RemoveEmptyEntries))
            {
                int i = s.IndexOf("=");
                if (i > 0 && i < s.Length - 1)
                    p.StartInfo.EnvironmentVariables[s.Substring(0, i - 1)] = s.Substring(i + 1);
            }
            p.StartInfo.WorkingDirectory = directory;
            p.StartInfo.CreateNoWindow = true;
            p.StartInfo.WindowStyle = ProcessWindowStyle.Hidden;
            p.StartInfo.RedirectStandardError = true;
            p.StartInfo.RedirectStandardOutput = true;
            p.StartInfo.RedirectStandardInput = true;
            p.StartInfo.UseShellExecute = false;

            try
            {
                p.Start();
                string output = p.StandardError.ReadToEnd()
                              + p.StandardOutput.ReadToEnd();
                p.WaitForExit();
                WriteToOutputPane(output);
                if (p.ExitCode != 0)
                {
                    WriteToOutputPane("Error: " + executable + " exited with code " + p.ExitCode + "\n");
                    return false;
                }
            }
            catch (Exception e)
            {
                WriteToOutputPane("Error: failed to launch " + executable + "\n");
                return false;
            }

            return true;
        }

        private void ClearOutputPane()
        {
            IVsOutputWindow win = sp.GetService(typeof(SVsOutputWindow)) as IVsOutputWindow;
            if (null == win)
            {
                Trace.WriteLine("Failed to get a reference to IVsOutputWindow");
                pane = null;
            }

            Guid guid = Microsoft.VisualStudio.VSConstants.OutputWindowPaneGuid.BuildOutputPane_guid;
            if (Microsoft.VisualStudio.ErrorHandler.Failed(win.GetPane(ref guid, out pane)))
            {
                Trace.WriteLine("Failed to get a reference to the Output window Build pane");
                pane = null;
            }

            pane.Activate();
            pane.Clear();
        }

        private void WriteToOutputPane(string s)
        {
            if (pane != null)
                pane.OutputString(s);
        }

        private static IEnumerable<ProjectItem> ParseProjectItems(object o)
        {
            ProjectItems subitems;
            if (o is Project)
            {
                subitems = (o as Project).ProjectItems;
            }
            else
            {
                yield return (o as ProjectItem);
                subitems = (o as ProjectItem).ProjectItems;
            }

            foreach (ProjectItem item in subitems)
                foreach (ProjectItem i in ParseProjectItems(item))
                    yield return i;
        }

        private ServiceProvider sp;
        private IVsOutputWindowPane pane;

        private List<Project> projects;
    }
}

