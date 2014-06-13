<input type="hidden" name="bridgeip:static:settings/ip" value="<? query bridgeip:settings/type '<? query bridgeip:$01:settings/ip ?>' ?>" id="uiPostIPAddr" disabled>
<input type="hidden" name="<? echo $var:conid ?>:settings/type" value="bridge">
<input type="hidden" name="<? echo $var:encapsid ?>:settings/type" value="rfc2684" id="uiPostEncapsType">
<? if eq `1` `$var:HasDsl`
`<input type="hidden" name="$var:encapsid:rfc2684:settings/encaps"   value="<? query $var:encapsid:rfc2684:settings/encaps ?>"   id="uiPostBridgeEncaps">
<? include /usr/www_safe/html/setup/pvc.frm ?>
` ` ` ?> 
<? if eq `1` `$var:HasVlan`
`<? include /usr/www_safe/html/setup/vlan.frm ?>
` ` <input type="hidden" name="<? echo $var:muxid ?>:settings/type"            value="disable"            id="uiPostVlanEnable">
` ?>
<? include /usr/www_safe/html/setup/wst.frm ?>
<input type="hidden" name="var:langrp" value="<? echo $var:langrp ?>" id="uiPostLanGrp">
<input type="hidden" name="manager:command/bridge" value="" id="uiPostLANBridgeCmd">
<input type="hidden" name="manager:command/lan/group/<? echo $var:conid ?>" value="<? query manager:command/lan/group/$var:conid ?>" id="uiPostLANBridge" disabled>

