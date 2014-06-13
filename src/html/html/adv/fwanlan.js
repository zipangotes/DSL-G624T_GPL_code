
<script language="JavaScript">

<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

<? multiquery manager:command/connection/list <? setvariableonce var:conid $01 ?> ?>
<? multiquery fdb:settings/categorylist <? setvariableonce var:category $01 ?> ?>
function uiDoOnLoad()
{
  <? if eq $var:pagename fwan
  `document.getElementById("uiPostConId").value = uiDoShowConnection(document.getElementById("uiPostConId").value);`
  `jslPostToViewCheckBox("uiViewTrafficTypeAny","uiPostTrafficTypeAny");` ?>
  document.getElementById("uiPostCategory").value = uiDoShowFdbCategory(document.getElementById("uiPostCategory").value,<? query fdb:settings/categorycount ?>);
  document.getElementById("uiPostLanIp").value = uiDoShowLanIp(document.getElementById("uiPostLanIp").value);
  document.getElementById("uiPostPageMaster").value="<? echo $var:pagename ?>"; 
  <? if eq `$var:pagename` flan `uiDoChangeTraffic("uiViewTrafficTypeAny");` ?>
  jslPostToViewCheckBox("uiViewPingState","uiPostPingState");
  jslDoShowComboBox("uiViewLanGrp","uiPostLanGrp");
  //doShowPageErrors();
}
function uiDoSave()
{
  <? if eq $var:pagename flan
  `jslViewToPostCheckBox("uiPostTrafficTypeAny","uiViewTrafficTypeAny");`
   ?>
   jslViewToPostCheckBox("uiPostPingState","uiViewPingState");
   jslFormSubmit("uiPostForm");
}
function uiDoCancel()
{
  jslGoTo("","home");
}

function uiDoSelectLANGrp(value)
{
  switch(value) {
	case "lan1":
				<? if eq `0` `<? multiquerycount <? query manager:status/lan1/dhcps ?>:status/hostlist ?>`
				    `javascript:alert('$var:advMsgBlockLanc');return;
				    ` `<? multiquery <? query manager:status/lan1/dhcps ?>:status/hostlist `document.getElementById("uiPostLanIp").value="$12";` ?>` ?>
				break;
	case "lan2":
				<? if eq `0` `<? multiquerycount <? query manager:status/lan2/dhcps ?>:status/hostlist ?>`
				    `javascript:alert('$var:advMsgBlockLanc');return;
				    ` `<? multiquery <? query manager:status/lan2/dhcps ?>:status/hostlist `document.getElementById("uiPostLanIp").value="$12";` ?>` ?>
				break;
	case "lan0":
	default:
				<? if eq `0` `<? multiquerycount <? query manager:status/lan0/dhcps ?>:status/hostlist ?>`
				    `javascript:alert('$var:advMsgBlockLanc');return;
				    ` `<? multiquery <? query manager:status/lan0/dhcps ?>:status/hostlist `document.getElementById("uiPostLanIp").value="$12";` ?>` ?>
				break;				
  }
  document.getElementById("uiPostLanGrp").value=value;
  jslFormSetGet("uiPostForm");
  document.getElementById("uiPostPageName").value="<? echo $var:pagename ?>";
  jslFormSubmit("uiPostForm");
}

