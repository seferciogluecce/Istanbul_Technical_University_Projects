function [ y ] = sumCon2(c1,c2, w,test )
y=0;
for i=1:2
    y=y+ w(c1,i)*test(c2,i);
end
end