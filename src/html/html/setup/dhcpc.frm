
<input type="hidden" name="<? echo $var:conid ?>:settings/type" value="dhcpc" id="uiPostConnType">
<input type="hidden" name="<? echo $var:conid ?>:dhcpc:settings/defaultroute" value="<? query $var:conid:dhcpc:settings/defaultroute ?>" id="uiPostSetRoute">
<input type="hidden" name="<? echo $var:encapsid ?>:settings/type" value="rfc2684" id="uiPostEncapsType">
<? if eq `1` `$var:HasDsl` 
`<input type="hidden" name="$var:encapsid:rfc2684:settings/encaps"   value="<? query $var:encapsid:rfc2684:settings/encaps ?>"   id="uiPostDHCPEncaps">` 
` 
` ?>

<input type="hidden" name="<? echo $var:conid ?>:dhcpc:command/renew" value="" id="uiPostRenew" disabled>
<input type="hidden" name="<? echo $var:conid ?>:dhcpc:command/release" value="" id="uiPostRelease"  disabled>
<? if eq `1` `$var:HasDsl` `<? include /usr/www_safe/html/setup/pvc.frm ?>` ` ` ?>
<? if eq `1` `$var:HasVlan` `<? include /usr/www_safe/html/setup/vlan.frm ?>` 
`<input type="hidden" name="<? echo $var:muxid ?>:settings/type"            value="disable"            id="uiPostVlanEnable"> ` ?>
<? include /usr/www_safe/html/setup/wst.frm ?>
