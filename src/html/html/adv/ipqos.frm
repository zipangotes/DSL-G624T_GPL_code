<input type="hidden" name="var:qosconid" value="<? echo $var:qosconid ?>" id="uiPostConId">
<input type="hidden" name="var:contype" value="<? echo $var:contype ?>" id="uiPostConType">

<input type="hidden" name="<? echo $var:qosconid ?>:settings/enabled" value="<? query $var:qosconid:settings/enabled ?>" id="uiPostIPQoS">
<input type="hidden" name="<? echo $var:qosconid ?>:settings/qw_med" value="<? query $var:qosconid:settings/qw_med ?>" id="uiPostMedWeight">
<input type="hidden" name="<? echo $var:qosconid ?>:settings/qw_low" value="<? query $var:qosconid:settings/qw_low ?>" id="uiPostLowWeight">
<input type="hidden" name="<? echo $var:qosconid ?>:settings/trusted" value="<? query $var:qosconid:settings/trusted ?>" id="uiPostTrustedMode">
<input type="hidden" name="<? echo $var:qosconid ?>:settings/qos_type" value="" id="uiPostQosConType">

<? multiquery $var:qosconid:status/rules 
'<input type="hidden" name="<? echo $var:qosconid ?>:command/delete" value="$01" id="uiPostDelete$00" disabled>
' ?>
           
<input type="hidden" name="var:langrp" value="<? echo $var:langrp ?>" id="uiPostLanGrp">
           
