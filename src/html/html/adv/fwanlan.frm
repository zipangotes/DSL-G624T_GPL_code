
<input type="hidden" name="var:conid" value="<? echo $var:conid ?>" id="uiPostConId">
<input type="hidden" name="var:lanip" value="<? echo $var:lanip ?>" id="uiPostLanIp">
<input type="hidden" name="var:rule" value="<? echo $var:rule ?>" id="uiPostRule">
<input type="hidden" name="var:category" value="<? echo $var:category ?>" id="uiPostCategory">
<input type="hidden" name="fdb:settings/<? echo $var:category ?>/newrule" value="<? query fdb:settings/$var:category/newrule ?>" id="uiPostRuleNew" disabled>

<input type="hidden" name="<? if eq $var:pagename fwan `$var:conid:fwan` `$var:langrp:flan` ?>:command/<? echo $var:lanip ?>/" value="add" id="uiPostRuleAdd" disabled>
<input type="hidden" name="<? if eq $var:pagename fwan `$var:conid:fwan` `$var:langrp:flan` ?>:command/<? echo $var:lanip ?>/" value="delete" id="uiPostRuleDelete" disabled>

<? if eq `$var:pagename` fwan
`<input type="hidden" name="<? echo $var:conid ?>:fwan:settings/ping/state" value="<? query $var:conid:fwan:settings/ping/state ?>" id="uiPostPingState">`
`<input type="hidden" name="<? echo $var:langrp ?>:flan:settings/ping/state" value="<? query $var:langrp:flan:settings/ping/state ?>" id="uiPostPingState">` ?>
<input type="hidden" name="var:langrp" value="<? echo $var:langrp ?>" id="uiPostLanGrp">

