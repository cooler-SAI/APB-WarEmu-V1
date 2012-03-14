/*
Navicat MySQL Data Transfer

Source Server         : Local
Source Server Version : 50515
Source Host           : localhost:3306
Source Database       : war-world

Target Server Type    : MYSQL
Target Server Version : 50515
File Encoding         : 65001

Date: 2012-03-14 19:57:11
*/

SET FOREIGN_KEY_CHECKS=0;

-- ----------------------------
-- Table structure for `zone_areas`
-- ----------------------------
DROP TABLE IF EXISTS `zone_areas`;
CREATE TABLE `zone_areas` (
  `Zone_Areas_ID` varchar(255) NOT NULL,
  `ZoneId` smallint(5) unsigned DEFAULT NULL,
  `AreaId` smallint(5) unsigned DEFAULT NULL,
  `Realm` tinyint(3) unsigned DEFAULT NULL,
  `PieceId` tinyint(3) unsigned DEFAULT '0',
  `InfluenceId` int(10) unsigned DEFAULT NULL,
  `TokExploreEntry` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`Zone_Areas_ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
-- Records of zone_areas
-- ----------------------------
INSERT INTO `zone_areas` VALUES ('0068a789-6b57-42f6-bf68-eae3a477df2e', '205', '66', '2', '7', '148', '5812');
INSERT INTO `zone_areas` VALUES ('01014078-11db-4fec-9977-d73d8b5c4efb', '27', '47', '2', '0', '20', '0');
INSERT INTO `zone_areas` VALUES ('02251cc4-25b7-4300-a325-88202c7664f0', '191', '39', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('02afa0bf-d57b-4e09-84da-65705d1c489b', '5', '61', '1', '0', '42', '0');
INSERT INTO `zone_areas` VALUES ('039c7d60-ac9e-483d-804b-8153bfd4a640', '191', '21', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('03b34565-2281-4a2b-97d4-48d108629cd4', '3', '62', '2', '1', '16', '6150');
INSERT INTO `zone_areas` VALUES ('050cdbdf-52d0-4e87-8697-b862b9cdb6a5', '2', '1', '2', '0', '48', '0');
INSERT INTO `zone_areas` VALUES ('0518105c-d48b-4eb9-aabb-7b2051231a3a', '205', '61', '2', '5', '147', '5810');
INSERT INTO `zone_areas` VALUES ('0564470a-11c5-43dc-a599-4083739dc1b9', '100', '46', '1', '6', '70', '5650');
INSERT INTO `zone_areas` VALUES ('0579fdf2-63dd-4b71-a1d5-e45b1b9cd4ef', '107', '79', '1', '0', '94', '0');
INSERT INTO `zone_areas` VALUES ('05984bc2-6894-4b94-923b-0288fd8cf861', '100', '1', '1', '0', '119', '0');
INSERT INTO `zone_areas` VALUES ('05a0c68c-3fa0-4a1d-89dc-134309a56060', '101', '1', '1', '0', '120', '0');
INSERT INTO `zone_areas` VALUES ('0607ef4f-0b8b-4154-ae7b-9768e07d8a0b', '200', '107', '1', '0', '152', '0');
INSERT INTO `zone_areas` VALUES ('063c6684-7f6e-4679-a6a7-98e6172f7dd9', '191', '15', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('0771ad4d-15fd-4826-bbd8-3a4cd5cda012', '191', '26', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('07c76bb5-093a-4d09-a764-9fb8e4f1afd6', '8', '62', '2', '1', '13', '5605');
INSERT INTO `zone_areas` VALUES ('093dd050-010a-48a8-a210-215499752f98', '101', '77', '2', '0', '76', '0');
INSERT INTO `zone_areas` VALUES ('0a0f7669-1d85-416d-8b26-9ad28fa4a2ae', '209', '64', '2', '2', '150', '5831');
INSERT INTO `zone_areas` VALUES ('0a77289e-7b8d-46e1-b8bb-3c2c8d287290', '191', '7', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('0bcb016e-98f3-4e3a-b7ac-d205cf394f70', '3', '64', '1', '0', '43', '0');
INSERT INTO `zone_areas` VALUES ('0ce6d411-ab97-4b2e-9e0a-fb42640152ec', '108', '62', '1', '5', '100', '5703');
INSERT INTO `zone_areas` VALUES ('0d23d954-982b-41b0-bccf-2cab62a9b967', '107', '1', '1', '0', '120', '0');
INSERT INTO `zone_areas` VALUES ('0f86c2a7-f330-42b5-8582-f2627b5cb5b9', '120', '2', '1', '0', '105', '0');
INSERT INTO `zone_areas` VALUES ('10c300eb-35d5-46bd-a350-b2ed365137d6', '191', '27', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('10fb1142-a104-4ff4-ad1e-742620ac5763', '191', '10', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('11b4791b-717f-48a4-83fe-4260815947a2', '191', '5', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('128e899c-5415-4d52-b816-12bc78a71a18', '191', '13', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('13b341a5-768b-4a3c-ae34-db492491d7ab', '5', '64', '1', '0', '41', '0');
INSERT INTO `zone_areas` VALUES ('14f2dc8b-5a7c-44d7-a499-8875b287cb45', '160', '31', '2', '0', '128', '0');
INSERT INTO `zone_areas` VALUES ('160db93e-2146-4ff2-bc24-2a4fe66759d9', '100', '50', '2', '1', '66', '5645');
INSERT INTO `zone_areas` VALUES ('185dc0e8-f31b-4f05-a748-ad6af6fd9dcf', '191', '4', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('186a1afe-f9db-4926-91fe-f8312b1dfedf', '1', '1', '1', '0', '56', '0');
INSERT INTO `zone_areas` VALUES ('18ac1360-8010-49b7-900b-df0ec72c1499', '4', '61', '2', '0', '49', '0');
INSERT INTO `zone_areas` VALUES ('18ca7027-30fb-4631-ab7a-e4e71dc0aa1c', '205', '61', '1', '0', '169', '0');
INSERT INTO `zone_areas` VALUES ('1b31a08a-b256-44ac-9c02-16a696b7a8b3', '244', '31', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('1b79113e-e456-40a2-803e-2884b9d5c5d8', '191', '76', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('1b7997ba-30db-4739-af37-4dcb50ffdfc8', '27', '46', '2', '0', '20', '0');
INSERT INTO `zone_areas` VALUES ('1c29d056-b5b0-4c43-a74e-febc74bbd7bd', '191', '46', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('1cc25c6b-a654-451c-9ddc-790dc2ab1567', '209', '62', '1', '0', '167', '0');
INSERT INTO `zone_areas` VALUES ('1cdb0803-f826-4660-94d9-39a4096336ed', '209', '1', '1', '0', '186', '0');
INSERT INTO `zone_areas` VALUES ('1d9ff3c9-3245-4abf-bfb8-be79b2213303', '3', '59', '1', '0', '58', '0');
INSERT INTO `zone_areas` VALUES ('1dd07b0c-03b9-4644-8477-9d98bdd96aca', '1', '64', '1', '0', '28', '0');
INSERT INTO `zone_areas` VALUES ('1e76abdf-c981-4074-b85b-8e3d6bc7c733', '191', '16', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('2116ca6a-4854-4cc9-86a2-7de8ffe621c1', '105', '1', '1', '0', '122', '0');
INSERT INTO `zone_areas` VALUES ('212c9a92-3236-4929-b250-487d4cb1d54b', '109', '77', '1', '1', '103', '5741');
INSERT INTO `zone_areas` VALUES ('21b21e0a-2ada-4010-b13a-5253f2efe28e', '105', '76', '1', '0', '104', '0');
INSERT INTO `zone_areas` VALUES ('22cf0304-1324-4308-9f6f-7dd3f614aa2b', '3', '108', '2', '0', '49', '0');
INSERT INTO `zone_areas` VALUES ('2356949e-d83a-4841-b5a0-0b71fe579251', '191', '21', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('23a47a60-2012-48d4-b262-391e330cb5cd', '10', '61', '2', '0', '49', '0');
INSERT INTO `zone_areas` VALUES ('24091338-a930-4136-9991-4c722361a21c', '101', '82', '2', '5', '80', '5664');
INSERT INTO `zone_areas` VALUES ('24622b2f-410a-4a5e-8d0e-cc35983e9583', '108', '61', '1', '7', '101', '5701');
INSERT INTO `zone_areas` VALUES ('260de905-5391-4d71-bd97-c1fb6a2ac4a4', '191', '61', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('2665cc86-b598-43ca-9088-cb82c4261ac0', '5', '61', '2', '1', '20', '197');
INSERT INTO `zone_areas` VALUES ('27195311-686f-44b6-9a6c-fd103695b0e5', '102', '61', '1', '1', '97', '5708');
INSERT INTO `zone_areas` VALUES ('27328ff5-b983-43b6-b6c7-ae13c970fb60', '110', '1', '2', '0', '113', '0');
INSERT INTO `zone_areas` VALUES ('2809f010-56ca-436f-9475-1e6c9c18b37d', '191', '5', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('29c6eee1-8033-48ce-80d4-fd7ae425b209', '104', '1', '2', '0', '113', '0');
INSERT INTO `zone_areas` VALUES ('2a615fdd-b5b4-4d67-98bb-7b3ffe822f98', '242', '1', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('2a7f06fe-d0d7-44f0-b3cd-ac2dd0ee86a8', '105', '81', '2', '8', '90', '5738');
INSERT INTO `zone_areas` VALUES ('2b0af039-df30-4d22-9b3c-0cfa10072fe1', '1', '62', '1', '0', '32', '0');
INSERT INTO `zone_areas` VALUES ('2bf8747e-e9bf-4e3e-8a50-41971c6f4442', '103', '78', '1', '5', '108', '5725');
INSERT INTO `zone_areas` VALUES ('2c0076b3-eeb5-4cb8-9616-7e65fff0ad4a', '8', '63', '2', '1', '14', '5605');
INSERT INTO `zone_areas` VALUES ('2c30e89f-4826-4da0-a9ea-f308ec097bb0', '191', '14', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('2c48feb2-e98c-4acc-8192-3e2fc3c53f0d', '201', '1', '2', '7', '175', '5771');
INSERT INTO `zone_areas` VALUES ('2c676a60-d876-40ba-a384-3b244717b5da', '103', '77', '1', '6', '107', '5724');
INSERT INTO `zone_areas` VALUES ('2cfb0393-1ff7-450a-9241-8ba6086bb6f1', '9', '91', '2', '1', '22', '6123');
INSERT INTO `zone_areas` VALUES ('2d3bd7f5-afea-4119-a373-9495f20a04dc', '202', '62', '1', '0', '162', '0');
INSERT INTO `zone_areas` VALUES ('2d9384a5-caed-44be-997f-a1648c45d86b', '107', '78', '2', '8', '75', '5685');
INSERT INTO `zone_areas` VALUES ('2da15336-0e55-4858-9817-b9dcea2b90bc', '179', '31', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('2f58e608-11a9-4359-a237-ed7bf592a810', '108', '1', '2', '0', '112', '0');
INSERT INTO `zone_areas` VALUES ('30696681-b64b-4eaf-89f4-78bae40fcd0a', '9', '91', '1', '0', '38', '0');
INSERT INTO `zone_areas` VALUES ('31fdc93d-b53f-472e-8534-1849f0cdf48b', '209', '65', '2', '3', '151', '5832');
INSERT INTO `zone_areas` VALUES ('3395f84a-7ccf-4133-bf3b-07b667515232', '2', '63', '1', '0', '34', '0');
INSERT INTO `zone_areas` VALUES ('33a29f43-6f16-49c9-8e6f-8c3a405a2655', '27', '47', '1', '0', '42', '0');
INSERT INTO `zone_areas` VALUES ('33c052c7-9fd4-4302-acf5-f1387d45ca5a', '102', '63', '1', '8', '98', '5711');
INSERT INTO `zone_areas` VALUES ('3408b987-1d88-4969-ad53-ef5e70e11cd0', '105', '1', '2', '0', '113', '0');
INSERT INTO `zone_areas` VALUES ('34e10faf-691d-466f-8b85-c1d93ed35eaa', '6', '68', '1', '0', '25', '0');
INSERT INTO `zone_areas` VALUES ('351eb8cb-89ec-40d7-b724-91f5a297e4af', '200', '64', '1', '0', '154', '0');
INSERT INTO `zone_areas` VALUES ('352ca331-d384-4e10-82cf-6f2b1cf03ed7', '8', '1', '1', '0', '57', '0');
INSERT INTO `zone_areas` VALUES ('35563b58-bf2a-4be3-bce7-8955cb32f713', '191', '22', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('3747d253-6841-420a-850b-4f2611592158', '191', '76', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('37bffd6b-ba27-480e-9ac8-5a45742b7142', '101', '1', '2', '1', '111', '5660');
INSERT INTO `zone_areas` VALUES ('37c6a66b-b90b-4a24-802e-4e2a07a0ce94', '191', '12', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('37f2ee31-7b62-43d8-a100-c250c6301eb7', '191', '9', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('38205d7e-81d0-4065-b361-ac21fefd2ccc', '206', '1', '2', '0', '174', '0');
INSERT INTO `zone_areas` VALUES ('383dc314-b59e-48c5-b431-c0934ecfd558', '243', '31', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('38aa2ef4-409c-4bc3-80f1-2152911a3e5a', '106', '46', '1', '6', '72', '5656');
INSERT INTO `zone_areas` VALUES ('39b448fe-3ea9-42fb-bbe9-b096ee74e58a', '200', '106', '1', '0', '152', '0');
INSERT INTO `zone_areas` VALUES ('3b37720b-be9a-4030-8c4d-46f885adc804', '243', '31', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('3b43cea4-b6be-433a-a4ec-3d5e62e7047e', '105', '77', '1', '0', '105', '0');
INSERT INTO `zone_areas` VALUES ('3bbe9832-646b-4015-aaa4-baf3a421759a', '11', '66', '2', '8', '2', '5559');
INSERT INTO `zone_areas` VALUES ('3cb6647b-d280-4a67-809d-c9a2c081e386', '7', '61', '2', '2', '6', '5589');
INSERT INTO `zone_areas` VALUES ('3fc87b47-addd-4e94-ab9e-b331c4cc6e45', '205', '1', '1', '0', '186', '0');
INSERT INTO `zone_areas` VALUES ('3fd5d4d7-68f3-464a-b0c1-3771aec97591', '108', '61', '2', '7', '81', '5701');
INSERT INTO `zone_areas` VALUES ('4064aa85-d0b3-456a-8946-553b4bb7ba62', '102', '64', '1', '8', '99', '5711');
INSERT INTO `zone_areas` VALUES ('41ff63ac-0813-4a59-acea-c8a2d1f0d66b', '205', '63', '1', '0', '170', '0');
INSERT INTO `zone_areas` VALUES ('43ae4c88-d836-4edb-bc45-a6d6e47096fa', '201', '64', '2', '4', '135', '5768');
INSERT INTO `zone_areas` VALUES ('43c11e3d-3aaf-4efd-9a23-07fd1507fc4a', '207', '62', '2', '6', '137', '5787');
INSERT INTO `zone_areas` VALUES ('44b97fd0-5fb4-419b-ab65-c32befc2bd9d', '191', '18', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('44d65e04-d1f1-47d7-a609-59bfcfd98ed0', '6', '31', '1', '0', '24', '0');
INSERT INTO `zone_areas` VALUES ('47ec229c-9709-4cb7-953e-435386aa462b', '102', '62', '2', '5', '85', '5718');
INSERT INTO `zone_areas` VALUES ('49906614-3067-4620-af80-4b7231f69791', '191', '25', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('4ac0fd3e-8bef-445d-a013-8caa82314087', '179', '31', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('4d0174d3-cb96-4505-bdc1-14405fdef5b0', '191', '17', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('4d1626ce-a0e1-425e-9d8f-1354c64192e2', '191', '38', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('4d181fe1-d606-48d3-8cff-f49568c14d7c', '9', '62', '1', '0', '39', '0');
INSERT INTO `zone_areas` VALUES ('4d4d5b98-ff6b-4cc4-aa12-921bb65f4b4e', '205', '64', '1', '0', '168', '0');
INSERT INTO `zone_areas` VALUES ('4d60bc63-1360-4e8f-b1b7-992dbb5de7a7', '9', '1', '2', '1', '49', '6123');
INSERT INTO `zone_areas` VALUES ('4e66fe4a-7531-4ea5-abe3-015118155aff', '191', '29', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('4e8fd786-93e8-4c83-b69e-651b93df9dc0', '100', '48', '2', '3', '67', '5646');
INSERT INTO `zone_areas` VALUES ('4f8b1f8f-f9cd-4b94-9880-ad522410ac9f', '1', '65', '2', '1', '7', '193');
INSERT INTO `zone_areas` VALUES ('4fc502e8-3183-4470-9905-e24769a5eec8', '103', '76', '2', '1', '86', '5726');
INSERT INTO `zone_areas` VALUES ('4fe29c77-a891-4f1b-b8e1-ecf210fa80f5', '9', '2', '1', '0', '58', '0');
INSERT INTO `zone_areas` VALUES ('50121eaa-b788-4b9b-afbc-e785e46bd239', '120', '1', '1', '0', '104', '0');
INSERT INTO `zone_areas` VALUES ('5048963a-ed2c-43b4-bf02-aa12948e25e8', '200', '106', '2', '6', '130', '5753');
INSERT INTO `zone_areas` VALUES ('5278224b-5c5c-4319-b220-8b8dc792551c', '4', '61', '1', '0', '58', '0');
INSERT INTO `zone_areas` VALUES ('5428ba1a-d6cc-4719-82d9-cd097df9bf40', '105', '78', '1', '0', '106', '0');
INSERT INTO `zone_areas` VALUES ('54c4345b-b53e-45ee-b4c4-4b551b461341', '191', '3', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('54ce174a-7ebf-47d7-bcbe-3fb041475ca8', '191', '7', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('55c4f470-9cf4-4845-b3a4-46674c23d7df', '208', '1', '2', '0', '176', '0');
INSERT INTO `zone_areas` VALUES ('5697b46c-570a-4adf-b811-6037679dd9e3', '202', '65', '1', '0', '163', '0');
INSERT INTO `zone_areas` VALUES ('56a66668-074f-4ac1-916d-148b53b46854', '26', '46', '1', '0', '41', '0');
INSERT INTO `zone_areas` VALUES ('56ad61a9-cf6a-4f7a-9fbe-7cd80adcde08', '191', '14', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('574886d7-1776-4287-a909-61919d233363', '107', '77', '2', '0', '76', '0');
INSERT INTO `zone_areas` VALUES ('574949b2-d7ad-4eee-b080-3a0747ab53d3', '191', '30', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('578bcd97-1780-442a-ad14-621bca709629', '1', '1', '2', '0', '47', '0');
INSERT INTO `zone_areas` VALUES ('5809b9d9-2a97-432f-9731-0016526b5f50', '242', '1', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('581feddb-6e4f-412d-885e-a4922393f934', '200', '1', '1', '9', '183', '5756');
INSERT INTO `zone_areas` VALUES ('58bead28-de60-40fb-9157-d326f5ee3e6f', '5', '64', '2', '1', '19', '197');
INSERT INTO `zone_areas` VALUES ('58e7826d-5ab1-4081-9c7b-6206d3d9fb8b', '191', '2', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('59f47e14-357d-40e7-91fe-eff121e062e9', '191', '38', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('5ae99a19-475d-4676-bf70-11a5aa29a093', '11', '63', '2', '8', '2', '5559');
INSERT INTO `zone_areas` VALUES ('5af68b60-3d4a-46b8-992d-3958638c72df', '191', '6', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('5b5ecf97-407b-40aa-853f-4341eaa245aa', '109', '76', '1', '2', '102', '5742');
INSERT INTO `zone_areas` VALUES ('5c0b5661-2acc-435b-9763-c79f5a95861f', '191', '32', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('5cb44419-7f77-4470-b9f0-56ae0d467ae1', '202', '61', '2', '1', '139', '5791');
INSERT INTO `zone_areas` VALUES ('5d1cffef-9ac9-4d1d-9f32-117962cdeec1', '203', '61', '2', '5', '144', '5802');
INSERT INTO `zone_areas` VALUES ('5db2df9c-dfd4-4bc5-a2ae-302aa8fe7b3f', '244', '31', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('5e14f8af-051d-42e3-baa2-49bf9172c5de', '206', '63', '1', '5', '154', '5777');
INSERT INTO `zone_areas` VALUES ('5f6d3927-db54-4059-8dea-1c18b1a9b7c6', '9', '64', '2', '1', '23', '6123');
INSERT INTO `zone_areas` VALUES ('604bdbd5-0260-4d46-ae1a-cda67f9f8e1b', '109', '78', '2', '4', '91', '5744');
INSERT INTO `zone_areas` VALUES ('61675ce0-3fe8-4591-9bce-0c8095ea1707', '7', '1', '2', '2', '47', '5589');
INSERT INTO `zone_areas` VALUES ('62d1118e-4d3b-419e-9b8f-8dc687e70818', '191', '26', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('63a90bd8-f46e-42aa-a28f-17870f65e95c', '191', '17', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('63c7b6c2-cf5f-4f2c-9355-bd3250622a43', '101', '78', '1', '1', '73', '5660');
INSERT INTO `zone_areas` VALUES ('6408bdf6-1231-459a-b26e-500feada89eb', '191', '6', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('64130cfb-cab1-484b-b367-3b1b98c78842', '106', '49', '1', '2', '68', '5653');
INSERT INTO `zone_areas` VALUES ('64957a89-3860-4641-8bcf-e6b36fc1426d', '201', '62', '2', '5', '134', '5769');
INSERT INTO `zone_areas` VALUES ('64a0e313-6e2f-4627-8676-a5ef4736b7f2', '191', '19', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('65132da3-6154-47ad-a386-a7c743e74d14', '202', '61', '1', '0', '161', '0');
INSERT INTO `zone_areas` VALUES ('655015ac-1ab4-41e7-9340-7764a8c4f30d', '241', '31', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('6626a269-ccf1-41b8-a5b2-80db9ab9b401', '8', '64', '2', '1', '15', '5605');
INSERT INTO `zone_areas` VALUES ('66743c71-7555-491b-ba82-4736fedfefbd', '108', '1', '1', '0', '121', '0');
INSERT INTO `zone_areas` VALUES ('67ab599c-7f75-4ba8-acfa-cb200638e76e', '103', '1', '2', '1', '113', '5726');
INSERT INTO `zone_areas` VALUES ('67e550fb-1a93-4046-bdff-ade0a4d03b6a', '208', '64', '1', '0', '165', '0');
INSERT INTO `zone_areas` VALUES ('68118c7b-b143-494b-a3f2-4a8cd69fc713', '191', '15', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('68935e60-3874-4299-bbb7-e66eb97c5bde', '103', '1', '1', '0', '122', '0');
INSERT INTO `zone_areas` VALUES ('68be94e9-6872-4a27-9159-8442b3252b43', '9', '2', '2', '1', '49', '6123');
INSERT INTO `zone_areas` VALUES ('6a2e8fac-5fab-4dbe-9977-a70193de3fb7', '202', '1', '2', '1', '176', '5791');
INSERT INTO `zone_areas` VALUES ('6ab0b685-29aa-42a9-88c8-a7304e7fac02', '7', '62', '2', '2', '8', '5589');
INSERT INTO `zone_areas` VALUES ('6d60f700-9989-4514-8d0f-dd23801c3414', '205', '66', '1', '0', '171', '0');
INSERT INTO `zone_areas` VALUES ('6ebac24b-bc34-482a-9765-e32191e9c5b0', '101', '80', '1', '0', '94', '0');
INSERT INTO `zone_areas` VALUES ('6fae9a91-7aa6-434d-92e2-5b7bc0d88f9c', '191', '12', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('7039a46f-7a73-4574-99c1-e58d5f5f845c', '191', '25', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('70a6a9b7-0da5-4a89-951a-30bd8e2b6555', '191', '2', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('7171e883-0332-4a07-acad-082cf904d5f9', '191', '28', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('71febb73-6a3a-4711-8ece-7538de975603', '207', '64', '2', '3', '138', '5784');
INSERT INTO `zone_areas` VALUES ('72f60616-430b-4e32-9bcd-9f5295e6ebc7', '10', '61', '1', '0', '58', '0');
INSERT INTO `zone_areas` VALUES ('73d6bea2-a4b9-417b-9cc1-0f1885e30f49', '102', '65', '1', '0', '100', '0');
INSERT INTO `zone_areas` VALUES ('76cf81f3-cf2f-4240-b8ca-fc1d1401e1b1', '8', '1', '2', '1', '48', '5605');
INSERT INTO `zone_areas` VALUES ('77cb0b96-e2b2-4ee0-8bc5-e665052bd28d', '11', '64', '2', '6', '4', '5564');
INSERT INTO `zone_areas` VALUES ('78bedb66-76d6-49b6-aec3-6416bc9b388c', '191', '28', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('7969ed0c-1cff-417f-a521-7f5a68e1e896', '100', '1', '2', '1', '110', '5645');
INSERT INTO `zone_areas` VALUES ('7bb4cfed-4655-4a53-8711-46c25f86e7d5', '220', '32', '1', '0', '168', '0');
INSERT INTO `zone_areas` VALUES ('7bb8d260-9bd8-4b15-a7b8-0e47857293ad', '101', '80', '2', '0', '78', '0');
INSERT INTO `zone_areas` VALUES ('7cbf5524-7839-438a-a2e1-795f7fe73bd2', '60', '31', '1', '0', '64', '0');
INSERT INTO `zone_areas` VALUES ('7d4af57b-55c0-4bd4-95a9-032060b43c9e', '206', '61', '2', '5', '133', '5777');
INSERT INTO `zone_areas` VALUES ('7df27030-e83f-4edd-aad7-67710c593050', '191', '23', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('7f101844-d69d-4fb2-86fa-7a723d92bc1c', '11', '1', '1', '0', '55', '0');
INSERT INTO `zone_areas` VALUES ('7fbcdb4e-8068-44d8-ad7d-62de9ac4a4e6', '3', '65', '1', '0', '44', '0');
INSERT INTO `zone_areas` VALUES ('801bdbc8-17b6-4c07-9f5b-e23af19ce0fa', '27', '48', '1', '0', '42', '0');
INSERT INTO `zone_areas` VALUES ('805092c1-405f-4581-b677-eabd68575d90', '120', '3', '2', '0', '88', '0');
INSERT INTO `zone_areas` VALUES ('80f9a80e-3402-4855-95db-fcd63bdb1f5a', '206', '61', '1', '0', '155', '0');
INSERT INTO `zone_areas` VALUES ('818b606d-cdc2-460f-93bf-4bf7cae00b58', '102', '64', '2', '8', '84', '5711');
INSERT INTO `zone_areas` VALUES ('8257272e-7387-42a3-bf0a-4f59f8e25a5c', '11', '63', '1', '0', '27', '0');
INSERT INTO `zone_areas` VALUES ('82de6e24-512c-4e77-88cf-dc963d5d25c1', '3', '1', '1', '0', '58', '0');
INSERT INTO `zone_areas` VALUES ('86341c06-c8b4-42df-993b-475355eef5e4', '191', '24', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('8708b197-269b-46b3-9c08-7419aa7e1a2f', '200', '107', '2', '6', '130', '5753');
INSERT INTO `zone_areas` VALUES ('872f6a1d-c5af-49de-9f22-e14e743b8034', '27', '48', '2', '0', '20', '0');
INSERT INTO `zone_areas` VALUES ('887d2aed-f8ee-48ad-8884-1fc0fbb2b55d', '191', '10', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('88ee0178-016c-47cf-8919-b5a040710f47', '3', '61', '2', '1', '17', '6150');
INSERT INTO `zone_areas` VALUES ('8a0d4c20-bb5a-4829-8e73-db22b55bef76', '206', '63', '2', '0', '132', '0');
INSERT INTO `zone_areas` VALUES ('8d356059-650e-4cd2-95b2-9d311147657c', '201', '63', '1', '0', '157', '0');
INSERT INTO `zone_areas` VALUES ('8d48a70f-8742-45af-8b26-f69eb92ef876', '9', '1', '1', '0', '58', '0');
INSERT INTO `zone_areas` VALUES ('8f1b2ae6-686a-4192-89b8-b7b3c0efe919', '106', '47', '1', '3', '74', '5655');
INSERT INTO `zone_areas` VALUES ('900c6de6-2288-49ad-b021-c161677aee73', '1', '63', '1', '0', '31', '0');
INSERT INTO `zone_areas` VALUES ('903aef6a-f424-4013-a689-d3ba49ce0b9f', '191', '13', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('908131ed-200b-49f6-9aa3-d35d14a2842d', '106', '50', '1', '2', '119', '5653');
INSERT INTO `zone_areas` VALUES ('90b36dff-468b-42a6-877d-58142bc0fa8f', '206', '1', '1', '0', '183', '0');
INSERT INTO `zone_areas` VALUES ('90ed87e2-77a0-44b6-a2c9-7c3f2ba65e24', '103', '77', '2', '7', '87', '5723');
INSERT INTO `zone_areas` VALUES ('91eefdf0-0443-4632-a662-a83b679d1f57', '109', '1', '1', '0', '122', '0');
INSERT INTO `zone_areas` VALUES ('932e716f-1a29-4588-8ff3-49b431611cbf', '11', '62', '2', '7', '3', '5570');
INSERT INTO `zone_areas` VALUES ('95d4055d-bfc0-4c18-a4ca-a15ccc7bdb22', '202', '1', '1', '0', '185', '0');
INSERT INTO `zone_areas` VALUES ('9618abf3-a4f6-410d-b629-b1070753b1b6', '26', '47', '2', '0', '19', '0');
INSERT INTO `zone_areas` VALUES ('975130e1-8e99-46d9-ba80-47456d0c3572', '209', '63', '2', '1', '149', '5830');
INSERT INTO `zone_areas` VALUES ('99d4ec03-835c-4cdb-849b-bb04222527e4', '200', '62', '1', '0', '153', '0');
INSERT INTO `zone_areas` VALUES ('9ae367b5-0fe7-4748-9148-8ade18691534', '5', '63', '1', '0', '40', '0');
INSERT INTO `zone_areas` VALUES ('9b1cee33-26c3-4d0e-9184-e8b3b3a6f5f2', '5', '1', '2', '0', '49', '0');
INSERT INTO `zone_areas` VALUES ('9c56b115-f0cf-41bd-889c-749858e0c40a', '11', '64', '1', '0', '26', '0');
INSERT INTO `zone_areas` VALUES ('9d0da76f-bba2-4c7d-b359-b02ddf1c2813', '191', '20', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('9d1c9434-d862-4b07-afa5-87147640e2ea', '2', '64', '1', '0', '36', '0');
INSERT INTO `zone_areas` VALUES ('9d3d3be3-f766-4c30-b0a2-c5ea7ebb5580', '6', '1', '1', '0', '55', '0');
INSERT INTO `zone_areas` VALUES ('9f44498f-b55d-4ae0-b2bf-a996314c1256', '2', '62', '1', '0', '33', '0');
INSERT INTO `zone_areas` VALUES ('9ff155ca-81e9-438b-93ff-ad406032d16a', '203', '1', '2', '5', '177', '5802');
INSERT INTO `zone_areas` VALUES ('a19bafe7-dccf-411f-8ed4-6d0a5150baca', '191', '16', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('a211f477-5eaa-4362-8444-d951b704a7c3', '106', '50', '2', '0', '110', '0');
INSERT INTO `zone_areas` VALUES ('a22a9f58-9cd6-4b11-86be-131144287ec1', '5', '63', '2', '1', '18', '197');
INSERT INTO `zone_areas` VALUES ('a3f29bb7-d099-4bdc-835c-11346969a3d0', '204', '61', '1', '0', '186', '0');
INSERT INTO `zone_areas` VALUES ('a5dcb032-e949-4957-87c8-4314b6643c45', '207', '1', '2', '0', '175', '0');
INSERT INTO `zone_areas` VALUES ('a661ed6d-cf42-42fb-b542-a3f5bba22737', '3', '59', '2', '0', '49', '0');
INSERT INTO `zone_areas` VALUES ('a6be410f-de92-457f-a157-eb5a8ed4e0e6', '220', '32', '2', '0', '146', '0');
INSERT INTO `zone_areas` VALUES ('a882d824-480d-4829-a3b1-40d543dd3753', '203', '65', '1', '1', '173', '5798');
INSERT INTO `zone_areas` VALUES ('a905677e-8895-4e6f-a65c-2caf10b0e394', '7', '65', '2', '2', '10', '5589');
INSERT INTO `zone_areas` VALUES ('aaa140b5-e6f0-4d33-b86f-b8a8193ead5b', '191', '32', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('aad14b93-4d68-430a-9ef6-75e403406ade', '191', '19', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('aafc5703-67fa-492a-97eb-f0f4cfd13d2b', '202', '64', '2', '4', '141', '5793');
INSERT INTO `zone_areas` VALUES ('ab760b02-a26c-46be-bfc9-0054151010d0', '5', '1', '1', '0', '58', '0');
INSERT INTO `zone_areas` VALUES ('ace9a179-be32-494d-a801-e7570de77054', '201', '61', '1', '0', '156', '0');
INSERT INTO `zone_areas` VALUES ('add706af-d786-454a-92e2-e2f9e5d1795c', '9', '63', '2', '1', '21', '6123');
INSERT INTO `zone_areas` VALUES ('ae008abb-21d3-4626-a4ea-7bfbfe5be1c2', '208', '62', '1', '0', '164', '0');
INSERT INTO `zone_areas` VALUES ('af0a444f-1d9b-4847-b469-39d43afcf48f', '3', '1', '2', '0', '49', '0');
INSERT INTO `zone_areas` VALUES ('af358c99-2814-4ac9-aee8-15e1c8042b2a', '2', '65', '1', '0', '37', '0');
INSERT INTO `zone_areas` VALUES ('af7a52b8-ea3f-4d18-82ef-e09f94656e82', '200', '62', '2', '3', '131', '5750');
INSERT INTO `zone_areas` VALUES ('afc53bc1-2869-433d-9d98-f462b719ef57', '191', '18', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('b005fc03-4ad5-4b0c-a23e-f6a7bb78feff', '9', '61', '2', '1', '22', '6123');
INSERT INTO `zone_areas` VALUES ('b02cdf93-ef9a-4cf9-be73-78a4cdb30d39', '191', '30', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('b1f4c976-c169-4918-9437-2b56f74056f7', '109', '80', '2', '6', '93', '5746');
INSERT INTO `zone_areas` VALUES ('b22fdb02-6f38-4f9e-b9b3-1a96ba9960d6', '107', '81', '1', '0', '96', '0');
INSERT INTO `zone_areas` VALUES ('b4188cf0-c6f3-4bc1-af59-791ff5ca211b', '191', '46', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('b4dacd8c-5bea-4bac-9dee-55f9cf178606', '191', '22', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('b4f6b5dd-1138-4984-bcfc-bce6ffaaab4c', '2', '63', '2', '1', '12', '5594');
INSERT INTO `zone_areas` VALUES ('b655e83c-186e-4145-b4b7-35058d813e29', '191', '8', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('b6ad654b-1a84-4e22-bada-7c6f549daddd', '203', '62', '2', '6', '145', '5803');
INSERT INTO `zone_areas` VALUES ('b736bfc9-4f7f-45bf-b0a5-e6ed58a40081', '1', '65', '1', '0', '29', '0');
INSERT INTO `zone_areas` VALUES ('b79a0a09-1aba-4ec3-9840-5be54320af4e', '191', '31', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('babe2a9f-8b2f-4455-bf40-85269480f16a', '191', '39', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('bb7b3597-0b3f-47ae-b885-b7b3deb83b4d', '120', '3', '1', '0', '106', '0');
INSERT INTO `zone_areas` VALUES ('bbe7cf6a-5837-4d5f-8947-8aed90d11764', '209', '1', '2', '0', '177', '0');
INSERT INTO `zone_areas` VALUES ('bbf2b62e-e2a8-4fa4-94d0-a320ebd345da', '108', '62', '2', '1', '83', '5695');
INSERT INTO `zone_areas` VALUES ('bd7f0a8d-8f0b-460e-9d86-0fe5ec6c082c', '191', '33', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('befe016e-59cb-4000-83a1-4259286011d9', '191', '4', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('c08373e5-44c6-4efa-bba5-f8a6fb6019d0', '200', '1', '2', '9', '174', '5756');
INSERT INTO `zone_areas` VALUES ('c0c2be2d-4af6-4d88-9ee5-3707e8a734f8', '191', '24', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('c112e8a0-a945-4489-bbc2-5826714fc5d7', '109', '1', '2', '0', '113', '0');
INSERT INTO `zone_areas` VALUES ('c18b5078-15dd-4455-a58b-227ba6aed6c4', '7', '1', '1', '0', '56', '0');
INSERT INTO `zone_areas` VALUES ('c2a60e05-6fb5-4208-9bfe-0fdbb39fa12b', '191', '9', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('c3b07909-bc6d-4c39-8236-1fc87e18e780', '191', '33', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('c3f053df-7da2-4475-a72c-6d79e58e8ed0', '191', '27', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('c5005803-9723-497e-9cf8-b4085f5c0cc6', '6', '68', '2', '5', '5', '5556');
INSERT INTO `zone_areas` VALUES ('c56fa533-63bd-4688-bf79-dc1203898e2b', '208', '63', '2', '1', '142', '5818');
INSERT INTO `zone_areas` VALUES ('c63eafbc-6fba-45c4-ad2a-4c5e0b8a1a9d', '220', '31', '1', '0', '168', '0');
INSERT INTO `zone_areas` VALUES ('c67b69b4-f6aa-4cb4-abfc-3a742cdd4119', '191', '61', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('c6abc01a-5e69-4fba-8890-b0a06d105a2e', '3', '66', '1', '0', '45', '0');
INSERT INTO `zone_areas` VALUES ('c7806aa0-1fe8-475c-a39f-928fb40a39d6', '191', '23', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('c8d6821a-1df0-4c7d-bc0b-95e22e86f967', '11', '1', '2', '0', '46', '0');
INSERT INTO `zone_areas` VALUES ('c9142ed5-c02a-44e5-9402-f27202359e83', '8', '62', '1', '0', '35', '0');
INSERT INTO `zone_areas` VALUES ('cc63f6b4-8887-4276-be01-f3f40b5675f9', '26', '47', '1', '0', '41', '0');
INSERT INTO `zone_areas` VALUES ('cc921280-6a7f-4318-bf97-b1ae45c0ae60', '191', '29', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('ccce1258-50ae-4acd-b0ff-72775a6004e7', '7', '62', '1', '0', '30', '0');
INSERT INTO `zone_areas` VALUES ('cd39e07f-3c46-4d10-b1f5-7ff729e2dcaf', '191', '8', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('ce5e3389-c255-4d6e-b0b5-6586217211ff', '120', '2', '2', '0', '90', '0');
INSERT INTO `zone_areas` VALUES ('cf00f13b-d88e-4f3e-9ef7-25455b129082', '203', '63', '1', '0', '171', '0');
INSERT INTO `zone_areas` VALUES ('cfc2baa5-83d3-423a-8d14-307e6c3f8b68', '9', '61', '1', '0', '38', '0');
INSERT INTO `zone_areas` VALUES ('d0e560fb-a070-4cf0-9c99-834354829d45', '191', '2', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('d1ddb36d-76b6-4cf4-9f00-76e90487ac6e', '201', '66', '2', '1', '136', '5765');
INSERT INTO `zone_areas` VALUES ('d25fc8d4-1890-46e2-a7e2-dc82ce48c60d', '110', '1', '1', '0', '122', '0');
INSERT INTO `zone_areas` VALUES ('d2a9f87c-2382-46a5-ba1a-4829661cbd5b', '202', '63', '2', '3', '140', '5795');
INSERT INTO `zone_areas` VALUES ('d36cae36-4026-4c86-bd15-e022a4f7e201', '6', '1', '2', '5', '46', '5556');
INSERT INTO `zone_areas` VALUES ('d5eeb1d2-6706-4b71-a411-952967bec8c1', '101', '79', '1', '0', '77', '0');
INSERT INTO `zone_areas` VALUES ('d98e8468-42ce-4bae-8169-235048bb493e', '205', '1', '2', '0', '177', '0');
INSERT INTO `zone_areas` VALUES ('da5c9cbe-739f-4471-97a1-8a3f86b22cf7', '103', '79', '1', '3', '109', '5729');
INSERT INTO `zone_areas` VALUES ('dd8bb716-59bb-45f5-9554-dade0d9cd777', '191', '20', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('df2d61e9-1299-4d60-8740-8bb4aa076b98', '107', '80', '1', '8', '95', '5685');
INSERT INTO `zone_areas` VALUES ('df85fd0d-7efb-4772-ae19-bb5023686bbe', '100', '47', '2', '1', '66', '5645');
INSERT INTO `zone_areas` VALUES ('e0319a28-d23f-4cd1-8fa0-5dfa95dfaae2', '105', '79', '2', '1', '88', '5731');
INSERT INTO `zone_areas` VALUES ('e1234df5-340f-4654-8660-f63e250d72fd', '8', '61', '2', '1', '11', '5605');
INSERT INTO `zone_areas` VALUES ('e19580bd-65a1-435f-a364-f90ebc5eed1e', '107', '79', '2', '4', '78', '5681');
INSERT INTO `zone_areas` VALUES ('e224aee0-bc98-4430-b2d3-4d58822cb3af', '200', '64', '2', '7', '132', '5754');
INSERT INTO `zone_areas` VALUES ('e4274fe8-4fee-4a8e-9bb3-87f7478c0d30', '204', '61', '2', '0', '177', '0');
INSERT INTO `zone_areas` VALUES ('e4f06a67-3a71-45d7-b67e-5c33e1de9d88', '102', '1', '1', '0', '121', '0');
INSERT INTO `zone_areas` VALUES ('e5aff479-be7b-4f8b-850c-773991ce20a2', '11', '65', '2', '8', '2', '5559');
INSERT INTO `zone_areas` VALUES ('e5c01b6c-88b1-4b5d-8544-1341311120dd', '201', '65', '1', '0', '158', '0');
INSERT INTO `zone_areas` VALUES ('e658da2c-cc24-4aa1-9f2b-c78407efd75a', '3', '108', '1', '0', '58', '0');
INSERT INTO `zone_areas` VALUES ('e6ebfff9-038f-45e2-9cc2-74e2cc2f54c1', '107', '1', '2', '0', '111', '0');
INSERT INTO `zone_areas` VALUES ('e6fc6f85-f38c-4bc2-9563-a0c0f234156d', '208', '65', '2', '5', '143', '5820');
INSERT INTO `zone_areas` VALUES ('e81f8272-ec23-4699-ba91-0a71d30f018f', '207', '61', '1', '0', '159', '0');
INSERT INTO `zone_areas` VALUES ('e8954b84-98d1-46d8-8cd4-6bb9889ab648', '210', '61', '2', '0', '177', '0');
INSERT INTO `zone_areas` VALUES ('ea137a06-f2f4-4c05-8a8a-26657a09e282', '210', '61', '1', '0', '186', '0');
INSERT INTO `zone_areas` VALUES ('ea81109f-857a-4646-8439-b8a691a75e85', '105', '80', '2', '7', '89', '5739');
INSERT INTO `zone_areas` VALUES ('ebb4bd0a-6342-42e4-a9f1-59af385a6793', '205', '63', '2', '7', '148', '5812');
INSERT INTO `zone_areas` VALUES ('ec717ed4-d1ed-4b6e-bf88-18cb44bda8e5', '208', '1', '1', '0', '185', '0');
INSERT INTO `zone_areas` VALUES ('edcf04af-a809-4a1f-a76b-b82aa56548f2', '191', '3', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('edd136ad-2cf5-4aa0-8d72-4f8303b96d85', '160', '31', '1', '0', '129', '0');
INSERT INTO `zone_areas` VALUES ('f04402d9-2fb8-4ae0-8d5f-d68f3e742138', '203', '1', '1', '0', '186', '0');
INSERT INTO `zone_areas` VALUES ('f045ff38-2ada-43f6-b5e5-2a34ea4f54a2', '241', '31', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('f08616e5-d88f-42d4-bfdc-f00ce99d3fab', '106', '48', '2', '2', '69', '5653');
INSERT INTO `zone_areas` VALUES ('f0a666d9-a8ee-4274-8442-afd535db6760', '108', '63', '2', '2', '82', '5698');
INSERT INTO `zone_areas` VALUES ('f0e235d5-83dc-42dd-a5bf-7061ecb14d16', '207', '1', '1', '0', '184', '0');
INSERT INTO `zone_areas` VALUES ('f12e9af4-76a3-4b20-b471-5eac75af00da', '102', '1', '2', '2', '112', '5715');
INSERT INTO `zone_areas` VALUES ('f1c5530e-5f8c-4c33-b17b-8185e2baa119', '191', '31', '2', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('f2d76884-5e45-4218-89d9-d8fb533cc64a', '109', '79', '2', '5', '92', '5745');
INSERT INTO `zone_areas` VALUES ('f425e41e-0d4b-47c0-bc5b-d9762fbcf99e', '7', '64', '2', '2', '9', '5589');
INSERT INTO `zone_areas` VALUES ('f4752aed-52c8-4b5d-8619-34407f909504', '207', '63', '1', '0', '160', '0');
INSERT INTO `zone_areas` VALUES ('f47eeef2-d446-4242-8b9f-11e5c83ee358', '205', '65', '2', '1', '146', '5806');
INSERT INTO `zone_areas` VALUES ('f50344c7-a46d-4c18-87a6-e6b1ad7e020b', '27', '46', '1', '0', '42', '0');
INSERT INTO `zone_areas` VALUES ('f55c9d53-29ee-42f6-9e47-ac606ac2d5cd', '203', '64', '1', '2', '172', '5799');
INSERT INTO `zone_areas` VALUES ('f6021ca7-5f6c-4940-95da-f823f3d4779d', '104', '1', '1', '0', '122', '0');
INSERT INTO `zone_areas` VALUES ('f69068dc-d39c-4a42-bac2-003e053ff8c1', '106', '49', '2', '2', '71', '5653');
INSERT INTO `zone_areas` VALUES ('f710c800-4973-4714-bdfe-fc1f4152cacd', '191', '2', '1', '0', '0', '0');
INSERT INTO `zone_areas` VALUES ('f85c67d8-ef90-43d3-ab4a-bc582a0c5a09', '101', '81', '2', '8', '79', '5667');
INSERT INTO `zone_areas` VALUES ('f88df968-3d04-434f-b90b-0e4145e96187', '60', '31', '2', '0', '65', '0');
INSERT INTO `zone_areas` VALUES ('f8b96067-d616-4920-ba6e-84f81274962c', '209', '61', '1', '6', '166', '5835');
INSERT INTO `zone_areas` VALUES ('fa295f74-434e-4021-95cf-e392aba679a3', '201', '1', '1', '0', '184', '0');
INSERT INTO `zone_areas` VALUES ('fa864e2d-e261-4728-8906-ba723c6033e8', '2', '1', '1', '0', '57', '0');
INSERT INTO `zone_areas` VALUES ('fbb9b665-c4c1-4d19-b4fd-afb9d03d43a6', '26', '46', '2', '0', '19', '0');
INSERT INTO `zone_areas` VALUES ('fd0132ce-1677-444a-ad48-4b7bb2eebc74', '220', '31', '2', '0', '146', '0');
