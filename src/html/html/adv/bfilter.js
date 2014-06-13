<script language="JavaScript">
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

var NoofEntries=0;
var LastSelected=-1;
var Modified=0;

function uiDoOnLoad()
{
  var i;

  jslPostToViewCheckBox("uiViewBridgeFilterStatus","uiPostBridgeFilterState");	
  jslPostToViewCheckBox("uiViewBridgeFilterHiddenStatus","uiPostBridgeFilterHiddenState");	
  NoofEntries=<? multiquerycount <? query manager:status/$var:langrp/below ?>:settings/accesslist/array ?>;
  jslDoShowComboBox("uiViewBridgeFilterLANPort","uiPostBridgeFilterLANPort");
  if(Number(NoofEntries) >= 20) jslDisable("uiViewAdd"); 
  document.getElementById("uiEditSelectProtocol").selectedIndex=0;
  document.getElementById("uiEditSelectAccess").selectedIndex=0;
  switch(document.getElementById("uiPostBridgeFilterHiddenState").value) {
  		case "1":
			  	jslEnable("uiViewBridgeFilterLANPort");
				break;
		case "0":
		default:
			  	jslDisable("uiViewBridgeFilterLANPort");
				break;
  }
  jslDoShowComboBox("uiViewLanGrp","uiPostLanGrp");
}
function uiLoadEdit(index)
{
  jslSetValue("uiEditSelectSrcPort","uiPostSrcPort"+index);
  jslSetValue("uiEditSelectDstPort","uiPostDstPort"+index);
  jslSetValue("uiEditSrcMac","uiPostSrcMac"+index);
  jslSetValue("uiEditDstMac","uiPostDstMac"+index);
  jslSetValue("uiEditSelectAccess","uiPostAccess"+index);
  jslSetValue("uiEditSelectProtocol","uiPostProtocol"+index);
}
function uiBridgeFilterStatus()
{
  	jslViewToPostCheckBox("uiPostBridgeFilterState","uiViewBridgeFilterStatus");
	Modified=1;
}

function uiBridgeFilterHiddenStatus(value)
{
  	jslViewToPostCheckBox("uiPostBridgeFilterHiddenState","uiViewBridgeFilterHiddenStatus");
  	switch(value) {
  		case "on":
			  	jslEnable("uiViewBridgeFilterLANPort");
				break;
		case "off":
		default:
			  	jslDisable("uiViewBridgeFilterLANPort");
				break;
       }
	Modified=1;
}

function uiBridgeFilterLANPort()
{
	Modified=1;
}

function uiBridgeLAN(value)
{
	document.getElementById("uiPostLanGrp").value=value;
	document.getElementById("uiPostForm").method="GET";
       document.getElementById("uiPostPageName").value="bfilter";
       document.getElementById("uiPostForm").submit();
}

