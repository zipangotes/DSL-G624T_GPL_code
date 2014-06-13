
<? if eq 1 1 '<script language="JavaScript">' '' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/val.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/val.js"></script>' ?>


var numSSIDList;
var privacy_type;
var SSIDList = new Array(10);
var ssid0_vlan_id;
var wdsmode;

function uiDoOnLoad()
{

	var i=0;

	jslPostToViewCheckBox("uiViewEnableMSSID","uiPostEnableMSSID");
    if ( document.getElementById("uiPostEnableMSSID").value == 0 )
	{
        jslDisable("uiViewAddSSID");
        jslDisable("uiViewAddVlanID");
        jslDisable("uiViewAddPriority");
	}
    else
    {
        jslEnable("uiViewAddSSID");
        jslEnable("uiViewAddVlanID");
        jslEnable("uiViewAddPriority");
	}
  for ( i=0; i< 10; i++)
    SSIDList[ i ] = 0;
  i = 0;
  <? multiquery ap:status/mssid_list "SSIDList[i++]='$01';" ?>
  ssid0_vlan_id = <? query ap:settings/ssid0/vlan ?>;
  wdsmode = <? query ap:settings/wds_mode ?>;
  numSSIDList = i;
}

function uiDoSave()
{
     return;
}

function uiDoAddSSID()
{
  var i;

  if ( document.getElementById("uiViewEnableMSSID").value == "off" ){
    alert("Multiple SSID not enabled.");
    return;
  }

  if(numSSIDList==4) 
  { 
        alert("Maximum number 3 of secondary SSIDs supported");
        return; 
  } 

  if ( document.getElementById("uiViewAddSSID").value.length == 0  ){
    alert("Invalid SSID");
    return;
  }
  if ((document.getElementById("uiViewAddVlanID").value.length == 0) ||
      (valDoValidateInteger(document.getElementById("uiViewAddVlanID").value) != null))
  {
    alert ("Invalid VLAN ID");
    return;
  }

  if (document.getElementById("uiViewAddVlanID").value > 4095 ||
      document.getElementById("uiViewAddVlanID").value == 0 )
  {
    alert ("Secondary VLAN ID range is [1, 4095]");
    return;
  }
  if ((document.getElementById("uiViewAddPriority").value.length == 0) ||
      (valDoValidateInteger(document.getElementById("uiViewAddPriority").value) != null))
            uiViewAddPriority.value = 0;
  if (document.getElementById("uiViewAddPriority").value < 0 ||
      document.getElementById("uiViewAddPriority").value > 7 )
  {
    alert ("SSID Priority range is [0, 7]");
    return;
  }

    jslEnable ("uiPostAddSSID");
    jslEnable ("uiPostAddVlanID");
    jslEnable ("uiPostAddPriority");
	jslSetValue("uiPostAddSSID","uiViewAddSSID");
    jslSetValue("uiPostAddVlanID","uiViewAddVlanID");
    jslSetValue("uiPostAddPriority","uiViewAddPriority");
	jslViewToPostCheckBox("uiPostEnableMSSID","uiViewEnableMSSID");

  // next ssid to be added will have index number i
  // index 0 is always reserved for primary ssid
  for (i = 1; i< 4; i++)
	{ 
    if ( SSIDList[ i ] == 0 ){
      // need to enable input tag to wan bridge interface so vlan_id
      // tag under wan bridge be changed accordingly
      document.getElementById( "uiPostWbVlanID"+i ).disabled = false;
      jslSetValue("uiPostWbVlanID"+i, "uiViewAddVlanID");
      document.getElementById( "uiPostWbPBit"+i ).disabled = false;
      jslSetValue("uiPostWbPBit"+i, "uiViewAddPriority");
      break;
    }
  }
  
  if ( numSSIDList == 1)
	{
    // now adding first secondary ssid, check to see if ssid0 vlan is zero
    // if so, alert the user
    if ( ssid0_vlan_id == 0 )
      alert("Adding Secondary SSID will have to turn on VLAN. Please change Primary SSID VLAN ID to be non-zero.");

	}	
	jslDisable("uiPostDelSSID");
    jslDisable("uiPostPrimaryVlanID");
    jslDisable("uiPostPrimaryPBit");
	document.getElementById("SSIDError").value = "";
	jslFormSubmit("uiPostForm");
	return;	
}

