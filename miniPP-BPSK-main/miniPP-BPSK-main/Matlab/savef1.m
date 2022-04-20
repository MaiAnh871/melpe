v = yt;
v = v/max(abs(v));
v = resample (v,48,8);
writeBin(2^14*v,'2-mod.bin','short');
data_mod = readBin('2-rx.bin','short');
data_mod = data_mod/max(abs(data_mod));