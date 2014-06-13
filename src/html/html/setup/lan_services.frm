<input type="hidden" name="<? echo $var:langrp ?>:static:settings/ip" value="<? query $var:langrp:settings/type '<? query $var:langrp:static:settings/ip ?>' ?>" id="uiPostIPAddr">
<input type="hidden" name="<? echo $var:langrp ?>:static:settings/netmask" value="<? query $var:langrp:settings/type '<? query $var:langrp:static:settings/netmask ?>' ?>" id="uiPostNetMask">
<input type="hidden" name="<? echo $var:langrp ?>:settings/type" value="<? query $var:langrp:settings/type ?>" id="uiPostIPType">

<input type="hidden" name="resolver:settings/gw" value="<? query resolver:settings/gw ?>" id="uiPostGateway">
<input type="hidden" name="<? echo $var:langrp ?>:hostname:settings/name" value="<? query $var:langrp:hostname:settings/name ?>" id="uiPostHostname">
<input type="hidden" name="<? echo $var:langrp ?>:hostname:settings/domain" value="<? query $var:langrp:hostname:settings/domain ?>" id="uiPostDomainname">

<input type="hidden" name="<? query manager:status/$var:langrp/dhcps ?>:settings/start" value="<? query <? query manager:status/$var:langrp/dhcps ?>:settings/start ?>" id="uiPostStartIP" >
<input type="hidden" name="<? query manager:status/$var:langrp/dhcps ?>:settings/end" value="<? query <? query manager:status/$var:langrp/dhcps ?>:settings/end ?>" id="uiPostEndIP" >
<input type="hidden" name="<? query manager:status/$var:langrp/dhcps ?>:settings/lease" value="<? query <? query manager:status/$var:langrp/dhcps ?>:settings/lease ?>" id="uiPostLease" >
<input type="hidden" name="<? query manager:status/$var:langrp/relay ?>:settings/ip" value="<? query <? query manager:status/$var:langrp/relay ?>:settings/ip ?>" id="uiPostRelayIP" >

<input type="hidden" name="<? query manager:status/$var:langrp/relay ?>:settings/state" value="<? query <? query manager:status/$var:langrp/relay ?>:settings/state ?>" id="uiPostRelay" >
<input type="hidden" name="<? query manager:status/$var:langrp/dhcps ?>:settings/state" value="<? query <? query manager:status/$var:langrp/dhcps ?>:settings/state ?>" id="uiPostDHCPS" >

<input type="hidden" name="<? echo $var:langrp ?>:dhcpc:command/renew" value="" id="uiPostRenew" disabled>
<input type="hidden" name="<? echo $var:langrp ?>:dhcpc:command/release" value="" id="uiPostRelease"  disabled>

<input type="hidden" name="var:redirectpath" value="/usr/www_safe/html/blank.html" id="uiPostRedirect">
<input type="hidden" name="var:langrp" value="<? echo $var:langrp ?>" id="uiPostLanGrp">

