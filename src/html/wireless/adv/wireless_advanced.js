
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

var MaxPreambleCount	= 0;
var MaxTimeSlotCount  	= 0;
var MaxPbccCount  		= 0;
var MaxErpModeCount  	= 0;
var MaxErpTypeCount  	= 0;

function uiDoValidate()
{
	if (parseInt(document.getElementById("uiPostSuppRate").value) < parseInt(document.getElementById("uiPostBasicRate").value))
	{
		alert ("Max Supported Rate should be greater than Max basic rate.");
		return false;
	}
	
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

function uiDoOnLoad()
{
	var i = 0; 
	var Temp = new Array();

	<? multiquery ap:valid_settings/premble_algo "Temp[i++]='$1';" ?>
	MaxPreambleCount = i;   
	uiSelectValue("uiViewPreambleAlgo", document.getElementById("uiPostPreambleAlgo").value, MaxPreambleCount);

	i=0;
	<? multiquery ap:valid_settings/time_slot_algo "Temp[i++]='$1';" ?>
	MaxTimeSlotCount = i;   
	uiSelectValue("uiViewTimeSlotAlgo", document.getElementById("uiPostTimeSlotAlgo").value, MaxTimeSlotCount);

	i=0;
	<? multiquery ap:valid_settings/pbcc_algo "Temp[i++]='$1';" ?>
	MaxPbccCount = i;   
	uiSelectValue("uiViewPBCCAlgo", document.getElementById("uiPostPBCCAlgo").value, MaxPbccCount);

	i=0;
	<? multiquery ap:valid_settings/erp_mode "Temp[i++]='$1';" ?>
	MaxErpModeCount = i;   
	uiSelectValue("uiViewERPMode", document.getElementById("uiPostERPMode").value, MaxErpModeCount);

	i=0;
	<? multiquery ap:valid_settings/erp_type "Temp[i++]='$1';" ?>
	MaxErpTypeCount = i;   
	uiSelectValue("uiViewERPType", document.getElementById("uiPostERPType").value, MaxErpTypeCount);

	document.getElementById("uiPostBasicRate").value = uiDoShowRate("uiViewBasicRate", document.getElementById("uiPostBasicRate").value);  

	document.getElementById("uiPostSuppRate").value = uiDoShowRate("uiViewSuppRate", document.getElementById("uiPostSuppRate").value);  

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

	if(uiDoValidate()==true) jslFormSubmit("uiPostForm");
	return;
}

function uiChangeBasicRate(arg)
{
	document.getElementById("uiPostBasicRate").value = arg;
}

function uiChangeSuppRate(arg)
{
	document.getElementById("uiPostSuppRate").value = arg;
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
