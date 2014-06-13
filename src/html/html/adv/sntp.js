
<script language="JavaScript">

<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/val.js ?>
<? include /usr/www_safe/html/js/error.js ?>

function uiDoOnLoad()
{ 
  jslPostToViewCheckBox("uiViewSNTPState","uiPostSNTPState");
  uiDoChangeSNTPState(document.getElementById("uiViewSNTPState").value);
  jslSetValue("uiViewPrimary","uiPostPrimary");
  jslSetValue("uiViewSecondary","uiPostSecondary");
  jslSetValue("uiViewTertiary","uiPostTertiary");
  jslSetValue("uiViewTimeout","uiPostTimeout");
  jslSetValue("uiViewPollInterval","uiPostPollInterval");
  jslSetValue("uiViewRetryCount","uiPostRetryCount");
  jslDoShowComboBox("uiViewTimezone","uiPostTimezone");
  jslPostToViewCheckBox("uiViewDayLight","uiPostDayLight");
}

function SntpDoValidatePage()
{
  var message=null;
  var value;
  var count=0;
  
  //value = document.getElementById("uiViewPrimary").value;
  //if(value != "") {	
  //    message = valDoValidateHostName(value);
  //    if(message!=null) { erlDoAddError("bad","uiViewPrimary","",value,message); count++; }
  //}
  
  //value = document.getElementById("uiViewSecondary").value;
  //if(value != "") {	
  //   message = valDoValidateHostName(value);
  //   if(message!=null) { erlDoAddError("bad","uiViewSecondary","",value,message); count++; }
  //}
  
  //value = document.getElementById("uiViewTertiary").value;
  //if(value != "") {	
  //   message = valDoValidateHostName(value);
  //   if(message!=null) { erlDoAddError("bad","uiViewTertiary","",value,message); count++; }
  //}
  
  value = document.getElementById("uiViewTimeout").value;
  if(value != "") {	
     message = valDoValidateInteger(value);
     if(message!=null) { erlDoAddError("bad","uiViewTimeout","",value,message); count++; }
  }
  
  value = document.getElementById("uiViewPollInterval").value;
  if(value != "") {	
     message = valDoValidateInteger(value);
     if(message!=null) { erlDoAddError("bad","uiViewPollInterval","",value,message); count++; }
  }
  
  value = document.getElementById("uiViewRetryCount").value;
  if(value != "") {	
     message = valDoValidateInteger(value);
     if(message!=null) { erlDoAddError("bad","uiViewRetryCount","",value,message); count++; }
  }
  
  if(count!=0) message="SNTP";
   else message="";
  return message;
}

function uiTimeZone()
{

}

function uiDoValidate()
{
  var msg;
  
  msg=SntpDoValidatePage();
  if(msg!="") {
  	erlDoShowPageErrors();
  	return jslPageErrorMsg(msg);
  }
  return true;
}

function uiDoChangeSNTPState(value)
{

	switch (value) {
		case "on":
			jslEnable("uiViewPrimary","uiViewSecondary","uiViewTertiary","uiViewTimeout","uiViewPollInterval","uiViewRetryCount")
			jslEnable("uiViewTimezone");
			//jslEnable("uiPostPrimary","uiPostSecondary","uiPostTertiary","uiPostTimeout","uiPostPollInterval","uiPostRetryCount","uiPostTimezone");
			break;
		case "off":
			jslDisable("uiViewPrimary","uiViewSecondary","uiViewTertiary","uiViewTimeout","uiViewPollInterval","uiViewRetryCount");
			jslDisable("uiViewTimezone");			
			//jslDisable("uiPostPrimary","uiPostSecondary","uiPostTertiary","uiPostTimeout","uiPostPollInterval","uiPostRetryCount","uiPostTimezone");
		default:
			break;
		}
}


function uiDoSave()
{
  jslViewToPostCheckBox("uiPostSNTPState","uiViewSNTPState");
  jslSetValue("uiPostPrimary","uiViewPrimary");
  jslSetValue("uiPostSecondary","uiViewSecondary");
  jslSetValue("uiPostTertiary","uiViewTertiary");
  jslSetValue("uiPostTimeout","uiViewTimeout");
  jslSetValue("uiPostPollInterval","uiViewPollInterval");
  jslSetValue("uiPostRetryCount","uiViewRetryCount");
  jslSetValue("uiPostTimezone","uiViewTimezone");
  jslViewToPostCheckBox("uiPostDayLight","uiViewDayLight");

  if(uiDoValidate()==true) {
     if(confirm("Warning : SNTP corrects the system time and you may logout if time difference is high."))
    {
       jslFormSubmit("uiPostForm");
    }
  }
}

function uiDoCancel()
{
  jslGoTo(null,"home");
}


</script>
