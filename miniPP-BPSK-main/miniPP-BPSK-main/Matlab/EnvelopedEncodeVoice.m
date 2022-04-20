clear; 
clc;
delete _pesq_results.txt
%% File 

for idx = 1:1
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
rxDataInPath  = [binDir 'Rx/DataIn/'     sample '-rx.bin'];
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

% Them 0 vao truoc file de co them nhieu tin hieu dong bo tu silence frame
dataTxI = [zeros(16000,1);dataTxI];
writeBin(dataTxI,txDataInPath,'int16');

%% Tx: trien khai tx va doc du lieu file txOut
system([txExe ' ' txDataInPath ' ' txDataOutPath]);

dataTxO = readBin(txDataOutPath,'int16');
%dataTxo = resample(dataTxo,8e3,48e3) / 2^15; % Neu su dung tx.exe 48k
dataTxO = dataTxO/2^15;                       % Neu su dung tx.exe 8k

%% Vad cua Tx: tim vad cua file TxIn
[vad, vad_upsample] = vadBin(txDataInPath);

%% Envelope
Env = readBin(envPath,'int16') / 2^15;

%% Main envelope - making envelope for compress data
Env = mean(Env,2);
[up,lo] = envelope(Env,300,'peak');
Env = abs(up);
% Neu su dung envelope la ca audio
% offset = 0.05;
% Env = Env + offset;
% Neu su dung envelope la mot phan co muc nang luong > 0 cua audio
Env = Env(28250:31450);

%% Keo dai Env bang do dai cua dataTxO
Env2 = Env;
while(length(Env2) < length(dataTxO))
    Env2 = [Env2 ; Env];
end
Env2 = Env2/max(Env2); %normalize
% offset = 0.4;
% Env2 = Env2 + offset;
Env2 = Env2/max(Env2);
Env = Env2(1:length(dataTxO));

%% Env for VAD: su dung cho option 4
% Env_vad = Env(1:26670) - offset;
% Env3 = Env_vad;
% 
% while(length(Env3) < length(dataTx))
%     Env3 = [Env3 ; Env_vad];
% end
% Env3 = Env3/max(Env3); %normalize
% Env_vad = Env3(1:length(dataTx));
% Env_vad = Env_vad + 0.2;

%% Dieu che dataTxo voi Envelope dau ra la dataTxE
%% Mul voice data with ENVELOPE, keep original and cut several VADs (option 1)
Env = Env.*vad_upsample;
dataTxE = dataTxO .* Env  + dataTxO.*~vad_upsample*0.5;
%% mul voice data, VAD with envelope, no cut (option 2)
% dataTxE = dataTxO .* Env;

%% No mul data with Envelope, only cut several VAD (option 3)
% dataTxE = dataTxO;

%% Mul data with envelope offset, mul control pkt with envelope no offset(option 4)
% Env = Env.*vad_upsample;
% dataTxE = dataTxO.* Env  + dataTxO.*~vad_upsample.*Env_vad;
%% Transform vad
% Cut several VADs 
shape;
sin8Khz(1,:)=sin8Khz(1,:)/max(sin8Khz(1,:));
sin8Khz(2,:)=sin8Khz(2,:)/max(sin8Khz(2,:));
sin8Khz(3,:)=sin8Khz(3,:)/max(sin8Khz(3,:));
sin8Khz(4,:)=sin8Khz(4,:)/max(sin8Khz(4,:));
k = 0;
ii = [];

% % Cut frame
% for i = length(vad) - 1 :-1: 20
%    if(k == 0)
%       A = 0.005*kron(2*randi([0,1],1,90*1)-1,sin8Khz(randi([1,4]),:))+0.002*randn(1,540*1);
%       vad_upsample(540*(i-1) + 1 : 540*i) = -1* ones(1,540)';
%       dataTxE = [dataTxE(1:540*i); A' ;dataTxE(540*i+1:end)];
%       ii = [ii i];  
%    end
%    k = mod((k+1),25);
% end

%%
% for i = 10:length(vad)
%    k = mod((k+1),30);
%    if(k == 0 || (vad(i) == 0 && k > 10))
%       A = 0.005*kron(2*randi([0,1],1,90*1)-1,sin8Khz(randi([1,4]),:))+0.002*randn(1,540*1);
%       dataTxE(540*(i-1)+1:540*(i)) = A; %cut ith frame VAD = low power frame
%       ii = [ii i];  
%       k = 0;
%    end
%    
% end


%% Convert to INT16 DATA TX ENVELOPE
dataTxE = int16(dataTxE/max(dataTxE)*2^15);

writeBin(dataTxE,txDataEnvPath,'int16');

%% Rx
dataRxI = readBin(txDataEnvPath,'int16') / 2^15;
% dataRxI = resample(dataRxI,48e3,8e3);    %Neu su dung rx.exe 48 KHz 
dataRxI = int16(dataRxI*2^15);
writeBin(dataRxI,rxDataInPath,'int16');
system([rxExe ' ' rxDataInPath ' ' rxDataOutPath]);

dataRxO = readBin(rxDataOutPath,'int16');
% sound(dataRxO/2^15);

%% Ghi file rx.wav
audiowrite(AudioRxPath,dataRxO(16001:end)/2^15,8e3);

%% Tao file.h de truyen tren dien thoai

% createHeaderFile(sample,dataTxE);
% movefile([matlabDir sample '.h'],headerPath);

%% Cham PESQ
copyfile(AudioTxPath,'1.wav')
copyfile(AudioRxPath,'2.wav')
system([pesqExe ' 1.wav' ' 2.wav']);
delete('1.wav');
delete('2.wav');
delete();
clearvars -except dataTxI dataTxO dataTxE dataRxO dataRxI Env Env2 ii vad vad_upsample up
end
plot(double(dataRxI)/max(double(dataRxI)))
hold on;
plot(vad_upsample)
PESQ = getPESQscore();
mean(PESQ)
