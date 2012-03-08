    -- ----------------------------
    -- Table structure for `renown_infos`
    -- ----------------------------
    DROP TABLE IF EXISTS `renown_infos`;
    CREATE TABLE `renown_infos` (
      `Level` tinyint(3) unsigned NOT NULL DEFAULT '0',
      `Renown` int(10) unsigned NOT NULL DEFAULT '0',
      `RenownInfo_ID` varchar(255) NOT NULL,
      `Renown_Infos_ID` varchar(255) NOT NULL,
      PRIMARY KEY (`Level`)
    ) ENGINE=MyISAM DEFAULT CHARSET=latin1;

    -- ----------------------------
    -- Records of renown_infos
    -- ----------------------------
REPLACE INTO `renown_infos` VALUES ('1', '10', '', '');
REPLACE INTO `renown_infos` VALUES ('2', '80', '', '');
REPLACE INTO `renown_infos` VALUES ('3', '190', '', '');
REPLACE INTO `renown_infos` VALUES ('4', '370', '', '');
REPLACE INTO `renown_infos` VALUES ('5', '610', '', '');
REPLACE INTO `renown_infos` VALUES ('6', '910', '', '');
REPLACE INTO `renown_infos` VALUES ('7', '1270', '', '');
REPLACE INTO `renown_infos` VALUES ('8', '1690', '', '');
REPLACE INTO `renown_infos` VALUES ('9', '2170', '', '');
REPLACE INTO `renown_infos` VALUES ('10', '2710', '', '');
REPLACE INTO `renown_infos` VALUES ('11', '3310', '', '');
REPLACE INTO `renown_infos` VALUES ('12', '3970', '', '');
REPLACE INTO `renown_infos` VALUES ('13', '4690', '', '');
REPLACE INTO `renown_infos` VALUES ('14', '5470', '', '');
REPLACE INTO `renown_infos` VALUES ('15', '6310', '', '');
REPLACE INTO `renown_infos` VALUES ('16', '7210', '', '');
REPLACE INTO `renown_infos` VALUES ('17', '8170', '', '');
REPLACE INTO `renown_infos` VALUES ('18', '9190', '', '');
REPLACE INTO `renown_infos` VALUES ('19', '10270', '', '');
REPLACE INTO `renown_infos` VALUES ('20', '11410', '', '');
REPLACE INTO `renown_infos` VALUES ('21', '12610', '', '');
REPLACE INTO `renown_infos` VALUES ('22', '13870', '', '');
REPLACE INTO `renown_infos` VALUES ('23', '15190', '', '');
REPLACE INTO `renown_infos` VALUES ('24', '16570', '', '');
REPLACE INTO `renown_infos` VALUES ('25', '18010', '', '');
REPLACE INTO `renown_infos` VALUES ('26', '19510', '', '');
REPLACE INTO `renown_infos` VALUES ('27', '21070', '', '');
REPLACE INTO `renown_infos` VALUES ('28', '22690', '', '');
REPLACE INTO `renown_infos` VALUES ('29', '24370', '', '');
REPLACE INTO `renown_infos` VALUES ('30', '26110', '', '');
REPLACE INTO `renown_infos` VALUES ('31', '27910', '', '');
REPLACE INTO `renown_infos` VALUES ('32', '29770', '', '');
REPLACE INTO `renown_infos` VALUES ('33', '31690', '', '');
REPLACE INTO `renown_infos` VALUES ('34', '33670', '', '');
REPLACE INTO `renown_infos` VALUES ('35', '35710', '', '');
REPLACE INTO `renown_infos` VALUES ('36', '37810', '', '');
REPLACE INTO `renown_infos` VALUES ('37', '39970', '', '');
REPLACE INTO `renown_infos` VALUES ('38', '42190', '', '');
REPLACE INTO `renown_infos` VALUES ('39', '44470', '', '');
REPLACE INTO `renown_infos` VALUES ('40', '46810', '', '');
REPLACE INTO `renown_infos` VALUES ('41', '49210', '', '');
REPLACE INTO `renown_infos` VALUES ('42', '51670', '', '');
REPLACE INTO `renown_infos` VALUES ('43', '54190', '', '');
REPLACE INTO `renown_infos` VALUES ('44', '56770', '', '');
REPLACE INTO `renown_infos` VALUES ('45', '59410', '', '');
REPLACE INTO `renown_infos` VALUES ('46', '62110', '', '');
REPLACE INTO `renown_infos` VALUES ('47', '64870', '', '');
REPLACE INTO `renown_infos` VALUES ('48', '67690', '', '');
REPLACE INTO `renown_infos` VALUES ('49', '70570', '', '');
REPLACE INTO `renown_infos` VALUES ('50', '73510', '', '');
REPLACE INTO `renown_infos` VALUES ('51', '76510', '', '');
REPLACE INTO `renown_infos` VALUES ('52', '79570', '', '');
REPLACE INTO `renown_infos` VALUES ('53', '82690', '', '');
REPLACE INTO `renown_infos` VALUES ('54', '85870', '', '');
REPLACE INTO `renown_infos` VALUES ('55', '89110', '', '');
REPLACE INTO `renown_infos` VALUES ('56', '92410', '', '');
REPLACE INTO `renown_infos` VALUES ('57', '95770', '', '');
REPLACE INTO `renown_infos` VALUES ('58', '99190', '', '');
REPLACE INTO `renown_infos` VALUES ('59', '102670', '', '');
REPLACE INTO `renown_infos` VALUES ('60', '106210', '', '');
REPLACE INTO `renown_infos` VALUES ('61', '109810', '', '');
REPLACE INTO `renown_infos` VALUES ('62', '113470', '', '');
REPLACE INTO `renown_infos` VALUES ('63', '117190', '', '');
REPLACE INTO `renown_infos` VALUES ('64', '120970', '', '');
REPLACE INTO `renown_infos` VALUES ('65', '124810', '', '');
REPLACE INTO `renown_infos` VALUES ('66', '128710', '', '');
REPLACE INTO `renown_infos` VALUES ('67', '132670', '', '');
REPLACE INTO `renown_infos` VALUES ('68', '136690', '', '');
REPLACE INTO `renown_infos` VALUES ('69', '140770', '', '');
REPLACE INTO `renown_infos` VALUES ('70', '144910', '', '');
REPLACE INTO `renown_infos` VALUES ('71', '149110', '', '');
REPLACE INTO `renown_infos` VALUES ('72', '153370', '', '');
REPLACE INTO `renown_infos` VALUES ('73', '157690', '', '');
REPLACE INTO `renown_infos` VALUES ('74', '162070', '', '');
REPLACE INTO `renown_infos` VALUES ('75', '166510', '', '');
REPLACE INTO `renown_infos` VALUES ('76', '171010', '', '');
REPLACE INTO `renown_infos` VALUES ('77', '175570', '', '');
REPLACE INTO `renown_infos` VALUES ('78', '180190', '', '');
REPLACE INTO `renown_infos` VALUES ('79', '184870', '', '');
REPLACE INTO `renown_infos` VALUES ('80', '189610', '', '');