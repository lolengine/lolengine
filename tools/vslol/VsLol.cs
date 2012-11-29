using System;
using System.ComponentModel.Design;
using System.Diagnostics;
using System.Globalization;
using System.Runtime.InteropServices;
using System.Security.Permissions;
using System.Text;
using IOleServiceProvider = Microsoft.VisualStudio.OLE.Interop.IServiceProvider;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.Shell.Interop;

namespace Lol.VisualStudio.Plugin
{
    [PackageRegistration(UseManagedResourcesOnly = true)]

    /* LolFx syntax highlighting */
    [ProvideServiceAttribute(typeof(LolFxLanguageService),
                             ServiceName = "LolFx Service")]
    [ProvideLanguageServiceAttribute(typeof(LolFxLanguageService),
                                     "LolFx", 106 /* resource ID */,
                                     CodeSense = true,
                                     RequestStockColors = true,
                                     EnableCommenting = true,
                                     EnableAsyncCompletion = true)]
    [ProvideLanguageExtensionAttribute(typeof(LolFxLanguageService),
                                       ".lolfx")]

    [ProvideMenuResource(1000, 1)]
    [Guid("f96f7ac5-16ac-4061-8b92-0a02bb455ae9")]
    [InstalledProductRegistration("#110", "#112", "1.0", IconResourceID = 400)]
    [ComVisible(true)]

    /* Autoload package */
    [ProvideAutoLoad("f1536ef8-92ec-443c-9ed7-fdadf150da82")]

    public sealed class PluginPackage : Package
    {
        public PluginPackage()
        {
            Trace.WriteLine(String.Format(CultureInfo.CurrentUICulture, "Entering constructor for: {0}", this.ToString()));
        }

        [SecurityPermission(SecurityAction.Demand, Flags=SecurityPermissionFlag.UnmanagedCode)]
        protected override void Initialize()
        {
            // Trace the beginning of this method and call the base implementation.
            Trace.WriteLine(String.Format(CultureInfo.CurrentUICulture, "Entering Initialize() of: {0}", this.ToString()));
            base.Initialize();

            Logger.Initialize(GetService(typeof(SVsOutputWindow)) as IVsOutputWindow);

            /* Register the "Generate Compilers" context menu */
            OleMenuCommandService mcs = GetService(typeof(IMenuCommandService)) as OleMenuCommandService;
            if (null != mcs)
            {
                CommandID id = new CommandID(GuidsList.guidVsLolCmdSet,
                                             VsLolIDList.cmdidGenerateCompilers);
                OleMenuCommand command = new MenuGenerateCompilers(new ServiceProvider((IOleServiceProvider)this.GetService(typeof(IOleServiceProvider))), id);
                mcs.AddCommand(command);
            }

            /* Register the LolFx language service */
            IServiceContainer serviceContainer = this as IServiceContainer;
            LolFxLanguageService lolfxserv = new LolFxLanguageService();
            lolfxserv.SetSite(this);
            serviceContainer.AddService(typeof(LolFxLanguageService),
                                        lolfxserv, true);
        }

    }

    internal static class Logger
    {
        public static void Initialize(IVsOutputWindow window)
        {
            m_window = window;

            OpenBuildPane();

            if (m_pane == null)
                Trace.WriteLine("Failed to get a reference to the Output window Build pane");
        }

        private static void OpenBuildPane()
        {
            /* Ensure the "Build" output pane exists */
            if (m_window != null)
            {
                Guid guidBuild = Microsoft.VisualStudio.VSConstants.OutputWindowPaneGuid.BuildOutputPane_guid;
                m_window.CreatePane(guidBuild, "Build", 1, 0);

                if (Microsoft.VisualStudio.ErrorHandler.Failed(m_window.GetPane(ref guidBuild, out m_pane)))
                    m_pane = null;
            }

            if (m_pane != null)
                m_pane.Activate();
        }

        public static void Clear()
        {
            OpenBuildPane();

            if (m_pane == null)
            {
                m_backlog = "";
                return;
            }

            m_pane.Clear();
        }

        public static void Info(string s)
        {
            OpenBuildPane();

            if (m_pane == null)
            {
                m_backlog += s;
                return;
            }

            m_pane.OutputString(m_backlog);
            m_backlog = "";
            m_pane.OutputString(s);
        }

        private static IVsOutputWindow m_window = null;
        private static IVsOutputWindowPane m_pane = null;
        private static string m_backlog = "";
    }

    internal static class GuidsList
    {
        // Now define the list of guids as public static members.
        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public static readonly Guid guidVsLolPkg = new Guid("{f96f7ac5-16ac-4061-8b92-0a02bb455ae9}");

        public static readonly Guid guidVsLolCmdSet = new Guid("{ce508d12-530e-45d0-8b52-1e9ee3f8eaaf}");

        [System.Diagnostics.CodeAnalysis.SuppressMessage("Microsoft.Performance", "CA1823:AvoidUnusedPrivateFields")]
        public static readonly Guid guidGearBmp = new Guid("{560dba06-c26b-4731-8229-b816818e5992}");
    }

    internal static class VsLolIDList
    {
        public const int cmdidGenerateCompilers = 0x2001;
        public const int cmdidUnused1 = 0x2002;
        public const int cmdidUnused2 = 0x2003;
    }
}
