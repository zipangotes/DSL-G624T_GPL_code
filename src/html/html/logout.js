
<? if eq 1 1 '<script language="JavaScript"><? include /usr/www_safe/html/js/jsl.js ?></script>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>

<script language="JavaScript">
function uiDoOnLoad()
{
}
function uiDoLogout()
{
  document.getElementById("uiPostForm").submit();
  return;
}
function uiDoCancel()
{
  jslGoTo("home","home");
}
</script>
