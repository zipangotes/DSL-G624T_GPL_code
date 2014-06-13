
function pppDoOnLoad()
{
  jslSetValue("uiViewUserName","uiPostPppUsername");
  jslSetValue("uiViewPassword","uiPostPppPassword");
  jslSetValue("uiViewIdleTime","uiPostPppIdleTimeout");
  jslSetValue("uiViewMRU","uiPostPppMRU");
  jslSetValue("uiViewKeepAlive","uiPostPppKeepAlive");
  pppSetAuth();
  pppDoSetOnDemand(document.getElementById("uiPostPppMode").value);
  pppDoSetDefRoute(document.getElementById("uiPostPppDefRoute").value);
  <? if eq pppoe $var:contype 'pppDoSetEnforceMRU(document.getElementById("uiPostEnforceMRU").value);' '' ?>
  pppDoSetDebug(document.getElementById("uiPostPppDebug").value);
  <? if eq `new` `<? echo $var:mode ?>` 
  	`jslDisable("uiViewConnectButton","uiViewDisconnectButton");` ` ` ?>
}

function pppSetAuth()
{
	switch(document.getElementById("uiPostPppAuthType").value) {
		case "CHAP":
					jslDoToggleRadio("uiViewAuth",1,3);
					
					break;
		case "PAP":
					jslDoToggleRadio("uiViewAuth",2,3);
					break;
		case "Auto":
		default:
					jslDoToggleRadio("uiViewAuth",0,3);
					break;					
       }			
}

function uiDoChangeAuth(type)
{
	document.getElementById("uiPostPppAuthType").value=type;
}

function pppDoGetOnDemand()
{
  if(document.getElementById("uiViewOnDemand").value=="off") return "0";
  else return "1";
}

function pppDoSetOnDemand(ondemand)
{
  var checkbox = document.getElementById('uiViewOnDemand');
  switch(ondemand)
  {  
    case "1":
     checkbox.value="off";
	 jslDoToggleCheckBox('uiViewOnDemand');
	 jslEnable("uiViewIdleTime");
	 jslDisable("uiViewKeepAlive");
    break;
    case "0":
    default:
     checkbox.value="on";
	 jslDoToggleCheckBox('uiViewOnDemand');
	 jslDisable("uiViewIdleTime");
	 jslEnable("uiViewKeepAlive");
    break;
  }
}

function pppDoChangeOnDemand(arg)
{  
  if(document.getElementById(arg).value=="off") pppDoSetOnDemand("1");
    else pppDoSetOnDemand("0");
}

function pppDoChangeEnforceMRU(arg)
{  
  if(document.getElementById(arg).value=="off") {
  	document.getElementById("uiPostEnforceMRU").value="0";
  }
  else {
  	document.getElementById("uiPostEnforceMRU").value="1";
  }
}

function pppDoSetEnforceMRU(value)
{  
  switch (value) {
	case "1":
	  	document.getElementById("uiViewEnforceMRU").value="on";
		document.getElementById("uiViewEnforceMRU").src="<? echo $var:btnCheckBoxOn ?>";
		break;

	case "0":
	default:
	  	document.getElementById("uiPostEnforceMRU").value="0";
		document.getElementById("uiViewEnforceMRU").value="off";
		document.getElementById("uiViewEnforceMRU").src="<? echo $var:btnCheckBoxOff ?>";
		break;
  }
}

function pppDoGetEnforceMRU()
{
  if(document.getElementById("uiViewEnforceMRU").value=="off") return "0";
  else return "1";
}


function pppDoChangeDebug(arg)
{  
  if(document.getElementById(arg).value=="off") {
  	document.getElementById("uiPostPppDebug").value="0";
  	document.getElementById("uiPostDefaultLevel").value="notice";
  	document.getElementById("uiPostUserLevel").value="notice";
  }
  else {
  	document.getElementById("uiPostPppDebug").value="1";
  	document.getElementById("uiPostDefaultLevel").value="debug";
  	document.getElementById("uiPostUserLevel").value="debug";
  }
}

function pppDoSetDebug(value)
{  
  switch (value) {
	case "1":
	  	document.getElementById("uiViewDebug").value="on";
		document.getElementById("uiViewDebug").src="<? echo $var:btnCheckBoxOn ?>";
		break;

	case "0":
	default:
	  	document.getElementById("uiPostPppDebug").value="0";
		document.getElementById("uiViewDebug").value="off";
		document.getElementById("uiViewDebug").src="<? echo $var:btnCheckBoxOff ?>";
		break;
  }
}

function pppDoGetDebug()
{
  if(document.getElementById("uiViewDebug").value=="off") return "0";
  else return "1";
}

