<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/val.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/val.js"></script>' ?>
<? if eq 1 1 '' '<script language="JavaScript">' ?>

<? include /usr/www_safe/html/wireless/wireless_security.js ?>

var sec_profile = new Array();

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
			alert ("Invalid secret");
			return false;
		}
	}
	else if (type == 1)
	{
		if((document.getElementById("uiPostPskValue").value.length == 0) || 
			(document.getElementById("uiPostPskValue").value.length > 63))
		{
			alert ("Invalid PSK String. Maximum length is 63 characters");
			return false;
		}
	}
	
	if ((document.getElementById("uiPostGroupInterval").value.length == 0) || 
		 parseInt(document.getElementById("uiPostGroupInterval").value) < 0)
	{
		alert ("Invalid Group Interval");	
		return false;
	}
	//alert ("Privacy type is set to WPA. Multiple SSID support will be disabled");	
	return true;
}

function uiDoOnLoad()
{
  var wpa_type =  parseInt(document.getElementById("uiPostPrivacyType").value);
  var i=0;
  <? multiquery ap:status/mssid_sec_profile `sec_profile[i++]=$01;` ?>
	jslSetValue("uiViewGroupInterval","uiPostGroupInterval");
	jslSetValue("uiViewServerIp","uiPostServerIp");
	jslSetValue("uiViewPort","uiPostPort");
	jslSetValue("uiViewSecret","uiPostSecret");
  jslPostToViewCheckBox("uiViewWpa2PreAuth","uiPostWpa2PreAuth");
  switch (wpa_type)
  {
  case 1:
    jslDoToggleRadio("uiViewWpaType", 0, 3); 
    jslDisable("uiViewWpa2PreAuth");
    break;
  case 8:
    jslDoToggleRadio("uiViewWpaType", 1, 3);
    break;
  case 9:
    jslDoToggleRadio("uiViewWpaType", 2, 3);
    break;
  default:
    jslDoToggleRadio("uiViewWpaType", 0, 3);
    jslDisable("uiViewWpa2PreAuth");
    uiChangeWpaType(1);
    uiSelectPskType(0);
    document.getElementById("uiViewWpaType0").value = "on"
    document.getElementById("uiViewWpaType1").value = "off"
    document.getElementById("uiViewWpaType2").value = "off"
    break;
  }

	var value = parseInt(document.getElementById("uiPostPskType").value);
	uiSelectPskType(value);
}

function uiDoSave()
{
	var psk_type = document.getElementById("uiPostPskType").value;
	var wpa_type = document.getElementById("uiPostPrivacyType").value;

	jslSetValue("uiPostGroupInterval", "uiViewGroupInterval");

	if (wpa_type == 1)
	{
	  jslDisable("uiPostWpa2PreAuth");
	}
	
	if (psk_type == 0)
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

		//if (psk_type == 1)
			jslSetValue("uiPostPskValue", "uiViewPskStringValue");	
		//else
		//{
		//	jslDisable("uiPostPskValue");
		//	jslDisable("uiPostGroupInterval");
		//}
		}
	jslDisable("uiPostChangeCurrentSSID");
	jslViewToPostCheckBox("uiPostWpa2PreAuth", "uiViewWpa2PreAuth");
	if(uiDoValidate(psk_type)==true) jslFormSubmit("uiPostForm");
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
		jslDoToggleRadio("uiViewPskType",0, 2);
	}
	else if (value == 1)
	{
		jslDoToggleRadio("uiViewPskType", 1, 2);
		jslSetValue("uiViewPskStringValue", "uiPostPskValue");	
	}
	uiCheckWpaType(value);
}

function uiChangePskType (arg)
{
	document.getElementById("uiPostPskType").value = arg;
	uiCheckWpaType(arg);
}

function uiChangeWpaType(wpa_type)
{
  // wpa_type: 1 WPA, 8 WPA2, 9 AnyWPA
  switch( wpa_type )
  {
  case 1:
    if ( document.getElementById("uiViewWpa2PreAuth").value == "on" ){
      alert("WPA is selected, WPA2 Pre-Authentication will be turned off");
      jslDoToggleCheckBox("uiViewWpa2PreAuth");
    }
    jslDisable("uiViewWpa2PreAuth");
    break;
  case 8:
    jslEnable("uiViewWpa2PreAuth");
    break;
  case 9:
    jslEnable("uiViewWpa2PreAuth");
    break;
  default:
    break;
  }
  document.getElementById("uiPostPrivacyType").value = wpa_type;
}

function uiCheckWpaType(value)
{
	switch (value)
	{
		case 0: // 802.1x. TODO Grey out others.
 				doPsk8021x();
				break;
		case 1: //PSK String
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
	jslDisable("uiViewPskStringValue");
}

function doPskString( )
{
	jslEnable("uiViewPskStringValue");
	jslEnable("uiViewGroupInterval");
	jslDisable("uiViewServerIp");
	jslDisable("uiViewPort");
	jslDisable("uiViewSecret");
}

function doNone( )
{
	jslDisable("uiViewPskStringValue");
	jslDisable("uiViewServerIp");
	jslDisable("uiViewPort");
	jslDisable("uiViewSecret");
	jslDisable("uiViewGroupInterval");
}

function uiChangeCurrentSSID(new_id)
{
  currentSSID = new_id;
  document.getElementById("uiPostChangeCurrentSSID").value=new_id;
  jslDisable("uiPostPrivacyType");
  jslDisable("uiPostServerIp");
  jslDisable("uiPostPort");
  jslDisable("uiPostSecret");
  jslDisable("uiPostGroupInterval");
  jslDisable("uiPostPskType");
  jslDisable("uiPostPskValue");
  jslDisable("uiPostWpa2PreAuth");
  jslFormSubmit("uiPostForm");
}

</script>
