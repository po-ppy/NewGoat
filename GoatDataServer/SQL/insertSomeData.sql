set names utf8;
use goatdb;
# 羊舍数据表中插入数据
insert into houseInfo(houseId) values('舍1');
insert into houseInfo(houseId) values('舍2');
insert into houseInfo(houseId) values('舍3');
insert into houseInfo(houseId) values('舍4');
insert into houseInfo(houseId) values('舍5');
insert into houseInfo(houseId) values('舍6');
# 在奶山羊数据表中插入某些数据
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10001','舍1',12.3,now(),now());
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10002','舍2',42.3,now(),now());
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10003','舍1',82.5,now(),now());
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10004','舍1',19.3,now(),now());
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10005','舍2',12.3,now(),now());
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10006','舍5',43.8,now(),now());
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10007','舍3',45.3,now(),now());
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10008','舍4',72.9,now(),now());

# 在设备信息表中插入数据
insert into deviceInfo(deviceId,deviceState,inTime) values('A&G:1','闲置',now());
insert into deviceInfo(deviceId,deviceState,inTime) values('A&G:2','闲置',now());
insert into deviceInfo(deviceId,deviceState,inTime) values('A&G:3','闲置',now());
insert into deviceInfo(deviceId,deviceState,inTime) values('A&G:4','闲置',now());
insert into deviceInfo(deviceId,deviceState,inTime) values('A&G:5','闲置',now());
insert into deviceInfo(deviceId,deviceState,inTime) values('A&G:6','闲置',now());
insert into deviceInfo(deviceId,deviceState,inTime) values('A&G:7','闲置',now());

# 在绑定信息表中插入数据
#insert into bindingInfo(goatId,deviceId) values('G10001','D10001');
#insert into bindingInfo(goatId,deviceId) values('G10002','D10002');
#insert into bindingInfo(goatId,deviceId) values('G10003','D10003');
#insert into bindingInfo(goatId,deviceId) values('G10004','D10004');
#insert into bindingInfo(goatId,deviceId) values('G10005','D10005');
#insert into bindingInfo(goatId,deviceId) values('G10006','D10006');

insert into bindingInfo(goatId,deviceId) values('G10001','A&G:1') on duplicate key update goatId = values(goatId),deviceId = values(deviceId);
insert into bindingInfo(goatId,deviceId) values('G10002','A&G:2') on duplicate key update goatId = values(goatId),deviceId = values(deviceId);
insert into bindingInfo(goatId,deviceId) values('G10003','A&G:3') on duplicate key update goatId = values(goatId),deviceId = values(deviceId);
insert into bindingInfo(goatId,deviceId) values('G10004','A&G:4') on duplicate key update goatId = values(goatId),deviceId = values(deviceId);
insert into bindingInfo(goatId,deviceId) values('G10005','A&G:5') on duplicate key update goatId = values(goatId),deviceId = values(deviceId);
insert into bindingInfo(goatId,deviceId) values('G10006','A&G:6') on duplicate key update goatId = values(goatId),deviceId = values(deviceId);

# 在疫苗信息表中插入数据
insert into vacineInfo(vacineId,vacineName,vacineType,vacineTime,vacineDose,vacinePart,vacineRemark) values('VC0001','羊三联四防灭活苗','快疫、猝狙、肠毒血症、羔羊痢疾','每年于2月底3月初和9月下旬分两次接种','1头份','皮下或肌肉注射', '不论羊只大小');
insert into vacineInfo(vacineId,vacineName,vacineType,vacineTime,vacineDose,vacinePart,vacineRemark) values('VC0002','羊痘弱毒疫苗','羊痘','每年3-4月份接种','1头份','皮下注射', '不论羊只大小');
insert into vacineInfo(vacineId,vacineName,vacineType,vacineTime,vacineDose,vacinePart,vacineRemark) values('VC0003','羊口蹄疫苗','羊口蹄疫','每年3月和9月', '1毫升/2毫升','皮下注射','4月龄-2年/2年以上');
insert into vacineInfo(vacineId,vacineName,vacineType,vacineTime,vacineDose,vacinePart,vacineRemark) values('VC0004','口疮弱毒细胞冻干苗','山羊口疮','每年3月和9月','0.2毫升','口腔粘膜内注射', '不论羊只大小');

