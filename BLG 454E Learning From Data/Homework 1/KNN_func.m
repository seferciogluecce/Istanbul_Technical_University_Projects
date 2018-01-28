function [colour] = KNN_func(A, n, x, y)
max = 0;
maxindex = 0;
output = zeros(n,2);
for c = 1:n
    %output(end+1)
    output(c, 1) = sqrt((A(c,1)-x)^2 + (A(c,2)-y)^2);
    output(c, 2) = A(c,3);
    if max < output(c, 1)
        max = output(c, 1);
        maxindex = c;
    end
end
for p = n+1:2000
    distance = sqrt((A(p,1)-x)^2 + (A(p,2)-y)^2);
    if distance < max
        output(maxindex, 1) = distance;
        output(maxindex, 2) = A(p,3);
        max = distance;
        for c = 1:n
            if max < output(c, 1)
            max = output(c, 1);
            maxindex = c;
            end
        end
    end
end
count0 = 0;
count1 = 0;
for c = 1:n
    if output(c, 2) == 0
        count0 = count0+1;
    else
        count1 = count1+1;
    end
end
if count0>count1
    colour = 0;
else
    colour = 1;
end
count0
count1

