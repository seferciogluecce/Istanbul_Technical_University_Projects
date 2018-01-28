import cv2
import numpy as np
from matplotlib import pyplot as plt

I=cv2.imread("blob.png",0)

pdf,Bins,Patches=plt.hist([a for a in I.ravel()],256,[0,256],[0,1],color = "blue")  #pdf of image is calculated
plt.title("Histogram")
plt.show()

maxBCV=0  #between class variance value
optimumTs=(0,2)

mmG=0
for i in range(len(pdf)):  #mean global intensity calculation
    mmG+=i*pdf[i]
    
mG=(1/np.sum(pdf))*mmG

for a in range(len(pdf)-2):       #to find threshold possible values are iterated through
    for b in range(a+2,len(pdf)-1):
                        
        P1=np.sum(pdf[:a+1])     #probability distribution of each class
        P2=np.sum(pdf[a+1:b+1])
        P3=np.sum(pdf[b+1:])

        mm1=0
        mm2=0
        mm3=0

        for i in range(0,a+1): #mean intensity of each class
            mm1+=i*pdf[i]
        for i in range(a+1,b+1):
            mm2+=i*pdf[i]        
        for i in range(b+1,len(rN)):
            mm3+=i*pdf[i]        

        m1=(1/P1)*mm1 if P1!=0 else 0
        m2=(1/P2)*mm2 if P2!=0 else 0
        m3=(1/P3)*mm3 if P3!=0 else 0

        tempBCV= P1*(( mG-m1 )**2)  +   P2*((mG- m2  )**2)  +   P3*((mG-m3 )**2)  #between class variance calculated
        if tempBCV>maxBCV:  #acording to found value maxBCV and optimum thresholds get updated
            maxBCV=tempBCV
            optimumTs=(a,b)

print("Thresholds",optimumTs)

fC=np.zeros(I.shape[:2],dtype=np.uint8)    #image with different classes get filled by thresholds found
sC=np.zeros(I.shape[:2],dtype=np.uint8)
tC=np.zeros(I.shape[:2],dtype=np.uint8)
other=np.zeros(I.shape[:2],dtype=np.uint8)

rows,cols =I.shape[:2]

for y in range(rows):
    for x in range(cols):   
        if I[y][x]<optimumTs[0]:
            fC[y][x]=I[y][x]
        elif I[y][x]>=optimumTs[0] and I[y][x]<optimumTs[1]:
            sC[y][x]=I[y][x]            
        else:
            tC[y][x]=I[y][x]

cv2.imshow("Original Image",I)
cv2.imshow("First Class Threshold",fC)
cv2.imshow("Second Class Threshold",sC)
cv2.imshow("Third Class Threshold",tC)

cv2.waitKey(0)
cv2.destroyAllWindows()
print(optimumTs)

