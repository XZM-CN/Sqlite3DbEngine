-- 更新表T_Sys、T_Event、T_Warn的SyslogFlag值为0
UPDATE "T_Sys"   SET SyslogFlag = 0;
UPDATE "T_Event" SET SyslogFlag = 0;
UPDATE "T_Warn"  SET SyslogFlag = 0;