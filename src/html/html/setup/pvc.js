var pvcPageErrors = new Array();
var pvcPageErrorCount = 0;

function pvcDoOnLoad()
{
  jslPostToViewCheckBox("uiViewAutoPvcEnable", "uiPostAutoPvcEnable");
  jslSetValue("uiViewPvcVpi","uiPostPvcVpi");
  jslSetValue("uiViewPvcVci","uiPostPvcVci");
  jslDoShowComboBox("uiViewPvc","uiPostPvc")

  jslSetValue("uiViewPvcPcr","uiPostPvcPcr");
  jslSetValue("uiViewPvcCdvt","uiPostPvcCdvt");
  jslSetValue("uiViewPvcScr","uiPostPvcScr");
  jslSetValue("uiViewPvcMbs","uiPostPvcMbs");

  pvcDoSetQoS(document.getElementById("uiPostPvcQoS").value);

  // For not a new connection disable all pvc settings.
  <? if neq `new` `<? echo $var:mode ?>`
     ` jslDisable("uiViewPvc","uiViewPvcVpi","uiViewPvcVci");
        jslDisable("uiViewQoS","uiViewPvcPcr");
        jslDisable("uiViewPvcScr","uiViewPvcMbs","uiViewPvcCdvt");
        jslDisable("uiPostPvc","uiPostPvcVpi","uiPostPvcVci");
        jslDisable("uiPostPvcQoS","uiPostPvcPcr");
        jslDisable("uiPostPvcScr","uiPostPvcMbs","uiPostPvcCdvt");
        jslDisable("uiPostPvcDes");
     `  
     ` ` ?>

  // For not a new encapsid disable all vpi vci etc
  <? if neq `<? query manager:command/encaps/new ?>` `<? echo $var:encapsid ?>`
     ` jslDisable("uiViewPvcVpi","uiViewPvcVci","uiViewQoS","uiViewPvcPcr");
        jslDisable("uiViewPvcScr","uiViewPvcMbs","uiViewPvcCdvt");
        jslDisable("uiPostPvcVpi","uiPostPvcVci","uiPostPvcQoS","uiPostPvcPcr");
        jslDisable("uiPostPvcScr","uiPostPvcMbs","uiPostPvcCdvt");
        jslDisable("uiPostPvcDes","uiPostEncapsType");
        // document.getElementById("uiPostAutoPvcEnable").value="0";
        // jslPostToViewCheckBox("uiViewAutoPvcEnable", "uiPostAutoPvcEnable");
        jslDisable("uiViewAutoPvcEnable","uiPostAutoPvcEnable");
        <? if eq `pppoa` `$var:contype`  `jslEnable("uiPostPvcVpi","uiPostPvcVci")` `jslDisable("uiPostPvcVpi","uiPostPvcVci")` ?>
     `  
     ` ` ?>

  // Disable VPI, VCI if auto PVC is enabled
  if (document.getElementById("uiPostAutoPvcEnable").value == 1)
  {
     	jslDisable("uiViewPvc","uiViewPvcVpi","uiViewPvcVci");
  }
}


function pvcDoSave()
{
  var message;

  jslViewToPostCheckBox("uiPostAutoPvcEnable", "uiViewAutoPvcEnable");
  jslSetValue("uiPostPvcVpi","uiViewPvcVpi");
  jslSetValue("uiPostPvcVci","uiViewPvcVci");
  jslSetValue("uiPostPvcQoS","uiViewQoS");
  jslSetValue("uiPostPvc","uiViewPvc");
  document.getElementById("uiPostPvcDes").value = document.getElementById("uiPostPvcVpi").value + ":" + document.getElementById("uiPostPvcVci").value;

  switch(document.getElementById("uiPostPvcQoS").value)
  {
    case "CBR":
      jslSetValue("uiPostPvcPcr","uiViewPvcPcr");
      jslSetValue("uiPostPvcCdvt","uiViewPvcCdvt");
    break;
    case "VBR":
      jslSetValue("uiPostPvcPcr","uiViewPvcPcr");
      jslSetValue("uiPostPvcScr","uiViewPvcScr");
      jslSetValue("uiPostPvcMbs","uiViewPvcMbs");
      jslSetValue("uiPostPvcCdvt","uiViewPvcCdvt");
    break;
    case "UBR":
    default:
	break;
  }
}
function pvcDoSetQoS(arg)
{
  var index = 0;
  switch(arg)
  {
    case "CBR":
      index = 1;
	  jslEnable("uiViewPvcPcr");jslEnable("uiPostPvcPcr");
	  jslDisable("uiViewPvcScr");jslDisable("uiPostPvcScr");
	  jslDisable("uiViewPvcMbs");jslDisable("uiPostPvcMbs");
	  jslEnable("uiViewPvcCdvt");jslEnable("uiPostPvcCdvt");
    break;
    case "VBR":
      index = 2;
	  jslEnable("uiViewPvcPcr");jslEnable("uiPostPvcPcr");
	  jslEnable("uiViewPvcScr");jslEnable("uiPostPvcScr")
	  jslEnable("uiViewPvcMbs");jslEnable("uiPostPvcMbs")
	  jslEnable("uiViewPvcCdvt");jslEnable("uiPostPvcCdvt")
    break;
    case "UBR":
    default:
      index = 0;
	  jslDisable("uiViewPvcPcr");jslDisable("uiPostPvcPcr");
	  jslDisable("uiViewPvcScr");jslDisable("uiPostPvcScr");
	  jslDisable("uiViewPvcMbs");jslDisable("uiPostPvcMbs");
	  jslDisable("uiViewPvcCdvt");jslDisable("uiPostPvcCdvt");
	break;
  }

  jslSetValue("uiViewPvcPcr","uiPostPvcPcr");
  jslSetValue("uiViewPvcCdvt","uiPostPvcCdvt");
  jslSetValue("uiViewPvcScr","uiPostPvcScr");
  jslSetValue("uiViewPvcMbs","uiPostPvcMbs");

  document.getElementById("uiViewQoS").selectedIndex=index;
}

