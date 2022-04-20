%   made by hsdfat8
%   date: 02/01/2021
%% Muc dich: Kiem tra BER giua 2 buffer ben TX va RX
% TX: Voice -> TXData (buffer: 14 phan tu)
% Channel: TXData ------ Channel ------ RXData
% RX: RXData (buffer: 14 phan tu) -> Voice 

clear;
close;


%% Path
% Command:  'tx txDataFile txDataInFile txBufFile'
%           txDataFile: Voice data file
%           txDataInFile: Encode file
%           txBufFile: TX-Buffer file
% Command:  'rx rxDataInPath rxDataOutPath rxBufPath'
%           rxDataInPath: Receive file
%           rxDataOutPath: Voice data file
%           rxBufPath: RX-Buffer file
% sample: ten sample check
BerTotal = zeros(20,1);

for index=1
txBufPath =     'buftx.bin';
txDataInPath =  '1.bin';
txDataOutPath = 'bpsk.bin';

rxBufPath =     'bufrx.bin';
rxDataInPath =  'bpsk.bin';
rxDataOutPath = 'out.bin';

txExcute = '..\tx';
rxExcute = '..\rx';
%% Thuc thi lenh - Doc file buffer
system(['..\EXE\' txExcute ' ' txDataInPath ' ' txDataOutPath ' ' txBufPath]);
system(['..\EXE\' rxExcute ' ' rxDataInPath ' ' rxDataOutPath ' ' rxBufPath]);

data = uint8(readBin(rxBufPath,'uint8'));                  % Read rxData
rxDataBuf = cutFrameByFrame(data);                          %   
txData = uint8(readBin(txBufPath,'uint8'));                % Read txData
txDataBuf = cutFrameByFrame(txData);                        %
rxSignal = readBin(rxDataOutPath,'short');
txSignal = readBin(txDataInPath,'short');

[~,nuFrame] = size(rxDataBuf);
%% Tim vi tri co tin hieu syn

% Kiem tra vi tri frame co flag dong bo, bit thu 8 buffer(14) moi frame
% Neu flag = 1 thi dong do khoi, vi tri bat dau check BER   
loc1 = [];
locFrameCut = [];
for i = 1:nuFrame-1
    if bitand(rxDataBuf(14,i),128)==128 && bitand(rxDataBuf(14,i+1),128)==0 && bitand(rxDataBuf(14,i),64)~=64  
        loc1 = [loc1 i];                    % Tim vi tri co tin hieu syn
    end
    if bitand(rxDataBuf(14,i),64)==64
        locFrameCut = [locFrameCut; i];
    end
end
                      
locSynCnt = uint16(rxDataBuf(13,loc1+1))+256*uint16(bitand(rxDataBuf(14,loc1+1),127));                      % Check cnt tin hieu syn

%Tim vi tri counter tuong ung trong buffer ben TX va RX
% Counter = 8 bit buffer(13) va 7 bit buffer(14)
locSynTx = zeros(1,length(locSynCnt));
locSynRx = zeros(1,length(locSynCnt));
for i = 1: length(locSynCnt)
    locSynTx(i) = find (txDataBuf(13,:) == mod(locSynCnt(i),256) & bitand(txDataBuf(14,:),127) == floor(double(locSynCnt(i))/256));           % Tim cnt vi tri ben Tx 
    locSynRx(i) = find (rxDataBuf(13,:) == mod(locSynCnt(i),256) & bitand(rxDataBuf(14,:),127) == floor(double(locSynCnt(i))/256) & bitand(rxDataBuf(14,:),128) == 0);           % Tim cnt vi tri ben Rx 
end


% Tinh tong so VAD ben TX
txVADsum = sum(~mod(txDataBuf(12,:),2));


%% Check ber va so frame sai
numFrameChecked = 0;            %So frame check
bitError = 0;                   %So bit loi
bitErrorVad = 0;                %So frame bi sai VAD flag
frameError = [];                %Vi tri frame sai
errorVAD = [];                  %Vi tri frame VAD sai
errorVoice = [];                %Vi tri frame Voice sai
errorTxVoice2RxVAD = [];        %Vi tri frame Voice -> VAD
errorTxVAD2RxVoice = [];        %Vi tri frame VAD -> Voice
a = zeros(3240*nuFrame,1);
Ber = zeros(1,nuFrame);
rxVAD = zeros(1,nuFrame);
lockPhase = zeros(1,nuFrame);
lockBlock = zeros(1,nuFrame);
for idx = 1: nuFrame
    lockBlock(idx) = bitand(rxDataBuf(12,idx),64)/64;
    lockPhase(idx) = bitand(rxDataBuf(12,idx),32)/32;
    rxVAD(idx) = bitand(rxDataBuf(12,idx),1);
end


for idx = 1: length(locSynRx)
    if idx == length(locSynRx)
        endSyn = nuFrame;
    else
        endSyn = locSynRx(idx+1)-1;
    end
    for i = locSynRx(idx): endSyn
        numFrameChecked = numFrameChecked +1;
        if bitand(rxDataBuf(14,i),64)~=64   
        [bitError1Frame, bitErrorVad1Frame] = checkBerIn1Frame (txDataBuf(:,locSynTx(idx)+i-locSynRx(idx)), rxDataBuf(:,i));
        bitError = bitError + bitError1Frame;
        bitErrorVad = bitErrorVad + bitErrorVad1Frame;
        end
        Ber(i) = (double(bitError)+double(bitErrorVad))/(81*(i-locSynRx(1)+1));
        if (bitErrorVad1Frame+bitError1Frame) ~= 0 
           if mod(rxDataBuf(12,i),2) == 0       %RX nhan VAD
               if mod(rxDataBuf(12,i),2) == mod(txDataBuf(12,locSynTx(idx)+i-locSynRx(idx)),2) %Tx cung nhan la VAD
                   errorVAD = [errorVAD i];
               else                             %Rx nhan nham thanh Voice
                   errorTxVoice2RxVAD = [errorTxVoice2RxVAD i];
               end
           else                                 %RX nhan Voice
%                rxVADup = [rxVADup ones(1,540)];
               if mod(rxDataBuf(12,i),2) == mod(txDataBuf(12,locSynTx(idx)+i-locSynRx(idx)),2) %Tx cung nhan la Voice
                   errorVoice = [errorVoice i]; 
               else                             %RX nhan nham thanh VAD
                   errorTxVAD2RxVoice = [errorTxVAD2RxVoice i];
               end
           end
           frameError = [frameError i]; 
           a(3240*(i-1)+1:3240*i) = ones(3240,1);
        end
    end
end
rxVADup = kron(rxVAD,ones(1,540));
temp = [zeros(1,250) ones(1,40) zeros(1,250)];
lockBlockup  = kron(~lockBlock,temp);
lockPhaseup  = kron(~lockPhase,temp);
FrameErrorRate = length(frameError)/numFrameChecked;
BER = (double(bitError)+double(bitErrorVad))/(81*numFrameChecked);



%% Plot tin hieu ben TX va RX
figure;
subplot(411);
[vadIdx, wavIn, txVADup, modulated] = vadBin(txDataInPath);
plot(txSignal/max(abs(txSignal)));
hold on;
plot(txVADup);
hold off;
title(['Tin hieu goc ben Tx sample: ']);
subplot(412);
plot(rxSignal/max(abs(rxSignal)));
title(['Tin hieu giai dieu che ben Rx sample: ']);
hold on;
plot(rxVADup,'LineWidth',1);
plot(lockPhaseup,'LineWidth',1.5);
plot(lockBlockup,'LineWidth',1.5);
legend('signal','VAD','lost lockPhase','lost lockBlock');
hold off;
subplot(413);
plot(readBin(rxDataInPath,'short'));
title(['Tin hieu thu duoc tren kenh truyen: ']);
subplot(414);
plot(Ber);
title(['BER theo frame - BER toan bo: ' num2str(round(BER*100,2)) '%']);

%% Xoa file trung gian
system(['del /f ' txDataOutPath]);
system(['del /f ' rxDataOutPath]);
system(['del /f ' txBufPath]);
system(['del /f ' rxBufPath]);
BerTotal(index) = BER;


end
