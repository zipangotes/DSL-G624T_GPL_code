
<? setvariable var:contype clip ?>

<script language="JavaScript">

<? include /usr/www_safe/html/js/val.js ?>
<? include /usr/www_safe/html/setup/wst.js ?>
<? include /usr/www_safe/html/setup/pvc.js ?>
<? if eq `1` `$var:HasVlan` `<? include /usr/www_safe/html/setup/vlan.js ?>` ` ` ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{
  wstDoShowWanType("clip");

  pvcDoOnLoad();
  <? if eq `1` `$var:HasVlan` `vlanDoOnLoad();` ` ` ?>
  jslPostToViewCheckBox("uiViewNatControl","uiPostNatControl");
  jslPostToViewCheckBox("uiViewFirewallControl","uiPostFirewallControl");
  wstDoCheckFwNAT();			

  //doShowPageErrors();
  jslSetValue("uiViewIpAddress","uiPostIpAddress");
  jslSetValue("uiViewNetMask","uiPostMask");
  jslSetValue("uiViewATMARPGateway","uiPostATMARPGateway");
  jslSetValue("uiViewDefaultGateway","uiPostDefaultGateway");

  <? if eq `1` `$var:HasVlan` `
  // default vlan is enable but for clip and ppoa default make it disable 
  document.getElementById("uiPostVlanEnable").value="disable";
  jslDoShowComboBox("uiViewVlanEnable", "uiPostVlanEnable");             
  jslDisable("uiViewVlanEnable","uiViewVlanId","uiViewPvc");
  jslDisable("uiPostVlanId");
  jslDisable("uiPostPbits","uiViewPbits");
  ` `jslDisable("uiViewPvc"); ` ?>
  
  <? if neq `new` `$var:mode`
    `  jslDisable("uiViewWanType");
    `  
    ` ` ?>
  
  return true;
}
function clipDoValidatePage()
{
  var message;
  var value;
  var count=0;

  value = document.getElementById("uiPostIpAddress").value;
  message = valDoValidateIp(value);
  if(message!=null) { erlDoAddError("bad","uiViewIpAddress","",value,message); count++; }

  value = document.getElementById("uiPostMask").value;
  message = valDoValidateMask(value);
  if(message!=null) { erlDoAddError("bad","uiViewNetMask","",value,message);  count++; }

  value = document.getElementById("uiPostATMARPGateway").value;
  if(value!="")
  {
    message = (jslIsString("uiViewATMARPGateway")!=false)? valDoValidateIp(value): "";
    if(message!=null) { erlDoAddError("bad","uiViewATMARPGateway","",value,message); count++; }
  }

  value = document.getElementById("uiPostDefaultGateway").value;
  if(value!="")
  {
    message = valDoValidateIp(value);
    if(message!=null) { erlDoAddError("bad","uiViewDefaultGateway","",value,message);  count++; }
  }

  if(count!=0) message="CLIP";
   else message="";
  return message;
}
function uiDoValidate()
{
  var msg="";
  var message="";

  if((msg=wstDoValidate())!="") {
  	message=msg;
  }
  if((msg=clipDoValidatePage())!="") {
  	if(message!="") message=" and "+msg;
  	else message=msg;
  }
   if((msg=pvcDoValidatePage())!="") {
  	if(message!="") message=" and "+msg;
  	else message=msg;
   }
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
  pvcDoSave();
  <? if eq `1` `$var:HasVlan` `vlanDoSave();` ` ` ?>
  wstDoSave();
  jslSetValue("uiPostConDesc","uiViewConDesc");
  jslViewToPostCheckBox("uiPostNatControl","uiViewNatControl");
  jslViewToPostCheckBox("uiPostFirewallControl","uiViewFirewallControl");
  jslSetValue("uiPostIpAddress","uiViewIpAddress");
  jslSetValue("uiPostMask","uiViewNetMask");
  jslSetValue("uiPostStaticIpAddress","uiViewIpAddress");
  jslSetValue("uiPostStaticMask","uiViewNetMask");
  jslSetValue("uiPostATMARPGateway","uiViewATMARPGateway");
  if(uiDoValidate()==true) {
	if(document.getElementById("uiPostDefaultGateway").value != document.getElementById("uiViewDefaultGateway").value) {
	    if (confirm("You may lose any existing connections in case of incorrect default gateway. Are you sure you want to change the default gateway?")) {
               jslSetValue("uiPostDefaultGateway","uiViewDefaultGateway");
  	    } 
  	}
  	jslFormSubmit("uiPostForm");
  }
}
function uiDoCancel()
{
  jslGoTo(null,"home");
}
</script>

