set names utf8;
#初始化数据库
drop database if exists `goatdb`;
create database goatdb;
use goatdb;

# 羊舍
create table houseInfo(houseId varchar(100) primary key);

# 奶山羊信息表
create table goatInfo(id int primary key auto_increment, goatId varchar(40) not null unique,houseId varchar(100) not null, weight float, inTime datetime, outTime datetime);

# 设备信息表
create table deviceInfo(id  int primary key auto_increment,deviceId varchar(40) not null unique,deviceState varchar(20), inTime datetime);
create table houseDeviceInfo(id  int primary key auto_increment,deviceId varchar(40) not null unique,deviceState varchar(20), inTime datetime);

# 绑定信息表
create table bindingInfo(bindingId int primary key auto_increment,goatId varchar(40) not null unique, deviceId varchar(40) not null unique);
create table houseBindingInfo(id int primary key auto_increment,houseId varchar(100) not null,deviceId varchar(40) not null unique);
# 路由设备表
create table routerInfo(id int primary key auto_increment,deviceId varchar(40) not null unique,deviceState varchar(20),inTime datetime);

# 运动数据表
create table sportData(id int primary key auto_increment,goatId varchar(40) not null,datatimem varchar(20) not null,sportx varchar(20) not null,sporty varchar(20) not null, sportz varchar(20) not null,anglex varchar(20),angley varchar(20),anglez varchar(20),status varchar(40));

# 羊舍数据表
#create table houseData(id int primary Key auto_increment,houseId varchar(100) not null,datatimem varchar(20) not null,wendu varchar(20),anqi varchar(20),shidu varchar(20),eryang varchar(20));
create table houseData(datatimem varchar(20) not null,houseId varchar(100) not null,wendu varchar(20),anqi varchar(20),shidu varchar(20),eryang varchar(20),guangzhao varchar(20),pm25 varchar(20),pm10 varchar(20),yanwu varchar(20), primary key(datatimem,houseId));

# 疫苗信息表
create table vacineInfo(vacineId varchar(40) primary key,vacineName varchar(40) not null, vacineType varchar(200) not null,vacineTime varchar(100),vacineDose varchar(100),vacinePart varchar(100), vacineRemark varchar(200));

#防疫信息表
create table antiepidemicData(id int primary key auto_increment,goatId varchar(40) not null, vacineId varchar(40) not null,inTime datetime,antiepidemicRemark varchar(200));

# 饲料信息表
create table feedInfo(feedId varchar(40) primary key, feedName varchar(40),feedRange varchar(200),feedUsage varchar(200),feedRemark varchar(200));

# 饲喂信息表
create table feedingData(id int primary key auto_increment,goatId varchar(40) not null,feedId varchar(40) not null,feedLevel varchar(40),inTime datetime,feedingRemark varchar(200));

# 产品信息表
create table productInfo(productId varchar(40) primary key, productName varchar(40),productRemark varchar(200));

# 产量信息表
create table yieldData(id int primary key auto_increment,goatId varchar(40) not null,productId varchar(40) not null,yield varchar(40) not null,outTime datetime, yieldRemark varchar(200));

# 事件信息表
create table eventInfo(eventId varchar(20) primary key, eventMeaning varchar(200) not null);

# 事件记录表
create table eventData(id int primary key auto_increment,routerId varchar(40) not null,datatimem varchar(20) not null,eventId varchar(20) not null,deviceId varchar(40) not null, eventState enum('未处理','已处理','已知晓'));
# 外键约束
alter table goatInfo add constraint fk_houseId foreign key(houseId) references houseInfo(houseId) on delete cascade on update cascade;

alter table bindingInfo add constraint fk_goatId foreign key(goatId) references goatInfo(goatId) on delete cascade on update cascade;
alter table bindingInfo add constraint fk_deviceId foreign key(deviceId) references deviceInfo(deviceId) on delete cascade on update cascade;

alter table houseBindingInfo add constraint fk_houseId_to_houseBingdingInfo foreign key(houseId) references houseInfo(houseId) on delete cascade on update cascade;
alter table houseBindingInfo add constraint fk_deviceId_to_houseBindignInfo foreign key(deviceId) references houseDeviceInfo(deviceId) on delete cascade on update cascade;

