clearvars;
%read from file and initiate test and training data************************
training_file = fopen('optdigits.tra');

tline = fgetl(training_file);
c=strsplit(tline, ',');
str = char(c);
lline=zeros(1,65);
for a=1:65
    lline(a) =str2num(char(c(a)));
end
training=lline;
tline = fgetl(training_file);
while ischar(tline)
    
    c = strsplit(tline, ',');
    for a=1:65
        lline(a) =str2num(char(c(a)));
    end
    training = [training ;lline];
    tline = fgetl(training_file);
end


fclose(training_file);


test_file = fopen('optdigits.tes');

tline = fgetl(training_file);
a=0;
c=strsplit(tline, ',');
str = char(c);
lline=zeros(1,65);
for a=1:65
    lline(a) =str2num(char(c(a)));
end

test=lline;
tline = fgetl(training_file);
while ischar(tline)
    
    c = strsplit(tline, ',');
    for a=1:65
        lline(a) =str2num(char(c(a)));
    end
    
    test = [test ;lline];
    tline = fgetl(test_file);
end


fclose(test_file);

%training****************************************
%below code is implemetated with the aid of https://www.youtube.com/watch?v=TIgfjmp-4BA

%j,d = 64 #features
% K,i #clas  10
%h=10 

dataOriginal = [training; test];

for fold=0:9
    generalerror=0;
    
    
    [coeff]=pca(dataOriginal(:,1:64));
    data= dataOriginal(:,1:64)*coeff(:,1:2);
    
    training = data([1:(fold)*500, (fold+1)*500+1:5000], :);
    test = data(fold*500+1:(fold+1)*500, :);
    H=10;
    K=10;
    d=2;
    
    v = zeros(K,H);
    for i = 1:K
        for j = 1:H
            v(i,j) = -0.01 + (0.01+0.01).*rand(1);
        end
    end
    
    w = zeros(H,d);
    for i = 1:H
        for j = 1:d
            w(i,j) = -0.01 + (0.01+0.01).*rand(1);
        end
    end
    deltaW= zeros(10,2);
    deltaV=zeros(K,H);
    sigmoid=@(x) 1./(1+exp(-x));
    
    X = training(:, (1:2));
    r = dataOriginal([1:(fold)*500, (fold+1)*500+1:5000], 65);
    y = zeros(1, 10);
    past = zeros(10,2);
    rate = 0.002;
    X(randperm(4500),:);
    z=zeros(1,H);
    etta=0.02;
    sum=0;
    
    while (~converge(w,past,H,d))
        for t = 1:4500
            
            for h=1:H
                z(h)= sigmoid(transpose(w(h))*X(t));
            end
           for i=1:K
                y(1,i)=z*transpose(v(i,:));
            end
            
            for i=1:K
                deltaV(i)=etta*(r(t)-y(i))*z(i);
                
            end
            
            
            for h=1:H
                sum=0;
                for i=1:K
                    sum=sum+ (r(t)-y(i))*v(i,h);
                    
                    
                end
                deltaW(h)=etta*sum *z(h)*(1-z(h))*X(t);
            end
            
            
            for i = 1:K
                for j = 1:H
                    v(i,j) =  v(i,j) + deltaV(i,j);
                end
            end
            
            
            for i = 1:H
                for j = 1:d
                    past(i,j)=w(i,j);
                    w(i,j) = deltaW(i,j) + w(i,j);
                end
            end
        end
    end
    
    %test
    k=1;
    confusion(45,4);
    for i=1:K
        for j=1:H
            fp=0;
            tp=0;
            tn=0;
            fn=0;
            
            for m=1:500
                
                
                if dataOriginal(fold*500+m ,65)==i-1
                    if sumCon2(i,m,w,test)>0
                        tp=tp+1;
                    else
                        fn=fn+1;
                    end
                    
                elseif dataOriginal(fold*500+m ,65)==j-1
                    
                    if sumCon2(i,m,w,test)>0
                        fp=fp+1;
                    else
                        tn=tn+1;
                    end
                    
                end
                
                
            end
            
            
            confusion(k,1)=tp ;
            confusion(k,2)=fn ;
            confusion(k,3)= fp;
            confusion(k,4)=tn ;
            k=k+1;
        end
    end
    sumer=0;
    error=zeros(1,45);
    for i=1:46
        error(i)=(confusion(i,1)+confusion(i,4))/(confusion(i,1)+confusion(i,4)+confusion(i,2)+confusion(i,3));
        sumer=sumer+error(i);
    end
    sumer;
    diver=sumer/45;
    generalerror= generalerror+diver;
end
PCAError=generalerror/10
