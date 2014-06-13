
uiMenu["left"]= new Array();
uiMenu["left"]["btn_on"]=null;
uiMenu["left"]["btn_off"]=null;
uiMenu["left"]["btn_cur"]=null;
uiMenu["left"]["id"]=null;
uiMenu["left"]["next"]="wireless_sec";

uiMenu["wireless_sec"]= new Array();
uiMenu["wireless_sec"]["btn_on"]="<? echo $var:btnMenuAdvWirelessSec ?>";
uiMenu["wireless_sec"]["btn_off"]="<? echo $var:btnMenuAdvWirelessSec ?>";
uiMenu["wireless_sec"]["btn_cur"]="";
uiMenu["wireless_sec"]["id"]="uiViewMenuAdvWirelessSecBtn";
uiMenu["wireless_sec"]["next"]="wireless_mgmt";

uiMenu["wireless_mgmt"]= new Array();
uiMenu["wireless_mgmt"]["btn_on"]="<? echo $var:btnMenuAdvWirelessMgmt ?>";
uiMenu["wireless_mgmt"]["btn_off"]="<? echo $var:btnMenuAdvWirelessMgmt ?>";
uiMenu["wireless_mgmt"]["btn_cur"]="";
uiMenu["wireless_mgmt"]["id"]="uiViewMenuAdvWirelessMgmtBtn";
uiMenu["wireless_mgmt"]["next"]="voice";

uiMenu["voice"]= new Array();
uiMenu["voice"]["btn_on"]="<? echo $var:btnMenuAdvUPnP ?>";
uiMenu["voice"]["btn_off"]="<? echo $var:btnMenuAdvUPnP ?>";
uiMenu["voice"]["btn_cur"]="";
uiMenu["voice"]["id"]="uiViewMenuAdvUPnPBtn";
uiMenu["voice"]["next"]="upnp";

uiMenu["upnp"]= new Array();
uiMenu["upnp"]["btn_on"]="<? echo $var:btnMenuAdvUPnP ?>";
uiMenu["upnp"]["btn_off"]="<? echo $var:btnMenuAdvUPnP ?>";
uiMenu["upnp"]["btn_cur"]="";
uiMenu["upnp"]["id"]="uiViewMenuAdvUPnPBtn";
uiMenu["upnp"]["next"]="sntp";

uiMenu["sntp"]= new Array();
uiMenu["sntp"]["btn_on"]="<? echo $var:btnMenuAdvSNTP ?>";
uiMenu["sntp"]["btn_off"]="<? echo $var:btnMenuAdvSNTP ?>";
uiMenu["sntp"]["btn_cur"]="";
uiMenu["sntp"]["id"]="uiViewMenuAdvSNTPBtn";
uiMenu["sntp"]["next"]="snmp";

uiMenu["snmp"]= new Array();
uiMenu["snmp"]["btn_on"]="<? echo $var:btnMenuToolsSNMP ?>";
uiMenu["snmp"]["btn_off"]="<? echo $var:btnMenuToolsSNMP ?>";
uiMenu["snmp"]["btn_cur"]="";
uiMenu["snmp"]["id"]="uiViewMenuToolsSNMPBtn";
uiMenu["snmp"]["next"]="portforwarding";

uiMenu["portforwarding"]= new Array();
uiMenu["portforwarding"]["btn_on"]="<? echo $var:btnMenuAdvPortf ?>";
uiMenu["portforwarding"]["btn_off"]="<? echo $var:btnMenuAdvPortf ?>";
uiMenu["portforwarding"]["btn_cur"]="";
uiMenu["portforwarding"]["id"]="uiViewMenuAdvPortForwardingBtn";
uiMenu["portforwarding"]["next"]="accesscontrol";

