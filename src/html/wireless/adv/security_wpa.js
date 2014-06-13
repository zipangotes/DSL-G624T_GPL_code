<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/val.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/val.js"></script>' ?>
<? if eq 1 1 '' '<script language="JavaScript">' ?>

<? include /usr/www_safe/html/adv/wireless_security.js ?>

function uiDoValidate(type)
{
	var message;

	if (type == 0)
	{
		var port = parseInt(document.getElementById("uiPostPort").value);

		message = valDoValidateIp(document.getElementById("uiPostServerIp").value);
	
		if(message!=null) 
		{
			alert ("Invalid IP Address");	
			return false;
		}

		if ((port < 0) || (port > 65535) || isNaN(port))
		{
			alert ("Port out of range.");
			return false;
		}

		if(document.getElementById("uiPostSecret").value.length == 0)
		{
			jslDisable("uiPostSecret");
		}
	}
	else if (type == 1)
	{
	/* TODO This shd probably be only digits*/
		if ((document.getElementById("uiPostPskValue").value.length == 0) || 
			(document.getElementById("uiPostPskValue").value.match("[^0-9a-fA-F]") != null))
		{
			alert ("Invalid PSK Hex Value");	
			return false;
		}
	}
	else if (type == 2)
	{
		if(document.getElementById("uiPostPskValue").value.length == 0)
		{
			alert ("Invalid PSK String");	
			return false;
		}
	}
	
	if ((document.getElementById("uiPostGroupInterval").value.length == 0) || 
		 parseInt(document.getElementById("uiPostGroupInterval").value) < 0)
	{
		alert ("Invalid Group Interval");	
		return false;
	}

	return true;
}

function uiDoOnLoad()
{
	jslSetValue("uiViewGroupInterval","uiPostGroupInterval");
	jslSetValue("uiViewServerIp","uiPostServerIp");
	jslSetValue("uiViewPort","uiPostPort");
	jslSetValue("uiViewSecret","uiPostSecret");

	var value = parseInt(document.getElementById("uiPostPskType").value);
	uiSelectPskType(value);
}

function uiDoSave()
{
	var type = document.getElementById("uiPostPskType").value;

	document.getElementById("uiPostPrivacyType").value = 3;
	jslSetValue("uiPostGroupInterval", "uiViewGroupInterval");

	if (type == 0)
	{
		jslEnable("uiPostServerIp");
		jslEnable("uiPostPort");
		jslEnable("uiPostSecret");
		
		jslSetValue("uiPostServerIp", "uiViewServerIp");
		jslSetValue("uiPostPort", "uiViewPort");
		jslSetValue("uiPostSecret", "uiViewSecret");
		jslDisable("uiPostPskValue");
	}
	else 
	{
		jslEnable("uiPostPskValue");
		jslDisable("uiPostServerIp");
		jslDisable("uiPostPort");
		jslDisable("uiPostSecret");

		if (type == 1)
			jslSetValue("uiPostPskValue", "uiViewPskHexValue");		
		else if (type == 2)
			jslSetValue("uiPostPskValue", "uiViewPskStringValue");	
		else
		{
			jslDisable("uiPostPskValue");
			jslDisable("uiPostGroupInterval");
		}
	}
	
	if(uiDoValidate(type)==true) jslFormSubmit("uiPostForm");
	return;
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}

function uiSelectPskType(value)
{
	if (value == 0)
	{
		jslDoToggleRadio("uiViewPskType",0, 3);
	}
	if (value == 1)
	{
		jslDoToggleRadio("uiViewPskType", 1, 3);
		jslSetValue("uiViewPskHexValue", "uiPostPskValue");	
	}
	else if (value == 2)
	{
		jslDoToggleRadio("uiViewPskType", 2, 3);
		jslSetValue("uiViewPskStringValue", "uiPostPskValue");	
	}
	uiCheckWpaType(value);
}

function uiChangePskType (arg)
{
	document.getElementById("uiPostPskType").value = arg;
	uiCheckWpaType(arg);
}

function uiCheckWpaType(value)
{
	switch (value)
	{
		case 0: // 802.1x. TODO Grey out others.
 				doPsk8021x();
				break;
		case 1: //PSK Hex
 				doPskHex();
				break;
		case 2: //PSK String
 				doPskString();
				break;
		default:
				doNone();
				break;
	}
}

function doPsk8021x( )
{
	jslEnable("uiViewServerIp");
	jslEnable("uiViewPort");
	jslEnable("uiViewSecret");
	jslEnable("uiViewGroupInterval");
	jslDisable("uiViewPskHexValue");
	jslDisable("uiViewPskStringValue");
}

function doPskHex( )
{
	jslEnable("uiViewPskHexValue");
	jslEnable("uiViewGroupInterval");
	jslDisable("uiViewPskStringValue");
	jslDisable("uiViewServerIp");
	jslDisable("uiViewPort");
	jslDisable("uiViewSecret");
}

function doPskString( )
{
	jslEnable("uiViewPskStringValue");
	jslEnable("uiViewGroupInterval");
	jslDisable("uiViewPskHexValue");
	jslDisable("uiViewServerIp");
	jslDisable("uiViewPort");
	jslDisable("uiViewSecret");
}

function doNone( )
{
	jslDisable("uiViewPskHexValue");
	jslDisable("uiViewPskStringValue");
	jslDisable("uiViewServerIp");
	jslDisable("uiViewPort");
	jslDisable("uiViewSecret");
	jslDisable("uiViewGroupInterval");
}

</script>
