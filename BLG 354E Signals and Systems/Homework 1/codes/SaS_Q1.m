A=[1 2 2 2 2];
omega0=[pi/6 pi/6 pi/6 pi/6 1/6];
theta=[0 0 4*pi pi pi];
n=0:30;

for a = 1:5
    figure(a)
    stem(A(a)*cos(omega0(a)*n+theta(a)))
    title([num2str(a)])
    axis([0 30 -2.5 2.5])
end