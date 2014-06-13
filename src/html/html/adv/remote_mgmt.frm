<input type="hidden" name="fdb:settings/categoryH/acl_state" value="<? query fdb:settings/categoryH/acl_state ?>" id="uiPostRemoteManagementState">
<input type="hidden" name="fdb:settings/categoryH/acl/acl_add" value="" id="uiPostAddNewIP" disabled>
<input type="hidden" name="fdb:settings/categoryH/acl/acl_del" value="" id="uiPostDeleteIP" disabled>


<? multiquery fdb:settings/categoryH/acl/acl_services 
`<input type="hidden" name="$02" value="$03" id="uiPostServiceWAN$00">
<input type="hidden" name="$04" value="$05" id="uiPostServiceLAN$00"> ` ?>
