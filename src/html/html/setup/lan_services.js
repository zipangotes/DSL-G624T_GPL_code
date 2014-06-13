
<script language="JavaScript">

<? include /usr/www_safe/html/js/val.js ?>
<? include /usr/www_safe/html/setup/pvc.js ?>
<? if eq `1` `$var:HasVlan` `<? include /usr/www_safe/html/setup/vlan.js ?>` ` ` ?>
<? include /usr/www_safe/html/js/ip.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>


function uiDoOnLoad()
{
   jslSetValue("uiViewIPAddr","uiPostIPAddr");
   jslSetValue("uiViewNetMask","uiPostNetMask");
   jslSetValue("uiViewGateway","uiPostGateway");
   jslSetValue("uiViewHostname","uiPostHostname");
   jslSetValue("uiViewDomainname","uiPostDomainname");

   jslSetValue("uiViewStartIP","uiPostStartIP");
   jslSetValue("uiViewEndIP","uiPostEndIP");
   jslSetValue("uiViewLease","uiPostLease");
   jslSetValue("uiViewRelayIP","uiPostRelayIP");
   uiShowLANIPType();
   jslSetValue("uiViewPPPIPAddr","uiPostIPAddr");
   jslDisable("uiViewPPPIPAddr");
}

function uiShowLANIPType()
{
     switch (document.getElementById("uiPostIPType").value) {
          case "unmanaged": 
          	       jslDoToggleRadio('uiViewIPType',0,4);
		   	jslDisable("uiViewRelease", "uiViewRenew");
		   	jslDisable("uiViewIPAddr", "uiViewNetMask");
          	       jslDisable("uiPostStartIP","uiPostEndIP","uiPostLease","uiPostRelayIP");
          	       jslDisable("uiViewStartIP","uiViewEndIP","uiViewLease","uiViewRelayIP");
          	       jslDisable("uiViewGateway","uiPostGateway");
          	       jslDisable("uiViewHostname","uiPostHostname");
          	       jslDisable("uiViewDomainname","uiPostDomainname");
          	       jslDisable("uiViewStaticIPType0","uiViewStaticIPType1","uiViewStaticIPType2");
          		break;
          case "ppp": 
          	       jslDoToggleRadio('uiViewIPType',1,4);
		   	jslDisable("uiViewRelease", "uiViewRenew");
		   	jslDisable("uiViewIPAddr", "uiViewNetMask");
          	       jslDisable("uiPostStartIP","uiPostEndIP","uiPostLease","uiPostRelayIP");
          	       jslDisable("uiViewStartIP","uiViewEndIP","uiViewLease","uiViewRelayIP");
          	       jslDisable("uiViewGateway","uiPostGateway");
          	       jslDisable("uiViewHostname","uiPostHostname");
          	       jslDisable("uiViewDomainname","uiPostDomainname");
          	       jslDisable("uiViewStaticIPType0","uiViewStaticIPType1","uiViewStaticIPType2");
          		break;
          case "dhcpc":
          	       jslDoToggleRadio('uiViewIPType',2,4);
		   	jslEnable("uiViewRelease", "uiViewRenew");
          	       jslDisable("uiPostStartIP","uiPostEndIP","uiPostLease","uiPostRelayIP");
          	       jslDisable("uiViewStartIP","uiViewEndIP","uiViewLease","uiViewRelayIP");
          	       jslDisable("uiViewGateway","uiPostGateway");
          	       jslDisable("uiViewStaticIPType0","uiViewStaticIPType1","uiViewStaticIPType2");
          	       jslDisable("uiPostIPAddr","uiPostNetMask");
          		break;
          case "static":
		   	jslDisable("uiViewRelease", "uiViewRenew");
          	       jslDoToggleRadio('uiViewIPType',3,4);
          	       jslEnable("uiViewStaticIPType0","uiViewStaticIPType1","uiViewStaticIPType2");
          	       jslDoToggleRadio('uiViewStaticIPType',2,3);
                     jslDisable("uiViewStartIP","uiViewEndIP","uiViewLease","uiViewRelayIP");
          	       jslDisable("uiPostStartIP","uiPostEndIP","uiPostLease","uiPostRelayIP");
          		break;
          default:
		   	jslDisable("uiViewRelease", "uiViewRenew");
          	       jslDoToggleRadio('uiViewIPType',0,4);
          	       jslDisable("uiViewStartIP","uiViewEndIP","uiViewLease","uiViewRelayIP");
          	       jslDisable("uiPostStartIP","uiPostEndIP","uiPostLease","uiPostRelayIP");
          	       jslDisable("uiViewStaticIPType0","uiViewStaticIPType1","uiViewStaticIPType2");
          		break;
    }    
     if (document.getElementById("uiPostDHCPS").value=="1") {
//          	       jslDoToggleRadio('uiViewIPType',2,3);
          	       jslDoToggleRadio('uiViewStaticIPType',0,3);
          	       jslEnable("uiViewStartIP","uiViewEndIP","uiViewLease");
          	       jslEnable("uiPostStartIP","uiPostEndIP","uiPostLease");
          	       jslDisable("uiViewRelayIP");
          	       jslDisable("uiPostRelayIP");
     }
     else if (document.getElementById("uiPostRelay").value=="1") {
    //      	       jslDoToggleRadio('uiViewIPType',2,3);
          	       jslDoToggleRadio('uiViewStaticIPType',1,3);
          	       jslDisable("uiViewStartIP","uiViewEndIP","uiViewLease");
          	       jslDisable("uiPostStartIP","uiPostEndIP","uiPostLease");
          	       jslEnable("uiViewRelayIP");
          	       jslEnable("uiPostRelayIP");
    } else {
          	       jslDoToggleRadio('uiViewStaticIPType',2,3);
          	       jslDisable("uiViewStartIP","uiViewEndIP","uiViewLease");
          	       jslDisable("uiPostStartIP","uiPostEndIP","uiPostLease");
          	       jslEnable("uiViewRelayIP");
          	       jslEnable("uiPostRelayIP");
    }    
}

