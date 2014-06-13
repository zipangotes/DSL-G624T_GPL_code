
<? if eq 1 1 '<script language="JavaScript">' '' ?>

<? if eq 1 1 '<? include /usr/www_safe/html/js/jsl.js ?>'   '<script language="JavaScript" src="/usr/www_safe/html/js/jsl.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/error.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/error.js"></script>' ?>
<? if eq 1 1 '<? include /usr/www_safe/html/js/val.js ?>' '<script language="JavaScript" src="/usr/www_safe/html/js/val.js"></script>' ?>

<? if eq 1 1 '' '<script language="JavaScript">' ?>

var mssid;
var qos;

function checkMACAddress(value)
{
    if((value.length == 0) || 
        (value.match("^[0-9a-fA-F]{2}\-[0-9a-fA-F]{2}\-[0-9a-fA-F]{2}\-[0-9a-fA-F]{2}\-[0-9a-fA-F]{2}\-[0-9a-fA-F]{2}$") == null))
        return false;
    else
        return true;
}

function uiValidateWdsName()
{
    if (document.getElementById("uiPostWdsName").value.length == 0)
    {
        alert ("Invalid WDS Name");
        return false;
    }
    else
        return true;
}

function uiValidateMacAddr(id)
{

    if (checkMACAddress(document.getElementById(id).value) == false) 
    {
        alert("Invalid MAC address. Enter MAC address in XX-XX-XX-XX-XX-XX format");
        return false;
    }
    return true;
}

function uiValidateRoot()
{
    if (document.getElementById("uiViewWdsRoot").value == "on" &&
        document.getElementById("uiViewUplink").value == "on")
    {
        alert("WDS root and uplink connection cannot be supported simultaneously!");
        return false;
    }
    return true;
}

function uiSelect(id, value)
{
    var i;
    var selector = document.getElementById(id);
    if(selector==null) return;
	
    for(i=0; i < selector.length; i++)
    {
        if(selector.options[i].value == value)
    	{
            selector.selectedIndex = i;
            return(value);
        }
    }
    return(selector.options[0].value);
}

function uiEnableMacField(id)
{
    var toggleValue = document.getElementById(id).value;

    if (document.getElementById("uiPostWdsMode").value == 3)

        return;

    if (toggleValue == "on")
    {
        if (id == "uiViewUplink")
            jslEnable("uiViewUplinkMac");
        else if (id == "uiViewDownlink1")
            jslEnable("uiViewDownlinkMac1");
        else if (id == "uiViewDownlink2")
            jslEnable("uiViewDownlinkMac2");
        else if (id == "uiViewDownlink3")
            jslEnable("uiViewDownlinkMac3");
        else if (id == "uiViewDownlink4")
            jslEnable("uiViewDownlinkMac4");
    }
    else 
    {
        if (id == "uiViewUplink")
            jslDisable("uiViewUplinkMac");
        else if (id == "uiViewDownlink1")
            jslDisable("uiViewDownlinkMac1");
        else if (id == "uiViewDownlink2")
            jslDisable("uiViewDownlinkMac2");
        else if (id == "uiViewDownlink3")
            jslDisable("uiViewDownlinkMac3");
        else if (id == "uiViewDownlink4")
            jslDisable("uiViewDownlinkMac4");
    }
}

function uiOnClickWdsPrivacy(id)
{
    if ((document.getElementById("uiPostWdsMode").value == 0) ||
        (document.getElementById("uiPostWdsMode").value == 1))
    {
        if (document.getElementById(id).value == "on")
            jslEnable("uiViewWdsSecret");
        else 
            jslDisable("uiViewWdsSecret");
    }
    else
    {
        document.getElementById("uiViewWdsPrivacy").value = "0";
        jslDisable("uiViewWdsPrivacy");
        jslDisable("uiViewWdsSecret");
    }
}

function uiOnClickWdsRoot(id)
{
    jslEnable("uiViewWdsRoot");
    if ((document.getElementById("uiPostWdsMode").value == 0) ||
        (document.getElementById("uiPostWdsMode").value == 1))
    {
        if (document.getElementById(id).value == "on")
        {
            document.getElementById("uiViewUplink").value = "off";
            document.getElementById("uiViewUplink").src="<? echo $var:btnCheckBoxOff ?>";
            jslDisable("uiViewUplink");
            jslDisable("uiViewUplinkMac");
        }
        else 
        {
            jslEnable("uiViewUplink");
            if (document.getElementById("uiViewUplink").value == "1")
                jslEnable("uiViewUplinkMac");
        }
    }
    else
    {
        document.getElementById("uiViewWdsRoot").value = "off";
        document.getElementById("uiViewWdsRoot").src="<? echo $var:btnCheckBoxOff ?>";
        jslDisable("uiViewWdsRoot");
    }
}

