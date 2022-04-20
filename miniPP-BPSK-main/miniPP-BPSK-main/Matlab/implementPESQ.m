function implementPESQ(num,folder)
 
        REF = [folder '/' num '.wav'];
        DEG = [folder '/' num '-rx.wav'];
        system(['PESQ +8000 ' REF ' ' DEG]);
