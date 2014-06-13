<? include /usr/www_safe/html/js/blank.js ?>
function uiDoNone(arg)
{
	document.getElementById("uiPostForm").method="GET";
	document.getElementById("uiPostPageName").value="security_none";
	document.getElementById("uiPostForm").submit();
}

function uiDoWepPage(arg)
{
	document.getElementById("uiPostForm").method="GET";
	document.getElementById("uiPostPageName").value="security_wep";
	document.getElementById("uiPostForm").submit();
}

function uiDo8021xPage(arg)
{
	document.getElementById("uiPostForm").method="GET";
	document.getElementById("uiPostPageName").value="security_8021x";
	document.getElementById("uiPostForm").submit();
}

function uiDoWPAPage(arg)
{
	document.getElementById("uiPostForm").method="GET";
	document.getElementById("uiPostPageName").value="security_wpa";
	document.getElementById("uiPostForm").submit();
}

