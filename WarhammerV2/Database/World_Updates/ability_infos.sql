/*
Navicat MySQL Data Transfer

Source Server         : Local
Source Server Version : 50515
Source Host           : localhost:3306
Source Database       : war-world

Target Server Type    : MYSQL
Target Server Version : 50515
File Encoding         : 65001

Date: 2011-09-13 03:04:50
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `ability_infos`
-- ----------------------------
DROP TABLE IF EXISTS `ability_infos`;
CREATE TABLE `ability_infos` (
  `Ability_Infos_ID` text,
  `Entry` smallint(5) unsigned NOT NULL,
  `Level` tinyint(3) unsigned DEFAULT NULL,
  `AbilityType` smallint(5) unsigned DEFAULT NULL,
  `MinRange` int(11) DEFAULT NULL,
  `MaxRange` int(11) DEFAULT NULL,
  `CastTime` int(11) DEFAULT NULL,
  `CoolDown` int(11) DEFAULT NULL,
  `TargetType` tinyint(3) unsigned DEFAULT NULL,
  `Damage` int(11) DEFAULT NULL,
  `ActionPoints` smallint(5) unsigned DEFAULT NULL,
  `CareerLine` tinyint(3) unsigned DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of ability_infos
-- ----------------------------
INSERT INTO `ability_infos` VALUES ('a7712c1f-dd9f-11e0-95b8-60eb69729418', '8159', '1', '1', '0', '100', '3000', '0', '0', '119', '30', '11');
INSERT INTO `ability_infos` VALUES ('a7713112-dd9f-11e0-95b8-60eb69729418', '8159', '2', '1', '0', '100', '3000', '0', '0', '131', '30', '11');
INSERT INTO `ability_infos` VALUES ('a77134ef-dd9f-11e0-95b8-60eb69729418', '8159', '3', '1', '0', '100', '3000', '0', '0', '145', '30', '11');
INSERT INTO `ability_infos` VALUES ('a771380f-dd9f-11e0-95b8-60eb69729418', '8159', '4', '1', '0', '100', '3000', '0', '0', '159', '30', '11');
INSERT INTO `ability_infos` VALUES ('a77139ee-dd9f-11e0-95b8-60eb69729418', '8081', '1', '1', '0', '5', '0', '0', '0', '61', '40', '9');
INSERT INTO `ability_infos` VALUES ('a7713b52-dd9f-11e0-95b8-60eb69729418', '8081', '2', '1', '0', '5', '0', '0', '0', '66', '40', '9');
INSERT INTO `ability_infos` VALUES ('a7713ca9-dd9f-11e0-95b8-60eb69729418', '8081', '3', '1', '0', '5', '0', '0', '0', '72', '40', '9');
INSERT INTO `ability_infos` VALUES ('a7713e0c-dd9f-11e0-95b8-60eb69729418', '9237', '1', '1', '0', '100', '2000', '0', '0', '139', '30', '20');
INSERT INTO `ability_infos` VALUES ('a7713f5e-dd9f-11e0-95b8-60eb69729418', '9237', '2', '1', '0', '100', '2000', '0', '0', '147', '30', '20');
INSERT INTO `ability_infos` VALUES ('a77140a9-dd9f-11e0-95b8-60eb69729418', '1665', '1', '1', '5', '65', '1000', '0', '0', '33', '20', '5');
INSERT INTO `ability_infos` VALUES ('a77141f5-dd9f-11e0-95b8-60eb69729418', '1664', '1', '1', '0', '5', '0', '0', '0', '67', '35', '5');
INSERT INTO `ability_infos` VALUES ('a77143ab-dd9f-11e0-95b8-60eb69729418', '9315', '1', '1', '0', '5', '0', '0', '0', '41', '30', '21');
INSERT INTO `ability_infos` VALUES ('a771452c-dd9f-11e0-95b8-60eb69729418', '9314', '1', '1', '10', '65', '1000', '0', '0', '24', '20', '21');
INSERT INTO `ability_infos` VALUES ('a78f5759-dd9f-11e0-95b8-60eb69729418', '1743', '1', '1', '5', '65', '1000', '0', '0', '39', '20', '6');
INSERT INTO `ability_infos` VALUES ('a78f5bfa-dd9f-11e0-95b8-60eb69729418', '1745', '1', '1', '0', '5', '0', '0', '0', '119', '35', '6');
INSERT INTO `ability_infos` VALUES ('a78f5e85-dd9f-11e0-95b8-60eb69729418', '8314', '1', '1', '5', '65', '1000', '0', '0', '32', '20', '13');
INSERT INTO `ability_infos` VALUES ('a78f60f9-dd9f-11e0-95b8-60eb69729418', '8315', '1', '1', '0', '5', '0', '0', '0', '38', '30', '13');
INSERT INTO `ability_infos` VALUES ('a78f6372-dd9f-11e0-95b8-60eb69729418', '9552', '1', '1', '0', '5', '0', '0', '0', '48', '30', '23');
INSERT INTO `ability_infos` VALUES ('a78f65da-dd9f-11e0-95b8-60eb69729418', '9568', '1', '1', '0', '65', '1000', '0', '0', '38', '15', '23');
INSERT INTO `ability_infos` VALUES ('a78f6848-dd9f-11e0-95b8-60eb69729418', '1509', '1', '1', '0', '5', '0', '0', '0', '60', '35', '4');
INSERT INTO `ability_infos` VALUES ('a78f6aaf-dd9f-11e0-95b8-60eb69729418', '1508', '1', '1', '5', '100', '2000', '0', '0', '179', '40', '4');
INSERT INTO `ability_infos` VALUES ('a78f6d1d-dd9f-11e0-95b8-60eb69729418', '1352', '1', '1', '5', '65', '1000', '0', '0', '39', '20', '1');
INSERT INTO `ability_infos` VALUES ('a78f6f91-dd9f-11e0-95b8-60eb69729418', '1355', '1', '1', '0', '5', '0', '0', '0', '37', '30', '1');
INSERT INTO `ability_infos` VALUES ('a78f71ed-dd9f-11e0-95b8-60eb69729418', '8002', '1', '1', '0', '5', '0', '0', '0', '212', '30', '10');
INSERT INTO `ability_infos` VALUES ('a78f7623-dd9f-11e0-95b8-60eb69729418', '8003', '1', '1', '5', '65', '1000', '0', '0', '35', '20', '10');
INSERT INTO `ability_infos` VALUES ('a78f7896-dd9f-11e0-95b8-60eb69729418', '8470', '1', '1', '0', '100', '2000', '0', '0', '179', '35', '16');
INSERT INTO `ability_infos` VALUES ('a78f7afe-dd9f-11e0-95b8-60eb69729418', '8484', '1', '1', '0', '5', '0', '0', '0', '135', '35', '16');
INSERT INTO `ability_infos` VALUES ('a78f7d66-dd9f-11e0-95b8-60eb69729418', '8393', '1', '1', '5', '65', '1000', '0', '0', '32', '20', '14');
INSERT INTO `ability_infos` VALUES ('a78f7fc8-dd9f-11e0-95b8-60eb69729418', '9392', '1', '1', '0', '5', '0', '0', '0', '43', '30', null);
INSERT INTO `ability_infos` VALUES ('a78f823b-dd9f-11e0-95b8-60eb69729418', '1586', '1', '1', '0', '100', '2000', '0', '0', '74', '25', '3');
INSERT INTO `ability_infos` VALUES ('a78f84a9-dd9f-11e0-95b8-60eb69729418', '9082', '1', '1', '5', '100', '2000', '0', '0', '158', '40', '18');
INSERT INTO `ability_infos` VALUES ('a78f870b-dd9f-11e0-95b8-60eb69729418', '9083', '1', '1', '0', '5', '0', '0', '0', '75', '35', '18');
INSERT INTO `ability_infos` VALUES ('a78f8979-dd9f-11e0-95b8-60eb69729418', '1899', '1', '1', '0', '100', '2000', '0', '0', '53', '30', '7');
INSERT INTO `ability_infos` VALUES ('a78f8be1-dd9f-11e0-95b8-60eb69729418', '1430', '1', '1', '5', '65', '1000', '0', '0', '33', '20', '2');
INSERT INTO `ability_infos` VALUES ('a78f8e5a-dd9f-11e0-95b8-60eb69729418', '1433', '1', '1', '0', '5', '0', '0', '0', '73', '35', '2');
INSERT INTO `ability_infos` VALUES ('a78f90ce-dd9f-11e0-95b8-60eb69729418', '9470', '1', '1', '0', '110', '3000', '0', '0', '249', '30', null);
INSERT INTO `ability_infos` VALUES ('a78f9330-dd9f-11e0-95b8-60eb69729418', '1820', '1', '1', '0', '5', '0', '0', '0', '69', '35', '8');
INSERT INTO `ability_infos` VALUES ('a78f95a9-dd9f-11e0-95b8-60eb69729418', '1821', '1', '1', '5', '100', '2000', '0', '0', '158', '40', '8');
INSERT INTO `ability_infos` VALUES ('a78f9811-dd9f-11e0-95b8-60eb69729418', '9003', '1', '1', '5', '65', '1000', '0', '0', '32', '20', '17');
INSERT INTO `ability_infos` VALUES ('a78f9a85-dd9f-11e0-95b8-60eb69729418', '9010', '1', '1', '0', '5', '0', '0', '0', '78', '35', '17');
INSERT INTO `ability_infos` VALUES ('a78f9d0a-dd9f-11e0-95b8-60eb69729418', '8236', '1', '1', '0', '65', '1000', '0', '0', '56', '15', '12');
INSERT INTO `ability_infos` VALUES ('a78f9f78-dd9f-11e0-95b8-60eb69729418', '8240', '1', '1', '0', '5', '0', '0', '0', '66', '30', '12');
INSERT INTO `ability_infos` VALUES ('a78fa1ec-dd9f-11e0-95b8-60eb69729418', '9158', '1', '1', '5', '65', '1000', '0', '0', '32', '20', '19');
INSERT INTO `ability_infos` VALUES ('a78fa453-dd9f-11e0-95b8-60eb69729418', '9160', '1', '1', '0', '5', '0', '0', '0', '41', '30', '19');
INSERT INTO `ability_infos` VALUES ('a78fa6e5-dd9f-11e0-95b8-60eb69729418', '9394', '1', '1', '5', '65', '1000', '0', '0', '28', '20', '22');
INSERT INTO `ability_infos` VALUES ('a78fa958-dd9f-11e0-95b8-60eb69729418', '9398', '1', '1', '0', '5', '0', '0', '0', '191', '40', '22');
INSERT INTO `ability_infos` VALUES ('a78fabcc-dd9f-11e0-95b8-60eb69729418', '8548', '1', '1', '0', '100', '2000', '0', '0', '111', '25', '15');
