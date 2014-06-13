<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/val.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/val.js"></script>' ?>
<? if eq 1 1 '' '<script language="JavaScript">' ?>

<? include /usr/www_safe/html/adv/wireless_security.js ?>

function uiDoValidate()
{
	var port = parseInt(document.getElementById("uiPostPort").value);
	var message;

	message = valDoValidateIp(document.getElementById("uiPostServerIp").value);
	
	if(message!=null) 
	{
		alert ("Invalid IP Address");	
		return false;
	}

	if ((port < 0) || (port > 65535) || isNaN(port))
	{
		alert ("Port out of range");
		return false;
	}

	
	if(document.getElementById("uiPostSecret").value.length == 0)
	{
		jslDisable("uiPostSecret");
	}
	
	if ((document.getElementById("uiPostGroupInterval").value.length == 0) || 
		 parseInt(document.getElementById("uiPostGroupInterval").value) <= 0)
	{
		alert ("Invalid Group Interval");	
		return false;
	}
	
	return true;
}

function uiDoOnLoad()
{
	jslSetValue("uiViewServerIp","uiPostServerIp");
	jslSetValue("uiViewPort","uiPostPort");
	jslSetValue("uiViewSecret","uiPostSecret");
	jslSetValue("uiViewGroupInterval","uiPostGroupInterval");
}

function uiDoSave()
{
	document.getElementById("uiPostPrivacyType").value = 2;
	jslSetValue("uiPostServerIp", "uiViewServerIp");
	jslSetValue("uiPostPort", "uiViewPort");
	jslSetValue("uiPostSecret", "uiViewSecret");
	jslSetValue("uiPostGroupInterval", "uiViewGroupInterval");

	if(uiDoValidate()==true) jslFormSubmit("uiPostForm");
	return;
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}
</script>
