clear all;
close all;
clc;

T = csvread('Part8TumbleTestData.csv');

accData = T(:,1:3);
magData = T(:,4:6);


[accA, accB] = CalibrateEllipsoidData3D(accData(:,1),accData(:,2),accData(:,3),20,0)
[magA, magB] = CalibrateEllipsoidData3D(magData(:,1),magData(:,2),magData(:,3),20,0)

mi = [0; 0; 1];
si = [22.768; 5.313; 41.5083];
Rmishat_0 = eye(3);

[Rmis, Pbody] = AlignMasterSlave(accData',magData',mi,si,Rmishat_0,1000);
