
<script language="JavaScript">

<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/val.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{
  
  //doShowPageErrors();
}

function uiDoSelectProtocol(value)
{
    if(value=="icmp") jslDisable("uiViewDestStartPort","uiViewSrcStartPort","uiViewDestEndPort","uiViewSrcEndPort");
    else jslEnable("uiViewDestStartPort","uiViewSrcStartPort","uiViewDestEndPort","uiViewSrcEndPort");
}


function uiDoValidatePort(id)
{
var message=null;

  value=document.getElementById(id).value;
  if(value != "" && value !="*") {	
      message=valDoValidateInteger(value);
      if(message==null) { 
            value=Number(value);
            if ((value < 1) || (value > 65535)) {
 	        message="Port must to be an integer between 1 and 65535"; 
           }
      }
  } 
  return message;
}

function IPQoSDoValidatePage()
{
  var message;
  var value;
  var count=0;

  value = document.getElementById("uiViewRuleName").value;
  if(value == "" ) {
     message="Rule name needed";
  } else {
     message=valDoValidateName(value);
  }
  if(message!=null) { erlDoAddError("bad","uiViewRuleName","",value,message); count++; }

  value = document.getElementById("uiViewDestIPAddr").value;
  if(value != "" && value !="*") {	
      message = valDoValidateIp(value);
      if(message!=null) { erlDoAddError("bad","uiViewDestIPAddr","",value,message); count++; }
  } 
  value = document.getElementById("uiViewDestNetmask").value;
  if(value != "" && value !="*") {	
      message = valDoValidateMask(value);
      if(message!=null) { erlDoAddError("bad","uiViewDestNetmask","",value,message); count++; }
  } 
  value = document.getElementById("uiViewSrcIPAddr").value;
  if(value != "" && value !="*") {	
      message = valDoValidateIp(value);
     if(message!=null) { erlDoAddError("bad","uiViewSrcIPAddr","",value,message); count++; }
  }
  value = document.getElementById("uiViewSrcNetmask").value;
  if(value != "" && value !="*") {	
      message = valDoValidateMask(value);
      if(message!=null) { erlDoAddError("bad","uiViewSrcNetmask","",value,message); count++; }
  } 

  value = document.getElementById("uiViewDestStartPort").value;
  message=uiDoValidatePort("uiViewDestStartPort");
  if(message!=null) { erlDoAddError("bad","uiViewDestStartPort","",value,message); count++; }

  value = document.getElementById("uiViewDestEndPort").value;
  message=uiDoValidatePort("uiViewDestEndPort");
  if(message!=null) { erlDoAddError("bad","uiViewDestEndPort","",value,message); count++; }
  
  value = document.getElementById("uiViewSrcStartPort").value;
  message=uiDoValidatePort("uiViewSrcStartPort");
  if(message!=null) { erlDoAddError("bad","uiViewSrcStartPort","",value,message); count++; }

  value = document.getElementById("uiViewSrcEndPort").value;
  message=uiDoValidatePort("uiViewSrcEndPort");
  if(message!=null) { erlDoAddError("bad","uiViewSrcEndPort","",value,message); count++; }
  
  if(document.getElementById("uiViewSrcNetmask").value != "") {
  	if(document.getElementById("uiViewSrcIPAddr").value == "") {
          message="Need IP in presence of Netmask";
          erlDoAddError("bad","uiViewSrcIPAddr","","",message); count++;
  	}
  }
  
  if(document.getElementById("uiViewDestNetmask").value != "") {
  	if(document.getElementById("uiViewDestIPAddr").value == "") {
          message="Need IP in presence of Netmask";
          erlDoAddError("bad","uiViewDestIPAddr","","",message); count++;
  	}
  }
  
  if(document.getElementById("uiViewDestEndPort").value != "") {
  	if(document.getElementById("uiViewDestStartPort").value == "") {
          message="Need start port in presence of end port";
          erlDoAddError("bad","uiViewDestStartPort","","",message); count++;
  	} else {
           if(Number(document.getElementById("uiViewDestEndPort").value ) < Number(document.getElementById("uiViewDestStartPort").value )) {
              message="Start port can not be greater than end port";
              erlDoAddError("bad","uiViewDestEndPort","",document.getElementById("uiViewDestEndPort").value,message); count++;
           }
  	}
  }
  if(document.getElementById("uiViewSrcEndPort").value != "") {
  	if(document.getElementById("uiViewSrcStartPort").value == "") {
          message="Need start port in presence of end port";
          erlDoAddError("bad","uiViewSrcStartPort","","",message); count++;
  	} else {
           if(Number(document.getElementById("uiViewSrcEndPort").value ) < Number(document.getElementById("uiViewSrcStartPort").value )) {
              message="Start port can not be greater than end port";
              erlDoAddError("bad","uiViewSrcEndPort","",document.getElementById("uiViewSrcEndPort").value,message); count++;
           }
  	}
  }
  	
  if(count!=0) message="IPQoS";
   else message="";
  return message;
}


function uiDoValidate()
{
  var msg;
  
  msg=IPQoSDoValidatePage();
  if(msg!="") {
  	erlDoShowPageErrors();
  	return jslPageErrorMsg(msg);
  }
  return true;
}

function uiNormalService(value)
{
  var i;
  var selector = document.getElementById("uiViewTOS");

 	switch (value) {
	case "on":
                     for(i=0; i < selector.length; i++)
                     {
                          if(selector.options[i].selected == true)
	                   {
                               selector.options[i].selected=false;
	                   }
                     }
			jslDisable("uiViewTOS");
		break;
	case "off":
	default:
			jslEnable("uiViewTOS");
		break;

 	}
}

function uiDoTOS()
{
  var i;
  var selector = document.getElementById("uiViewTOS");
  var value=0; // normal service
  var selected=0;
  
  if(document.getElementById("uiViewNormalService").value=="on") return value; //normal service
  
  if(selector==null) return -1; // No change

  for(i=0; i < selector.length; i++)
  {
    if(selector.options[i].selected == true)
	{
         value = value | selector.options[i].value;
         selected=1;
	}
  }
  if (selected==0) return (-1);
  else return value;
}


function uiDoSave()
{

  document.getElementById("uiPostAdd").value= document.getElementById("uiPostConType").value + "|" +
  	                                                                document.getElementById("uiViewRuleName").value + "|" +
  	                                                                document.getElementById("uiViewSrcIPAddr").value + "|" +
  										 document.getElementById("uiViewSrcNetmask").value + "|" +
  										 document.getElementById("uiViewSrcStartPort").value + "|" +
  										 document.getElementById("uiViewSrcEndPort").value + "|" +
                                                                  	 document.getElementById("uiViewDestIPAddr").value + "|" +
  										 document.getElementById("uiViewDestNetmask").value + "|" +
  										 document.getElementById("uiViewDestStartPort").value + "|" +
  										 document.getElementById("uiViewDestEndPort").value + "|" +
  										 document.getElementById("uiViewProtocol").value + "|" +
  										 document.getElementById("uiViewPriority").value + "|" +
  										 document.getElementById("uiViewPHYPort").value + "|" +
  										 uiDoTOS();

  if(uiDoValidate()==true) {
  	document.getElementById("uiPostErrorPageName").value="ipqos_rule";
  	document.getElementById("uiPostPageName").value="ipqos";
       jslFormSubmit("uiPostForm");
  }
  
}

function uiDoCancel()
{
  jslGoTo("","ipqos");
}
</script>
