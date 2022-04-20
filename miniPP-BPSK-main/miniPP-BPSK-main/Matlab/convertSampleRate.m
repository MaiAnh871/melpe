function wavOut = read_wav(fileName,numBit)
%   Read wav file to 16bit-integer array
%   Ex: wavOut = read_wav('1.wav',16);

    [wavOut,Fs] = audioread(fileName);
    wavOut = resample(wavOut,8e3,Fs);
    wavOut = mean(wavOut,2);
    wavOut = round(wavOut*2^(numBit-1));
    wavOut = int16(wavOut);



    
