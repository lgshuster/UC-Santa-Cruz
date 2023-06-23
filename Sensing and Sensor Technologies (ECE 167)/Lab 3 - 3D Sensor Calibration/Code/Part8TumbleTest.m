clear all;
close all;

T = csvread('Part8TumbleTestData.csv');

%%%%%%%%%%% ACC DATA %%%%%%%%%%
accDataX = T(:,1);
accDataY = T(:,2);
accDataZ = T(:,3);

accBiasX = mean(accDataX);
accBiasY = mean(accDataY);
accBiasZ = mean(accDataZ);

accDataXScaleFactor = (2*9.80665)/(max(accDataX)-min(accDataX));
accDataYScaleFactor = (2*9.80665)/(max(accDataY)-min(accDataY));
accDataZScaleFactor = (2*9.80665)/(max(accDataZ)-min(accDataZ));

accDataXScaled = accDataX*accDataXScaleFactor;
accDataYScaled = accDataY*accDataYScaleFactor;
accDataZScaled = accDataZ*accDataZScaleFactor;

accDataPreCalibNorm = sqrt(accDataXScaled.^2 + accDataYScaled.^2 + accDataZScaled.^2);
accDataPreCalibAvg = mean(accDataPreCalibNorm)
accDataPreCalibStd = std(accDataPreCalibNorm)

accDataXCalib1 = accDataXScaled*0.0006 - 0.2613;
accDataYCalib1 = accDataYScaled*0.0006 - 0.1297;
accDataZCalib1 = accDataZScaled*0.0006 - 0.0025;

accDataPostCalibNorm1 = sqrt(accDataXCalib1.^2 + accDataYCalib1.^2 + accDataZCalib1.^2);
accDataPostCalibAvg1 = mean(accDataPostCalibNorm1)
accDataPostCalibStd1 = std(accDataPostCalibNorm1)


[accA, accB] = CalibrateEllipsoidData3D(accDataXScaled,accDataYScaled,accDataZScaled,20,0);
[accPostCalibDataX,accPostCalibDataY,accPostCalibDataZ] = CorrectEllipsoidData3D(accDataXScaled,accDataYScaled,accDataZScaled,accA,accB);

accDataPostCalibNorm2 = sqrt(accPostCalibDataX.^2 + accPostCalibDataY.^2 + accPostCalibDataZ.^2);
accDataPostCalibAvg2 = mean(accDataPostCalibNorm2)
accDataPostCalibStd2 = std(accDataPostCalibNorm2)











%%%%%%%%%%% MAG DATA %%%%%%%%%%
magDataX = T(:,4);
magDataY = T(:,5);
magDataZ = T(:,6);

magBiasX = mean(magDataX);
magBiasY = mean(magDataY);
magBiasZ = mean(magDataZ);

magDataXScaleFactor = (2*0.4784)/(max(magDataX)-min(magDataX));
magDataYScaleFactor = (2*0.4784)/(max(magDataY)-min(magDataY));
magDataZScaleFactor = (2*0.4784)/(max(magDataZ)-min(magDataZ));

magDataXScaled = magDataX*magDataXScaleFactor;
magDataYScaled = magDataY*magDataYScaleFactor;
magDataZScaled = magDataZ*magDataZScaleFactor;

magDataPreCalibNorm = sqrt(magDataXScaled.^2 + magDataYScaled.^2 + magDataZScaled.^2);
magDataPreCalibAvg = mean(magDataPreCalibNorm)
magDataPreCalibStd = std(magDataPreCalibNorm)

magDataXCalib1 = magDataXScaled*0.0054 - 1.9114;
magDataYCalib1 = magDataYScaled*-0.0027 - 0.5226;
magDataZCalib1 = magDataZScaled*-0.0036 - 0.1272;

magDataPostCalibNorm1 = sqrt(magDataXCalib1.^2 + magDataYCalib1.^2 + magDataZCalib1.^2);
magDataPostCalibAvg1 = mean(magDataPostCalibNorm1)
magDataPostCalibStd1 = std(magDataPostCalibNorm1)


[magA, magB] = CalibrateEllipsoidData3D(magDataXScaled,magDataYScaled,magDataZScaled,20,0);
[magPostCalibDataX,magPostCalibDataY,magPostCalibDataZ] = CorrectEllipsoidData3D(magDataXScaled,magDataYScaled,magDataZScaled,magA,magB);

magDataPostCalibNorm2 = sqrt(magPostCalibDataX.^2 + magPostCalibDataY.^2 + magPostCalibDataZ.^2);
magDataPostCalibAvg2 = mean(magDataPostCalibNorm2)
magDataPostCalibStd2 = std(magDataPostCalibNorm2)






%%%%%%%%%%% PLOTTING %%%%%%%%%%


figure (1)

subplot(3,1,1);
plot(linspace(0,170,17220),accDataPreCalibNorm);
title('acc pre calibrated');

subplot(3,1,2);
plot(linspace(0,170,17220),accDataPostCalibNorm1);
title('acc post calibrated');

subplot(3,1,3);
plot(linspace(0,170,17220),accDataPostCalibNorm2);
title('acc post calibrated/corrected');





figure (2)
subplot(3,1,1);
plot(linspace(0,170,17220),magDataPreCalibNorm);
title('mag pre calibrated');

subplot(3,1,2);
plot(linspace(0,170,17220),magDataPostCalibNorm1);
title('mag post calibrated');

subplot(3,1,3);
plot(linspace(0,170,17220),magDataPostCalibNorm2);
title('mag post calibrated/corrected');






figure (3)
scatter3(accDataXScaled,accDataYScaled,accDataZScaled);
title('acc pre calib');

figure (4)
scatter3(accDataXCalib1,accDataYCalib1,accDataZCalib1);
title('acc post calib');

figure (5)
scatter3(accPostCalibDataX,accPostCalibDataY,accPostCalibDataZ);
title('acc post calib/correct');

figure (6)
scatter3(magDataXScaled,magDataYScaled,magDataZScaled);
title('mag pre calib');

figure (7)
scatter3(magDataXCalib1,magDataYCalib1,magDataZCalib1);
title('mag post calib');

figure (8)
scatter3(magPostCalibDataX,magPostCalibDataY,magPostCalibDataZ);
title('mag post calib/correct');



