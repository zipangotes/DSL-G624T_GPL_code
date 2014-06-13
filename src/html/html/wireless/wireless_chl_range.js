
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/val.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/val.js"></script>' ?>

<? setvariable var:bandmode `<? query ap:settings/band_mode ?>` ?>

function uiDoOnLoad()
{
}

function uiDoAdd()
{	
   if(document.getElementById("uiViewStart").value=="") { alert("Start is blank");return; }
   if(document.getElementById("uiViewStop").value=="") { alert("Stop is blank");return; }
   if(document.getElementById("uiViewStep").value=="") { alert("Step is blank");return; }
   if(document.getElementById("uiViewPower").value=="") { alert("Power is blank");return; }
   
   uiChangeBand(document.getElementById("uiViewBand").value );
   document.getElementById("uiPostRangeAdd").value =  	document.getElementById("uiViewBand").value + "|" +
   													document.getElementById("uiViewStart").value + "|" +
   													document.getElementById("uiViewStop").value + "|" +
   													document.getElementById("uiViewStep").value + "|" +
   													document.getElementById("uiViewPower").value;
   
   jslFormSubmit("uiPostForm");
   return;
}

function uiChangeBand(value)
{
	switch(value) {
		case "a":
				document.getElementById("uiPostRangeAdd").name="ap:command/edit_ch_range_a";
 				break;
 		case "g":
				document.getElementById("uiPostRangeAdd").name="ap:command/edit_ch_range_g";
 				break;
	}	
}

function uiDoSave()
{	
   jslDisable("uiPostRangeAdd");
   jslFormSubmit("uiPostForm");
   return;
}

function uiDeleteA(context)
{
	if(context.value=="on") jslEnable("uiPostDeleteA"+context.name); 
	 else jslDisable("uiPostDeleteA"+context.name); 
}

function uiDoDeleteAll_A(context)
{
	switch(context.value) {
		case "on":
			<? multiquery ap:status/ch_range_a
      			` document.getElementById("uiViewDeleteA$00").src="<? echo $var:btnCheckBoxOn ?>";
      			   document.getElementById("uiViewDeleteA$00").value="on"; 			
      			` ?>
			jslEnable("uiPostRangeFlushA"); 
			break;
		case "off":
		default:
			<? multiquery ap:status/ch_range_a
			` document.getElementById("uiViewDeleteA$00").src="<? echo $var:btnCheckBoxOff ?>";
      			   document.getElementById("uiViewDeleteA$00").value="off"; 
			` ?>
		 	jslDisable("uiPostRangeFlushA"); 
			break;
	 }
}

function uiDeleteBG(context)
{
	if(context.value=="on") jslEnable("uiPostDeleteBG"+context.name); 
	 else jslDisable("uiPostDeleteBG"+context.name); 
}

function uiDoDeleteAll_BG(context)
{
	switch(context.value) {
		case "on":
			<? multiquery ap:status/ch_range_g
      			` document.getElementById("uiViewDeleteBG$00").src="<? echo $var:btnCheckBoxOn ?>";
      			   document.getElementById("uiViewDeleteBG$00").value="on"; 
      			` ?>
			jslEnable("uiPostRangeFlushBG"); 
			break;
		case "off":
		default:
			<? multiquery ap:status/ch_range_g
			` document.getElementById("uiViewDeleteBG$00").src="<? echo $var:btnCheckBoxOff ?>";
      			   document.getElementById("uiViewDeleteBG$00").value="off"; 
			` ?>
		 	jslDisable("uiPostRangeFlushBG"); 
			break;
	 }
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}

</script>
