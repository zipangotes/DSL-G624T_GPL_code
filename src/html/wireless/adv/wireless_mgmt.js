<? include /usr/www_safe/html/js/blank.js ?>
function uiDoStaPage()
{
  document.getElementById("uiPostForm").method="GET";
  document.getElementById("uiPostPageName").value="wireless_sta";
  document.getElementById("uiPostForm").submit();
}

function uiDoAccessPage()
{
  document.getElementById("uiPostForm").method="GET";
  document.getElementById("uiPostPageName").value="wireless_access";
  document.getElementById("uiPostForm").submit();
}

function uiDoSSIDPage()
{
  document.getElementById("uiPostForm").method="GET";
  document.getElementById("uiPostPageName").value="wireless_ssid";
  document.getElementById("uiPostForm").submit();
}

function uiDoRefresh()
{
  document.getElementById("uiPostForm").method="GET";
  document.getElementById("uiPostForm").submit();
}

