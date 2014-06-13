
<script language="JavaScript">

<? include /usr/www_safe/html/js/ip.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

<? multiquery manager:command/connection/list <? setvariableonce var:conid $01 ?> ?>
function uiDoOnLoad()
{
  jslPostToViewCheckBox("uiViewFwanNATState","uiPostFwanNATState");
  //doShowPageErrors();
  return true;
}
function uiDoSave()
{
  var obj; 
  jslViewToPostCheckBox("uiPostFwanNATState","uiViewFwanNATState");
  document.getElementById("uiPostForm").submit();
}
function uiDoCancel()
{
  jslGoTo(null,"home");
}

</script>
