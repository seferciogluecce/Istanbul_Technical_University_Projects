import cv2
import numpy as np

 
def dot(matrix,x,y,image) : #matrix multiplication for each pixel
    rows,cols=image.shape[:2]
    xcc=int(cols/2)
    ycc=int(rows/2)
    xr=x-xcc
    yr=y-ycc
    
    u=matrix[0][0]*xr+matrix[0][1]*yr
    v=matrix[1][0]*xr+matrix[1][1]*yr
            
    u=u+xcc
    v=v+ycc
    return [u,v]


def rotate(imageO,deg):  #backward transform rotation
    bufferY=33 #to contain original image after rotation we add padding to y axis
    image=np.zeros([imageO.shape[0]+bufferY*2,imageO.shape[1]],dtype=np.uint8)
    image[bufferY:-bufferY,:]=imageO
    (h,w)=image.shape[:2]

    InverseTransMatrix=[[np.cos(-deg),-np.sin(-deg),0],[np.sin(-deg),np.cos(-deg),0],[0,0,1]]
    imageCopy=np.zeros([image.shape[0],image.shape[1]],dtype=np.uint8)
    for y in range(len(imageCopy)):  
        for x in range(len(imageCopy[y])):
            a,b=dot(InverseTransMatrix,x,y,image)
            if int(a)<w and int(b)<h and int(a)>=0 and int(b)>=0:
                imageCopy[y][x]=image[int(b)][int(a)]
                
    
    return imageCopy 

for a in range(20): #for each image
    T=[]
    temp=cv2.imread("Guitar_images/guitar-"+str(a+1)+".png",0) #a 
    rows,cols=temp.shape
    tX=[]
    tY=[]
    for y in range(rows):      #matrix T construction
        for x in range(cols):  
            if temp[y][x]!=0:
                tX.append(x)
                tY.append(y)
    mean0=np.mean(tX)
    mean1=np.mean(tY)
    for b in range(len(tX)):            
        tX[b]=tX[b]-mean0
        tY[b]=tY[b]-mean1
    T.append(tX)
    T.append(tY)    
    
    '''for b in range(len(tX)):            
        print(tX[b],tY[b],T[0][b],T[1][b])'''
    
    S=np.matmul(T,np.transpose(T))       #covariance matrix
    U,s,V=np.linalg.svd(S)  

    idx=0 if s[0]>s[1] else 1  #index of max eigenvalue 

    theta = np.tanh((V[idx][0])/(V[idx][1]))  
    dst=rotate(temp,theta) #image rotated
   
    cv2.imshow("guitar-alligned-"+str(a+1),dst) #image showed and saved
    cv2.imwrite("guitar-alligned-"+str(a+1)+".png",dst)
    
    
cv2.waitKey(0)
cv2.destroyAllWindows()


