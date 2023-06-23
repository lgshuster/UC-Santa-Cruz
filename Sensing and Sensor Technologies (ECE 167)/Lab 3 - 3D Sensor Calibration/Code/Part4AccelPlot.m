clear all;
close all;

xAxis = csvread('Part4AccelDataX.csv');
yAxis = csvread('Part4AccelDataY.csv');
zAxis = csvread('Part4AccelDataZ.csv');

xavg = mean(xAxis)

gPos = repmat(9.80665, 1000, 1);
gNeg = repmat(-9.80665, 1000, 1);

gExt = vertcat(gPos, gNeg);

Mx = [xAxis ones(size(xAxis))];
My = [yAxis ones(size(yAxis))];
Mz = [zAxis ones(size(zAxis))];

kx = Mx \ gExt
ky = My \ gExt
kz = Mz \ gExt