
<script language="JavaScript">

<? include /usr/www_safe/html/js/ip.js ?>
<? include /usr/www_safe/html/setup/pvc.js ?>
<? if eq `1` `$var:HasVlan` `<? include /usr/www_safe/html/setup/vlan.js ?>` ` ` ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{
  var dhcpSrvrsStates = "<?  multiquery logic:status/instances/dhcps '<? query $01:settings/state ?> ' ?>";
  var dhcpStates = dhcpSrvrsStates.split(" ");

  jslSetValue("uiViewStartIP","uiPostStartIP");
  jslSetValue("uiViewEndIP","uiPostEndIP");
  jslSetValue("uiViewLease","uiPostLease");
  jslSetValue("uiViewRelayIP","uiPostRelayIP");

  uiDoSrvRelayStatus();
}

function uiDoSrvRelayStatus()
{
	if(document.getElementById("uiViewServerState").value=="1") {
		if(document.getElementById("uiViewRelayState").value=="0") {
			jslDoToggleRadio('uiViewSrvRelayStatus',1,3); //server on
			uiServerStatus(document.getElementById("uiViewSrvRelayStatus1"));			
		}
		else {
			jslDoToggleRadio('uiViewSrvRelayStatus',0,3); // none
			uiNoneStatus(document.getElementById("uiViewSrvRelayStatus0"));
		}
	}
	else {
		if(document.getElementById("uiViewRelayState").value=="1") {
			jslDoToggleRadio('uiViewSrvRelayStatus',2,3); // relay on
			uiRelayStatus(document.getElementById("uiViewSrvRelayStatus2"));
		}
		else {
			jslDoToggleRadio('uiViewSrvRelayStatus',0,3); // none
			uiNoneStatus(document.getElementById("uiViewSrvRelayStatus0"));
		}
	}
}


function uiDoSave()
{
var Element;
var ElementValue;
  // Validate data in visual elements and indicate errors if any

  Element = document.getElementById("uiViewStartIP");
  if(Element.disabled==false)
  	if(doValidateIP(Element.value)!=true) return;
  	
  Element = document.getElementById("uiViewEndIP");
  if(Element.disabled==false)
	if(doValidateIP(Element.value)!=true) return;

  Element = document.getElementById("uiViewRelayIP");
  if(Element.disabled==false)
	if(doValidateIP(Element.value)!=true) return;
  

  Element = document.getElementById("uiViewLease");
  ElementValue = Element.value;
  if(ElementValue.match("[^0-9]") != null) { 
 		alert("Lease Time needs to be an positive integer"); 
      		return false; 
  }

  // Post values of all the edit boxes
  jslSetValue("uiPostStartIP","uiViewStartIP");
  jslSetValue("uiPostEndIP","uiViewEndIP");
  jslSetValue("uiPostLease","uiViewLease");
  jslSetValue("uiPostRelayIP","uiViewRelayIP");

  // Submit and Refresh
  document.getElementById("uiPostForm").submit();
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}

function uiNoneStatus(context)
{
	if(context.value=="on") {
		jslDisable("uiViewStartIP");
		jslDisable("uiViewEndIP");
		jslDisable("uiViewLease");
		
		jslDisable("uiViewStartIPLabel");
		jslDisable("uiViewEndIPLabel");
		jslDisable("uiViewLeaseLabel");
		
		jslDisable("uiViewRelayIP");
		jslDisable("uiViewRelayIPLabel");

		jslEnable("uiPostServerFirst");
		jslEnable("uiPostRelaySecond");
		jslDisable("uiPostRelayFirst");
		jslDisable("uiPostServerSecond");
		
	       document.getElementById("uiPostServerFirst").value="0";
	       document.getElementById("uiPostRelaySecond").value="0";

	}
}

function uiServerStatus(context)
{
	if(context.value=="on") {
		jslEnable("uiViewStartIP");
		jslEnable("uiViewEndIP");
		jslEnable("uiViewLease");
		
		jslEnable("uiViewStartIPLabel");
		jslEnable("uiViewEndIPLabel");
		jslEnable("uiViewLeaseLabel");
		
		jslDisable("uiViewRelayIP");
		jslDisable("uiViewRelayIPLabel");

		jslDisable("uiPostServerFirst");
		jslDisable("uiPostRelaySecond");
		jslEnable("uiPostRelayFirst");
		jslEnable("uiPostServerSecond");

	       document.getElementById("uiPostRelayFirst").value="0";
	       document.getElementById("uiPostServerSecond").value="1";

	}
}
function uiRelayStatus(context)
{
	if(context.value=="on") {
		jslDisable("uiViewStartIP");
		jslDisable("uiViewEndIP");
		jslDisable("uiViewLease");
		
		jslDisable("uiViewStartIPLabel");
		jslDisable("uiViewEndIPLabel");
		jslDisable("uiViewLeaseLabel");
		
		jslEnable("uiViewRelayIP");
		jslEnable("uiViewRelayIPLabel");

		jslEnable("uiPostServerFirst");
		jslEnable("uiPostRelaySecond");
		jslDisable("uiPostRelayFirst");
		jslDisable("uiPostServerSecond");

	       document.getElementById("uiPostServerFirst").value="0";
	       document.getElementById("uiPostRelaySecond").value="1";

	}
}


</script>

