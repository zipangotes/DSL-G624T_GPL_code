<input type="hidden" name="<? query manager:status/$var:langrp/below ?>:settings/activated" value="<? query `<? query manager:status/$var:langrp/below ?>:settings/activated` ?>" id="uiPostBridgeFilterState">
<input type="hidden" name="<? query manager:status/$var:langrp/below ?>:settings/hidden_rules_en" value="<? query `<? query manager:status/$var:langrp/below ?>:settings/hidden_rules_en` ?>" id="uiPostBridgeFilterHiddenState">
<input type="hidden" name="<? query manager:status/$var:langrp/below ?>:settings/bf_mng_port" value="<? query `<? query manager:status/$var:langrp/below ?>:settings/bf_mng_port` ?>" id="uiPostBridgeFilterLANPort">
<input type="hidden" name="<? query manager:status/$var:langrp/below ?>:command/add" value="" id="uiPostAdd" disabled>
<input type="hidden" name="<? query manager:status/$var:langrp/below ?>:settings/entry" value="" id="uiPostEdit" disabled>
<input type="hidden" name="<? query manager:status/$var:langrp/below ?>:command/flush" value="" id="uiPostFlush" disabled>

<? multiquery <? query manager:status/$var:langrp/below ?>:settings/accesslist/array
  '<input type="hidden" value="$04" id="uiPostSrcPort$00" disabled></td>
   <input type="hidden" value="$02" id="uiPostDstPort$00" disabled></td>
  <input type="hidden" value="$07" id="uiPostSrcMac$00" disabled></td>
   <input type="hidden" value="$06" id="uiPostDstMac$00" disabled></td>
   <input type="hidden" value="$08" id="uiPostProtocol$00" disabled></td>
   <input type="hidden" value="$0a" id="uiPostAccess$00" disabled></td> 
  <input type="hidden" name = "<? query manager:status/$var:langrp/below ?>:command/delete"  value="entry$00" id="uiPostDelete$00" disabled>
  ' ?>
<input type="hidden" name="var:langrp" value="<? echo $var:langrp ?>" id="uiPostLanGrp">

  <? multiquery <? query manager:status/$var:langrp/below ?>:status/frame_type_list
  ' <input type="hidden" value="$01" id="uiPostProtoValue$00">
    <input type="hidden" value="$02" id="uiPostProtoText$00">
  ' ?>

