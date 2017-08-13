-- MYSQL数据库设计ChatDB数据库
-- ____________________________________________________________________________________________________________________________


-- 删除所有表
-- ____________________________________________________________________________________________________________________________
DROP TABLE IF EXISTS User;
DROP TABLE IF EXISTS UserState;
DROP TABLE IF EXISTS Friends;
DROP TABLE IF EXISTS User_FriendshipPolicy;
DROP TABLE IF EXISTS FriendGroups;
DROP TABLE IF EXISTS FriendType;
DROP TABLE IF EXISTS MessagesType;
DROP TABLE IF EXISTS Messages;
DROP TABLE IF EXISTS Nation;
DROP TABLE IF EXISTS Province;
DROP TABLE IF EXISTS City;
DROP TABLE IF EXISTS User_Groups;
DROP TABLE IF EXISTS User_GroupsToUser;
DROP TABLE IF EXISTS ProvinceType;

-- 表09--Nation—国家
-- ____________________________________________________________________________________________________________________________
CREATE TABLE Nation (
	N_ID   INT UNSIGNED AUTO_INCREMENT   COMMENT '国家ID	主键',
	N_Name VARCHAR(100) NOT NULL         COMMENT '名字',
	PRIMARY KEY ( N_ID )                                                                            -- 主键
)ENGINE=InnoDB DEFAULT CHARSET=utf8;
INSERT INTO Nation VALUES(0   ,'中国');
INSERT INTO Nation VALUES(NULL,'德国');
INSERT INTO Nation VALUES(NULL,'加拿大');
INSERT INTO Nation VALUES(NULL,'英国');
INSERT INTO Nation VALUES(NULL,'美国');
INSERT INTO Nation VALUES(NULL,'瑞典');
INSERT INTO Nation VALUES(NULL,'澳洲');
INSERT INTO Nation VALUES(NULL,'日本');
INSERT INTO Nation VALUES(NULL,'法国');
INSERT INTO Nation VALUES(NULL,'荷兰');
INSERT INTO Nation VALUES(NULL,'丹麦');
INSERT INTO Nation VALUES(NULL,'新加坡');
INSERT INTO Nation VALUES(NULL,'韩国');



-- 表10--ProvinceType—省份类别
-- ____________________________________________________________________________________________________________________________
CREATE TABLE ProvinceType (
	PT_ID       INT UNSIGNED AUTO_INCREMENT   COMMENT '省份ID,主键',
	PT_Name     VARCHAR(100) NOT NULL         COMMENT '名字',
	PRIMARY KEY ( PT_ID )                                                                            -- 主键
)ENGINE=InnoDB DEFAULT CHARSET=utf8;
INSERT INTO ProvinceType VALUES(NULL,'省');
INSERT INTO ProvinceType VALUES(NULL,'直辖市');
INSERT INTO ProvinceType VALUES(NULL,'特别行政区');
INSERT INTO ProvinceType VALUES(NULL,'自治区');


