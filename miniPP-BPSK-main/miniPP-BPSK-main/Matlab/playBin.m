function playBin(fileName)
<<<<<<< HEAD
% Convert file .bin to audio (16bits,8Khz) and play it.
fid = fopen(fileName, 'r');
sample = fread(fid, 'short');

audio = sample/max(abs(sample));
sound(audio, 8e3);
=======
% Dung de play file .bin ra audio 8Khz
    fid = fopen(fileName, 'r');
    sample = fread(fid, 'short');
    
    audio = sample/2^15;
    sound(audio, 8e3);
    fclose('all');  
>>>>>>> txrx8K-dongnh
