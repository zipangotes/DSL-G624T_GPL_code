
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

var MaxPreambleCount	= 0;
var MaxTimeSlotCount  	= 0;
var MaxPbccCount  		= 0;
var MaxErpModeCount  	= 0;
var MaxErpTypeCount  	= 0;

<? setvariable var:bandmode <? query ap:settings/band_mode ?> ?>
<? if eq `` `<? echo $var:wone ?>`
   ` 
     <? if eq `2` `$var:bandmode` `<? setvariable var:wone `wone_a` ?>``` ?>
     <? if eq `1` `$var:bandmode` `<? setvariable var:wone `wone_g` ?>``` ?>
     <? if eq `0` `$var:bandmode` `<? setvariable var:wone `wone_a` ?>``` ?>
   ` `` ?>
   
function uiDoValidate()
{
	
	if (uiCheckRate("uiPostRateG1", document.getElementById("uiPostRateG1").value) == false)
	{
		alert ("Invalid Legacy Rate - 6M");
		return false;
	}
	if (uiCheckRate("uiPostRateG2", document.getElementById("uiPostRateG2").value) == false)
	{
		alert ("Invalid Legacy Rate - 9M");
		return false;
	}
	if (uiCheckRate("uiPostRateG3", document.getElementById("uiPostRateG3").value) == false)
	{
		alert ("Invalid Legacy Rate - 12M");
		return false;
	}
	if (uiCheckRate("uiPostRateG4", document.getElementById("uiPostRateG4").value) == false)
	{
		alert ("Invalid Legacy Rate - 18M");
		return false;
	}
	if (uiCheckRate("uiPostRateG5", document.getElementById("uiPostRateG5").value) == false)
	{
		alert ("Invalid Legacy Rate - 24M");
		return false;
	}
	if (uiCheckRate("uiPostRateG6", document.getElementById("uiPostRateG6").value) == false)
	{
		alert ("Invalid Legacy Rate - 36M");
		return false;
	}
	if (uiCheckRate("uiPostRateG7", document.getElementById("uiPostRateG7").value) == false)
	{
		alert ("Invalid Legacy Rate - 48M");
		return false;
	}
	if (uiCheckRate("uiPostRateG8", document.getElementById("uiPostRateG8").value) == false)
	{
		alert ("Invalid Extended Rate - 54M");
		return false;
	}

	if (uiCheckRate("uiPostRateGP1", document.getElementById("uiPostRateGP1").value) == false)
	{
		alert ("Invalid Extended Rate - 6M");
		return false;
	}
	if (uiCheckRate("uiPostRateGP2", document.getElementById("uiPostRateGP2").value) == false)
	{
		alert ("Invalid Extended Rate - 9M");
		return false;
	}
	if (uiCheckRate("uiPostRateGP3", document.getElementById("uiPostRateGP3").value) == false)
	{
		alert ("Invalid Extended Rate - 12M");
		return false;
	}
	if (uiCheckRate("uiPostRateGP4", document.getElementById("uiPostRateGP4").value) == false)
	{
		alert ("Invalid Extended Rate - 18M");
		return false;
	}
	if (uiCheckRate("uiPostRateGP5", document.getElementById("uiPostRateGP5").value) == false)
	{
		alert ("Invalid Extended Rate - 24M");
		return false;
	}
	if (uiCheckRate("uiPostRateGP6", document.getElementById("uiPostRateGP6").value) == false)
	{
		alert ("Invalid Extended Rate - 36M");
		return false;
	}
	if (uiCheckRate("uiPostRateGP7", document.getElementById("uiPostRateGP7").value) == false)
	{
		alert ("Invalid Extended Rate - 48M");
		return false;
	}
	if (uiCheckRate("uiPostRateGP8", document.getElementById("uiPostRateGP8").value) == false)
	{
		alert ("Invalid Extended Rate - 54M");
		return false;
	}

	return true;
}

function uiChangeWoneBand(value)
{
	document.getElementById("uiPostWoneBand").value=value;
	document.getElementById("uiPostForm").method="GET";
       document.getElementById("uiPostPageName").value="wireless_advanced";
       document.getElementById("uiPostForm").submit();
}