function uiUnManagedIP()
{
       document.getElementById("uiPostIPType").value="unmanaged";
       document.getElementById("uiPostDHCPS").value="0"; // server off and relay off
       document.getElementById("uiPostRelay").value="0";
   		jslDisable("uiViewIPAddr", "uiViewNetMask");
   	jslDisable("uiPostIPAddr", "uiPostNetMask");
       jslDisable("uiViewStaticIPType0","uiViewStaticIPType1","uiViewStaticIPType2");
       jslDisable("uiViewStartIP", "uiViewEndIP","uiViewLease");
       jslDisable("uiViewRelayIP");	
       jslDisable("uiPostStartIP","uiPostEndIP","uiPostLease","uiPostRelayIP");
       jslDoToggleRadio('uiViewStaticIPType',2,3); // no server and relay for non static
       jslDisable("uiViewGateway","uiPostGateway");
       jslDisable("uiViewHostname","uiPostHostname");
       jslDisable("uiViewDomainname","uiPostDomainname");
}

function uiPPPIP()
{
       document.getElementById("uiPostIPType").value="ppp";
       document.getElementById("uiPostDHCPS").value="0"; // server off and relay off
       document.getElementById("uiPostRelay").value="0";
   		jslDisable("uiViewIPAddr", "uiViewNetMask");
   	jslDisable("uiPostIPAddr", "uiPostNetMask");
       jslDisable("uiViewStaticIPType0","uiViewStaticIPType1","uiViewStaticIPType2");
       jslDisable("uiViewStartIP", "uiViewEndIP","uiViewLease");
       jslDisable("uiViewRelayIP");	
       jslDisable("uiPostStartIP","uiPostEndIP","uiPostLease","uiPostRelayIP");
       jslDoToggleRadio('uiViewStaticIPType',2,3); // no server and relay for non static
       jslDisable("uiViewGateway","uiPostGateway");
       jslDisable("uiViewHostname","uiPostHostname");
       jslDisable("uiViewDomainname","uiPostDomainname");
}


