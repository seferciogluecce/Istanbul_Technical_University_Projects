import math
import cv2
import numpy as np


def swirl(matriks,xc,yc,weight):  #giving swirl effect
    retMatriks=matriks.copy()
    for y in range(len(matriks)):
        for x in range(len(matriks[y])):   
            p=((x-xc)**2+(y-yc)**2)**0.5  #polar transforms
            theta=(math.atan2(x-xc,y-yc))
            degree=theta+p*weight
            xx=p*np.cos(degree)+xc 
            yy=p*np.sin(degree)+yc
            if xx<xc*2 and yy<yc*2:
                retMatriks[int(yy)][int(xx)][0]=matriks[y][x][0]
                retMatriks[int(yy)][int(xx)][1]=matriks[y][x][1]
                retMatriks[int(yy)][int(xx)][2]=matriks[y][x][2]

    return retMatriks

image=cv2.imread("swirl.jpg")
newImage=image.copy() 

rows,cols,x = image.shape
retMar=swirl(image,cols/2,rows/2,0.03)

cv2.imshow("Swirl",retMar)

cv2.waitKey(0)
cv2.destroyAllWindows()

cv2.imshow("Swirl Vertical Flip",retMar[:,::-1])

cv2.waitKey(0)
cv2.destroyAllWindows()




