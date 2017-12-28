'''
Ece Naz Sefercioğlu
150130140
Hw4 - Q2
'''

import cv2
import numpy as np


def thresholdM(M,threshold): # thresholding the image by applying nonmaxima
    MT=M.copy()
    rows,cols =M.shape[:2]

    for y in range(rows):
        for x in range(cols):   
            if M[y][x]<threshold:
                MT[y][x]=0
            else:              
                MT[y][x]=255
                      
    return MT
def eucledianDist(x0,y0,x1,y1):  #distance calculated by pixel distance 
    return ((x0-x1)**2+(y0-y1)**2)**0.5

def valueDist(I,x0,y0,x1,y1): #distance calculated by pixel value 
    return np.abs(int(I[y0][x0])-int(I[y1][x1]))

def knnDist(I,y1,x1,y0,x0):#knn distance for data
    alpha=0.45 #weight for values
    return valueDist(I,x0,y0,x1,y1)*alpha+(1-alpha)*eucledianDist(x0,y0,x1,y1)

def classify(y1,x1,dataSet,n):
    points=[]
    for y in range(len(dataSet)):
        points.append([y1,x1,dataSet[y][0],dataSet[y][1],knnDist(I,y1,x1,dataSet[y][0],dataSet[y][1]),dataSet[y][2]])
    points=sorted(points,key=lambda x:x[4]) #traning data set is sorted by ascending to knn distance
    #print(points)
    c0=0
    c1=0
    for a in range(n): #for n class class numbers are counted
        if points[a][5]==0:
            c0+=1
        else :
            c1+=1
    if c1>c0: #higher class number is chosen for data's class
        return 1
    else: 
        return 0
def implementedKnn(data,maskedPoints): #Filling the image by classes 175 for healthy segment, 2555 for tumor
    rows,cols=maskedPoints.shape[:2]
    endData=np.zeros([rows,cols],dtype=np.uint8) #all brain
    endData2=np.zeros([rows,cols],dtype=np.uint8) #only tumor data

    for y in range(rows):
        for x in range(cols): 
            if maskedPoints[y][x]==0:
                if data[y][x][1]==0:
                    endData[y][x]=175
                else:
                    endData[y][x]=255
                    endData2[y][x]=255 

    return endData,endData2

def KNN(trainData,M,n,maskedPoints): #knn function
    rows,cols =M.shape[:2]
    data=np.zeros([rows,cols,2])
    for y in range(len(trainData)):
        data[trainData[y][0]][trainData[y][1]][0]=M[trainData[y][0]][trainData[y][1]]
        data[trainData[y][0]][trainData[y][1]][1]=trainData[y][1]
    for y in range(rows):
        for x in range(cols):
            if maskedPoints[y][x]==0:

                data[y][x][0]=M[y][x]
                data[y][x][1]=classify(y,x,trainData,n) #classifying points
    T=implementedKnn(data,maskedPoints)
    return T







trainData=[]

nextIm=False
def PickCorrespondig(event, x, y, flags, param):  #choosing training data points 
    global nextIm,trainData

    if event == cv2.EVENT_LBUTTONDOWN and nextIm==True: #healthy points
        trainData.append([y,x,0])
        if len(trainData)!=20:
            print("Select ",len(trainData)-9,". HEALTHY segment point")



    
    if event == cv2.EVENT_LBUTTONDOWN and nextIm==False: #tumor points
        trainData.append([y,x,1])
        if len(trainData)==10:
            print("Select ",len(trainData)-9,". HEALTHY segment point")
        else:
            print("Select ",len(trainData)+1,". TUMOR segment point")

    if len(trainData) ==10:
        nextIm=True
    if event==cv2.EVENT_LBUTTONDOWN and len(trainData)==20:
        cv2.destroyAllWindows()
        
        
        
image = cv2.imread("mr.jpg")
(h,w)=image.shape[:2]  #height and width values of the image
print("Please provide training data set for KNN")
print("Select  1 . TUMOR segment point")

clone = image.copy()
cv2.namedWindow("image")
cv2.setMouseCallback("image", PickCorrespondig)

while len(trainData)<20:
    
    cv2.imshow("image", image)
    key = cv2.waitKey(0) & 0xFF
    if len(trainData)>=20:
        cv2.destroyAllWindows()
        break



I=cv2.imread("mr.jpg",0)

cv2.imshow("1 . Image",I)

M=thresholdM(I,50) #threshold image
cv2.imshow("2 . Thresholded Image",M)


kernel = np.ones((5,5),np.uint8)  #erosion and dilation operations to remove skull
erosion = cv2.erode(M,kernel,iterations = 4)

dilation = cv2.dilate(erosion,kernel,iterations = 4)


cv2.imshow("3 . Skull Image",dilation)
rows,cols=erosion.shape[:2]


maskedPic=I.copy()  #mask the image
maskedPoints=np.zeros([rows,cols])
for y in range(rows):
    for x in range(cols):
        if dilation[y][x]==0:
            maskedPoints[y][x]=1
            maskedPic[y][x]=0


cv2.imshow("4 . Masked Image",maskedPic)



n=3 #nearest neighbour value
print("KNN with ",n,"nearest neighbour")
knn,tumor=KNN(trainData,maskedPic,n,maskedPoints)  #apply knn


cv2.imshow("5 . KNN Aplied Image",knn)
cv2.imshow("6 . Tumor",tumor)

kernel2 = np.ones((3,3),np.uint8)

gradient = cv2.morphologyEx(tumor, cv2.MORPH_GRADIENT, kernel2)#boundaries of tumor 
cv2.imshow("7 . Tumor Boundary",gradient)


cI=np.zeros([rows,cols,3],dtype=np.uint8) #blue boundary added to image
cI[:,:,0]=I
cI[:,:,1]=I
cI[:,:,2]=I

for y in range(rows): 
    for x in range(cols): 
        if gradient[y][x]==255:
            cI[y][x][:]=[255,0,0]



cv2.imshow("8 . Output Image", cI) 

cv2.waitKey(0)
cv2.destroyAllWindows()


