function uiDhcpcIP()
{
       document.getElementById("uiPostIPType").value="dhcpc";
       document.getElementById("uiPostDHCPS").value="0"; // server off and relay off

       document.getElementById("uiPostRelay").value="0";
  	jslDisable("uiViewIPAddr", "uiViewNetMask"); 
   	jslDisable("uiPostIPAddr", "uiPostNetMask");
       jslDisable("uiViewStaticIPType0","uiViewStaticIPType1","uiViewStaticIPType2");
       jslDisable("uiViewStartIP", "uiViewEndIP","uiViewLease");
       jslDisable("uiViewRelayIP");	
       jslDisable("uiPostStartIP","uiPostEndIP","uiPostLease","uiPostRelayIP");
       jslDoToggleRadio('uiViewStaticIPType',2,3); // no server and relay for non static
       jslDisable("uiViewGateway","uiPostGateway");
       jslEnable("uiViewHostname","uiPostHostname");
       jslEnable("uiViewDomainname","uiPostDomainname");
   	}  	

function uiStatic()
{
       document.getElementById("uiPostIPType").value="static";
       document.getElementById("uiPostDHCPS").value="0"; // server off and relay off
       document.getElementById("uiPostRelay").value="0";
  	jslEnable("uiViewIPAddr", "uiViewNetMask");
       jslEnable("uiViewStaticIPType0","uiViewStaticIPType1","uiViewStaticIPType2");
       jslDisable("uiViewStartIP", "uiViewEndIP","uiViewLease");
       jslDisable("uiViewRelayIP");
       jslDisable("uiPostStartIP","uiPostEndIP","uiPostLease","uiPostRelayIP");
       jslEnable("uiViewGateway","uiPostGateway");
       jslEnable("uiViewHostname","uiPostHostname");
       jslEnable("uiViewDomainname","uiPostDomainname");
}

function uiDhcps()
{
       document.getElementById("uiPostIPType").value="static";
       document.getElementById("uiPostDHCPS").value="1"; // server on and relay off
       document.getElementById("uiPostRelay").value="0";
  	jslEnable("uiViewIPAddr", "uiViewNetMask");
       jslEnable("uiViewStaticIPType0","uiViewStaticIPType1","uiViewStaticIPType2");
       jslEnable("uiViewStartIP", "uiViewEndIP","uiViewLease");
       jslDisable("uiViewRelayIP");	
       jslEnable("uiPostStartIP","uiPostEndIP","uiPostLease");
       jslDisable("uiPostRelayIP");
}

function uiRelay()
{
       document.getElementById("uiPostIPType").value="static";
       document.getElementById("uiPostRelay").value="1"; // relay on and server off
       document.getElementById("uiPostDHCPS").value="0";
  	jslEnable("uiViewIPAddr", "uiViewNetMask");
       jslEnable("uiViewStaticIPType0","uiViewStaticIPType1","uiViewStaticIPType2");
       jslDisable("uiViewStartIP", "uiViewEndIP","uiViewLease");
       jslEnable("uiViewRelayIP");	
       jslDisable("uiPostStartIP","uiPostEndIP","uiPostLease");
       jslEnable("uiPostRelayIP");
}

function uiDoRenew()
{
	jslEnable("uiPostRenew");
	jslFormSubmit("uiPostForm");;
}

function uiDoRelease()
{
	jslEnable("uiPostRelease");
       jslFormSubmit("uiPostForm");;
}

