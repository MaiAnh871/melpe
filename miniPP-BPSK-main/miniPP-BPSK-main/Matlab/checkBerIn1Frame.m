function  [bitError, bitErrorVad] = checkBerIn1Frame(txDataBuf, rxDataBuf)
[~,i] = size(rxDataBuf);
bitError = 0;
bitErrorVad = 0;
    for j = 1:12 %Xor 80 bit trong buf 1-> 10 va bit cuoi buf 11
       if j<11
           bitErrorFrame = sum(de2bi(bitxor(rxDataBuf(j),txDataBuf(j) ))); 
           bitError = bitError + bitErrorFrame;

       elseif j == 11
           bitErrorVadFrame = bitxor(mod(rxDataBuf(j),2),mod(txDataBuf(j),2));
           bitErrorVad = bitErrorVad + bitErrorVadFrame;
       end
    end

end