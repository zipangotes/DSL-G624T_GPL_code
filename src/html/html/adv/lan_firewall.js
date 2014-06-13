
<script language="JavaScript">

<? include /usr/www_safe/html/setup/wst.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{ 
   jslPostToViewCheckBox("uiViewLAN0LAN1","uiPostLAN0LAN1");
<? if eq `3` `<? multiquerycount manager:command/lan/list ?>`
`   jslPostToViewCheckBox("uiViewLAN0LAN2","uiPostLAN0LAN2");
   jslPostToViewCheckBox("uiViewLAN1LAN2","uiPostLAN1LAN2");
` ` ` ?>
}
function uiDoSave()
{
   jslViewToPostCheckBox("uiPostLAN0LAN1","uiViewLAN0LAN1");
   jslViewToPostCheckBox("uiPostLAN1LAN0","uiViewLAN0LAN1");
   
   <? if eq `3` `<? multiquerycount manager:command/lan/list ?>`
   `
   jslViewToPostCheckBox("uiPostLAN0LAN2","uiViewLAN0LAN2");
   jslViewToPostCheckBox("uiPostLAN2LAN0","uiViewLAN0LAN2");

   jslViewToPostCheckBox("uiPostLAN1LAN2","uiViewLAN1LAN2");
   jslViewToPostCheckBox("uiPostLAN2LAN1","uiViewLAN1LAN2");
   ` ` ` ?>
   document.getElementById("uiPostForm").submit();
  return;
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}


</script>
