
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/encrypt.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/encrypt.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/val.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/val.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

var numSSIDList;
var SSIDList = new Array(10);
var wdsmode;

function uiDoValidate()
{
	var output;

	if (document.getElementById("uiPostSSID").value.length == 0)
	{
		alert ("Invalid SSID");
		return false;
	}
	else
	{
		output = encrypt(document.getElementById("uiPostSSID").value);
		document.getElementById("uiPostSSID").value = output;
	}
	if (valDoValidateInteger(document.getElementById("uiViewVlanID").value) 
	    != null)
	{
	  alert ("Invalid VLAN ID");
	  return false;
	}
	if (document.getElementById("uiViewVlanID").value  != 0 &&
	    numSSIDList == 1 ){
	  alert("Multiple SSID support is disabled. Cannot configure VLAN to be non-zero.");
	  document.getElementById("uiViewVlanID").value = 0;
	  return false;
	}

    if (document.getElementById("uiViewAddPriority").value < 0 ||
        document.getElementById("uiViewAddPriority").value > 7 )
    {
        alert ("SSID Priority range is [0, 7]");
        return false;
    }

	return true;
}

function uiDoOnLoad()
{
  var i;
  for ( i=0; i< 10; i++)
    SSIDList[ i ] = 0;
  i = 0;
  <? multiquery ap:status/mssid_list "SSIDList[i++]='$01';" ?>
  numSSIDList = i;

	jslPostToViewCheckBox("uiViewAPEnabled","uiPostAPEnabled");
	document.getElementById("uiViewSSID").value = decrypt(document.getElementById("uiPostSSID").value);
//	jslSetValue("uiViewSSID","uiPostSSID");
	<? if eq `1` `<? query ap:status/dot11a_supported ?>`
	` jslDoToggleRadio('uiViewBandMode',document.getElementById("uiPostBandMode").value,3);
       jslSetValue("uiViewChannelA", "uiPostChannelA");  
   	   uiChangeBandMode(document.getElementById("uiPostBandMode").value);
       ` ` ` ?>
	jslSetValue("uiViewChannelBG", "uiPostChannelBG");  
    jslSetValue("uiViewVlanID", "uiPostVlanID");  
    jslSetValue("uiViewAddPriority", "uiPostAddPriority");  
	uiSelect("uiViewBGMode", document.getElementById("uiPostBGMode").value);  
	jslPostToViewCheckBox("uiView4x","uiPost4x");
	jslPostToViewCheckBox("uiViewHiddenSSID","uiPostHiddenSSID");
	jslPostToViewCheckBox("uiViewUserIsolation","uiPostUserIsolation");
    jslPostToViewCheckBox("uiViewQosType","uiPostQosType");
    wdsmode = <? query ap:settings/wds_mode ?>;
}

function uiChangeQosType(new_qos_type)
{
  if ((numSSIDList == 1) && 
       (document.getElementById("uiViewQosType").value == "on"))
  {
    alert("Multiple SSID support is disabled. Cannot enable QOS");
    jslDoToggleCheckBox("uiViewQosType");
    return;
  }
  if ((wdsmode != 3) && 
       (document.getElementById("uiViewQosType").value == "on"))
  {
    alert("WDS is enabled. Cannot enable QOS");
    jslDoToggleCheckBox("uiViewQosType");
    return;
  }

  document.getElementById("uiPostQosType").value = new_qos_type;
}

function uiDoSave()
{
	jslViewToPostCheckBox("uiPostAPEnabled", "uiViewAPEnabled");
	jslSetValue("uiPostSSID", "uiViewSSID");
	jslSetValue("uiPostVlanID", "uiViewVlanID");
	jslSetValue("uiPostWbVlanID", "uiViewVlanID");
    jslSetValue("uiPostAddPriority", "uiViewAddPriority");  
	jslSetValue("uiPostWbPBit0", "uiViewAddPriority");
	jslViewToPostCheckBox("uiPost4x","uiView4x");
	jslViewToPostCheckBox("uiPostHiddenSSID","uiViewHiddenSSID");
	jslViewToPostCheckBox("uiPostUserIsolation","uiViewUserIsolation");
	jslViewToPostCheckBox("uiPostQosType", "uiViewQosType");
	<? if eq `1` `<? query ap:status/dot11a_supported ?>`
	` jslSetValue("uiPostChannelA", "uiViewChannelA");  
	` ` ` ?>
	jslSetValue("uiPostChannelBG", "uiViewChannelBG");  

	if(uiDoValidate()==true) jslFormSubmit("uiPostForm");
	return;
}

function uiChangeChannelA(channel)
{
  document.getElementById("uiPostChannelA").value = channel;
}

function uiChangeChannelBG(channel)
{
  document.getElementById("uiPostChannelBG").value = channel;
}

function uiChangeBGMode(mode)
{
  document.getElementById("uiPostBGMode").value = mode;
}

function uiChangeBandMode(value)
{
    switch(value) {
    	case '0': 
    		       jslDisable("uiViewChannelBG");
    		       jslEnable("uiViewChannelA");
    		       jslDisable("uiViewBGMode");
    			break;
    	case '1':
    		       jslDisable("uiViewChannelA");
    		       jslEnable("uiViewChannelBG");
    		       jslEnable("uiViewBGMode");
    			break;
    	case '2':
    		       jslEnable("uiViewChannelA","uiViewChannelBG");
    		       jslEnable("uiViewBGMode");
    			break;
    }
document.getElementById("uiPostBandMode").value=value;
}

function uiSelect(id, value)
{
  var i;
  var selector = document.getElementById(id);
  if(selector==null) return;
	
  for(i=0; i < selector.length; i++)
  {
    if(selector.options[i].value == value)
	{
      selector.selectedIndex = i;
	  return(value);
	}
  }
  return(selector.options[0].value);
}

function uiDoCancel()
{
     jslGoTo(null,"home");
}
</script>
