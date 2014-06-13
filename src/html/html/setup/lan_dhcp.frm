
<input type="hidden" name="<? query manager:status/$var:langrp/dhcps ?>:settings/start" value="<? query <? query manager:status/$var:langrp/dhcps ?>:settings/start ?>" id="uiPostStartIP">
<input type="hidden" name="<? query manager:status/$var:langrp/dhcps ?>:settings/end" value="<? query <? query manager:status/$var:langrp/dhcps ?>:settings/end ?>" id="uiPostEndIP">
<input type="hidden" name="<? query manager:status/$var:langrp/dhcps ?>:settings/lease" value="<? query <? query manager:status/$var:langrp/dhcps ?>:settings/lease ?>" id="uiPostLease">
<input type="hidden" name="<? query manager:status/$var:langrp/relay ?>:settings/ip" value="<? query <? query manager:status/$var:langrp/relay ?>:settings/ip ?>" id="uiPostRelayIP">

<input type="hidden" name="<? query manager:status/$var:langrp/dhcps ?>:settings/state" value="" id="uiPostServerFirst" disabled>
<input type="hidden" name="<? query manager:status/$var:langrp/relay ?>:settings/state" value="" id="uiPostRelaySecond" disabled>

<input type="hidden" name="<? query manager:status/$var:langrp/relay ?>:settings/state" value="" id="uiPostRelayFirst" >
<input type="hidden" name="<? query manager:status/$var:langrp/dhcps ?>:settings/state" value="" id="uiPostServerSecond" >

<input type="hidden" name="var:langrp" value="<? echo $var:langrp ?>" id="uiPostLanGrp">

