% Set input 
compr_input = de2bi (data, 16, 'left-msb');             % convert decimal to 16 bit binary
compr_bin = reshape (compr_input', 1, []);              % reshape into 1 row
compr_length = length (compr_bin);                      % length of input data

% Generate ber
berI = 0.00;                                            % bit error rate at input
compr_nbe = round (berI * compr_length);                % number of bit error
compr_berbit = zeros (1, compr_length);                 % init bit error array
compr_berbit (randperm (compr_length, compr_nbe)) = 1;  % set bit error to 1

% Create output
compr_output = mod (compr_bin + compr_berbit, 2);     % turn 1 to 0, 0 to 1

% Calculate ber again
compr_berO = nnz (compr_output - compr_bin) / compr_length;
