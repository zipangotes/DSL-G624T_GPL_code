
function jslDoToggleCheckBox(id)
{
  var checkbox,i;
  var num = jslDoToggleCheckBox.arguments.length;

  if(num==0) return;
  if(num==1)
  {
    // Toggle only one checkbox
    checkbox = document.getElementById(id);
	if(checkbox == null) return;

    if(checkbox.value=="on")
    {
      checkbox.value="off";
      checkbox.src="<? echo $var:btnCheckBoxOff ?>";
    }
    else
    {
      checkbox.value="on";
      checkbox.src="<? echo $var:btnCheckBoxOn ?>";
    }
  }
  else
  {
    checkbox = document.getElementById(id);
	if(checkbox == null) return;

    // Set the first one to "checked" and the rest to "unchecked"
    checkbox.value="on";
    checkbox.src="<? echo $var:btnCheckBoxOn ?>";

    for(i=1;i < num; i++)
	{
      checkbox = document.getElementById(jslDoToggleCheckBox.arguments[i]);
      if(checkbox == null) return;

      checkbox.value="off";
      checkbox.src="<? echo $var:btnCheckBoxOff ?>";
	}
  }
  return false;
}

function jslDoToggleViews(id)
{
  var view,i;
  var num = jslDoToggleViews.arguments.length;

  if(num==0) return;
  if(num==1)
  {
    // Toggle only one view
    view = document.getElementById(id);
	if(view == null) return;

    if(view.style.display=="block")
      view.style.display="none";
	else
      view.style.display="";
  }
  else
  {
    for(i=1;i < num; i++)
	{
      view = document.getElementById(jslDoToggleViews.arguments[i]);
      if(view == null) return;

      view.style.display="none";
	}

    view = document.getElementById(id);
	if(view == null) return;

    // Set the first view to "block"
    view.style.display="";
  }
  return false;
}
function doLoadPage(id,page)
{
  var handle = document.getElementById(id);
  if(handle!=null) handle.src=page;
}
function jslDoClearHighlight(id)
{
  var element;
  element = document.getElementById(id);
  if(element != null)
  {
	element.style.background = "white";
    //element.style.border="none";
  }
  element.blur();  
  return false;
}

function jslDoSetHighlight(id,color)
{
  var element;
  element = document.getElementById(id);
  if(element != null)
  {
	//element.style.borderStyle="solid";
	//element.style.borderColor="black";
	//element.style.borderBottomWidth="1";
	//element.style.borderTopWidth="1";
	//element.style.borderLeftWidth="1";
	//element.style.borderRightWidth="1";
	element.style.background = color;
  }
  element.select();
  element.focus();
  return false;
}

function jslIsString(id)
{
  if(document.getElementById(id).value.match("[a-zA-Z0-9_]+")!=null) return true;
  return false;
}

function jslSetValue(variable,value)
{
  document.getElementById(variable).value=document.getElementById(value).value;
}

function jslDisable(id)
{
  var i;
  var num=jslDisable.arguments.length;

  if(num==0) return;
  for(i=0;i < num; i++)
	  document.getElementById(arguments[i]).disabled=true;
}

function jslEnable(id)
{
  var i;
  var num=jslEnable.arguments.length;

  if(num==0) return;
  for(i=0;i < num; i++)
	  document.getElementById(arguments[i]).disabled=false;
}

function jslDoToggleRadio(id,index,maxId)
{
  var radiobox,i;
  var ids;

  if(maxId==1)
  {
    // Toggle only one radio
    radiobox = document.getElementById(id+index);
	if(radiobox == null) return;

    if(radiobox.value=="on")
    {
      radiobox.value="off";
      radiobox.src="<? echo $var:btnRadioBtnOff ?>";
    }
    else
    {
      radiobox.value="on";
      radiobox.src="<? echo $var:btnRadioBtnOn ?>";
    }
  }
  else
  {
    radiobox = document.getElementById(id+index);
	if(radiobox == null) return;

    // Set the current one to "checked" and the rest to "unchecked"
    radiobox.value="on";
    radiobox.src="<? echo $var:btnRadioBtnOn ?>";
      for(i=0;i<maxId; i++)
	{
		if(i==index) continue;
      		radiobox = document.getElementById(id+i);
      		if(radiobox == null) return;

      		radiobox.value="off";
      		radiobox.src="<? echo $var:btnRadioBtnOff ?>";
	}
  }
  return false;
}

