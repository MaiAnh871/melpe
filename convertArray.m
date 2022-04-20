function convertArray(Array)    
    fd1 = fopen('coeff.c','wb');
    fd2 = fopen('coeff.h','wb');
    fprintf(fd1,'#include "coeff.h"\n');
    fprintf(fd1,'short speechIn[speechLength] = {\n');
    for i=1:length(Array)
       fprintf(fd1,'\t%d,\n',Array(i));
    end
    fprintf(fd1,'};\n');
    
    fprintf(fd2,'#define speechLength %d\n',length(Array));
    fprintf(fd2,'extern short speechIn[];\n');
    
    fclose('all');
end