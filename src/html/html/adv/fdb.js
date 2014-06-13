
<script language="JavaScript">

<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

var uiVarProtoPortCount = <? multiquerycount fdb:settings/$var:category/$var:rule/protocolportlist ?>;

function uiDoOnLoad()
{
  <? if eq 1 <? query fdb:settings/$var:category/edit ?> `jslSetValue("uiViewRuleName", "uiPostRuleName");` `` ?>
  //doShowPageErrors();
  return true;
}
function uiDoValidate()
{
  value = document.getElementById("uiViewRuleName").value;
  if(value=="") {
  	alert("Empty rule name");
  	return false;
  }
  if(value.match("^[0-9]") != null) { alert("A name can not start with a number."); return false}
  if(value.match("[^0-9a-zA-Z_-]") != null) { alert("A name can only have alpha-numeric characters"); return false}
   else if((value.length == 0) || (value.length > 32)) { alert("A name must contain 1..32 characters"); return false}

  return true;
}

function uiDoSave()
{
  var i;

  if(uiDoValidate()==false) return false;
  jslSetValue("uiPostRuleName", "uiViewRuleName");
  jslSetValue("uiPostProtocol", "uiViewProtocol");
  jslSetValue("uiPostPortStart", "uiViewPortStart");
  jslSetValue("uiPostPortEnd", "uiViewPortEnd");
  jslSetValue("uiPostPortMap", "uiViewPortMap");
  for(i=0; i < uiVarProtoPortCount; i++ )
   document.getElementById("uiViewDeletePPCheckBox"+i).disabled = true;
  document.getElementById("uiViewApplyButton").disabled = true;
  document.getElementById("uiViewCancelButton").disabled = true;
  document.getElementById("uiPostForm").submit();
}
function uiDoCancel()
{
  var i = 0;
  var handle = null;

  <? if eq 1 <? query fdb:settings/$var:category/edit ?> `
  for(i=0; i < uiVarProtoPortCount; i++ )
  {
   handle = document.getElementById("uiViewDeletePPCheckBox"+i);
   if(handle!=null) handle.disabled=true;
  }
  document.getElementById("uiViewApplyButton").disabled = true;
  ` ?>

  document.getElementById("uiViewCancelButton").disabled = true;

  document.getElementById("uiPostForm").method="GET";
  jslSetValue("uiPostPageName","uiPostPageMaster");
  document.getElementById("uiPostForm").submit();
}
function uiDoToggleDelete(id)
{
  if(document.getElementById(id).disabled==true)
  {
    document.getElementById(id).disabled = false;
  }
  else
  {
    document.getElementById(id).disabled = true;
  }
}
</script>
