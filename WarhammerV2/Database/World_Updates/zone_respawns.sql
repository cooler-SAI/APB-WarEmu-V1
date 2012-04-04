/*
Navicat MySQL Data Transfer

Source Server         : Local
Source Server Version : 50515
Source Host           : localhost:3306
Source Database       : war-world

Target Server Type    : MYSQL
Target Server Version : 50515
File Encoding         : 65001

Date: 2012-04-04 10:44:05
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `zone_respawns`
-- ----------------------------
DROP TABLE IF EXISTS `zone_respawns`;
CREATE TABLE `zone_respawns` (
  `Zone_Respawns_ID` text,
  `RespawnID` int(11) NOT NULL AUTO_INCREMENT,
  `ZoneID` int(11) NOT NULL,
  `Realm` tinyint(3) unsigned NOT NULL,
  `PinX` smallint(5) unsigned NOT NULL,
  `PinY` smallint(5) unsigned NOT NULL,
  `PinZ` smallint(5) unsigned NOT NULL,
  `WorldO` smallint(5) unsigned NOT NULL,
  PRIMARY KEY (`RespawnID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of zone_respawns
-- ----------------------------
INSERT INTO `zone_respawns` VALUES ('591904e9-c935-4965-b182-e9879574a107', '1', '106', '1', '22535', '27200', '10384', '2092');
