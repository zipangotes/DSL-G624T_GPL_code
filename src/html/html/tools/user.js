
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/val.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/val.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

function uiDoOnLoad()
{  
  jslSetValue("uiViewUserName","uiPostUserName");
  jslSetValue("uiViewPassword","uiPostPassword");
  jslSetValue("uiViewPasswordConfirm","uiPostPassword");
  jslSetValue("uiViewIdleTimeout","uiPostIdleTimeout");
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}

function uiDoSave()
{
var timeout;
var message;

  if (document.getElementById("uiViewUserName").value.length == 0)
  {
    if (confirm("Do you want to set an empty User Name ? ") == 0)
    return;
  }	
  if((message=valDoValidateUserName(document.getElementById("uiViewUserName").value))!=null) {
    alert(message);
    return;
  }
  if (document.getElementById("uiViewPassword").value.length == 0)
  {
    if (confirm("Do you want to set an empty password ? ") == 0)
		return;
  }
  if (document.getElementById("uiViewPassword").value != document.getElementById("uiViewPasswordConfirm").value)
  {
    alert("Your Password and Confirmed Password must match before you can apply. ");
    return;
  }

   timeout=document.getElementById("uiViewIdleTimeout").value;

   if(timeout.match("^[0-9]+\$")) { 
     if (parseInt(timeout) < 0)
     {
         alert("Please enter a valid idle timeout value.");
         return;
     }
  }
  else return("An integer can only have digits");

  jslSetValue("uiPostUserName","uiViewUserName");
  jslSetValue("uiPostPassword","uiViewPassword");
  jslSetValue("uiPostPasswordConfirm","uiViewPassword");
  jslSetValue("uiPostIdleTimeout","uiViewIdleTimeout");

  document.getElementById("uiPostForm").submit();
  return;
}
</script>


