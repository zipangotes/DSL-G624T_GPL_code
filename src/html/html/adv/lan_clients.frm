<input type="hidden" name="<? query manager:status/$var:langrp/dhcps ?>:settings/<? query <? query manager:status/$var:langrp/dhcps ?>:settings/newid ?>/ip" value="" id="uiPostNewIP">
<input type="hidden" name="<? query manager:status/$var:langrp/dhcps ?>:settings/<? query <? query manager:status/$var:langrp/dhcps ?>:settings/newid ?>/hname" value="" id="uiPostNewHostname">
<input type="hidden" name="<? query manager:status/$var:langrp/dhcps ?>:settings/<? query <? query manager:status/$var:langrp/dhcps ?>:settings/newid ?>/mac" value="" id="uiPostNewMAC">

<? multiquery <? query manager:status/$var:langrp/dhcps ?>:status/staticlist
  ' <input type="hidden" name="<? query manager:status/$var:langrp/dhcps ?>:command/del" value="$01" id="uiPostDeletePP$00" disabled>
' ?>

<? multiquery <? query manager:status/$var:langrp/dhcps ?>:status/dynamiclist
  ' <input type="hidden" name="<? query manager:status/$var:langrp/dhcps ?>:command/reserve" value="$01" id="uiPostReservePP$00" disabled>
' ?>
<input type="hidden" name="var:langrp" value="<? echo $var:langrp ?>" id="uiPostLanGrp">

