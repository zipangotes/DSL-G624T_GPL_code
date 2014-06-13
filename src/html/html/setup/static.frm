
<input type="hidden" name="<? echo $var:conid ?>:settings/type"                   value="static">
<input type="hidden" name="<? echo $var:encapsid ?>:settings/type"                   value="rfc2684" id="uiPostEncapsType">
<? if eq `1` `$var:HasDsl`
 `<input type="hidden" name="<? echo $var:encapsid ?>:rfc2684:settings/encaps"         value="<? query $var:encapsid:rfc2684:settings/encaps ?>"         id="uiPostStaticEncaps">
 ` ` ` ?>
<input type="hidden" name="<? echo $var:conid ?>:static:settings/ip"              value="<? query $var:conid:static:settings/ip ?>"              id="uiPostStaticIpAddress">
<input type="hidden" name="<? echo $var:conid ?>:static:settings/netmask"         value="<? query $var:conid:static:settings/netmask ?>"         id="uiPostStaticNetMask">
<? if eq `1` `$var:HasVoice` 
`<input type="hidden" name="<? echo $var:conid ?>:static:settings/gw"                                  value="<? query $var:conid:static:settings/gw ?>"                       id="uiPostStaticConnGateway">
` ` ` ?>
<input type="hidden" name="resolver:settings/gw"                                  value="<? query resolver:settings/gw ?>"                       id="uiPostStaticGateway">
<input type="hidden" name="resolver:settings/nameserver1"                         value="<? query resolver:settings/nameserver1 ?>"              id="uiPostStaticDns1">
<input type="hidden" name="resolver:settings/nameserver2"                         value="<? query resolver:settings/nameserver2 ?>"              id="uiPostStaticDns2">
<input type="hidden" name="resolver:settings/nameserver3"                         value="<? query resolver:settings/nameserver3 ?>"              id="uiPostStaticDns3">
<? if eq `1` `$var:HasDsl`
 `<? include /usr/www_safe/html/setup/pvc.frm ?>
 ` ` ` ?>
<? if eq `1` `$var:HasVlan` `<? include /usr/www_safe/html/setup/vlan.frm ?>` 
`<input type="hidden" name="<? echo $var:muxid ?>:settings/type"            value="disable"            id="uiPostVlanEnable"> ` ?>
<? include /usr/www_safe/html/setup/wst.frm ?>
