
<script language="JavaScript">

<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{
  uiDoSetRipStatus();
  <? multiquery manager:command/lan/list_used 
    ` jslDoShowComboBox("uiViewLanDirection$00", "uiPostLanDirection$00"); ` ?>
  <? multiquery manager:command/connection/list_group 
    ` jslDoShowComboBox("uiViewWanDirection$00", "uiPostWanDirection$00"); ` ?>
    
  uiDoSetRipProtocol();
  uiDoSetRipPasswordReq();
  uiDoSetRipPassword();
}
function uiDoSave()
{
  uiDoSavePassword();
  jslFormSubmit("uiPostForm");
}
function uiDoCancel()
{
  jslGoTo(null,"home");
}
function uiDoSetRipStatus()
{
  var hnd = document.getElementById("uiViewRipStatus");
  var posthnd = document.getElementById("uiPostRipStatus");
  switch(posthnd.value)
  {
    case "0":
	  hnd.value="off";
	  hnd.src="<? echo $var:btnCheckBoxOff ?>"
	break;
	case "1":
	  hnd.value="on";
	  hnd.src="<? echo $var:btnCheckBoxOn ?>"
	break;
	default:
	  hnd.value="off";
	  hnd.src="<? echo $var:btnCheckBoxOff ?>"
	  posthnd.value="0";				 	  	
	break;
  }
}
function uiDoSetRipDirection()
{
  var hnd = document.getElementById("uiViewDirection");
  var direction = document.getElementById("uiPostRipDirection");
  switch(direction.value)
  {
	case "2":
	  hnd.selectedIndex=1;
	break;
	case "0":
	  hnd.selectedIndex=2;
	break;
    case "1":
	default:
	  hnd.selectedIndex=0;
      direction.value = "1";
	break;
  }
}
function uiDoSetRipProtocol()
{
  var protocol = document.getElementById("uiPostRipVersion").value;
  switch(protocol)
  {
	case "2":
	  document.getElementById("uiViewRipProtocol").selectedIndex=1;
	  jslEnable("uiViewRipPasswordReq");
	  jslEnable("uiViewRipPassword");
	  jslEnable("uiViewText1");
	  jslEnable("uiViewText2");
	break;
	case "3":
	  document.getElementById("uiViewRipProtocol").selectedIndex=2;
	  jslEnable("uiViewRipPasswordReq");
	  jslEnable("uiViewRipPassword");
	  jslEnable("uiViewText1");
	  jslEnable("uiViewText2");
	break;
       case "1":
	default:
	  document.getElementById("uiViewRipProtocol").selectedIndex=0;
	  jslDisable("uiViewRipPasswordReq");
	  jslDisable("uiViewRipPassword");
	  jslDisable("uiViewText1");
	  jslDisable("uiViewText2");
	break;
  }
}
function uiDoChangeRipProtocol(protocol)
{
  document.getElementById("uiPostRipVersion").value = protocol;
  switch(protocol)
  {
	case "2":
	case "3":
	  jslEnable("uiViewRipPasswordReq");
	  jslEnable("uiViewRipPassword");
	  jslEnable("uiViewText1");
	  jslEnable("uiViewText2");
	break;
	case "1":
	default:
	  jslDisable("uiViewRipPasswordReq");
	  jslDisable("uiViewRipPassword");
	  jslDisable("uiViewText1");
	  jslDisable("uiViewText2");
	  document.getElementById("uiViewRipPasswordReq").value="off";
	  document.getElementById("uiViewRipPasswordReq").src="<? echo $var:btnCheckBoxOff ?>";
         document.getElementById("uiPostRipPasswordReq").value = "0";
         document.getElementById("uiViewRipPassword").value = "";
	break;
  }
  uiDoChangeRipPasswordReq();
}
function uiDoChangeRipStatus(status)
{
  switch(status)
  {
    case "off":
	  document.getElementById("uiPostRipStatus").value="0";
	break;
	case "on":
	  document.getElementById("uiPostRipStatus").value="1";	
	break;
	default:
	  document.getElementById("uiPostRipStatus").value="0";				 	  	
	break;
  }
}
function uiDoChangeRipDirection(direction,PostId)
{
   document.getElementById(PostId).value = direction;
}
function uiDoChangeRipPasswordReq()
{
  if(document.getElementById("uiViewRipPasswordReq").value=="on")
  {
    document.getElementById("uiPostRipPasswordReq").value = "1";
    document.getElementById("uiViewRipPassword").value = document.getElementById("uiPostRipPassword").value;
    jslEnable("uiViewRipPassword");
    jslEnable("uiViewText2");
  }
  else
  {
    document.getElementById("uiPostRipPasswordReq").value = "0";
    document.getElementById("uiViewRipPassword").value = "";
    jslDisable("uiViewRipPassword");
    jslDisable("uiViewText2");
  }
}
function uiDoSetRipPasswordReq()
{
  switch(document.getElementById("uiPostRipPasswordReq").value)
  {
	case "1":
      document.getElementById("uiViewRipPasswordReq").value="on";
      document.getElementById("uiViewRipPasswordReq").src="<? echo $var:btnCheckBoxOn ?>";
	break;
       case "0":
	default:
      document.getElementById("uiPostRipPasswordReq").value = "0";
      document.getElementById("uiViewRipPasswordReq").value="off";
      document.getElementById("uiViewRipPasswordReq").src="<? echo $var:btnCheckBoxOff ?>";
	break;
  }
}
function uiDoSetRipPassword()
{
  var passwordReq = document.getElementById("uiPostRipPasswordReq").value;
  switch(passwordReq)
  {
    case "1":
      document.getElementById("uiViewRipPassword").value = document.getElementById("uiPostRipPassword").value;
      jslEnable("uiViewRipPassword");
      jslEnable("uiViewText2");
	break;
	case "0":
      document.getElementById("uiViewRipPassword").value = "";
      jslDisable("uiViewRipPassword");
      jslDisable("uiViewText2");
	break;
	default:
	break;
  }
}
function uiDoSavePassword()
{
  if(document.getElementById("uiViewRipPasswordReq").value=="on")
    jslSetValue("uiPostRipPassword","uiViewRipPassword");
  else
    jslDisable("uiPostRipPassword");
}
</script>
