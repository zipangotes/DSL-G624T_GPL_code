
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

function uiDoOnLoad()
{
    document.getElementById("uiPostVarName").name = "logic:command/reboot";
    setTimeout("jslFormSubmit('uiPostForm')", 5000);
}

function uiDoOnImageLoad()
{
   setTimeout("uiDoNextLoc()", 30000);
}

function uiDoNextLoc()
{
   document.location.href="http://"+document.getElementById("uiPostIPAddr").value+"/";
}

</script>