function jslDoToggleDisabled(preText,currentId,maxId)
{
  var Element,i;
  var ids;

  if(maxId==1)
  {
    Element = document.getElementById(preText+currentId);
	if(Element == null) return;
    if(Element.disabled==true)
     Element.disabled=false;
    else Element.disabled=true;
  }
  else
  {
    Element = document.getElementById(preText+currentId);
	if(Element == null) return;

    // Set the current one to "true" and the rest to "false"
    Element.disabled=false;
   for(i=0;i<maxId; i++)
   {
     if(i==currentId) continue;
     Element = document.getElementById(preText+i);
     if(Element == null) return;
     Element.disabled=true;
   }
  }
  return false;
}

function jslPostToViewCheckBox(ViewId,PostId)
{
  if(document.getElementById(PostId).value=="1")
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
function jslViewToPostCheckBox(PostId,ViewId)
{
  if(document.getElementById(ViewId).value=="on")
   document.getElementById(PostId).value="1";
  else document.getElementById(PostId).value="0";
}
function jslFormSetPost(id)
{
  document.getElementById(id).method="POST";
}
function jslFormSetGet(id)
{
  document.getElementById(id).method="GET";
}
function jslFormSubmit(id)
{
  document.getElementById(id).submit();
}
function jslPageErrorMsg(msg)
{
  var hint;
  if(msg!="")
  {
    hint=document.getElementById("uiViewErrorHint");
    hint.style.color = "<? echo $var:txtErrorHintColor ?>";
	alert("Check "+msg+" errors.");
	return false;
  }
  return true;
}

function jslGoTo(Menu,Page)
{
  // Menu: { null, "home", "setup", "adv", "tools", "status", "help" }
  // Page: { null, pagename (from left menu once you switch to a category) }

  if((Menu!=null) && (Menu!="")) document.getElementById("uiPostMenu").value=Menu;
  if((Page!=null) && (Page!="")) document.getElementById("uiPostPageName").value=Page;
  jslFormSetGet("uiPostForm");
  jslFormSubmit("uiPostForm");
}

function jslDebugLoadOrPost(name)
{
  var handle;
  var postform;
  var i;

  handle = window.open("",name,"height=1,width=1",true);
  handle.close();
  handle = window.open("",name,"height=400,width=600,status,scrollbars,resizable",true);
  if(handle==null) { alert("can't open debug window"); return; }
  
  postform=document.getElementById("uiPostForm");
  if(postform==null) { alert("can't populate debug window"); return; }

  handle.document.write('<html><head></head><body>');

  handle.document.write('<table border=1 align="center" bgcolor="#D3D9DF">');
  handle.document.write('<tr><td colspan=20 align="center"><b>Page Variables</b></td></tr>');
  handle.document.write('<tr><td colspan=20 align="center"><b>Page Title: <? echo $var:pagetitle ?></b></td></tr>');
  handle.document.write('<tr>');
  handle.document.write('<td align="center">&nbsp;</td>');
  handle.document.write('<td align="center"><b>Name</b></td>');
  handle.document.write('<td align="center"><b>Value</b></td>');
  handle.document.write('<td align="center"><b>Post</b></td>');
  handle.document.write('</tr>');
  
  for(i=0; i < postform.length; i++ )
  {
    handle.document.write('<tr>');
    handle.document.write('<td align="center"><b>'+i+'</b></td>');
    handle.document.write('<td>'+postform.elements[i].name+'</td>');
	if(postform.elements[i].value!="")
	    handle.document.write('<td>'+postform.elements[i].value+'</td>');
	else
	    handle.document.write('<td>&nbsp;</td>');
	if(postform.elements[i].disabled==true)
	    handle.document.write('<td style="color:red;"><b>disabled</b></td>');
	else
	    handle.document.write('<td style="color:green;"><b>enabled</b></td>');
    handle.document.write('</tr>');
  }
  handle.document.write('</table>');

  handle.document.write('</body></html>');
}

function jslDoOnLoad()
{
  if("1"=="<? query webui:settings/debug/forms ?>")
   jslDebugLoadOrPost("uiOnLoadDebug");
  uiDoOnLoad();
}

function jslDoOnSubmit()
{
  if("1"=="<? query webui:settings/debug/forms ?>")
   jslDebugLoadOrPost("uiOnPostDebug");
}

function jslDoShowComboBox(viewid,postid)
{
  var i;
  var selector = document.getElementById(viewid);
  var value=document.getElementById(postid).value;
  
  if(selector==null) return;

  for(i=0; i < selector.length; i++)
  {
    if(selector.options[i].value == value)
	{
         selector.selectedIndex = i;
         break;
	}
  }
}

