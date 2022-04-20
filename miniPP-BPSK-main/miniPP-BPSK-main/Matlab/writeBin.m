function writeBin(bitStream,fileName,dataType)
%   Write binary file from data array
%   Ex: write_bin(data,'fileName.bin','short');
fid = fopen(fileName,'w');
fwrite(fid,bitStream,dataType);
fclose(fid);