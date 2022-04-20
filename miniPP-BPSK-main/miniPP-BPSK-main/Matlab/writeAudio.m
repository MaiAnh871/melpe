sample = '22';
wavPath = ['.\VoiceData\wav\raw\' sample '.wav'];
binPath = ['.\VoiceData\bin\' sample '.bin'] ;
[audio,fs] = audioread(wavPath);
audio = resample(audio,8e3,fs);
audio = [zeros(16000,1); audio];
audio = round(audio*2^15);
writeBin(audio,binPath,'short');