
<? if eq 1 1 '<script language="JavaScript">' '' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? include /usr/www_safe/html/adv/wireless_mgmt.js ?>

function uiDoOnLoad()
{
	return true;
}

function uiDoBanStation(arg)
{
	jslEnable("uiPostBanStation");
	document.getElementById("uiPostBanStation").value = arg;
	jslFormSubmit("uiPostForm");
	return;
}

function uiDoSave()
{
	return;
}

function uiDoCancel()
{
	jslGoTo(null,"home");
}
</script>
