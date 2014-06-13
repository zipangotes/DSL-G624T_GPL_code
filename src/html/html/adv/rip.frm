

    <input type="hidden" name="rip:settings/version" value="<? query rip:settings/version ?>" id="uiPostRipVersion">
    <? multiquery manager:command/lan/list_used 
      ` <input type="hidden" name="rip:settings/interface/$01/options" value="<? query rip:settings/interface/$01/options ?>" id="uiPostLanDirection$00">
      ` ?>
   <? multiquery manager:command/connection/list_group 
      ` <input type="hidden" name="rip:settings/interface/$01/options" value="<? query rip:settings/interface/$01/options ?>" id="uiPostWanDirection$00">
      ` ?>
    <input type="hidden" name="rip:settings/pwdreqd" value="<? query rip:settings/pwdreqd ?>" id="uiPostRipPasswordReq">
    <input type="hidden" name="rip:settings/password" value="<? query rip:settings/password ?>" id="uiPostRipPassword">
    <input type="hidden" name="rip:settings/state" value="<? query rip:settings/state  ?>" id="uiPostRipStatus">


