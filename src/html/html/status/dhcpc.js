
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

var message;
var result=new Array();
function uiDoOnLoad()
{
  jslDoShowComboBox("uiViewLanGrp","uiPostLanGrp");
  return true;
}

function uiDoSelectLAN(value)
{
	document.getElementById("uiPostLanGrp").value=value;
	document.getElementById("uiPostForm").method="GET";
       document.getElementById("uiPostPageName").value="dhcpc";
  document.getElementById("uiPostForm").submit();
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}
function uiDoRefresh()
{
  document.getElementById("uiPostForm").method="GET";
  document.getElementById("uiPostForm").submit();
}
</script>

