
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

function uiDoOnLoad()
{
  var uiViewScreenResolution = document.getElementById("uiPostScreenResolution").value;
  var uiViewStyle = document.getElementById("uiPostUIStyle").value;
  var mainmenu = "menu1";

  switch(uiViewScreenResolution)
  {
    case "uiView640x480":
     top.resizeTo(640,480);
	break;
    case "uiView800x600":
	 top.resizeTo(800,600);
	break;
    case "uiView1024x780":
	default:
	 top.resizeTo(1024,780);
	 uiViewScreenResolution="uiView1024x780";
	break;
  }

  document.getElementById(uiViewScreenResolution).value="off";
  jslDoToggleCheckBox(uiViewScreenResolution);

  jslPostToViewCheckBox("uiViewDebugForms","uiPostDebugForms");
  jslPostToViewCheckBox("uiViewDebugTables","uiPostDebugTables");

  switch(uiViewStyle)
  {
    case "uiViewTIAlt1":
     document.getElementById("uiViewTIAlt1").value="off";
     jslDoToggleCheckBox("uiViewTIAlt1");
	break;

    case "uiViewTIBasic":
     document.getElementById("uiViewTIBasic").value="off";
     jslDoToggleCheckBox("uiViewTIBasic");
	break;uiViewTIBasic

    case "uiViewTIMain":
	default:
     document.getElementById("uiViewTIMain").value="off";
     jslDoToggleCheckBox("uiViewTIMain");
	break;
  }

}
function uiDoCancel()
{
  jslGoTo(null,"home");
}
function uiDoSave()
{
  var uiViewScreenResolution = "uiView1024x780";

  if(document.getElementById("uiView640x480").value=="on") { uiViewScreenResolution="uiView640x480";  }
  if(document.getElementById("uiView800x600").value=="on") { uiViewScreenResolution="uiView800x600";  }
  if(document.getElementById("uiView1024x780").value=="on")	{ uiViewScreenResolution="uiView1024x780"; }
  document.getElementById("uiPostScreenResolution").value=uiViewScreenResolution;

  jslViewToPostCheckBox("uiPostDebugForms","uiViewDebugForms");
  jslViewToPostCheckBox("uiPostDebugTables","uiViewDebugTables");

  if(document.getElementById("uiViewTIMain").value=="on")
  {
    document.getElementById("uiPostUIStyle").value="uiViewTIMain";
	document.getElementById("uiPostMenuMain").value="menu1";
	document.getElementById("uiPostMenuStyle").value="style1";
    document.getElementById("uiPostGetPage").value="/usr/www_safe/html/defs/"+document.getElementById("uiPostMenuStyle").value+"/menus/"+document.getElementById("uiPostMenuMain").value+".html";
  }
  else
  if(document.getElementById("uiViewTIAlt1").value=="on")
  {
    document.getElementById("uiPostUIStyle").value="uiViewTIAlt1";
	document.getElementById("uiPostMenuMain").value="menu2";
	document.getElementById("uiPostMenuStyle").value="style2";
    document.getElementById("uiPostGetPage").value="/usr/www_safe/html/defs/"+document.getElementById("uiPostMenuStyle").value+"/menus/"+document.getElementById("uiPostMenuMain").value+".html";
  }
  else
  if(document.getElementById("uiViewTIBasic").value=="on")
  {
    document.getElementById("uiPostUIStyle").value="uiViewTIBasic";
	document.getElementById("uiPostMenuMain").value="menu";
	document.getElementById("uiPostMenuStyle").value="style5";
    document.getElementById("uiPostGetPage").value="/usr/www_safe/html/defs/"+document.getElementById("uiPostMenuStyle").value+"/menus/"+document.getElementById("uiPostMenuMain").value+".html";
  }

  jslFormSubmit("uiPostForm");
}

</script>
