function vlanDoOnLoad()
{

  <? if neq `` `$var:sharing` `document.getElementById("uiPostVlanEnable").value="$var:sharing"` ` ` ?>;
  uiDoVlan(document.getElementById("uiPostVlanEnable").value);

  jslDoShowComboBox("uiViewVlanEnable", "uiPostVlanEnable");             
  jslSetValue("uiViewVlanId","uiPostVlanId");
  jslDoShowComboBox("uiViewPbits","uiPostPbits");

  // For not a new connection disable all vlan settings.
  <? if neq `new` `<? echo $var:mode ?>`
   `   jslDisable("uiViewVlanEnable");
        jslDisable("uiViewVlanId","uiViewPbits");
        jslDisable("uiPostVlanEnable");
        jslDisable("uiPostVlanId","uiPostPbits");
     `  
     ` ` ?>

   
}

function uiDoVlan(value)
{
      switch(value)
      	{
	     case "vlan": //VLAN
                           jslEnable("uiViewVlanId");
                           jslEnable("uiPostVlanId");
                           jslEnable("uiViewPbits");
                           jslEnable("uiPostPbits");
                           <? if eq `new` `<? echo $var:mode ?>` `<? if eq `1` `$var:HasDsl` `jslEnable("uiViewPvc");` ` ` ?>` ` ` ?>
	     			break;
	     case "enable": //Enable sharing
	     		      document.getElementById("uiViewVlanId").value=0;
	     		      document.getElementById("uiPostVlanId").value=0;
                           jslDisable("uiViewVlanId");
                           jslDisable("uiViewPbits");
                           jslDisable("uiPostPbits");
                           <? if eq `new` `<? echo $var:mode ?>` `<? if eq `1` `$var:HasDsl` `jslEnable("uiViewPvc");` ` ` ?>` ` ` ?>
	     			break;	     
	     case "disable": //Disable sharing
	     default:	
                            jslDisable("uiViewVlanId");
                            jslDisable("uiPostVlanId");
                            jslDisable("uiViewPbits");
                            jslDisable("uiPostPbits");
                           <? if eq `new` `<? echo $var:mode ?>` `<? if eq `1` `$var:HasDsl` `jslDisable("uiViewPvc");` ` ` ?>` ` ` ?>
	     			break;
	}
}

function vlanDoValidatePage()
{
  var message;
  var value;
  var vlanPageErrorCount=0;

  value = document.getElementById("uiPostVlanId").value;
  message = valDoValidateInteger(value);
  if(message!=null) { erlDoAddError("bad","uiViewVlanId","",value,message); vlanPageErrorCount++; }
  else {
     if(document.getElementById("uiViewVlanEnable").value=="vlan") { // sharing has value of 2 i.e. VLAN	
  	if(Number(value) > 1024) { erlDoAddError("bad","uiViewVlanId","",value,"VlanId must be between 1 to 1024"); vlanPageErrorCount++; }
  	if(Number(value) < 1) { erlDoAddError("bad","uiViewVlanId","",value,"VlanId must be between 1 to 1024"); vlanPageErrorCount++; }
  }
  }
  if(vlanPageErrorCount!=0) message="VLAN";
  else message="";
  return message;

}


function vlanDoSave()
{
  jslSetValue("uiPostVlanEnable","uiViewVlanEnable");
  jslSetValue("uiPostVlanId","uiViewVlanId");
  jslSetValue("uiPostPbits","uiViewPbits");
}

