if (IsUsingNaCl()) NaClLoadingInit();

function InitNaClModuleVar() { g_embed_module = GetiFrameModuleVar(); }

function RegisterListener()
{
    //Register all the correct functions to the listener
    var div_listener = GetiFrameDivEmbed();
    if (div_listener)
    {
        div_listener.addEventListener('loadstart', ModuleStartedLoad, true);
        div_listener.addEventListener('load', ModuleDidLoad, true);
        div_listener.addEventListener('error', ModuleError, true);
        div_listener.addEventListener('progress', ModuleLoadUpdate, true);
        div_listener.addEventListener('message', ModuleSentMessage, true);
        div_listener.addEventListener('crash', ModuleCrash, true);
        window.setTimeout("UpdateTextStatus(0.1)", 100);
    }
    else if (IsUsingNaCl())
        window.setTimeout("RegisterListener()", 200);
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
function ModuleStartedLoad()
{
    AddTextStatus('Module Started Loading');
    InitNaClModuleVar();
}

//Module progress event
function ModuleLoadUpdate(event)
{
    if (event.lengthComputable)
        UpdateProgressBarValue('Please wait, loading', event.loaded, event.total);
    else
        UpdateProgressBarValue('Please wait, calculating load balance ...');
}

//Indicate module load success.
function ModuleDidLoad()
{
    if (!g_embed_module)
        InitNaClModuleVar();

    //Hide the progress div
    AddTextStatus('Module is live, thank you for your patience.');
    window.setTimeout('HideProgressStatus(true)', GetMaxStatusTime(0.5) * 1000);
}

//Module did crash
function ModuleCrash(event)
{
    RestartModule();
    AddTextStatus("Module has crashed ! Restarting ...");
}

//Module had an error
function ModuleError(event)
{
    RestartModule();
    AddTextStatus("Module Load/start Error ! Restarting ...");
}

//Used to restart module on crash/error/load fail ....
function RestartModule()
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
function ModuleSentMessage(message)
{
    alert('Module sent message: ' + message.data);
}

//Called by the "Send Mesh Command !" button
function SendMessageToModule()
{
    if (g_embed_module)
        g_embed_module.postMessage(GetTextAreaCodeSrc().value);
    else
        alert("Module not loaded !");
}
