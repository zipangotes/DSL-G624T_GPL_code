
<script language="JavaScript">

<? include /usr/www_safe/html/setup/wst.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{ 
  jslPostToViewCheckBox("uiViewUpnpState","uiPostUpnpState");
  uiChangeUPNPStatus(document.getElementById("uiViewUpnpState").value);
  jslDoShowComboBox("uiViewUpnpWAN","uiPostUpnpWAN");
  jslDoShowComboBox("uiViewUpnpLAN","uiPostUpnpLAN");
  }

function uiUPNPLAN(value)
     {
	document.getElementById("uiPostLanGrp").value=value;
	document.getElementById("uiPostForm").method="GET";
       document.getElementById("uiPostPageName").value="upnp";
  document.getElementById("uiPostForm").submit();
}

function uiDoSelectWAN(value)
{
    	document.getElementById("uiPostUpnpWAN").value=value;
}

function uiDoSelectLAN(value)
{
	document.getElementById("uiPostUpnpLAN").value=value;
}

function uiDoSave()
{ 
  jslSetValue("uiPostUpnpWAN", "uiViewUpnpWAN");
  jslSetValue("uiPostUpnpLAN", "uiViewUpnpLAN");
  jslViewToPostCheckBox("uiPostUpnpState","uiViewUpnpState");
  document.getElementById("uiPostForm").submit();
}

function uiDoCancel()
{ 
  jslGoTo(null,"home");
}

function uiChangeUPNPStatus(value)
{
  switch (value)  {
  	case "on" :
			    jslEnable("uiViewUpnpWAN");
			    jslEnable("uiViewUpnpLAN");
			    break;	
      case "off":
      	default:
			    jslDisable("uiViewUpnpWAN");
			    jslDisable("uiViewUpnpLAN");
  }
}

</script>
