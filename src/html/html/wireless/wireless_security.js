<? include /usr/www_safe/html/js/blank.js ?>

<? if eq '' '$var:currentSSID' '<? setvariable var:currentSSID '<? query ap:status/current_ssid ?>' ?>' ' ' ?>

function uiDoGetSecPage(new_ssid)
{
  var i =0;
  currentSSID = new_ssid;

  do {
    i++;
  } while (i < 1500000)

  switch ( sec_profile[ new_ssid ] ){
  case 0:
    uiDoNone(new_ssid);
    break;
  case 1:
    uiDoWPAPage(new_ssid);
    break;
  case 2:
    uiDoWepPage(new_ssid);
    break;
  case 4:
    uiDo8021xPage(new_ssid);
    break;
  case 8:
    uiDoWPAPage(new_ssid);
    break;
  default:
    break;
  }
  
}

function uiDoNone(arg)
{
	document.getElementById("uiPostForm").method="GET";
	document.getElementById("uiPostPageName").value="security_none";
	document.getElementById("uiPostForm").submit();
}

function uiDoWepPage(arg)
{
	document.getElementById("uiPostForm").method="GET";
	document.getElementById("uiPostPageName").value="security_wep";
	document.getElementById("uiPostForm").submit();
}

function uiDo8021xPage(arg)
{
	document.getElementById("uiPostForm").method="GET";
	document.getElementById("uiPostPageName").value="security_8021x";
	document.getElementById("uiPostForm").submit();
}

function uiDoWPAPage(arg)
{
	document.getElementById("uiPostForm").method="GET";
	document.getElementById("uiPostPageName").value="security_wpa";
	document.getElementById("uiPostForm").submit();
}

