
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
   jslDoShowComboBox("uiViewPort1","uiPostPort1");
   jslDoShowComboBox("uiViewPort2","uiPostPort2");
   jslDoShowComboBox("uiViewPort3","uiPostPort3");
   jslDoShowComboBox("uiViewPort4","uiPostPort4");
   if(document.getElementById("uiPostPort1").value==5)jslDisable("uiViewPort1","uiPostPort1");
   if(document.getElementById("uiPostPort2").value==5)jslDisable("uiViewPort2","uiPostPort2");
   if(document.getElementById("uiPostPort3").value==5)jslDisable("uiViewPort3","uiPostPort3");
   if(document.getElementById("uiPostPort4").value==5)jslDisable("uiViewPort4","uiPostPort4");
}

function uiMgmtIpDoValidate()
{
  var message,value,count=0;

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
  if(count!=0) message="IP Management";
   else message="";
  return message;
}
function uiDoValidate()
{
  var msg;
  msg = uiMgmtIpDoValidate();
  if((msg!=null)&&(msg!="")) erlDoShowPageErrors();
  return jslPageErrorMsg(msg);
}

function uiDoSave()
{
  jslSetValue("uiPostNetMask","uiViewNetMask");
  jslSetValue("uiPostGateway","uiViewGateway");
  jslSetValue("uiPostHostname","uiViewHostname");
  jslSetValue("uiPostDomainname","uiViewDomainname");
  if(uiDoValidate()==true)
  {
   if(document.getElementById("uiPostIPAddr").value!=document.getElementById("uiViewIPAddr").value) 
   {
     if(!confirm("In case new IP Address is not reachable then browser will lose connectivity. Confirm to assign new IP"))
         return false;
   }
   // Overload the get and error page to blank, as we change the IP halfway through.
   // We get the real form after the 2 second delay
   jslSetValue("uiPostIPAddr","uiViewIPAddr");
   document.getElementById("uiPostRedirect").value="http://"+document.getElementById("uiPostIPAddr").value+"/cgi-bin/webcm?getpage=/usr/www_safe/html/defs/<? echo $var:style ?>/menus/<? echo $var:main ?>.html&var:main=<? echo $var:main ?>&var:style=<? echo $var:style ?>&var:menu=setup&var:menutitle=Setup&var:pagename=mgmt_ip&var:pagetitle=Management%20IP";
   document.getElementById("uiPostGetPage").value="/usr/www_safe/html/redirect.html";
   document.getElementById("uiPostErrPage").value="/usr/www_safe/html/redirect.html";
   jslFormSubmit("uiPostForm");
   setTimeout("uiDoNextLoc()", 2000);
  }
}
function uiDoNextLoc()
{
   document.location.href="http://"+document.getElementById("uiPostIPAddr").value+"/cgi-bin/webcm?getpage=/usr/www_safe/html/defs/<? echo $var:style ?>/menus/<? echo $var:main ?>.html&var:style=<? echo $var:style ?>&var:main=<? echo $var:main ?>&var:menu=setup&var:menutitle=Setup&var:pagename=mgmt_ip&var:pagetitle=Management%20IP";
}
function uiDoCancel()
{
  jslGoTo(null,"home");
}
</script>
