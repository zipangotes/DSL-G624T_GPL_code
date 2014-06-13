
<? setvariable var:contype dhcpc ?>

<script language="JavaScript">

<? include /usr/www_safe/html/js/val.js ?>
<? include /usr/www_safe/html/setup/wst.js ?>
<? if eq `1` `$var:HasDsl` `<? include /usr/www_safe/html/setup/pvc.js ?>` ` ` ?>
<? if eq `1` `$var:HasVlan` `<? include /usr/www_safe/html/setup/vlan.js ?>` ` ` ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{
  wstDoShowWanType("dhcpc");

  // 1. Load POSTFORM values
  <? if eq `1` `$var:HasDsl` `pvcDoOnLoad();` ` ` ?>
  <? if eq `1` `$var:HasVlan` `vlanDoOnLoad();` ` ` ?>
  jslPostToViewCheckBox("uiViewNatControl","uiPostNatControl");
  jslPostToViewCheckBox("uiViewFirewallControl","uiPostFirewallControl");
  jslPostToViewCheckBox("uiViewSetRoute","uiPostSetRoute");
  wstDoCheckFwNAT();
  <? if eq `1` `$var:HasDsl` `uiDoSetDHCPEncaps(document.getElementById("uiPostDHCPEncaps").value);
			
  <? if neq `new` `$var:mode`
     ` jslDisable("uiViewEncaps0","uiViewEncaps1");
     `  
     ` ` ?>
  // For not a new encapsid disable encaps
  <? if neq `<? query manager:command/encaps/new ?>` `$var:encapsid`
     ` jslDisable("uiPostDHCPEncaps");
     `  
     ` ` ?>
  ` ` ` ?>			
  // 2. Load ERROR values
  //doShowPageErrors();

  return true;
}
function dhcpcDoValidatePage()
{
  var msg="";
  return(msg);
}
function uiDoValidate()
{
  var msg="";
  var message="";

  if((msg=wstDoValidate())!="") {
  	message=msg;
  }
  if((msg=dhcpcDoValidatePage())!="") {
  	if(message!="") message=" and "+msg;
  	else message=msg;
  }
  <? if eq `1` `$var:HasDsl` `
   if((msg=pvcDoValidatePage())!="") {
  	if(message!="") message=" and "+msg;
  	else message=msg;
   }
  ` ` ` ?>
  <? if eq `1` `$var:HasVlan` 
  `if((msg=vlanDoValidatePage())!="") {
  	if(message!="") message=" and "+msg;
  	else message=msg;
   }
  ` ` ` ?>
  erlDoShowPageErrors();
  return jslPageErrorMsg(message);
}
function uiDoSave()
{
  <? if eq `1` `$var:HasDsl` `pvcDoSave();` ` ` ?>
  <? if eq `1` `$var:HasVlan` `vlanDoSave();` ` ` ?>
  wstDoSave();
  jslSetValue("uiPostConDesc","uiViewConDesc");
  jslViewToPostCheckBox("uiPostNatControl","uiViewNatControl");
  jslViewToPostCheckBox("uiPostFirewallControl","uiViewFirewallControl");
  jslViewToPostCheckBox("uiPostSetRoute","uiViewSetRoute");
  <? if eq `1` `$var:HasDsl` `document.getElementById("uiPostDHCPEncaps").value=uiDoGetDHCPEncaps();` ` ` ?>
  if(uiDoValidate()==true) jslFormSubmit("uiPostForm");
}
function uiDoCancel()
{
  jslGoTo(null,"home");
}
function uiDoRenew()
{
	jslEnable("uiPostRenew");
	jslDisable("uiPostSetRoute");
<? if eq `1` `$var:HasDsl` 
   `	jslDisable("uiPostDHCPEncaps");
	jslDisable("uiPostConnType");	
	jslDisable("uiPostPvcVpi");
	jslDisable("uiPostPvcVci");
	jslDisable("uiPostPvcQoS");
	jslDisable("uiPostPvcPcr");
	jslDisable("uiPostPvcScr");
	jslDisable("uiPostNatControl");
	jslDisable("uiPostFirewallControl");
	jslDisable("uiPostAutoPvcEnable");
   ` ` ` ?>
	uiDoSave();
}


function uiDoRelease()
{
	jslEnable("uiPostRelease");
	jslDisable("uiPostSetRoute");
<? if eq `1` `$var:HasDsl` 
    `	jslDisable("uiPostDHCPEncaps");
	jslDisable("uiPostConnType");	
	jslDisable("uiPostPvcVpi");
	jslDisable("uiPostPvcVci");
	jslDisable("uiPostPvcQoS");
	jslDisable("uiPostPvcPcr");
	jslDisable("uiPostPvcScr");
	jslDisable("uiPostNatControl");
	jslDisable("uiPostFirewallControl");
	jslDisable("uiPostAutoPvcEnable");
    ` ` ` ?>
       uiDoSave();
}

function uiDoGetDHCPEncaps()
{
	  if(document.getElementById("uiViewEncaps0").value=="on") // LLC 
	  	return "0"; // Bridged LLC
	  if(document.getElementById("uiViewEncaps1").value=="on") // VC 
	  	return "1"; // Bridged VC
}
function uiDoSetDHCPEncaps(arg)
{
  switch(arg)
  {
     case "1":
	  jslDoToggleRadio("uiViewEncaps",1,2);  // VC	  
     	break;
     case "0":
     default:
	  jslDoToggleRadio("uiViewEncaps",0,2);  // LLC	  
     	break;
    }  
}

</script>

