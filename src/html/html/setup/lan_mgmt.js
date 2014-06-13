
<script language="JavaScript">

<? include /usr/www_safe/html/js/val.js ?>
<? include /usr/www_safe/html/setup/pvc.js ?>
<? if eq `1` `$var:HasVlan` `<? include /usr/www_safe/html/setup/vlan.js ?>` ` ` ?>
<? include /usr/www_safe/html/js/ip.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{

}

function uiDoSave()
{
   jslDisable("uiPostLANAdd");
   jslDisable("uiPostLANDelete");
   jslFormSubmit("uiPostForm");
}

function uiDoAddRule(lan)
{
   document.getElementById("uiPostLANAdd").value = document.getElementById("uiViewInterface").value + "|" + lan;
   jslEnable("uiPostLANAdd");
   jslDisable("uiPostLANDelete");
   jslFormSubmit("uiPostForm");
}


function uiDoDeleteRule(lan)
{
   if(document.getElementById("uiViewLAN"+lan).value=="eth0") {
        alert("Ethernet interface can not be removed out of LAN Group 1");
        return;
   }
   document.getElementById("uiPostLANDelete").value = document.getElementById("uiViewLAN"+lan).value + "|" + lan;
   jslEnable("uiPostLANDelete");
   jslDisable("uiPostLANAdd");
   jslFormSubmit("uiPostForm");
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}
</script>
