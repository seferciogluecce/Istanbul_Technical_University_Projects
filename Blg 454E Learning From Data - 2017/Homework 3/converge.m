function y = converge( w,past,K,d ) % if little to no change presented on weight values 
counter = 0  ;                        %returns false as convergence is met
for i=1:K
    
    for j=1:d
        if abs(past(i,j)-w(i,j))>0.0001
            counter = counter +1;
        end
    end
end

if counter >0
    y =  false;
else
    y= true;
end
sumw=sum(w);
end

