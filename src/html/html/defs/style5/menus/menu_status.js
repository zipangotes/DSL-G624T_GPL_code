
uiMenu["left"]= new Array();
uiMenu["left"]["btn_on"]=null;
uiMenu["left"]["btn_off"]=null;
uiMenu["left"]["btn_cur"]=null;
uiMenu["left"]["id"]=null;
uiMenu["left"]["next"]="netstats";

uiMenu["netstats"]= new Array();
uiMenu["netstats"]["btn_on"]="<? echo $var:btnMenuStatusNetStat ?>";
uiMenu["netstats"]["btn_off"]="<? echo $var:btnMenuStatusNetStat ?>";
uiMenu["netstats"]["btn_cur"]="";
uiMenu["netstats"]["id"]="uiViewMenuStatusNetStatBtn";
uiMenu["netstats"]["next"]="constatus";

uiMenu["constatus"]= new Array();
uiMenu["constatus"]["btn_on"]="<? echo $var:btnMenuStatusConStat ?>";
uiMenu["constatus"]["btn_off"]="<? echo $var:btnMenuStatusConStat ?>";
uiMenu["constatus"]["btn_cur"]="";
uiMenu["constatus"]["id"]="uiViewMenuStatusConStatBtn";
uiMenu["constatus"]["next"]="dhcpclients";

uiMenu["dhcpclients"]= new Array();
uiMenu["dhcpclients"]["btn_on"]="<? echo $var:btnMenuStatusDHCP ?>";
uiMenu["dhcpclients"]["btn_off"]="<? echo $var:btnMenuStatusDHCP ?>";
uiMenu["dhcpclients"]["btn_cur"]="";
uiMenu["dhcpclients"]["id"]="uiViewMenuStatusDHCPBtn";
uiMenu["dhcpclients"]["next"]="modemstatus";

uiMenu["modemstatus"]= new Array();
uiMenu["modemstatus"]["btn_on"]="<? echo $var:btnMenuStatusModem ?>";
uiMenu["modemstatus"]["btn_off"]="<? echo $var:btnMenuStatusModem ?>";
uiMenu["modemstatus"]["btn_cur"]="";
uiMenu["modemstatus"]["id"]="uiViewMenuStatusModemBtn";
uiMenu["modemstatus"]["next"]="prodinfo";

uiMenu["prodinfo"]= new Array();
uiMenu["prodinfo"]["btn_on"]="<? echo $var:btnMenuStatusSysInfo ?>";
uiMenu["prodinfo"]["btn_off"]="<? echo $var:btnMenuStatusSysInfo ?>";
uiMenu["prodinfo"]["btn_cur"]="";
uiMenu["prodinfo"]["id"]="uiViewMenuStatusSysInfoBtn";
uiMenu["prodinfo"]["next"]="syslog";

uiMenu["syslog"]= new Array();
uiMenu["syslog"]["btn_on"]="<? echo $var:btnMenuStatusSysLog ?>";
uiMenu["syslog"]["btn_off"]="<? echo $var:btnMenuStatusSysLog ?>";
uiMenu["syslog"]["btn_cur"]="";
uiMenu["syslog"]["id"]="uiViewMenuStatusSysLogBtn";
uiMenu["syslog"]["next"]="wdsreport";

uiMenu["wdsreport"]= new Array();
uiMenu["wdsreport"]["btn_on"]="<? echo $var:btnMenuStatusWdsReport ?>";
uiMenu["wdsreport"]["btn_off"]="<? echo $var:btnMenuStatusWdsReport ?>";
uiMenu["wdsreport"]["btn_cur"]="";
uiMenu["wdsreport"]["id"]="uiViewMenuStatusWdsReportBtn";
uiMenu["wdsreport"]["next"]="logout";

<? include /usr/www_safe/html/defs/$var:style/menus/<? echo $var:main ?>_logout.js ?>
