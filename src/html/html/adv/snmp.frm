<input type="hidden" name="snmpcm:settings/snmpstate" value="<? query snmpcm:settings/snmpstate ?>" id="uiPostSNMPAgent">
<input type="hidden" name="snmpcm:settings/trapstate" value="<? query snmpcm:settings/trapstate ?>" id="uiPostSNMPTrap">

<? multiquery snmpcm:settings/community/entry
  ' <input type="hidden" name="snmpcm:settings/community/entry$00/name" value="$01" id="uiPostName$00">
   <input type="hidden" name="snmpcm:settings/community/entry$00/access" value="$02" id="uiPostAccessRights$00">
   ' ?>

<? multiquery snmpcm:settings/system
  ' 
   <input type="hidden" name="snmpcm:settings/system/sysdescription" value="$01" id="uiPostSysDesc" disabled>
   <input type="hidden" name="snmpcm:settings/system/sysoid" value="$02" id="uiPostSysOid">
   <input type="hidden" name="snmpcm:settings/system/sysuptime" value="$03" id="uiPostSysUptime" disabled>
   <input type="hidden" name="snmpcm:settings/system/syscontact" value="$04" id="uiPostSysContact">
   <input type="hidden" name="snmpcm:settings/system/sysname" value="$05" id="uiPostSysName">
   <input type="hidden" name="snmpcm:settings/system/syslocation" value="$06" id="uiPostSysLocation">
   <input type="hidden" name="snmpcm:settings/system/sysservice" value="$07" id="uiPostSysService" disabled>
   ' ?>

   <? multiquery snmpcm:settings/trapinfo/entry `
      <input type="hidden" name="snmpcm:settings/trapinfo/entry$00/trapdest"           value="$01" id="uiPostTrapDest$00">
      <input type="hidden" name="snmpcm:settings/trapinfo/entry$00/trapcommunity" value="$02" id="uiPostTrapComm$00">
      <input type="hidden" name="snmpcm:settings/trapinfo/entry$00/trapversion"      value="$03" id="uiPostTrapVer$00">
   ` ?>

