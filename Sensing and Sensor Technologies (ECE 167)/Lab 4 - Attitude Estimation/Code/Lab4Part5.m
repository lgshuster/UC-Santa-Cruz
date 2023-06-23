clear all;
close all;
clc;

%%%%%%%%%%%%%%%%%%%%%%% NOT NOISY %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
deltaT = 0.02;
noiseFlag = 0;

[Acc,Mag,wGyro,EulTrue] = CreateTrajectoryData(deltaT,noiseFlag);


%%%%%%%%%%%%%%%%%%%%%%%%% FORWARD INTEGRATION %%%%%%%%%%%%%%%%%%%%%%%%%%%

DCMInitial = eul2dcm(EulTrue(1,:));
%DCMInitial = [1 0 0; 0 1 0; 0 0 1];
UseMatrixExponential = 0;

DCMCurrent = DCMInitial;
for i = 1:100, 
    EulInteg(i,:) = [atan2(DCMCurrent(1,2),DCMCurrent(1,1)) ...
                   -asin(DCMCurrent(1,3)) ...
                    atan2(DCMCurrent(2,3),DCMCurrent(3,3))]*180/pi; 
    DCMCurrent = IntegrateOpenLoop(DCMCurrent, wGyro(i,:)'/131*pi/180, deltaT, UseMatrixExponential)
end

% figure(1)
% subplot(4,2,1)
% plot(linspace(1,length(EulTrue),length(EulTrue)),EulTrue(:,1)-real(EulInteg(:,1)))
% hold on
% plot(linspace(1,length(EulTrue),length(EulTrue)),EulTrue(:,2)-real(EulInteg(:,2)))
% hold on
% plot(linspace(1,length(EulTrue),length(EulTrue)),EulTrue(:,3)-real(EulInteg(:,3)))
% hold on
% title('forward integration error')
% xlabel('trial number')
% ylabel('Eul true - Eul integrated')
% legend('yaw','pitch','roll')
% 
% 
% 
% subplot(4,2,3)
% histfit(EulTrue(:,1)-real(EulInteg(:,1)))
% title('yaw error')
% hold on
% subplot(4,2,5)
% histfit(EulTrue(:,2)-real(EulInteg(:,2)))
% title('pitch error')
% hold on
% subplot(4,2,7)
% histfit(EulTrue(:,3)-real(EulInteg(:,3)))
% title('roll error')
% hold on
% 
%
% %%%%%%%%%%%%%%%%%%%%%%% EXPONENTIAL INTEGRATION %%%%%%%%%%%%%%%%%%%%%%%%%%%
% 
% DCMInitial = eul2dcm(EulTrue(1,:));
% %DCMInitial = [1 0 0; 0 1 0; 0 0 1];
% UseMatrixExponential = 1;
% 
% DCMCurrent = DCMInitial;
% for i = 1:length(EulTrue), 
%     EulInteg(i,:) = [atan2(DCMCurrent(1,2),DCMCurrent(1,1)) ...
%                    -asin(DCMCurrent(1,3)) ...
%                     atan2(DCMCurrent(2,3),DCMCurrent(3,3))]*180/pi; 
%     DCMCurrent = IntegrateOpenLoop(DCMCurrent, wGyro(i,:)'*pi/180, deltaT, UseMatrixExponential);
% end
% 
% 
% subplot(4,2,2)
% plot(linspace(1,length(EulTrue),length(EulTrue)),EulTrue(:,1)-EulInteg(:,1))
% hold on
% plot(linspace(1,length(EulTrue),length(EulTrue)),EulTrue(:,2)-EulInteg(:,2))
% hold on
% plot(linspace(1,length(EulTrue),length(EulTrue)),EulTrue(:,3)-EulInteg(:,3))
% title('exponential integration error')
% xlabel('trial number')
% ylabel('Eul true - Eul integrated')
% legend('yaw','pitch','roll')
% 
% subplot(4,2,4)
% histfit(EulTrue(:,1)-EulInteg(:,1))
% title('yaw error')
% hold on
% subplot(4,2,6)
% histfit(EulTrue(:,2)-EulInteg(:,2))
% title('pitch error')
% hold on
% subplot(4,2,8)
% histfit(EulTrue(:,3)-EulInteg(:,3))
% title('roll error')
% hold on
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% %%%%%%%%%%%%%%%%%%%%% NOISY %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% deltaT = 0.02;
% noiseFlag = 1;
% UseMatrixExponential = 0;
% 
% [Acc,Mag,wGyro,EulTrue] = CreateTrajectoryData(deltaT,noiseFlag);
% 
% 
% %%%%%%%%%%%%%%%%%%%%%%% FORWARD INTEGRATION %%%%%%%%%%%%%%%%%%%%%%%%%%%
% wGyro((1/deltaT):length(wGyro),1) = wGyro((1/deltaT):length(wGyro),1) - mean(wGyro(1:1/deltaT,1));
% wGyro((1/deltaT):length(wGyro),2) = wGyro((1/deltaT):length(wGyro),2) - mean(wGyro(1:1/deltaT,2));
% wGyro((1/deltaT):length(wGyro),3) = wGyro((1/deltaT):length(wGyro),3) - mean(wGyro(1:1/deltaT,3));
% 
% 
% DCMInitial = eul2dcm(EulTrue(1,:));
% %DCMInitial = [1 0 0; 0 1 0; 0 0 1];
% UseMatrixExponential = 0;
% 
% DCMCurrent = DCMInitial;
% for i = 1:length(EulTrue), 
%     EulInteg(i,:) = [atan2(DCMCurrent(1,2),DCMCurrent(1,1)) ...
%                    -asin(DCMCurrent(1,3)) ...
%                     atan2(DCMCurrent(2,3),DCMCurrent(3,3))]*180/pi; 
%     DCMCurrent = IntegrateOpenLoop(DCMCurrent, wGyro(i,:)'*pi/180, deltaT, UseMatrixExponential);
% end
% 
% figure(2)
% subplot(4,2,1)
% plot(linspace(1,length(EulTrue),length(EulTrue)),EulTrue(:,1)-real(EulInteg(:,1)))
% hold on
% plot(linspace(1,length(EulTrue),length(EulTrue)),EulTrue(:,2)-real(EulInteg(:,2)))
% hold on
% plot(linspace(1,length(EulTrue),length(EulTrue)),EulTrue(:,3)-real(EulInteg(:,3)))
% hold on
% title('forward integration error')
% xlabel('trial number')
% ylabel('Eul true - Eul integrated')
% legend('yaw','pitch','roll')
% 
% 
% subplot(4,2,3)
% histfit(EulTrue(:,1)-real(EulInteg(:,1)))
% title('yaw error')
% hold on
% subplot(4,2,5)
% histfit(EulTrue(:,2)-real(EulInteg(:,2)))
% title('pitch error')
% hold on
% subplot(4,2,7)
% histfit(EulTrue(:,3)-real(EulInteg(:,3)))
% title('roll error')
% hold on
% 
% %%%%%%%%%%%%%%%%%%%%%%% EXPONENTIAL INTEGRATION %%%%%%%%%%%%%%%%%%%%%%%%%%%
% 
% DCMInitial = eul2dcm(EulTrue(1,:));
% %DCMInitial = [1 0 0; 0 1 0; 0 0 1];
% UseMatrixExponential = 1;
% 
% DCMCurrent = DCMInitial;
% for i = 1:length(EulTrue), 
%     EulInteg(i,:) = [atan2(DCMCurrent(1,2),DCMCurrent(1,1)) ...
%                    -asin(DCMCurrent(1,3)) ...
%                     atan2(DCMCurrent(2,3),DCMCurrent(3,3))]*180/pi; 
%     DCMCurrent = IntegrateOpenLoop(DCMCurrent, wGyro(i,:)'*pi/180, deltaT, UseMatrixExponential);
% end
% 
% subplot(4,2,2)
% plot(linspace(1,length(EulTrue),length(EulTrue)),EulTrue(:,1)-EulInteg(:,1))
% hold on
% plot(linspace(1,length(EulTrue),length(EulTrue)),EulTrue(:,2)-EulInteg(:,2))
% hold on
% plot(linspace(1,length(EulTrue),length(EulTrue)),EulTrue(:,3)-EulInteg(:,3))
% title('exponential integration error')
% xlabel('trial number')
% ylabel('Eul true - Eul integrated')
% legend('yaw','pitch','roll')
% 
% subplot(4,2,4)
% histfit(EulTrue(:,1)-EulInteg(:,1))
% title('yaw error')
% hold on
% subplot(4,2,6)
% histfit(EulTrue(:,2)-EulInteg(:,2))
% title('pitch error')
% hold on
% subplot(4,2,8)
% histfit(EulTrue(:,3)-EulInteg(:,3))
% title('roll error')
% hold on
% 
% 
% 






function C=eul2dcm(eul)
%----------------------------------------------------------------
% function C=eul2dcm(eul)
%
%   This functions determines the direction cosine matrix C
%   that transforms a vector in a reference axis system at time k
%   to one the same axis sytem at time k+1.  The input argument to
%   this function is a vector of the Euler angles in the following
%   order: eul = [yaw,pitch,roll]. (i.e., 3-2-1 rotation convention).  
%
%-----------------------------------------------------------------  

ps=eul(1); th=eul(2); ph=eul(3);

C1=[1 0 0; 0 cos(ph) sin(ph); 0 -sin(ph) cos(ph)];
C2=[cos(th) 0 -sin(th); 0 1 0; sin(th) 0 cos(th)];
C3=[cos(ps) sin(ps) 0; -sin(ps) cos(ps) 0; 0 0 1];

C=C1*C2*C3;
end