function y = SaS_Q4(a,b)
    w0=pi/3;
syms k;
     t = -11:.1:11;
    a0=1;        
    y=a0+2*symsum( ((1/(2*k*j*pi))*( 1+2*exp(-j*k.*pi)-3*exp(-j*k.*4*pi/3)))*exp(j*k*w0*t),k,[a b]);
    y;
    plot(t,y);
end