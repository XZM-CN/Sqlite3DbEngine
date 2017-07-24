PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;

CREATE TABLE [T_Warn] (
	LID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,
	LTime DOUBLE DEFAULT '' NOT NULL,
	Type INTEGER DEFAULT '0' NOT NULL,
	Flag INTEGER DEFAULT '0' NOT NULL,
	Count INTEGER DEFAULT '1' NOT NULL,
	Text VARCHAR DEFAULT '' NOT NULL,
	UID VARCHAR DEFAULT '' NOT NULL,
	SyncFlag INTEGER DEFAULT '0' NOT NULL);
INSERT INTO T_Warn VALUES(1,42901.466065000000526,6,0,1,'\Device\HarddiskVolume3\Windows\System32\SearchIndexer.exe','C:\Windows\System32\SearchProtocolHost.exe',0);
INSERT INTO T_Warn VALUES(2,42901.466065000000526,6,0,1,'\Device\HarddiskVolume3\Windows\System32\SearchIndexer.exe','C:\Windows\System32\SearchFilterHost.exe',0);
INSERT INTO T_Warn VALUES(3,42901.467001999997593,6,0,1,'\Device\HarddiskVolume3\Windows\explorer.exe','C:\Users\xzm\Desktop\FeiQ.exe',0);
INSERT INTO T_Warn VALUES(4,42901.467129999997268,6,0,1,'\Device\HarddiskVolume3\Users\xzm\Desktop\FeiQ.exe','C:\Program Files (x86)\SogouInput\7.7.0.6977\SGTool.exe',0);
INSERT INTO T_Warn VALUES(5,42901.467279999997116,6,0,1,'\Device\HarddiskVolume3\Windows\System32\svchost.exe','C:\Windows\System32\taskeng.exe',0);
INSERT INTO T_Warn VALUES(6,42901.467279999997116,6,0,1,'\Device\HarddiskVolume3\Windows\System32\taskeng.exe','C:\Program Files (x86)\Google\Update\GoogleUpdate.exe',0);
INSERT INTO T_Warn VALUES(7,42912.467580999997152,5,0,2,'\Device\HarddiskVolume3\Windows\explorer.exe','E:\QDAK-R-2.1.0\HXSafeSetup64.exe',0);
INSERT INTO T_Warn VALUES(8,42912.467569000000366,132,0,1,'\Device\HarddiskVolume3\Windows\explorer.exe','E:\QDAK-R-2.1.0\HXSafeSetup64.exe',0);
INSERT INTO T_Warn VALUES(9,42912.467569000000366,132,0,1,'\Device\HarddiskVolume3\Windows\System32\svchost.exe','E:\QDAK-R-2.1.0\HXSafeSetup64.exe',0);
INSERT INTO T_Warn VALUES(10,42912.467569000000366,6,0,1,'\Device\HarddiskVolume3\Windows\System32\svchost.exe','C:\Windows\System32\consent.exe',0);
INSERT INTO T_Warn VALUES(11,42912.467569000000366,132,0,1,'\Device\HarddiskVolume3\Windows\System32\consent.exe','E:\QDAK-R-2.1.0\HXSafeSetup64.exe',0);
INSERT INTO T_Warn VALUES(12,42912.467580999997152,6,0,2,'\Device\HarddiskVolume3\Windows\System32\winlogon.exe','C:\Windows\System32\AtBroker.exe',0);
INSERT INTO T_Warn VALUES(13,42912.467569000000366,6,0,1,'\Device\HarddiskVolume3\Windows\System32\AtBroker.exe','C:\Windows\System32\sethc.exe',0);
INSERT INTO T_Warn VALUES(14,42912.467580999997152,6,0,1,'\Device\HarddiskVolume3\Windows\System32\services.exe','C:\Program Files (x86)\Google\Update\GoogleUpdate.exe',0);
INSERT INTO T_Warn VALUES(15,42912.467580999997152,6,0,1,'\Device\HarddiskVolume3\Windows\System32\svchost.exe','C:\Windows\System32\dllhost.exe',0);
INSERT INTO T_Warn VALUES(16,42912.467580999997152,6,0,1,'\Device\HarddiskVolume3\Windows\System32\svchost.exe','C:\Windows\System32\dllhost.exe',0);
INSERT INTO T_Warn VALUES(17,42912.467766000001573,6,0,1,'\Device\HarddiskVolume3\Program Files\TDHX\HXSafeGuard\HXSafeApp.exe','C:\Program Files (x86)\SogouInput\7.7.0.6977\SGTool.exe',0);


-- http://www.runoob.com/sqlite/sqlite-delete.html
------------------------------------------------------------------------------
DELETE FROM sqlite_sequence WHERE name = 'T_Warn';
INSERT INTO sqlite_sequence VALUES('T_Warn',17);


COMMIT;
