
import cv2
import numpy as np



#PART (a)

def LoG(x,y,sigmaS): #laplacian of gaussian calculator

    log=(1/(2*np.pi*sigmaS))*np.exp(-(x*x+y*y)/(2*sigmaS))
    return log

def weightMatrix(sigmaS):
    matrice=[[0,0,0],[0,0,0],[0,0,0]]    #3*3matrice
    for y in range(-1,2):
        for x in range(-1,2):
            y=-y
            matrice[x+1][y+1]=LoG(x,y,sigmaS) #matrice elements are calculated by laplacian of gaussian value 
    sumAll=sum(map(sum,matrice))
    for y in range(3):
        for x in range(3):
            matrice[x][y]=matrice[x][y]/sumAll
    return matrice

def convol(image,matrice,i,j):  #convolution function
    k=0
    rows,cols =image.shape[:2]

    for y in range(-len(matrice)+2,-1+len(matrice)):
        for x in range(-len(matrice)+2,-1+len(matrice)):
             if i+y<0 or i+y>=rows or j+x>=cols or j+x<0:
                k+=0
             else:
                k+=image[i+y][j+x]*matrice[x+1][y+1]
    return k

def gaussianFilter(image,sigmaS):    #gaussian filter application 
    
    matrice=[[0,0,0],[0,0,0],[0,0,0]]    #3*3matrice
    matrice=weightMatrix(sigmaS)  #calculate weight matrice
    maOtherImage=image.copy()
    rows,cols =image.shape[:2]

    for y in range(rows):
        for x in range(cols):
            maOtherImage[y][x]=convol(image,matrice,y,x) #filter applied through concvolution 
    return maOtherImage

    
def computeJxJy(image,y,x): #gradient vector computation wrt discretized derivatives
    rows,cols =image.shape[:2]
    if y-1<0:
        Jy=int((int(image[y+1][x])-int(image[y][x]))/2)
    elif y+1>=rows:
        Jy=int((int(image[y][x])-int(image[y-1][x]))/2)
    else: 
        Jy=int((int(image[y+1][x])-int(image[y-1][x]))/2)
           
    if x-1<0:
        Jx=int((int(image[y][x+1])-int(image[y][x]))/2)
    elif x+1>=cols:
        Jx=int((int(image[y][x])-int(image[y][x-1]))/2)
    else :
        Jx=int((int(image[y][x+1])-int(image[y][x-1]))/2)

    return Jx,Jy


def computeGradient(image): #value of image gradient is calculated
    M=image.copy()
    A=np.zeros(image.shape[:2])
    rows,cols =image.shape[:2]

    for y in range(rows):
        for x in range(cols):
            Jx,Jy=computeJxJy(image,y,x)  #gradient vectors
            M[y][x]=(Jx*Jx+Jy*Jy)**0.5            #gradient magitude
            A[y][x]=np.arctan2(Jy,Jx)* 180 / np.pi       #gradient angle      
             
    return M,A


def gradientDirection(A): #calculates normal of edge direction  for the current pixel
    P=[]
    P.append([-1,-1])
    P.append([0,1])
    P.append([1,1])
    
    P.append([-1,0])
    P.append([0,0])
    P.append([1,0])
    
    P.append([-1,-1])
    P.append([0,-1])
    P.append([1,-1])

    returnVal=[]

    if (A<=180 and A>=157.5) or (A<22.5 and A>=0) or ( A<0 and A>=-22.5) or (A<-157.5 and A>=-180):
        returnVal.append([P[3],P[5]])

    elif (A<157.5 and A>=112.5) or (A<-22.5 and A>=-67.5):
        returnVal.append([P[0],P[8]])
    
    elif (A<112.5 and A>=67.5) or (A<-67.5 and A>=-112.5):
        returnVal.append([P[7],P[1]])     

    
    elif (A<67.5 and A>=22.5) or ( A<-112.5 and A>=-157.5):
        returnVal.append([P[6],P[2]]) 


    return returnVal[0][0][0],returnVal[0][0][1],returnVal[0][1][0],returnVal[0][1][1]       
  


def thresholdM(image,M,A,threshold): # thresholding the image by applying nonmaxima
    MT=M.copy()
    rows,cols =M.shape[:2]

    for y in range(rows):
        for x in range(cols):   
            if M[y][x]<threshold:
                MT[y][x]=0
            else:
                x1,y1,x2,y2=gradientDirection(A[y][x])
                counter=0
                
                if y+y1<rows and x+x1<cols and x+x1>=0 and y+y1>=0 :  
                    if M[y][x]<M[y1+y][x1+x]:
                        counter+=1

                if y+y2<rows and x+x2<cols and y+y2>=0 and x+x2>=0 :
                    if M[y][x]<M[y2+y][x2+x]:
                        
                        counter+=1
                        
                
                if(counter==2): #if counter < 2 means at least one of neighbours has greater value than current pixel in edge direction
                    MT[y][x]=0
                else:
                    MT[y][x]=255
                      
    return MT
          


def canny(I,sig,tau):  #main canny edge detection function 
    filteredImage=I.copy()
    filteredImage=gaussianFilter(I,sig)  #gaussian filter the image
    M,A=computeGradient(filteredImage) #compute image gradients
    #cv2.imshow("Non Thresholded Picture sigma = "+str(sig)+" tau= "+str(tau) ,M)
    E=thresholdM(I,M,A,tau) #apply nonmaxima
    return E,M,A #(E,M,A)
        



#PART (b) #edge detection implemented on Fig2wirebond_mask with required variable values 


ImageWireBond=cv2.imread("Fig2wirebond_mask.jpg",0)


E,M,A=canny(ImageWireBond, 0.5,5)

cv2.imshow("Fig2wirebond_mask sig = 0.5 tau = 5",E)
cv2.waitKey(0)
cv2.destroyAllWindows()


E,M,A=canny(ImageWireBond, 1,5)

cv2.imshow("Fig2wirebond_mask sig = 1 tau = 5",E)
cv2.waitKey(0)
cv2.destroyAllWindows()



E,M,A=canny(ImageWireBond, 3,5)

cv2.imshow("Fig2wirebond_mask sig = 3 tau = 5",E)
cv2.waitKey(0)
cv2.destroyAllWindows()


# PART (c)

ImageBottles=cv2.imread("Fig3bottles.jpg",0) #edge detection implemented on Fig3bottles 

E,M,A=canny(ImageBottles, 1, 15)



cv2.imshow("Fig3bottles sig = 1 tau = 15",E)

cv2.waitKey(0)
cv2.destroyAllWindows()







# PART (d)


def thresholdNormal(M,threshold): #thresholding images normally for a threshold value 
    MT=M.copy()
    rows,cols =M.shape[:2]

    for y in range(rows):
        for x in range(cols):   
            if M[y][x]<threshold:
                MT[y][x]=0
            else:                
                MT[y][x]=255
                      
    return MT
   




cvCanny = cv2.Canny(ImageWireBond,100,255)  #canny edge detection calculated with built in function

gradientMagnitude, A =  computeGradient(ImageWireBond) # gradient image calculation 


gradientMagnitude=thresholdNormal(gradientMagnitude,100) # gradient image is thresholded

cv2.imshow("Gradient Magnitude Image",gradientMagnitude)
cv2.imshow("Built-in Canny Edge Detector",cvCanny)

cv2.waitKey(0)
cv2.destroyAllWindows()