# 在防疫信息表中插入数据
#insert into antiepidemicData(goatId,vacineId,inTime,antiepidemicRemark) values('G10001','VC0001',now(),'no remark') on duplicate key update goatId = values(goatId),vacineId = values(vacineId);
#insert into antiepidemicData(goatId,vacineId,antiepidemicRemark) values('G10001','VC0003','no remark') on duplicate key update goatId = values(goatId),vacineId = values(vacineId);

insert into antiepidemicData(goatId,vacineId,inTime,antiepidemicRemark) values('G10001','VC0001',now(),'no remark');
insert into antiepidemicData(goatId,vacineId,inTime,antiepidemicRemark) values('G10001','VC0002','2017-04-11 16:25:51','no remark');
insert into antiepidemicData(goatId,vacineId,antiepidemicRemark) values('G10002','VC0003','no remark');
insert into antiepidemicData(goatId,vacineId,antiepidemicRemark) values('G10003','VC0003','no remark');
insert into antiepidemicData(goatId,vacineId,antiepidemicRemark) values('G10004','VC0003','no remark');
insert into antiepidemicData(goatId,vacineId,antiepidemicRemark) values('G10005','VC0003','no remark');
insert into antiepidemicData(goatId,vacineId,antiepidemicRemark) values('G10002','VC0004','no remark');

# 在饲料信息表中插入数据
insert into feedInfo(feedId,feedName,feedRange,feedUsage,feedRemark) values('F0001','青饲料','1年龄及以上','适量','无备注');
insert into feedInfo(feedId,feedName,feedRange,feedUsage,feedRemark) values('F0002','精饲料','不限年龄','适量','无备注');
insert into feedInfo(feedId,feedName,feedRange,feedUsage,feedRemark) values('F0003','多汁饲料','5月龄及以上','适量','无备注');

# 在饲喂信息表中插入数据
insert into feedingData(goatId,feedId,feedLevel,inTime,feedingRemark) values('G10001','F0001','适量',now(),null);
insert into feedingData(goatId,feedId,feedLevel,inTime,feedingRemark) values('G10002','F0001','适量',now(),null);
insert into feedingData(goatId,feedId,feedLevel,inTime,feedingRemark) values('G10003','F0001','适量',now(),null);
insert into feedingData(goatId,feedId,feedLevel,inTime,feedingRemark) values('G10004','F0001','适量',now(),null);
insert into feedingData(goatId,feedId,feedLevel,inTime,feedingRemark) values('G10005','F0001','适量',now(),null);
insert into feedingData(goatId,feedId,feedLevel,inTime,feedingRemark) values('G10006','F0001','适量',now(),null);
insert into feedingData(goatId,feedId,feedLevel,inTime,feedingRemark) values('G10001','F0002','适量',now(),null);
insert into feedingData(goatId,feedId,feedLevel,inTime,feedingRemark) values('G10001','F0003','适量',now(),null);

# 在产品信息表中插入数据
insert into productInfo(productId,productName,productRemark) values('P0001','羊奶',null);
insert into productInfo(productId,productName,productRemark) values('P0002','羊肉',null);

# 在产量信息表中插入数据
insert into yieldData(goatId,productId,yield,outTime,yieldRemark) values('G10001','P0001','2升',now(),null);
insert into yieldData(goatId,productId,yield,outTime,yieldRemark) values('G10002','P0001','2升',now(),null);
insert into yieldData(goatId,productId,yield,outTime,yieldRemark) values('G10003','P0001','2升',now(),null);
insert into yieldData(goatId,productId,yield,outTime,yieldRemark) values('G10004','P0001','2升',now(),null);
insert into yieldData(goatId,productId,yield,outTime,yieldRemark) values('G10005','P0001','2升',now(),null);
insert into yieldData(goatId,productId,yield,outTime,yieldRemark) values('G10006','P0001','2升',now(),null);
insert into yieldData(goatId,productId,yield,outTime,yieldRemark) values('G10007','P0001','2升',now(),null);
insert into yieldData(goatId,productId,yield,outTime,yieldRemark) values('G10008','P0001','2升',now(),null);


