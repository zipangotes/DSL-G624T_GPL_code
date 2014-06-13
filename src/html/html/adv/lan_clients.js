
<script language="JavaScript">

<? include /usr/www_safe/html/js/val.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{
  jslSetValue("uiViewIpAddress", "uiPostNewIP");
  jslSetValue("uiViewNewHostname", "uiPostNewHostname");
  jslSetValue("uiViewNewMAC", "uiPostNewMAC");
  uiDoShowLAN("uiViewLANConn","<? echo $var:langrp ?>");
  //doShowPageErrors();
}

function uiDoShowLAN(viewid,value)
{
  var i;
  var selector = document.getElementById(viewid);
  
  if(selector==null) return;

  for(i=0; i < selector.length; i++)
  {
    if(selector.options[i].value == value)
	{
         selector.selectedIndex = i;
         break;
	}
  }
}

function uiDoSave()
{
  jslSetValue("uiPostNewIP", "uiViewIpAddress");
  jslSetValue("uiPostNewHostname", "uiViewNewHostname");
  jslSetValue("uiPostNewMAC", "uiViewNewMAC");

  if(uiDoValidate()==true)
  {
    jslFormSubmit("uiPostForm");
  }
}

function uiDoSelectLAN(value)
{
	document.getElementById("uiPostLanGrp").value=value;
	document.getElementById("uiPostForm").method="GET";
       document.getElementById("uiPostPageName").value="lan_clients";
       document.getElementById("uiPostForm").submit();       
}

function uiLanClientsDoValidate()
{
  var message;
  var value;
  var count=0;

  value = document.getElementById("uiPostNewIP").value;
  if(value!="")
  {
    message = valDoValidateIp(value);
    if(message!=null) { erlDoAddError("bad","uiViewIpAddress","",value,message); count++; }
  }
  
  value = document.getElementById("uiPostNewHostname").value;
  if(value!="")
  {
    message = valDoValidateHostName(value);
    if(message!=null) { erlDoAddError("bad","uiViewNewHostname","",value,message);  count++; }
  }

  if(count!=0) message="LAN Clients";
   else message="";
  return message;
}
function uiDoValidate()
{
  var msg;
  msg = uiLanClientsDoValidate();
  erlDoShowPageErrors();
  return jslPageErrorMsg(msg);
}
function uiDoCancel()
{
  jslGoTo(null,"home");
}
function uiDoToggleDelete(id,control)
{
  jslDoToggleCheckBox(id);
  if(document.getElementById(control).disabled==true)
  {
    document.getElementById(control).disabled = false;
  }
  else
  {
    document.getElementById(control).disabled = true;
  }
}
</script>