alter table antiepidemicData add constraint fk_goatId_to_antiepidemicData foreign key(goatId) references goatInfo(goatId) on delete cascade on update cascade;
alter table antiepidemicData add constraint fk_vacineId_to_antiepidemicData foreign key(vacineId) references vacineInfo(vacineId) on delete cascade on update cascade;

alter table feedingData add constraint fk_goatId_to_feedingData foreign key(goatId) references goatInfo(goatId) on delete cascade on update cascade;
alter table feedingData add constraint fk_feedId_to_feedingData foreign key(feedId) references feedInfo(feedId) on delete cascade on update cascade;

alter table yieldData add constraint fk_goatId_to_yieldData foreign key(goatId) references goatInfo(goatId) on delete cascade on update cascade;
alter table yieldData add constraint fk_productId_to_yieldData foreign key(productId) references productInfo(productId) on delete cascade on update cascade;


#触发器
delimiter ||
#create trigger bf_binding before insert
#on bindingInfo for each row
#begin
#  delete from bindingInfo where goatId = NEW.goatId;
#  delete from bindingInfo where deviceId = NEW.deviceId;
#end||

create trigger bf_antipidemic before insert
on antiepidemicData for each row
begin
  if isnull(NEW.inTime) then
    set NEW.inTime = now();
  end if;
end||

create trigger bf_feeding before insert
on feedingData for each row
begin
  if isnull(NEW.inTime) then
    set NEW.inTime = now();
  end if;
end||

create trigger bf_yieldData before insert
on yieldData for each row
begin
  if isnull(NEW.outTime) then
    set NEW.outTime = now();
  end if;
end||

create trigger af_binding after insert
on bindingInfo for each row
begin
  update deviceInfo set deviceState = '已绑定' where deviceId = NEW.deviceId;
end||

create trigger af_binding_update after update
on bindingInfo for each row
begin
  update deviceInfo set deviceState = '闲置' where deviceId = OLD.deviceId;
  update deviceInfo set deviceState = '已绑定' where deviceId = NEW.deviceId;
end||

#create trigger af_device_update after update
#on deviceInfo for each row
#begin
#  if NEW.deviceState = '故障' then
#  delete from bindingInfo where deviceId = NEW.deviceId;
#  end if;
#end||

create trigger bf_del_deivce before delete
on goatInfo for each row
begin
  update deviceInfo set deviceState = '闲置' where deviceInfo.deviceId in (select bindingInfo.deviceId from bindingInfo where bindingInfo.goatId = old.goatId);
end||

create trigger bf_del_binding before delete
on bindingInfo for each row
begin
  update deviceInfo set deviceState = '闲置' where deviceId = old.deviceId;
end||




########################
create trigger af_house_binding after insert
on houseBindingInfo for each row
begin
  update houseDeviceInfo set deviceState = '已绑定' where deviceId = NEW.deviceId;
  update houseData set houseId = NEW.houseId where houseId = NEW.deviceId;
  update eventData set routerId = NEW.houseId where routerId = NEW.deviceId;
end||

create trigger af_house_binding_update after update
on houseBindingInfo for each row
begin
  update houseDeviceInfo set deviceState = '闲置' where deviceId = OLD.deviceId;
  update houseDeviceInfo set deviceState = '已绑定' where deviceId = NEW.deviceId;
  update houseData set houseId = NEW.houseId where houseId = NEW.deviceId;
  update eventData set routerId = NEW.houseId where routerId = NEW.deviceId;
end||

#create trigger af_device_update after update
#on deviceInfo for each row
#begin
#  if NEW.deviceState = '故障' then
#  delete from bindingInfo where deviceId = NEW.deviceId;
#  end if;
#end||

create trigger bf_del_house_deivce before delete
on houseInfo for each row
begin
  update houseDeviceInfo set deviceState = '闲置' where houseDeviceInfo.deviceId in (select houseBindingInfo.deviceId from houseBindingInfo where houseBindingInfo.houseId = old.houseId);
end||

