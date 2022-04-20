%   made by hsdfat8
%   date: 02/01/2021


%   Giai dieu che tin hieu thu duoc tao thanh buffer 
%   rxBuff = BPSKDemodPP(rxModul)
%   rxBuff - Output: buffer kieu char 12 phan tu
%   rxData - Input:  RX demodulated - tan so 48Khz
function [rxBuff,ReOutputBit] = BPSKDemodPP(rxData)
    global f180 fd align blk fg qq falign 
    global lock fr u ffg dr cnt r rr lag bitOut 
    global countbit countframe
    countbit = 0; 
    countframe = 0;
    bitOut = zeros(90,1);
    falign = 50;
    r = uint32(zeros(1,9));
    rr = uint32(0);
    fg = zeros(4,36);ffg = zeros(4,36);
    dr = 0;
    lock = 0;
    falign = 50;
    lastb = 0;
    vadtr = 0;
    fr = zeros(90,1);
    fd = zeros(90,1);
    ffd = zeros(90,1);
    lag = 1;
    cnt = 0;
    cq = 0;
    mlag = 0;
    blk = 0;
    align = 1;
    f180 =0;
    u = int32(0);
    pointer = 0;
    rxBuff = [];
    ReOutputBit = [];
    while (pointer+36*25<length(rxData))
        [nuSampleProcessed, data2] = BPSKDemodulated(rxData,pointer);
        pointer = pointer + nuSampleProcessed;
        if(bitand(data2(12),128)~=0)
            rxBuff = [rxBuff data2];    
            
        end    
        if countbit == 90
            ReOutputBit = [ReOutputBit bitOut];
        end
    end 
end
