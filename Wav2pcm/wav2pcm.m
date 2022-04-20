function data = wav2pcm(fileName, readWav)
% convert WAV/M4A/MP3-file to PCM-file 48KHz
% example: wav2pcm('audio.wav')
[data, sampleRate] = audioread(fileName);
% sampleRate
a = size(data);
if a(2) ~= 1
    data = mean(data, 2);
end
data = resample(data, 8000, sampleRate);
data = int16(data*2^15);
namePcm =  [fileName(1:length(fileName)-3) 'pcm']
if nargin == 1
    f = fopen(namePcm, 'w');
    fwrite(f, data, 'short');
    fclose('all')
else
    if readWav
    end
end