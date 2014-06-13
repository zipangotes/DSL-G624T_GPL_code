<input type="hidden" name="fdb:settings/categoryH/acl_state" value="<? query fdb:settings/categoryH/acl_state ?>" id="uiPostRemoteManagementState">
<input type="hidden" name="fdb:settings/categoryH/acl/acl_add" value="" id="uiPostAddNewIP" disabled>
<input type="hidden" name="fdb:settings/categoryH/acl/acl_del" value="" id="uiPostDeleteIP" disabled>


<? multiquery fdb:settings/categoryH/acl/acl_services/wan 
`<input type="hidden" name="$02" value="$03" id="uiPostWanService$00">
` ?>

  <? multiquery manager:command/lan/list_used 
     `<? multiquery fdb:settings/categoryH/acl/acl_services/$01
          ` <input type="hidden" name="$12" value="$13" id="uiPostLan$00Service$10">
          ` ?>
     ` ?>
