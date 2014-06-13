<script language="JavaScript">
<? include /usr/www_safe/html/js/jsl.js ?>
<? include /usr/www_safe/html/js/error.js ?>
var hexVal=new Array();
var strVal=new Array();
function uiDoOnLoad()
{
	var i = 0;
	var j = 0;
	var k = 0;
	var cmpStr,cmpStr1,cmpStr2;
	var ccat=new String("0x");
	<? multiquery sar:valid_settings/modulation "hexVal[i++]='$01';" ?>
	<? multiquery sar:valid_settings/modulation "strVal[j++]='$02';" ?>	
	cmpStr=new String(document.getElementById("uiPostModulationVal").value);
	cmpStr1=cmpStr.split("0x");
	cmpStr2=ccat.concat(cmpStr1[cmpStr1.length-1]);
	for(k=0;k<strVal.length;k++)
	{
		if(hexVal[k]==cmpStr2)
			break;
	}
	document.getElementById("uiViewModulationStr").selectedIndex=k;
	document.getElementById("uiViewModulationVal").value=hexVal[k];
}
function uiChangeModulationStr(value)
{
	var cnt=i=0;
	for(cnt=0;cnt<hexVal.length;cnt++)
	{
		if(strVal[cnt]==value)
			break;
	}
	document.getElementById("uiViewModulationVal").value=hexVal[cnt];
}
function uiDoCancel()
{
  jslGoTo(null,"home");
}

function uiDoSave()
{
	var setHex=document.getElementById("uiViewModulationVal").value;
	var setStr=document.getElementById("uiViewModulationStr").value;
	var str1=new String(setHex);
	var str2=str1.split("0x");
	if(setHex=="0x0"&&setStr!="NO_MODE")
	{
		alert("Choose NO_MODE for 0x0 Modulation Value");
	}
	else
	{
		document.getElementById("uiPostModulationVal").name="sar:settings/modulation";
		document.getElementById("uiPostModulationVal").value=document.getElementById("uiViewModulationVal").value;
		document.getElementById("uiPostForm").submit();
	}
}
</script>
