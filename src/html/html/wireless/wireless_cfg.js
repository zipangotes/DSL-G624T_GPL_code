
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/val.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/val.js"></script>' ?>

<? setvariable var:bandmode `<? query ap:settings/band_mode ?>` ?>

function uiDoOnLoad()
{
   jslSetValue("uiViewBeconPeriod","uiPostBeconPeriod");
   jslSetValue("uiViewDTIMPeriod","uiPostDTIMPeriod");
   jslSetValue("uiViewRTSThreshold","uiPostRTSThreshold");
   jslSetValue("uiViewFragThreshold","uiPostFragThreshold");
   jslSetValue("uiViewPowerLevel","uiPostPowerLevel");
   jslPostToViewCheckBox("uiViewMultiDomain","uiPostMultiDomain");
   jslSetValue("uiViewCountryString","uiPostCountryString");
 <? if eq `1` `<? query ap:status/dot11a_supported ?>`
  `jslDoShowComboBox("uiViewCurrRegDomainA","uiPostCurrRegDomainA");
   jslSetValue("uiViewPrivateRegDomainA","uiPostPrivateRegDomainA");
    jslDisable("uiViewCurrRegDomainA","uiViewPrivateRegDomainA");
  ` ` ` ?>
   jslDoShowComboBox("uiViewCurrRegDomainBG","uiPostCurrRegDomainBG");
   jslSetValue("uiViewPrivateRegDomainBG","uiPostPrivateRegDomainBG");
   jslDisable("uiViewMultiDomain","uiViewCountryString");
   jslDisable("uiViewCurrRegDomainBG","uiViewPrivateRegDomainBG");
}

function uiDoValidate()
{
   var message;
   var value;
   
   value = document.getElementById("uiViewBeconPeriod").value;
   message = valDoValidateInteger(value);
  
   if(message!=null) { alert("Beacon Period: "+message);return false; }

   value = document.getElementById("uiViewDTIMPeriod").value;
   message = valDoValidateInteger(value);
   
   if(message!=null) { alert("DTIM Period: "+message);return false; }

   value = document.getElementById("uiViewFragThreshold").value;
   message = valDoValidateInteger(value);
   
   if(message!=null) { alert("Frag Threshold: "+message);return false; }

   //value = document.getElementById("uiViewPowerConstraint").value;
   //message = valDoValidateInteger(value);
   
  // if(message!=null) { alert("Power Constraint: "+message);return false; }

   
 <? if eq `1` `<? query ap:status/dot11a_supported ?>`
 ` value = document.getElementById("uiViewPrivateRegDomainA").value;
   message = valDoValidateInteger(value);
   
   if(message!=null) { alert("Private Reg. Domain for A: "+message);return false; }
 ` ` ` ?>  

   value = document.getElementById("uiViewPrivateRegDomainBG").value;
   message = valDoValidateInteger(value);
   
   if(message!=null) { alert("Private Reg. Domain for B/G: "+message);return false; }

   return true;
}

function uiDoSave()
{	
  if(uiDoValidate()==true) { 
   jslSetValue("uiPostBeconPeriod","uiViewBeconPeriod");
   jslSetValue("uiPostDTIMPeriod","uiViewDTIMPeriod");
   jslSetValue("uiPostRTSThreshold","uiViewRTSThreshold");
   jslSetValue("uiPostFragThreshold","uiViewFragThreshold");
   jslSetValue("uiPostPowerLevel","uiViewPowerLevel");
   jslViewToPostCheckBox("uiPostMultiDomain","uiViewMultiDomain");
   jslSetValue("uiPostCountryString","uiViewCountryString");
 <? if eq `1` `<? query ap:status/dot11a_supported ?>`
  ` jslSetValue("uiPostCurrRegDomainA","uiViewCurrRegDomainA");
   jslSetValue("uiPostPrivateRegDomainA","uiViewPrivateRegDomainA");
  ` ` ` ?>
   jslSetValue("uiPostCurrRegDomainBG","uiViewCurrRegDomainBG");
   jslSetValue("uiPostPrivateRegDomainBG","uiViewPrivateRegDomainBG");
  }
   jslFormSubmit("uiPostForm");
   return;
}

function uiChangePowerLevel(value)
{
	document.getElementById("uiPostPowerLevel").value=value;
}

function uiChangeRegDomainA(value)
{
	document.getElementById("uiPostCurrRegDomainA").value=value;
}

function uiChangeRegDomainBG(value)
{
	document.getElementById("uiPostCurrRegDomainBG").value=value;
}



function uiDoCancel()
{
  jslGoTo(null,"home");
}

</script>
