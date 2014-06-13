
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/val.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/val.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

function uiDoOnLoad()
{

  jslPostToViewCheckBox("uiViewSNMPAgent","uiPostSNMPAgent");	
  jslPostToViewCheckBox("uiViewSNMPTrap","uiPostSNMPTrap");	

  <? multiquery snmpcm:settings/community/entry
  ' 
  jslSetValue("uiViewName$00","uiPostName$00");
  jslSetValue("uiViewAccessRights$00","uiPostAccessRights$00");
  jslDoShowComboBox("uiViewAccessRights$00","uiPostAccessRights$00");
   ' ?>

  <? multiquery snmpcm:settings/trapinfo/entry
  ' 
  jslSetValue("uiViewTrapDest$00","uiPostTrapDest$00");
  jslSetValue("uiViewTrapComm$00","uiPostTrapComm$00");
  jslSetValue("uiViewTrapVer$00","uiPostTrapVer$00");
  jslDoShowComboBox("uiViewTrapVer$00","uiPostTrapVer$00");
   ' ?>

   jslSetValue("uiViewSysContact","uiPostSysContact");
   jslSetValue("uiViewSysName","uiPostSysName");
   jslSetValue("uiViewSysLocation","uiPostSysLocation");
  
  // jslSetValue("uiViewSysIdleTime","uiPostSysIdleTime");
  //doShowPageErrors();
}
function uiDoSave()
{
  <? multiquery snmpcm:settings/community/entry
  ' 
  jslSetValue("uiPostName$00","uiViewName$00");
  jslSetValue("uiPostAccessRights$00","uiViewAccessRights$00");
   ' ?>

  <? multiquery snmpcm:settings/trapinfo/entry
  ' 
  jslSetValue("uiPostTrapDest$00","uiViewTrapDest$00");
  jslSetValue("uiPostTrapComm$00","uiViewTrapComm$00");
  jslSetValue("uiPostTrapVer$00","uiViewTrapVer$00");
   ' ?>

  jslSetValue("uiPostSysContact","uiViewSysContact");
  jslSetValue("uiPostSysName","uiViewSysName");
  jslSetValue("uiPostSysLocation","uiViewSysLocation");

  // jslSetValue("uiPostSysIdleTime","uiViewSysIdleTime");

  jslViewToPostCheckBox("uiPostSNMPAgent","uiViewSNMPAgent");
  jslViewToPostCheckBox("uiPostSNMPTrap","uiViewSNMPTrap");

  jslFormSubmit("uiPostForm");
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}
</script>
