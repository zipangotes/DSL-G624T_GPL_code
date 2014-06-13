
<script language="JavaScript">

<? include /usr/www_safe/html/setup/wst.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>


function uiDoOnLoad()
{ 
   uiSetIGMPStatus (document.getElementById("uiPostIGMPStatus").value);
   
 <? multiquery manager:command/lan/list_used 
    ` jslDoShowComboBox("uiViewLanDirection$00", "uiPostLanDirection$00"); ` ?>
  <? multiquery manager:command/connection/list_group 
    ` jslDoShowComboBox("uiViewWanDirection$00", "uiPostWanDirection$00"); ` ?>      
}

function uiDoSave()
{  
	var retValue;
	
	retValue = IgmpValidate();
	
	if(retValue == 1) return;
	 
	 jslFormSubmit("uiPostForm");
}

var WanConnectionId = new Array();
var LanConnectionId = new Array();
  
var WanIdCount = 0;
var LanIdCount = 0;
  
	<? multiquery manager:command/connection/list_group 
			` <? if neq `bridge` `<? query $01:settings/type ?>` 
			` WanConnectionId[WanIdCount++]='uiViewWanDirection$00'; ` ` ` ?>
			 ` ?>
	<? multiquery manager:command/lan/list_used 
      ` LanConnectionId[LanIdCount++]='uiViewLanDirection$00'; ` ` ` ?>


function IgmpValidate()
{
	var tempUpStream = 0;
	var tempDownStream = 0;
	
	var wanIndex=0;
	var lanIndex=0;
	var tempWanValue = 0;
	var tempLanValue = 0;
	
	for(wanIndex = 0; wanIndex < WanIdCount; wanIndex++)
	{
		tempWanValue = document.getElementById(WanConnectionId[wanIndex]).value;
		if(tempWanValue == 0)
		  tempUpStream++;
		else if(tempWanValue == 1)		
			tempDownStream++;		
	}
		
	for(lanIndex = 0; lanIndex < LanIdCount; lanIndex++)
	{
		tempLanValue = document.getElementById(LanConnectionId[lanIndex]).value;
		if(tempLanValue == 0)
		  tempUpStream++;
		else if(tempLanValue == 1)		
			tempDownStream++;		
	}
	 
	if((tempUpStream > 0) && (tempDownStream > 0))
	{
		 return 0;
	}
	else
	{
		alert("There should be atleast One Upstream and One Downstream Interface");
		return 1;
	}
}

function uiDoCancel()
{
    jslGoTo(null,"home");
}

function uiDoChangeInterfaceDirection(val,id)
{	
	document.getElementById(id).value = val;	
}


function uiSetIGMPStatus(arg)
{
  if(arg=="1") 
    uiInitializeIGMPStatus('on');
  else
    uiInitializeIGMPStatus('off');
}

function uiInitializeIGMPStatus(arg)
{ 
  var checkbox = document.getElementById("uiViewIGMP");
  if (checkbox.value != arg)
    jslDoToggleCheckBox ("uiViewIGMP");
  if (arg == "on")
    document.getElementById("uiPostIGMPStatus").value = "1";
  else
    document.getElementById("uiPostIGMPStatus").value = "0";
}

function uiChangeIGMPStatus(arg)
{
  if (arg == "on")  
	document.getElementById("uiPostIGMPStatus").value = "1";
  else
    document.getElementById("uiPostIGMPStatus").value = "0";
}

</script>
