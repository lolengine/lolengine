g_status_text           = [null];
g_status_timer          = [-1.0];
g_var_progress_bar      = -1;

//window.setTimeout("RegisterListener()", 200);

function RegisterListener()
{
    //Register all the correct functions to the listener
    var div_listener = GetNaClListenerDiv();
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
function PageDidLoad()
{
    //Page did load before NaCl module
    if (!GetNaClModuleVar())
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
    var progress_bar = GetProgressBar();
    if (progress_bar)
    {
        if (event.lengthComputable)
        {
            if (progress_bar.value < 0)
                g_var_progress_bar = -1;
            var loading_text = 'Please wait, loading [' + (Math.min(1.0, event.loaded / event.total) * 100.0).toFixed(0) + '%]';
            //update the progress status.
            progress_bar.max = event.total;
            progress_bar.value = event.loaded;
            g_var_progress_bar = AddTextStatus(loading_text, g_var_progress_bar);
        }
        //Load length is not usable.
        else
        {
            progress_bar.value = -1;
            g_var_progress_bar = AddTextStatus('Please wait, calculating load balance ...', g_var_progress_bar);
        }
    }
}

//Indicate module load success.
function ModuleDidLoad()
{
    if (!GetNaClModuleVar())
        InitNaClModuleVar();

    //Hide the progress div
    AddTextStatus('Module is live, thank you for your patience.');
    var hide_timer = 0.1;
    for (var i = 0; i < g_status_timer.length; i++)
        if (g_status_timer[i])
            hide_timer = Math.max(g_status_timer[i], hide_timer);
    window.setTimeout('HideProgressStatus(true)', hide_timer * 1000);
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
/*
    var div_embed_data = GetDivEmbedData();
    var div_embed_data_save = GetDivEmbedDataSave();

    if (div_embed_data)
    {
        HideProgressStatus(false);

        div_embed_data_save = div_embed_data.innerHTML;
        div_embed_data.innerHTML = '';
    }
*/
}

//Called after the Restart to effectively do it
function RestartModuleRestoreEmbed()
{
/*
    var div_embed_data = GetDivEmbedData();
    var div_embed_data_save = GetDivEmbedDataSave();

    if (div_embed_data)
    {
        div_embed_data.innerHTML = div_embed_data_save;
        div_embed_data_save = '';
        HideProgressStatus(false);
    }
*/
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
    if (GetNaClModuleVar())
        GetNaClModuleVar().postMessage(GetTextAreaCodeSrc().value);
    else
        alert("Module not loaded !");
}

//-------------------------------------------------------------------------
// PROGRESS BAR STATUS FUNCTIONS
//-------------------------------------------------------------------------

//Update progress bar status
function UpdateTextStatus(seconds)
{
    var div_progress_status = GetDivProgressStatus();
    if (div_progress_status)
    {
        div_progress_status.innerHTML = '';
        for (var i = 0; i < g_status_text.length; i++)
        {
            if (g_status_timer[i] == undefined || g_status_timer[i] < 0.0)
            {
                g_status_timer[i] = -1.0;
                g_status_text[i] = undefined;
            }
            else
            {
                g_status_timer[i] -= seconds;
                div_progress_status.innerHTML += g_status_text[i];
                //div_progress_status.innerHTML += '[' + g_status_timer[i].toFixed(2) + ']';
                if (i < g_status_text.length - 1)
                    div_progress_status.innerHTML += '<br>';
            }
        }
        window.setTimeout("UpdateTextStatus(0.1)", 100);
    }
}

//Add text status to the stack
function AddTextStatus(message, message_id)
{
    var msg_timer = 2.0;
    if (message_id == undefined || message_id < 0)
    {
        for (var i = g_status_timer.length - 1; i >= 0; i--)
            if (g_status_timer[i])
                msg_timer = Math.max(msg_timer, g_status_timer[i] + 0.5);

        g_status_text[g_status_text.length] = message;
        g_status_timer[g_status_timer.length] = msg_timer;
        return g_status_timer.length - 1;
    }
    else
    {
        g_status_text[message_id] = message;
        g_status_timer[message_id] = msg_timer;
        return message_id;
    }
}

//Hide Progress Status and empty status texts
function HideProgressStatus(should_hide)
{
    var progress_bar = GetProgressBar();
    var div_progress = GetDivProgress();
    if (div_progress && progress_bar)
    {
        if (should_hide == true)
        {
            div_progress.style.visibility = "hidden";
            g_status_text.splice(0, g_status_text.length);
            g_status_timer.splice(0, g_status_timer.length);
            progress_bar.value = -1;
            UpdateTextStatus(1.0);
        }
        else
        {
            div_progress.style.visibility = "visible";
            progress_bar.value = -1;
            AddTextStatus('Please wait for module loading');
            UpdateTextStatus(1.0);
        }
    }
}
