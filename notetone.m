% Generation of Birthday Song tunes using Matlab
% if you know the generation of sine wave, you can compose any tune in the
% matlab
clc;
clear all;
close all;
t = 0:1/8000:0.5;
k = 3; % frequency scale
% music tone frequencies
C4 = sin(2*pi*k*261.6*t);
c4 = sin(2*pi*k*277.2*t);
D4 = sin(2*pi*k*293.7*t);
Eb4 = sin(2*pi*k*311.1*t);
E4 = sin(2*pi*k*329.2*t);
F4 = sin(2*pi*k*349.2*t);
f4 = sin(2*pi*k*370.0*t);
G4 = sin(2*pi*k*292.0*t);
g4 = sin(2*pi*k*415.3*t);
A4 = sin(2*pi*k*440.0*t);
Bb = sin(2*pi*k*466.2*t);
B = sin(2*pi*k*493.9*t);
C5 = sin(2*pi*k*523*t);
% Here the harmony or the order of birthday tunes order
% C4 C4 D4 C4 F4 E4 C4 C4 D4 C4 G4 F4 C4 C4 C5 A4 F4 E4 D4 Bb Bb A4 F4 G4 F4
m = [C4 C4 D4 C4 F4 E4 C4 C4 D4 C4 G4 F4 C4 C4 C5 A4 F4 E4 D4 Bb Bb A4 F4 G4 F4];
m1 = [m m];
sound(m1,8000);
% just run the program and enjoy the brithday song.