function pppDoSetDefRoute(arg)
{
  if(arg=="0") document.getElementById("uiViewSetRoute").value="on";
  else document.getElementById("uiViewSetRoute").value="off";
  jslDoToggleCheckBox("uiViewSetRoute");
}

function pppDoGetDefRoute()
{
  if(document.getElementById("uiViewSetRoute").value=="on") return "1";
  return "0";
}

function pppDoOnSave()
{
  document.getElementById("uiPostPppMode").value=pppDoGetOnDemand();
  document.getElementById("uiPostPppDefRoute").value=pppDoGetDefRoute();
  <? if eq pppoe $var:contype 'document.getElementById("uiPostEnforceMRU").value=pppDoGetEnforceMRU();' '' ?>
  document.getElementById("uiPostPppDebug").value=pppDoGetDebug();
  jslSetValue("uiPostPppUsername","uiViewUserName");
  jslSetValue("uiPostPppPassword","uiViewPassword");
  jslSetValue("uiPostPppIdleTimeout","uiViewIdleTime");
  jslSetValue("uiPostPppMRU","uiViewMRU");
  jslSetValue("uiPostPppKeepAlive","uiViewKeepAlive");
}

function pppDoValidatePage()
{
  var message;
  var value;
  var maxmru = 1500;
  //var maxidle = 100;
  var count=0;

  //value = document.getElementById("uiPostPppUsername").value;
  //message = valDoValidateName(value);
  //if(message!=null) { erlDoAddError("bad","uiViewUserName","",value,message); count++; }

  //value = document.getElementById("uiPostPppPassword").value;
  //message = valDoValidateName(value);
  //if(message!=null) { erlDoAddError("bad","uiViewPassword","",value,message);  count++; }

  //value = document.getElementById("uiPostPppIdleTimeout").value;
  //message = valDoValidateInteger(value);
  //if(message!=null) { erlDoAddError("bad","uiViewIdleTime","",value,message);  count++; }
  // else if(value > maxidle) { erlDoAddError("bad","uiViewIdleTime","",value,"Idle Timeout must be less than"+maxidle); count++; }

  
  value = document.getElementById("uiPostPppMRU").value;
  message = valDoValidateInteger(value);
  if(message!=null) { erlDoAddError("bad","uiViewMRU","",value,message); count++; }
   else if(value > maxmru) { erlDoAddError("bad","uiViewMRU","",value,"MRU must be less than"+maxmru); count++; }

  value = document.getElementById("uiPostPppKeepAlive").value;
  message = valDoValidateInteger(value);
  if(message!=null) { erlDoAddError("bad","uiViewKeepAlive","",value,message); count++; }

  if(count!=0) message="PPP";
   else message="";

  return message;
}

function uiDoConnect()
{
       if(document.getElementById("uiViewOnDemand").value=="on") {
          alert("In 'On Demand' mode connection gets established on accessing the WAN side.");
          return false;
	}
       jslEnable("uiPostPppConnect");
	jslDisable("uiPostPppUsername","uiPostPppPassword",
		          "uiPostPppIdleTimeout","uiPostPppMRU",
		          "uiPostPppDebug","uiPostPppMode",
		          "uiPostPppKeepAlive","uiPostPppDefRoute",
		          "uiPostPppAuthType","uiPostPppUnNum","uiPostPppUnLan");
	<? if eq `1` `$var:HasDsl`
		`jslDisable("uiPostPvcVpi","uiPostPvcVci","uiPostPvcQoS","uiPostPvcPcr",
		          "uiPostPvcScr","uiPostPvcCdvt","uiPostPvcMbs");
		` ` ` ?>
			
	jslEnable( "uiPostConDesc","uiPostNatControl","uiPostFirewallControl","uiPostFwanNATState");
       jslFormSubmit("uiPostForm");
}

function uiDoDisconnect()	
{
       if(document.getElementById("uiViewOnDemand").value=="on") {
          alert("In 'On Demand' mode disconnect is based on idle timeout.");
          return false;
	}
	jslEnable("uiPostPppDisconnect");
	jslDisable("uiPostPppUsername","uiPostPppPassword",
		          "uiPostPppIdleTimeout","uiPostPppMRU",
		          "uiPostPppDebug","uiPostPppMode",
		          "uiPostPppKeepAlive","uiPostPppDefRoute",
		          "uiPostPppAuthType","uiPostPppUnNum","uiPostPppUnLan");
	<? if eq `1` `$var:HasDsl`
		` jslDisable("uiPostPvcVpi","uiPostPvcVci","uiPostPvcQoS","uiPostPvcPcr",
		          "uiPostPvcScr","uiPostPvcCdvt","uiPostPvcMbs");
		` ` ` ?>
	jslDisable("uiPostConDesc","uiPostNatControl","uiPostFirewallControl","uiPostFwanNATState");
	jslFormSubmit("uiPostForm");
}

