function writePCM(fileName, data)

f = fopen(fileName, 'w');
fwrite(f, data, 'short');
fclose('all');