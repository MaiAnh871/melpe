%% initialize
T = 0.5;        % time to play the note in 2 sec
Fs = 8000;      % sampling frequency
t = 0: 1/Fs: T; % vector of time
Amp = 2;        % amplitude of the tone

%% music tone frequency
C = Amp * sin(2*pi*262*t);
D = Amp * sin(2*pi*294*t);
E = Amp * sin(2*pi*330*t);
F = Amp * sin(2*pi*349*t);
G = Amp * sin(2*pi*392*t);
A = Amp * sin(2*pi*440*t);
B = Amp * sin(2*pi*494*t);
S = 0;
%% harmony
s = [C C G G A A G S S F F E E D D C S S];
sound (s, Fs); 

