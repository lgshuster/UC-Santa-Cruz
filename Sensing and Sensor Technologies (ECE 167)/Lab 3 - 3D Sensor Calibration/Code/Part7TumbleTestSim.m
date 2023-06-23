clear all;
close all;

[accData, magData] = CreateTumbleData(1000);

%%%%%%%%% NAIVE CALIBRATION %%%%%%%%%
%%%%%%% ACC DATA %%%%%%%

accDataX = accData(:,1);
accDataY = accData(:,2);
accDataZ = accData(:,3);

% accPreNaiveNorm = sqrt(accDataX.^2 + accDataY.^2 + accDataZ.^2);
% accPreNaiveStd = std(accPreNaiveNorm)

accNaiveBiasX = mean(accDataX);
accNaiveScaleX = (2*9.80665)/(max(accDataX)-min(accDataX));

accNaiveBiasY = mean(accDataY);
accNaiveScaleY = (2*9.80665)/(max(accDataY)-min(accDataY));

accNaiveBiasZ = mean(accDataZ);
accNaiveScaleZ = (2*9.80665)/(max(accDataZ)-min(accDataZ));

% accNaiveDataX = (accDataX-accNaiveBiasX) / accNaiveScaleX;
% accNaiveDataY = (accDataY-accNaiveBiasY) / accNaiveScaleY;
% accNaiveDataZ = (accDataZ-accNaiveBiasZ) / accNaiveScaleZ;

accNaiveDataX = (accDataX-accNaiveBiasX) * accNaiveScaleX;
accNaiveDataY = (accDataY-accNaiveBiasY) * accNaiveScaleY;
accNaiveDataZ = (accDataZ-accNaiveBiasZ) * accNaiveScaleZ;

accPostNaiveNorm = sqrt(accNaiveDataX.^2 + accNaiveDataY.^2 + accNaiveDataZ.^2);
accPostNaiveAvg = mean(accPostNaiveNorm)
accPostNaiveStd = std(accPostNaiveNorm)

% %%%%%%% MAG DATA %%%%%%%

magDataX = magData(:,1);
magDataY = magData(:,2);
magDataZ = magData(:,3);

% magPreNaiveNorm = sqrt(magDataX.^2 + magDataY.^2 + magDataZ.^2);
% magPreNaiveStd = std(magPreNaiveNorm)

magNaiveBiasX = mean(magDataX);
magNaiveScaleX = (2*0.4784)/(max(magDataX)-min(magDataX));

magNaiveBiasY = mean(magDataY);
magNaiveScaleY = (2*0.4784)/(max(magDataY)-min(magDataY));

magNaiveBiasZ = mean(magDataZ);
magNaiveScaleZ = (2*0.4784)/(max(magDataZ)-min(magDataZ));

magNaiveDataX = (magDataX-magNaiveBiasX) * magNaiveScaleX;
magNaiveDataY = (magDataY-magNaiveBiasY) * magNaiveScaleY;
magNaiveDataZ = (magDataZ-magNaiveBiasZ) * magNaiveScaleZ;

% magNaiveDataX = magDataX*magNaiveScaleX + magNaiveBiasX;
% magNaiveDataY = magDataY*magNaiveScaleY + magNaiveBiasY;
% magNaiveDataZ = magDataZ*magNaiveScaleZ + magNaiveBiasZ;

magPostNaiveNorm = sqrt(magNaiveDataX.^2 + magNaiveDataY.^2 + magNaiveDataZ.^2);
magPostNaiveAvg = mean(magPreEllipseNorm)
magPostNaiveStd = std(magPostNaiveNorm)

















%%%%%%%% ELLIPSOID CALLIBRATION %%%%%%%%%%%

%%%%%%%% ACC DATA %%%%%%%%

% accEllipseBiasX = mean(accDataX);
% accEllipseScaleX = 9.80665/(max(abs(accDataX)));
% 
% accEllipseBiasY = mean(accDataY);
% accEllipseScaleY = 9.80665/(max(abs(accDataY)));
% 
% accEllipseBiasZ = mean(accDataZ);
% accEllipseScaleZ = 9.80665/(max(abs(accDataZ)));
% 
% accEllipseDataX = (accDataX-accEllipseBiasX) * accEllipseScaleX;
% accEllipseDataY = (accDataY-accEllipseBiasY) * accEllipseScaleY;
% accEllipseDataZ = (accDataZ-accEllipseBiasZ) * accEllipseScaleZ;

accEllipseBiasX = mean(accDataX);
accEllipseBiasY = mean(accDataY);
accEllipseBiasZ = mean(accDataZ);

