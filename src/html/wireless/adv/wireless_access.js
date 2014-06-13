
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/val.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/val.js"></script>' ?>

<? include /usr/www_safe/html/adv/wireless_mgmt.js ?>

var AccessList = new Array();
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
  var i = 0;
  <? multiquery ap:status/access_list "AccessList[i++]='$1';" ?>
  MaxAccessList = i;

  uiSelectAccessType(document.getElementById("uiPostAccessType").value);
}

function uiSelectAccessType(value)
{
	var i;
	
	if (value == 0)	
	{
  		document.getElementById("uiViewAccessEnable").value="off";
	  	document.getElementById("uiViewAccessEnable").src="/usr/www_safe/html/images/ti_check_off.gif";
		jslDisable("uiViewAccessType0");
		jslDisable("uiViewAccessType1");
		jslDisable("uiViewAddMac");
		jslDisable("uiViewAddMacButton");
	
		for (i=0; i < MaxAccessList; i++)
  			jslDisable("uiViewMacAddress"+i);
	}
	else
	{
		document.getElementById("uiViewAccessEnable").value="on";
	  	document.getElementById("uiViewAccessEnable").src="/usr/www_safe/html/images/ti_check_on.gif";

		jslEnable("uiViewAccessType0");
		jslEnable("uiViewAccessType1");
		jslEnable("uiViewAddMac");
		jslEnable("uiViewAddMacButton");

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

	
	message = checkMACAddress(document.getElementById("uiPostAddMacAddress").value);

	if(message!=true) 
	{
		alert ("Invalid MAC Address. Enter MAC Address separated by - ");	
		return;
	}

	jslDisable("uiPostDelMacAddress");
	jslFormSubmit("uiPostForm");
	return;
}

function uiDeleteMacAddress(arg)
{
	jslEnable ("uiPostDelMacAddress");
	jslDisable("uiPostAddMacAddress");
	document.getElementById("uiPostDelMacAddress").value = arg;
	jslFormSubmit("uiPostForm");
	return;
}

function uiDoSave()
{	
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

function uiCancel()
{
  jslGoTo(null,"home");
}

</script>