-- 表10--Province—省份
-- ____________________________________________________________________________________________________________________________
CREATE TABLE Province (
	P_ID       INT UNSIGNED AUTO_INCREMENT   COMMENT '省份ID		主键',
	P_Name     VARCHAR(100) NOT NULL         COMMENT '名字',
	P_NationID INT UNSIGNED NOT NULL         COMMENT '所属国家		外键',
	P_PT_ID    INT UNSIGNED NOT NULL         COMMENT '省类别		外键',
	PRIMARY KEY ( P_ID ),                                                                           -- 主键
	CONSTRAINT `FK_ID05` FOREIGN KEY (`P_NationID`) REFERENCES `Nation` (`N_ID`),                   -- 外键
	CONSTRAINT `FK_ID18` FOREIGN KEY (`P_PT_ID`)    REFERENCES `ProvinceType` (`PT_ID`)             -- 外键
)ENGINE=InnoDB DEFAULT CHARSET=utf8;
INSERT INTO Province VALUES(0,'河北省',1,1);
INSERT INTO Province VALUES(0,'山东省',1,1);
INSERT INTO Province VALUES(0,'辽宁省',1,1);
INSERT INTO Province VALUES(0,'黑龙江省',1,1);
INSERT INTO Province VALUES(0,'吉林省',1,1);
INSERT INTO Province VALUES(0,'甘肃省',1,1);
INSERT INTO Province VALUES(0,'青海省',1,1);
INSERT INTO Province VALUES(0,'河南省',1,1);
INSERT INTO Province VALUES(0,'江苏省',1,1);
INSERT INTO Province VALUES(0,'湖北省',1,1);
INSERT INTO Province VALUES(0,'湖南省',1,1);
INSERT INTO Province VALUES(0,'江西省',1,1);
INSERT INTO Province VALUES(0,'浙江省',1,1);
INSERT INTO Province VALUES(0,'广东省',1,1);
INSERT INTO Province VALUES(0,'云南省',1,1);
INSERT INTO Province VALUES(0,'福建省',1,1);
INSERT INTO Province VALUES(0,'台湾省',1,1);
INSERT INTO Province VALUES(0,'海南省',1,1);
INSERT INTO Province VALUES(0,'山西省',1,1);
INSERT INTO Province VALUES(0,'四川省',1,1);
INSERT INTO Province VALUES(0,'陕西省',1,1);
INSERT INTO Province VALUES(0,'贵州省',1,1);
INSERT INTO Province VALUES(0,'安徽省',1,1);
INSERT INTO Province VALUES(0,'重庆市',1,2);
INSERT INTO Province VALUES(0,'北京市',1,2);
INSERT INTO Province VALUES(0,'上海市',1,2);
INSERT INTO Province VALUES(0,'天津市',1,2);
INSERT INTO Province VALUES(0,'广西壮族自治区',1,4);
INSERT INTO Province VALUES(0,'内蒙古自治区',1,4);
INSERT INTO Province VALUES(0,'西藏自治区',1,4);
INSERT INTO Province VALUES(0,'新疆维吾尔自治区',1,4);
INSERT INTO Province VALUES(0,'宁夏回族自治区',1,4);
INSERT INTO Province VALUES(0,'澳门特别行政区',1,3);
INSERT INTO Province VALUES(0,'香港特别行政区',1,3);



-- 表11--City—城市
-- ____________________________________________________________________________________________________________________________
CREATE TABLE City (
	C_ID         INT UNSIGNED AUTO_INCREMENT   COMMENT  '城市ID',
	C_Name       VARCHAR(100) NOT NULL         COMMENT '名字',
	C_ProvinceID INT UNSIGNED NOT NULL         COMMENT '所属省份		外键',
	PRIMARY KEY ( C_ID ),                                                                           -- 主键
	CONSTRAINT `FK_ID06` FOREIGN KEY (`C_ProvinceID`) REFERENCES `Province` (`P_ID`)                -- 外键
)ENGINE=InnoDB DEFAULT CHARSET=utf8;
INSERT INTO City VALUES(NULL,'绥化市'    ,4);
INSERT INTO City VALUES(NULL,'哈尔滨市'  ,4);
INSERT INTO City VALUES(NULL,'齐齐哈尔市',4);
INSERT INTO City VALUES(NULL,'牡丹江市'  ,4);
INSERT INTO City VALUES(NULL,'双鸭山市'  ,4);


-- 表04--User_FriendshipPolicy—添加好有策略
-- ____________________________________________________________________________________________________________________________
CREATE TABLE User_FriendshipPolicy (
	U_FP_ID            INT UNSIGNED AUTO_INCREMENT   COMMENT '策略ID	主键',
	U_FriendshipPolicy VARCHAR(100)                  COMMENT '好友添加方式',
	PRIMARY KEY ( U_FP_ID )                                                                         -- 主键
)ENGINE=InnoDB DEFAULT CHARSET=utf8;
INSERT INTO User_FriendshipPolicy VALUES(NULL,'电话号码');
INSERT INTO User_FriendshipPolicy VALUES(NULL,'头像');
INSERT INTO User_FriendshipPolicy VALUES(NULL,'用户ID');



