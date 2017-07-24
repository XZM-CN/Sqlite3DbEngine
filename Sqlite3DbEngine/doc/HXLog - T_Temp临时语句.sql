-- 向表T_Sys中插入列SyslogFlag类型为INTEGER
ALTER TABLE T_Sys ADD COLUMN SyslogFlag INTEGER;
-- 更新表T_Sys中SyslogFlag字段的值为0
UPDATE "T_Sys" SET SyslogFlag = 0;

-- 向表T_Warn中插入列SyslogFlag类型为INTEGER
ALTER TABLE T_Warn ADD COLUMN SyslogFlag INTEGER;
-- 更新表"T_Warn"中SyslogFlag字段的值为0
UPDATE "T_Warn" SET SyslogFlag = 0;