<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

<? include /usr/www_safe/html/adv/wireless_security.js ?>

function uiDoValidate()
{
	if (document.getElementById("uiPostKeyValue1").value.length == 0)	
	{
		jslDisable("uiPostKeyValue1");	
	}
	if (document.getElementById("uiPostKeyValue2").value.length == 0)	
	{
		jslDisable("uiPostKeyValue2");	
	}
	if (document.getElementById("uiPostKeyValue3").value.length == 0)	
	{
		jslDisable("uiPostKeyValue3");	
	}
	if (document.getElementById("uiPostKeyValue4").value.length == 0)	
	{
		jslDisable("uiPostKeyValue4");	
	}

	return true;
}

function uiDoOnLoad()
{
	jslPostToViewCheckBox("uiViewWepEnabled","uiPostWepEnabled");
	document.getElementById("uiPostAuthType").value = uiDoShowAuthType(document.getElementById("uiPostAuthType").value); 
	uiSelectValue("uiViewKeyId", document.getElementById("uiPostKeyId").value, 4);
	jslSetValue("uiViewKeyValue1", "uiPostKeyValue1"); 
	document.getElementById("uiPostKeyLen1").value = uiDoShowKeyLen("uiViewKeyLen1", document.getElementById("uiPostKeyLen1").value); 
	jslSetValue("uiViewKeyValue2", "uiPostKeyValue2"); 
	document.getElementById("uiPostKeyLen2").value = uiDoShowKeyLen("uiViewKeyLen2", document.getElementById("uiPostKeyLen2").value);  
	jslSetValue("uiViewKeyValue3", "uiPostKeyValue3"); 
	document.getElementById("uiPostKeyLen3").value = uiDoShowKeyLen("uiViewKeyLen3", document.getElementById("uiPostKeyLen3").value);  
	jslSetValue("uiViewKeyValue4", "uiPostKeyValue4"); 
	document.getElementById("uiPostKeyLen4").value = uiDoShowKeyLen("uiViewKeyLen4", document.getElementById("uiPostKeyLen4").value);  
}

function uiDoSave()
{
	document.getElementById("uiPostPrivacyType").value = 1;
	jslViewToPostCheckBox("uiPostWepEnabled", "uiViewWepEnabled");
	jslSetValue("uiPostKeyValue1", "uiViewKeyValue1");
	jslSetValue("uiPostKeyValue2", "uiViewKeyValue2");
	jslSetValue("uiPostKeyValue3", "uiViewKeyValue3"); 
	jslSetValue("uiPostKeyValue4", "uiViewKeyValue4"); 
 
	if(uiDoValidate()==true) jslFormSubmit("uiPostForm");
	return;
}

function uiDoCancel()
{
	jslGoTo(null,"home");
}

function uiChangeAuthType(arg)
{
  document.getElementById("uiPostAuthType").value = arg;
}

function uiDoShowAuthType(value)
{
  var i;
  var selector = document.getElementById("uiViewAuthType");
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

function uiChangeKeyLen(id, arg)
{
	document.getElementById(id).value = arg;
}

function uiDoShowKeyLen(id, value)
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

function uiDoSelectWepKeyId (arg)
{
	document.getElementById("uiPostKeyId").value = arg;
}

</script>
