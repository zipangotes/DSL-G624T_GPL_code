
<script language="JavaScript">

<? include /usr/www_safe/html/js/ip.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{

  jslDoShowComboBox("uiViewConnectionSelector","uiPostConId");
  //doShowPageErrors();
  return true;
}


function uiDoSave()
{
  var PostElement;
  var i;
  var TableSize;
	if(document.getElementById("uiViewDestIP").value!="") {
		if(doValidateIP(document.getElementById("uiViewDestIP").value)==false) return;
		if(doValidateMask(document.getElementById("uiViewMask").value)==false) return;
		if(document.getElementById("uiViewGateway").value!="") { // gateway can be empty string
	   		if(doValidateGateway(document.getElementById("uiViewGateway").value)==false) return;
		}
		if(document.getElementById("uiViewMetric").value.match("[^0-9]") != null) { 
	 		alert("Metric needs to be an positive integer"); 
	      		return false; 
		}
		document.getElementById("uiPostAdd").disabled=false;
	} else document.getElementById("uiPostAdd").disabled=true;

	PostElement=document.getElementById("uiPostAdd");
	PostElement.value=document.getElementById("uiViewConnectionSelector").value;
	PostElement.value=PostElement.value+"|";	
	PostElement.value=PostElement.value+document.getElementById("uiViewDestIP").value;
	PostElement.value=PostElement.value+"|";	
	PostElement.value=PostElement.value+document.getElementById("uiViewMask").value;
	PostElement.value=PostElement.value+"|";
	PostElement.value=PostElement.value+document.getElementById("uiViewGateway").value;
	PostElement.value=PostElement.value+"|";
	PostElement.value=PostElement.value+document.getElementById("uiViewMetric").value;
  
  // 3. Validate visual data and trap errors

  	document.getElementById("uiPostForm").submit();
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}
function uiDoToggleDelete(id,control)
{
  
  jslDoToggleCheckBox(id);
  if(document.getElementById(control).disabled==true)
  {
    document.getElementById(control).disabled = false;
  }
  else
  {
    document.getElementById(control).disabled = true;
  }
}

function uiDoSelectConnection(connection)
{
    document.getElementById("uiPostConId").value=connection;
    document.getElementById("uiPostForm").method="GET";
    document.getElementById("uiPostPageName").value="routing";
    document.getElementById("uiPostForm").submit();       

}
</script>
