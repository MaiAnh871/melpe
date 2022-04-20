%   made by hsdfat8
%   date: 02/01/2021
% Check vad for binary file
    % Ex: [vadIdx, vad, wavIn, modulated] = vadBin('1.bin')
    % vadIdx: 1-voiced frame, 0-silenced frame, each frame contains 540 samples 
    % wavIn: data read from file bin
    % vad: upsample vad, ex: each voiced frame contains 540 samples 1
    % modulated: signal after modulated by pairphone
function [vadIdx, vad, wavIn, modulated] = vadBin(filePath)
    
    %% Pre-processing
    wavIn = readBin(filePath,'short');
    wavIn = wavIn/max(wavIn);
    
    VADPath = 'vadIdx.bin';
    txPath = '..\tx.bin';
    %% vad---TX
    system(['..\EXE\vad ' filePath ' ' VADPath]);
    vadIdx = readBin(VADPath,'short');
    system([ '..\tx ' filePath ' ' txPath]);
    modulated = readBin(txPath,'short');
    
    vad = kron(vadIdx,ones(540,1));
    wavIn = wavIn(1:length(vad));
    system(['del /f ' VADPath]);
    system(['del /f ' txPath]);
end
    