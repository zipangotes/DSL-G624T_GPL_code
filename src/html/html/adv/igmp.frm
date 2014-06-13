
<input type="hidden" name="igmpproxy:settings/state" value="<? query igmpproxy:settings/state ?>" id="uiPostIGMPStatus">

<? multiquery manager:command/lan/list_used 
      ` <input type="hidden" name="igmpproxy:settings/connection/$01/state" value="<? query igmpproxy:settings/connection/$01/state ?>" id="uiPostLanDirection$00">
      ` ?>
     
<? multiquery manager:command/connection/list_group 
      ` <input type="hidden" name="igmpproxy:settings/connection/$01/state" value="<? query igmpproxy:settings/connection/$01/state ?>" id="uiPostWanDirection$00">
      ` ?>
      
