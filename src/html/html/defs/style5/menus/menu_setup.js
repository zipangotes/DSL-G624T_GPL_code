
uiMenu["left"]= new Array();
uiMenu["left"]["btn_on"]=null;
uiMenu["left"]["btn_off"]=null;
uiMenu["left"]["btn_cur"]=null;
uiMenu["left"]["id"]=null;
uiMenu["left"]["next"]="dhcpconfig";

uiMenu["dhcpconfig"]= new Array();
uiMenu["dhcpconfig"]["btn_on"]="<? echo $var:btnMenuSetupDHCP ?>";
uiMenu["dhcpconfig"]["btn_off"]="<? echo $var:btnMenuSetupDHCP ?>";
uiMenu["dhcpconfig"]["btn_cur"]="";
uiMenu["dhcpconfig"]["id"]="uiViewMenuSetupDHCPBtn";
uiMenu["dhcpconfig"]["next"]="managementip";

uiMenu["managementip"]= new Array();
uiMenu["managementip"]["btn_on"]="<? echo $var:btnMenuSetupMgmtIP ?>";
uiMenu["managementip"]["btn_off"]="<? echo $var:btnMenuSetupMgmtIP ?>";
uiMenu["managementip"]["btn_cur"]="";
uiMenu["managementip"]["id"]="uiViewMenuSetupMgmtIPBtn";
uiMenu["managementip"]["next"]="newconnection";

uiMenu["newconnection"]= new Array();
uiMenu["newconnection"]["btn_on"]="<? echo $var:btnMenuSetupNewConn ?>";
uiMenu["newconnection"]["btn_off"]="<? echo $var:btnMenuSetupNewConn ?>";
uiMenu["newconnection"]["btn_cur"]="";
uiMenu["newconnection"]["id"]="uiViewMenuSetupNewConnBtn";
uiMenu["newconnection"]["next"]="modemsetup";

uiMenu["modemsetup"]= new Array();
uiMenu["modemsetup"]["btn_on"]="<? echo $var:btnMenuSetupModem ?>";
uiMenu["modemsetup"]["btn_off"]="<? echo $var:btnMenuSetupModem ?>";
uiMenu["modemsetup"]["btn_cur"]="";
uiMenu["modemsetup"]["id"]="uiViewMenuSetupModemBtn";

<? setvariable var:numberofconnections <? multiquerycount manager:command/connection/list ?> ?>
<? setvariable var:lastconnectionindex '0' ?>
<? repeat [ 0 $var:numberofconnections [ `<? setvariable var:lastconnectionindex '$00' ?>` ?>

uiMenu["modemsetup"]["next"]=<? if eq 0 $var:numberofconnections `"logout"` `"connection0"` ?>;

<? repeat [ 0  $var:numberofconnections [ `
uiMenu["connection$00"]= new Array();
uiMenu["connection$00"]["btn_on"]="<? echo $var:btnMenuSetupConn ?>$00<? echo $var:btnMenuSetupConnExt ?>";
uiMenu["connection$00"]["btn_off"]="<? echo $var:btnMenuSetupConn ?>$00<? echo $var:btnMenuSetupConnExt ?>";
uiMenu["connection$00"]["btn_cur"]="";
uiMenu["connection$00"]["id"]="uiViewMenuSetupConn$00";
uiMenu["connection$00"]["next"]=<? if eq $00 $var:lastconnectionindex `"logout"` `"connection"+($00+1);` ?>
` ?>

function DoMenuRollOut(id)
{
    id.style.Color = "<? echo $var:menuLeftRollOutColor ?>";
    id.style.backgroundColor = "<? echo $var:menuLeftRollOutBGColor ?>";
}

function DoMenuRollOver(id)
{
    id.style.Color = "<? echo $var:menuLeftRollOverColor ?>";
    id.style.backgroundColor = "<? echo $var:menuLeftRollOverBGColor ?>";
}

<? include /usr/www_safe/html/defs/$var:style/menus/<? echo $var:main ?>_logout.js ?>

