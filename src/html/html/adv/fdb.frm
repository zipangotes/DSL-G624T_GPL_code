

<input type="hidden" name="var:conid" value="<? echo $var:conid ?>" id="uiPostConId">
<input type="hidden" name="var:category" value="<? echo $var:category ?>" id="uiPostCategory">
<input type="hidden" name="var:rule" value="<? echo $var:rule ?>"  id="uiPostRule">

<input type="hidden" name="fdb:settings/<? echo $var:category ?>/<? echo $var:rule ?>/description" value="<? query fdb:settings/$var:category/$var:rule/description ?>"  id="uiPostRuleName">
<input type="hidden" name="fdb:settings/<? echo $var:category ?>/<? echo $var:rule ?>/<? query fdb:settings/$var:category/$var:rule/newprotocolport ?>/protocol" value="" id="uiPostProtocol">
<input type="hidden" name="fdb:settings/<? echo $var:category ?>/<? echo $var:rule ?>/<? query fdb:settings/$var:category/$var:rule/newprotocolport ?>/portstart" value="" id="uiPostPortStart">
<input type="hidden" name="fdb:settings/<? echo $var:category ?>/<? echo $var:rule ?>/<? query fdb:settings/$var:category/$var:rule/newprotocolport ?>/portend" value="" id="uiPostPortEnd">
<input type="hidden" name="fdb:settings/<? echo $var:category ?>/<? echo $var:rule ?>/<? query fdb:settings/$var:category/$var:rule/newprotocolport ?>/portmap" value="" id="uiPostPortMap">

<? multiquery fdb:settings/$var:category/$var:rule/protocolportlist
  ' 
   <input type="hidden" name="fdb:command/$var:category/$var:rule/$01" value="delete" id="uiPostDeletePP$00" disabled>
' ?>

