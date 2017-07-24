PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;



CREATE TABLE [T_Sys] (
	LID INTEGER PRIMARY KEY AUTOINCREMENT UNIQUE,
	LTime DOUBLE DEFAULT '' NOT NULL,
	Type INTEGER DEFAULT '0' NOT NULL,
	Text VARCHAR DEFAULT '' NOT NULL,
	UID VARCHAR DEFAULT '' NOT NULL,
	UName VARCHAR DEFAULT '' NOT NULL,
	SyslogFlag INTEGER DEFAULT '0' NOT NULL,
	SyncFlag INTEGER DEFAULT '0' NOT NULL);
INSERT INTO T_Sys VALUES(1,42899.6466900000014,2,'程序白名单管理操作验证权限','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(2,42899.646700999997848,2,'验证权限成功','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(3,42899.646700999997848,2,'执行全盘扫描','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(4,42899.646770999999715,2,'执行白名单程序扫描','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(5,42899.65038199999981,2,'执行白名单程序扫描','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(6,42899.655417000001761,2,'程序白名单管理操作验证权限','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(7,42899.655417000001761,2,'验证权限成功','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(8,42899.655427999998208,2,'执行全盘扫描','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(9,42899.655891000002157,2,'程序白名单管理操作验证权限','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(10,42899.655902999998943,2,'验证权限成功','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(11,42899.655902999998943,2,'执行全盘扫描','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(12,42899.655925999999454,2,'执行白名单程序扫描','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(13,42899.656504999999014,2,'程序白名单管理操作验证权限','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(14,42899.656504999999014,2,'验证权限成功','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(15,42899.656504999999014,2,'执行全盘扫描','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(16,42899.656574000000546,2,'执行白名单程序扫描','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(17,42899.664722000001347,2,'执行白名单程序入库','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(18,42901.419698999998216,0,'开启自身保护','','',0,0);
INSERT INTO T_Sys VALUES(19,42901.463356000000201,0,'查询应用预警日志','','',0,0);
INSERT INTO T_Sys VALUES(20,42901.463750000002621,2,'程序白名单管理操作验证权限','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(21,42901.463761999999407,2,'验证权限失败','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(22,42901.464051000002654,2,'程序白名单管理操作验证权限','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(23,42901.464051000002654,2,'验证权限成功','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(24,42901.464362999999139,0,'查询应用预警日志','','',0,0);
INSERT INTO T_Sys VALUES(25,42901.464513999999325,4,'日志审计操作验证权限','_audit','Audit',0,0);
INSERT INTO T_Sys VALUES(26,42901.464513999999325,4,'验证权限失败','_audit','Audit',0,0);
INSERT INTO T_Sys VALUES(27,42901.464583000000858,4,'日志审计操作验证权限','_audit','Audit',0,0);
INSERT INTO T_Sys VALUES(28,42901.464583000000858,4,'验证权限成功','_audit','Audit',0,0);
INSERT INTO T_Sys VALUES(29,42901.467836000003442,2,'修改安全防护配置验证权限','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(30,42901.467846999999894,2,'验证权限成功','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(31,42901.467846999999894,2,'关闭安全防护','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(32,42902.751204000000142,0,'_sa登录时没有插入合法的安全U卡','_sa','SuperAdmin',0,0);
INSERT INTO T_Sys VALUES(33,42905.411504999996395,0,'_admin登录时没有插入合法的安全U卡','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(34,42905.411747999998624,0,'_admin登录时没有插入合法的安全U卡','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(35,42912.4679049999977,2,'修改安全防护配置验证权限','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(36,42912.4679049999977,2,'验证权限成功','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(37,42912.467917000001764,2,'关闭安全防护','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(38,42914.708565000000816,2,'程序白名单管理操作验证权限','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(39,42914.708565000000816,2,'验证权限成功','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(40,42914.708575999997264,2,'执行全盘扫描','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(41,42914.710531999997328,2,'程序白名单管理操作验证权限','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(42,42914.710531999997328,2,'验证权限成功','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(43,42914.710531999997328,2,'执行全盘扫描','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(44,42914.728367999996406,2,'程序白名单管理操作验证权限','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(45,42914.728380000000469,2,'验证权限成功','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(46,42914.728380000000469,2,'执行全盘扫描','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(47,42914.729015999997499,2,'程序白名单管理操作验证权限','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(48,42914.729015999997499,2,'验证权限成功','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(49,42914.731331000002681,2,'程序白名单管理操作验证权限','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(50,42914.731342999999468,2,'验证权限成功','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(51,42914.731342999999468,2,'执行全盘扫描','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(52,42914.731446999998297,2,'程序白名单管理操作验证权限','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(53,42914.731446999998297,2,'验证权限成功','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(54,42914.734629999999014,2,'程序白名单管理操作验证权限','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(55,42914.734641000002739,2,'验证权限成功','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(56,42914.734641000002739,2,'执行全盘扫描','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(57,42915.732372999998915,1,'修改自身安全配置验证权限','_sa','SuperAdmin',0,0);
INSERT INTO T_Sys VALUES(58,42915.73238400000264,1,'验证权限失败','_sa','SuperAdmin',0,0);
INSERT INTO T_Sys VALUES(59,42915.732465000000954,1,'修改自身安全配置验证权限','_sa','SuperAdmin',0,0);
INSERT INTO T_Sys VALUES(60,42915.732476999997745,1,'验证权限成功','_sa','SuperAdmin',0,0);
INSERT INTO T_Sys VALUES(61,42936.399791999996524,0,'_admin登录时没有插入合法的安全U卡','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(62,42936.400184999998601,0,'_admin登录时没有插入合法的安全U卡','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(63,42936.403367999999318,0,'_admin登录时没有插入合法的安全U卡','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(64,42936.404884000003222,0,'_admin登录时没有插入合法的安全U卡','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(65,42936.4396179999967,0,'_admin登录时没有插入合法的安全U卡','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(66,42936.484271000001171,0,'_admin登录时没有插入合法的安全U卡','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(67,42936.595972000002803,0,'_admin登录时没有插入合法的安全U卡','_admin','Admin',0,0);
INSERT INTO T_Sys VALUES(68,42940.474420999998981,4,'开启Syslog日志上传','_audit','Audit',0,0);
INSERT INTO T_Sys VALUES(69,42940.478992999996988,4,'开启Syslog日志上传','_audit','Audit',0,0);
INSERT INTO T_Sys VALUES(70,42940.48114599999826,4,'开启Syslog日志上传','_audit','Audit',0,0);
INSERT INTO T_Sys VALUES(71,42940.488772999997309,4,'开启Syslog日志上传','_audit','Audit',0,0);
INSERT INTO T_Sys VALUES(72,42940.491249999999125,4,'开启Syslog日志上传','_audit','Audit',0,0);
INSERT INTO T_Sys VALUES(73,42940.492684999997437,4,'开启Syslog日志上传','_audit','Audit',0,0);
INSERT INTO T_Sys VALUES(74,42940.497094999998807,4,'开启Syslog日志上传','_audit','Audit',0,0);
INSERT INTO T_Sys VALUES(75,42940.534433000000719,4,'开启Syslog日志上传','_audit','Audit',0,0);


-- http://www.runoob.com/sqlite/sqlite-delete.html
------------------------------------------------------------------------------
DELETE FROM sqlite_sequence WHERE name = 'T_Sys';
INSERT INTO sqlite_sequence VALUES('T_Sys',75);
-- 更新字段name = 'T_Sys'时seq字段值
UPDATE "sqlite_sequence" SET seq = 75 WHERE name = 'T_Sys';


COMMIT;
