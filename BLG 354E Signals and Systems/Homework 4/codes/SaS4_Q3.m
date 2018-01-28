syms t;
T=3;
t=0:.1:15;
f1=exp(-mod(t,T));
plot(t,f1)

%****Part a is in up

w0=2*pi/3;
%ak=0;
aks=0;
fx=@(t,k) exp(-mod(t,T)).*exp(-j.*k*t*w0);
for k=-100:100
    k;
ak =  1/3*integral( @(t)fx(t,k), 0,3);
aks=[aks ak]; 
X=[ real(k) ,  ak];
disp( X )
end

%*******^Part b is in up


t=-11:0.1:11;
y=0;
for n=1:201
y= y+ aks(n)*exp(j*(n-101)*w0*mod(t,T));
end
      plot(t,abs(y))
%*******^Part c is in up

      
