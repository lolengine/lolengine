function TypeDictionnary(m_name)
{
    this.m_name = m_name;
    this.m_cmds = new Array();
    this.m_vars = new Array();
}

//-----------------------------------------------------------------------------
//Command Var object
//-----------------------------------------------------------------------------
function CmdVarObj() { }
function CmdVar(m_type, m_syntax)
{
    var current_dict = GetCmdDictionnary();
    new_obj = new CmdVarObj();
    new_obj.m_type = m_type;
    new_obj.m_syntax = m_syntax;
    new_obj.ToString = function()
    {
        var res = m_type + ' ';
        if (m_syntax != undefined)
            for (var i = 0; i < m_syntax.length; i++)
                res += m_syntax[i] + ' ';
    }
    current_dict.m_vars[current_dict.m_vars.length] = new_obj;
}

//-----------------------------------------------------------------------------
//Command Argument object
//-----------------------------------------------------------------------------
function CmdArgObj() { }
function CmdArg(m_type, m_name, m_optional)
{
    new_obj = new CmdArgObj();
    new_obj.m_type = m_type;
    new_obj.m_name = m_name;
    new_obj.m_optional = m_optional;
    new_obj.ToString = function()
    {
        if (m_optional != undefined)
            return m_type + ' ' + m_name + ' = ' + m_optional;
        return m_type + ' ' + m_name;
    }
    return new_obj;
}

//-----------------------------------------------------------------------------
//Command Typing object
//-----------------------------------------------------------------------------
function CmdTypeObj() { }
function CmdType(m_name, m_comment, m_arg)
{
    var current_dict = GetCmdDictionnary();
    new_obj = new CmdTypeObj();
    new_obj.m_name = m_name;
    new_obj.m_comment = m_comment;
    new_obj.m_arg = m_arg;
    new_obj.ToString = function()
    {
        var str = m_name.toString() + ' [' + m_comment + ']';
        if (m_arg != undefined)
        {
            str += '{';
            var found_optional = false;
            for (var vi = 0; vi < m_arg.length; vi++)
            {
                if (m_arg[vi].m_optional != undefined && found_optional == false)
                {
                    str += ' [';
                    found_optional = true;
                }
                if (vi != 0)
                    str += ', ';
                str += m_arg[vi].ToString();
            }
            if (found_optional == true)
                str += ']';
            str += '}';
        }
        return str;
    }
    new_obj.CheckCommand = function(check_name)
    {
        if (m_name != undefined && check_name.length > 0)
        {
            for (var i = 0; i < m_name.length; i++)
            {
                if (m_name[i] != undefined)
                {
                    var pattern = new RegExp("^[ ]*" + check_name + "[a-z]*");
                    if (pattern.test(m_name[i]))
                        return true;
                }
            }
        }
        return false;
    }
    current_dict.m_cmds[current_dict.m_cmds.length] = new_obj;
}

