function folder = PP_in_GSM(fileName,folderFile)
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Implement Pairphone in AMR-WB channel.
% Model in this function:
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
    if (nargin < 2)
        folder = '';
    else 
        folder = [folderFile '/'];
    end % checking folder
%% Pre-processing
    wavIn = convertSampleRate([folder fileName '.wav'],8e3);
    bitStream = wav2bin(wavIn,13);
    writeBin(bitStream,[folder fileName '.bin'],'short');

%% PairPhone---TX
    system([DIR 'tx ' [folder fileName '.bin'] ' ' [folder fileName '-txI.bin']]);
    bitStream = readBin([folder fileName '-txI.bin'],'short');
    bin2wav(bitStream,13,48e3,[folder fileName '-txI.wav']);
    
%% AMR-WB
    % UE1 processing
    % resampling 48kHz -> 16kHz
    wavIn = convertSampleRate([folder fileName '-txI.wav'],8e3);    
    bitStream = wav2bin(wavIn,13);
    writeBin(bitStream,[folder fileName '-txI.bin'],'short');
    
    % Implement GSM channel
    system(['coder ' [folder fileName '-txI.bin'] ' ' [folder fileName '-txI.cod'] ]);
    
    system(['ed_iface ' [folder fileName '-txI.cod'] ' ' [folder fileName '-txI.dec']]);
    
    system(['decoder ' [folder fileName '-txI.dec'] ' ' [folder fileName '-txO.bin']]);
    
    bitStream = readBin([folder fileName '-txO.bin'],'short');
    bin2wav(bitStream,16,8e3,[folder fileName '-txO.wav']);

%% Pairphone---RX
    % resampling 16kHz -> 48kHz
    wavIn = convertSampleRate([folder fileName '-txO.wav'],48e3);
    bitStream = wav2bin(wavIn,16);
    writeBin(bitStream,[folder fileName '-txO.bin'],'short');
    system([DIR 'rx ' [folder fileName '-txO.bin'] ' ' [folder fileName '-rx.bin']]);
    
    bitStream = readBin([folder fileName '-rx.bin'],'short');
    bin2wav(bitStream,16,8e3,[folder fileName '-rx.wav']);
    
