A = importdata('hw1.mat');

b=size(A);
b=b(1,1);
onesy=zeros(1,3);
zerosy=zeros(1,3);
sayac1=0;
sayac0=0;

for a = 1:b
    if (A(a,3)==1)
        sayac1=sayac1+1;
        onesy(sayac1,1)=A(a,1);
         onesy(sayac1,2)=A(a,2);
          onesy(sayac1,3)=A(a,3);
    end
    if (A(a,3)==0)
        sayac0=sayac0+1;
        zerosy(sayac0,1)=A(a,1);
         zerosy(sayac0,2)=A(a,2);
          zerosy(sayac0,3)=A(a,3);
    end
end
prompt = 'What is K? ';
n = input(prompt);
scatter(zerosy(1:1000,1),zerosy(1:1000,2),'b', 'filled');hold on;
title (['KNN', num2str(n)]);
legend('class 0','class 1');
xlabel('x');
ylabel('y');
scatter(onesy(1:1000,1),onesy(1:1000,2),'r', 'filled');hold on;
px = [];
py = [];
[px,py]=ginput(1);
scatter(px, py, 'g', 'filled')
renk = KNN_func(A, n, px, py);
title (['KNN', num2str(n), ' Your input belongs to class ', num2str(renk)]);

