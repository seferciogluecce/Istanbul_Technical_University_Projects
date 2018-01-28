function [ y ] = sumCon(c1,c2, w,test )
y=0;               
for i=1:64
    y=y+ w(c1,i)*test(c2,i);
end
end

