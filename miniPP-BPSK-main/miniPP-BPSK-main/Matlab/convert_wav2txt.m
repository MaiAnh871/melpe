for i = 1:5
    A = convertSampleRate([num2str(i) '.wav'],...
                    num2str(i),8e3,'wav_origin','wav_origin');
    %A = audioread(['wav_origin/' num2str(i) '.wav']);
    A = A(:,1);
    A = wav2bin(A,16);
    fid = fopen(['wav_origin/' num2str(i) '.bin'],'w');
    fwrite(fid,A,'short');
    fclose(fid);
end
    
