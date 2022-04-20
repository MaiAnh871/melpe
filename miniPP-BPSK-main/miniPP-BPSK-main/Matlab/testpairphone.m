clear; 
clc;
delete _pesq_results.txt
%% File 

for idx = 2
sample = num2str(idx);
%% Tao duong dan
% Duong dan thu muc chinh
matlabDir = '.\';
exeDir    = '..\EXE\';
binDir    = '.\Bin\';
% audioDir1  = 'G:/My Drive/data/wav/';
audioDir1  = 'D:/voice-compress/GD/data/wav/';
audioDir  = '.\Audio\';

% Header Path

% Audio Path
AudioTxPath   = [audioDir1 sample '.wav'];
AudioRxPath   = [audioDir 'Rx/' sample '.wav'];
 
% Tx Path
txDataInPath  = [binDir 'Tx/Origin/'     sample '.bin'];
txDataOutPath = [binDir 'Tx/DataOut/'    sample '-txo.bin'];
txDataEnvPath = [binDir 'Tx/DataEnv/'    sample '.tx'];
% Rx Path
rxDataInPath  = txDataOutPath;
rxDataOutPath = [binDir 'Rx/DataOut/'    sample '-rxo.bin'];

% Envelope Path
envPath       = [binDir 'Envelope/1.bin'];

% Execute Path
txExe         = [exeDir '..\tx'];
rxExe         = [exeDir '..\rx'];
pesqExe       = [exeDir 'PESQ +8000'];

% Header Path
headerPath     = [matlabDir 'Header/' sample '.h'];


%% Doc audio tu file wav va ghi lai vao file bin
dataTxI = convertSampleRate(AudioTxPath,16); %data in 16 bit integerindex = 100000;
writeBin(dataTxI,txDataInPath,'int16');

%% Tx: trien khai tx va doc du lieu file txOut
system([txExe ' ' txDataInPath ' ' txDataOutPath]);

dataTxO = readBin(txDataOutPath,'int16');
%dataTxo = resample(dataTxo,8e3,48e3) / 2^15; % Neu su dung tx.exe 48k
dataTxO = dataTxO/2^15;                       % Neu su dung tx.exe 8k

%% No mul data with Envelope, only cut several VAD (option 3)
% dataTxE = dataTxO;

shape;
sin8Khz(1,:)=sin8Khz(1,:)/max(sin8Khz(1,:));
sin8Khz(2,:)=sin8Khz(2,:)/max(sin8Khz(2,:));
sin8Khz(3,:)=sin8Khz(3,:)/max(sin8Khz(3,:));
sin8Khz(4,:)=sin8Khz(4,:)/max(sin8Khz(4,:));
k = 0;
ii = [];


dataRxI = readBin(rxDataInPath,'int16');
system([rxExe ' ' rxDataInPath ' ' rxDataOutPath]);

dataRxO = readBin(rxDataOutPath,'int16');
% sound(dataRxO/2^15);

%% Ghi file rx.wav
audiowrite(AudioRxPath,dataRxO,8e3);

%% Tao file.h de truyen tren dien thoai

% createHeaderFile(sample,dataTxE);
% movefile([matlabDir sample '.h'],headerPath);

%% Cham PESQ
copyfile(AudioTxPath,'1.wav')
copyfile(AudioRxPath,'2.wav')
system([pesqExe ' 1.wav' ' 2.wav']);
delete('1.wav');
delete('2.wav');
clearvars -except dataTxI dataTxO dataTxE dataRxO dataRxI Env Env2 ii vad vad_upsample up
end
plot(double(dataRxI)/max(double(dataRxI)))
hold on;
