
function wstDoSetWanType(contype)
{
  var page;
  page="../cgi-bin/webcm?getpage=/usr/www_safe/html/defs/<? echo $var:style ?>/menus/<? echo $var:main ?>.html&var:style=<? echo $var:style ?>&var:main=<? echo $var:main ?>&var:menu=setup&var:menutitle=Setup&var:pagename=";
  page=page+contype;
  page=page+"&var:pagetitle=New%20Connection&var:conid=<? echo $var:conid ?>&var:encapsid=<? echo $var:encapsid ?>&var:muxid=<? echo $var:muxid ?>&var:mode=<? echo $var:mode ?>";
  <? if eq $var:mode new `page=page+"&var:conname="+document.getElementById("uiViewConDesc").value;` `` ?>
  <? if eq `1` `$var:HasVlan` `page=page+"&var:sharing="+document.getElementById("uiViewVlanEnable").value;` `` ?>
  document.location.href=page;
}
function wstDoShowWanType(contype)
{
  var i;
  var selector=document.getElementById("uiViewWanType");

  <? if eq $var:mode new
  `document.getElementById("uiViewConDesc").value="<? echo $var:conname ?>"; document.getElementById("uiViewConDesc").focus();`
  `jslSetValue("uiViewConDesc","uiPostConDesc");` ?>
  for(i=0;i < selector.length; i++)
   if(selector.options[i].value==contype)
     selector.selectedIndex=i;
}
function wstDoCancel()
{
  document.getElementById("uiPostForm").method="GET";
  document.getElementById("uiPostPageName").value="home";
  document.getElementById("uiPostForm").submit();
}
function wstDoDeleteConnection()
{
  if("<? echo $var:mode ?>"=="new") { alert("You must create this connection first"); return; }
  //document.getElementById("uiPostForm").method="POST";
  document.getElementById("uiPostPageName").value="home";
  jslEnable("uiPostDelete"); 
  <? if eq `pppoa` `$var:contype`
  	` if(document.getElementById("uiPostPppUnNum").value=="1") {
  			document.getElementById("uiViewPppUnLan").value
  			document.getElementById("uiPostDeleteSwitch").name= "<? query $var:encapsid:pppoa:settings/pppun_lan ?>:settings/type";
  			document.getElementById("uiPostDeleteSwitch").value= "static";
  			document.getElementById("uiPostDeleteSwitchIPAddr").name= "<? query $var:encapsid:pppoa:settings/pppun_lan ?>:static:settings/ip";
  			document.getElementById("uiPostDeleteSwitchIPAddr").value= "<? query  <? query $var:encapsid:pppoa:settings/pppun_lan ?>:static:settings/ip ?>";
  			jslEnable("uiPostDeleteSwitch");
  			jslEnable("uiPostDeleteSwitchIPAddr");
  	  }
  	`
  	`
  	<? if eq `pppoe` `$var:contype`
  	   `if(document.getElementById("uiPostPppUnNum").value=="1") {
  			document.getElementById("uiViewPppUnLan").value
  			document.getElementById("uiPostDeleteSwitch").name= "<? query $var:conid:pppoe:settings/pppun_lan ?>:settings/type";
  			document.getElementById("uiPostDeleteSwitch").value= "static";
  			document.getElementById("uiPostDeleteSwitchIPAddr").name= "<? query $var:conid:pppoe:settings/pppun_lan ?>:static:settings/ip";
  			document.getElementById("uiPostDeleteSwitchIPAddr").value= "<? query  <? query $var:conid:pppoe:settings/pppun_lan ?>:static:settings/ip ?>";
  			jslEnable("uiPostDeleteSwitch");
  			jslEnable("uiPostDeleteSwitchIPAddr");
  	     }
  	   ` ` ` ?>
  	` ?>
  jslFormSubmit("uiPostForm"); 
}


function wstDoValidate()
{
  var value, count=0;
  var message=null;

  value = document.getElementById("uiPostConDesc").value;
  if(value=="") message="Empty connection name";
  else message = valDoValidateName(value);
  if(message!=null) { erlDoAddError("bad","uiViewConDesc","",value,message); count++; }

  if(count!=0) message="setup";
   else message="";
  return message;
}

function wstDoCheckFwNAT()
{

	 switch(document.getElementById("uiPostFwanNATState").value) {
  		case "1":
	       	jslEnable("uiViewNatControl");
  			jslEnable("uiViewFirewallControl");
			break;  		
  		case "0":
		default:
                     jslPostToViewCheckBox("uiViewNatControl","uiPostFwanNATState");
                     jslPostToViewCheckBox("uiViewFirewallControl","uiPostFwanNATState");
	       	jslDisable("uiViewNatControl");
  			jslDisable("uiViewFirewallControl");
			break;
  	}
}

function wstDoSave()
{
  <? if eq `new` `$var:mode`
     ` document.getElementById("uiPostBind").value="$var:conid"+"|"+"$var:muxid"+"|"+ <? if eq `1` `$var:HasDsl` `document.getElementById("uiViewPvc").value;` `"$var:encapsid"` ?>
        <? if eq `1` `$var:HasDsl` `jslSetValue("uiPostEncapsId","uiViewPvc");` ` ` ?>
        jslEnable("uiPostBind");
     `  
     ` //jslSetValue("uiPostEncapsId","uiViewPvc");
     ` ?>

}

