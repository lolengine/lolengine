//-------------------------------------------------------------------------
// MODULE INIT FUNCTIONS
//-------------------------------------------------------------------------
function RegisterListener()
{
    //Register all the correct functions to the listener
    var div_listener = GetiFrameDivEmbed();
    if (div_listener)
    {
        div_listener.addEventListener('loadstart', NaClModuleStartedLoad, true);
        div_listener.addEventListener('load', NaClModuleDidLoad, true);
        div_listener.addEventListener('error', NaClModuleError, true);
        div_listener.addEventListener('progress', NaClModuleLoadUpdate, true);
        div_listener.addEventListener('message', NaClModuleSentMessage, true);
        div_listener.addEventListener('crash', NaClModuleCrash, true);
        window.setTimeout("UpdateTextStatus(0.1)", 100);
    }
    else if (IsUsingNaCl())
        window.setTimeout("RegisterListener()", 200);
}

function NaClInitModuleVar()
{
    InitModuleVar();
    if (g_embed_module)
        g_embed_module['SendMessage'] = function(message) { g_embed_module.postMessage(message); }
}

//-------------------------------------------------------------------------
// MODULE LOADING FUNCTIONS
//-------------------------------------------------------------------------
//Indicate page has been loaded.
function NaClLoadingInit()
{
    HideProgressStatus(false);
    //Page did load before NaCl module
    if (!g_embed_module)
        AddTextStatus('Please wait for module loading');
    RegisterListener();
}

//Module starts load
function NaClModuleStartedLoad()
{
    AddTextStatus('Module Started Loading');
    NaClInitModuleVar();
}

//Module progress event
function NaClModuleLoadUpdate(event)
{
    if (event.lengthComputable)
        UpdateProgressBarValue('Please wait, loading', event.loaded, event.total);
    else
        UpdateProgressBarValue('Please wait, calculating load balance ...');
}

//Indicate module load success.
function NaClModuleDidLoad()
{
    if (!g_embed_module)
        NaClInitModuleVar();

    //Hide the progress div
    AddTextStatus('Module is live, thank you for your patience.');
    window.setTimeout('HideProgressStatus(true)', GetMaxStatusTime(0.5) * 1000);

    ModuleIsLive();
}

//Module did crash
function NaClModuleCrash(event)
{
    NaClRestartModule();
    AddTextStatus("Module has crashed ! Restarting ...");
}

//Module had an error
function NaClModuleError(event)
{
    NaClRestartModule();
    AddTextStatus("Module Load/start Error ! Restarting ...");
}

//Used to restart module on crash/error/load fail ....
function NaClRestartModule()
{
    var id_frame_embed = GetFrameData();
    if (id_frame_embed)
    {
        HideProgressStatus(false);
        window.setTimeout('GetFrameData().contentDocument.location.reload(true)', 1000);
    }
}

//-------------------------------------------------------------------------
// MODULE COMMUNICATION FUNCTIONS
//-------------------------------------------------------------------------

//Handle message from the NaCl module
function NaClModuleSentMessage(message)
{
    ModuleSentMessage(message);
}