create trigger bf_del_house_binding before delete
on houseBindingInfo for each row
begin
  update houseDeviceInfo set deviceState = '闲置' where deviceId = old.deviceId;
end||

create trigger bf_insert_houseDeviceInfo before insert
on houseDeviceInfo for each row
begin
    if(NEW.inTime is null) then
        set NEW.inTime = NOW();
    end if;
    if(NEW.deviceState is null) then
        set NEW.deviceState = '闲置';
    end if;
end||


########################

create trigger bf_insert_sportData before insert
on sportData for each row
begin
  set @tempGoatId = (select goatId from bindingInfo where deviceId = NEW.goatId);
  set @gg = sqrt(power(NEW.sportx,2)+power(NEW.sporty,2)+power(NEW.sportz,2))/17884*9.8;
  if(@tempGoatId is not null) then
    set NEW.goatId = @tempGoatId;
  elseif not exists(select deviceId from deviceInfo where deviceId = NEW.goatId) then
    insert into deviceInfo(deviceId,deviceState,inTime) values(NEW.goatId,'闲置',now());
  end if;
   if @gg >= 15 then
	set NEW.status = '奔跑';
  elseif @gg >= 10.4 then
	set NEW.status = '缓步';
  else
	set NEW.status = '静止';
  end if;
end||

create trigger bf_insert_housetData before insert
on houseData for each row
begin
  set @tempHouseId = (select houseId from houseBindingInfo where deviceId = NEW.houseId);
  if(@tempHouseId is not null) then
    set NEW.houseId = @tempHouseId;
  end if;
end||

create trigger bf_insert_eventData before insert
on eventData for each row
begin
    set @tempDeviceId = (select deviceId from houseDeviceInfo where deviceId = NEW.routerId);
    if(@tempDeviceId is null) then
        insert into houseDeviceInfo(deviceId) values(NEW.routerId);
    end if;
    set @tempSensorId = (select deviceId from houseDeviceInfo where deviceId = NEW.deviceId);
    if(@tempSensorId is null) then
        insert into houseDeviceInfo(deviceId) values(NEW.deviceId);
    end if;
    if New.eventId = '0' then
        set New.eventState = '已处理';
    else
        set NEW.eventState = '未处理';
    end if;
    set @tempHouseId = (select houseId from houseBindingInfo where deviceId = NEW.routerId);
    if(@tempHouseId is not null) then
        set NEW.routerId = @tempHouseId;
    end if;
end||

delimiter ;
alter table sportData add index(datatimem);
alter table houseDeviceInfo add index(deviceId);
#alter table houseBindingInfo add index(houseId);
#alter table houseBindingInfo add index(deviceId);
#alter table houseData add index(datatimem);
create view tempsportDataView as select a.id,a.goatId,a.datatimem,a.sportx,a.sporty,a.sportz,a.status,b.houseId from sportData a left join goatInfo b on a.goatId = b.goatId where a.datatimem in (select distinct max(datatimem) from sportData);
#create view houseDataView as select * from houseData where datatimem in (select max(datatimem) from houseData);
create view maxDatatimemView as select goatId,max(datatimem) as datatimem from tempsportDataView group by goatId;
create view sportDataView as select a.goatId,a.datatimem,b.sportx,b.sporty,b.sportz,b.status,b.houseId from maxDatatimemView a left join tempsportDataView b on a.goatId = b.goatId and a.datatimem = b.datatimem;

#create view tempsportDataView as select a.id,a.goatId,a.datatimem,a.sportx,a.sporty,a.sportz,a.status,b.houseId from sportData a left join goatInfo b on a.goatId = b.goatId where a.datatimem in (select max(datatimem) from sportData group by goatId );
#create view houseDataView as select * from houseData where datatimem in (select max(datatimem) from houseData);
#create view maxDatatimemView as select goatId,max(datatimem) as datatimem from tempsportDataView group by goatId;
#create view sportDataView as select a.goatId,a.datatimem,b.sportx,b.sporty,b.sportz,b.status,b.houseId from maxDatatimemView a left join tempsportDataView b on a.goatId = b.goatId and a.datatimem = b.datatimem;
#
