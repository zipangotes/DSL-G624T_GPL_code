<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

<? include /usr/www_safe/html/wireless/wireless_security.js ?>

var sec_profile = new Array();
var mssid;

function uiDoValidate()
{
	var wep_enabled = parseInt(document.getElementById("uiPostWepEnabled").value);
	var i;
	if (wep_enabled == 0)
	{
		jslDisable("uiPostAuthType");
		jslDisable("uiPostKeyId");
		jslDisable("uiPostKeyValue1");	
		jslDisable("uiPostKeyLen1");
		jslDisable("uiPostKeyValue2");	
		jslDisable("uiPostKeyLen2");
		jslDisable("uiPostKeyValue3");	
		jslDisable("uiPostKeyLen3");
		jslDisable("uiPostKeyValue4");	
		jslDisable("uiPostKeyLen4");
	}
	else
	{
        // Tentative: APDK limitations
        for (i=0; i<= 3; i++) 
	    {
	        if (i ==  <? echo $var:currentSSID ?>) 
    	      continue;
	        if (sec_profile[ i ] == 2)
            {
        	      alert("Multiple WEP security profiles are not supported.");
	              return false;
            }
            if (sec_profile[ i ] == 4)
            {
                alert("WEP is not supported in combination with 802.1x.");
                return false;
	        }
	    }
        if ((mssid) && (document.getElementById("uiViewAuthType").value == 1))
        {
            alert("Shared Authentication is not supported when Multiple SSID is Enabled.");
            return false;
	    }
	}
	return true;
}

function uiDoOnLoad()
{
    var i = 0;

    <? multiquery ap:status/mssid_sec_profile `sec_profile[i++]=$01;` ?>
    mssid = <? query ap:settings/multiple_ssid ?>;
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
 
	uiEnableWEP(document.getElementById("uiPostWepEnabled").value);
}

function uiDoSave()
{
	document.getElementById("uiPostPrivacyType").value = 2;
	jslViewToPostCheckBox("uiPostWepEnabled", "uiViewWepEnabled");
	jslSetValue("uiPostKeyValue1", "uiViewKeyValue1");
	jslSetValue("uiPostKeyValue2", "uiViewKeyValue2");
	jslSetValue("uiPostKeyValue3", "uiViewKeyValue3"); 
	jslSetValue("uiPostKeyValue4", "uiViewKeyValue4"); 
	jslDisable("uiPostChangeCurrentSSID");
    if(document.getElementById("uiViewKeyValue1").value=="")
      document.getElementById("uiPostKeyLen1").value=0; 

    if(document.getElementById("uiViewKeyValue2").value=="")
      document.getElementById("uiPostKeyLen2").value=0; 

    if(document.getElementById("uiViewKeyValue3").value=="")
      document.getElementById("uiPostKeyLen3").value=0; 

    if(document.getElementById("uiViewKeyValue4").value=="")
      document.getElementById("uiPostKeyLen4").value=0; 

	document.getElementById("AccessError").value = "";
	if(uiDoValidate()==true) jslFormSubmit("uiPostForm");
	return;
}

function uiDoCancel()
{
	jslGoTo(null,"home");
}

function uiEnableWEP(value)
{
	if (value == 0)	
	{
		jslDisable("uiViewAuthType");
		jslDisable("uiViewKeyId0");
		jslDisable("uiViewKeyValue1");
		jslDisable("uiViewKeyLen1");
		jslDisable("uiViewKeyId1");
		jslDisable("uiViewKeyValue2");
		jslDisable("uiViewKeyLen2")
		jslDisable("uiViewKeyId2");;
		jslDisable("uiViewKeyValue3");
		jslDisable("uiViewKeyLen3");
		jslDisable("uiViewKeyId3");
		jslDisable("uiViewKeyValue4");
		jslDisable("uiViewKeyLen4");
	}
	else
	{
		jslEnable("uiViewAuthType");
		jslEnable("uiViewKeyId0");
		jslEnable("uiViewKeyValue1");
		jslEnable("uiViewKeyLen1");
		jslEnable("uiViewKeyId1");
		jslEnable("uiViewKeyValue2");
		jslEnable("uiViewKeyLen2")
		jslEnable("uiViewKeyId2");;
		jslEnable("uiViewKeyValue3");
		jslEnable("uiViewKeyLen3");
		jslEnable("uiViewKeyId3");
		jslEnable("uiViewKeyValue4");
		jslEnable("uiViewKeyLen4");
	}
}

function uiChangeAuthType(arg)
{
	document.getElementById("uiPostAuthType").value = arg;
}

function uiDoEnableWEP()
{
	if (document.getElementById("uiViewWepEnabled").value == "off")
		uiEnableWEP(0);
	else
		uiEnableWEP(1);
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

function uiChangeCurrentSSID(new_id)
{
  currentSSID = new_id;
  document.getElementById("uiPostChangeCurrentSSID").value=new_id;
  jslDisable("uiPostPrivacyType");
  jslDisable("uiPostWepEnabled");
  jslDisable("uiPostAuthType");
  jslDisable("uiPostKeyId");
  jslDisable("uiPostKeyValue1");	
  jslDisable("uiPostKeyLen1");
  jslDisable("uiPostKeyValue2");	
  jslDisable("uiPostKeyLen2");
  jslDisable("uiPostKeyValue3");	
  jslDisable("uiPostKeyLen3");
  jslDisable("uiPostKeyValue4");	
  jslDisable("uiPostKeyLen4");

  jslFormSubmit("uiPostForm");
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
