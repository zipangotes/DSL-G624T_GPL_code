
<? include /usr/www_safe/html/js/blank.js ?>
function uiDoEthPage()
{
  document.getElementById("uiPostForm").method="GET";
  document.getElementById("uiPostPageName").value="ethernet";
  document.getElementById("uiPostForm").submit();
}

function uiDoUSBPage()
{
  document.getElementById("uiPostForm").method="GET";
  document.getElementById("uiPostPageName").value="usb";
  document.getElementById("uiPostForm").submit();
}

function uiDoDSLPage()
{
  document.getElementById("uiPostForm").method="GET";
  document.getElementById("uiPostPageName").value="dsl";
  document.getElementById("uiPostForm").submit();
}

function uiDoWirelessPage()
{
  document.getElementById("uiPostForm").method="GET";
  document.getElementById("uiPostPageName").value="wireless";
  document.getElementById("uiPostForm").submit();
}

function uiDoRefresh()
{
  document.getElementById("uiPostForm").method="GET";
  document.getElementById("uiPostForm").submit();
}




