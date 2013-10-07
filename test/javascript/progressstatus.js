g_var_progress_bar      = -1;
g_status_text           = [null];
g_status_timer          = [-1.0];

//-------------------------------------------------------------------------
// PROGRESS BAR STATUS FUNCTIONS
//-------------------------------------------------------------------------

//Update the actual progress bar
function UpdateProgressBarValue(new_status, new_value, new_max)
{
    var progress_bar = GetProgressBar();
    if (progress_bar)
    {
        if (new_value != undefined && new_max != undefined)
        {
            if (progress_bar.value < 0)
                g_var_progress_bar = -1;
            var loading_text = new_status + '[' + (Math.min(1.0, new_value / new_max) * 100.0).toFixed(0) + '%]';
            //update the progress status
            progress_bar.max = new_max;
            progress_bar.value = new_value;
            g_var_progress_bar = AddTextStatus(loading_text, g_var_progress_bar);
        }
        //Load length is not usable
        else
        {
            if (progress_bar.value > 0)
                g_var_progress_bar = -1;
            progress_bar.value = -1;
            g_var_progress_bar = AddTextStatus(new_status, g_var_progress_bar);
        }
        //Force Status text update for coherency
        UpdateTextStatus(0.0);
    }
}

//Add text status to the stack
function GetMaxStatusTime(min_value)
{
    var higher_time = (min_value) ? (min_value) : (0.0);
    for (var i = 0; i < g_status_timer.length; i++)
        if (g_status_timer[i])
            higher_time = Math.max(higher_time, g_status_timer[i]);
}

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
        g_status_text[g_status_text.length] = message;
        g_status_timer[g_status_timer.length] = GetMaxStatusTime(msg_timer) + 0.5;
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
            div_progress.style.zIndex = "-999";
            g_status_text.splice(0, g_status_text.length);
            g_status_timer.splice(0, g_status_timer.length);
            progress_bar.value = -1;
            UpdateTextStatus(1.0);
        }
        else
        {
            div_progress.style.visibility = "visible";
            div_progress.style.zIndex = "999";
            progress_bar.value = -1;
            AddTextStatus('Please wait for module loading');
            UpdateTextStatus(1.0);
        }
    }
}

