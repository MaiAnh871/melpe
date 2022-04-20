function folder = PP_in_AMR_NB(fileName,num_modeIn,num_modeOut,folderFile)
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

%convert number of mode to mode's name

    DIR = 'c:/voice_compress/BKIC-pairphone/';

switch num_modeIn
    case 0
        modeIn = 'MR475';
    case 1
        modeIn = 'MR515';
    case 2
        modeIn = 'MR59';
    case 3
        modeIn = 'MR67';
    case 4
        modeIn = 'MR74';
    case 5
        modeIn = 'MR795';
    case 6
        modeIn = 'MR102';
    case 7
        modeIn = 'MR122';    
end

switch num_modeOut
    case 0
        modeOut = 'MR475';
    case 1
        modeOut = 'MR515';
    case 2
        modeOut = 'MR59';
    case 3
        modeOut = 'MR67';
    case 4
        modeOut = 'MR74';
    case 5
        modeOut = 'MR795';
    case 6
        modeOut = 'MR102';
    case 7
        modeOut = 'MR122';    
end

    if (nargin < 4)
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
    bin2wav(bitStream,16,48e3,[folder fileName '-txI.wav']);
    
%% AMR-WB
    % UE1 processing
    % resampling 48kHz -> 8kHz
    wavIn = convertSampleRate([folder fileName '-txI.wav'],8e3);    
    bitStream = wav2bin(wavIn,13);
    writeBin(bitStream,[folder fileName '-txI.bin'],'short');
    
    % Implement AMR channel
    % UE1 -> MS
    
    system(['encoder ' modeIn ' ' [folder fileName '-txI.bin'] ' ' [folder fileName '-txI1.bin'] ]);
    
    % MS processing
    system(['decoder ' [folder fileName '-txI1.bin'] ' ' [folder fileName '-txI12.bin']]);
    system(['encoder ' modeOut ' ' [folder fileName '-txI12.bin'] ' ' [folder fileName '-txI2.bin']]);
    
%     system(['decoder ' [folder fileName '-txI2.bin'] ' ' [folder fileName '-txI22.bin']]);
%     system(['encoder ' modeOut ' ' [folder fileName '-txI22.bin'] ' ' [folder fileName '-txI2.bin']]);
    
    % MS -> UE2
    system(['decoder '  [folder fileName '-txI2.bin'] ' ' [folder fileName '-txO.bin']]);
    
    bitStream = readBin([folder fileName '-txO.bin'],'short');
%     bitStream = readBin([folder fileName '-txI.bin'],'short');
    bin2wav(bitStream,13,8e3,[folder fileName '-txO.wav']);

    
%% Pairphone---RX
    % resampling 8kHz -> 48kHz
    wavIn = convertSampleRate([folder fileName '-txO.wav'],48e3);
    bitStream = wav2bin(wavIn,16);
    writeBin(bitStream,[folder fileName '-txO.bin'],'short');
    system([DIR 'rx ' [folder fileName '-txO.bin'] ' ' [folder fileName '-rx.bin']]);
    
    bitStream = readBin([folder fileName '-rx.bin'],'short');
    bin2wav(bitStream,16,8e3,[folder fileName '-rx.wav']);
    
