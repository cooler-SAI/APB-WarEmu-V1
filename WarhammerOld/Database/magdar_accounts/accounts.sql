/*
Navicat MySQL Data Transfer

Source Server         : Local
Source Server Version : 50148
Source Host           : localhost:3306
Source Database       : magdar_accounts

Target Server Type    : MYSQL
Target Server Version : 50148
File Encoding         : 65001

Date: 2011-01-15 01:28:51
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `accounts`
-- ----------------------------
DROP TABLE IF EXISTS `accounts`;
CREATE TABLE `accounts` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(32) NOT NULL DEFAULT '',
  `password` varchar(64) NOT NULL,
  `password_crypt` varchar(64) NOT NULL DEFAULT '',
  `email_address` text,
  `date_joined` datetime NOT NULL DEFAULT '0000-00-00 00:00:00',
  `last_ip` varchar(15) DEFAULT '127.0.0.1',
  `failed_login_attempts` int(11) DEFAULT '0',
  `locked` int(11) DEFAULT '0',
  `last_login` datetime DEFAULT '0000-00-00 00:00:00',
  `permission_level` int(11) DEFAULT '0',
  `token` varchar(80) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=2607 DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of accounts
-- ----------------------------
