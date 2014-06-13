
<script language="JavaScript">

<? include /usr/www_safe/html/js/val.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{
 jslPostToViewCheckBox("uiViewRemoteManagementState", "uiPostRemoteManagementState")
  //doShowPageErrors();
 uiDoChangeState(document.getElementById("uiViewRemoteManagementState").value);
}

function uiDoChangeState(value)
{
  switch(value) 
  {
    case "on":
     <? multiquery fdb:settings/categoryH/acl/acl_services/wan 
     	`jslEnable("uiViewWanService$00");
       ` ?>
     
      <? multiquery manager:command/lan/list_used 
        ` <? multiquery fdb:settings/categoryH/acl/acl_services/$01
               `  jslEnable("uiViewLan$00Service$10"); ` ?>
        ` ?> 	
     jslEnable("uiViewLanAccessNewIP", "uiViewAccessItemAdd","uiViewAccessItemDelete","uiViewLanAccessListSelector");
	 break;
	 
     case "off":
     default:
     <? multiquery fdb:settings/categoryH/acl/acl_services/wan 
     	`jslDisable("uiViewWanService$00");
       ` ?>

     <? multiquery manager:command/lan/list_used         
      	` <? multiquery fdb:settings/categoryH/acl/acl_services/$01
      	      ` jslDisable("uiViewLan$00Service$10"); ` ?>
       ` ?>
         	
     jslDisable("uiViewLanAccessNewIP", "uiViewAccessItemAdd","uiViewAccessItemDelete","uiViewLanAccessListSelector");
     break;
	}
}

function uiDoSave()
{
  var error=null;
  
  if((document.getElementById("uiViewLanAccessNewIP").value!="") && (document.getElementById("uiViewAccessItemAdd").value == "on"))
  {
    error=valDoValidateIp(document.getElementById("uiViewLanAccessNewIP").value);
    if(error!=null) {
    	alert(error);
    	return;
    }
    jslEnable("uiPostAddNewIP");
    jslSetValue("uiPostAddNewIP","uiViewLanAccessNewIP");
  }

  if((document.getElementById("uiViewLanAccessListSelector").value!="invalid") &&
  (document.getElementById("uiViewAccessItemDelete").value=="on")
  )
  {
   jslEnable("uiPostDeleteIP");
   jslSetValue("uiPostDeleteIP","uiViewLanAccessListSelector");
  }
  else jslDisable("uiPostDeleteIP");

  jslViewToPostCheckBox("uiPostRemoteManagementState", "uiViewRemoteManagementState")

  if(confirm("Are you sure access is not completely blocked"))jslFormSubmit("uiPostForm");
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}
</script>
