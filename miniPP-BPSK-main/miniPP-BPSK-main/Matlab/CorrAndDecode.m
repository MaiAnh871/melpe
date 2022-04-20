%   made by hsdfat8
%   date: 02/01/2021

%% Tuong quan va giai ma file nhan duoc
clearvars -except hCoff;
close;
clc;
delete _pesq_results.txt
%% vong for
for i = 3
%% Setup duong dan
sample = num2str(i);           % So thu tu sample
reReceive = '';       % '_x' Gia tri truyen nhan lan thu x
folder = 'try to cut only vad frame - new vad2';             %Loai truyen 8kNoEnc: truyen khong encrypt
                                                %Loai truyen 8kCutVoice: Truyen khong encrypt,
                                                    %cat voice theo chuky
% if i == 2 || i == 9
%     locCor = 5;             % Vi tri lay tuong quan, check mang locas de thay doi
% else
    locCor = 1;             % Vi tri lay tuong quan, check mang locas de thay doi
% end

rxExcute = '..\rx';  % rx thuc thi
% rxExcute = 'rx8kBufNe';  % rx thuc thi
%% Doc file
% Tin hieu thu duoc % Tin hieu truyen di
% dataRx = readBin(['RX\rawReceived\' folder '\' sample reReceive '.bin'],'int16');
% dataTx = readBin(['TX\' folder '\' sample '-txE.bin'],'int16');
dataRx = readBin(['G:\My Drive\data\' folder '\' sample  '.rx'],'int16');
dataTx = readBin(['G:\My Drive\data\' folder '\' sample  '.tx'],'int16');

% dataRx = readBin(['D:\voice-compress\GD\data\' folder '\' sample  '.rx'],'int16');
% dataTx = readBin(['D:\voice-compress\GD\data\' folder '\' sample  '.tx'],'int16');
%% Lay tuong quan
[corre, lag] = xcorr (dataRx, dataTx);
% Tim cac vi tri co bien do lon hon threshold * max 
% locas: cac vi tri co bien do duong lon hon threshold * max
threshold = 0.5;
locas = find ((corre > threshold * max (abs(corre))) | (corre < -threshold * max (abs(corre))));
locas =  locas(locas > length(dataRx));

%% Cat file theo 1 vi tri
if length(locas) == 1
   locs = locas; 
else
   locs = locas(locCor);
end
locs = locs - length(dataRx);
if length(dataTx)+locs <= length(dataRx)
    dataRcCorr = dataRx(1+locs:length(dataTx)+locs);
else
    dataRcCorr = dataRx(1+locs:end);
end

% dataRxCorrUpsamle = resample(dataRcCorr,48e3,8e3);
dataRxCorrUpsamle = dataRcCorr;
%dataRxCorrUpsamle = dataRxCorrUpsamle/max(dataRxCorrUpsamle);
%dataRxCorrUpsamle = round(dataRxCorrUpsamle*2^12);

%% Ghi du lieu ra file va giai ma
filein = ['RX\corr\' sample '-rx.bin'];
fileout = ['RX\decoded\' sample '-out.bin'];
writeBin(dataRxCorrUpsamle,filein,'short');
system(['..\EXE\' rxExcute ' ' filein  ' '  fileout]);
wavOut = readBin(fileout,'short');


%% Kiem tra PESQ
% DirRefWavFile = ['VoiceData\wav\raw\' sample '.wav'];
DirRefWavFile = ['G:\My Drive\data\wav\' sample '.wav'];
% DirRefWavFile = ['D:\voice-compress\GD\data\wav\' sample '.wav'];
copyfile(DirRefWavFile,'1.wav');
DirWavFileCut = ['VoiceData\wav\decode-cut\' sample '-rxcut.wav'];

[ref, fs] = audioread(DirRefWavFile);
ref = resample(ref,8e3,fs);
audiowrite(DirRefWavFile,ref , 8e3);
% Kiem tra PESQ - file cut
cut = 16000;
wavOut_cut = wavOut(cut:end)/max(abs(wavOut(cut:end)));
% [vadIdx, vad] = vadRaw(ref);
%wavOut_cut = wavOut_cut .* vad(1:length(wavOut_cut));
audiowrite(DirWavFileCut, wavOut_cut , 8e3);
system(['..\EXE\PESQ +8000 ' '1.wav '  DirWavFileCut]);
delete('1.wav')
end
PESQ = getPESQscore();
mean(PESQ)