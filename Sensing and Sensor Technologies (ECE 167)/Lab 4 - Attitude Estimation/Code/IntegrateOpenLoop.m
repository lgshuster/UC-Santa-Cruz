function [Rplus] = IntegrateOpenLoop(Rminus, gyros, deltaT, UseMatrixExponential)
% function [Rplus] = IntegrateOpenLoop(Rminus, gyros, deltaT)
%
% Function to Integrate the gyros to the attitude DCM
%
% Inputs: Previous attitute DCM (Rminus)
%         Body Fixed Rotation rates ([p;q;r]) in rad/s
%         Time between samples (deltaT) in seconds
%
% Outputs: New DCM (Rplus)
%
% Note: This code implements both a forward integration as well as the
% matrix exponential version of the integration, change the flag in the
% code below to pick between them.

if (UseMatrixExponential),
    Rplus = Rminus * Rexp(gyros * deltaT);
else
    Rplus = Rminus + rcross(gyros* deltaT) * Rminus ;
end
