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
