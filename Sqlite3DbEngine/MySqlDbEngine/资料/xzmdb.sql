/*
Navicat MySQL Data Transfer

Source Server         : localhost
Source Server Version : 50168
Source Host           : localhost:3306
Source Database       : xzmdb

Target Server Type    : MYSQL
Target Server Version : 50168
File Encoding         : 65001

Date: 2017-01-05 05:43:37
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for hzhtest
-- ----------------------------
DROP TABLE IF EXISTS `hzhtest`;
CREATE TABLE `hzhtest` (
  `myid` int(11) NOT NULL AUTO_INCREMENT,
  `mytime` datetime DEFAULT NULL,
  `myname` varchar(30) DEFAULT NULL,
  PRIMARY KEY (`myid`)
) ENGINE=InnoDB AUTO_INCREMENT=30 DEFAULT CHARSET=gb2312;

-- ----------------------------
-- Records of hzhtest
-- ----------------------------
INSERT INTO `hzhtest` VALUES ('1', '2006-06-01 01:01:01', '中文显示测试ChineseＣＨＩＮＡ1');
INSERT INTO `hzhtest` VALUES ('2', '2006-06-02 02:01:01', '中文显示测试ChineseＣＨＩＮＡ2');
INSERT INTO `hzhtest` VALUES ('3', '2006-06-03 00:01:01', null);
INSERT INTO `hzhtest` VALUES ('4', '2006-06-04 01:01:01', '中文显示测试ChineseＣＨＩＮＡ1');
INSERT INTO `hzhtest` VALUES ('5', '2006-06-05 02:01:01', '中文显示测试ChineseＣＨＩＮＡ2');
INSERT INTO `hzhtest` VALUES ('6', '2006-06-06 00:01:01', null);
INSERT INTO `hzhtest` VALUES ('7', '2006-06-07 01:01:01', '中文显示测试ChineseＣＨＩＮＡ1');
INSERT INTO `hzhtest` VALUES ('8', '2006-06-08 02:01:01', '中文显示测试ChineseＣＨＩＮＡ2');
INSERT INTO `hzhtest` VALUES ('9', '2006-06-09 00:01:01', null);
INSERT INTO `hzhtest` VALUES ('10', '2006-06-10 01:01:01', '中文显示测试ChineseＣＨＩＮＡ1');
INSERT INTO `hzhtest` VALUES ('11', '2006-06-11 02:01:01', '中文显示测试ChineseＣＨＩＮＡ2');
INSERT INTO `hzhtest` VALUES ('12', '2006-06-12 00:01:01', null);
INSERT INTO `hzhtest` VALUES ('13', '2006-06-13 01:01:01', '中文显示测试ChineseＣＨＩＮＡ1');
INSERT INTO `hzhtest` VALUES ('14', '2006-06-14 02:01:01', '中文显示测试ChineseＣＨＩＮＡ2');
INSERT INTO `hzhtest` VALUES ('15', '2006-06-15 00:01:01', null);
INSERT INTO `hzhtest` VALUES ('16', '2006-06-16 01:01:01', '中文显示测试ChineseＣＨＩＮＡ1');
INSERT INTO `hzhtest` VALUES ('17', '2006-06-17 02:01:01', '中文显示测试ChineseＣＨＩＮＡ2');
INSERT INTO `hzhtest` VALUES ('18', '2006-06-18 00:01:01', null);
INSERT INTO `hzhtest` VALUES ('19', '2006-06-19 01:01:01', '中文显示测试ChineseＣＨＩＮＡ1');
INSERT INTO `hzhtest` VALUES ('20', '2006-06-20 02:01:01', '中文显示测试ChineseＣＨＩＮＡ2');
INSERT INTO `hzhtest` VALUES ('21', '2006-06-21 00:01:01', null);
INSERT INTO `hzhtest` VALUES ('22', '2006-06-22 01:01:01', '中文显示测试ChineseＣＨＩＮＡ1');
INSERT INTO `hzhtest` VALUES ('23', '2006-06-23 02:01:01', '中文显示测试ChineseＣＨＩＮＡ2');
INSERT INTO `hzhtest` VALUES ('24', '2006-06-24 00:01:01', null);
INSERT INTO `hzhtest` VALUES ('25', '2006-06-25 01:01:01', '中文显示测试ChineseＣＨＩＮＡ1');
INSERT INTO `hzhtest` VALUES ('26', '2006-06-26 02:01:01', '中文显示测试ChineseＣＨＩＮＡ2');
INSERT INTO `hzhtest` VALUES ('27', '2006-06-27 00:01:01', null);
INSERT INTO `hzhtest` VALUES ('28', '2006-06-28 01:01:01', '中文显示测试ChineseＣＨＩＮＡ1');
INSERT INTO `hzhtest` VALUES ('29', '2006-06-29 02:01:01', '中文显示测试ChineseＣＨＩＮＡ2');

-- ----------------------------
-- Table structure for tlb_person_info
-- ----------------------------
DROP TABLE IF EXISTS `tlb_person_info`;
CREATE TABLE `tlb_person_info` (
  `id` int(11) NOT NULL COMMENT '主键ID',
  `name` varchar(255) DEFAULT NULL,
  `age` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk;

-- ----------------------------
-- Records of tlb_person_info
-- ----------------------------
INSERT INTO `tlb_person_info` VALUES ('1', '谢志敏', '28');
INSERT INTO `tlb_person_info` VALUES ('2', '贾晓鹏', '28');