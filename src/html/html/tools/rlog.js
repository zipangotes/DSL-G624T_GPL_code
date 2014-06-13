
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/ip.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/ip.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>
function uiDoOnLoad()
{ 
  uiSelect (document.getElementById("uiPostUserLevel").value, "uiViewUserLevel");  
}
function uiSelect(value, selectBoxId)
{
  var i;
  for (i=0; i< document.getElementById(selectBoxId).length; i++)
  {
  	if (value == document.getElementById(selectBoxId).options[i].value)
      document.getElementById(selectBoxId).options[i].selected = true;
  }
}
function uiAdd()
{
  if (doValidateIP (document.getElementById("uiViewAddIPAddress").value) == false)
	return;

  document.getElementById("uiPostAddIPAddress").disabled = false;
  document.getElementById("uiPostDelIPAddress").disabled = true;
  document.getElementById("uiPostUserLevel").disabled = true;

  jslSetValue("uiPostAddIPAddress","uiViewAddIPAddress");
  document.getElementById("uiPostForm").submit();
  return;
}
function uiRemove()
{
  var delDestination = document.getElementById("uiViewDestination");  
  document.getElementById("uiPostDelIPAddress").value = delDestination.options[delDestination.selectedIndex].value;
  document.getElementById("uiPostDelIPAddress").disabled = false;
  document.getElementById("uiPostAddIPAddress").disabled = true;
  document.getElementById("uiPostUserLevel").disabled = true;
  document.getElementById("uiPostForm").submit();
  return;
}
function uiDoCancel()
{
  jslGoTo(null,"home");
}
function uiDoSave ()
{
  var obj;

  obj = document.getElementById("uiViewUserLevel");
  document.getElementById("uiPostUserLevel").value = obj.options[obj.selectedIndex].value;

  document.getElementById("uiPostAddIPAddress").disabled = true;
  document.getElementById("uiPostDelIPAddress").disabled = true;
  document.getElementById("uiPostUserLevel").disabled = false;
  document.getElementById("uiPostForm").submit();
}
</script>
