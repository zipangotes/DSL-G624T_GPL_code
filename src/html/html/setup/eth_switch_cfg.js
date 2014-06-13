
<script language="JavaScript">

<? include /usr/www_safe/html/js/ip.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

<? multiquery manager:command/connection/list <? setvariableonce var:conid $01 ?> ?>
function uiDoOnLoad()
{
   jslDoShowComboBox("uiViewPort1","uiPostPort1");
   jslDoShowComboBox("uiViewPort2","uiPostPort2");
   jslDoShowComboBox("uiViewPort3","uiPostPort3");
   jslDoShowComboBox("uiViewPort4","uiPostPort4");
  return true;
}
function uiDoSave()
{
  document.getElementById("uiPostForm").submit();
}
function uiDoCancel()
{
  jslGoTo(null,"home");
}

</script>
