clear all;
close all;

Part5Bias;

xTable = textread('Part5_1h_xGyro.txt');
yTable = textread('Part5_1h_yGyro.txt');
zTable = textread('Part5_1h_zGyro.txt');

xScaledTable = (xTable - xAverage)/131;
yScaledTable = (yTable - yAverage)/131;
zScaledTable = (zTable - zAverage)/131;

xInt = cumtrapz(linspace(0,3600,1048448), xScaledTable);
yInt = cumtrapz(linspace(0,3600,1048021), yScaledTable);
zInt = cumtrapz(linspace(0,3600,1047226), zScaledTable);

t = linspace(0,3600,1048448);
t2 = linspace(0,3600,1048021);
t3 = linspace(0,3600,1047226);

plot(t, xInt)
hold on
plot(t2, yInt)
hold on
plot(t3, zInt)

legend('x','y','z')