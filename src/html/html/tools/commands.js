
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

function uiDoOnLoad()
{
   return true;
}

function uiRestart()
{
  if(confirm("RESTART?  If you have not done a \"Save All Changes\" command, you will lose any changes you have made."))
  {
    document.getElementById("uiPostVarName").name = "logic:command/reboot";
    jslFormSubmit("uiPostForm");
    setTimeout("uiDoNextLoc()", 2000);
  }
}

function uiRestartAP()
{
  if(confirm("RESTART AP?  This will restart AP with the new configuration."))
  {
    document.getElementById("uiPostVarName").name = "ap:status/cli_reboot";
    document.getElementById("uiPostForm").submit();
  }
}

function uiDoNextLoc()
{
    document.location.href="http://"+document.getElementById("uiPostIPAddr").value+"/cgi-bin/webcm?getpage=/usr/www_safe/html/defs/<? echo $var:style ?>/menus/<? echo $var:main ?>.html&var:style=<? echo $var:style ?>&var:main=<? echo $var:main ?>&var:menu=tools&var:menutitle=Tools&var:pagename=restart&var:pagetitle=Restart";
}

function uiRestoreFactoryDefaults()
{
  if(confirm("Are you sure you want to restore factory defaults?") )
  {
    document.getElementById("uiPostVarName").name  = "logic:command/defaults";
    document.getElementById("uiPostVarName").value = "../gateway/commands/saveconfig.html";
    document.getElementById("uiPostForm").submit();
  }
}    
function uiSaveAll()
{
   document.getElementById("uiPostVarName").name  = "logic:command/save";
   document.getElementById("uiPostVarName").value = "../gateway/commands/saveconfig.html";
   document.getElementById("uiPostForm").submit();
}
</script>
