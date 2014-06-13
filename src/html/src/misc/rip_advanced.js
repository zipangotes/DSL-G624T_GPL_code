
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/riptable.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>
function gotoBasicPage()
{
	page = "../cgi-bin/webcm?getpage=/usr/www_safe/html/menus/menu1_adv_rip_setup.html";
	document.location.href = page;  
}

function gotoAdvancedPage()
{
	page = "../cgi-bin/webcm?getpage=/usr/www_safe/html/menus/menu1_adv_rip_table.html";
	document.location.href = page;  
}
</script>
