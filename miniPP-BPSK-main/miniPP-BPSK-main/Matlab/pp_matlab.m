function modulate = pp_matlab(filename)
    system(['melpe ' [filename '.bin'] ' ' [filename '_melpe.bin'] ]);
    data_melpe= readBin('1_melpe.bin','unsigned char');
end
