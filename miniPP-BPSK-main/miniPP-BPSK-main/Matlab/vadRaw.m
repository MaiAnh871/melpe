%   made by hsdfat8
%   date: 02/01/2021

function [vadIdx, vad, modulated] = vadRaw(wavIn)
    % Check vad for raw data
    % Ex:[vadIdx, vad] = vad_raw(wavIn)
    % vadIdx: 1-voiced frame, 0-silenced frame 
    % wavIn: input data 
    % vad: upsample vad, ex: each voiced frame contains 540 samples 1
    %% Pre-processing - Write file .bin
    filePath = '..\signal.bin';
    wavIn = wavIn/max(abs(wavIn));
    writeBin(round(2^15.*wavIn),filePath,'short');
    %% vad---TX
   VADPath = 'VAD\vadIdx.bin';
    txPath = '..\tx.bin';
    %% vad---TX
    system(['..\EXE\vad ' filePath ' ' VADPath]);
    vadIdx = readBin(VADPath,'short');
    system([ '..\EXE\tx ' filePath ' ' txPath]);
    modulated = readBin(txPath,'short');
    
    vad = kron(vadIdx,ones(540,1));
    wavIn = wavIn(1:length(vad));
    system(['del /f ' filePath]);
    system(['del /f ' VADPath]);
    system(['del /f ' txPath]);
end
    