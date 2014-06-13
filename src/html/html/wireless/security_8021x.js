<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/val.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/val.js"></script>' ?>
<? if eq 1 1 '' '<script language="JavaScript">' ?>

<? include /usr/www_safe/html/wireless/wireless_security.js ?>

var sec_profile = new Array();

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
		alert ("Invalid secret");
		return false;
	}
	
	if ((document.getElementById("uiPostGroupInterval").value.length == 0) || 
		 parseInt(document.getElementById("uiPostGroupInterval").value) <= 0)
	{
		alert ("Invalid Group Interval");	
		return false;
	}
	// Tentative: APDK limitations
	for ( i=0; i<= 3; i++) 
	{
	  if ( i ==  <? echo $var:currentSSID ?> ) 
	    continue;
	  if ( sec_profile[ i ] == 2 )
          {
	    alert("802.1x is not supported in combination with WEP, please select a different type of security for this SSID.");
	    return false;
	  }
	  if ( sec_profile[ i ] == 4 ){
	    alert("Two 802.1x security profiles are not supported, please select a different type of security for this SSID.");
	    return false;
	  }
	  
	}
	return true;
}

function uiDoOnLoad()
{
  var i = 0;

  <? multiquery ap:status/mssid_sec_profile `sec_profile[i++]=$01;` ?>
	jslSetValue("uiViewServerIp","uiPostServerIp");
	jslSetValue("uiViewPort","uiPostPort");
	jslSetValue("uiViewSecret","uiPostSecret");
	jslSetValue("uiViewGroupInterval","uiPostGroupInterval");
}

function uiDoSave()
{
	document.getElementById("uiPostPrivacyType").value = 4;
	jslSetValue("uiPostServerIp", "uiViewServerIp");
	jslSetValue("uiPostPort", "uiViewPort");
	jslSetValue("uiPostSecret", "uiViewSecret");
	jslSetValue("uiPostGroupInterval", "uiViewGroupInterval");
	jslDisable("uiPostChangeCurrentSSID");
	if(uiDoValidate()==true) jslFormSubmit("uiPostForm");
	return;
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
  jslFormSubmit("uiPostForm");
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}
</script>
