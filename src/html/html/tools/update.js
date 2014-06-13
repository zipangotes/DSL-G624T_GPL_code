

<? if eq 1 1 '<script language="JavaScript">' '' ?>



<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>



<? if eq 1 1 '' '<script language="JavaScript">' ?>



function uiDoOnLoad()

{
	
  document.getElementById("uiStatus").value = "None";
  document.getElementById("uiStatus").disabled=true;	
  return true;

}


function uiDoUpdate(value)

{

  if(value=="") alert("You must select a file to update.");

  else

  {

    uiToolsTopDoSetLock();
    document.getElementById("uiStatus").value = "Upgrade in progress, Please wait...";
    document.getElementById("uiStatus").disabled=true;	
    document.getElementById("uiUpgrade").disabled=true;	
    document.getElementById("uiConfig").disabled=true;	
	document.getElementById("uiPostUpdateForm").submit();

  }

}


function uiDoCancel()

{
  jslGoTo(null,"home");

}

</script>



