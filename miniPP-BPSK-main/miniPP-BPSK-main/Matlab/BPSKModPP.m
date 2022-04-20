%   made by hsdfat8
%   date: 02/01/2021

function [transmittSignal, outputBit] = BPSKModPP(txBuff)
    % Modulate data to signal to transmit
    % Ex: [transmittSignal, outputBit] = modulated(txBuff)
    %       txBuff: bitstream input, 81 bits for one frame  (12 buff each frame)
    %       outputBit: bitstream output interleaved and include 9 parity bit
    %           Ex: input:  b0 b1 b2 ... b9 b10 ... b71 b72 b73 b74 .. b80
    %               output: b0 b9 ... b63 b72 b1 b10 ... b73 ... b8 .. b80   p0 p1 p2 p3 ...p8  (p - parity bit)
    %                   p0 = b0 ^ b9 ^ b18 ^ ... ^ b72
    %                   p1 = b1 ^ b10 ^ b19 ^ ... ^ b73
    %                   ....
    %                   p7 = b7 ^ b16 ^ b25 ^ ... ^ b79
    %                   p8 = b8 ^ b17 ^ b26 ^ ... ^ b80 ^ 1    
    %       signal: signal after modulate
    shape                       % create sine-signal for 1 cycle
    [~,N] = size (txBuff);
    NoFrame = N;                % number of frame
    lastb = 0;                  % last bit
    vadtr = 0;                  % after each frame, change the maximum amplitude of the symbol according to the variable vadtr
    outputBit = [];
    
    transmittSignal = [];       % signal are modulated for transmit
    for m=1:NoFrame
        outputBitFr = [];
        parity = zeros(1,9);    % parity bits for each frame
        parity(9) = 1;          
        buf = txBuff(:,m);    % input buff: each frame have 12 elements
        frameModulate = [];     % modulate each frame
        for i=1:10
            for j = 1:9
               if i==10         % add parity bit
                   bit = parity(j);
                   
               else
                   k = 9*(j-1)+i;       % 
                   if bitand(buf(floor((k-1)/8)+1),2^(mod(k-1,8))) ~= 0
                       bit = 1;
                   else
                       bit = 0;
                   end
                   
                   parity(j) = xor(parity(j),bit);   % parity
               end
               outputBitFr = [outputBitFr; bit];
               isi = xor (bit,lastb);    % avoid isi when change from 0 to 1 or 1 to 0
               lastb = bit;
               if isi
                  bit = bit + 2;
               end
               if vadtr
                  bit = bit + 4; 
               end
               sp = sin8Khz(bit+1,:)';                      % create wave
               frameModulate = [frameModulate ; sp];        % add symbol's wave into frame
            end
        end
        outputBit = [outputBit outputBitFr];
        transmittSignal = [transmittSignal; frameModulate]; % add frame into signal
        vadtr = xor (vadtr,1);
    end
end