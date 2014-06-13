
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

function uiDoValidate()
{
	var beacon = parseInt(document.getElementById("uiPostBeacon").value);
	var dtim = parseInt(document.getElementById("uiPostDtim").value);
	var rts_thresh = parseInt(document.getElementById("uiPostRTSThreshold").value);
	var frag_thresh = parseInt(document.getElementById("uiPostFragThreshold").value);

	if (document.getElementById("uiPostSSID").value.length == 0)
	{
		alert ("Invalid SSID");
		return false;
	}	

	if ((beacon < 1) || (beacon > 65535) || isNaN(beacon))
	{
		alert ("Beacon period out of range");
		return false;
	}

	if ((dtim < 1) || (dtim > 255) || isNaN(dtim))
	{
		alert ("DTIM period out of range");
		return false;
	}

	if ((rts_thresh < 0) || (rts_thresh > 4096) || isNaN(rts_thresh) )
	{
		alert ("RTS threshold out of range");
		return false;
	}

	if ((frag_thresh < 256) || (frag_thresh > 4096) || isNaN(frag_thresh) )
	{
		alert ("Fragmentation threshold out of range");
		return false;
	}
	return true;
}

function uiDoOnLoad()
{
	jslPostToViewCheckBox("uiViewAPEnabled","uiPostAPEnabled");
	document.getElementById("uiPostChannel").value = uiSelect("uiViewChannel", document.getElementById("uiPostChannel").value);  
	jslSetValue("uiViewSSID","uiPostSSID");
	jslSetValue("uiViewBeacon","uiPostBeacon");
	jslSetValue("uiViewDtim","uiPostDtim");
	jslSetValue("uiViewRTSThreshold","uiPostRTSThreshold");
	jslSetValue("uiViewFragThreshold","uiPostFragThreshold");

	document.getElementById("uiPostPowerLevel").value = uiSelect("uiViewPowerLevel", document.getElementById("uiPostPowerLevel").value - 1) + 1;

	document.getElementById("uiPostbgMode").value = uiSelect("uiViewbgMode", document.getElementById("uiPostbgMode").value);
	
	jslPostToViewCheckBox("uiViewHiddenSSID","uiPostHiddenSSID");
	jslPostToViewCheckBox("uiViewagEnabled","uiPostagEnabled");
}

function uiDoSave()
{
	jslViewToPostCheckBox("uiPostAPEnabled", "uiViewAPEnabled");
	jslSetValue("uiPostSSID", "uiViewSSID");
	jslSetValue("uiPostBeacon", "uiViewBeacon");
	jslSetValue("uiPostDtim", "uiViewDtim");
	jslSetValue("uiPostRTSThreshold", "uiViewRTSThreshold");
	jslSetValue("uiPostFragThreshold", "uiViewFragThreshold");
	jslViewToPostCheckBox("uiPostHiddenSSID", "uiViewHiddenSSID");
	jslViewToPostCheckBox("uiPostagEnabled", "uiViewagEnabled");

	if(uiDoValidate()==true) jslFormSubmit("uiPostForm");
	return;
}

function uiChangeChannel(channel)
{
  document.getElementById("uiPostChannel").value = channel;
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


function uiChangePowerLevel (arg)
{
	var power_level = parseInt(document.getElementById("uiViewPowerLevel").value);
	document.getElementById("uiPostPowerLevel").value = power_level + 1;
}

function uiChangebgMode (arg)
{
	document.getElementById("uiPostbgMode").value = arg;
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}
</script>
