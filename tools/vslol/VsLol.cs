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

    [ProvideMenuResource(1000, 1)]
    [Guid("f96f7ac5-16ac-4061-8b92-0a02bb455ae9")]
    [InstalledProductRegistration("#110", "#112", "1.0", IconResourceID = 400)]
    [ComVisible(true)]
    [ProvideAutoLoad("f1536ef8-92ec-443c-9ed7-fdadf150da82")] // This is needed for the package to be autoloaded
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

            // Ensure the "Build" output pane exists
            IVsOutputWindow outputWindow = GetService(typeof(SVsOutputWindow)) as IVsOutputWindow;
            if (null != outputWindow)
            {
                Guid guidBuild = Microsoft.VisualStudio.VSConstants.OutputWindowPaneGuid.BuildOutputPane_guid;
                outputWindow.CreatePane(guidBuild, "Build", 1, 0);
            }

            // Register the "Generate Compilers" context menu
            OleMenuCommandService mcs = GetService(typeof(IMenuCommandService)) as OleMenuCommandService;
            if (null != mcs)
            {
                CommandID id = new CommandID(GuidsList.guidVsLolCmdSet, VsLolIDList.cmdidGenerateCompilers);
                OleMenuCommand command = new MenuGenerateCompilers(new ServiceProvider((IOleServiceProvider)this.GetService(typeof(IOleServiceProvider))), id);
                mcs.AddCommand(command);
            }
        }
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

