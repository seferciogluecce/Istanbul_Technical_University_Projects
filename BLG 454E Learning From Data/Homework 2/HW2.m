%Güllü Katık 150130109
%Ece Naz Sefercioğlu 150130140

A = importdata('data_logistic.mat');
B = A([1:78, 109:180], :);
b=size(B);
b=b(1,1);
onesy=zeros(1,3);
zerosy=zeros(1,3);
sayac1=0;
sayac0=0;
for a = 1:b
    if (B(a,3)==1)
        sayac1=sayac1+1;
        onesy(sayac1,1)=B(a,1);
         onesy(sayac1,2)=B(a,2);
          onesy(sayac1,3)=B(a,3);
    end
    if (B(a,3)==0)
        sayac0=sayac0+1;
        zerosy(sayac0,1)=B(a,1);
         zerosy(sayac0,2)=B(a,2);
          zerosy(sayac0,3)=B(a,3);
    end
end
scatter(zerosy(1:sayac0,1),zerosy(1:sayac0,2),'b');hold on;
title (['Data']);
scatter(onesy(1:sayac1,1),onesy(1:sayac1,2),'r');hold on;
legend('class 0','class 1');
xlabel('x1');
ylabel('x2');
X = A([1:78, 109:180], [1, 2]);
X = [ones(150,1) X];
Y = A([1:78, 109:180], 3);
test = A([79:108], :);

w = zeros(1,3);
sigmoid=@(x) 1./(1+exp(-x));

for j = 1:3
    w(j) = -0.01 + (0.01+0.01).*rand(1);
end
past1=1;
past2=1;
past3=1;
o=0;
deltaw=zeros(1,3);
learningfac=1;
sayac = 0;
while abs(w(1)-past1) > 0.001 & abs(w(2)-past2) > 0.001 & abs(w(3)-past3) > 0.001
    sayac = sayac + 1;
    past1=w(1);
    past2=w(2);
    past3=w(3);
    for j=1:3
        deltaw(j)=0;
    end
    for t = 1:150
        o=0;
        for j=1:3
            o=o+w(j)*X(t,j); 
        end
        y=sigmoid(o);
        for j=1:3
            deltaw(j)=deltaw(j)+(Y(t)-y)*X(t,j);
        end
    end
    for j=1:3
        w(j)=w(j)+learningfac*deltaw(j);
    end
end

syms x1;
x2 = @(x1)(w(1) + w(2)*x1)/-w(3);
x1 = 0:.01:.1;
plot(x1, x2(x1));

true_count=0;
false_count=0;
for i = 1:30
    if x2(test(i,1)) > test(i,2) & test(i,3) == 1
        true_count = true_count+1;
    elseif x2(test(i,1)) < test(i,2) & test(i,3) == 0
        true_count = true_count+1;
    else
        false_count = false_count+1;
    end
end
for i=1:30
    if test(i,3) == 0
        scatter(test(i,1),test(i,2),'b', 'filled');hold on; %data in test dataset belongs to class 0
    else
        scatter(test(i,1),test(i,2),'r', 'filled');hold on; %data in test dataset belongs to class 1
    end
end

disp(['Iteration    ', 'Learning Rate    ', 'True Predictions    ', 'False Predictions    ']);
disp([sayac, learningfac, true_count, false_count]);
