
<script language="JavaScript">

<? include /usr/www_safe/html/setup/wst.js ?>
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

var ConnectionDatabase = new Array();
var MaxConnections;
var CurrentConnectionSelection = -1;
function uiDoOnLoad()
{ 
  var i = 0;
  var currConn;
  <? multiquery manager:command/connection/list "ConnectionDatabase[i++]='$01';" ?>
  MaxConnections = i;
    
  uiSetVoiceStatus (document.getElementById("uiPostVoiceState").value);
  
  currConn = document.getElementById("uiPostVoiceConnection").value;
  for(i=0;i< MaxConnections;i++)
  {
    if(ConnectionDatabase[i]==currConn)
    {
      uiChangeConnection(i);
	  break;
	}
  }
}
function uiDoSave()
{
   if (document.getElementById("uiPostVoiceState").value == "1")
   {
     if (CurrentConnectionSelection == -1)
     {
       alert ("You must select a connection");
	   return;
     }
  }
  document.getElementById("uiPostVoiceConnection").value = ConnectionDatabase[CurrentConnectionSelection];
  document.getElementById("uiPostForm").submit();
  return;
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}

function uiSetVoiceStatus(arg)
{
  if(arg=="1") 
    uiInitializeVoiceStatus("on");
  else
    uiInitializeVoiceStatus("off");
}

function uiChangeVoiceStatus(arg)
{ 
  if (arg == "on") {
    document.getElementById("uiPostVoiceState").value = "1";
  <? multiquery manager:command/connection/list `jslEnable("uiViewConnection$00");`
  ?>	
  }
  else {
  <? multiquery manager:command/connection/list `jslDisable("uiViewConnection$00");`
  ?>	
    document.getElementById("uiPostVoiceState").value = "0";
}
}

function uiInitializeVoiceStatus(arg)
{ 
  var checkbox = document.getElementById("uiViewVoice");
  if (checkbox.value != arg)
    jslDoToggleCheckBox ("uiViewVoice");

  if (arg == "on")
    document.getElementById("uiPostVoiceState").value = "1";
  else
    document.getElementById("uiPostVoiceState").value = "0";  
}

function uiChangeConnection (arg)
{
  jslDoToggleRadio("uiViewConnection",arg, MaxConnections);

  if (document.getElementById("uiViewConnection" + arg).value == "off")
	CurrentConnectionSelection = -1;
  else
    CurrentConnectionSelection = arg;
}
</script>
