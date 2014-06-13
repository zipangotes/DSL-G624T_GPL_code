
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/ip.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/ip.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

function uiPollResult()
{  
  document.getElementById("uiPostPingResultForm").submit();
}
function uiDoOnLoad()
{
  jslSetValue("uiViewPingIPAddress","uiPostPingIPAddress");
  jslSetValue("uiViewPingSize","uiPostPingSize");
  jslSetValue("uiViewPingNumber","uiPostPingNumber");

  if (document.getElementById("uiPostPingMore").value == 1)
    setInterval("uiPollResult()",1000);
}
function uiCancel()
{
}
function uiSave()
{
  if(doValidateIP(document.getElementById("uiViewPingIPAddress").value) == false)
	return false;

  if (parseInt(document.getElementById("uiViewPingSize").value) == 0)
  {
    alert("Please select a ping size more than 0 and less than or equal to 64,000.");
	return false;
  }
  if (parseInt(document.getElementById("uiViewPingNumber").value) == 0)
  {
    alert("Please select a ping count more than 0 and less than or equal to 9.");
	return false;
  }
  if (parseInt(document.getElementById("uiViewPingSize").value) > 64000)
  {
    alert("Please select a ping size less than or equal to 64,000.");
	return false;
  }
  if (parseInt(document.getElementById("uiViewPingNumber").value) >= 10)
  {
    alert("Please select a ping count less than or equal to 9.");
	return false;
  }
  if (parseInt(document.getElementById("uiViewPingNumber").value) < 1)
  {
    alert("Please select a ping count from 1 to 9.");
	return false;
  }
  jslSetValue("uiPostPingIPAddress","uiViewPingIPAddress");
  jslSetValue("uiPostPingSize","uiViewPingSize");
  jslSetValue("uiPostPingNumber","uiViewPingNumber");

  document.getElementById("uiPostForm").submit();
}
</script>
