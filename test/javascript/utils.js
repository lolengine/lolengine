//-----------------------------------------------------------------------------
// COOKIE MANAGEMENT - nom nom nom nom -
//-----------------------------------------------------------------------------

function LolCookie() { }
function LolCookieDays(m_name, m_value, m_expire_days)
{
    var cookie = new LolCookie();

    cookie.m_name = m_name;
    cookie.m_value = m_value;
    cookie.m_expire_date = new Date();
    cookie.m_expire_date.setDate(cookie.m_expire_date.getDate() + m_expire_days);

    return cookie;
}
function LolCookieDate(m_name, m_value, m_expire_date)
{
    var cookie = new LolCookie();

    cookie.m_name = m_name;
    cookie.m_value = m_value;
    cookie.m_expire_date = m_expire_date;

    return cookie;
}

//Set a cookie
function StoreLolCookie(cookie)
{
    //Get the cookie and removes it if it exists.
    GetLolCookie(cookie.m_name, true);

    var enc_value = escape(cookie.m_value) + ";";

    //Add expire days
    if (cookie.m_expire_date)
        enc_value += " expires=" + cookie.m_expire_date.toUTCString() + ";";
    enc_value = cookie.m_name + "=" + enc_value;

    //Store cookies
    document.cookie = enc_value;
}

//Get a cookie
function GetLolCookie(name, remove)
{
    var cki_doc = document.cookie;
    var cookie = null;

    var cki_start = cki_doc.indexOf(name + "=");
    if (cki_start > -1)
    {
        cookie = new LolCookie();
        cookie.m_name = name;
        cookie.m_expire_date = null;

        //Retrieve value
        var val_start = cki_doc.indexOf("=", cki_start) + 1;
        var val_end = cki_doc.indexOf(";", cki_start);
        cookie.m_value = unescape(cki_doc.substring(val_start, val_end));

        if (remove)
            document.cookie = name + "=; expires=0;";
    }
    return cookie;
}

//Check the existence of a cookie
function DoesLolCookieExist(name)
{
    var cki_doc = document.cookie;
    if (cki_doc.indexOf(name + "=") > -1)
        return true;
    return false;
}

//-----------------------------------------------------------------------------
// FILE MANAGEMENT
//-----------------------------------------------------------------------------

//Dynamic load
function DynLoadFile(filename)
{
    var filetype = filename.match(/\.[a-zA-Z]+$/);
    //if filename is a external JavaScript file
    if (filetype == ".js")
    {
        var fileref = document.createElement('script');
        fileref.setAttribute("type","text/javascript");
        fileref.setAttribute("src", filename);
    }
    //if filename is an external CSS file
    else if (filetype == ".css")
    {
        var fileref = document.createElement("link");
        fileref.setAttribute("rel", "stylesheet");
        fileref.setAttribute("type", "text/css");
        fileref.setAttribute("href", filename);
    }
    if (fileref)
        document.getElementsByTagName("head")[0].appendChild(fileref);
}

//Dynamic remove
function DynRemoveFile(filename)
{
    var filetype = filename.match(/\.[a-zA-Z]+$/);
    var targetelement = "none";
    var targetattr = "none";
    //Determine element type to create nodelist from and corresponding attribute to test for
    if (filetype==".js")
    {
        targetelement = "script";
        targetattr = "src";
    }
    else if (filetype == ".css")
    {
        targetelement = "link";
        targetattr = "href";
    }

    var allsuspects = document.getElementsByTagName(targetelement);
    //Search backwards within nodelist for matching elements to remove
    for (var i = allsuspects.length; i >= 0; i--)
        if (allsuspects[i] &&
            allsuspects[i].getAttribute(targetattr) &&
            allsuspects[i].getAttribute(targetattr).indexOf(filename) != -1)
            allsuspects[i].parentNode.removeChild(allsuspects[i]); //remove element by calling parentNode.removeChild()
}
