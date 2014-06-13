
<? if eq 1 1 '<script language="JavaScript">' '' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/encrypt.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/encrypt.js"></script>' ?>

<? include /usr/www_safe/html/wireless/wireless_mgmt.js ?>

var access_type=0;
function uiDoOnLoad()
{
	access_type = <? query ap:settings/access_type ?>
	<? multiquery ap:status/sta_table
	`document.getElementById("uiViewSSID$00").value = decrypt(document.getElementById("uiPostSSID$00").value);
	jslDisable("uiViewSSID$00");
    ` ?>
}

function uiDoBanStation(arg)
{
	if (access_type == '0')
	{
		alert ("Wireless Access List is not enabled. Cannot Ban station");
	}
	else
	{
		if(confirm("Ban Station ?"))
		{
			jslEnable("uiPostBanStation");
			document.getElementById("uiPostBanStation").value = arg;
			jslFormSubmit("uiPostForm");
		}
	}
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
