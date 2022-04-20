clear;
close;
delete _pesq_results.txt
% audioDir  = 'G:\My Drive\data\wav\';
audioDir  = 'D:\voice-compress\GD\data\';
audioDirRx  = '.\Audio\';
binDir    = '.\Bin\';
exeDir    = '..\EXE\';
melpeExe         = [exeDir 'melpe'];
pesqExe       = [exeDir 'PESQ +8000'];
for idx = 1:20
    sample = num2str(idx);
% audioDir1  = 'D:/voice-compress/GD/data/bin/';
AudioTxPath   = [audioDir 'wav\' sample '.wav'];
AudioRxPath   = [audioDirRx 'Rx\' sample '.wav'];
txDataInPath  = [audioDir 'bin\'     sample '.bin'];
rxDataOutPath = [binDir 'Rx\DataOut\'    sample '-rxo.bin'];
system([melpeExe ' ' txDataInPath ' ' rxDataOutPath]);

dataRxO = readBin(rxDataOutPath,'int16');
% sound(dataRxO/2^15);

%% Ghi file rx.wav
audiowrite(AudioRxPath,dataRxO(16001:end)/2^16,8e3);
copyfile(AudioTxPath,'1.wav')
copyfile(AudioRxPath,'2.wav')
system([pesqExe ' 1.wav' ' 2.wav']);
delete('1.wav');
delete('2.wav');
end
PESQ = getPESQscore();
mean(PESQ)