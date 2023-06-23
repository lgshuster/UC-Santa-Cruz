T = csvread('Part5Bias.csv');

xAxis = T(:,1);
yAxis = T(:,2);
zAxis = T(:,3);

xAverage = mean(xAxis)
yAverage = mean(yAxis)
zAverage = mean(zAxis)
