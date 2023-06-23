clear all;
close all;

Part6Bias;

xRun = csvread('Part 6 20s xGyro.csv');
yRun = csvread('Part 6 20s yGyro.csv');
zRun = csvread('Part 6 20s zGyro.csv');

xTable = xRun(:,1);
yTable = yRun(:,2);
zTable = zRun(:,3);

xScaledTable = (xTable - xAverage)/131;
yScaledTable = (yTable - yAverage)/131;
zScaledTable = (zTable - zAverage)/131;

xInt = cumtrapz(linspace(0,20,6994), xScaledTable);
yInt = cumtrapz(linspace(0,20,5614), yScaledTable);
zInt = cumtrapz(linspace(0,20,5206), zScaledTable);

xInt = xInt * (180/max(abs(xInt)));
yInt = yInt * (180/max(abs(yInt)));
zInt = zInt * (180/max(abs(zInt)));

t = linspace(0,20,6994);
t2 = linspace(0,20,5614);
t3 = linspace(0,20,5206);

plot(t, xInt)
hold on
plot(t2, yInt)
hold on
plot(t3, zInt)

legend('x','y','z')