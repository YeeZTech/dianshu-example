/*
 Navicat Premium Data Transfer

 Source Server         : mysql
 Source Server Type    : MySQL
 Source Server Version : 50733
 Source Host           : localhost:3306
 Source Schema         : cbd

 Target Server Type    : MySQL
 Target Server Version : 50733
 File Encoding         : 65001

 Date: 19/05/2023 11:04:05
*/

SET NAMES utf8mb4;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
-- Table structure for dic_ent_type
-- ----------------------------
DROP TABLE IF EXISTS `dic_ent_type`;
CREATE TABLE `dic_ent_type`  (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT 'id',
  `qylx_dl_id` varchar(60) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '企业大类',
  `qylx_dl_name` varchar(60) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '企业大类名称',
  `qylx_xl_id` varchar(60) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '企业小类',
  `qylx_xl_name` varchar(60) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '企业小类名称',
  `qylx_name` varchar(100) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '非国家企业分类标准',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '企业类型字典表' ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of dic_ent_type
-- ----------------------------
INSERT INTO `dic_ent_type` VALUES ('dca8623b356e11ec95aa0cda411e019e', '100', '内资企业', '159', '其他有限责任公司', '其他有限责任公司');
INSERT INTO `dic_ent_type` VALUES ('dca863bd356e11ec95aa0cda411e019e', '100', '内资企业', '173', '私营有限责任公司', '私营有限责任公司');
INSERT INTO `dic_ent_type` VALUES ('dca86a8b356e11ec95aa0cda411e019e', '100', '内资企业', '196', '事业单位', '事业单位');
INSERT INTO `dic_ent_type` VALUES ('dca874a0356e11ec95aa0cda411e019e', '100', '内资企业', '159', '其他有限责任公司', '股份制企业（非法人）');
INSERT INTO `dic_ent_type` VALUES ('dca87ddd356e11ec95aa0cda411e019e', '100', '内资企业', '159', '其他有限责任公司', '有限责任公司（自然人投资或控股）');
INSERT INTO `dic_ent_type` VALUES ('dca87ea7356e11ec95aa0cda411e019e', '100', '内资企业', '159', '其他有限责任公司', '有限责任公司（国有控股）');
INSERT INTO `dic_ent_type` VALUES ('dca87eef356e11ec95aa0cda411e019e', '100', '内资企业', '159', '其他有限责任公司', '有限责任公司分公司（自然人投资或控股）');
INSERT INTO `dic_ent_type` VALUES ('dca87f3a356e11ec95aa0cda411e019e', '100', '内资企业', '159', '其他有限责任公司', '有限责任公司分公司（国有控股）');
INSERT INTO `dic_ent_type` VALUES ('dca87f81356e11ec95aa0cda411e019e', '100', '内资企业', '159', '其他有限责任公司', '其他有限责任公司分公司');
INSERT INTO `dic_ent_type` VALUES ('dca8840a356e11ec95aa0cda411e019e', '100', '内资企业', '173', '私营有限责任公司', '有限责任公司（自然人独资）');
INSERT INTO `dic_ent_type` VALUES ('dca88452356e11ec95aa0cda411e019e', '100', '内资企业', '173', '私营有限责任公司', '有限责任公司（法人独资）');
INSERT INTO `dic_ent_type` VALUES ('dca8849c356e11ec95aa0cda411e019e', '100', '内资企业', '173', '私营有限责任公司', '有限责任公司分公司（自然人独资）');
INSERT INTO `dic_ent_type` VALUES ('dca884e5356e11ec95aa0cda411e019e', '100', '内资企业', '173', '私营有限责任公司', '有限责任公司分公司（法人独资）');
INSERT INTO `dic_ent_type` VALUES ('dca8963c356e11ec95aa0cda411e019e', '100', '内资企业', '173', '私营有限责任公司', '有限责任公司（自然人投资或控股的法人独资）');
INSERT INTO `dic_ent_type` VALUES ('dca8a2ab356e11ec95aa0cda411e019e', '100', '内资企业', '173', '私营有限责任公司', '有限责任公司(自然人投资或控股)');
INSERT INTO `dic_ent_type` VALUES ('dca8a334356e11ec95aa0cda411e019e', '100', '内资企业', '173', '私营有限责任公司', '有限责任公司(自然人独资)');
INSERT INTO `dic_ent_type` VALUES ('dca8a3b5356e11ec95aa0cda411e019e', '100', '内资企业', '173', '私营有限责任公司', '有限责任公司(法人独资)');

-- ----------------------------
-- Table structure for dic_industry
-- ----------------------------
DROP TABLE IF EXISTS `dic_industry`;
CREATE TABLE `dic_industry`  (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '编号',
  `hylx_ml_id` varchar(60) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '行业门类代码',
  `hylx_ml_name` varchar(60) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业门类名称',
  `hylx_dl_id` varchar(60) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业大类代码',
  `hylx_dl_name` varchar(60) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业大类名称',
  `hylx_zl_id` varchar(61) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业中类代码',
  `hylx_zl_name` varchar(60) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业中类名称',
  `hylx_xl_id` varchar(62) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业小类代码',
  `hylx_xl_name` varchar(60) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业小类名称',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '行业字典表' ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of dic_industry
-- ----------------------------
INSERT INTO `dic_industry` VALUES ('8f36199f357011ec95aa0cda411e019e', 'C', '制造业', '20', '木材加工和木、竹、藤、棕、草制品业', '202', '人造板制造', '2029', '其他人造板制造');
INSERT INTO `dic_industry` VALUES ('8f3fb33c357011ec95aa0cda411e019e', 'C', '制造业', '40', '仪器仪表制造业', '401', '通用仪器仪表制造', '4014', '实验分析仪器制造');
INSERT INTO `dic_industry` VALUES ('8f406c30357011ec95aa0cda411e019e', 'F', '批发和零售业', '51', '批发业', '512', '食品、饮料及烟草制品批发', '5125', '盐及调味品批发');
INSERT INTO `dic_industry` VALUES ('8f408d22357011ec95aa0cda411e019e', 'F', '批发和零售业', '51', '批发业', '516', '矿产品、建材及化工产品批发', '5164', '金属及金属矿批发');
INSERT INTO `dic_industry` VALUES ('8f41105d357011ec95aa0cda411e019e', 'J', '金融业', '67', '资本市场服务', '673', '非公开募集证券投资基金', '6739', '其他非公开募集证券投资基金');
INSERT INTO `dic_industry` VALUES ('8f4145af357011ec95aa0cda411e019e', 'K', '房地产业', '70', '房地产业', '701', '房地产开发经营', '7010', '房地产开发经营');
INSERT INTO `dic_industry` VALUES ('8f41a10b357011ec95aa0cda411e019e', 'M', '科学研究和技术服务业', '73', '研究和试验发展', '732', '工程和技术研究和试验发展', '7320', '工程和技术研究和试验发展');
INSERT INTO `dic_industry` VALUES ('8f445339357011ec95aa0cda411e019e', 'M', '科学研究和技术服务业', '75', '科技推广和应用服务业', '751', '技术推广服务', '7519', '其他技术推广服务');
INSERT INTO `dic_industry` VALUES ('8f44c537357011ec95aa0cda411e019e', 'P', '教育', '83', '教育', '833', '中等教育', '8334', '普通高中教育');

-- ----------------------------
-- Table structure for dic_region
-- ----------------------------
DROP TABLE IF EXISTS `dic_region`;
CREATE TABLE `dic_region`  (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '编号',
  `jx_id` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '街乡代码',
  `jx_name` text CHARACTER SET utf8 COLLATE utf8_general_ci NULL COMMENT '街乡名称简称',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '街乡字典表' ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of dic_region
-- ----------------------------
INSERT INTO `dic_region` VALUES ('049f4e06194543b0a3cdb17614cc6a0b', '110114004', '沙河镇');
INSERT INTO `dic_region` VALUES ('70cb9e7881f14b6b9f3a55b927ff41ab', '110114115', '北七家镇');
INSERT INTO `dic_region` VALUES ('f06a7e772fce413d856ca3b31835770c', '110114110', '小汤山镇');

-- ----------------------------
-- Table structure for t_org_info
-- ----------------------------
DROP TABLE IF EXISTS `t_org_info`;
CREATE TABLE `t_org_info`  (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '编号',
  `shxydm` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '社会信用代码',
  `qymc` varchar(100) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '企业名称',
  `hylx` varchar(60) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业名称',
  `qylx` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '企业类型名称',
  `jxmc` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '街乡名称',
  `zcdz` varchar(200) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '注册地址',
  `jydz` varchar(200) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '经营地址',
  PRIMARY KEY (`id`) USING BTREE,
  INDEX `idex_summary_org_name`(`qymc`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of t_org_info
-- ----------------------------
INSERT INTO `t_org_info` VALUES ('16bd1e57a63e437db987194d9ff4c10d', '91110114MA006F5TXU', '北京君紫优创基金管理有限公司', '其他非公开募集证券投资基金', '有限责任公司（自然人投资或控股）', '沙河镇', '沙河镇能源东路1号院1号楼3层305-1', '沙河镇能源东路1号院1号楼3层305-1');
INSERT INTO `t_org_info` VALUES ('18a3c05538454dc8b2cfb51d6367b0bd', '91110114553050040D', '北京科博力工贸有限公司', '其他人造板制造', '其他有限责任公司', '北七家镇', '西沙各庄34号', '西沙各庄34号');
INSERT INTO `t_org_info` VALUES ('5b81bc2700104c34b50ab14d62cb9155', '91110114MA01QFK168', '北京中公未来教育科技有限公司', '其他技术推广服务', '有限责任公司（自然人独资）', '小汤山镇', '北京市昌平区未来科学城英才北三街16号院15号楼2单元401-6室', '北京市昌平区未来科学城英才北三街16号院15号楼2单元401-6室');
INSERT INTO `t_org_info` VALUES ('65a188298b224c9f9255e7c954c2f294', '911101147226653000', '北京中石化井田工程建设有限公司', '工程和技术研究和试验发展', '有限责任公司（自然人投资或控股）', '沙河镇', '北京市昌平区沙河镇丰善村委会', '北京市昌平区沙河镇丰善村委会');
INSERT INTO `t_org_info` VALUES ('7215d8f524144177975aa0261c7c74ac', '1211010273512264XT', '北京师范大学亚太实验学校', '普通高中教育', '有限责任公司（自然人独资）', '北七家镇', '北京市昌平区八仙庄', '北京市昌平区八仙庄');
INSERT INTO `t_org_info` VALUES ('8f5c246b18604d44851ea031ed14d7f8', '91110114097178760B', '北京远腾置业有限公司', '房地产开发经营', '有限责任公司（法人独资）', '北七家镇', '北京市昌平区未来科技城定泗路237号都市绿洲210', '北京市昌平区未来科技城定泗路237号都市绿洲210');
INSERT INTO `t_org_info` VALUES ('905b41554cfd4df886d640d0429316f1', '91110114787775909K', '伊特克斯惰性气体系统（北京）有限公司', '实验分析仪器制造', '有限责任公司（自然人投资或控股）', '沙河镇', '北京市昌平区沙河镇昌平路97号7幢409(昌平示范园)', '科星西路106号院5号楼8层809');
INSERT INTO `t_org_info` VALUES ('aae0ad204a194c0b991b180658e47a2e', '91110000717825595Q', '国网能源研究院有限公司 ', '工程和技术研究和试验发展', '有限责任公司（自然人投资或控股）', '小汤山镇', '北京市昌平区未来科技城滨河大道18号A栋三、四层', '北京市昌平区未来科技城滨河大道18号A栋三、四层');
INSERT INTO `t_org_info` VALUES ('ec4c6c45d64c4a73b667644b0147c37b', '91110114MA01AE1X7E', '北京华中润贸易有限公司', '金属及金属矿批发', '有限责任公司（自然人独资）', '沙河镇', '站前路11号3号楼8321室', '站前路11号3号楼8321室');
INSERT INTO `t_org_info` VALUES ('f9ebe98eae554a579872461dc71f782e', '91110114MA00AC8A7H', '北京卯申商贸有限公司', '盐及调味品批发', '内资', '沙河镇', '沙河镇昌平路97号5幢516室', '沙河镇昌平路97号5幢516室');

-- ----------------------------
-- Table structure for t_summary_org_info
-- ----------------------------
DROP TABLE IF EXISTS `t_summary_org_info`;
CREATE TABLE `t_summary_org_info`  (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '编号',
  `shxydm` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '社会信用代码',
  `qymc` varchar(100) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '企业名称',
  `hylx` varchar(60) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业名称',
  `qylx` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '企业类型名称',
  `jxmc` varchar(50) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '街乡名称',
  `zcdz` varchar(200) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '注册地址',
  `jydz` varchar(200) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '经营地址',
  `zzjgdm` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '组织机构代码',
  `qylx_dl_id` varchar(100) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '企业大类id',
  `qylx_dl_mc` varchar(100) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '企业大类名称',
  `hylx_ml_id` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业门类id',
  `hylx_ml_name` varchar(100) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业门类名称',
  `hylx_dl_id` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业大类id',
  `hylx_dl_name` varchar(100) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业大类名称',
  `hylx_zl_id` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业中类id',
  `hylx_zl_name` varchar(100) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业中类名称',
  `hylx_xl_id` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业细类id',
  `hylx_xl_name` varchar(100) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '行业细类名称',
  `jx_id` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '街乡id',
  `sjrq` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '数据日期',
  `tax` decimal(18, 2) NULL DEFAULT NULL COMMENT '税收',
  `qjsr` decimal(18, 2) NULL DEFAULT NULL COMMENT '区级收入',
  `tax_l` decimal(18, 2) NULL DEFAULT NULL COMMENT '累计税收',
  `qjsr_l` decimal(18, 2) NULL DEFAULT NULL COMMENT '累计区级收入',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of t_summary_org_info
-- ----------------------------
INSERT INTO `t_summary_org_info` VALUES ('8a3780dff5f111ed9bbbd7614c4130be', '91110114MA006F5TXU', '北京君紫优创基金管理有限公司', '其他非公开募集证券投资基金', '有限责任公司（自然人投资或控股）', '沙河镇', '沙河镇能源东路１号院１号楼３层３０５－１', '沙河镇能源东路１号院１号楼３层３０５－１', 'MA006F5TX', '100', '内资企业', 'J', '金融业', '67', '资本市场服务', '673', '非公开募集证券投资基金', '6739', '其他非公开募集证券投资基金', '110114004', '202302', 23003.46, 19040.40, 23003.46, 19040.40);
INSERT INTO `t_summary_org_info` VALUES ('8a37a7f0f5f111ed9bbbd7614c4130be', '91110114553050040D', '北京科博力工贸有限公司', '其他人造板制造', '其他有限责任公司', '北七家镇', '西沙各庄３４号', '西沙各庄３４号', '553050040', '100', '内资企业', 'C', '制造业', '20', '木材加工和木、竹、藤、棕、草制品业', '202', '人造板制造', '2029', '其他人造板制造', '110114115', '202302', 181378.26, 38159.94, 181378.26, 38159.94);
INSERT INTO `t_summary_org_info` VALUES ('8a37cf01f5f111ed9bbbd7614c4130be', '91110114MA01QFK168', '北京中公未来教育科技有限公司', '其他技术推广服务', '有限责任公司（自然人独资）', '小汤山镇', '北京市昌平区未来科学城英才北三街１６号院１５号楼２单元４０１－６室', '北京市昌平区未来科学城英才北三街１６号院１５号楼２单元４０１－６室', 'MA01QFK16', '100', '内资企业', 'M', '科学研究和技术服务业', '75', '科技推广和应用服务业', '751', '技术推广服务', '7519', '其他技术推广服务', NULL, '202302', 5743300.50, 1784019.51, 5743300.50, 1784019.51);
INSERT INTO `t_summary_org_info` VALUES ('8a37f612f5f111ed9bbbd7614c4130be', '911101147226653000', '北京中石化井田工程建设有限公司', '工程和技术研究和试验发展', '有限责任公司（自然人投资或控股）', '沙河镇', '北京市昌平区沙河镇丰善村委会', '北京市昌平区沙河镇丰善村委会', '722665300', '100', '内资企业', 'M', '科学研究和技术服务业', '73', '研究和试验发展', '732', '工程和技术研究和试验发展', '7320', '工程和技术研究和试验发展', '110114004', '202302', 2438230.23, 1083365.01, 3976383.01, 1171752.60);
INSERT INTO `t_summary_org_info` VALUES ('8a381d23f5f111ed9bbbd7614c4130be', '1211010273512264XT', '北京师范大学亚太实验学校', '普通高中教育', '有限责任公司（自然人独资）', '北七家镇', '北京市昌平区八仙庄', '北京市昌平区八仙庄', '73512264X', '100', '内资企业', 'P', '教育', '83', '教育', '833', '中等教育', '8334', '普通高中教育', '110114115', '202302', 195160.41, 11646.18, 195160.41, 11646.18);
INSERT INTO `t_summary_org_info` VALUES ('8a386b44f5f111ed9bbbd7614c4130be', '91110114097178760B', '北京远腾置业有限公司', '房地产开发经营', '有限责任公司（法人独资）', '北七家镇', '北京市昌平区未来科技城定泗路２３７号都市绿洲２１０', '北京市昌平区未来科技城定泗路２３７号都市绿洲２１０', '097178760', '100', '内资企业', 'K', '房地产业', '70', '房地产业', '701', '房地产开发经营', '7010', '房地产开发经营', '110114115', '202302', 394110.45, 73133.97, 394110.45, 73133.97);
INSERT INTO `t_summary_org_info` VALUES ('8a38b965f5f111ed9bbbd7614c4130be', '91110114787775909K', '伊特克斯惰性气体系统（北京）有限公司', '实验分析仪器制造', '有限责任公司（自然人投资或控股）', '沙河镇', '北京市昌平区沙河镇昌平路９７号７幢４０９（昌平示范园）', '科星西路１０６号院５号楼８层８０９', '787775909', '100', '内资企业', 'C', '制造业', '40', '仪器仪表制造业', '401', '通用仪器仪表制造', '4014', '实验分析仪器制造', '110114004', '202302', 45268.92, 38172.80, 49793.04, 40825.60);
INSERT INTO `t_summary_org_info` VALUES ('8a38e076f5f111ed9bbbd7614c4130be', '91110000717825595Q', '国网能源研究院有限公司', '工程和技术研究和试验发展', '有限责任公司（自然人投资或控股）', '小汤山镇', '北京市昌平区未来科技城滨河大道１８号Ａ栋三、四层', '北京市昌平区未来科技城滨河大道１８号Ａ栋三、四层', '717825595', '100', '内资企业', 'M', '科学研究和技术服务业', '73', '研究和试验发展', '732', '工程和技术研究和试验发展', '7320', '工程和技术研究和试验发展', NULL, '202302', 2151454.50, 292882.59, 2151454.50, 292882.59);
INSERT INTO `t_summary_org_info` VALUES ('8a392e97f5f111ed9bbbd7614c4130be', '91110114MA01AE1X7E', '北京华中润贸易有限公司', '金属及金属矿批发', '有限责任公司（自然人独资）', '沙河镇', '站前路１１号３号楼８３２１室', '站前路１１号３号楼８３２１室', 'MA01AE1X7', '100', '内资企业', 'F', '批发和零售业', '51', '批发业', '516', '矿产品、建材及化工产品批发', '5164', '金属及金属矿批发', '110114004', '202302', 7387.38, 6381.38, 7387.38, 6381.38);
INSERT INTO `t_summary_org_info` VALUES ('8a3955a8f5f111ed9bbbd7614c4130be', '91110114MA00AC8A7H', '北京卯申商贸有限公司', '盐及调味品批发', '内资', '沙河镇', '沙河镇昌平路９７号５幢５１６室', '沙河镇昌平路９７号５幢５１６室', 'MA00AC8A7', NULL, NULL, 'F', '批发和零售业', '51', '批发业', '512', '食品、饮料及烟草制品批发', '5125', '盐及调味品批发', '110114004', '202302', 20987.17, 23764.67, 20987.17, 23764.67);

-- ----------------------------
-- Table structure for t_tax
-- ----------------------------
DROP TABLE IF EXISTS `t_tax`;
CREATE TABLE `t_tax`  (
  `id` varchar(32) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL COMMENT '唯一编号',
  `shxydm` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '社会信用代码',
  `tax` decimal(18, 4) NULL DEFAULT NULL COMMENT '税收',
  `qjsr` decimal(18, 4) NULL DEFAULT NULL COMMENT '区级收入',
  `sjrq` varchar(6) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '数据日期',
  `zzjgdm` varchar(30) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL COMMENT '组织机构代码',
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci COMMENT = '税务税收规范表' ROW_FORMAT = Dynamic;

-- ----------------------------
-- Records of t_tax
-- ----------------------------
INSERT INTO `t_tax` VALUES ('057675ece2594b2f81123b16e7c4df5a', '91110114787775909K', 4524.1200, 2652.8000, '202301', NULL);
INSERT INTO `t_tax` VALUES ('057675ece2594b2f81a18b16e7c4df5a', '91110114787775909K', 45268.9200, 38172.8000, '202302', NULL);
INSERT INTO `t_tax` VALUES ('3ee464202d744abb9397f5e4560d0678', '911101147226653000', 1538152.7800, 88387.5900, '202301', NULL);
INSERT INTO `t_tax` VALUES ('3ee464202d744abb9397f5e5750d0678', '911101147226653000', 2438230.2300, 1083365.0100, '202302', NULL);
INSERT INTO `t_tax` VALUES ('47ab39be057b4d1fb2c14f65fd050b42', '91110114553050040D', 181378.2600, 38159.9400, '202302', NULL);
INSERT INTO `t_tax` VALUES ('5ee88e585e6443f980d8d83a733567d5', '91110114097178760B', 394110.4500, 73133.9700, '202302', NULL);
INSERT INTO `t_tax` VALUES ('8c6ab09758354a3888c7231fc80d44df', '91110114MA006F5TXU', 23003.4600, 19040.4000, '202302', NULL);
INSERT INTO `t_tax` VALUES ('8e42dd29bf4c4c16a4fda2b0e8ede857', '91110000717825595Q', 2151454.5000, 292882.5900, '202302', NULL);
INSERT INTO `t_tax` VALUES ('9904c1e816b143c8bcf8301c0bfdfb4d', '91110114MA00AC8A7H', 20987.1700, 23764.6700, '202302', NULL);
INSERT INTO `t_tax` VALUES ('a8a63c1005a54aaab0d9a117385610cf', '91110114MA01AE1X7E', 7387.3800, 6381.3800, '202302', NULL);
INSERT INTO `t_tax` VALUES ('cac1253bf7e449d79c6dec4a319231d3', '91110114MA01QFK168', 5743300.5000, 1784019.5100, '202302', NULL);
INSERT INTO `t_tax` VALUES ('d4abf94c6146472c95d59b7f04913fff', '1211010273512264XT', 195160.4100, 11646.1800, '202302', NULL);

SET FOREIGN_KEY_CHECKS = 1;
