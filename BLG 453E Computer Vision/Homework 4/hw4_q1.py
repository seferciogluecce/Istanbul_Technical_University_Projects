
import cv2
import numpy as np



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
    Ix=np.zeros(image.shape[:2])
    Iy=np.zeros(image.shape[:2])
    rows,cols =image.shape[:2]

    for y in range(rows):
        for x in range(cols):
            Jx,Jy=computeJxJy(image,y,x)  #gradient vectors
            Ix[y][x]=Jx
            Iy[y][x]=Jy
    return Ix,Iy

def calculateTensor(I,Ix,Iy): #G matrix calculation

    rows,cols =I.shape[:2]
    G=np.zeros([rows,cols,2,2], dtype=np.int)
    Ixx=Ix*Ix
    Iyy=Iy*Iy
    Ixy=Ix*Iy
    for y in range(rows):
        for x in range(cols):
            for j in range(-1,2):
                for i in range(-1,2):
                    if not(i+y<0 or i+y>=rows or j+x>=cols or j+x<0):
                        
                        G[y][x][0][0]+=Ixx[y+i][x+j]
                        G[y][x][1][1]+=Iyy[y+i][x+j]
                        G[y][x][0][1]+=Ixy[y+i][x+j]
                        G[y][x][1][0]+=Ixy[y+i][x+j]


    return G 

def calculateR(G,k): #R value calculation 
    rows,cols =G.shape[:2]
    R=np.zeros(G.shape[:2])
    print(R.shape[:])
    for y in range(rows):
        for x in range(cols):
            det=float(G[y][x][0][0]*G[y][x][1][1]-((G[y][x][1][0])**2))
            trace=float(G[y][x][0][0]+G[y][x][1][1])
            R[y][x]=float(det-k*(trace**2))
    return R

def thresholdR(R,T): #R bigger than threshold -> corner
    RT=np.zeros(R.shape[:2])
    rows,cols =R.shape[:2]

    for y in range(rows):
        for x in range(cols):
            if R[y][x] > T:
                RT[y][x]=255
    return RT

def colorCorners(I,RT): #Coloring neighbours of weighted corner as well
    rows,cols =RT.shape[:2]
    
    cI=np.zeros([rows,cols,3],dtype=np.uint8)
    cI[:,:,0]=I
    cI[:,:,1]=I
    cI[:,:,2]=I

    for y in range(rows):
        for x in range(cols): 
            if RT[y][x]==255:
                for j in range(-1,2):
                    for i in range(-1,2):
                        if not(i+y<0 or i+y>=rows or j+x>=cols or j+x<0):
                            cI[y+i][x+j][:]=[0,255,0]
    return cI               

I=cv2.imread("blocks.jpg",0)


sig=0.5
filteredImage=gaussianFilter(I,sig)  #gaussian filter the image
print("Image Filtered")

Ix,Iy=computeGradient(filteredImage)
print("Gradient Computed")



G=calculateTensor(filteredImage,Ix,Iy)
print("G Computed")

k=0.04
R=calculateR(G,k)
print("R Computed")

T=50000
RT=thresholdR(R,T)
print("R Thresholded")


cI=colorCorners(I,RT)
print("Image processed")





cv2.imshow("Original",I)
cv2.imshow("Filtered",filteredImage)
cv2.imshow("Processed",cI)
cv2.imshow("Thresholded",RT)

cv2.waitKey(0)
cv2.destroyAllWindows()


























