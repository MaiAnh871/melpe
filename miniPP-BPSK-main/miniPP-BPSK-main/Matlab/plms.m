function [yhat, e] = plms(main)
mu=1/2^6;
bhat(:,1) = transpose(zeros(1,32));
W = zeros(1,32);
for i=1:length(main)
    if (i==1) bhat(:,i+1)=bhat(:,i);
    else bhat(:,i+1)=bhat(:,i)+mu*e(i-1)*(transpose(W));
    end
       W=[W(2:32) main(i)];
    y=main(i);
    yhat(i)=W*bhat(:,i);
    e(i)=y-yhat(i);
end