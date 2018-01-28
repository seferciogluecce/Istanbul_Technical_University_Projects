import cv2
import numpy as np

picNum=84

I=[]
num=""
for a in range(picNum): #images read into array
    if a<10:
        num="0"+str(a)
    else:
        num=str(a)        
    temp=cv2.imread("daria_walk/daria_walk_00"+num+".png",0)
    I.append(temp)

binaryImage=[np.zeros([I[0].shape[0],I[0].shape[1]],dtype=np.uint8) for a in range(picNum)] #background substraction result binary image array

bg=np.zeros(I[0].shape) #background model
rows,cols=I[0].shape

for y in range(rows): #model calculated by median of each pixel from all images
    for x in range(cols):  
        d1=[]                 
        for b in range(picNum):
            d1.append(I[b][y][x])
        bg[y][x]=np.median(d1) if len(d1)>0 else 0  
            
cv2.imwrite("backGround.png",bg)

for a in range(picNum):   #binary images are calculated for each image by substracting original image from background model
    for y in range(rows):
        for x in range(cols):  
            binaryImage[a][y][x]=255 if bg[y][x]-I[a][y][x] > 40 else 0        #thresholding output image to get binary image    
    cv2.imwrite('daria_walk_binary_00'+str(a)+'.png',binaryImage[a])

cv2.waitKey(0)
cv2.destroyAllWindows()
