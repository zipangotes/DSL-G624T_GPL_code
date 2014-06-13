
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/val.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/val.js"></script>' ?>

<? include /usr/www_safe/html/wireless/wireless_mgmt.js ?>

var MaxAccessList;

function checkMACAddress(value)
{
	if((value.length == 0) || 
		(value.match("^[0-9a-fA-F]{2}\-[0-9a-fA-F]{2}\-[0-9a-fA-F]{2}\-[0-9a-fA-F]{2}\-[0-9a-fA-F]{2}\-[0-9a-fA-F]{2}$")			 		== null))
		return false;
	else
		return true;
}

function uiDoOnLoad()
{
  var AccessList = new Array();
  var i = 0;
  <? multiquery ap:status/access_list "AccessList[i++]='$01';" ?>
  MaxAccessList = i;

  uiSelectAccessType(document.getElementById("uiPostAccessType").value);
}

function uiSelectAccessType(value)
{
	var i;
	
	if (value == 0)	
	{
  		document.getElementById("uiViewAccessEnable").value="off";
	  	document.getElementById("uiViewAccessEnable").src="<? echo $var:btnCheckBoxOff ?>";
		jslDisable("uiViewAccessType0");
		jslDisable("uiViewAccessType1");
		jslDisable("uiViewAddMac");
		jslDisable("uiViewAddMacButton");
	    <? if neq 0 <? multiquerycount ap:status/access_list?>  
	    `  
		document.getElementById("uiViewDeleteAllStatus").value="off";
	  	document.getElementById("uiViewDeleteAllStatus").src="<? echo $var:btnCheckBoxOff ?>";
		jslDisable("uiViewDeleteAllStatus");
	    ` ` ` ?>
		for (i=0; i < MaxAccessList; i++)
  			jslDisable("uiViewMacAddress"+i);
	}
	else
	{
		document.getElementById("uiViewAccessEnable").value="on";
	  	document.getElementById("uiViewAccessEnable").src="<? echo $var:btnCheckBoxOn ?>";

		jslEnable("uiViewAccessType0");
		jslEnable("uiViewAccessType1");
		jslEnable("uiViewAddMac");
		jslEnable("uiViewAddMacButton");
	    <? if neq 0 <? multiquerycount ap:status/access_list?>  
	    `  jslEnable("uiViewDeleteAllStatus");
	    ` ` ` ?>

		for (i=0; i < MaxAccessList; i++)
  			jslEnable("uiViewMacAddress"+i);

		if (value == 1)
		{
			jslDoToggleRadio ("uiViewAccessType", 0, 2);
			document.getElementById("uiPostAccessType").value = 1;
		}
		else
		{
			jslDoToggleRadio ("uiViewAccessType", 1, 2);
			document.getElementById("uiPostAccessType").value = 2;
		}	
	}	
}

function uiDoAccessTypeChange(val)
{
	document.getElementById("uiPostAccessType").value = val;
}

function uiDoAddMacAddress()
{
	var message;

	jslEnable ("uiPostAddMacAddress");
	jslSetValue("uiPostAddMacAddress","uiViewAddMac");

    if(MaxAccessList==64) 
	{ 
       	alert("Maximum 64 number of MAC Address supported");
       	return; 
    } 
	
	message = checkMACAddress(document.getElementById("uiPostAddMacAddress").value);

	if(message!=true) 
	{
		alert ("Invalid MAC Address. Enter valid MAC Address separated by - ");	
		return;
	}

	jslDisable("uiPostDelMacAddress");
	document.getElementById("AccessError").value = "";
	jslFormSubmit("uiPostForm");
	return;
}

function uiDeleteMacAddress(arg)
{
	if(confirm("Delete MAC Address ?"))
	{
		jslEnable ("uiPostDelMacAddress");
		jslDisable("uiPostAddMacAddress");
		document.getElementById("uiPostDelMacAddress").value = arg;
		document.getElementById("AccessError").value = "";
		jslFormSubmit("uiPostForm");
	}
	return;
}

function uiViewDeleteAll(value)
{
   switch(value) {
   	case "on":
		     jslEnable("uiPostFlush");
		     break;
	case "off":
	default:
		     jslDisable("uiPostFlush");
		     break;
   }
}

function uiDoSave()
{	
	document.getElementById("AccessError").value = "";
	jslDisable("uiPostAddMacAddress");
	jslFormSubmit("uiPostForm");
	return;
}

function uiDoEnableAccess()
{
	var value = 1;

	jslDoToggleCheckBox ("uiViewAccessEnable");
	if (document.getElementById("uiViewAccessEnable").value == "off")
	{
		document.getElementById("uiPostAccessType").value = 0;
		value = 0;
	}
	uiSelectAccessType(value);
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}

</script>
