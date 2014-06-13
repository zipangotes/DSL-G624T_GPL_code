<script language="JavaScript">

<? include /usr/www_safe/html/js/ip.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{ 
  uiSelect (document.getElementById("uiPostDefaultLevel").value, "uiViewDefaultLevel");
  uiSelect (document.getElementById("uiPostLogicLevel").value, "uiViewLogicLevel");
  uiSelect (document.getElementById("uiPostEntityLevel").value, "uiViewEntityLevel");
  uiSelect (document.getElementById("uiPostModuleLevel").value, "uiViewModuleLevel");
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

function uiDoCancel()
{
  jslGoTo(null,"home");
}

function uiDoSave ()
{
  var obj;
  
  obj = document.getElementById("uiViewDefaultLevel");
  document.getElementById("uiPostDefaultLevel").value = obj.options[obj.selectedIndex].value;

  obj = document.getElementById("uiViewLogicLevel");
  document.getElementById("uiPostLogicLevel").value = obj.options[obj.selectedIndex].value;

  obj = document.getElementById("uiViewEntityLevel");
  document.getElementById("uiPostEntityLevel").value = obj.options[obj.selectedIndex].value;

  obj = document.getElementById("uiViewModuleLevel");
  document.getElementById("uiPostModuleLevel").value = obj.options[obj.selectedIndex].value;

  obj = document.getElementById("uiViewUserLevel");
  document.getElementById("uiPostUserLevel").value = obj.options[obj.selectedIndex].value;

  document.getElementById("uiPostForm").submit();
}

</script>

