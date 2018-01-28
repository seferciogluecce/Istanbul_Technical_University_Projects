
[y, Fs] = audioread('Frank_C_Stanley_-_08_-_When_The_Mockingbirds_Are_Singing_In_The_Wildwood.wav');

yx = zeros(size(y,1),2);

for i=2:size(y,1)
   yx(i,1) = exp(y(i,1)/2.5); 
   yx(i,2) = exp(y(i,2)/2.5); 
end

audiowrite('signal_div_a.wav',yx,Fs,'BitsPerSample',64)

[ww,Fs] = audioread('signal_div_a.wav');

y2 = zeros(size(ww,1),2);

for i=1:size(ww,1)   
   y2(i,1) = log(ww(i,1))*2.5;    
   y2(i,2) = log(ww(i,2))*2.5; 
end

audiowrite('signal_reinverted_a.wav',y2,Fs,'BitsPerSample',64);



[y, Fs] = audioread('Frank_C_Stanley_-_08_-_When_The_Mockingbirds_Are_Singing_In_The_Wildwood.wav');

yx = zeros(size(y,1),2);

for i=2:size(y,1)
     yx(i,1) = y(i,1)+yx(i-1,1); 
   yx(i,2) = y(i,2)+yx(i-1,1); 
end

audiowrite('signal_div_b.wav',yx,Fs,'BitsPerSample',64)


[ww,Fs] = audioread('signal_div_b.wav');

y2 = zeros(size(ww,1),2);

for i=1:size(ww,1)   
   y2(i,1) = ww(i,1)-y2(i+1,1);    
   y2(i,2) = ww(i,2)-y2(i+1,2);    
end

audiowrite('signal_reinverted_b.wav',y2,Fs,'BitsPerSample',64);