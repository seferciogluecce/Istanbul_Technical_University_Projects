# -*- coding: utf-8 -*-
"""
Created on Fri Oct 20 21:57:03 2017

@author: EccE
"""
import cv2
import numpy as np
from math import sqrt 
maxtitude=[0,0]

def filterValue(i,j,n,pic):
    filterR=[]
    rows,cols = pic.shape[:2]
    for x in range(-n+2,n-1):
        for y in range(-n+2,n-1):
            if i+x<0 or i+x>=rows or j+y>=cols or j+y<0:
                filterR.append(0)
            else:
                filterR.append(pic[i+x][j+y])
    return filterR

def gaussianFilter(picName):    #4 times mean filter
    image = cv2.imread(picName,0)    
    for y in range(len(image)):
        for x in range(len(image[y])):
            image[y][x]=np.mean(filterValue(y,x,3,image))
    for y in range(len(image)):
        for x in range(len(image[y])):
            image[y][x]=np.mean(filterValue(y,x,3,image))
    for y in range(len(image)):
        for x in range(len(image[y])):
            image[y][x]=np.mean(filterValue(y,x,3,image))
    for y in range(len(image)):
        for x in range(len(image[y])):
            image[y][x]=np.mean(filterValue(y,x,3,image))
    
      
    cv2.imshow("Gaussian "+picName,image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    return image
    

def convol(image,matrice,i,j):  #convolution function
    k=0
    rows,cols = image.shape[:2]

    for y in range(-len(matrice)+2,-1+len(matrice)):
        for x in range(-len(matrice)+2,-1+len(matrice)):
             if i+y<0 or i+y>=cols or j+x>=rows or j+x<0:
                k+=0
             else:
                k+=image[i+y][j+x]*matrice[y+1][x+1]
    return k

#part a
N=[[-3,-3,-3],[-3,0,-3],[5,5,5]]    #compass operators

W=[[-3,-3,5],[-3,0,5],[-3,-3,5]]

S=[[5,5,5],[-3,0,-3],[-3,-3,-3]]  # hw assignment example

E=[[5,-3,-3],[5,0,-3],[5,-3,-3]]

NW=[[-3,-3,-3],[-3,0,5],[-3,5,5]]

SW=[[-3,5,5],[-3,0,5],[-3,-3,-3]]

SE=[[5,5,-3],[5,0,-3],[-3,-3,-3]]

NE=[[-3,-3,-3],[5,0,-3],[5,5,-3]]

I = cv2.imread("StairsBuildingsN.png",0)    

copyI=I.copy()

edges=[N,E,S,W,NE,SE,NW,NW]  #operator array

edgeValues=[(0,1),(1,0),(0,-1),(-1,0),(sqrt(2)/2,sqrt(2)/2),(sqrt(2)/2,-sqrt(2)/2),(-sqrt(2)/2,sqrt(2)/2),(-sqrt(2)/2,-sqrt(2)/2)] #for U and V
Jdir=np.zeros(I.shape[:2])
U=np.zeros(I.shape[:2])
V=np.zeros(I.shape[:2])


Jmag=np.zeros(I.shape[:2])
for a in range(8):    
    for y in range(len(I)):
        for x in range(len(I[y])):
            convo=convol(I,edges[a],y,x)
            copyI[y][x]=max(min(convo,255),0)
            if Jmag[y][x]<convo :                # compute jmag value consisting of highest magnitudes
                Jmag[y][x]=convo
                if Jmag[y][x]>254:
                    Jdir[y][x]=a+1                
                    U[y][x]=edgeValues[a][0]
                    V[y][x]=edgeValues[a][1]
                    

    
    cv2.imshow("Edge "+str(a),copyI)  #show each operator result


cv2.waitKey(0)
cv2.destroyAllWindows()


#part b
               
U2=[]
V2=[]
for y in range(len(I)):          
    for x in range(len(I[y])):
        if  Jmag[y][x]<255:     # clear Jdir, U, V values under threshold
            Jdir[y][x]=0                
            U[y][x]=0
            V[y][x]=0
            Jmag[y][x]=0
        else:
            Jmag[y][x]=255
cv2.imshow("Jmag",Jmag)
cv2.waitKey(0)
cv2.destroyAllWindows()


#part b end


#part c
import matplotlib.pyplot as plt

X2=[]
Y2=[]
X=[]
Y=[]

for y in range(len(I)):
    for x in range(len(I[y])):
        X2.append(x)
        Y2.append(y)
        if  x%5==0 and y%5==0 and Jmag[y][x]==255:  #prepare vector fields
            U2.append(U[y][x])
            V2.append(V[y][x])
            U[y][x]=0
            V[y][x]=0
            X.append(x)
            Y.append(y)
    
plt.figure()
plt.imshow(Jmag)
plt.quiver(X,Y,U2,V2,color="red")
plt.savefig("vector_field.png",dpi=1200)

plt.show()

#part c end


#d

Fx=[[-1/3,0,1/3],[-1/3,0,1/3],[-1/3,0,1/3]]  

Fy=[[1/3,1/3,1/3],[0,0,0,],[-1/3,-1/3,-1/3]]



copyIx=I.copy()

for y in range(len(I)):
        for x in range(len(I[y])):
            convo=convol(I,Fx,y,x)
            copyIx[y][x]=max(min(convo,255),0)
            if(copyIx[y][x]>30):
                copyIx[y][x]=255
            else:
                copyIx[y][x]=0

  
cv2.imshow("Ix ",copyIx)  #x gradient
    
copyIy=I.copy()

for y in range(len(I)):
        for x in range(len(I[y])):
            convo=convol(I,Fy,y,x)
            copyIy[y][x]=max(min(convo,255),0)
            if(copyIy[y][x]>30):
                copyIy[y][x]=255
            else:
                copyIy[y][x]=0
            

cv2.imshow("Iy ",copyIy)  #y gradient


Imag=I.copy()

for y in range(len(I)):
        for x in range(len(I[y])):
            Imag[y][x]=sqrt(copyIy[y][x]**2+copyIx[y][x]**2) # gradient magnitude
cv2.imshow("Imag ",Imag)




plt.figure()
plt.imshow(Imag)
plt.quiver(copyIy,copyIx,color="red")
plt.savefig("vector_field2.png",dpi=1200)

plt.show()

#part d end



'''import math
Gaus=I.copy()
sigma=5
for y in range(len(I)):
        for x in range(len(I[y])):      #gaussian try
            Gaus[y][x]=np.convolve( I[y][x] , math.exp(-(x**2+y**2)/2*sigma**2) * ((x**2+y**2-2*sigma**2)/sigma**4))
        

cv2.imshow("Gaus ",Gaus)

cv2.waitKey(0)
cv2.destroyAllWindows()'''






#f

gausFilteredIm=gaussianFilter("StairsBuildingsN.png")



cv2.waitKey(0)
cv2.destroyAllWindows()


