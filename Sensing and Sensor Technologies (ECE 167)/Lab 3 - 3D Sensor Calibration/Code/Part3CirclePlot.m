clear all;
close all;

syms xo yo a b

EllipseXYData;

figure (1)

scatter(Xmeas, Ymeas);
axis('equal')
hold on

M = [Xmeas -(Ymeas.^2) Ymeas ones(size(Xmeas))];

k = M \ (Xmeas.^2)

syms xo yo a b

eqn1 = 2*xo == 0.7923;
eqn2 = a^2/b^2 == 0.2658;
eqn3 = ((2*a^2)/b^2)*yo == -0.1801;
eqn4 = -xo^2 - ((a^2/b^2)*yo^2) + a^2*R^2 == 1.0349;

sol = solve([eqn1, eqn2, eqn3, eqn4], [xo, yo, a, b]);

xoSol = sol.xo;
yoSol = sol.yo;
aSol = sol.a;
bSol = sol.b;

xoSol(1)
yoSol(1)
aSol(2)
bSol(3)

xhat = (1/aSol(2)) * (Xmeas - xoSol(1));
yhat = (1/bSol(3)) * (Ymeas - yoSol(1));

scatter(xhat, yhat, 'filled');

figure (2)

twoNormEllipse = sqrt(Xmeas.^2 + Ymeas.^2);

scatter([1:1:150], twoNormEllipse)
hold on

twoNormCircle = sqrt(xhat.^2 + yhat.^2);

scatter([1:1:150], twoNormCircle, 'filled')
