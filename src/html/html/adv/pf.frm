
<input type="hidden" name="var:conid" value="<? echo $var:conid ?>" id="uiPostConId">
<input type="hidden" name="<? echo $var:conid ?>:fwan:command/add" value="" id="uiPostAdd">
<input type=hidden name="<? echo $var:conid ?>:fwan:settings/action" value="1" id="uiPostState" disabled>

<? multiquery $var:conid:fwan:status/pf
' <input type="hidden" name="$var:conid:fwan:command/delete" value="$01" id="uiPostDelete$00" disabled>
' ?>

