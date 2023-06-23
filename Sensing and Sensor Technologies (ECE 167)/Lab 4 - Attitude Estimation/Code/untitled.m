close all;
clear all;
clc;


Rminus = eye(3);

Bminus = [1;1;1];
gyros = [1;1;1];
mags = [2;5;13];
accels = [19;5;2];
magInertial = [22.768;5.313;41.5083];
accelInertial = [0;0;1];
deltaT = 0.02;

for i=1:2,
    [Rplus, Bplus] = IntegrateClosedLoop(Rminus, Bminus, gyros, mags, accels, magInertial, accelInertial, deltaT)
    Rminus = Rplus;
    Bminus = Bplus;
end 
