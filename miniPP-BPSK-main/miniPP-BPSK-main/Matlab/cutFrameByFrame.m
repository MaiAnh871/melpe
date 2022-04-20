function txdata3 = cutFrameByFrame(txdata)
txdata3 = [];
for i = 1: length(txdata)/14
   txdata2 = txdata(14*(i-1)+1:14*i);
   txdata3 = [txdata3 txdata2];
end