function uiDisableConfigParams(mode)
{
    if (mode == true)
    {
        jslEnable("uiViewWdsRoot");
        document.getElementById("uiViewWdsRoot").value = "off";
        document.getElementById("uiViewWdsRoot").src="<? echo $var:btnCheckBoxOff ?>";
        jslDisable("uiViewWdsRoot");
        jslDisable("uiViewWdsName");
        jslEnable("uiViewWdsPrivacy");
        document.getElementById("uiViewWdsPrivacy").value = "off";
        document.getElementById("uiViewWdsPrivacy").src="<? echo $var:btnCheckBoxOff ?>";
        jslDisable("uiViewWdsPrivacy");
        jslDisable("uiViewWdsSecret");
    }
    else
    {
        jslEnable("uiViewWdsRoot");
        jslEnable("uiViewWdsName");
        jslEnable("uiViewWdsPrivacy");
        jslEnable("uiViewWdsSecret");
    }
}

function uiDisableNetParams(mode)
{
    if (mode == true)
    {
        jslEnable("uiViewUplink");
        document.getElementById("uiViewUplink").value = "off";
        document.getElementById("uiViewUplink").src="<? echo $var:btnCheckBoxOff ?>";
        jslDisable("uiViewUplink");
        jslDisable("uiViewUplinkMac");
        jslEnable("uiViewDownlink1");
        document.getElementById("uiViewDownlink1").value = "off";
        document.getElementById("uiViewDownlink1").src="<? echo $var:btnCheckBoxOff ?>";
        jslDisable("uiViewDownlink1");
        jslDisable("uiViewDownlinkMac1");
        jslEnable("uiViewDownlink2");
        document.getElementById("uiViewDownlink2").value = "off";
        document.getElementById("uiViewDownlink2").src="<? echo $var:btnCheckBoxOff ?>";
        jslDisable("uiViewDownlink2");
        jslDisable("uiViewDownlinkMac2");
        jslEnable("uiViewDownlink3");
        document.getElementById("uiViewDownlink3").value = "off";
        document.getElementById("uiViewDownlink3").src="<? echo $var:btnCheckBoxOff ?>";
        jslDisable("uiViewDownlink3");
        jslDisable("uiViewDownlinkMac3");
        jslEnable("uiViewDownlink4");
        document.getElementById("uiViewDownlink4").value = "off";
        document.getElementById("uiViewDownlink4").src="<? echo $var:btnCheckBoxOff ?>";
        jslDisable("uiViewDownlink4");
        jslDisable("uiViewDownlinkMac4");
    }
    else
    {
        jslEnable("uiViewUplink");
        if (document.getElementById("uiViewUplink").value == "on")
            jslEnable("uiViewUplinkMac");
        jslEnable("uiViewDownlink1");
        if (document.getElementById("uiViewDownlink1").value == "on")
            jslEnable("uiViewDownlinkMac1");
        jslEnable("uiViewDownlink2");
        if (document.getElementById("uiViewDownlink2").value == "on")
            jslEnable("uiViewDownlinkMac2");
        jslEnable("uiViewDownlink3");
        if (document.getElementById("uiViewDownlink3").value == "on")
            jslEnable("uiViewDownlinkMac3");
        jslEnable("uiViewDownlink4");
        if (document.getElementById("uiViewDownlink4").value == "on")
            jslEnable("uiViewDownlinkMac4");
    }
}

function uiChangeWdsMode(mode)
{
    document.getElementById("uiPostWdsMode").value = mode;
    if (mode == 3) 
    {
        uiDisableConfigParams(true);
        uiDisableNetParams(true);
    }
    else if (mode == 2) 
    {
        uiDisableConfigParams(true);
        uiDisableNetParams(false);
    }
    else
    {
        uiDisableConfigParams(false);
        uiDisableNetParams(false); 
        uiOnClickWdsRoot("uiViewWdsRoot");

    }
}