function uiDoDelSSID(arg, id, index)
{
  if ( numSSIDList == 2 ) 
  {
    if ( confirm("Delete the last secondary SSID? VLAN and QOS will be turned off." ))
    {
      // if this is the last secondary ssid to delete, need to turn off
      // vlan
      document.getElementById("uiPostPrimaryVlanID").disabled = false;
      document.getElementById("uiPostPrimaryVlanID").value = 0;
      document.getElementById("uiPostPrimaryPBit").disabled = false;
      document.getElementById("uiPostPrimaryPBit").value = 0;
      document.getElementById("uiPostWbVlanID0").disabled = false;
      document.getElementById("uiPostWbVlanID0").value = 0;
      document.getElementById("uiPostWbPBit0").disabled = false;
      document.getElementById("uiPostWbPBit0").value = 0;

      document.getElementById( "uiPostWbVlanID"+index ).disabled = false;
      document.getElementById( "uiPostWbVlanID"+index ).value = 0;
      document.getElementById( "uiPostWbPBit"+index ).disabled = false;
      document.getElementById( "uiPostWbPBit"+index ).value = 0;

      jslDisable("uiPostAddSSID");
      jslEnable ("uiPostDelSSID");
      jslViewToPostCheckBox("uiPostEnableMSSID","uiViewEnableMSSID");
      document.getElementById("uiPostDelSSID").value = arg;
      document.getElementById("SSIDError").value = "";
      document.getElementById("uiViewAddSSID").disabled = true;
      document.getElementById("uiViewAddVlanID").disabled = true;
      document.getElementById("uiViewAddPriority").disabled = true;
      jslFormSubmit("uiPostForm");
    }
    else {
      document.getElementById("uiViewAddSSID").disabled = false;
      document.getElementById("uiViewAddVlanID").disabled = false;
      document.getElementById("uiViewAddPriority").disabled = false;
      jslDoToggleCheckBox(id);
      return;
    }
  }
  else 
{
	if(confirm("Delete SSID ?"))
	{
		jslDisable("uiPostAddSSID");
		jslEnable ("uiPostDelSSID");
        document.getElementById( "uiPostWbVlanID"+arg ).disabled = false;
        document.getElementById( "uiPostWbVlanID"+arg ).value = 0;
        document.getElementById( "uiPostWbPBit"+arg ).disabled = false;
        document.getElementById( "uiPostWbPBit"+arg ).value = 0;

		jslViewToPostCheckBox("uiPostEnableMSSID","uiViewEnableMSSID");
		document.getElementById("uiPostDelSSID").value = arg;
		document.getElementById("SSIDError").value = "";
        document.getElementById("uiViewAddSSID").disabled = true;
        document.getElementById("uiViewAddVlanID").disabled = true;
        document.getElementById("uiViewAddPriority").disabled = true;

		jslFormSubmit("uiPostForm");
	}
    else {
      document.getElementById("uiViewAddSSID").disabled = false;
      document.getElementById("uiViewAddVlanID").disabled = false;
      document.getElementById("uiViewAddPriority").disabled = false;
      jslDoToggleCheckBox(id);
	return;	
}
  }
}

function uiViewDeleteAll(value, id)
{
  var i;
  
   switch(value) {
   	case "on":
	  if (confirm("Delete All Secondary SSID(s)? This will turn off the VLAN and QOS."))
	  {
	    document.getElementById("uiPostPrimaryVlanID").disabled = false;
	    document.getElementById("uiPostPrimaryVlanID").value = 0;
        document.getElementById("uiPostPrimaryPBit").disabled = false;
        document.getElementById("uiPostPrimaryPBit").value = 0;

	    document.getElementById("uiPostWbVlanID0").disabled = false;
	    document.getElementById("uiPostWbVlanID0").value = 0;
        document.getElementById("uiPostWbPBit0").disabled = false;
        document.getElementById("uiPostWbPBit0").value = 0;

	    for ( i =1; i< 4; i++)
	    {
		    document.getElementById( "uiPostWbVlanID"+i ).disabled = false;
    		document.getElementById( "uiPostWbVlanID"+i ).value = 0;
		    document.getElementById( "uiPostWbPBit"+i ).disabled = false;
    		document.getElementById( "uiPostWbPBit"+i ).value = 0;
	    }
	    document.getElementById("uiViewAddSSID").disabled = true;
	    document.getElementById("uiViewAddVlanID").disabled = true;
	    document.getElementById("uiViewAddPriority").disabled = true;
	    jslEnable("uiPostFlush");
	    jslFormSubmit("uiPostForm");
	  }
      else
	  {
	    document.getElementById("uiViewAddSSID").disabled = false;
	    document.getElementById("uiViewAddVlanID").disabled = false;
        document.getElementById("uiViewAddPriority").disabled = false;
	    jslDoToggleCheckBox(id);
	  }
     break;
	case "off":
	default:
		     jslDisable("uiPostFlush");
		     break;
   }
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}

function uiToggleSSIDFields(value)
{
    if (value == "on")
    {
        if (wdsmode != 3)
        {
            alert("WDS is enabled. Cannot enable Multiple SSID");
            jslDoToggleCheckBox("uiViewEnableMSSID");
            return;
        }
        jslEnable('uiViewAddSSID'); 
        jslEnable('uiViewAddVlanID');
        jslEnable('uiViewAddPriority');
    }
    else 
    {
        jslDisable('uiViewAddSSID'); 
        jslDisable('uiViewAddVlanID');
        jslDisable('uiViewAddPriority');
    }
}
</script>