function uiLANServicesDoValidate()
{
  var message,value,count=0;

if(document.getElementById("uiPostIPType").value=="static") {
  value = document.getElementById("uiViewIPAddr").value;
  message = valDoValidateIp(value);
  if(message!=null) { erlDoAddError("bad","uiViewIPAddr","",value,message);  count++; }

  value = document.getElementById("uiPostNetMask").value;
  message = valDoValidateMask(value);
  if(message!=null) { erlDoAddError("bad","uiViewNetMask","",value,message);  count++; }

  value = document.getElementById("uiPostGateway").value;
  if(value!="")
  {
    message = (jslIsString("uiViewGateway")!=false)? valDoValidateIp(value):valDoValidateIp(value);
    if(message!=null) { erlDoAddError("bad","uiViewGateway","",value,message); count++; }
  }

  value = document.getElementById("uiPostHostname").value;
  if(value!="")
  {
    message = (jslIsString("uiViewHostname")!=false)? valDoValidateHostName(value): "";
    if(message!=null) { erlDoAddError("bad","uiViewHostname","",value,message); count++; }
  }

  value = document.getElementById("uiPostDomainname").value;
  if(value!="")
  {
    message = (jslIsString("uiViewDomainname")!=false)? valDoValidateHostName(value): "";
    if(message!=null) { erlDoAddError("bad","uiViewDomainname","",value,message); count++; }
  }
}
  if(count!=0) message="IP configuration";
   else message="";
  return message;
}

function uiDoValidate()
{
  var msg;

  if( document.getElementById("uiPostIPType").value=="static") {
	if( <? multiquerycount manager:command/lan/list_used ?> > 1 ) {
 		alert("While configuring multiple LAN Groups with Static IP Addressing, proper care should be taken to ensure routability. Please see help section \"Lan Group configuration\" for additional information.") ;
	}
  }
  msg = uiLANServicesDoValidate();
  if((msg!=null)&&(msg!="")) erlDoShowPageErrors();
  return jslPageErrorMsg(msg);

}

function uiDoSave()
{
  jslSetValue("uiPostNetMask","uiViewNetMask");
  jslSetValue("uiPostHostname","uiViewHostname");
  jslSetValue("uiPostDomainname","uiViewDomainname");
  jslSetValue("uiPostStartIP","uiViewStartIP");
  jslSetValue("uiPostEndIP","uiViewEndIP");
  jslSetValue("uiPostLease","uiViewLease");
  jslSetValue("uiPostRelayIP","uiViewRelayIP");

  if(uiDoValidate()==true)
  {
   if(document.getElementById("uiPostIPAddr").value!=document.getElementById("uiViewIPAddr").value) 
   {
     if(!confirm("In case new IP Address is not reachable then browser will lose connectivity. Confirm to assign new IP"))
         return false;
   }
   if(document.getElementById("uiPostGateway").value != document.getElementById("uiViewGateway").value) {
     if (confirm("You may lose any existing connections in case of incorrect default gateway. Are you sure you want to change the default gateway?")) {
 	jslSetValue("uiPostGateway","uiViewGateway");
     } 
  }   
   // Overload the get and error page to blank, as we change the IP halfway through.
   // We get the real form after the 2 second delay
   jslSetValue("uiPostIPAddr","uiViewIPAddr");
   // document.getElementById("uiPostRedirect").value="http://"+document.getElementById("uiPostIPAddr").value+"/cgi-bin/webcm?getpage=/usr/www_safe/html/defs/<? echo $var:style ?>/menus/<? echo $var:main ?>.html&var:main=<? echo $var:main ?>&var:style=<? echo $var:style ?>&var:menu=setup&var:menutitle=Setup&var:pagename=lan_cfg&var:pagetitle=LAN%20Configuration&var:langrp=<? echo $var:langrp ?>";
   // document.getElementById("uiPostGetPage").value="/usr/www_safe/html/redirect.html";
   // document.getElementById("uiPostErrPage").value="/usr/www_safe/html/redirect.html";
   jslFormSubmit("uiPostForm");
   // setTimeout("uiDoNextLoc()", 2000);
  }
}
function uiDoNextLoc()
{
   document.location.href="http://"+document.getElementById("uiPostIPAddr").value+"/cgi-bin/webcm?getpage=/usr/www_safe/html/defs/<? echo $var:style ?>/menus/<? echo $var:main ?>.html&var:style=<? echo $var:style ?>&var:main=<? echo $var:main ?>&var:menu=setup&var:menutitle=Setup&var:pagename=lan_cfg&var:pagetitle=LAN%20Configuration&var:langrp=lan";
}
function uiDoCancel()
{
  jslGoTo(null,"home");
}
</script>