-- 表02--UserState—用户状态表
-- ____________________________________________________________________________________________________________________________
CREATE TABLE UserState (
	US_ID   INT UNSIGNED AUTO_INCREMENT,
	US_Name VARCHAR(100) NOT NULL      ,
	PRIMARY KEY ( US_ID )                                                                           -- 主键
)ENGINE=InnoDB DEFAULT CHARSET=utf8;
ALTER TABLE UserState                                              COMMENT='这是表的注释';
ALTER TABLE UserState MODIFY US_ID   INT UNSIGNED AUTO_INCREMENT   COMMENT '用户状态ID	主键';
ALTER TABLE UserState MODIFY US_Name VARCHAR(100) NOT NULL         COMMENT '状态名字';
INSERT INTO UserState VALUES(NULL,'在线');
INSERT INTO UserState VALUES(NULL,'隐身');
INSERT INTO UserState VALUES(NULL,'离线');


-- 表01--User--用户表
-- ____________________________________________________________________________________________________________________________
CREATE TABLE User (
	U_ID                    INT UNSIGNED AUTO_INCREMENT     COMMENT '用户ID		主键，自增',
	U_LoginID               VARCHAR(100)                    COMMENT '登录账号',
	U_NickName              VARCHAR(100)                    COMMENT '昵称',
	U_PassWord              VARCHAR(100)                    COMMENT '密码',
	U_SignaTure             VARCHAR(100)                    COMMENT '个性签名',
	U_Sex                   ENUM('男','女','其他') NOT NULL COMMENT '性别',
	U_Birthday              DATETIME                      COMMENT '生日',
	U_Telephone             VARCHAR(100)                  COMMENT '电话',
	U_Name                  VARCHAR(100)                  COMMENT '真实姓名',
	U_Email                 VARCHAR(100)                  COMMENT '邮箱',
	U_Intro                 VARCHAR(100)                  COMMENT '简介',
	U_HeadPortrait          VARCHAR(100)                  COMMENT '头像地址',
	U_ShengXiao             VARCHAR(100)                  COMMENT '生肖',
	U_Age                   INT                           COMMENT '年龄',
	U_Constellation         VARCHAR(100)                  COMMENT '星座',
	U_BloodType             ENUM('A','B','AB','O') NOT NULL   COMMENT '血型',
	U_SchoolTag             VARCHAR(100)                  COMMENT '毕业院校',
	U_Vocation              VARCHAR(100)                  COMMENT '职业',
	U_NationID              INT UNSIGNED NOT NULL         COMMENT '国家ID		外键',
	U_ProvinceID            INT UNSIGNED NOT NULL         COMMENT '省份ID		外键',
	U_CityID                INT UNSIGNED NOT NULL         COMMENT '城市ID		外键',
	U_FriendshipPolicyID    INT UNSIGNED NOT NULL         COMMENT '好友策略ID	外键',
	U_UserStateID           INT UNSIGNED NOT NULL         COMMENT '用户状态ID	外键',
	U_FriendPolicyQuestion  VARCHAR(100)                  COMMENT '好有策略问题',
	U_FriendPolicyAnswer    VARCHAR(100)                  COMMENT '好有策略答案',
	U_FriendPolicyPassword  VARCHAR(100)                  COMMENT '好有策略密码',
	PRIMARY KEY ( U_ID )                                                                            -- 主键
)ENGINE=InnoDB DEFAULT CHARSET=utf8;
ALTER TABLE User ADD CONSTRAINT FK_ID13 FOREIGN KEY(U_NationID)           REFERENCES Nation (N_ID);
ALTER TABLE User ADD CONSTRAINT FK_ID14 FOREIGN KEY(U_ProvinceID)         REFERENCES Province (P_ID);
ALTER TABLE User ADD CONSTRAINT FK_ID15 FOREIGN KEY(U_CityID)             REFERENCES City (C_ID);
ALTER TABLE User ADD CONSTRAINT FK_ID16 FOREIGN KEY(U_FriendshipPolicyID) REFERENCES User_FriendshipPolicy (U_FP_ID);
ALTER TABLE User ADD CONSTRAINT FK_ID17 FOREIGN KEY(U_UserStateID)        REFERENCES UserState (US_ID);
INSERT INTO User VALUES(NULL,'登录账号','昵称','密码','个性签名','男',NOW(),'电话','真实姓名','邮箱','简介','头像地址','生肖',24,'星座','A','毕业院校','职业',1,4,1,1,1,'ss','aa','AA');
INSERT INTO User VALUES(NULL,'登录账号','昵称','密码','个性签名','男',NOW(),'电话','真实姓名','邮箱','简介','头像地址','生肖',24,'星座','A','毕业院校','职业',1,4,1,1,1,'ss','aa','AA');


