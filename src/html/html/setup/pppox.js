
<script language="JavaScript">

<? include /usr/www_safe/html/js/val.js ?>
<? include /usr/www_safe/html/setup/wst.js ?>
<? if eq `1` `$var:HasDsl` `<? include /usr/www_safe/html/setup/pvc.js ?>` `` ?>
<? if eq `1` `$var:HasVlan` `<? include /usr/www_safe/html/setup/vlan.js ?>` ` ` ?>
<? include /usr/www_safe/html/setup/ppp.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{

  wstDoShowWanType("<? echo $var:contype ?>");
  pppDoOnLoad();
  <? if eq `1` `$var:HasDsl` `pvcDoOnLoad();` `` ?>
  <? if eq `1` `$var:HasVlan` `vlanDoOnLoad();` ` ` ?>
  <? if eq pppoa $var:contype 
   ` uiDoSetEncapsulation(document.getElementById("uiPostPppEncaps").value);
      <? if eq `1` `$var:HasVlan` 
   	` // default vlan is enable but for clip and ppoa default make it disable 
        document.getElementById("uiPostVlanEnable").value="disable";
        jslDoShowComboBox("uiViewVlanEnable", "uiPostVlanEnable");             
       jslDisable("uiViewVlanEnable","uiViewVlanId","uiViewPvc");
        jslDisable("uiPostVlanId");
        jslDisable("uiPostPbits","uiViewPbits");
         <? if neq `new` `$var:mode`
         ` jslDisable("uiViewEncaps0","uiViewEncaps1");
            jslDisable("uiViewWanType");
         `  
         ` ` ?>
        ` `jslDisable("uiViewPvc");` ?>
      `
      ` ` ?>
  jslPostToViewCheckBox("uiViewNatControl","uiPostNatControl");
  jslPostToViewCheckBox("uiViewFirewallControl","uiPostFirewallControl");
  wstDoCheckFwNAT();
  jslDoShowComboBox("uiViewPppUnLan", "uiPostPppUnLan");             
  jslPostToViewCheckBox("uiViewPppUnNum","uiPostPppUnNum");
  uiPPPUnnum(document.getElementById("uiViewPppUnNum").value);
  //doShowPageErrors();
  return true;
}
<? if eq pppoa $var:contype
'
function uiDoGetEncapsulation()
{
  if(document.getElementById("uiViewEncaps0").value=="on") return "0";
  if(document.getElementById("uiViewEncaps1").value=="on") return "1";
}
function uiDoSetEncapsulation(arg)
{
   if(arg=="0") jslDoToggleRadio("uiViewEncaps",0,2); 
   if(arg=="1") jslDoToggleRadio("uiViewEncaps",1,2);
}
' ?>

function uiDoSelectLAN(value)
{
	document.getElementById("uiPostPppUnLan").value=value;
}

function uiPPPUnnum(value)
{
    switch(value) {
    	case 'on': 
			document.getElementById("uiPostPppDefRoute").value="1";
			jslPostToViewCheckBox("uiViewSetRoute","uiPostPppDefRoute");
			document.getElementById("uiPostNatControl").value="0";			
			jslPostToViewCheckBox("uiViewNatControl","uiPostNatControl");
    		       jslDisable("uiViewNatControl","uiViewSetRoute");
    		       jslEnable("uiViewPppUnLan");
    			break;
    	case 'off':
    	default:
    		       jslEnable("uiViewNatControl","uiViewSetRoute");
    		       jslDisable("uiViewPppUnLan");
    			break;
    }

}

function uiDoValidate()
{
  var msg="";
  var message="";
  var value;

  if((msg=wstDoValidate())!="") {
  	message=msg;
  }
  if((msg=pppDoValidatePage())!="") {
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
  jslViewToPostCheckBox("uiPostNatControl","uiViewNatControl");
  jslViewToPostCheckBox("uiPostFirewallControl","uiViewFirewallControl");
  <? if eq pppoa $var:contype 'document.getElementById("uiPostPppEncaps").value=uiDoGetEncapsulation();' '' ?>
  <? if eq `1` `$var:HasDsl` `pvcDoSave();` `` ?>
  <? if eq `1` `$var:HasVlan` `vlanDoSave();` `` ?> 
  wstDoSave();
  pppDoOnSave();
  jslSetValue("uiPostConDesc","uiViewConDesc");
  jslViewToPostCheckBox("uiPostPppUnNum","uiViewPppUnNum");
  if(uiDoValidate()==true) jslFormSubmit("uiPostForm");
}
function uiDoCancel()
{
  jslGoTo(null,"home");
}

</script>