function uiDoOnLoad()
{
    jslSetValue("uiViewWdsName", "uiPostWdsName");
    jslPostToViewCheckBox("uiViewWdsRoot","uiPostWdsRoot");
    jslPostToViewCheckBox("uiViewWdsPrivacy","uiPostWdsPrivacy");
    jslSetValue("uiViewWdsSecret", "uiPostWdsSecret");
    if (document.getElementById("uiViewWdsPrivacy").value == "on")
        jslEnable("uiViewWdsSecret");
     else 
        jslDisable("uiViewWdsSecret");

    jslDisable("uiViewAutoChSelect");
    jslDisable("uiViewAutoConfig");

    jslPostToViewCheckBox("uiViewUplink","uiPostUplink");
    jslSetValue("uiViewUplinkMac", "uiPostUplinkMac");
    if (document.getElementById("uiViewUplink").value == "on")
        jslEnable("uiViewUplinkMac");
    else 
        jslDisable("uiViewUplinkMac");

    jslPostToViewCheckBox("uiViewDownlink1","uiPostDownlink1");
    jslSetValue("uiViewDownlinkMac1", "uiPostDownlinkMac1");
    if (document.getElementById("uiViewDownlink1").value == "on")
        jslEnable("uiViewDownlinkMac1");
    else 
        jslDisable("uiViewDownlinkMac1");

    jslPostToViewCheckBox("uiViewDownlink2","uiPostDownlink2");
    jslSetValue("uiViewDownlinkMac2", "uiPostDownlinkMac2");
    if (document.getElementById("uiViewDownlink2").value == "on")
        jslEnable("uiViewDownlinkMac2");
    else 
        jslDisable("uiViewDownlinkMac2");

    jslPostToViewCheckBox("uiViewDownlink3","uiPostDownlink3");
    jslSetValue("uiViewDownlinkMac3", "uiPostDownlinkMac3");
    if (document.getElementById("uiViewDownlink3").value == "on")
        jslEnable("uiViewDownlinkMac3");
    else 
        jslDisable("uiViewDownlinkMac3");

    jslPostToViewCheckBox("uiViewDownlink4","uiPostDownlink4");
    jslSetValue("uiViewDownlinkMac4", "uiPostDownlinkMac4");
    if (document.getElementById("uiViewDownlink4").value == "on")
        jslEnable("uiViewDownlinkMac4");
    else 
        jslDisable("uiViewDownlinkMac4");
    
    uiSelect("uiViewWdsMode", document.getElementById("uiPostWdsMode").value);  
    uiChangeWdsMode(document.getElementById("uiPostWdsMode").value);
    mssid = <? query ap:settings/multiple_ssid ?>;
    qos = <? query ap:settings/qos_type ?>;
}

function uiDoSave()
{
    if (document.getElementById("uiPostWdsMode").value != 3)
    {
        if (mssid == 1)
        {
            alert("Multiple SSID is enabled. Cannot enable WDS");
            return;
        }
        if (qos == 1)
        {
            alert("QOS is enabled. Cannot enable WDS");
            return;
        }
    }
    
    jslDisable("uiPostAutoChSelect");
    jslDisable("uiPostAutoConfig");
    jslSetValue("uiPostWdsMode", "uiViewWdsMode");
    jslSetValue("uiPostWdsName", "uiViewWdsName");
    jslViewToPostCheckBox("uiPostWdsRoot", "uiViewWdsRoot");
    jslViewToPostCheckBox("uiPostWdsPrivacy", "uiViewWdsPrivacy");
    jslSetValue("uiPostWdsSecret", "uiViewWdsSecret");

    if (((document.getElementById("uiPostWdsMode").value == 0) ||
        (document.getElementById("uiPostWdsMode").value == 1)) &&
        (uiValidateWdsName() == false))
            return;

    if (document.getElementById("uiViewWdsPrivacy").value == "on" && 
        document.getElementById("uiViewWdsSecret").value.length == 0)
    {
        alert("Invalid WDS privacy secret!");
        return;
    }

    jslViewToPostCheckBox("uiPostUplink", "uiViewUplink");
    jslSetValue("uiPostUplinkMac", "uiViewUplinkMac");
    if (document.getElementById("uiViewUplink").value == "on" &&
       uiValidateMacAddr("uiViewUplinkMac") == false)
        return;

    jslViewToPostCheckBox("uiPostDownlink1","uiViewDownlink1");
    jslSetValue("uiPostDownlinkMac1","uiViewDownlinkMac1");
    if (document.getElementById("uiViewDownlink1").value == "on" &&
       uiValidateMacAddr("uiViewDownlinkMac1") == false) 
        return;

    jslViewToPostCheckBox("uiPostDownlink2","uiViewDownlink2");
    jslSetValue("uiPostDownlinkMac2","uiViewDownlinkMac2");
    if (document.getElementById("uiViewDownlink2").value == "on" &&
        uiValidateMacAddr("uiViewDownlinkMac2") == false)
        return;

    jslViewToPostCheckBox("uiPostDownlink3","uiViewDownlink3");
    jslSetValue("uiPostDownlinkMac3","uiViewDownlinkMac3");
    if (document.getElementById("uiViewDownlink3").value == "on" &&
       uiValidateMacAddr("uiViewDownlinkMac3") == false)
        return;

    jslViewToPostCheckBox("uiPostDownlink4","uiViewDownlink4");
    jslSetValue("uiPostDownlinkMac4","uiViewDownlinkMac4");
    if (document.getElementById("uiViewDownlink4").value == "on" &&
       uiValidateMacAddr("uiViewDownlinkMac4") == false)
        return;

    if (uiValidateRoot() == false)
        return;

    jslFormSubmit("uiPostForm");
}

function uiDoCancel()
{
    jslGoTo(null,"home");
}
</script>
