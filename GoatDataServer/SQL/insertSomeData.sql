set names utf8;
use goatdb;
#羊舍数据表中插入数据
insert into houseInfo(houseId) values('舍1');
insert into houseInfo(houseId) values('舍2');
insert into houseInfo(houseId) values('舍3');
insert into houseInfo(houseId) values('舍4');
insert into houseInfo(houseId) values('舍5');
insert into houseInfo(houseId) values('舍6');
#在奶山羊数据表中插入某些数据
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10001','舍1',12.3,now(),now());
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10002','舍2',42.3,now(),now());
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10003','舍1',82.5,now(),now());
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10004','舍1',19.3,now(),now());
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10005','舍2',12.3,now(),now());
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10006','舍5',43.8,now(),now());
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10007','舍3',45.3,now(),now());
insert into goatInfo(goatId,houseId,weight,inTime,outTime) values('G10008','舍4',72.9,now(),now());

#在设备信息表中插入数据
insert into deviceInfo(deviceId,deviceState,inTime) values('A&G:1','闲置',now());
insert into deviceInfo(deviceId,deviceState,inTime) values('A&G:2','闲置',now());
insert into deviceInfo(deviceId,deviceState,inTime) values('A&G:3','闲置',now());
insert into deviceInfo(deviceId,deviceState,inTime) values('A&G:4','闲置',now());
insert into deviceInfo(deviceId,deviceState,inTime) values('A&G:5','闲置',now());
insert into deviceInfo(deviceId,deviceState,inTime) values('A&G:6','闲置',now());
insert into deviceInfo(deviceId,deviceState,inTime) values('A&G:7','闲置',now());

#在绑定信息表中插入数据
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
