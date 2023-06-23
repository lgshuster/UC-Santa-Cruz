close all;
clear all;
clc;

mags = [10;4;7];
accels = [3;9;1];
magInertial = [22.768;5.313;41.5083];
accelInertial = [0;0;1];

R = DCMfromTriad(mags, accels, magInertial, accelInertial)