uiMenu["advancedsecurity"]= new Array();
uiMenu["advancedsecurity"]["btn_on"]="<? echo $var:btnMenuAdvSec ?>";
uiMenu["advancedsecurity"]["btn_off"]="<? echo $var:btnMenuAdvSec ?>";
uiMenu["advancedsecurity"]["btn_cur"]="";
uiMenu["advancedsecurity"]["id"]="uiViewMenuAdvAdvSecurityBtn";
uiMenu["advancedsecurity"]["next"]="accesscontrol";

uiMenu["accesscontrol"]= new Array();
uiMenu["accesscontrol"]["btn_on"]="<? echo $var:btnMenuAdvAccess ?>";
uiMenu["accesscontrol"]["btn_off"]="<? echo $var:btnMenuAdvAccess ?>";
uiMenu["accesscontrol"]["btn_cur"]="";
uiMenu["accesscontrol"]["id"]="uiViewMenuAdvAccessControlBtn";
uiMenu["accesscontrol"]["next"]="lanclients";

uiMenu["lanclients"]= new Array();
uiMenu["lanclients"]["btn_on"]="<? echo $var:btnMenuAdvLANC ?>";
uiMenu["lanclients"]["btn_off"]="<? echo $var:btnMenuAdvLANC ?>";
uiMenu["lanclients"]["btn_cur"]="";
uiMenu["lanclients"]["id"]="uiViewMenuAdvLANClientsBtn";
uiMenu["lanclients"]["next"]="bridgefilters";

uiMenu["bridgefilters"]= new Array();
uiMenu["bridgefilters"]["btn_on"]="<? echo $var:btnMenuAdvBridge ?>";
uiMenu["bridgefilters"]["btn_off"]="<? echo $var:btnMenuAdvBridge ?>";
uiMenu["bridgefilters"]["btn_cur"]="";
uiMenu["bridgefilters"]["id"]="uiViewMenuAdvBridgeFiltersBtn";
uiMenu["bridgefilters"]["next"]="webfilters";

uiMenu["webfilters"]= new Array();
uiMenu["webfilters"]["btn_on"]="<? echo $var:btnMenuAdvWebFilters ?>";
uiMenu["webfilters"]["btn_off"]="<? echo $var:btnMenuAdvWebFilters ?>";
uiMenu["webfilters"]["btn_cur"]="";
uiMenu["webfilters"]["id"]="uiViewMenuAdvWebFiltersBtn";
uiMenu["webfilters"]["next"]="multicast";

uiMenu["multicast"]= new Array();
uiMenu["multicast"]["btn_on"]="<? echo $var:btnMenuAdvMulti ?>";
uiMenu["multicast"]["btn_off"]="<? echo $var:btnMenuAdvMulti ?>";
uiMenu["multicast"]["btn_cur"]="";
uiMenu["multicast"]["id"]="uiViewMenuAdvMulticastBtn";
uiMenu["multicast"]["next"]="staticrouting";

uiMenu["staticrouting"]= new Array();
uiMenu["staticrouting"]["btn_on"]="<? echo $var:btnMenuAdvStatic ?>";
uiMenu["staticrouting"]["btn_off"]="<? echo $var:btnMenuAdvStatic ?>";
uiMenu["staticrouting"]["btn_cur"]="";
uiMenu["staticrouting"]["id"]="uiViewMenuAdvStaticRoutingBtn";
uiMenu["staticrouting"]["next"]="dynamicrouting";

uiMenu["dynamicrouting"]= new Array();
uiMenu["dynamicrouting"]["btn_on"]="<? echo $var:btnMenuAdvDynamic ?>";
uiMenu["dynamicrouting"]["btn_off"]="<? echo $var:btnMenuAdvDynamic ?>";
uiMenu["dynamicrouting"]["btn_cur"]="";
uiMenu["dynamicrouting"]["id"]="uiViewMenuAdvDynamicRoutingBtn";
uiMenu["dynamicrouting"]["next"]="logout";

<? include /usr/www_safe/html/defs/$var:style/menus/<? echo $var:main ?>_logout.js ?>
