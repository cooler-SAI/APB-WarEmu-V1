/*
Navicat MySQL Data Transfer

Source Server         : Local
Source Server Version : 50148
Source Host           : localhost:3306
Source Database       : war_characters

Target Server Type    : MYSQL
Target Server Version : 50148
File Encoding         : 65001

Date: 2011-05-13 15:13:16
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `character_socials`
-- ----------------------------
DROP TABLE IF EXISTS `character_socials`;
CREATE TABLE `character_socials` (
  `Character_socials_ID` varchar(255) NOT NULL,
  `CharacterId` int(11) NOT NULL,
  `DistCharacterId` int(11) NOT NULL,
  `DistName` varchar(255) NOT NULL,
  `Friend` tinyint(3) unsigned NOT NULL,
  `Ignore` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`Character_socials_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of character_socials
-- ----------------------------

-- ----------------------------
-- Table structure for `character_toks`
-- ----------------------------
DROP TABLE IF EXISTS `character_toks`;
CREATE TABLE `character_toks` (
  `Character_Toks_ID` varchar(255) NOT NULL,
  `CharacterId` int(11) DEFAULT NULL,
  `TokEntry` int(10) unsigned DEFAULT NULL,
  `Count` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`Character_Toks_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of character_toks
-- ----------------------------

-- ----------------------------
-- Table structure for `character_value`
-- ----------------------------
DROP TABLE IF EXISTS `character_value`;
CREATE TABLE `character_value` (
  `Character_value_ID` varchar(255) NOT NULL,
  `CharacterId` int(11) DEFAULT NULL,
  `Level` tinyint(3) unsigned NOT NULL,
  `Xp` int(10) unsigned NOT NULL,
  `RestXp` int(11) NOT NULL,
  `Renown` int(10) unsigned NOT NULL,
  `RenownRank` tinyint(3) unsigned NOT NULL,
  `Money` int(10) unsigned NOT NULL,
  `Speed` int(11) NOT NULL,
  `RegionId` int(11) NOT NULL,
  `ZoneId` smallint(5) unsigned NOT NULL,
  `WorldX` int(11) NOT NULL,
  `WorldY` int(11) NOT NULL,
  `WorldZ` int(11) NOT NULL,
  `WorldO` int(11) NOT NULL,
  `RallyPoint` smallint(5) unsigned NOT NULL,
  `BagBuy` tinyint(3) unsigned NOT NULL,
  `Skills` int(10) unsigned NOT NULL,
  `Online` tinyint(1) NOT NULL,
  PRIMARY KEY (`Character_value_ID`),
  UNIQUE KEY `CharacterId` (`CharacterId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of character_value
-- ----------------------------

-- ----------------------------
-- Table structure for `characters`
-- ----------------------------
DROP TABLE IF EXISTS `characters`;
CREATE TABLE `characters` (
  `Characters_ID` varchar(255) NOT NULL,
  `CharacterId` int(11) NOT NULL,
  `Name` varchar(255) NOT NULL,
  `RealmId` int(11) NOT NULL,
  `AccountId` int(11) NOT NULL,
  `SlotId` tinyint(3) unsigned NOT NULL,
  `ModelId` tinyint(3) unsigned NOT NULL,
  `Career` tinyint(3) unsigned NOT NULL,
  `CareerLine` tinyint(3) unsigned NOT NULL,
  `Realm` tinyint(3) unsigned NOT NULL,
  `HeldLeft` int(11) NOT NULL,
  `Race` tinyint(3) unsigned NOT NULL,
  `Traits` text NOT NULL,
  `Sex` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`Characters_ID`),
  UNIQUE KEY `Name` (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of characters
-- ----------------------------

-- ----------------------------
-- Table structure for `characters_items`
-- ----------------------------
DROP TABLE IF EXISTS `characters_items`;
CREATE TABLE `characters_items` (
  `Characters_items_ID` varchar(255) NOT NULL,
  `Guid` bigint(20) NOT NULL,
  `CharacterId` int(11) NOT NULL,
  `Entry` int(10) unsigned NOT NULL,
  `SlotId` smallint(5) unsigned NOT NULL,
  `ModelId` int(10) unsigned NOT NULL,
  `Counts` smallint(5) unsigned NOT NULL,
  PRIMARY KEY (`Characters_items_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of characters_items
-- ----------------------------