function uiDoOnLoad()
{
	var i = 0; 
	var Temp = new Array();

	<? multiquery ap:valid_settings/premble_algo "Temp[i++]='$01';" ?>
	MaxPreambleCount = i;   
	uiSelectValue("uiViewPreambleAlgo", document.getElementById("uiPostPreambleAlgo").value, MaxPreambleCount);

	i=0;
	<? multiquery ap:valid_settings/time_slot_algo "Temp[i++]='$01';" ?>
	MaxTimeSlotCount = i;   
	uiSelectValue("uiViewTimeSlotAlgo", document.getElementById("uiPostTimeSlotAlgo").value, MaxTimeSlotCount);

	i=0;
	<? multiquery ap:valid_settings/pbcc_algo "Temp[i++]='$01';" ?>
	MaxPbccCount = i;   
	uiSelectValue("uiViewPBCCAlgo", document.getElementById("uiPostPBCCAlgo").value, MaxPbccCount);

	i=0;
	<? multiquery ap:valid_settings/erp_mode "Temp[i++]='$01';" ?>
	MaxErpModeCount = i;   
	uiSelectValue("uiViewERPMode", document.getElementById("uiPostERPMode").value, MaxErpModeCount);


	i=0;
	<? multiquery ap:valid_settings/erp_type "Temp[i++]='$01';" ?>
	MaxErpTypeCount = i;   
	uiSelectValue("uiViewERPType", document.getElementById("uiPostERPType").value, MaxErpTypeCount);

	jslSetValue("uiViewRateG1","uiPostRateG1");
	jslSetValue("uiViewRateG2","uiPostRateG2");
	jslSetValue("uiViewRateG3","uiPostRateG3");
	jslSetValue("uiViewRateG4","uiPostRateG4");
	jslSetValue("uiViewRateG5","uiPostRateG5");
	jslSetValue("uiViewRateG6","uiPostRateG6");
	jslSetValue("uiViewRateG7","uiPostRateG7");
	jslSetValue("uiViewRateG8","uiPostRateG8");
	jslSetValue("uiViewRateGP1","uiPostRateGP1");
	jslSetValue("uiViewRateGP2","uiPostRateGP2");
	jslSetValue("uiViewRateGP3","uiPostRateGP3");
	jslSetValue("uiViewRateGP4","uiPostRateGP4");
	jslSetValue("uiViewRateGP5","uiPostRateGP5");
	jslSetValue("uiViewRateGP6","uiPostRateGP6");
	jslSetValue("uiViewRateGP7","uiPostRateGP7");
	jslSetValue("uiViewRateGP8","uiPostRateGP8");
	
     <? if eq `1` `<? query ap:status/dot11a_supported ?>`
     `
	jslSetValue("uiViewRate11AGP1","uiPostRate11AGP1");
	jslSetValue("uiViewRate11AGP2","uiPostRate11AGP2");
	jslSetValue("uiViewRate11AGP3","uiPostRate11AGP3");
	jslSetValue("uiViewRate11AGP4","uiPostRate11AGP4");
	jslSetValue("uiViewRate11AGP5","uiPostRate11AGP5");
	jslSetValue("uiViewRate11AGP6","uiPostRate11AGP6");
	jslSetValue("uiViewRate11AGP7","uiPostRate11AGP7");
	jslSetValue("uiViewRate11AGP8","uiPostRate11AGP8");
     ` ` ` ?>

	jslSetValue("uiViewCWMinMode","uiPostCWMinMode");
	jslSetValue("uiViewUsageTime","uiPostUsageTime");
	jslSetValue("uiViewLongRetryLimit","uiPostLongRetryLimit");
	jslSetValue("uiViewShortRetryLimit","uiPostShortRetryLimit");
	jslSetValue("uiViewTxLifeTimeLimit","uiPostTxLifeTimeLimit");
	jslSetValue("uiViewRxLifeTimeLimit","uiPostRxLifeTimeLimit");
	jslSetValue("uiViewRadioCalibration","uiPostRadioCalibration");
	jslSetValue("uiViewRadioCaliInterval","uiPostRadioCaliInterval");
	jslSetValue("uiViewRateFallback","uiPostRateFallback");

	jslPostToViewCheckBox("uiViewERPTI","uiPostERPTI");
	jslPostToViewCheckBox("uiViewRateAdaption","uiPostRateAdaption");
	jslPostToViewCheckBox("uiViewEnergyDetect","uiPostEnergyDetect");
       jslDoShowComboBox("uiViewWoneBand","uiPostWoneBand");
       <? if eq `1` `<? query ap:status/dot11a_supported ?>`
       `jslDoToggleRadio('uiViewWONE',document.getElementById("uiPostWONE").value,2);
       ` ` ` ?>
}

