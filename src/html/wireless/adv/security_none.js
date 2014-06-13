<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

<? include /usr/www_safe/html/adv/wireless_security.js ?>

function uiDoValidate()
{
	return true;
}

function uiDoOnLoad()
{
	return;
}

function uiDoSave()
{
	document.getElementById("uiPostPrivacyType").value = 0;

	if(uiDoValidate()==true) jslFormSubmit("uiPostForm");
	return;
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}

</script>
