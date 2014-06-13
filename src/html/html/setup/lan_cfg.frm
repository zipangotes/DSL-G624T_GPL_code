<!-- LAN Configuration -->
<? if eq `lan` `$var:langrp` `<? include /usr/www_safe/html/setup/lan_mgmt.frm ?>` 
                                         `<? if eq `` `<? echo $var:langrp ?>` `<? include /usr/www_safe/html/setup/lan_mgmt.frm ?>` 
                                                                                                 `<? include /usr/www_safe/html/setup/lan_services.frm ?>` ?>` ?>
<!-- END LAN Configuration -->
