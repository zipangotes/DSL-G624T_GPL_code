
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/val.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/val.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

<? setvariable var:bandmode `<? query ap:settings/band_mode ?>` ?>

function uiDoValidate()
{
   var message;
   var value;

   return true;
}

function uiDoOnLoad()
{
       jslPostToViewCheckBox("uiViewSpectrumMgmt","uiPostSpectrumMgmt");
	   <? if eq `1` `<? query ap:status/dot11a_supported ?>`
       `jslPostToViewCheckBox("uiViewRadarDetect","uiPostRadarDetect");
       ` ` ` ?>
}

function uiDoSave()
{
       jslViewToPostCheckBox("uiPostSpectrumMgmt","uiViewSpectrumMgmt");
	   <? if eq `1` `<? query ap:status/dot11a_supported ?>`
       ` jslViewToPostCheckBox("uiPostRadarDetect","uiViewRadarDetect");
	   ` ` ` ?>

	if(uiDoValidate()==true) jslFormSubmit("uiPostForm");
	return;
}


function uiDoCancel()
{
     jslGoTo(null,"home");
}
</script>
