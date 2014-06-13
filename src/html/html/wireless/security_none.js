<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

<? include /usr/www_safe/html/wireless/wireless_security.js ?>

var sec_profile = new Array();

function uiDoValidate()
{
	return true;
}

function uiDoOnLoad()
{
  var i = 0;

  <? multiquery ap:status/mssid_sec_profile `sec_profile[i++]=$01;` ?>
	return;
}

function uiDoSave()
{
	document.getElementById("uiPostPrivacyType").value = 0;
  jslEnable("uiPostPrivacyType");
  jslDisable("uiPostChangeCurrentSSID");
	if(uiDoValidate()==true) jslFormSubmit("uiPostForm");
	return;
}

function uiChangeCurrentSSID(new_id)
{
  currentSSID = new_id;
  document.getElementById("uiPostChangeCurrentSSID").value=new_id;
  jslDisable("uiPostPrivacyType");
  jslFormSubmit("uiPostForm");
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}

</script>
