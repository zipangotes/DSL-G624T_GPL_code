
<input type="hidden" name="fdb:settings/lan_isolation/lanbridge0/lanbridge1"    value="<? query fdb:settings/lan_isolation/lanbridge0/lanbridge1 ?>"    id="uiPostLAN0LAN1">
<input type="hidden" name="fdb:settings/lan_isolation/lanbridge1/lanbridge0"    value="<? query fdb:settings/lan_isolation/lanbridge1/lanbridge0 ?>"    id="uiPostLAN1LAN0">
<? if eq `3` `<? multiquerycount manager:command/lan/list ?>`
`<input type="hidden" name="fdb:settings/lan_isolation/lanbridge0/lanbridge2"    value="<? query fdb:settings/lan_isolation/lanbridge0/lanbridge2 ?>"    id="uiPostLAN0LAN2">
<input type="hidden" name="fdb:settings/lan_isolation/lanbridge2/lanbridge0"    value="<? query fdb:settings/lan_isolation/lanbridge2/lanbridge0 ?>"    id="uiPostLAN2LAN0">
<input type="hidden" name="fdb:settings/lan_isolation/lanbridge1/lanbridge2"    value="<? query fdb:settings/lan_isolation/lanbridge1/lanbridge2 ?>"    id="uiPostLAN1LAN2">
<input type="hidden" name="fdb:settings/lan_isolation/lanbridge2/lanbridge1"    value="<? query fdb:settings/lan_isolation/lanbridge2/lanbridge1 ?>"    id="uiPostLAN2LAN1">
` ` ` ?>
