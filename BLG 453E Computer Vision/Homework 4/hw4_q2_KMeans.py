

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

def implementedKMeans(data,maskedPoints): #Filling the image by classes 175 for healthy segment, 2555 for tumor
    rows,cols=maskedPoints.shape[:2]
    endData=np.zeros([rows,cols],dtype=np.uint8) #all brain
    endData2=np.zeros([rows,cols],dtype=np.uint8) #only tumor data

    for y in range(rows):
        for x in range(cols): 
            if maskedPoints[y][x]==0:
                if data[y][x][1]==1:
                    endData[y][x]=255
                    endData2[y][x]=255 

                else:
                    endData[y][x]=175

    return endData,endData2


def kMeans(M,maskedPoints):
    rows,cols =M.shape[:2]

    j=np.random.randint(0,cols)
    i=np.random.randint(0,rows)
    data=np.zeros([rows,cols,2])

    while maskedPoints[j][i]==1:
        j=np.random.randint(0,cols)
        i=np.random.randint(0,rows)

    jj=np.random.randint(0,cols)
    ii=np.random.randint(0,rows)

    while maskedPoints[jj][ii]==1 or (jj==j and ii==i):
        jj=np.random.randint(0,cols)
        ii=np.random.randint(0,rows)
        

    '''ci0=M[235][166]      #predefined points for mean values
    ci1=M[235][280]'''

    ci0=M[j][i]     #random points for mean values
    ci1=M[jj][ii]
    
    conv0=0  #convergence controllers
    conv1=0
    conv01=-1
    conv11=-1
    for y in range(rows):
        for x in range(cols):
            if maskedPoints[y][x]==0:    
                data[y][x][0]=M[y][x]     

    while(conv0!=conv11 and conv1!=conv01): #controlling convergence 
        for y in range(rows):
            for x in range(cols):
                if maskedPoints[y][x]==0:    
                    data[y][x][1]=classifyMean(data[y][x][0],ci1,ci0) #classifying points
                    
        class0=[0,0]
        class1=[0,0]
        conv0=ci0
        conv1=ci1
        for y in range(rows):
            for x in range(cols):
                if maskedPoints[y][x]==0:
                    if data[y][x][1]==1:
                        class0[0]+=1
                        class0[1]+=M[y][x]
                    else:
                        class1[0]+=1
                        class1[1]+=M[y][x]     
                        
        ci0= class0[1]/class0[0]  #calculating new means
        ci1= class1[1]/class1[0]
        conv01=ci0
        conv11=ci1
    
    return   implementedKMeans(data,maskedPoints)


def classifyMean(val0,val1,val2): #calculate distance value for each mean 
    dist1=kMeansDist(val0,val1)    #1
    dist2=kMeansDist(val0,val2)  #0
    if dist1>dist2:
        return 0
    else:
        return 1

def kMeansDist(val1,val0):
    return np.abs(val1-val0)


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



kMeans,tumor=kMeans(maskedPic,maskedPoints)  #apply kmeans
noNoisekMeans = cv2.morphologyEx(kMeans, cv2.MORPH_OPEN, kernel)  #reduce noise in output and tumor
noNoiseTumor = cv2.morphologyEx(tumor, cv2.MORPH_OPEN, kernel)


cv2.imshow("5 . KMeans Aplied Image",noNoisekMeans)
cv2.imshow("6 . Tumor",noNoiseTumor)

kernel2 = np.ones((3,3),np.uint8)

gradient = cv2.morphologyEx(noNoiseTumor, cv2.MORPH_GRADIENT, kernel2)#boundaries of tumor 
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


































