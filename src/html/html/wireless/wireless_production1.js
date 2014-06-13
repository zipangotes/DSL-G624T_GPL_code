
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/val.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/val.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

<? setvariable var:bandmode `<? query ap:settings/band_mode ?>` ?>

function uiDoValidate()
{
   var message;
   var value;

 <? if eq `1` `<? query ap:status/dot11a_supported ?>`
 `value = document.getElementById("uiViewPrivateRegDomainA").value;
   message = valDoValidateInteger(value);
   
   if(message!=null) { alert("Private Reg. Domain for A: "+message);return false; }
 ` ` ` ?>
   value = document.getElementById("uiViewPrivateRegDomainBG").value;
   message = valDoValidateInteger(value);
   
   if(message!=null) { alert("Private Reg. Domain for B/G: "+message);return false; }


   return true;
}

function uiDoOnLoad()
{
	uiSelect("uiViewFeatureSet", document.getElementById("uiPostFeatureSet").value);  
	jslPostToViewCheckBox("uiView4xConcat","uiPost4xConcat");
	jslPostToViewCheckBox("uiView4XPktBurst","uiPost4XPktBurst");
	jslPostToViewCheckBox("uiView4xMixed","uiPost4xMixed");
       jslSetValue("uiViewPowerConstraint","uiPostPowerConstraint");
       jslPostToViewCheckBox("uiViewMultiDomain","uiPostMultiDomain");
       jslSetValue("uiViewCountryString","uiPostCountryString");
      <? if eq `1` `<? query ap:status/dot11a_supported ?>`
      ` jslDoShowComboBox("uiViewCurrRegDomainA","uiPostCurrRegDomainA");
       jslSetValue("uiViewPrivateRegDomainA","uiPostPrivateRegDomainA");
      ` ` ` ?>
       jslDoShowComboBox("uiViewCurrRegDomainBG","uiPostCurrRegDomainBG");
       jslSetValue("uiViewPrivateRegDomainBG","uiPostPrivateRegDomainBG");
      uiChangeMultiDomain(document.getElementById("uiViewMultiDomain").value);
      <? if eq `1` `<? query ap:status/dot11a_supported ?>`
      ` <? if eq `1` `$var:bandmode` `jslDisable("uiViewCurrRegDomainA","uiViewPrivateRegDomainA")` ` ` ?>
      ` ` ` ?>
      <? if eq `0` `$var:bandmode` `jslDisable("uiViewCurrRegDomainBG","uiViewPrivateRegDomainBG")` ` ` ?>
}

function uiDoSave()
{
	jslViewToPostCheckBox("uiPost4xConcat","uiView4xConcat");
	jslViewToPostCheckBox("uiPost4XPktBurst","uiView4XPktBurst");
	jslViewToPostCheckBox("uiPost4xMixed","uiView4xMixed");
       jslSetValue("uiPostPowerConstraint","uiViewPowerConstraint");
       jslViewToPostCheckBox("uiPostMultiDomain","uiViewMultiDomain");
       jslSetValue("uiPostCountryString","uiViewCountryString");
     <? if eq `1` `<? query ap:status/dot11a_supported ?>`
     `  jslSetValue("uiPostCurrRegDomainA","uiViewCurrRegDomainA");
       jslSetValue("uiPostPrivateRegDomainA","uiViewPrivateRegDomainA");
     ` ` ` ?>
       jslSetValue("uiPostCurrRegDomainBG","uiViewCurrRegDomainBG");
       jslSetValue("uiPostPrivateRegDomainBG","uiViewPrivateRegDomainBG");

	if(uiDoValidate()==true) jslFormSubmit("uiPostForm");
	return;
}

function uiChangeMultiDomain(value)
{
	switch(value) {
		case "on":
					//jslEnable("uiViewCountryString");
					break;
		case "off":
		default:
					//jslDisable("uiViewCountryString");					
					break;
	}
}

function uiChangeFeatureSet(value)
{
document.getElementById("uiPostFeatureSet").value=value;
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
