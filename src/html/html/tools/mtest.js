
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

var PVC = new Array();
var MaxConnections;
var CurrentConnectionSelection = -1;
function uiPollResult()
{  
  document.getElementById("uiPostMTestResultForm").submit();
}
function uiDoOnLoad()
{
  var i = 0;

  <? multiquery manager:command/connection/list_group 'PVC[i++]="<? query $03:pvc:settings/vpi ?>" + "." + "<? query $03:pvc:settings/vci ?>";' ?>

  MaxConnections = i;
  switch(document.getElementById("uiPostTestCheckResult").value)
  {
    case "0":
       document.getElementById("uiViewTestResult").value = "Failure";
	   break;
    case "1":
	   document.getElementById("uiViewTestResult").value = "Success";
	   break;
    case "2":
	   document.getElementById("uiViewTestResult").value = "In progress...";
       setInterval("uiPollResult()",1000);
	   break;
    default:
	   document.getElementById("uiViewTestResult").value = "No test is running";
	   break;
  }  
  switch(document.getElementById("uiPostTestConnection").value) {
  	<? multiquery manager:command/connection/list_group
	 `case "$01":
	 		CurrentConnectionSelection = $00;
	 		jslDoToggleRadio("uiViewTestStatus",$00, MaxConnections);
			break;
	 ` ?>
  }
  jslDoShowComboBox("uiViewModemTest", "uiPostTestCommand");
}
function uiDoSave()
{
  if (CurrentConnectionSelection == -1)
  {
	alert ("Select a connection on which the test needs to execute");
	return;
  } 
  jslSetValue("uiPostTestCommand", "uiViewModemTest");
  document.getElementById("uiPostTestPVC").value = PVC[CurrentConnectionSelection];
  document.getElementById("uiPostForm").submit();
}
function uiChangeTestStatus (arg)
{
  CurrentConnectionSelection = arg;
  jslDoToggleRadio("uiViewTestStatus",arg, MaxConnections);
}
function uiCurrentConnid (conid)
{
  document.getElementById("uiPostTestConnection").value = conid;
}

</script>