accEllipseScaleX = (2*9.80665)/(max(accDataX)-min(accDataX));
accEllipseScaleY = (2*9.80665)/(max(accDataY)-min(accDataY));
accEllipseScaleZ = (2*9.80665)/(max(accDataZ)-min(accDataZ));

accEllipseDataX = accDataX*accEllipseScaleX;
accEllipseDataY = accDataY*accEllipseScaleY;
accEllipseDataZ = accDataZ*accEllipseScaleZ;

accPreEllipseNorm = sqrt(accEllipseDataX.^2 + accEllipseDataY.^2 + accEllipseDataZ.^2);
accPreEllipseAvg = mean(accPreEllipseNorm)
accPreEllipseStd = std(accPreEllipseNorm)

[accEllipseA,accEllipseB] = CalibrateEllipsoidData3D(accEllipseDataX,accEllipseDataY,accEllipseDataZ,20,0);
[accEllipseCorrX,accEllipseCorrY,accEllipseCorrZ] = CorrectEllipsoidData3D(accEllipseDataX,accEllipseDataY,accEllipseDataZ,accEllipseA,accEllipseB);

accPostEllipseNorm = sqrt(accEllipseCorrX.^2 + accEllipseCorrY.^2 + accEllipseCorrZ.^2);

accPostEllipseAvg = mean(accPostEllipseNorm)
accPostEllipseStd = std(accPostEllipseNorm)


%%%%%%% MAG DATA %%%%%%%

% magEllipseBiasX = mean(magDataX);
% magEllipseScaleX = 0.4784/(max(abs(magDataX)));
% 
% magEllipseBiasY = mean(magDataY);
% magEllipseScaleY = 0.4784/(max(abs(magDataY)));
% 
% magEllipseBiasZ = mean(magDataZ);
% magEllipseScaleZ = 0.4784/(max(abs(magDataZ)));
% 
% magEllipseDataX = (magDataX-magEllipseBiasX) * magEllipseScaleX;
% magEllipseDataY = (magDataY-magEllipseBiasY) * magEllipseScaleY;
% magEllipseDataZ = (magDataZ-magEllipseBiasZ) * magEllipseScaleZ;

magEllipseBiasX = mean(magDataX);
magEllipseBiasY = mean(magDataY);
magEllipseBiasZ = mean(magDataZ);

magEllipseScaleX = (2*0.4784)/(max(magDataX)-min(magDataX));
magEllipseScaleY = (2*0.4784)/(max(magDataY)-min(magDataY));
magEllipseScaleZ = (2*0.4784)/(max(magDataZ)-min(magDataZ));

magEllipseDataX = magDataX*magEllipseScaleX;
magEllipseDataY = magDataY*magEllipseScaleY;
magEllipseDataZ = magDataZ*magEllipseScaleZ;

magPreEllipseNorm = sqrt(magEllipseDataX.^2 + magEllipseDataY.^2 + magEllipseDataZ.^2);
magPreEllipseStd = std(magPreEllipseNorm)
magPreEllipseAvg = mean(magPreEllipseNorm)


[magEllipseA, magEllipseB] = CalibrateEllipsoidData3D(magEllipseDataX,magEllipseDataY,magEllipseDataZ,50,0);
[magEllipseCorrX,magEllipseCorrY,magEllipseCorrZ] = CorrectEllipsoidData3D(magEllipseDataX,magEllipseDataY,magEllipseDataZ,magEllipseA,magEllipseB);

magPostEllipseNorm = sqrt(magEllipseCorrX.^2 + magEllipseCorrY.^2 + magEllipseCorrZ.^2);

magPostEllipseAvg = mean(magPostEllipseNorm)
magPostEllipseStd = std(magPostEllipseNorm)











%%%%%%% PLOTTING %%%%%%%%


figure(1)

subplot(4,1,1);
sphere(50),hold on
colormap('gray');
plot3(accData(:,1),accData(:,2),accData(:,3),'b.');
hold on
axis('equal');
title('acc');

subplot(4,1,2);
histfit(accPreEllipseNorm);
title('acc pre calib norm');

subplot(4,1,3);
histfit(accPostNaiveNorm);
title('acc post naive norm');

subplot(4,1,4);
histfit(accPostEllipseNorm);
title('acc post ellipse norm');



figure (2)

subplot(4,1,1);
sphere(50),hold on
colormap('gray');
plot3(magData(:,1),magData(:,2),magData(:,3),'g.');
hold on
axis('equal');
title('mag');

subplot(4,1,2);
histfit(magPreEllipseNorm);
title('mag pre calib norm');

subplot(4,1,3);
histfit(magPostNaiveNorm);
title('mag post naive norm');

subplot(4,1,4);
histfit(magPostEllipseNorm);
title('mag post ellipse norm');