function uiMacFilterStatus(id)
{
  var context;

  context = document.getElementById(id);

  jslDoToggleRadio("uiViewMacFilterStatus",context.name,NoofEntries);

  document.getElementById("uiViewMacFilterDelete"+context.name).value="off";
  document.getElementById("uiViewMacFilterDelete"+context.name).src="<? echo $var:btnCheckBoxOff ?>";
  uiFilterDelete(document.getElementById("uiViewMacFilterDelete"+context.name));
  
  jslEnable("uiEditSrcMac","uiEditDstMac","uiEditSelectProtocol","uiEditSelectAccess");

  uiLoadEdit(context.name);
  if(context.value=="on") LastSelected=context.name;
	else LastSelected=-1;
}
function uiFilterDelete(context)
{
	if(context.value=="on") jslEnable("uiPostDelete"+context.name); 
	 else jslDisable("uiPostDelete"+context.name); 
	Modified=1;
}
function uiViewDeleteAll(context)
{
var i;
var Element;

      if(context.value=="on" ) {
	      for(i=0;i < NoofEntries; i++)		{
      			Element = document.getElementById("uiViewMacFilterDelete"+i);
      			Element.value="on";
      			Element.src="<? echo $var:btnCheckBoxOn ?>";
      			Element.disabled=true;
      			jslDisable("uiViewMacFilterStatus"+i);
		}
		jslDisable("uiEditSrcMac","uiEditDstMac","uiEditSelectProtocol","uiEditSelectAccess","uiViewAdd");
      		jslEnable("uiPostFlush");
      }
      else {
	      for(i=0;i < NoofEntries; i++)
		{
      			Element = document.getElementById("uiViewMacFilterDelete"+i);
      			Element.value="off";
      			Element.src="<? echo $var:btnCheckBoxOff ?>";
      			Element.disabled=false;
      			jslEnable("uiViewMacFilterStatus"+i);
		}
		jslEnable("uiEditSrcMac","uiEditDstMac","uiEditSelectProtocol","uiEditSelectAccess","uiViewAdd");
      		jslDisable("uiPostFlush");
      }      
	Modified=1;
}
function uiAdd()
{
var index;
var i;

	if(NoofEntries==0) index="0";
	else index=NoofEntries;

	if(document.getElementById("uiEditSrcMac").value.match("[^0-9a-fA-F_-]") != null) 	{ 
		alert("A mac address can only have characters 0 to 9 and A to F octet separated by -"); 
		return false;
	}
	if(document.getElementById("uiEditDstMac").value.match("[^0-9a-fA-F_-]") != null) 	{ 
		alert("A mac address can only have characters 0 to 9 and A to F octet separated by -"); 
		return false;
	}
	document.getElementById("uiPostAdd").value="entry"+index+'|'+document.getElementById("uiEditSelectAccess").value+'|'+document.getElementById("uiEditSelectDstPort").value+'|'+document.getElementById("uiEditSelectSrcPort").value+'|'+document.getElementById("uiEditDstMac").value+'|'+document.getElementById("uiEditSrcMac").value+'|'+document.getElementById("uiEditSelectProtocol").value;
	jslEnable("uiPostAdd");
	jslDisable("uiPostEdit");
       jslSetValue("uiPostBridgeFilterLANPort","uiViewBridgeFilterLANPort");
	document.getElementById("uiPostForm").submit();
}
function uiDoSave()
{
var index;
var PostElement;
var i;
	index = LastSelected;
	PostElement=document.getElementById("uiPostEdit");
	if(document.getElementById("uiEditSrcMac").value.match("[^0-9a-fA-F_-]") != null) 	{ 
		alert("A mac address can only have characters 0 to 9 and A to F octet separated by -"); 
		return false;
	}
	if(document.getElementById("uiEditDstMac").value.match("[^0-9a-fA-F_-]") != null) { 
		alert("A mac address can only have characters 0 to 9 and A to F octet separated by -"); 
		return false;
	}
	if(index!=-1) { // Edit case
			PostElement.value="entry"+index+'|'+document.getElementById("uiEditSelectAccess").value+'|'+document.getElementById("uiEditSelectDstPort").value+'|'+document.getElementById("uiEditSelectSrcPort").value+'|'+document.getElementById("uiEditDstMac").value+'|'+document.getElementById("uiEditSrcMac").value+'|'+document.getElementById("uiEditSelectProtocol").value;
			PostElement.name="<? query manager:status/$var:langrp/below ?>:settings/entry" + index;
	} else {
			for(i=0;i < NoofEntries;i++) if(document.getElementById("uiPostDelete"+i).disabled==false) break;
			if(i==NoofEntries) {
				if(NoofEntries!=0) {
					if(Modified==0) {
						alert("Select an entry to Edit or Add using Add button");
						return;
					}
				} else {
					if(Modified==0) {
						alert("Add a filter using Add button");
						return;
					}
				}					
			}
	}
	jslDisable("uiPostAdd");
       if(document.getElementById("uiPostFlush").disabled==true && index!=-1) jslEnable("uiPostEdit"); else jslDisable("uiPostEdit");

       jslSetValue("uiPostBridgeFilterLANPort","uiViewBridgeFilterLANPort");

	document.getElementById("uiPostForm").submit();
}
function uiDoCancel()
{
	  jslGoTo(null,"home");
}
</script>
