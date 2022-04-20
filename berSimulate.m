% Generate input
nSample = 10000;                                % sample
sSample = 16;                                   % bit
lengthSample = nSample * sSample;               % bit
input = randi ([0 1], 1, lengthSample);         % bit

% Generate ber
berI = 0.01;                                    % bit error rate input
nbe = round (berI * lengthSample);              % number of bit error
berbit = zeros (1, lengthSample);               % init bit error array
berbit (randperm (lengthSample, nbe)) = 1;      % set bit error into 1

% Create output
output = mod (input + berbit, 2);               % turn 1 to 0 and 0 to 1

% Calculate ber again
berO = nnz(output - input) / lengthSample;      % bit error rate output