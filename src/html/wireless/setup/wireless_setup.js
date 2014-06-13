
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

function uiDoValidate()
{
	if (document.getElementById("uiPostSSID").value.length == 0)
	{
		alert ("Invalid SSID");
		return false;
	}	

	return true;
}

function uiDoOnLoad()
{
	jslPostToViewCheckBox("uiViewAPEnabled","uiPostAPEnabled");
	document.getElementById("uiPostChannel").value = uiSelect("uiViewChannel", document.getElementById("uiPostChannel").value);  
	jslSetValue("uiViewSSID","uiPostSSID");
}

function uiDoSave()
{
	jslViewToPostCheckBox("uiPostAPEnabled", "uiViewAPEnabled");
	jslSetValue("uiPostSSID", "uiViewSSID");

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

function uiDoCancel()
{
     jslGoTo(null,"home");
}
</script>
