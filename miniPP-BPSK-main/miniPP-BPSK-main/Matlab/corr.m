%% Tuong quan va giai ma file nhan duoc


load PESQ.mat
clearvars -except PESQ hCoff;

clc;
sample = '19';
reReceive = '';
% Tin hieu thu duoc % Tin hieu truyen di
dataRx = readBin(['RX\rawReceived\' sample reReceive '.bin'],'int16');
dataTx = readBin(['TX\noenc8k\' sample '-txE.bin'],'int16');
% Lay tuong quan
[corre, lag] = xcorr (dataRx, dataTx);
% [pks,locs] = max(corre);

% Tim cac vi tri co bien do lon hon threshold * max 
% locas: cac vi tri co bien do duong lon hon threshold * max
threshold = 0.5;
locas = find ((corre > threshold * max (abs(corre))) | (corre < -threshold * max (abs(corre))));
locas =  locas(locas > length(dataRx));
% Cat file theo 1 vi tri
if length(locas) == 1
   locs = locas; 
else
   locs = locas(1);
end



locs = locs - length(dataRx);
if length(dataTx)+locs <= length(dataRx)
    data_rx = dataRx(1+locs:length(dataTx)+locs);
else
    data_rx = dataRx(1+locs:end);
end


datarx = resample(data_rx,48e3,8e3);
datarx = datarx/max(datarx);

% add them nhieu
% a = randi(length(datarx)/2,1,1);
% b = 50000;
% c = 10^(-40/20)*randn(b,1);
% datarx = [datarx(1:a) ; c ; datarx(a+1:length(datarx))];

%[datarx, e] = plms(datarx);
datarx = round(datarx*2^12);
filein = ['RX\corr\' sample '-rx.bin'];
fileout = ['RX\decoded\' sample '-out.bin'];
writeBin(datarx,filein,'short');

% Kiem tra giai ma
%system(['rx ' filein  ' '  fileout]);
%wavOut_modi = readsBin(fileout,'short');
system(['rx_noenc_noinv ' filein  ' '  fileout]);
wavOut = readBin(fileout,'short');


% Kiem tra PESQ
DirWavFile = ['VoiceData\wav\decode\' sample '-rx.wav'];
audiowrite(DirWavFile, wavOut/max(abs(wavOut)), 8e3);
DirRefWavFile = ['VoiceData\wav\raw\' sample '.wav'];
[ref, fs] = audioread(DirRefWavFile);
ref = resample(ref,8e3,fs);
audiowrite(DirRefWavFile,ref , 8e3);
%system(['PESQ +8000 ' DirRefWavFile ' ' DirWavFile]);

                                                              
% Kiem tra PESQ - file cut
cut = 16000;
DirWavFileCut = ['VoiceData\wav\decode-cut\' sample '-rxcut.wav'];
wavOut_cut = wavOut(cut:end)/max(abs(wavOut(cut:end)));
audiowrite(DirWavFileCut, wavOut_cut , 8e3);
DirRefWavFileCut = ['VoiceData\wav\raw-cut\' sample 'cut.wav'];
ref_cut =ref(cut:end)/max(abs(ref(cut:end)));
audiowrite(DirRefWavFileCut,ref_cut , 8e3);
system(['PESQ +8000 ' DirRefWavFile ' ' DirWavFileCut]);