function pvcDoSetPVC(shared_encapsid)
{
  var page;
  page="../cgi-bin/webcm?getpage=/usr/www_safe/html/defs/<? echo $var:style ?>/menus/<? echo $var:main ?>.html&var:style=<? echo $var:style ?>&var:main=<? echo $var:main ?>&var:menu=setup&var:menutitle=Setup&var:pagename=<? echo $var:pagename ?>&var:contype=<? echo $var:contype ?>";
  page=page+"&var:pagetitle=New%20Connection&var:conid=<? echo $var:conid ?>&var:encapsid="+ shared_encapsid + "&var:muxid=<? echo $var:muxid ?>&var:mode=<? echo $var:mode ?>";
  <? if eq $var:mode new `page=page+"&var:conname="+document.getElementById("uiViewConDesc").value;` `` ?>
  page=page+"&var:sharing="+document.getElementById("uiViewVlanEnable").value;
  document.location.href=page;

}

function uiDoAutoPvc(value)
{
	switch(value) {
	case "off":
			jslEnable("uiViewPvcVpi","uiPostPvcVpi");
			jslEnable("uiViewPvcVci","uiPostPvcVci");
			break;
	case "on":
			jslDisable("uiViewPvcVpi","uiPostPvcVpi");
			jslDisable("uiViewPvcVci","uiPostPvcVci");
			break;
	}
}

function pvcDoValidatePage()
{
  var message;
  var value;
  var autopvc;
  var pvcPageErrorCount=0;

  autopvc = document.getElementById("uiPostAutoPvcEnable").value;

  if (autopvc != 1)
  {
  value = document.getElementById("uiPostPvcVpi").value;
  message = valDoValidateInteger(value);
  if(message!=null) { erlDoAddError("bad","uiViewPvcVpi","",value,message); pvcPageErrorCount++; }
  else if(Number(value) > 255) { erlDoAddError("bad","uiViewPvcVpi","",value,"VPI must be less than or equal to 255"); pvcPageErrorCount++; }

  value = document.getElementById("uiPostPvcVci").value;
  message = valDoValidateInteger(value);
  if(message!=null) { erlDoAddError("bad","uiViewPvcVci","",value,message); pvcPageErrorCount++; }
  else if(Number(value) > 65535) { erlDoAddError("bad","uiViewPvcVci","",value,"VCI must be less than or equal to 65535"); pvcPageErrorCount++; }
  }

  value = document.getElementById("uiPostPvcQoS").value;
  switch(value)
  {
    case "UBR":
	 break;
	case "CBR":
     value = document.getElementById("uiPostPvcPcr").value;
     message = valDoValidateInteger(value);
     if(message!=null) { erlDoAddError("bad","uiViewPvcPcr","",value,message); pvcPageErrorCount++; }
     else if(Number(value) > 65534) { erlDoAddError("bad","uiViewPvcPcr","",value,"PCR must be less than 65534"); pvcPageErrorCount++; }
	 break;
	case "VBR":
     value = document.getElementById("uiPostPvcPcr").value;
     message = valDoValidateInteger(value);
     if(message!=null) { erlDoAddError("bad","uiViewPvcPcr","",value,message); pvcPageErrorCount++; }
     else if(Number(value) > 65534) { erlDoAddError("bad","uiViewPvcPcr","",value,"PCR must be less than 65534"); pvcPageErrorCount++; }

     value = document.getElementById("uiPostPvcScr").value;
     message = valDoValidateInteger(value);
     if(message!=null) { erlDoAddError("bad","uiViewPvcScr","",value,message); pvcPageErrorCount++; }
     else if(Number(value) > 65534) { erlDoAddError("bad","uiViewPvcScr","",value,"PCR must be less than 65534"); pvcPageErrorCount++; }
	 break;
	default:
	 break;
  }
  if(pvcPageErrorCount!=0) message="PVC";
  else message="";
  return message;
}

