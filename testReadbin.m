%% test read file bin
%data = readBin('E:\Melpe1200-2400-main\Melpe1200-2400-main\clean_8k_pcm\speech1.pcm', 'short');
%data = readBin('E:\Melpe1200-2400-main\Melpe1200-2400-main\clean_8k_pcm\compr1.pcm', 'uint16');
data = readBin('E:\Melpe1200-2400-main\Melpe1200-2400-main\clean_8k_pcm\compress_speech5.pcm', 'uint16');
%data = readBin('E:\Melpe1200-2400-main\Melpe1200-2400-main\clean_8k_pcm\decompress_speech1.pcm', 'short');
%data = readBin('E:\Melpe1200-2400-main\Melpe1200-2400-main\clean_8k_pcm\decompr1.pcm', 'short');
normalize_data = data/power (2,15);
sound (normalize_data, 8000);