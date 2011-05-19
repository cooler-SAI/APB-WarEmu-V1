/*
Navicat MySQL Data Transfer

Source Server         : Local
Source Server Version : 50148
Source Host           : localhost:3306
Source Database       : war_accounts

Target Server Type    : MYSQL
Target Server Version : 50148
File Encoding         : 65001

Date: 2011-05-13 15:13:26
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `accounts`
-- ----------------------------
DROP TABLE IF EXISTS `accounts`;
CREATE TABLE `accounts` (
  `Accounts_ID` text,
  `AccountId` int(11) NOT NULL AUTO_INCREMENT,
  `Username` varchar(255) DEFAULT NULL,
  `Password` varchar(255) DEFAULT NULL,
  `Ip` varchar(255) DEFAULT NULL,
  `Token` varchar(255) DEFAULT NULL,
  `GmLevel` tinyint(3) unsigned NOT NULL,
  PRIMARY KEY (`AccountId`),
  UNIQUE KEY `Username` (`Username`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of accounts
-- ----------------------------


-- ----------------------------
-- Table structure for `realms`
-- ----------------------------
DROP TABLE IF EXISTS `realms`;
CREATE TABLE `realms` (
  `Realms_ID` varchar(255) NOT NULL,
  `RealmId` tinyint(3) unsigned DEFAULT NULL,
  `Name` varchar(255) DEFAULT NULL,
  `Language` varchar(255) DEFAULT NULL,
  `Adresse` varchar(255) DEFAULT NULL,
  `Port` int(11) NOT NULL,
  `AllowTrials` varchar(255) DEFAULT NULL,
  `CharfxerAvailable` varchar(255) DEFAULT NULL,
  `Legacy` varchar(255) DEFAULT NULL,
  `BonusDestruction` varchar(255) DEFAULT NULL,
  `BonusOrder` varchar(255) DEFAULT NULL,
  `Redirect` varchar(255) DEFAULT NULL,
  `Region` varchar(255) DEFAULT NULL,
  `Retired` varchar(255) DEFAULT NULL,
  `WaitingDestruction` varchar(255) DEFAULT NULL,
  `WaitingOrder` varchar(255) DEFAULT NULL,
  `DensityDestruction` varchar(255) DEFAULT NULL,
  `DensityOrder` varchar(255) DEFAULT NULL,
  `OpenRvr` varchar(255) DEFAULT NULL,
  `Rp` varchar(255) DEFAULT NULL,
  `Status` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`Realms_ID`),
  UNIQUE KEY `RealmId` (`RealmId`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of realms
-- ----------------------------
INSERT INTO `realms` VALUES ('1', '1', 'Aps', 'FR', '127.0.0.1', '10622', '1', '0', '0', '0', '0', '0', 'STR_REGION_EUROPE', '0', '0', '0', '0', '0', '0', '0', '0');
