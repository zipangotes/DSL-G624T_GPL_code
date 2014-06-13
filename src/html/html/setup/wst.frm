
<input type="hidden" name="<? echo $var:conid ?>:settings/description" value="<? query $var:conid:settings/description ?>" id="uiPostConDesc">
<? if eq $var:contype bridge `` `
<input type="hidden" name="fdb:settings/service/state" value="<? query fdb:settings/service/state ?>" id="uiPostFwanNATState" disabled>
<input type="hidden" name="<? echo $var:conid ?>:fwan:settings/private/nat/state" value="<? query $var:conid:fwan:settings/private/nat/state ?>" id="uiPostNatControl">
<input type="hidden" name="<? echo $var:conid ?>:fwan:settings/private/spi/state" value="<? query $var:conid:fwan:settings/private/spi/state ?>" id="uiPostFirewallControl">
` ?>
<input type="hidden" name="manager:command/delete_group" value="<? echo $var:conid ?>|<? echo $var:muxid ?>|<? echo $var:encapsid ?>" id="uiPostDelete" disabled>
<input type="hidden" name="var:conid" value="<? echo $var:conid ?>" id="uiPostConId">
<input type="hidden" name="var:muxid" value="<? echo $var:muxid ?>" id="uiPostMuxId">
<input type="hidden" name="var:encapsid" value="<? echo $var:encapsid ?>" id="uiPostEncapsId">
<input type="hidden" name="manager:command/bind"             value=""            id="uiPostBind" disabled>

<input type="hidden" name="" value="" id="uiPostDeleteSwitch" disabled>
<input type="hidden" name="" value="" id="uiPostDeleteSwitchIPAddr" disabled>

