for idx = 1
    sample = num2str(idx);
    dataInPath = [sample '.bin'];
    dataOutPath = [sample '.melpe'];
    dataInWavPath = [sample '.wav'];
    dataOutWavPath = [sample 'melp.wav'];
    dataIn = readBin(dataInPath,'short');
    dataOut = readBin('1.meple','short');
    pesqEXE = 'C:\Project\voice-compress\BKIC-pairphone\EXE\';
    audiowrite(dataInWavPath,dataIn/2^15,8e3);
    audiowrite(dataOutWavPath,dataOut/2^15,8e3);
    system([pesqEXE 'PESQ +8000 ' dataInWavPath ' ' dataOutWavPath]); 
end