<!-- LAN Configuration -->
<? if eq `lan` `$var:langrp` `<? include /usr/www_safe/html/setup/lan_mgmt.js ?>` 
                                         `<? if eq `` `<? echo $var:langrp ?>` `<? include /usr/www_safe/html/setup/lan_mgmt.js ?>` 
                                                                                                 `<? include /usr/www_safe/html/setup/lan_services.js ?>` ?>` ?>
<!-- END LAN Configuration -->
