
<script language="JavaScript">

<? include /usr/www_safe/html/js/ip.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{
  var i, clientList;
  document.getElementById("uiPostConId").value = uiDoShowConnection(document.getElementById("uiPostConId").value);  
  jslDoShowComboBox("uiViewLanGrp","uiPostLanGrp");
  document.getElementById("uiPostFwanDmzInternalClient").value = uiDoShowLanIP(document.getElementById("uiPostFwanDmzInternalClient").value);
  jslPostToViewCheckBox("uiViewFwanDmzState","uiPostFwanDmzState");
  clientList = document.getElementById("uiViewDmzInternalClient");
   for(i=0; i < clientList.length; i++)
   if(clientList.options[i].value==document.getElementById("uiPostFwanDmzState").value) clientList.selectedIndex=i;
}

function uiDoSelectLANGrp(value)
{
  document.getElementById("uiPostLanGrp").value=value;
  jslFormSetGet("uiPostForm");
  document.getElementById("uiPostPageName").value="<? echo $var:pagename ?>";
  jslFormSubmit("uiPostForm");
}

function uiDoSave()
{
  jslViewToPostCheckBox("uiPostFwanDmzState","uiViewFwanDmzState");
  jslSetValue("uiPostFwanDmzInternalClient","uiViewDmzInternalClient");
  document.getElementById("uiPostForm").submit();
}
function uiDoCancel()
{
  jslGoTo(null,"home");
}
function uiDoSelectConnection(connection)
{
  document.getElementById("uiPostForm").method="GET";
  document.getElementById("uiPostConId").value = connection;
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
function uiDoShowLanIP (IPaddress)
{
  var i;
  var selector = document.getElementById("uiViewDmzInternalClient");
  if(selector==null) return;

  for(i=0; i < selector.length; i++)
  {
    if(selector.options[i].value == IPaddress)
	{
      selector.selectedIndex = i;
	  return(IPaddress);
	}
  }
  return(selector.options[0].value);
}
</script>
