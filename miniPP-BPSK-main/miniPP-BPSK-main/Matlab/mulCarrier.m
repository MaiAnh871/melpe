function [Out, bitArray] = mulCarrier(In, car)
    Out = zeros(length(In),1);
    len = length(car);
    bitArray = [];
    for i = 1: length(In)/len
       Out((i-1)*len+1:i*len) = In((i-1)*len+1:i*len).*car'; 
       bitArray = [bitArray (sum(Out((i-1)*len+1:i*len))>0)];
    end
end