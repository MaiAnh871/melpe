function [wavOut, pathWavOut]=implementAMR(wavIn,nameSample,mode,...
                              folderIN,folderCODER,folderOUT,folderWavOut)
    % Create file.inp
    INP = [folderIN '/' nameSample '.inp'];
    
    data = wav2bin(wavIn, 16);
    fid = fopen(INP,'w');
    fwrite(fid,data,'int16');
    fclose(fid);
    
    % Create folder path for file.cod and file.out
    COD = [folderCODER '/' nameSample 'mode' num2str(mode) '.cod'];
    OUT = [folderOUT '/' nameSample 'mode' num2str(mode) '.out'];
    
    % Execute operating system command
    [~,~] = system(['coder ' num2str(mode) ' ' INP ' ' COD]);
    [~,~] = system(['decoder '  COD ' ' OUT]);
    
    % Convert to file.wav
    fid = fopen(OUT);
    data = fread(fid,'int16');
    pathWavOut = [folderWavOut '/' nameSample 'mode' num2str(mode) '.wav'];
    wavOut = bin2wav(data,14,pathWavOut);
    fclose(fid);
