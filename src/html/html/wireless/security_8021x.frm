<input type="hidden" name="ap:settings/change_current_ssid" value="<? query ap:settings/change_current_ssid ?>" id="uiPostChangeCurrentSSID">
<input type="hidden" name="ap:settings/<? echo `ssid$var:currentSSID` ?>/privacy_type" value="<? query ap:settings/<? echo `ssid$var:currentSSID` ?>/privacy_type ?>" id="uiPostPrivacyType">
<input type="hidden" name="ap:settings/<? echo `ssid$var:currentSSID` ?>/radius_ip" value="<? query ap:settings/<? echo `ssid$var:currentSSID` ?>/radius_ip ?>" id="uiPostServerIp">
<input type="hidden" name="ap:settings/<? echo `ssid$var:currentSSID` ?>/radius_port" value="<? query ap:settings/<? echo `ssid$var:currentSSID` ?>/radius_port ?>" id="uiPostPort">
<input type="hidden" name="ap:settings/<? echo `ssid$var:currentSSID` ?>/radius_secret" value="<? query ap:settings/<? echo `ssid$var:currentSSID` ?>/radius_secret ?>" id="uiPostSecret">
<input type="hidden" name="ap:settings/<? echo `ssid$var:currentSSID` ?>/interval" value="<? query ap:settings/<? echo `ssid$var:currentSSID` ?>/interval ?>" id="uiPostGroupInterval">
