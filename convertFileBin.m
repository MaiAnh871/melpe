function convertFileBin(filename)
    fp = fopen(filename);
    Array = fread(fp,'short');
    convertArray(Array);
    fclose('all');
end