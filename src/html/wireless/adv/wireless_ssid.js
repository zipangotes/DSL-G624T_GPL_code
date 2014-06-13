
<? if eq 1 1 '<script language="JavaScript">' '' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? include /usr/www_safe/html/adv/wireless_mgmt.js ?>

function uiDoOnLoad()
{
	jslPostToViewCheckBox("uiViewEnableMSSID","uiPostEnableMSSID");
}

function uiDoSave()
{
	jslViewToPostCheckBox("uiPostEnableMSSID","uiViewEnableMSSID");
	jslFormSubmit("uiPostForm");
    return;
}

function uiDoAddSSID()
{

	jslEnable ("uiPostAddSSID");
	jslSetValue("uiPostAddSSID","uiViewAddSSID");

	if (document.getElementById("uiPostAddSSID").value.length == 0)
	{
		alert ("Invalid SSID");
		return;
	}	
	jslDisable("uiPostDelSSID");
	jslFormSubmit("uiPostForm");
	return;	
}

function uiDoDelSSID(arg)
{
	jslDisable("uiPostAddSSID");
	jslEnable ("uiPostDelSSID");
	document.getElementById("uiPostDelSSID").value = arg;
	jslFormSubmit("uiPostForm");
	return;	
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}
</script>
