function nameWav = pcm2wav(fileName, fsIn, fsOut)
% fileName: name of PCM-file
% fs: sample rate (should be 48e3)
% example: pcm2wav('speech.pcm', 48e3)
f = fopen(fileName, 'r');
data = fread(f, 'short');
data = resample(data, fsOut, fsIn);
nameWav = [fileName(1:length(fileName)-3) 'wav']
audiowrite(nameWav, int16(data), fsOut, 'BitsPerSample', 16);
fclose('all');