function uiDoSave()
{
	jslSetValue("uiPostRateG1", "uiViewRateG1");
	jslSetValue("uiPostRateG2", "uiViewRateG2");
	jslSetValue("uiPostRateG3", "uiViewRateG3");
	jslSetValue("uiPostRateG4", "uiViewRateG4");
	jslSetValue("uiPostRateG5", "uiViewRateG5");
	jslSetValue("uiPostRateG6", "uiViewRateG6");
	jslSetValue("uiPostRateG7", "uiViewRateG7");
	jslSetValue("uiPostRateG8", "uiViewRateG8");
	jslSetValue("uiPostRateGP1", "uiViewRateGP1");
	jslSetValue("uiPostRateGP2", "uiViewRateGP2");
	jslSetValue("uiPostRateGP3", "uiViewRateGP3");
	jslSetValue("uiPostRateGP4", "uiViewRateGP4");
	jslSetValue("uiPostRateGP5", "uiViewRateGP5");
	jslSetValue("uiPostRateGP6", "uiViewRateGP6");
	jslSetValue("uiPostRateGP7", "uiViewRateGP7");
	jslSetValue("uiPostRateGP8", "uiViewRateGP8");

     <? if eq `1` `<? query ap:status/dot11a_supported ?>`
     `jslSetValue("uiPostRate11AGP1", "uiViewRate11AGP1");
	jslSetValue("uiPostRate11AGP2", "uiViewRate11AGP2");
	jslSetValue("uiPostRate11AGP3", "uiViewRate11AGP3");
	jslSetValue("uiPostRate11AGP4", "uiViewRate11AGP4");
	jslSetValue("uiPostRate11AGP5", "uiViewRate11AGP5");
	jslSetValue("uiPostRate11AGP6", "uiViewRate11AGP6");
	jslSetValue("uiPostRate11AGP7", "uiViewRate11AGP7");
	jslSetValue("uiPostRate11AGP8", "uiViewRate11AGP8");
      ` ` ` ?>
	jslSetValue("uiPostCWMinMode","uiViewCWMinMode");
	jslSetValue("uiPostUsageTime","uiViewUsageTime");
	jslSetValue("uiPostLongRetryLimit","uiViewLongRetryLimit");
	jslSetValue("uiPostShortRetryLimit","uiViewShortRetryLimit");
	jslSetValue("uiPostTxLifeTimeLimit","uiViewTxLifeTimeLimit");
	jslSetValue("uiPostRxLifeTimeLimit","uiViewRxLifeTimeLimit");
	jslSetValue("uiPostRadioCalibration","uiViewRadioCalibration");
	jslSetValue("uiPostRadioCaliInterval","uiViewRadioCaliInterval");
	jslSetValue("uiPostRateFallback","uiViewRateFallback");

	jslViewToPostCheckBox("uiPostERPTI","uiViewERPTI");
	jslViewToPostCheckBox("uiPostRateAdaption","uiViewRateAdaption");
	jslViewToPostCheckBox("uiPostEnergyDetect","uiViewEnergyDetect");

	if(uiDoValidate()==true) jslFormSubmit("uiPostForm");
	return;
}

function uiChangeWONE(arg)
{
	document.getElementById("uiPostWONE").value = arg;
}


function uiDoShowRate(id, value)
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

	selector.selectedIndex = 0;
	return(selector.options[0].value);
}

function uiSelectValue(id, value, max)
{
	jslDoToggleRadio(id, value, max);
}

function uiChangePreambleAlgo (arg)
{
	jslDoToggleRadio("uiViewPreambleAlgo", arg, MaxPreambleCount);
	document.getElementById("uiPostPreambleAlgo").value = arg;
}

function uiChangeTimeSlotAlgo (arg)
{
	jslDoToggleRadio("uiViewTimeSlotAlgo", arg, MaxTimeSlotCount);
	document.getElementById("uiPostTimeSlotAlgo").value = arg;
}

function uiChangePBCCAlgo (arg)
{
	jslDoToggleRadio("uiViewPBCCAlgo", arg, MaxPbccCount);
	document.getElementById("uiPostPBCCAlgo").value = arg;
}

function uiChangeERPMode (arg)
{
	jslDoToggleRadio("uiViewERPMode", arg, MaxErpModeCount);
	document.getElementById("uiPostERPMode").value = arg;
}

function uiChangeERPType (arg)
{
	jslDoToggleRadio("uiViewERPType", arg, MaxErpTypeCount);
	document.getElementById("uiPostERPType").value = arg;
}

function uiCheckRate(id, value)
{
	if (value.length == 0)	
	{
		jslDisable(id);	
	}

	else if (value.match("[^BYN]") != null)
		return false;

	return true;
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}
</script>