//-----------------------------------------------------------------------------
//Tries to find a matching command in the dictionnary based on the current cursor location in the given TextArea.
//-----------------------------------------------------------------------------
function FindMatchingCommand(src_text_area)
{
    if (src_text_area != undefined)
    {
        var current_dict = GetCmdDictionnary();
        var found_match = "";
        var check = true;
        var cursor = Math.min(src_text_area.selectionStart, src_text_area.value.length - 1);
        //Weird test to ensure we don't start searching on the next line or on the next word.
        if (!src_text_area.value[cursor].match(/^[ \n]$/))
            cursor++;
        cursor = Math.min(cursor, src_text_area.value.length - 1);

        var o = 0;
        while (check && o < 10)
        {
            //Move backward to find the first letter on this line
            for (; cursor > 0; cursor--)
                if (src_text_area.value[cursor].match(/^[a-z\n]+$/))
                    break;

            //Move backward to find the start of the command
            for (; cursor > 0; cursor--)
                if (!src_text_area.value[cursor - 1].match(/^[#0-9a-z]+$/))
                    break;

            //If the cursor is on a "#" and the previous is a " ", we're on a color, repeat the operation
            if (cursor > 0 && src_text_area.value[cursor - 1].match(/[ ]/) &&
                              src_text_area.value[cursor].match(/[#]/))
                check = true;
            else
                check = false;
            o++;
        }

        //Move forward to find the end of the word
        for (; cursor < src_text_area.value.length; cursor++)
        {
            if (src_text_area.value[cursor].match(/^[a-z]+$/))
                found_match += src_text_area.value[cursor];
            else
                break;
        }

        //Try to match the command with the dictionnary
        var match_data = new Object();
        match_data.match = found_match;
        match_data.match_list = new Array();
        for (cursor = 0; cursor < current_dict.m_cmds.length; cursor++)
            if (current_dict.m_cmds[cursor].CheckCommand(found_match))
                match_data.match_list[match_data.match_list.length] = cursor;

        return match_data;
    }
}

//-----------------------------------------------------------------------------
//Build a TOC from all commands first letter
//-----------------------------------------------------------------------------
function BuildTOC(with_dot)
{
    var res = '';
    var current_dict = GetCmdDictionnary();
    for (var a = 'a'.charCodeAt(0); a <= 'z'.charCodeAt(0); a++)
    {
        var stop = false;
        for (var i = 0; !stop && i < current_dict.m_cmds.length; i++)
        {
            for (var j = 0; j < current_dict.m_cmds[i].m_name.length; j++)
            {
                if (current_dict.m_cmds[i].m_name[j][0] == String.fromCharCode(a))
                {
                    res += String.fromCharCode(a);
                    stop = true;
                    break;
                }
            }
        }
        if (!stop && with_dot)
            res += '.';
    }
    return res;
}

//-----------------------------------------------------------------------------
//Setup code lookup logic
//-----------------------------------------------------------------------------
function CmdLookup(div_cmds, div_args, div_cmnt, div_vars, text_src)
{
    var cur_dict = GetCmdDictionnary();
    if (text_src != undefined)
    {
        var cmd_size = 8;
        var found = FindMatchingCommand(text_src);
        if (found.match_list.length > 0)
        {
            var perfect_match = false;
            var best_match = 0;
            var best_length = 300000;
            //Find the best match to put it in first in the list
            for (var i = 0; i < found.match_list.length && best_length > 0; i++)
            {
                var cur_match = cur_dict.m_cmds[found.match_list[i]];
                for (var j = 0; j < cur_match.m_name.length && best_length > 0; j++)
                {
                    if (cur_match.m_name[j].length == found.match.length)
                    {
                        perfect_match = true;
                        best_match = i;
                        best_length = 0;
                    }
                    else if (cur_match.m_name[j].length < best_length &&
                             cur_match.m_name[j].length > found.match.length)
                    {
                        best_match = i;
                        best_length = cur_match.m_name[j].length;
                    }
                }
            }
            var tmp = found.match_list[0];
            found.match_list[0] = found.match_list[best_match];
            found.match_list[best_match] = tmp;

            var type_list = new Array();
            div_cmds[0].innerHTML = "";
            div_cmds[1].innerHTML = "";
            div_args.innerHTML = "";
            div_cmnt.innerHTML = "";
            //Go through the found matches and show them.
            for (var i = 0; i < found.match_list.length; i++)
            {
                var cur_match = cur_dict.m_cmds[found.match_list[i]];
                div_cmds[0].innerHTML += '[';
                var max = cur_match.m_name.length;
                var word = 0;
                for (var j = 0; j < max; j++)
                {
                    var mth = found.match;
                    var cmd = cur_match.m_name[j];
                    //Matching start caracters should be bold
                    if (mth == cmd.slice(0, mth.length))
                    {
                        div_cmds[word].innerHTML += '&nbsp;';
                        div_cmds[word].innerHTML += '<b>' + mth + '</b>';
                        div_cmds[word].innerHTML += cmd.slice(mth.length, cmd.length);
                        word++;
                    }
                }
                //Complete empty command by <br> so commands in the two columns are on the same line
                word = (word > 0)?(2):(0);
                while (word-- > 0)
                    div_cmds[word].innerHTML += "<br>";
                //Go through the arguments and show them, force if we found the perfect match
                if (perfect_match || (found.match_list.length < 4 && i == 0))
                {
                    div_args.innerHTML += "&nbsp;>&nbsp;";
                    if (cur_match.m_arg != undefined)
                    {
                        max = cur_match.m_arg.length;
                        var found_optional = false;
                        for (var j = 0; j < max; j++)
                        {
                            if (cur_match.m_arg[j].m_optional != undefined && found_optional == false)
                            {
                                var tab = '<br>'; for (var l = 0; l < 5; l++) tab += '&nbsp;';
                                div_args.innerHTML += tab + 'Opt: [';
                                found_optional = true;
                            }
                            else if (j > 0)
                                div_args.innerHTML += ', ';

                            //Types are bold
                            div_args.innerHTML += '<b>' + cur_match.m_arg[j].m_type + '</b> ';
                            type_list[type_list.length] = cur_match.m_arg[j].m_type;
                            //Names are not
                            div_args.innerHTML += cur_match.m_arg[j].m_name;
                            if (cur_match.m_arg[j].m_optional != undefined)
                                div_args.innerHTML += ' = <b>' + cur_match.m_arg[j].m_optional + '</b>';
                        }
                        if (found_optional == true)
                            div_args.innerHTML += '] ';
                        div_args.innerHTML += '&nbsp;<br>';
                    }
                    //Add the comment
                    if (cur_match.m_comment != undefined)
                    {
                        var tb_i = 16; var in_i = 8;
                        var tab = '<br>';
                        if (cur_match.m_arg == undefined) { tb_i -= 8; in_i -= 8; }
                        for (var l = 0; l < in_i; l++) div_args.innerHTML += '&nbsp;';
                        for (var l = 0; l < tb_i; l++) tab += '&nbsp;';
                        div_args.innerHTML += cur_match.m_comment + '&nbsp;';
                        while (div_args.innerHTML.indexOf('\n') > -1)
                            div_args.innerHTML = div_args.innerHTML.replace('\n', tab);
                    }
                }
            }
            //Go through the type list and bold the used ones.
            if (cur_dict.m_vars != undefined)
            {
                div_vars.innerHTML = "";
                var max = cur_dict.m_vars.length;
                for (var j = 0; j < max; j++)
                {
                    var cur_var = cur_dict.m_vars[j];
                    div_vars.innerHTML += "&nbsp;>&nbsp;";
                    var k = 0;
                    for (; k < type_list.length; k++)
                        if (cur_var.m_type == type_list[k])
                            break;

                    //Bold the used variables
                    if (k < type_list.length) 
                        div_vars.innerHTML += "<b>" + cur_var.m_type + "</b>";
                    else
                        div_vars.innerHTML += cur_var.m_type;
                        
                    if (cur_var.m_syntax != undefined)
                    {
                        var align_size = 9;
                        var cmd_size = cur_var.m_type.length + 3;
                        for (var m = 0; m < cur_var.m_syntax.length; m++)
                        {
                            for (var l = 0; l < align_size - cmd_size; l++)
                                div_vars.innerHTML += "&nbsp;";
                            div_vars.innerHTML += cur_var.m_syntax[m] + "<br>";
                            cmd_size = 0;
                        }
                        var tab = '';
                        for (var l = 0; l < align_size - cmd_size; l++)
                            tab += "&nbsp;";
                        while (div_vars.innerHTML.indexOf('\n') > -1)
                            div_vars.innerHTML = div_vars.innerHTML.replace('\n', '<br>' + tab);
                    }
                }
            }
        }
        else
        {
            div_cmds[0].innerHTML = "[ ...&nbsp;";
            div_cmds[1].innerHTML = "";
            div_args.innerHTML = "";
            div_cmnt.innerHTML = "";
        }
    }
}