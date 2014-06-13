
<input type="hidden" name="<? echo $var:conid ?>:settings/type" value="asis" id="uiPostConType">
<input type="hidden" name="<? echo $var:encapsid ?>:settings/type" value="pppoa" id="uiPostEncapsType">
<input type="hidden" name="<? echo $var:encapsid ?>:pppoa:settings/username"         value="<? query $var:encapsid:pppoa:settings/username ?>"         id="uiPostPppUsername">
<input type="hidden" name="<? echo $var:encapsid ?>:pppoa:settings/password"         value="<? query $var:encapsid:pppoa:settings/password ?>"         id="uiPostPppPassword">
<input type="hidden" name="<? echo $var:encapsid ?>:pppoa:settings/idle"             value="<? query $var:encapsid:pppoa:settings/idle ?>"             id="uiPostPppIdleTimeout">
<input type="hidden" name="<? echo $var:encapsid ?>:pppoa:settings/mru"          value="<? query $var:encapsid:pppoa:settings/mru ?>"          id="uiPostPppMRU">
<input type="hidden" name="<? echo $var:encapsid ?>:pppoa:settings/debug" value="<? query $var:encapsid:pppoa:settings/debug ?>" id="uiPostPppDebug">
<input type="hidden" name="<? echo $var:encapsid ?>:pppoa:settings/mode"             value="<? query $var:encapsid:pppoa:settings/mode ?>"             id="uiPostPppMode">
<input type="hidden" name="<? echo $var:encapsid ?>:pppoa:settings/lcp_echo_failure" value="<? query $var:encapsid:pppoa:settings/lcp_echo_failure ?>" id="uiPostPppKeepAlive">
<input type="hidden" name="<? echo $var:encapsid ?>:pppoa:settings/defaultroute"     value="<? query $var:encapsid:pppoa:settings/defaultroute ?>"     id="uiPostPppDefRoute">
<input type="hidden" name="<? echo $var:encapsid ?>:pppoa:settings/encaps"           value="<? query $var:encapsid:pppoa:settings/encaps ?>"           id="uiPostPppEncaps">
<input type="hidden" name="<? echo $var:encapsid ?>:pppoa:command/start"           value=""           id="uiPostPppConnect" disabled>
<input type="hidden" name="<? echo $var:encapsid ?>:pppoa:command/stop"           value=""           id="uiPostPppDisconnect" disabled>
<input type="hidden" name="<? echo $var:encapsid ?>:pppoa:settings/auth_type"           value="<? query $var:encapsid:pppoa:settings/auth_type ?>"           id="uiPostPppAuthType">
<input type="hidden" name="<? echo $var:encapsid ?>:pppoa:settings/pppun_lan"           value="<? query $var:encapsid:pppoa:settings/pppun_lan ?>"           id="uiPostPppUnLan">
<input type="hidden" name="<? echo $var:encapsid ?>:pppoa:settings/ppp_unnum"           value="<? query $var:encapsid:pppoa:settings/ppp_unnum ?>"           id="uiPostPppUnNum">
<input type="hidden" name="logger:settings/loglevel" value="<? query logger:settings/loglevel ?>" id="uiPostDefaultLevel">
<input type="hidden" name="logger:settings/facility/user" value="<? query logger:settings/facility/user ?>" id="uiPostUserLevel">
<? include /usr/www_safe/html/setup/pvc.frm ?>
<? if eq `1` `$var:HasVlan` `<? include /usr/www_safe/html/setup/vlan.frm ?>` 
`<input type="hidden" name="<? echo $var:muxid ?>:settings/type"            value="disable"            id="uiPostVlanEnable"> ` ?>
<? include /usr/www_safe/html/setup/wst.frm ?>
