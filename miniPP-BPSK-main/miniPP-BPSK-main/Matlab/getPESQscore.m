function PESQscore = getPESQscore()
    fid = fopen('_pesq_results.txt');
    i = 0;
    tline = fgetl(fid);
    while ~feof(fid)
        
        my_str = strsplit(tline);
        if i == 1
            PESQscore = str2double(my_str{2});
        elseif i > 1
            PESQscore = [PESQscore ; str2double(my_str{3})];
        end
        i = i+1;
        tline = fgetl(fid);
    end
    fclose(fid);
end