%% Set input 
hex = 16;                                               % hexa
row = 6;                                                % number of row each frame
space = hex * row;                                      % number of bit each frame
bin_es = de2bi (data, hex, 'left-msb');                 % convert decimal to 16 bit binary (5094x16)
trp_bin_es = bin_es.';                                  % transpose the matrix in to 16x5094
rs_bin_es = reshape (trp_bin_es, space, []);            % reshape into 96x849

% Get 81 mod bin out of 96 bit read from file
% Remove bit 81 to bit 95 each frame
binM = rs_bin_es;                                       % copy into mode one
binRm = binM (81:95, :);                                % removed array (15x849)
binM (81: 95, :) = [];                                  % remove edundant bit (81x849)
rs_binM = reshape (binM, 1, []);                        % data pre processing (1x68769)
binLength = length (rs_binM);                           % length of data
 
%% Generate ber
berI = 0.005;                                           % bit error rate at input
nbitEr = round (berI * binLength);                      % number of bit error
bitE_ar = zeros (1, binLength);                         % init bit error array
bitE_ar (randperm (binLength, nbitEr)) = 1;             % set bit error to 1
 
%% Create output
binEr = mod (rs_binM + bitE_ar, 2);                     % turn 1 to 0, 0 to 1
rs_binEr = reshape (binEr, 81, 849);                    % reshape in to 81x849

% Add the bit removed
lastRm = rs_binEr (81, :);                              % get the last row (bit 81)
rs_binEr (81, :) = [];                                  % remove bit 81
add1_binEr = [rs_binEr; binRm];                         % add edundant bit
add2_binEr = [add1_binEr; lastRm];                      % add bit 81 (96x849)
rs_binOut = reshape (add2_binEr, 16, 5094);             % reshape into 16x5094
trp_binOut = rs_binOut.';                               % transpose into 5094x16
output = bi2de (trp_binOut, 'left-msb');                % convert binary to decimal

%% Calculate ber again
berO = nnz (bin_es - trp_binOut) / (5094/6*81);
