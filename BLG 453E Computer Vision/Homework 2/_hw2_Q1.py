
import cv2
import numpy as np

def filterValue(i,j,n,pic): #applying filter to pixel and neighbours
    filterR=[]
    rows,cols = pic.shape[:2]
    for x in range(-n+2,n-1):
        for y in range(-n+2,n-1):
            if i+x<0 or i+x>=rows or j+y>=cols or j+y<0: #padding with 0
                filterR.append(0)
            else:
                filterR.append(pic[i+x][j+y])
    return filterR

def meanFilter(picName,filterMagnitude): 
    image = cv2.imread(picName,0)    
    for y in range(len(image)):
        for x in range(len(image[y])):
            image[y][x]=np.mean(filterValue(y,x,filterMagnitude,image))  #mean value of matrix is now pixels value
    cv2.imshow("meanFilter "+picName,image)

    
    
def medianFilter(picName,filterMagnitude):
    image = cv2.imread(picName,0)
    for y in range(len(image)):
        for x in range(len(image[y])):
            image[y][x]=np.median(filterValue(y,x,filterMagnitude,image))  #median value of matrix is now pixels value
    cv2.imshow("medianFilter "+picName,image)


    
def meanMedianFilter(picName,filterMagnitude,alpha):
    image = cv2.imread(picName,0)    
    for y in range(len(image)):
        for x in range(len(image[y])):
            filterVal=filterValue(y,x,filterMagnitude,image)
            image[y][x]=alpha*np.mean(filterVal)+(1-alpha)*np.median(filterVal)  #mean-median value of matrix with distrubuted by alpha is now pixels value
    cv2.imshow("meanMedianFilter "+picName+" alpha="+str(alpha),image)

    
    
pics=["cameramanN1.jpg","cameramanN2.jpg","cameramanN3.jpg"]
filterMagnitude=3
for a in range(3):                         # apply mean filter 
    meanFilter(pics[a],filterMagnitude)
cv2.waitKey(0)
cv2.destroyAllWindows() 
   
for a in range(3):                         # apply medaian filter 
    medianFilter(pics[a],filterMagnitude)
cv2.waitKey(0)
cv2.destroyAllWindows()  
    
alpha=0.32
for a in range(3):                         # apply mean-median filter 
    meanMedianFilter(pics[a],filterMagnitude,alpha)
cv2.waitKey(0)
cv2.destroyAllWindows()   