/*
Navicat MySQL Data Transfer

Source Server         : Local
Source Server Version : 50515
Source Host           : localhost:3306
Source Database       : war-world

Target Server Type    : MYSQL
Target Server Version : 50515
File Encoding         : 65001

Date: 2012-04-05 12:10:16
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `zone_infos`
-- ----------------------------
DROP TABLE IF EXISTS `zone_infos`;
CREATE TABLE `zone_infos` (
  `ZoneId` smallint(5) unsigned NOT NULL,
  `Name` varchar(255) NOT NULL,
  `MinLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `MaxLevel` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `Type` int(11) NOT NULL,
  `Tier` int(11) NOT NULL,
  `Price` smallint(5) unsigned NOT NULL,
  `Region` smallint(5) unsigned NOT NULL,
  `OffX` int(11) NOT NULL,
  `OffY` int(11) NOT NULL,
  `Zone_Infos_ID` varchar(255) NOT NULL,
  UNIQUE KEY `UNIQUE` (`ZoneId`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of zone_infos
-- ----------------------------
INSERT INTO `zone_infos` VALUES ('1', 'Marshes of Madness', '11', '20', '0', '2', '500', '12', '0', '0', '94133598-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('2', 'The Badlands', '21', '30', '0', '3', '0', '10', '0', '0', '94133abb-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('3', 'Black Crag', '21', '30', '0', '4', '0', '2', '0', '0', '94133b96-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('4', 'Butcher\'s Pass', '31', '40', '0', '4', '0', '2', '0', '0', '94133c4d-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('5', 'Thunder Mountain', '31', '40', '0', '4', '0', '2', '334', '218', '94133d05-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('6', 'Ekrund', '1', '10', '0', '1', '50', '1', '180', '206', '94133dbc-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('7', 'Barak Varr', '11', '20', '0', '2', '500', '12', '0', '0', '94133e6e-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('8', 'Black Fire Pass', '21', '30', '0', '3', '2500', '10', '0', '0', '94133f2b-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('9', 'Kadrin Valley', '31', '40', '0', '4', '0', '2', '0', '0', '94133fe3-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('10', 'Stonewatch', '31', '40', '0', '4', '0', '2', '334', '186', '94134095-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('11', 'Mount Bloodhorn', '1', '10', '0', '1', '50', '1', '196', '206', '9413414c-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('26', 'Cinderfall', '31', '40', '0', '4', '0', '2', '0', '0', '94134221-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('27', 'Death Peak', '31', '40', '0', '4', '0', '2', '0', '0', '941342b5-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('30', 'Gates of Ekrund', '1', '10', '0', '1', '0', '30', '7', '7', '9413433e-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('31', 'Mourkain Temple', '11', '20', '0', '2', '0', '31', '9', '7', '941343fb-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('33', 'Doomfist Crater', '21', '30', '0', '3', '0', '33', '7', '8', '9413470f-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('34', 'Thunder Valley', '31', '40', '0', '4', '0', '34', '8', '135', '941347ea-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('36', 'Kadrin Valley Pass', '31', '40', '0', '4', '0', '36', '0', '0', '941348ad-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('37', 'Black Crag Keep', '31', '40', '0', '4', '0', '37', '0', '0', '9413493b-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('38', 'Black Fire Basin', '21', '30', '0', '3', '0', '38', '8', '6', '94134ad4-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('39', 'Logrin\'s Forge', '31', '40', '0', '4', '0', '39', '5', '7', '94134b8b-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('43', 'Gromril Crossing', '31', '40', '0', '4', '0', '43', '5', '5', '94134c43-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('44', 'Howling Gorge', '31', '40', '0', '4', '0', '44', '6', '7', '94134cfb-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('60', 'Mount Gunbad', '22', '33', '0', '3', '0', '60', '0', '0', '94134db2-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('100', 'Norsca', '1', '10', '0', '1', '50', '8', '200', '200', '94134e6a-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('101', 'Troll Country', '11', '20', '0', '2', '500', '14', '248', '200', '94134f21-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('102', 'High Pass', '21', '30', '0', '3', '0', '6', '296', '200', '94134fd9-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('103', 'Chaos Wastes', '31', '40', '0', '4', '0', '11', '344', '184', '94135090-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('104', 'The Maw', '31', '40', '0', '4', '0', '11', '344', '0', '94135148-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('105', 'Praag', '31', '40', '0', '4', '0', '11', '344', '200', '941351ff-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('106', 'Nordland', '1', '10', '0', '1', '50', '8', '200', '216', '941352b7-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('107', 'Ostland', '11', '20', '0', '2', '500', '14', '248', '216', '9413536f-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('108', 'Talabecland', '21', '30', '0', '3', '2500', '6', '296', '216', '94135432-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('109', 'Reikland', '31', '40', '0', '4', '0', '11', '344', '216', '941354ef-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('110', 'Reikwald', '31', '40', '0', '4', '0', '11', '344', '232', '941355ad-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('117', 'Outer Dark', '31', '40', '0', '4', '0', '11', '0', '0', '94135665-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('118', 'The Kraken Sea', '0', '0', '0', '0', '0', '11', '0', '0', '9413571c-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('120', 'West Praag', '31', '40', '0', '4', '0', '11', '328', '200', '941357d4-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('130', 'Nordenwatch', '1', '10', '0', '1', '0', '130', '7', '7', '941358a3-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('131', 'Stonetroll Crossing', '11', '20', '0', '2', '0', '131', '6', '7', '9413595a-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('132', 'Talabec Dam', '21', '30', '0', '3', '50', '132', '6', '7', '94135a18-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('133', 'Maw of Madness', '21', '30', '0', '3', '0', '133', '7', '8', '94135ad0-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('134', 'Reikland Hills', '21', '30', '0', '3', '0', '134', '7', '8', '94135b58-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('135', 'Twisting Tower', '0', '0', '0', '4', '0', '135', '0', '0', '94135be0-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('136', 'Battle for Praag', '31', '40', '0', '4', '0', '136', '8', '8', '94135c68-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('137', 'Grovod Caverns', '31', '40', '0', '4', '0', '137', '11', '8', '94135cea-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('138', 'Castle Reikland', '31', '40', '0', '4', '0', '138', '7', '8', '94135d73-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('139', 'High Pass Cemetery', '21', '30', '0', '3', '2500', '139', '7', '8', '94135e12-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('144', 'Outer Dark', '31', '40', '0', '4', '0', '11', '0', '0', '94135e89-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('152', 'The Sewers of Altdorf: Wing 1', '31', '40', '0', '4', '0', '152', '0', '0', '94135ef9-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('153', 'The Sewers of Altdorf: Wing 2', '31', '40', '0', '4', '0', '153', '0', '0', '94135f70-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('155', 'Sacellum Dungeons: West Wing', '12', '18', '0', '4', '0', '155', '0', '0', '94135fe6-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('156', 'Sacellum Dungeons: East Wing', '12', '18', '0', '4', '0', '156', '0', '0', '9413605d-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('160', 'Bastion Stair', '34', '40', '0', '3', '0', '160', '0', '0', '941360cd-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('161', 'The Inevitable City', '31', '40', '0', '4', '2500', '17', '102', '24', '94136144-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('162', 'Altdorf', '31', '40', '0', '4', '0', '7', '25', '24', '941361ba-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('169', 'The Sewers of Altdorf: Wing 3', '31', '40', '0', '4', '0', '169', '0', '0', '9413622b-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('171', 'The Screaming Cat', '31', '40', '0', '4', '0', '171', '0', '0', '941362a1-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('173', 'Sacellum Dungeons: South Wing', '12', '18', '0', '4', '0', '173', '0', '0', '94136317-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('176', 'Sigmar Crypts', '40', '40', '0', '4', '0', '176', '0', '0', '94136388-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('177', 'Warpblade Tunnels', '40', '40', '0', '4', '0', '177', '0', '0', '941363fe-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('179', 'Tomb of the Vulture Lord', '31', '40', '0', '4', '0', '179', '0', '0', '9413646f-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('191', 'Necropolis of Zandri', '31', '40', '0', '4', '0', '9', '48', '363', '941364e5-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('195', 'Bloodwrought Enclave', '40', '10', '0', '3', '0', '195', '0', '0', '9413655c-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('196', 'Bilerot Burrow', '40', '20', '0', '3', '0', '196', '0', '0', '941365cc-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('200', 'The Blighted Isle', '1', '30', '0', '1', '50', '3', '248', '264', '94136643-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('201', 'The Shadowlands', '11', '40', '0', '2', '500', '15', '200', '296', '941366b9-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('202', 'Avelorn', '21', '40', '0', '3', '2500', '16', '344', '344', '9413672a-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('203', 'Caledor', '31', '40', '0', '4', '0', '4', '216', '392', '941367a0-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('204', 'Fell Landing', '31', '10', '0', '4', '2500', '4', '200', '392', '94136816-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('205', 'Dragonwake', '31', '20', '0', '4', '0', '4', '232', '392', '94136887-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('206', 'Chrace', '1', '30', '0', '1', '50', '3', '0', '0', '941368fd-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('207', 'Ellyrion', '11', '40', '0', '2', '500', '15', '200', '312', '94136974-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('208', 'Saphery', '21', '40', '0', '3', '25000', '16', '344', '360', '941369ea-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('209', 'Eataine', '31', '10', '0', '4', '0', '4', '248', '392', '94136a5b-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('210', 'Shining Way', '31', '0', '0', '4', '0', '4', '264', '392', '94136ad1-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('220', 'Isle of the Dead', '1', '0', '0', '1', '0', '4', '232', '360', '94136b48-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('230', 'Khaine\'s Embrace', '0', '0', '0', '0', '0', '230', '9', '9', '94136bb8-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('231', 'Phoenix Gate', '0', '0', '0', '0', '0', '231', '6', '7', '94136c2f-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('232', 'Tor Anroc', '0', '0', '0', '0', '0', '232', '6', '8', '94136ca5-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('234', 'Serpent\'s Passage', '0', '0', '0', '0', '0', '234', '9', '8', '94136d1b-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('235', 'Dragon\'s Bane', '0', '0', '0', '0', '0', '235', '7', '8', '94136d8c-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('236', 'Temple of Isha', '0', '0', '0', '0', '0', '236', '7', '8', '94136e02-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('237', 'Caledor Woods', '0', '0', '0', '0', '0', '237', '5', '7', '94136e79-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('242', 'Tomb of the Moon', '0', '0', '0', '0', '0', '242', '0', '0', '94136ee9-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('243', 'Tomb of the Sky', '0', '0', '0', '0', '0', '243', '0', '0', '94136f60-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('244', 'Tomb of the Sun', '0', '0', '0', '0', '0', '244', '0', '0', '94136fd6-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('260', 'The Lost Vale', '40', '40', '0', '3', '0', '260', '0', '0', '94137047-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('238', 'Blood of the Black Cairn', '0', '0', '0', '0', '0', '238', '5', '7', '941370c3-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('241', 'Tomb of the Stars', '0', '0', '0', '0', '0', '241', '0', '0', '94137139-bac4-11e0-84b4-60eb69729418');
INSERT INTO `zone_infos` VALUES ('168', 'Altdorf (contested)', '31', '40', '0', '4', '2500', '168', '3', '181', '941371aa-bac4-11e0-84b4-60eb69729418');
