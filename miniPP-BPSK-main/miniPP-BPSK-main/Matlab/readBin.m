function data = readBin(fileName,dataType)
%   Read data from binary file
%   Ex: data = read_bin('1.bin','short');
    fid = fopen(fileName,'r');
    data = fread(fid,dataType);
    fclose('all');
