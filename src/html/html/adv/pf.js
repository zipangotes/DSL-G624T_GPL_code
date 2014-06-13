
<script language="JavaScript">

<? include /usr/www_safe/html/js/ip.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

var LastSelected=-1;

function uiDoOnLoad()
{  
 uiDoShowConnection(document.getElementById("uiPostConId").value);
  jslPostToViewCheckBox("uiViewState","uiPostState");
  return true;
}

function uiDoSave()
{
  var PostElement;

       if(LastSelected!=-1) document.getElementById("uiPostAdd").name="<? echo $var:conid ?>:fwan:command/edit"; 
	if(document.getElementById("uiViewDstIPStart").value!="") {
		if(doValidateIP(document.getElementById("uiViewDstIPStart").value)==false) { return; }
	       if ((parseInt(document.getElementById("uiViewPortStart").value) < 1) && (parseInt(document.getElementById("uiViewPortStart").value) > 65535))
 		{
			alert("Port must to be an integer between 1 and 65535"); 
	    		return false; 
		}
	       if ((parseInt(document.getElementById("uiViewPortEnd").value) < 1) && (parseInt(document.getElementById("uiViewPortEnd").value) > 65535))
 		{
                         alert("Port must to be an integer between 1 and 65535");
                        return false;
                }
	       if ((parseInt(document.getElementById("uiViewPortMap").value) < 1) && (parseInt(document.getElementById("uiViewPortMap").value) > 65535))
 		{
                         alert("Port must to be an integer between 1 and 65535");
                        return false;
                }
		document.getElementById("uiPostAdd").disabled=false;
	} else document.getElementById("uiPostAdd").disabled=true;
	
	PostElement=document.getElementById("uiPostAdd");
	PostElement.value="pf";
	if(LastSelected!=-1) PostElement.value=PostElement.value+LastSelected;
	PostElement.value=PostElement.value+"|";
	PostElement.value=PostElement.value+document.getElementById("uiViewName").value;
        PostElement.value=PostElement.value+"|";
	PostElement.value=PostElement.value+"ALLOW";
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
	PostElement.value=PostElement.value+document.getElementById("uiViewPortMap").value;
	PostElement.value=PostElement.value+"|";
	PostElement.value=PostElement.value+document.getElementById("uiViewProtocol").value;
	
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

function uiDoSelectConnection(connection)
{
  jslFormSetGet("uiPostForm");
  document.getElementById("uiPostConId").value = connection;
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

function uiDoEdit(index)
{
  var NoofEntries=<? multiquerycount <? echo $var:conid ?>:fwan:status/pf `` ?>;

  jslDoToggleRadio("uiViewFilterStatus",index,NoofEntries);
  LastSelected=index;
  EditToViewCheckBox("uiViewState","uiEditEnable"+index);
  jslSetValue("uiViewName","uiEditName"+index);
  jslSetValue("uiViewSrcIPStart","uiEditSrcIPStart"+index);
  jslSetValue("uiViewSrcMask","uiEditSrcMask"+index);
  jslSetValue("uiViewDstIPStart","uiEditDstIPStart"+index);
  jslSetValue("uiViewPortStart","uiEditPortStart"+index);
  jslSetValue("uiViewPortEnd","uiEditPortEnd"+index);
  jslSetValue("uiViewPortMap","uiEditPortMap"+index);
  jslSetValue("uiViewProtocol","uiEditProtocol"+index);
}
</script>
