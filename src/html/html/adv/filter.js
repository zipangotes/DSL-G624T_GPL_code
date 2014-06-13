
<script language="JavaScript">

<? include /usr/www_safe/html/js/ip.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

var LastSelected=-1;

function uiDoOnLoad()
{
  jslPostToViewCheckBox("uiViewState","uiPostState");
  //doShowPageErrors();
  return true;
}

function uiDoSave()
{
  var PostElement;

       if(LastSelected!=-1) document.getElementById("uiPostAdd").name="<? echo $var:langrp ?>:flan:command/edit"; 
	if(document.getElementById("uiViewSrcIPStart").value!="") {
		if(document.getElementById("uiViewSrcIPStart").value!="*") {
		if(doValidateIP(document.getElementById("uiViewSrcIPStart").value)==false) return;
		}
		else {
			document.getElementById("uiViewSrcMask").value="*";
		}
		
		if(document.getElementById("uiViewSrcMask").value!="*")  {
		if(doValidateMask(document.getElementById("uiViewSrcMask").value)==false) return;
		}

		if(document.getElementById("uiViewDstIPStart").value!="*") {
		if(doValidateIP(document.getElementById("uiViewDstIPStart").value)==false) return;
		}
		else {
			document.getElementById("uiViewDstMask").value="*";
		}
		
		if(document.getElementById("uiViewDstMask").value!="*")  {
		if(doValidateMask(document.getElementById("uiViewDstMask").value)==false) return;
		}
		
		if(document.getElementById("uiViewPortStart").value!="*")  {
 		  if ((parseInt(document.getElementById("uiViewPortStart").value) < 1) && (parseInt(document.getElementById("uiViewPortStart").value) > 65535))
 		  {
 		       alert("Port must to be an integer between 1 and 65535"); 
	    		return false; 
		  }
		}
		else {
			document.getElementById("uiViewPortEnd").value="*";
		}
		
		if(document.getElementById("uiViewPortEnd").value!="*") {
 		  if ((parseInt(document.getElementById("uiViewPortEnd").value) < 1) && (parseInt(document.getElementById("uiViewPortEnd").value) > 65535))
  		  {
                         alert("Port must to be an integer between 1 and 65535");
                        return false;
                }
		}	
		document.getElementById("uiPostAdd").disabled=false;
	} else document.getElementById("uiPostAdd").disabled=true;
	
	PostElement=document.getElementById("uiPostAdd");
	PostElement.value="filter";
	if(LastSelected!=-1) PostElement.value=PostElement.value+LastSelected;
	PostElement.value=PostElement.value+"|";
	PostElement.value=PostElement.value+document.getElementById("uiViewName").value;
        PostElement.value=PostElement.value+"|";
	PostElement.value=PostElement.value+document.getElementById("uiViewAction").value;
        PostElement.value=PostElement.value+"|"; 
        PostElement.value=PostElement.value+uiGetCheckBox("uiViewState");
        PostElement.value=PostElement.value+"|";
	PostElement.value=PostElement.value+document.getElementById("uiViewSrcIPStart").value;
	PostElement.value=PostElement.value+"|";
	PostElement.value=PostElement.value+document.getElementById("uiViewSrcMask").value;
       PostElement.value=PostElement.value+"|";
	PostElement.value=PostElement.value+document.getElementById("uiViewDstIPStart").value;
	PostElement.value=PostElement.value+"|";
	PostElement.value=PostElement.value+document.getElementById("uiViewDstMask").value;
       PostElement.value=PostElement.value+"|";
	PostElement.value=PostElement.value+document.getElementById("uiViewPortStart").value;
	PostElement.value=PostElement.value+"|";
	PostElement.value=PostElement.value+document.getElementById("uiViewPortEnd").value;
	PostElement.value=PostElement.value+"|";
	PostElement.value=PostElement.value+document.getElementById("uiViewProtocol").value;
	
	// 3. Validate visual data and trap errors
  	document.getElementById("uiPostForm").submit();
}

function uiDoCancel()
{
 jslGoTo(null,"home");
}

function uiGetCheckBox(ViewId)
{
  if(document.getElementById(ViewId).value=="on")
  {
	return "1";
  }
  else
  {
	return "0";
  }
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

function EditToViewCheckBox(ViewId,EditId)
{
  if(document.getElementById(EditId).value=="on")
  {
  	document.getElementById(ViewId).value="on";
  	document.getElementById(ViewId).src="<? echo $var:btnCheckBoxOn ?>";
  }
  else
  {
  	document.getElementById(ViewId).value="off";
  	document.getElementById(ViewId).src="<? echo $var:btnCheckBoxOff ?>";
  }
}

function uiDoEdit(index)
{
  var NoofEntries=<? multiquerycount $var:langrp:flan:status/filter ?>;

  jslDoToggleRadio("uiViewFilterStatus",index,NoofEntries);
  LastSelected=index;
  EditToViewCheckBox("uiViewState","uiEditEnable"+index);
  jslSetValue("uiViewName","uiEditName"+index);
  jslSetValue("uiViewSrcIPStart","uiEditSrcIPStart"+index);
  jslSetValue("uiViewSrcMask","uiEditSrcMask"+index);
  jslSetValue("uiViewDstIPStart","uiEditDstIPStart"+index);
  jslSetValue("uiViewDstMask","uiEditDstMask"+index);
  jslSetValue("uiViewPortStart","uiEditPortStart"+index);
  jslSetValue("uiViewPortEnd","uiEditPortEnd"+index);
  jslSetValue("uiViewProtocol","uiEditProtocol"+index);

}

</script>
