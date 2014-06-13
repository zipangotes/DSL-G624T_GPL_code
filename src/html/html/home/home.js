

<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/encrypt.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/encrypt.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

function uiDoOnLoad()
{
  <? if eq `1` `$var:HasWireless`
  ` document.getElementById("uiViewSSID").value = decrypt(document.getElementById("uiPostSSID").value);
  jslDisable("uiViewSSID");
  ` ` ` ?>
  return true;
}
function uiDoCancel()
{
  jslGoTo(null,"home");
}
function uiDoRefresh()
{
  document.getElementById("uiPostForm").method="GET";
  document.getElementById("uiPostForm").submit();
}

function uiDoLogout()
{
  document.location.href="../cgi-bin/webcm?getpage=%2Fusr%2Fwww_safe%2Fhtml%2Fdefs%2F<? echo $var:style ?>%2Fmenus%2F<? echo $var:main ?>.html&var:style=<? echo $var:style ?>&var:main=<? echo $var:main ?>&var:pagename=logout&var:pagetitle=Log%20Out&var:menu=home&var:menutitle=Home";
}
</script>



