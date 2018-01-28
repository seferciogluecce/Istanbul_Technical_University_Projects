absC=1;
omega0=[2*pi 4*pi -2*pi 2*pi];
r=[1 1 1 -1];
t=0:.1:10;
theta=2*pi;
for z = 1:4
    C=absC*exp(j.*theta); 
    a=r(z)+j*omega0(z) ;
    xenvu=abs(C)*exp(r(z)*t); 
    xenvd=-abs(C)*exp(r(z)*t);
    func= C.*exp(a.*t);
    figure(z)
    plot(t,real(func),t,imag(func),'k:',t,xenvu,'k--',t,xenvd,'k--') 
    title([num2str(z)])
    legend('Real part','Imaginary part')     
    title([num2str(z)])
end