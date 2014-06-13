
<? setvariable var:contype bridge ?>

<script language="JavaScript">

<? include /usr/www_safe/html/js/val.js ?>
<? include /usr/www_safe/html/setup/wst.js ?>
<? if eq `1` `$var:HasDsl` `<? include /usr/www_safe/html/setup/pvc.js ?>` ` ` ?>
<? if eq `1` `$var:HasVlan` `<? include /usr/www_safe/html/setup/vlan.js ?>` ` ` ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{
  wstDoShowWanType("bridge");
<? if eq `1` `$var:HasDsl`
` pvcDoOnLoad();
  uiDoSetBridgeEncaps(document.getElementById("uiPostBridgeEncaps").value);
   <? if neq `new` `$var:mode`
     ` jslDisable("uiViewEncaps0","uiViewEncaps1");
     `  
     ` ` ?>
  <? if eq `1` `$var:HasVlan` `vlanDoOnLoad();` ` ` ?>
     // For not a new encapsid disable encaps
   <? if neq `<? query manager:command/encaps/new ?>` `$var:encapsid`
     ` jslDisable("uiPostBridgeEncaps");
     `  
     ` ` ?>
  ` ` ` ?> 
  <? if eq `new` `$var:mode` ` ` `jslSetValue("uiViewLANBridge","uiPostLANBridge");` ?>
  //doShowPageErrors();
  return true;
}
function brgDoValidatePage()
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
  if((msg=brgDoValidatePage())!="") {
  	if(message!="") message=" and "+msg;
  	else message=msg;
  }
  <? if eq `1` `$var:HasDsl` `
   if((msg=pvcDoValidatePage())!="") {
  	if(message!="") message=" and "+msg;
  	else message=msg;
   }
  ` ` ` ?>
  <? if eq `1` `$var:HasVlan` `
  if((msg=vlanDoValidatePage())!="") {
  	if(message!="") message=" and "+msg;
  	else message=msg;
  }
  ` ` ` ?>
  erlDoShowPageErrors();
  return jslPageErrorMsg(message);
}
function uiDoSave()
{
  <? if eq `1` `$var:HasDsl` `pvcDoSave();` `` ?>
  <? if eq `1` `$var:HasVlan` `vlanDoSave();` ` ` ?>
  wstDoSave();
  jslSetValue("uiPostConDesc","uiViewConDesc");
  uiBridgeLAN(document.getElementById("uiViewLANBridge").value);
  <? if eq `1` `$var:HasDsl` `document.getElementById("uiPostBridgeEncaps").value=uiDoGetBridgeEncaps();` ` ` ?>
  if(uiDoValidate()==true) {
  	<? if eq 1 <? query <? query manager:status/$var:langrp/dhcps ?>:settings/state ?> 
  		`alert("DHCP Server has been detected enable. It is recommended for bridge connection to disable DHCP server.");` ?>
  	jslFormSubmit("uiPostForm");
       setTimeout("uiDoNextLoc()", 15000);
  }
}

function uiDoNextLoc()
{
   document.location.href="http://"+document.getElementById("uiPostIPAddr").value+"/cgi-bin/webcm?getpage=/usr/www_safe/html/defs/<? echo $var:style ?>/menus/<? echo $var:main ?>.html&var:style=<? echo $var:style ?>&var:main=<? echo $var:main ?>&var:menu=setup&var:menutitle=Setup&var:pagename=bridge&var:pagetitle=New%20Connection&var:conid=<? echo $var:conid ?>&var:muxid=<? echo $var:muxid ?>&var:encapsid=<? echo $var:encapsid ?>";
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}
function uiDoGetBridgeEncaps()
{
	  if(document.getElementById("uiViewEncaps0").value=="on") // LLC 
	  	return "0"; // Bridged LLC
	  if(document.getElementById("uiViewEncaps1").value=="on") // VC 
	  	return "1"; // Bridged VC
}
function uiDoSetBridgeEncaps(arg)
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

function uiBridgeLAN(value)
{
    document.getElementById("uiPostLANBridgeCmd").value="<? echo $var:conid ?>"+"|"+value;
}

</script>

