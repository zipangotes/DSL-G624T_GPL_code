
uiMenu["left"]= new Array();
uiMenu["left"]["btn_on"]=null;
uiMenu["left"]["btn_off"]=null;
uiMenu["left"]["btn_cur"]=null;
uiMenu["left"]["id"]=null;
uiMenu["left"]["next"]="systemcommands";

uiMenu["systemcommands"]= new Array();
uiMenu["systemcommands"]["btn_on"]="<? echo $var:btnMenuToolsSysCom ?>";
uiMenu["systemcommands"]["btn_off"]="<? echo $var:btnMenuToolsSysCom ?>";
uiMenu["systemcommands"]["btn_cur"]="";
uiMenu["systemcommands"]["id"]="uiViewMenuToolsSysComBtn";
uiMenu["systemcommands"]["next"]="remotelog";

uiMenu["remotelog"]= new Array();
uiMenu["remotelog"]["btn_on"]="<? echo $var:btnMenuToolsRlog ?>";
uiMenu["remotelog"]["btn_off"]="<? echo $var:btnMenuToolsRlog ?>";
uiMenu["remotelog"]["btn_cur"]="";
uiMenu["remotelog"]["id"]="uiViewMenuToolsRlogBtn";
uiMenu["remotelog"]["next"]="usermanagement";

uiMenu["usermanagement"]= new Array();
uiMenu["usermanagement"]["btn_on"]="<? echo $var:btnMenuToolsUserM ?>";
uiMenu["usermanagement"]["btn_off"]="<? echo $var:btnMenuToolsUserM ?>";
uiMenu["usermanagement"]["btn_cur"]="";
uiMenu["usermanagement"]["id"]="uiViewMenuToolsUserMBtn";
uiMenu["usermanagement"]["next"]="updategateway";

uiMenu["updategateway"]= new Array();
uiMenu["updategateway"]["btn_on"]="<? echo $var:btnMenuToolsUpdate ?>";
uiMenu["updategateway"]["btn_off"]="<? echo $var:btnMenuToolsUpdate ?>";
uiMenu["updategateway"]["btn_cur"]="";
uiMenu["updategateway"]["id"]="uiViewMenuToolsUpdateBtn";
uiMenu["updategateway"]["next"]="pingtest";

uiMenu["pingtest"]= new Array();
uiMenu["pingtest"]["btn_on"]="<? echo $var:btnMenuToolsPing ?>";
uiMenu["pingtest"]["btn_off"]="<? echo $var:btnMenuToolsPing ?>";
uiMenu["pingtest"]["btn_cur"]="";
uiMenu["pingtest"]["id"]="uiViewMenuToolsPingBtn";
uiMenu["pingtest"]["next"]="modemtest";

uiMenu["modemtest"]= new Array();
uiMenu["modemtest"]["btn_on"]="<? echo $var:btnMenuToolsModem ?>";
uiMenu["modemtest"]["btn_off"]="<? echo $var:btnMenuToolsModem ?>";
uiMenu["modemtest"]["btn_cur"]="";
uiMenu["modemtest"]["id"]="uiViewMenuToolsModemBtn";
uiMenu["modemtest"]["next"]="logout";

<? include /usr/www_safe/html/defs/$var:style/menus/<? echo $var:main ?>_logout.js ?>
