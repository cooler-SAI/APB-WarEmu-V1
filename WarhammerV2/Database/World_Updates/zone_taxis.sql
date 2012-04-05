/*
Navicat MySQL Data Transfer

Source Server         : Local
Source Server Version : 50515
Source Host           : localhost:3306
Source Database       : war-world

Target Server Type    : MYSQL
Target Server Version : 50515
File Encoding         : 65001

Date: 2012-04-05 12:10:08
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `zone_taxis`
-- ----------------------------
DROP TABLE IF EXISTS `zone_taxis`;
CREATE TABLE `zone_taxis` (
  `ZoneID` smallint(5) unsigned NOT NULL,
  `RealmID` tinyint(3) unsigned NOT NULL,
  `WorldX` int(10) unsigned NOT NULL,
  `WorldY` int(10) unsigned NOT NULL,
  `WorldZ` smallint(5) unsigned NOT NULL,
  `WorldO` smallint(5) unsigned NOT NULL,
  `Zone_Taxis_ID` varchar(255) NOT NULL,
  UNIQUE KEY `UNIQUE` (`ZoneID`,`RealmID`) USING BTREE
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of zone_taxis
-- ----------------------------
INSERT INTO `zone_taxis` VALUES ('1', '1', '1071428', '901441', '4680', '3868', '0c08daea-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('2', '1', '0', '0', '0', '0', '0c08e01e-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('3', '1', '0', '0', '0', '0', '0c08e0e1-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('4', '1', '0', '0', '0', '0', '0c08e18d-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('5', '1', '0', '0', '0', '0', '0c08e22d-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('6', '1', '795068', '862049', '9358', '750', '0c08e2cd-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('7', '1', '1057100', '838069', '5408', '500', '0c08e367-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('8', '1', '1273886', '849243', '7762', '3549', '0c08e407-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('9', '1', '0', '0', '0', '0', '0c08e4a1-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('10', '1', '0', '0', '0', '0', '0c08e540-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('11', '1', '846848', '860922', '7408', '4050', '0c08e5e0-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('26', '1', '0', '0', '0', '0', '0c08e67a-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('27', '1', '0', '0', '0', '0', '0c08e71a-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('30', '1', '0', '0', '0', '0', '0c08e7ba-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('31', '1', '0', '0', '0', '0', '0c08e85a-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('33', '1', '0', '0', '0', '0', '0c08e8f4-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('34', '1', '0', '0', '0', '0', '0c08e994-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('36', '1', '0', '0', '0', '0', '0c08ea34-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('37', '1', '0', '0', '0', '0', '0c08ead4-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('38', '1', '0', '0', '0', '0', '0c08eb73-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('39', '1', '0', '0', '0', '0', '0c08ec0d-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('43', '1', '0', '0', '0', '0', '0c08ecad-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('44', '1', '0', '0', '0', '0', '0c08ed4d-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('60', '1', '0', '0', '0', '0', '0c08eded-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('100', '1', '0', '0', '0', '0', '0c08ee8d-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('101', '1', '1056567', '880077', '4136', '2776', '0c08ef2d-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('102', '1', '0', '0', '0', '0', '0c08efc7-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('103', '1', '0', '0', '0', '0', '0c08f03d-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('104', '1', '0', '0', '0', '0', '0c08f0ae-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('105', '1', '0', '0', '0', '0', '0c08f124-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('106', '1', '846209', '904145', '5247', '580', '0c08f195-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('107', '1', '1045899', '910100', '4728', '1786', '0c08f20b-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('108', '1', '1257526', '927650', '9131', '136', '0c08f27c-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('109', '1', '0', '0', '0', '0', '0c08f2ec-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('110', '1', '0', '0', '0', '0', '0c08f362-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('117', '1', '0', '0', '0', '0', '0c08f42c-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('118', '1', '0', '0', '0', '0', '0c08f4cc-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('120', '1', '0', '0', '0', '0', '0c08f623-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('130', '1', '0', '0', '0', '0', '0c08f6fe-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('131', '1', '0', '0', '0', '0', '0c08f79e-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('132', '1', '125258', '129373', '13275', '1763', '0c08f83e-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('133', '1', '0', '0', '0', '0', '0c08f8de-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('134', '1', '0', '0', '0', '0', '0c08f978-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('135', '1', '0', '0', '0', '0', '0c08fa18-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('136', '1', '0', '0', '0', '0', '0c08fb1c-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('137', '1', '0', '0', '0', '0', '0c08fbc2-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('138', '1', '0', '0', '0', '0', '0c08fc5c-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('139', '1', '1247090', '875283', '14147', '2525', '0c08fcfc-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('144', '1', '0', '0', '0', '0', '0c08fd9c-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('152', '1', '0', '0', '0', '0', '0c08fe36-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('153', '1', '0', '0', '0', '0', '0c08fed6-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('155', '1', '0', '0', '0', '0', '0c08ff70-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('156', '1', '0', '0', '0', '0', '0c090009-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('160', '1', '0', '0', '0', '0', '0c0900a3-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('161', '2', '0', '0', '0', '0', '0c0902ca-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('162', '1', '125258', '129373', '13275', '1763', '0c090393-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('169', '1', '0', '0', '0', '0', '0c090433-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('171', '1', '0', '0', '0', '0', '0c0904d3-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('173', '1', '0', '0', '0', '0', '0c09056d-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('176', '1', '0', '0', '0', '0', '0c090619-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('177', '1', '0', '0', '0', '0', '0c0906b9-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('179', '1', '0', '0', '0', '0', '0c090759-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('191', '1', '0', '0', '0', '0', '0c0907f3-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('195', '1', '0', '0', '0', '0', '0c090892-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('196', '1', '0', '0', '0', '0', '0c090932-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('200', '1', '1061219', '1065844', '4875', '1706', '0c0909cc-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('201', '1', '866188', '1254473', '6472', '3561', '0c090a6c-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('202', '1', '1450553', '1463338', '5024', '1786', '0c090b06-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('203', '1', '0', '0', '0', '0', '0c090ba6-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('204', '1', '1254816', '928062', '5720', '932', '0c090c40-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('205', '1', '0', '0', '0', '0', '0c090cf8-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('206', '1', '1068875', '114672', '7776', '1228', '0c090d6e-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('207', '1', '863975', '1320079', '7622', '1467', '0c090de4-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('208', '1', '1464407', '1519509', '9264', '2742', '0c090e55-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('209', '1', '0', '0', '0', '0', '0c090ecb-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('210', '1', '0', '0', '0', '0', '0c090f3c-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('220', '1', '0', '0', '0', '0', '0c090fb2-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('230', '1', '0', '0', '0', '0', '0c091023-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('231', '1', '0', '0', '0', '0', '0c0910ec-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('232', '1', '0', '0', '0', '0', '0c091186-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('234', '1', '0', '0', '0', '0', '0c091226-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('235', '1', '0', '0', '0', '0', '0c0912c0-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('236', '1', '0', '0', '0', '0', '0c091360-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('237', '1', '0', '0', '0', '0', '0c0913fa-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('242', '1', '0', '0', '0', '0', '0c091494-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('243', '1', '0', '0', '0', '0', '0c091534-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('244', '1', '0', '0', '0', '0', '0c0915ce-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('260', '1', '0', '0', '0', '0', '0c091667-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('238', '1', '0', '0', '0', '0', '0c091707-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('241', '1', '0', '0', '0', '0', '0c091876-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('161', '1', '436798', '128899', '17495', '2423', '0c0919f1-7eb3-11e1-b389-7a79051cf7b8');
INSERT INTO `zone_taxis` VALUES ('168', '1', '125258', '129373', '13275', '1763', '0c091abb-7eb3-11e1-b389-7a79051cf7b8');
