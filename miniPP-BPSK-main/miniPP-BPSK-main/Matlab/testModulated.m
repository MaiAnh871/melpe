clear;
close;
sample = '21';
txBufPath =     ['..\' sample '-txbuf.bin'];
txDataInPath =  ['VoiceData\bin\' sample '.bin'];
txDataOutPath = ['..\' sample '-txo.bin'];
rxBufPath =     ['..\' sample '-rxbuf.bin'];
rxDataInPath =  ['RX\corr\' sample  '-rx.bin'];
rxDataOutPath = ['..\' sample '-rxo.bin'];
txExcute = '..\tx';
rxExcute = '..\rx';
%% Thuc thi lenh - Doc file buffer
system(['..\EXE\' txExcute ' ' txDataInPath ' ' txDataOutPath ' ' txBufPath]);
system(['..\EXE\' rxExcute ' ' rxDataInPath ' ' rxDataOutPath ' ' rxBufPath]);
txData = uint8(readBin(txBufPath,'uint8'));                % Read txData
txDataBuf = cutFrameByFrame(txData); 
txDataBufRef = txDataBuf(1:12,:);%

[transmittSignal, outputBit] = BPSKModPP(txDataBufRef);

%rxDataInPath =    '..\1tx.bin';


% Thuc hien bang ham Matlab
rxModul = readBin(rxDataInPath,'short');
[rxBuff,ReOutputBit] = BPSKDemod8khz(rxModul);
rxBuff = uint8(rxBuff);
% Thuc hien bang code C
data = uint8(readBin(rxBufPath,'uint8'));                  % Read rxData
rxDataBuf = cutFrameByFrame(data);                          % 
rxDataBufRef = rxDataBuf(1:12,:);

% ER = abs(rxBuff(1:length(rxDataBufRef))-rxDataBufRef);
 A =outputBit(:,1:length(ReOutputBit)) - ~ReOutputBit;
 B =sum(abs(A));
 ans1 = uint8(~outputBit);
system(['del /f ' rxDataOutPath]);
system(['del /f ' rxBufPath]);