-- 表05--FriendGroups—好友分组
-- ____________________________________________________________________________________________________________________________
CREATE TABLE FriendGroups (
	FG_ID     INT UNSIGNED AUTO_INCREMENT   COMMENT '好友分组ID		主键',
	FG_Name   VARCHAR(100) NOT NULL         COMMENT '分组名字',
	FG_UserID INT UNSIGNED NOT NULL         COMMENT '用户ID		外键',
	PRIMARY KEY ( FG_ID ),                                                                          -- 主键
	CONSTRAINT `FK_ID01` FOREIGN KEY (`FG_UserID`) REFERENCES `User` (`U_ID`)                       -- 外键
)ENGINE=InnoDB DEFAULT CHARSET=utf8;
INSERT INTO FriendGroups VALUES(NULL,'默认分组',1);



-- 表06--FriendType—好友类型
-- ____________________________________________________________________________________________________________________________
CREATE TABLE FriendType (
	FT_ID   INT UNSIGNED AUTO_INCREMENT   COMMENT '好友类型ID		主键',
	FT_Name VARCHAR(100) NOT NULL         COMMENT '类型名称',
	PRIMARY KEY ( FT_ID )                                                                            -- 主键
)ENGINE=InnoDB DEFAULT CHARSET=utf8;
INSERT INTO User_FriendshipPolicy VALUES(NULL,'友好');
INSERT INTO User_FriendshipPolicy VALUES(NULL,'一般');


-- 表03--Friends—好友表
-- ____________________________________________________________________________________________________________________________
CREATE TABLE Friends (
	F_ID             INT UNSIGNED AUTO_INCREMENT   COMMENT '好友ID		主键',
	F_FriendID       INT UNSIGNED NOT NULL         COMMENT '朋友的ID	外键',
	F_UserID         INT UNSIGNED NOT NULL         COMMENT '自己的ID	外键',
	F_Name           VARCHAR(100) NOT NULL         COMMENT '备注昵称	NULL',
	F_FriendTypeID   INT UNSIGNED NOT NULL         COMMENT '好友类型	外键',
	F_FriendGroupsID INT UNSIGNED NOT NULL         COMMENT '所属分组	外键',
	PRIMARY KEY ( F_ID ),                                                                           -- 主键
	CONSTRAINT `FK_ID09` FOREIGN KEY (`F_FriendID`)       REFERENCES `User`         (`U_ID`)  ,     -- 外键
	CONSTRAINT `FK_ID10` FOREIGN KEY (`F_UserID`)         REFERENCES `User`         (`U_ID`)  ,     -- 外键
	CONSTRAINT `FK_ID11` FOREIGN KEY (`F_FriendTypeID`)   REFERENCES `FriendType`   (`FT_ID`)  ,    -- 外键
	CONSTRAINT `FK_ID12` FOREIGN KEY (`F_FriendGroupsID`) REFERENCES `FriendGroups` (`FG_ID`)       -- 外键
)ENGINE=InnoDB DEFAULT CHARSET=utf8;



