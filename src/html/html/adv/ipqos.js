
<script language="JavaScript">

<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

<? if eq 0 <? multiquerycount manager:command/connection/list_group ?> `<? setvariableonce var:qosconid lan0:qos ?> <? setvariableonce var:contype qoslan ?>` `` ?>
<? multiquery manager:command/connection/list_group `<? setvariableonce var:qosconid $03:qos ?> <? setvariableonce var:contype <? query $01:settings/type ?> ?>` ?>

function uiDoOnLoad()
{
       uiDoShowConnection(document.getElementById("uiPostConId").value);
	if(document.getElementById("uiPostMedWeight").value!="")
		jslSetValue("uiViewMedPriority","uiPostMedWeight");
	if(document.getElementById("uiPostLowWeight").value!="")
	       jslSetValue("uiViewLowPriority","uiPostLowWeight");	
	jslPostToViewCheckBox("uiViewIPQoS","uiPostIPQoS");
	jslPostToViewCheckBox("uiViewTrustedMode","uiPostTrustedMode");
}

function uiDoSave()
{
   if((parseInt(document.getElementById("uiViewMedPriority").value) + 
      parseInt(document.getElementById("uiViewLowPriority").value)) != 100) {
      alert("Medium and Low priority weights should sum up to 100%");
      return false;
   }
   jslSetValue("uiPostMedWeight","uiViewMedPriority");
   jslSetValue("uiPostLowWeight","uiViewLowPriority");	
   jslSetValue("uiPostQosConType","uiPostConType");
   jslFormSubmit("uiPostForm");
}

function uiDoCancel()
{
  jslGoTo("","home");
}
function uiDoIpQoSRule()
{
  if(document.getElementById("uiViewIPQoS").value=="on") jslGoTo("","ipqos_rule");
   else alert("Needs IPQoS in enabled state, Enable it and apply changes");
}

function uiDoSelectConnection(connection)
{
  jslFormSetGet("uiPostForm");
  switch(connection) {
  <? multiquery manager:command/connection/list_group `case "$03:qos" : document.getElementById("uiPostConType").value="<? query $01:settings/type ?>";break;` ?>
  <? multiquery manager:command/lan/list_used `case "$01:qos" : document.getElementById("uiPostConType").value="qoslan";break; ` ?>
  }
  document.getElementById("uiPostConId").value = connection;
  document.getElementById("uiPostIPQoS").name=connection+":settings/enabled";
  document.getElementById("uiPostMedWeight").name=connection+":settings/qw_med";
  document.getElementById("uiPostLowWeight").name=connection+":settings/qw_low";
  document.getElementById("uiPostTrustedMode").name=connection+":settings/trusted";
  jslFormSubmit("uiPostForm");
}

function uiDoShowConnection(connection)
{
  var i;
  var selector = document.getElementById("uiViewConnectionSelector");
  if(selector==null) return;

  for(i=0; i < selector.length; i++)
  {
    if(selector.options[i].value == connection)
	{
      selector.selectedIndex = i;
	  return(connection);
	}
  }
  return(selector.options[0].value);
}

function uiDoShowPriority(selector_id,priority)
{
  var i;
  var selector = document.getElementById(selector_id);
  if(selector==null) return;

  selector.value=priority;
  for(i=0; i < selector.length; i++)
  {
    if(selector.options[i].value == priority)
	{
      selector.selectedIndex = i;
	  return;
	}
  }
}

function uiDelete(context)
{
	if(context.value=="on") jslEnable("uiPostDelete"+context.name); 
	 else jslDisable("uiPostDelete"+context.name); 
}
</script>
