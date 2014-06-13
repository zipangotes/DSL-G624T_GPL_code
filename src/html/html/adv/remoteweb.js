<? if eq `` `$var:conid`  `<? multiquery manager:command/connection/list <? setvariableonce var:conid $11 ?> ?>` ` ` ?>



<script language="JavaScript">

<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/val.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{
<? if neq `0` `<? multiquerycount manager:command/connection/list ?>`
` jslPostToViewCheckBox("uiViewRhostState","uiPostRhostState");
  jslSetValue("uiViewRhostIP", "uiPostRhostIP");
  jslSetValue("uiViewRhostMask", "uiPostRhostMask");
  jslSetValue("uiViewRedirectPort", "uiPostRedirectPort");
  jslDoShowComboBox("uiViewConnectionSelector","uiPostConId");
` ?>
  return true;
}

function uiDoValidatePort(id)
{
var message=null;

  value=document.getElementById(id).value;
  if(value != "" && value !="*") {	
      message=valDoValidateInteger(value);
      if(message==null) { 
            value=Number(value);
            if ((value < 1) || (value > 65535)) {
 	        message="Port must to be an integer between 1 and 65535"; 
           }
      }
  } 
  return message;
}

function RemoteWebDoValidatePage()
{
  var message;
  var value;
  var count=0;

  value = document.getElementById("uiViewRhostIP").value;
  if(value != "" && value !="*") {	
      message = valDoValidateIp(value);
      if(message!=null) { erlDoAddError("bad","uiViewRhostIP","",value,message); count++; }
  } 
  value = document.getElementById("uiViewRhostMask").value;
  if(value != "" && value !="*") {	
      message = valDoValidateMask(value);
      if(message!=null) { erlDoAddError("bad","uiViewRhostMask","",value,message); count++; }
  } 

  value = document.getElementById("uiViewRedirectPort").value;
  message=uiDoValidatePort("uiViewRedirectPort");
  if(message!=null) { erlDoAddError("bad","uiViewRedirectPort","",value,message); count++; }

  	
  if(count!=0) message="Remote Web";
   else message="";
  return message;
}


function uiDoValidate()
{
  var msg;
  
  msg=RemoteWebDoValidatePage();
  if(msg!="") {
  	erlDoShowPageErrors();
  	return jslPageErrorMsg(msg);
  }
  return true;
}


function uiDoSave()
{
  if(uiDoValidate()==true) {
    jslSetValue("uiPostRhostIP", "uiViewRhostIP");
    jslSetValue("uiPostRhostMask", "uiViewRhostMask");
    jslSetValue("uiPostRedirectPort", "uiViewRedirectPort");
    jslViewToPostCheckBox("uiPostRhostState","uiViewRhostState");
    document.getElementById("uiPostErrorPageName").value="remoteweb";
    document.getElementById("uiPostPageName").value="remoteweb";
    jslFormSubmit("uiPostForm");
  }  
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}

function uiDoSelectConnection(connection)
{
    document.getElementById("uiPostConId").value=connection;
    document.getElementById("uiPostForm").method="GET";
    document.getElementById("uiPostPageName").value="remoteweb";
    document.getElementById("uiPostForm").submit();       
}

</script>
