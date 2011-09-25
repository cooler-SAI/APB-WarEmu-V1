/*
Navicat MySQL Data Transfer

Source Server         : Local
Source Server Version : 50148
Source Host           : localhost:3306
Source Database       : magdar_characters

Target Server Type    : MYSQL
Target Server Version : 50148
File Encoding         : 65001

Date: 2011-01-15 01:29:33
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `characters`
-- ----------------------------
DROP TABLE IF EXISTS `characters`;
CREATE TABLE `characters` (
  `character_id` int(11) NOT NULL AUTO_INCREMENT,
  `account` int(11) NOT NULL DEFAULT '0',
  `slot` int(11) NOT NULL DEFAULT '0',
  `name` varchar(25) NOT NULL DEFAULT '',
  `model` int(11) NOT NULL DEFAULT '0',
  `level` int(11) DEFAULT '1',
  `career` int(11) NOT NULL DEFAULT '0',
  `careerLine` int(11) NOT NULL DEFAULT '0',
  `realm` int(11) NOT NULL DEFAULT '0',
  `heldleft` int(11) DEFAULT '0',
  `race` int(11) NOT NULL DEFAULT '0',
  `last_time_played` timestamp NOT NULL DEFAULT '0000-00-00 00:00:00',
  `traits_0` int(11) DEFAULT '0',
  `traits_1` int(11) DEFAULT '0',
  `traits_2` int(11) DEFAULT '0',
  `traits_3` int(11) DEFAULT '0',
  `traits_4` int(11) DEFAULT '0',
  `traits_5` int(11) DEFAULT NULL,
  `traits_6` int(11) DEFAULT NULL,
  `traits_7` int(11) DEFAULT NULL,
  `wealth` int(11) DEFAULT '0',
  `speed` int(11) DEFAULT '100',
  `zone` int(11) NOT NULL DEFAULT '0',
  `region` int(11) NOT NULL DEFAULT '0',
  `posX` int(11) DEFAULT NULL,
  `posY` int(11) DEFAULT NULL,
  `posZ` int(11) DEFAULT NULL,
  `posHeading` int(11) DEFAULT NULL,
  `XP` int(11) DEFAULT '0',
  `restXP` int(11) DEFAULT '0',
  `renown` int(11) DEFAULT '0',
  `renownRank` int(11) DEFAULT '0',
  `rallyPoint` int(11) DEFAULT NULL,
  `playedTime` int(11) DEFAULT '0',
  `health` int(11) DEFAULT NULL,
  `maxHealth` int(11) DEFAULT NULL,
  `actionPoints` int(11) DEFAULT '0',
  `maxActionPoints` int(11) DEFAULT NULL,
  `sex` int(11) DEFAULT NULL,
  `skills` int(11) DEFAULT '0',
  `online` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`character_id`),
  UNIQUE KEY `name` (`name`) USING BTREE,
  UNIQUE KEY `account&slot` (`account`,`slot`) USING BTREE
) ENGINE=MyISAM AUTO_INCREMENT=2178 DEFAULT CHARSET=latin1 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Records of characters
-- ----------------------------

-- ----------------------------
-- Table structure for `characters_friends`
-- ----------------------------
DROP TABLE IF EXISTS `characters_friends`;
CREATE TABLE `characters_friends` (
  `character_id` int(30) NOT NULL,
  `friend_id` int(30) NOT NULL,
  `note` varchar(100) NOT NULL DEFAULT '',
  PRIMARY KEY (`character_id`,`friend_id`),
  KEY `a` (`character_id`),
  KEY `b` (`friend_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of characters_friends
-- ----------------------------

-- ----------------------------
-- Table structure for `characters_influence`
-- ----------------------------
DROP TABLE IF EXISTS `characters_influence`;
CREATE TABLE `characters_influence` (
  `character_id` int(30) NOT NULL,
  `influenceid` int(30) NOT NULL DEFAULT '0',
  `influence` int(30) NOT NULL DEFAULT '0',
  `rewarded_0` int(30) NOT NULL DEFAULT '0',
  `rewarded_1` int(30) NOT NULL DEFAULT '0',
  `rewarded_2` int(30) NOT NULL DEFAULT '0',
  PRIMARY KEY (`character_id`,`influenceid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of characters_influence
-- ----------------------------

-- ----------------------------
-- Table structure for `characters_items`
-- ----------------------------
DROP TABLE IF EXISTS `characters_items`;
CREATE TABLE `characters_items` (
  `guid` int(11) NOT NULL AUTO_INCREMENT,
  `character_id` int(11) NOT NULL,
  `entry` int(11) NOT NULL,
  `slot_id` int(11) NOT NULL DEFAULT '0',
  `counts` int(11) NOT NULL DEFAULT '1',
  PRIMARY KEY (`guid`)
) ENGINE=MyISAM AUTO_INCREMENT=51765 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of characters_items
-- ----------------------------

-- ----------------------------
-- Table structure for `characters_mails`
-- ----------------------------
DROP TABLE IF EXISTS `characters_mails`;
CREATE TABLE `characters_mails` (
  `guid` int(11) NOT NULL AUTO_INCREMENT,
  `receiver` int(11) NOT NULL DEFAULT '0',
  `sender` int(11) NOT NULL DEFAULT '0',
  `receivername` varchar(255) NOT NULL DEFAULT '',
  `sendername` varchar(255) NOT NULL DEFAULT '',
  `title` varchar(255) NOT NULL DEFAULT '',
  `content` varchar(4096) NOT NULL DEFAULT '',
  `money` int(30) NOT NULL DEFAULT '0',
  `cr` int(30) NOT NULL,
  `opened` int(11) NOT NULL DEFAULT '0',
  `items` varchar(4096) DEFAULT '',
  PRIMARY KEY (`guid`)
) ENGINE=MyISAM AUTO_INCREMENT=4064121 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of characters_mails
-- ----------------------------

-- ----------------------------
-- Table structure for `characters_quest`
-- ----------------------------
DROP TABLE IF EXISTS `characters_quest`;
CREATE TABLE `characters_quest` (
  `character_id` int(11) NOT NULL,
  `questid` int(11) NOT NULL,
  `objectif` text,
  `done` tinyint(1) NOT NULL DEFAULT '0',
  PRIMARY KEY (`character_id`,`questid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of characters_quest
-- ----------------------------

-- ----------------------------
-- Table structure for `characters_spells`
-- ----------------------------
DROP TABLE IF EXISTS `characters_spells`;
CREATE TABLE `characters_spells` (
  `character_id` int(11) NOT NULL DEFAULT '0',
  `spellid` int(11) NOT NULL DEFAULT '0',
  `level` int(30) NOT NULL DEFAULT '1',
  PRIMARY KEY (`character_id`,`spellid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of characters_spells
-- ----------------------------

-- ----------------------------
-- Table structure for `characters_stats`
-- ----------------------------
DROP TABLE IF EXISTS `characters_stats`;
CREATE TABLE `characters_stats` (
  `character_id` int(11) NOT NULL,
  `type_id` int(11) NOT NULL DEFAULT '0',
  `type_lv` float(11,2) DEFAULT NULL,
  PRIMARY KEY (`character_id`,`type_id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of characters_stats
-- ----------------------------

-- ----------------------------
-- Table structure for `characters_tok`
-- ----------------------------
DROP TABLE IF EXISTS `characters_tok`;
CREATE TABLE `characters_tok` (
  `character_id` int(30) NOT NULL,
  `toktype` int(30) NOT NULL DEFAULT '1',
  `tokid` int(30) NOT NULL DEFAULT '0',
  `checked` int(30) NOT NULL DEFAULT '0',
  PRIMARY KEY (`character_id`,`toktype`,`tokid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of characters_tok
-- ----------------------------

-- ----------------------------
-- Table structure for `characters_zonesacces`
-- ----------------------------
DROP TABLE IF EXISTS `characters_zonesacces`;
CREATE TABLE `characters_zonesacces` (
  `character_id` int(11) NOT NULL,
  `zoneid` int(11) NOT NULL DEFAULT '0',
  PRIMARY KEY (`character_id`,`zoneid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of characters_zonesacces
-- ----------------------------

-- ----------------------------
-- Table structure for `item_insert_queue`
-- ----------------------------
DROP TABLE IF EXISTS `item_insert_queue`;
CREATE TABLE `item_insert_queue` (
  `guid` int(8) unsigned NOT NULL AUTO_INCREMENT,
  `character_id` int(8) NOT NULL,
  `type` tinyint(2) NOT NULL,
  `item_or_spellid` int(8) NOT NULL,
  PRIMARY KEY (`guid`),
  KEY `idx_main` (`guid`,`character_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of item_insert_queue
-- ----------------------------
