function [a_k,k] =SaS4_Q1(K,N,P)
ks=0:.1:K*P;

ind= N+1<=ks<=K-N-1;
x= @(ind)0;
ind= mod(abs(ks),K)<=N ;
x=@(ind)1;

plot(ks,x(ks));

%*******^Plot with period P ^********************

%******** *down* Find ak********
syms n;
k=0;
a_k=0;
aks=0;
w0=2*pi/K;
for kk=0:K-1
    k=[k kk];
aks=[ aks eval((1/K*symsum( x(n)*exp(-j*kk*w0*n),n,0,K*P ) ))];
ak =aks(end);
a_k=[a_k ak];
end
k=k(2:end);
a_k=a_k(2:end);
out=aks;
for a=1:P-1
    out=[out aks];
end
stem(out)
%***** print out ak's for values
end