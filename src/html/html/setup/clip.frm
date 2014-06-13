
<input type="hidden" name="<? echo $var:conid ?>:settings/type" value="static">
<input type="hidden" name="<? echo $var:encapsid ?>:settings/type" value="clip" id="uiPostEncapsType">
<input type="hidden" name="<? echo $var:encapsid ?>:clip:settings/ip" value="<? query $var:encapsid:clip:settings/ip ?>" id="uiPostIpAddress">
<input type="hidden" name="<? echo $var:encapsid ?>:clip:settings/netmask" value="<? query $var:encapsid:clip:settings/netmask ?>" id="uiPostMask">
<input type="hidden" name="<? echo $var:encapsid ?>:clip:settings/gateway" value="<? query $var:encapsid:clip:settings/gateway ?>" id="uiPostATMARPGateway">

<input type="hidden" name="<? echo $var:conid ?>:static:settings/ip" value="<? query $var:conid:static:settings/ip ?>" id="uiPostStaticIpAddress">

<input type="hidden" name="<? echo $var:conid ?>:static:settings/netmask" value="<? query $var:conid:static:settings/ip ?>" id="uiPostStaticMask">
<input type="hidden" name="resolver:settings/gw" value="<? query resolver:settings/gw ?>" id="uiPostDefaultGateway">
<? include /usr/www_safe/html/setup/pvc.frm ?>
<? if eq `1` `$var:HasVlan` `<? include /usr/www_safe/html/setup/vlan.frm ?>` 
`<input type="hidden" name="<? echo $var:muxid ?>:settings/type"            value="disable"            id="uiPostVlanEnable"> ` ?>
<? include /usr/www_safe/html/setup/wst.frm ?>
