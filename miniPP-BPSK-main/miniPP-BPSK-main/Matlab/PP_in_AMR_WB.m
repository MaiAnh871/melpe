function out = PP_in_AMR_WB(fileName,modeIn,modeOut,folderFile)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Implement Pairphone in AMR-WB channel.
% Model in this function:
% Filename without .wav tail
%
% |file.wav(Tx)|=>|PP_Tx|=>|UE1|=>|MS|=>|UE2|=>|PP_Rx|=>|file.wav(Rx)|
%
%   PP layer:
%           PP_Tx: file.bin -> file-txI.bin 
%           PP_Rx: file-txO.bin -> file-rx.bin
%   AMR-WB layer: 
%           UE1: file-txI.bin   -> file-txI1.bin 
%           MS:  file-txI1.bin  -> file-txI12.bin  -> file-txI2.bin
%           UE2: file-txI2.bin  -> file-txO.bin
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    DIR = 'c:/voice_compress/BKIC-pairphone/';
    channel = 'c:/voice_compress/BKIC-pairphone/Matlab/WB_Channel/';
    if (nargin < 4)
        folder = '';
    else 
        folder = [folderFile '/'];
    end % checking folder
%% Pre-processing
    wavIn = convertSampleRate([folder fileName '.wav'],8e3);
    bitStream = wav2bin(wavIn,14);
    writeBin(bitStream,[folder fileName '.bin'],'short');

%% PairPhone---TX
    system([DIR 'tx ' [folder fileName '.bin'] ' ' [folder fileName '-txI.bin']]);
    bitStream = readBin([folder fileName '-txI.bin'],'short');
    bin2wav(bitStream,16,48e3,[folder fileName '-txI.wav']);
    
%% AMR-WB
    % UE1 processing
    % resampling 48kHz -> 16kHz
    wavIn = convertSampleRate([folder fileName '-txI.wav'],16e3);    
    bitStream = wav2bin(wavIn,14);
    writeBin(bitStream,[folder fileName '-txI.bin'],'short');
    
    % Implement AMR channel
    % UE1 -> MS
    system([channel 'coder.exe ' num2str(modeIn) ' ' [folder fileName '-txI.bin'] ' ' [folder fileName '-txI1.bin'] ]);
    
    % MS processing
    system([channel 'decoder.exe ' [folder fileName '-txI1.bin'] ' ' [folder fileName '-txI12.bin']]);
    
 %   system(['coder ' num2str(modeOut) ' ' [folder fileName '-txI12.bin'] ' ' [folder fileName '-txI2.bin']]);
 %   system(['decoder ' [folder fileName '-txI2.bin'] ' ' [folder fileName '-txI22.bin']]);
    
    system([channel 'coder.exe ' num2str(modeOut) ' ' [folder fileName '-txI12.bin'] ' ' [folder fileName '-txI23.bin']]);
    % MS -> UE2
    system([channel 'decoder.exe '  [folder fileName '-txI23.bin'] ' ' [folder fileName '-txO.bin']]);
    
    bitStream = readBin([folder fileName '-txO.bin'],'short');
    bin2wav(bitStream,16,16e3,[folder fileName '-txO.wav']);

%% Pairphone---RX
    % resampling 16kHz -> 48kHz
    %wavIn = convertSampleRate([folder fileName '-txO.wav'],48e3);
    wavIn = convertSampleRate('rxRaw.wav',48e3);
    bitStream = [zeros(20,1); wav2bin(wavIn,16)];
    out = bitStream;
    writeBin(bitStream,[folder fileName '-txO.bin'],'short');
    system([DIR 'rx ' [folder fileName '-txO.bin'] ' ' [folder fileName '-rx.bin']]);
    
    bitStream = readBin([folder fileName '-rx.bin'],'short');
    bin2wav(bitStream,16,8e3,[folder fileName '-rx.wav']);
    
