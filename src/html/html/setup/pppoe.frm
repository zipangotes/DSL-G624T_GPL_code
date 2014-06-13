
<input type="hidden" name="<? echo $var:conid ?>:settings/type" value="pppoe">
<input type="hidden" name="<? echo $var:encapsid ?>:settings/type" value="rfc2684" id="uiPostEncapsType">
<input type="hidden" name="<? echo $var:conid ?>:pppoe:settings/username"         value="<? query $var:conid:pppoe:settings/username ?>"         id="uiPostPppUsername">
<input type="hidden" name="<? echo $var:conid ?>:pppoe:settings/password"         value="<? query $var:conid:pppoe:settings/password ?>"         id="uiPostPppPassword">
<input type="hidden" name="<? echo $var:conid ?>:pppoe:settings/idle"             value="<? query $var:conid:pppoe:settings/idle ?>"             id="uiPostPppIdleTimeout">
<input type="hidden" name="<? echo $var:conid ?>:pppoe:settings/mru"          value="<? query $var:conid:pppoe:settings/mru ?>"          id="uiPostPppMRU">
<input type="hidden" name="<? echo $var:conid ?>:fwan:settings/private/mtu/state" value="<? query $var:conid:fwan:settings/private/mtu/state ?>" id="uiPostEnforceMRU">
<input type="hidden" name="<? echo $var:conid ?>:pppoe:settings/debug" value="<? query $var:conid:pppoe:settings/debug ?>" id="uiPostPppDebug">
<input type="hidden" name="<? echo $var:conid ?>:pppoe:settings/mode"             value="<? query $var:conid:pppoe:settings/mode ?>"             id="uiPostPppMode">
<input type="hidden" name="<? echo $var:conid ?>:pppoe:settings/lcp_echo_failure" value="<? query $var:conid:pppoe:settings/lcp_echo_failure ?>" id="uiPostPppKeepAlive">
<input type="hidden" name="<? echo $var:conid ?>:pppoe:settings/defaultroute"     value="<? query $var:conid:pppoe:settings/defaultroute ?>"     id="uiPostPppDefRoute">
<input type="hidden" name="<? echo $var:conid ?>:pppoe:command/start"           value=""           id="uiPostPppConnect" disabled>
<input type="hidden" name="<? echo $var:conid ?>:pppoe:command/stop"           value=""           id="uiPostPppDisconnect" disabled>
<input type="hidden" name="<? echo $var:conid ?>:pppoe:settings/auth_type"           value="<? query $var:conid:pppoe:settings/auth_type ?>"           id="uiPostPppAuthType">
<input type="hidden" name="<? echo $var:conid ?>:pppoe:settings/pppun_lan"           value="<? query $var:conid:pppoe:settings/pppun_lan ?>"           id="uiPostPppUnLan">
<input type="hidden" name="<? echo $var:conid ?>:pppoe:settings/ppp_unnum"           value="<? query $var:conid:pppoe:settings/ppp_unnum ?>"           id="uiPostPppUnNum">
<input type="hidden" name="logger:settings/loglevel" value="<? query logger:settings/loglevel ?>" id="uiPostDefaultLevel">
<input type="hidden" name="logger:settings/facility/user" value="<? query logger:settings/facility/user ?>" id="uiPostUserLevel">
<? if eq `1` `$var:HasDsl`
  `<? include /usr/www_safe/html/setup/pvc.frm ?>
  ` `` ?>
<? if eq `1` `$var:HasVlan` `<? include /usr/www_safe/html/setup/vlan.frm ?>` 
`<input type="hidden" name="<? echo $var:muxid ?>:settings/type"            value="disable"            id="uiPostVlanEnable"> ` ?>
<? include /usr/www_safe/html/setup/wst.frm ?>
