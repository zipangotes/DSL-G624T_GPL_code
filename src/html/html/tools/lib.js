
<? include /usr/www_safe/html/$var:menu/$var:pagename.js ?>

<script language="JavaScript">
var uiToolsTopLock=false;
function uiToolsTopDoOnClick(href)
{
  if(uiToolsTopLock==false) document.location.href=href;
   else { alert("You must restart the box."); }
}
function uiToolsTopDoSetLock()
{
  uiToolsTopLock=true;
}
</script>
