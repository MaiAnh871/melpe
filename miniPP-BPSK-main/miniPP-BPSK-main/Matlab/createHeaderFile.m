function createHFile(filename,sample)
%   Create HeaderFile from array 
%   filename: filename.h
%   varname: name of array
%   sample: values of array
%   AUDIO_length: length of array
%   Ex: createHFile('example','A',array,length);

    AUDIO_length = length(sample);
    varname = 'audio_transmit_male_arr';
    j = fopen([filename '.h'],'w');
    fprintf(j, '/* %-40s */\n', [filename '.h']);
    fprintf(j, '/* exported from MATLAB using createHeaderFile.m  */\n\n\n');
    
    fprintf(j, '#ifndef AUDIO_TRANSMIT_MALE_H_\n');
    fprintf(j, '#define AUDIO_TRANSMIT_MALE_H_\n');
    fprintf(j, '#define AUDIO_LEN_MALE %d\n\n', AUDIO_length);
    fprintf(j, 'int16_t %s[AUDIO_LEN_MALE] = {\n\n', varname);
    for i=1:AUDIO_length
          fprintf(j, '%12d,	/* B[%d] */\n', sample(i), i-1);
    end
    fprintf(j,'};\n');
    
    fprintf(j,'#endif /* AUDIO_TRANSMIT_MALE_H_ */');
    fclose(j);

