%   made by hsdfat8
%   date: 02/01/2021

function [nuSampleProcessed, rxDataBufAfterDemodulated]= BPSKDemodulated(speechData,pointerIn)
    shape
    global f180 fd align blk qq falign countbit countframe
    global lock fr u ffg dr cnt r rr lag bitOut
    lastbit = 0;
    sp = speechData;
    pointer = pointerIn+9;
    eg = zeros(1,36);
    %lock phase
     
    
    for i = 1:24
       for j= 1:36
            k = (i-1)*36+j;
            eg(j) = eg(j)+abs(sp(pointer+k)-sp(pointer+k+18));
       end
    end
    [k , q] = max(eg);
    q = q -1;
    f180 = f180*0.9;
    
    if (q>17)
        q = q-18;
        f180 = f180 - 1;
    else
        f180 = f180 + 1;
    end
   
    if(abs(f180)<1)
      if (lock ~= 0)
          fr = zeros(90,1);
          lock = 0;  
      end
    elseif (abs(f180)>9)
       lock = 1;
    end
   
    q = q - 9;
    if (f180<0) 
       pointer = pointer +18;
    end
    pointer = pointer + q;
    data = zeros(12,1);
    data(11) = bitand(data(11),hex2dec('7F'));
   subplot(211);
   plot(sp(pointer:pointer+216))
   %da~ dong bo duoc pha va tan so, bat dau giai dieu che bit
    for k = 1:6
       pj = (k-1)*36+1;
       u = 504 * u;
       aa = sp(pointer+pj:pointer+pj+35);
       u = u + int32(sum(sp(pointer+pj:pointer+pj+35)));
       if u>0
           u = int32(u/540);
       else
           u = int32(ceil(double(u)/540));   
       end
   
   
        spn = double(sp(pointer+pj:pointer+pj+35) - double(u));
        

        ffg0=0; 
        ffg1=0;
        ffg2=0;
        ffg3=0;
        ffg0 = ffg0+sum(spn.*ffg(1,:)');
        ffg0 = ffg0-sum(spn.*ffg(2,:)');
        ffg1 = ffg1+sum(spn.*ffg(3,:)');
        ffg1 = ffg1-sum(spn.*ffg(4,:)');

        if(abs(ffg1)>abs(ffg0)) 
           ffg0=ffg1;
        end
        if(ffg0>=0) 
            bbb=0; 
        else
            bbb=1;
        end

        lastbit = bbb+bitxor(lastbit,bbb)*2;   
        f = 0;

        
        ffg (lastbit+1,:) = ffg (lastbit+1,:)*0.95;
        ffg (lastbit+1,:) = ffg (lastbit+1,:) + (sp(pointer+pj:pointer+pj+35))'-double(u);
        f = f + sum(abs(ffg (lastbit+1,:)));
         f = f  / 48;   

        if(f==0)
            f = 1; 
        end
        ffg0 = ffg0/f;
        lastbit = bbb;

        b = bbb;      
        countbit = countbit + 1;
        if countbit == 91
            countframe = countframe + 1;
            countbit = 1;
        end
        
        if countframe == 9 &&  countbit == 70
                hello = 0
        end
        subplot(212);
        plot(spn);
        title(['Frame: ' num2str(countframe+1) ' q = ' num2str(q) ' bitcount: ' num2str(countbit) 'bit decoded: ' num2str(bbb)]);
        %tim dong bo khoi
        dr = uint16(dr);
        dr = bitshift(dr,1);
        dr = bitor(dr,b); 

        j = cnt*6+ (k-1) +1;
        i = mod(j-1,9);
        bitOut(j) = b;
        r(i+1) = uint32(r(i+1));
        r(i+1) = bitor(bitshift(r(i+1),1),b);


        p = bitand(hex2dec('3FF'),r(i+1));
        p = bitxor(p, bitshift(p,-1));
        p = bitxor(p, bitshift(p,-2));
        p = bitxor(p, bitshift(p,-4));
        p = bitxor(p, bitshift(p,-8));

        rr = bitor(bitshift(rr,1),bitand(p,1));

        if (lock ~= 0)
           fr(j)  = fr(j) *0.999;
        else
           fr(j)  = fr(j) *0.99;
        end

        if (mod(rr,2) == 1)
           p = bitshift(rr,-1);
           fr(j) = fr(j) + sum(~de2bi(bitand(p,255),8));
        end
        
        if j == 90
            test = 0;
        end
        i = (j-lag)-1;
        if (i<0) 
            i = i + 90 ;
        end

        ge = sum(spn.*spn);
        ge = sqrt(ge);
        if (ge<1) 
           ge = 1; 
        end
        fd(i+1) = (ffg0/ge);

        if( j == lag)
            data = zeros(12,1);

            kk = 1;
            bb = 0;
            for ii = 1:9
               f = 100000;
               if (ii == 9)
                  p = 1;
               else
                  p = 0;
               end
               for jj = 1:10 
                    j = (jj-1)*9 + ii; 
                  	if (fd(j) > 0)
                        b = 1;
                    else
                        b = 0;
                    end
                    
                   if (abs(fd(j))<f)
                       f=abs(fd(j)); 
                       pp=kk;
                   end
               if(jj<10)
                  if (b) 
                      data(floor((kk-1)/8)+1) = bitxor(data(floor((kk-1)/8)+1),2^(mod(kk-1,8)));

                  end
                  p = bitxor(p,b);
                  kk = kk + 1;
               elseif (b ~= p)
                  bb = bb +1;
                  if (pp ~= kk)
                     data(floor((pp-1)/8)+1) = bitxor(data(floor((pp-1)/8)+1),2^(mod(pp-1,8)));
                  end
               end





               end


            end
            data(12) = bb;
            falign = falign*0.8;
            falign = falign + bb;

            if (((falign) > 40) && (align ~=0))
                align = 0;
                fr = zeros(90,1);
                ffg(1:2,:) = sin48Khz(1:2,:);
                ffg(3:4,:) = ffg(1:2,:);
                i = 0;
            elseif ((falign<30)&&(align==0))
                align = 1;
            end

            if (blk == 0)
                data(12) = bitor(data(12),hex2dec('80')); 
                blk  = 1;
            end
        end
    end
    
    cnt = cnt +1;
    if cnt >= 15
        cnt = 0;
        f = 0;
        for i = 1:90
            if (f <= fr(i))
               f= fr(i);
               lag =i;
            end
        end
        if (blk == 0)
            data(12) = bitor(data(12) , hex2dec('8F'));
        else
            blk = 0;
        end
    end
    
    data(11) = data(11) + (lag-1)*2;
    
    if(align)
        data(12) = bitor(data(12) , hex2dec('40'));
    end
    
    if(lock)
        data(12) = bitor(data(12) , hex2dec('20'));
    end
    
%     if(qq>50)
%         data(12) = bitor(data(12) , hex2dec('10'));
%     end
    nuSampleProcessed = 216+q;
    rxDataBufAfterDemodulated = data; 
end