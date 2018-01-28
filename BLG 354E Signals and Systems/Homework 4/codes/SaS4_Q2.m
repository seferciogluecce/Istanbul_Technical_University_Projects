syms n;
x=@(n)3*sin(2*pi*n/7+pi/4); %main function to find coef
N=7; %period
w0=2*pi/7;
aks=0;
k=0;
for k=0:6
aks=[ aks eval((1/7*symsum( x(n)*exp(-j*k*w0*n),n,0,6 )))];
 k
ak =aks(end)
end

aks=aks(2:end);
