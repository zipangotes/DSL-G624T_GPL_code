

<? if eq 1 1 '<script language="JavaScript">' '' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '' '<script language="JavaScript">' ?>

<? include /usr/www_safe/html/js/blank.js ?>
function uiDoEthPage()
{
  document.getElementById("uiPostForm").method="GET";
  //document.getElementById("uiPostPageName").value="ethernet";
  document.getElementById("uiPostInterface").value="eth";
  document.getElementById("uiPostForm").submit();
}
function uiDoUSBPage()
{
  document.getElementById("uiPostForm").method="GET";
  //document.getElementById("uiPostPageName").value="usb";
  document.getElementById("uiPostInterface").value="usb";
  document.getElementById("uiPostForm").submit();
}
function uiDoDSLPage()
{
  document.getElementById("uiPostForm").method="GET";
  //document.getElementById("uiPostPageName").value="dsl";
  document.getElementById("uiPostInterface").value="dsl";
  document.getElementById("uiPostForm").submit();
}
function uiDoWANPage()
{
  document.getElementById("uiPostForm").method="GET";
  //document.getElementById("uiPostPageName").value="wan";
  document.getElementById("uiPostInterface").value="wan";
  document.getElementById("uiPostForm").submit();
}

function uiDoWirelessPage()
{
  document.getElementById("uiPostForm").method="GET";
  //document.getElementById("uiPostPageName").value="wireless";
  document.getElementById("uiPostInterface").value="wireless";
  document.getElementById("uiPostForm").submit();
}
function uiDoRefresh()
{
  document.getElementById("uiPostForm").method="GET";
  document.getElementById("uiPostForm").submit();
}
</script>
