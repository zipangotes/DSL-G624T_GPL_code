
<? setvariable var:contype static ?>
<script language="JavaScript">

<? include /usr/www_safe/html/js/val.js ?>
<? include /usr/www_safe/html/setup/wst.js ?>
<? if eq `1` `$var:HasDsl` `<? include /usr/www_safe/html/setup/pvc.js ?>` ` ` ?>
<? if eq `1` `$var:HasVlan` `<? include /usr/www_safe/html/setup/vlan.js ?>` ` ` ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{
  wstDoShowWanType("static");

  <? if eq `1` `$var:HasDsl` `pvcDoOnLoad();` ` ` ?>
  <? if eq `1` `$var:HasVlan` `vlanDoOnLoad();` ` ` ?>
  jslPostToViewCheckBox("uiViewNatControl","uiPostNatControl");
  jslPostToViewCheckBox("uiViewFirewallControl","uiPostFirewallControl");
  wstDoCheckFwNAT();

  <? if eq `1` `$var:HasDsl` `uiDoSetStaticEncaps(document.getElementById("uiPostStaticEncaps").value);` ` ` ?>
  jslSetValue("uiViewIpAddress","uiPostStaticIpAddress");
  jslSetValue("uiViewNetMask","uiPostStaticNetMask");
  jslSetValue("uiViewGateway","uiPostStaticGateway");
  <? if eq `1` `$var:HasVoice` `jslSetValue("uiViewConnGateway","uiPostStaticConnGateway");` ` ` ?>
  jslSetValue("uiViewDns1","uiPostStaticDns1");
  jslSetValue("uiViewDns2","uiPostStaticDns2");
  jslSetValue("uiViewDns3","uiPostStaticDns3");
  <? if eq `1` `$var:HasDsl` `
  <? if neq `new` `<? echo $var:mode ?>`
     ` jslDisable("uiViewEncaps0","uiViewEncaps1");
     `  
     ` ` ?>
  // For not a new encapsid disable encaps
  <? if neq `<? query manager:command/encaps/new ?>` `<? echo $var:encapsid ?>`
     ` jslDisable("uiPostStaticEncaps");
     `  
     ` ` ?>
  ` ` ` ?>
     
  //doShowPageErrors();
}
function staticDoValidatePage()
{
  var message;
  var value;
  var count=0;

  value = document.getElementById("uiPostStaticIpAddress").value;
  message = valDoValidateIp(value);
  if(message!=null) { erlDoAddError("bad","uiViewIpAddress","",value,message); count++; }

  value = document.getElementById("uiPostStaticNetMask").value;
  message = valDoValidateMask(value);
  if(message!=null) { erlDoAddError("bad","uiViewNetMask","",value,message);  count++; }

<? if eq `1` `$var:HasVoice`
 ` value = document.getElementById("uiPostStaticConnGateway").value;
  if(value!="")
  {
    message = (jslIsString("uiViewConnGateway")!=false)? valDoValidateIp(value): "";
    if(message!=null) { erlDoAddError("bad","uiViewConnGateway","",value,message); count++; }
  }
 ` ` ` ?>
  value = document.getElementById("uiPostStaticGateway").value;
  if(value!="")
  {
    message = (jslIsString("uiViewGateway")!=false)? valDoValidateIp(value): "";
    if(message!=null) { erlDoAddError("bad","uiViewGateway","",value,message); count++; }
  }

  value = document.getElementById("uiPostStaticDns1").value;
  if(value!="")
  {
    message = valDoValidateIp(value);
    if(message!=null) { erlDoAddError("bad","uiViewDns1","",value,message);  count++; }
  }

  value = document.getElementById("uiPostStaticDns2").value;
  if(value!="")
  {
    message = valDoValidateIp(value);
    if(message!=null) { erlDoAddError("bad","uiViewDns2","",value,message);  count++; }
  }

  value = document.getElementById("uiPostStaticDns3").value;
  if(value!="")
  {
    message = valDoValidateIp(value);
    if(message!=null) { erlDoAddError("bad","uiViewDns3","",value,message);  count++; }
  }

  if(count!=0) message="Static";
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
  if((msg=staticDoValidatePage())!="") {
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
  ` if((msg=vlanDoValidatePage())!="") {
  	if(message!="") message=" and "+msg;
  	else message=msg;
    }
  ` ` ` ?>
  erlDoShowPageErrors();
  return jslPageErrorMsg(message);
}

function uiDoBridged()
{
      	jslDoToggleRadio("uiViewBridgeRoute",0,2);
    <? if eq `1` `$var:HasVlan`  	
    `	 document.getElementById("uiViewVlanEnable").value="disable";
        document.getElementById("uiViewVlanId").value="0"; 
        document.getElementById("uiViewPbits").value="0"; 
        jslEnable("uiViewVlanEnable","uiViewVlanId","uiPostVlanId","uiViewPbits");
        <? if eq `1` `$var:HasDsl` `jslDisable("uiViewPvc","uiPostPvc");` ` ` ?>
  ` `jslDisable("uiViewPvc","uiPostPvc");` ?>
}

function uiDoRouted()
{
    <? if eq `1` `$var:HasVlan`
    `	document.getElementById("uiViewVlanEnable").value="disable";
       document.getElementById("uiViewVlanId").value="0"; 
        document.getElementById("uiViewPbits").value="0"; 
       jslDisable("uiViewVlanEnable","uiViewVlanId","uiPostVlanId","uiViewPbits");
    ` ` ` ?>
       jslDoToggleRadio("uiViewBridgeRoute",1,2);
    <? if eq `1` `$var:HasDsl` `jslDisable("uiViewPvc","uiPostPvc");` ` ` ?>
}

function uiDoSave()
{
  <? if eq `1` `$var:HasDsl` `pvcDoSave();` ` ` ?>
  <? if eq `1` `$var:HasVlan` `vlanDoSave();` ` ` ?>
  wstDoSave();
  jslSetValue("uiPostStaticIpAddress","uiViewIpAddress");
  jslSetValue("uiPostStaticNetMask","uiViewNetMask");
  <? if eq `1` `$var:HasVoice` `jslSetValue("uiPostStaticConnGateway","uiViewConnGateway");` ` ` ?>
  jslSetValue("uiPostStaticDns1","uiViewDns1");
  jslSetValue("uiPostStaticDns2","uiViewDns2");
  jslSetValue("uiPostStaticDns3","uiViewDns3");
  <? if eq `1` `$var:HasDsl` `document.getElementById("uiPostStaticEncaps").value=uiDoGetStaticEncaps();` ` ` ?>
  jslSetValue("uiPostConDesc","uiViewConDesc");
  jslViewToPostCheckBox("uiPostNatControl","uiViewNatControl");
  jslViewToPostCheckBox("uiPostFirewallControl","uiViewFirewallControl");
  if(uiDoValidate()==true) {
	if(document.getElementById("uiPostStaticGateway").value != document.getElementById("uiViewGateway").value) {
	    if (confirm("You may lose any existing connections in case of incorrect default gateway. Are you sure you want to change the default gateway?")) {
  		jslSetValue("uiPostStaticGateway","uiViewGateway");
  	    } 
  	}
  	jslFormSubmit("uiPostForm");
  }
}
function uiDoGetStaticEncaps()
{
  if(document.getElementById("uiViewBridgeRoute0").value=="on") // Bridged 
	  if(document.getElementById("uiViewEncaps0").value=="on") // LLC 
	  	return "0"; // Bridged LLC
  if(document.getElementById("uiViewBridgeRoute0").value=="on") // Bridged 
	  if(document.getElementById("uiViewEncaps1").value=="on") // VC 
	  	return "1"; // Bridged VC

  if(document.getElementById("uiViewBridgeRoute1").value=="on") // routed
	  if(document.getElementById("uiViewEncaps0").value=="on") // LLC 
	  	return "3"; // Routed LLC
  if(document.getElementById("uiViewBridgeRoute1").value=="on") // routed
	  if(document.getElementById("uiViewEncaps1").value=="on") // VC 
	  	return "4"; // Routed VC
}
function uiDoSetStaticEncaps(arg)
{
  switch(arg)
  {
     case "3":
	  jslDoToggleRadio("uiViewBridgeRoute",1,2);   // Routed 
	  jslDoToggleRadio("uiViewEncaps",0,2);  // LLC	  
     	break;
     case "4":
	  jslDoToggleRadio("uiViewBridgeRoute",1,2);  // Routed
	  jslDoToggleRadio("uiViewEncaps",1,2);  // VC	  
     	break;
     case "1":
	  jslDoToggleRadio("uiViewBridgeRoute",0,2);  // Bridged
	  jslDoToggleRadio("uiViewEncaps",1,2);  // VC	  
     	break;
     case "0":
     default:
	  jslDoToggleRadio("uiViewBridgeRoute",0,2);  // Bridged
	  jslDoToggleRadio("uiViewEncaps",0,2);  // LLC	  
     	break;
    }  
}
function uiDoCancel()
{
  jslGoTo(null,"home");
}
</script>
