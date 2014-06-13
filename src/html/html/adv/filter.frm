
<input type="hidden" name="<? echo $var:langrp ?>:flan:command/add" value="" id="uiPostAdd">
<input type=hidden name="<? echo $var:langrp ?>:flan:settings/action" value="1" id="uiPostState" disabled>

  <? multiquery $var:langrp:flan:status/filter
  ' <input type="hidden" name="$var:langrp:flan:command/delete" value="$01" id="uiPostDelete$00" disabled>
  ' ?>
<input type="hidden" name="var:langrp" value="<? echo $var:langrp ?>" id="uiPostLanGrp">