-- 表07--MessagesType—消息类型
-- ____________________________________________________________________________________________________________________________
CREATE TABLE MessagesType (
	MT_ID   INT UNSIGNED AUTO_INCREMENT   COMMENT '消息类型ID		主键',
	MT_Name VARCHAR(100) NOT NULL         COMMENT '类型名称',
	PRIMARY KEY ( MT_ID )                                                                           -- 主键
)ENGINE=InnoDB DEFAULT CHARSET=utf8;
INSERT INTO User_FriendshipPolicy VALUES(NULL,'文字');
INSERT INTO User_FriendshipPolicy VALUES(NULL,'语音');
INSERT INTO User_FriendshipPolicy VALUES(NULL,'文件');



-- 表08--Messages—聊天记录
-- ____________________________________________________________________________________________________________________________
CREATE TABLE Messages (
	M_ID             INT UNSIGNED AUTO_INCREMENT   COMMENT '聊天记录ID				主键，自增',
	M_PostMessages   VARCHAR(100) NOT NULL         COMMENT '消息内容',
	M_status         VARCHAR(100) NOT NULL         COMMENT '接收状态',
	M_Time           DATETIME                      COMMENT '发送时间',
	M_MessagesTypeID INT UNSIGNED NOT NULL         COMMENT '消息类型ID				外键',
	M_FromUserID     INT UNSIGNED NOT NULL         COMMENT '发送者ID（指向用户表）	外键',
	M_ToUserID       INT UNSIGNED NOT NULL         COMMENT '接受者ID（指向用户表）	外键',
	PRIMARY KEY ( M_ID ),                                                                           -- 主键
	CONSTRAINT `FK_ID02` FOREIGN KEY (`M_MessagesTypeID`) REFERENCES `MessagesType` (`MT_ID`)  ,    -- 外键
	CONSTRAINT `FK_ID03` FOREIGN KEY (`M_FromUserID`)     REFERENCES `User` (`U_ID`)           ,    -- 外键
	CONSTRAINT `FK_ID04` FOREIGN KEY (`M_ToUserID`)       REFERENCES `User` (`U_ID`)                -- 外键
)ENGINE=InnoDB DEFAULT CHARSET=utf8;



-- 表12--User_Groups—用户群表
-- ___________________________________________________________________________________________________________________________
CREATE TABLE User_Groups (
	UG_ID         INT UNSIGNED AUTO_INCREMENT   COMMENT '群ID		主键',
	UG_Name       VARCHAR(100) NOT NULL         COMMENT '群名称',
	UG_CreateTime DATETIME                      COMMENT '创建时间',
	UG_AdminID    INT          NOT NULL         COMMENT '群主ID（指向用户表）',
	UG_ICon       VARCHAR(100) NOT NULL         COMMENT '群图标',
	UG_Notice     VARCHAR(100) NOT NULL         COMMENT '群公告',
	UG_Intro      VARCHAR(100) NOT NULL         COMMENT '群简介',
	PRIMARY KEY ( UG_ID )                                                                           -- 主键
)ENGINE=InnoDB DEFAULT CHARSET=utf8;



-- 表13--User_GroupsToUser—群用户关联表
-- ___________________________________________________________________________________________________________________________
CREATE TABLE User_GroupsToUser (
	UG_ID      INT UNSIGNED AUTO_INCREMENT   COMMENT 'ID		主键',
	UG_UserID  INT UNSIGNED NOT NULL         COMMENT '用户ID	外键',
	UG_GroupID INT UNSIGNED NOT NULL         COMMENT '群ID		外键',
	UG_CreateTime DATETIME                   COMMENT '发送时间	NULL',
	PRIMARY KEY ( UG_ID ),                                                                          -- 主键
	CONSTRAINT `FK_ID07` FOREIGN KEY (`UG_UserID`)  REFERENCES `User`         (`U_ID`)   ,          -- 外键
	CONSTRAINT `FK_ID08` FOREIGN KEY (`UG_GroupID`) REFERENCES `FriendGroups` (`FG_ID`)             -- 外键
)ENGINE=InnoDB DEFAULT CHARSET=utf8;
