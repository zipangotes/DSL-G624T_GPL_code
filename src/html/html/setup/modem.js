<script language="JavaScript">

<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>

var MaxModulations  = <? multiquerycount sar:valid_settings/modulation ?>;
var CurrentModulationSelection=-1;
var hexVal=new Array();
var strVal=new Array();
function uiDoOnLoad()
{
	var i = 0;
	var j = 0;
	var k = 0;
	var index=0;
	var cmpStr,cmpStr1,cmpStr2;
	var ccat=new String("0x");
	<? multiquery sar:valid_settings/modulation "hexVal[i++]='$01';" ?>
	<? multiquery sar:valid_settings/modulation "strVal[j++]='$02';" ?>		
	cmpStr=new String(document.getElementById("uiPostModulation").value);
	cmpStr1=cmpStr.split("0x");
	cmpStr2=ccat.concat(cmpStr1[cmpStr1.length-1]);
	for(index=0;index<hexVal.length;index++)
	{
		if(hexVal[index]==cmpStr2)
			break;
	}
	for(k=0;k<hexVal.length;k++)
	{
		if(hexVal[k]=="0x0"&&strVal[k]!="NO_MODE")
		{
			document.getElementById("uiViewModulation"+k).disabled=true;
			//document.getElementById("uiViewModulation"+k).src="../html/defs/style5/images/ti_radio_off_grey.gif";
		}
	}	
	jslMyDoToggleRadio("uiViewModulation",index, MaxModulations );
	return true;
}

function uiDoSave()
{
   var str=new String("0x");
   if (CurrentModulationSelection == -1) {
     CurrentModulationSelection=document.getElementById("uiPostModulation").value;
   }
   document.getElementById("uiPostModulation").value = hexVal[CurrentModulationSelection];
   document.getElementById("uiPostForm").submit();
   return;
}
function uiDoCancel()
{
  jslGoTo(null,"home");
}
function uiChangeModulation (arg)
{
  jslMyDoToggleRadio("uiViewModulation",arg, MaxModulations);
  if (document.getElementById("uiViewModulation" + arg).value == "off")
	CurrentModulationSelection = -1;
  else
    CurrentModulationSelection = arg;
}
//Added
function jslMyDoToggleRadio(id,index,maxId)
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
		if(i==index||(hexVal[i]=="0x0"&&strVal[i]!=="NO_MODE")) continue;
      		radiobox = document.getElementById(id+i);
      		if(radiobox == null) return;

      		radiobox.value="off";
      		radiobox.src="<? echo $var:btnRadioBtnOff ?>";
	}
  }
  return false;
}
</script>
