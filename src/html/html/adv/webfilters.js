
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>
function uiDoOnLoad()
{
	uiInitRadioButtons("uiViewProxy","uiPostProxy");
	uiInitRadioButtons("uiViewCookies","uiPostCookies");
	uiInitRadioButtons("uiViewJavaApp","uiPostJavaApp");
	uiInitRadioButtons("uiViewActiveX","uiPostActiveX");
	uiInitRadioButtons("uiViewPopups","uiPostPopups");
}

function uiInitRadioButtons(ViewId,PostId)
{
	if(document.getElementById(PostId).value=='1') {
		jslDoToggleRadio(ViewId,0,2)
	} else {
		jslDoToggleRadio(ViewId,1,2)
	}
}

function uiSetRadioButtons(PostId,ViewId)
{
	if(document.getElementById(ViewId).value=="on") {
		document.getElementById(PostId).value='1';
	} else {
		document.getElementById(PostId).value='0';
	}
}

// ********************* Visual Page Control ***********************

function uiDoSave()
{
	uiSetRadioButtons("uiPostProxy","uiViewProxy0");
	uiSetRadioButtons("uiPostCookies","uiViewCookies0");
	uiSetRadioButtons("uiPostJavaApp","uiViewJavaApp0");
	uiSetRadioButtons("uiPostActiveX","uiViewActiveX0");
	uiSetRadioButtons("uiPostPopups","uiViewPopups0");
  	document.getElementById("uiPostForm").submit();
}

function uiDoCancel()
{
	  jslGoTo(null,"home");
}


</script>
