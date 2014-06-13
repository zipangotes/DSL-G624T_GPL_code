
<input type="hidden" name="<? echo $var:muxid ?>:settings/type"            value="<? query $var:muxid:settings/type ?>"            id="uiPostVlanEnable">
<? if eq pppoa $var:contype 
  `
    <input type="hidden" name="<? echo $var:muxid ?>:settings/vlan_id"            value="<? query $var:muxid:wbridge:settings/vlan_id ?>"            id="uiPostVlanId">
  ` `
    <? if eq clip $var:contype 
      `
         <input type="hidden" name="<? echo $var:muxid ?>:settings/vlan_id"            value="<? query $var:muxid:wbridge:settings/vlan_id ?>"            id="uiPostVlanId">
      ` ` 
<input type="hidden" name="<? echo $var:muxid ?>:wbridge:settings/vlan_id"            value="<? query $var:muxid:wbridge:settings/vlan_id ?>"            id="uiPostVlanId">
      ` ?>
   ` ?>
<input type="hidden" name="<? echo $var:muxid ?>:wbridge:settings/priority_bit" value="<? query $var:muxid:wbridge:settings/priority_bit ?>"            id="uiPostPbits">

