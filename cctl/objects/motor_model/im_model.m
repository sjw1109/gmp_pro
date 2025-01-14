syms a0 a1 a2 a3

A = [a0, a1; a2, a3];

inv(A)

%% inductor motor

% [R] matrix

syms Rs Rr

R = diag([Rs, Rs, Rr, Rr]);

% [L] matrix
syms Ls Lr Lm

L = [Ls, 0, Lm, 0; 0, Ls, 0, Lm; Lm, 0, Lr, 0; 0, Lm, 0, Lr];

% [omega] matrix
syms omega_da omega_dA

Omega = [0, -omega_da, 0, 0; omega_da, 0, 0, 0; 0,0,0,-omega_dA; 0,0,omega_dA,0];

%% reuslt

A = -(inv(L) * R + inv(L) * Omega * L)

% [                                           (Lr*Rs)/(Lm^2 - Lr*Ls),  (Lm^2*omega_dA)/(Lm^2 - Lr*Ls) - (Lr*Ls*omega_da)/(Lm^2 - Lr*Ls),                                           -(Lm*Rr)/(Lm^2 - Lr*Ls), (Lm*Lr*omega_dA)/(Lm^2 - Lr*Ls) - (Lm*Lr*omega_da)/(Lm^2 - Lr*Ls)]
% [ (Lr*Ls*omega_da)/(Lm^2 - Lr*Ls) - (Lm^2*omega_dA)/(Lm^2 - Lr*Ls),                                            (Lr*Rs)/(Lm^2 - Lr*Ls), (Lm*Lr*omega_da)/(Lm^2 - Lr*Ls) - (Lm*Lr*omega_dA)/(Lm^2 - Lr*Ls),                                           -(Lm*Rr)/(Lm^2 - Lr*Ls)]
% [                                          -(Lm*Rs)/(Lm^2 - Lr*Ls), (Lm*Ls*omega_da)/(Lm^2 - Lr*Ls) - (Lm*Ls*omega_dA)/(Lm^2 - Lr*Ls),                                            (Ls*Rr)/(Lm^2 - Lr*Ls),  (Lm^2*omega_da)/(Lm^2 - Lr*Ls) - (Lr*Ls*omega_dA)/(Lm^2 - Lr*Ls)]
% [(Lm*Ls*omega_dA)/(Lm^2 - Lr*Ls) - (Lm*Ls*omega_da)/(Lm^2 - Lr*Ls),                                           -(Lm*Rs)/(Lm^2 - Lr*Ls),  (Lr*Ls*omega_dA)/(Lm^2 - Lr*Ls) - (Lm^2*omega_da)/(Lm^2 - Lr*Ls),                                            (Ls*Rr)/(Lm^2 - Lr*Ls)]
 

