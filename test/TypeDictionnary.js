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
    var current_mesh_dict = GetCmdDictionnary();
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
    current_mesh_dict.m_vars[current_mesh_dict.m_vars.length] = new_obj;
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
    var current_mesh_dict = GetCmdDictionnary();
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
    current_mesh_dict.m_cmds[current_mesh_dict.m_cmds.length] = new_obj;
}

//-----------------------------------------------------------------------------
//Tries to find a matching command in the dictionnary based on the current cursor location in the given TextArea.
//-----------------------------------------------------------------------------
function FindMatchingCommand(src_text_area)
{
    if (src_text_area != undefined)
    {
        var current_mesh_dict = GetCmdDictionnary();
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
        for (cursor = 0; cursor < current_mesh_dict.m_cmds.length; cursor++)
            if (current_mesh_dict.m_cmds[cursor].CheckCommand(found_match))
                match_data.match_list[match_data.match_list.length] = cursor;

        return match_data;
    }
}