function uiDoSelectConnection(connection)
{
  jslFormSetGet("uiPostForm");
  document.getElementById("uiPostConId").value = connection;
  jslFormSubmit("uiPostForm");
}
function uiDoSelectLanIp(lanip)
{
  jslFormSetGet("uiPostForm");
  document.getElementById("uiPostLanIp").value = lanip;
  jslFormSubmit("uiPostForm");
}
function uiDoLanClientPage()
{
  jslFormSetGet("uiPostForm");
  document.getElementById("uiPostPageName").value="lan_clients";
  document.getElementById("uiPostForm").submit();
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
function uiDoShowLanIp(lanip)
{
  var i;
  var selector = document.getElementById("uiViewLanIpSelector");
  if(selector==null) return;

  for(i=0; i < selector.length; i++)
  {
    if(selector.options[i].value == lanip)
	{
      selector.selectedIndex = i;
	  return(lanip);
	}
  }

  return(selector.options[0].value);
}
function uiDoSelectFdbCategory(category)
{
  jslFormSetGet("uiPostForm");
  document.getElementById("uiPostPageName").value="<? echo $var:pagename ?>";
  document.getElementById("uiPostCategory").value = category;
  jslFormSubmit("uiPostForm");
}
function uiDoShowFdbCategory(category,max)
{
  var i;
  var selector;

  for(i=0; i < max; i++)
  {
    selector=document.getElementById("uiViewFdbCategory"+i);
	if(selector!=null)
	{
      if(selector.name == category)
	  {
	    jslDoToggleRadio('uiViewFdbCategory',i,max);
	    return(category);
	  }
	}
  }
  jslDoToggleRadio('uiViewFdbCategory',0,max);
  return(document.getElementById("uiViewFdbCategory0").name);
}
function uiDoNewFdbRule()
{
   <? if eq $var:pagename flan
       `
          if(document.getElementById("uiViewTrafficTypeAny").value=="off")  {
	  	jslFormSetGet("uiPostForm");
	  	document.getElementById("uiPostPageName").value="fdb";
	  	jslSetValue("uiPostRule","uiPostRuleNew");
	  	jslFormSubmit("uiPostForm");
   	   } else alert("Uncheck block all traffic and apply before adding new rule");
       `
   	`
	  	jslFormSetGet("uiPostForm");
	  	document.getElementById("uiPostPageName").value="fdb";
	  	jslSetValue("uiPostRule","uiPostRuleNew");
	  	jslFormSubmit("uiPostForm");
   	`
    ?>
}
function uiDoEditFdbRule()
{
  var selector = document.getElementById("uiViewFdbRules");

  if((selector.selectedIndex >= 0) && (selector.value!="invalid"))
  {
    jslFormSetGet("uiPostForm");
    document.getElementById("uiPostPageName").value="fdb";
    jslSetValue("uiPostRule","uiViewFdbRules");
    //document.getElementById("uiPostRule").value = selector.value;
    jslFormSubmit("uiPostForm");
  }
  else
  {
   <? if eq $var:pagename flan
	`if(document.getElementById("uiViewTrafficTypeAny").value=="off")  {
	    alert("Please, select an FDB rule to <? if eq 1 <? query fdb:settings/$var:category/edit ?> `edit` `view` ?>.");
	  } else alert("Uncheck block all traffic and apply before <? if eq 1 <? query fdb:settings/$var:category/edit ?> `edit` `view` ?>.");
	`
	`alert("Please, select an FDB rule to <? if eq 1 <? query fdb:settings/$var:category/edit ?> `edit` `view` ?>.");`
   ?>
  }
}
function uiDoDeleteFdbRule()
{
  var selector = document.getElementById("uiViewFdbRules");

  if((selector.selectedIndex >= 0) && (selector.value!="invalid"))
  {
    jslFormSetPost("uiPostForm");
    document.getElementById("uiPostRule").name  = "fdb:command/<? echo $var:category ?>/" + selector.value;
    document.getElementById("uiPostRule").value = "delete";
    jslFormSubmit("uiPostForm");
  }
  else
  {
   <? if eq $var:pagename flan
	`if(document.getElementById("uiViewTrafficTypeAny").value=="off")  {
	       alert("Please, select an FDB rule to delete.");
	} else alert("Uncheck block all traffic and apply before delete");	
	`
	`alert("Please, select an FDB rule to delete.");` ?>
  }
}
function uiDoAddRule()
{
  var selector = document.getElementById("uiViewFdbRules");

  if((selector.selectedIndex >= 0) && (selector.value!="invalid"))
  {
    document.getElementById("uiPostRuleAdd").name = document.getElementById("uiPostRuleAdd").name + selector.value;
    document.getElementById("uiPostRuleAdd").disabled = false;
    document.getElementById("uiPostRuleDelete").disabled = true;
    document.getElementById("uiPostForm").submit();
  }
  else {
   <? if eq $var:pagename flan
	`if(document.getElementById("uiViewTrafficTypeAny").value=="off")  {
	  	alert("Please select an Available Rule to add.");
	} else alert("Uncheck block all traffic and apply before add");	
	`
	`alert("Please select an Available Rule to add.");`
   ?>
  }
}
function uiDoDeleteRule()
{
  var selector = document.getElementById("uiViewLanIpRules");
  if((selector.selectedIndex >= 0) && (selector.value!="invalid"))
  {
    document.getElementById("uiPostRuleDelete").name = document.getElementById("uiPostRuleDelete").name + selector.options[selector.selectedIndex].value;
    document.getElementById("uiPostRuleAdd").disabled = true;
    document.getElementById("uiPostRuleDelete").disabled = false;
    document.getElementById("uiPostForm").submit();
  }
  else {
   <? if eq $var:pagename flan
	`
	if(document.getElementById("uiViewTrafficTypeAny").value=="off")  {
	  	alert("Please select an Available Rule to delete.");
	} else alert("Uncheck block all traffic and apply before delete");
	`
	`alert("Please select an Available Rule to delete.");`
   ?>
  }
}
function uiDoChangeTraffic(id)
{
	switch(document.getElementById(id).value)  {
		case "on":
			 <? multiquery fdb:settings/categorylist
			 ` document.getElementById("uiViewFdbCategory$00").disabled = true;
			 ` ?>
			document.getElementById("uiViewFdbRules").disabled = true;
			document.getElementById("uiViewLanIpRules").disabled = true;
			break;
		case "off":
			 <? multiquery fdb:settings/categorylist
			 ` document.getElementById("uiViewFdbCategory$00").disabled = false;
			 ` ?>
			document.getElementById("uiViewFdbRules").disabled = false;
			document.getElementById("uiViewLanIpRules").disabled = false;
			break;
	}
}
</script>
