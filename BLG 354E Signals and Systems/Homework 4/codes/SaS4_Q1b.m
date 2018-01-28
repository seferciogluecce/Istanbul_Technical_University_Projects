
SaS4_Q1(10,1,2);


t=-11:0.1:11;
y=0;
for n=1:10
y= y+ a_k(n)*exp(j*(n)*w0*mod(t,10));
end
plot(t,abs(y))