def translate(matrix,xv,yv,cols,rows): #move and enhance imageto right bottom corner of the image
    rowsm,colsm,xm = image.shape

    matrixC=np.zeros([rows+rowsm,cols+colsm,3],dtype=np.uint8)
    for y in range(len(matrix)):
        for x in range(len(matrix[y])):
            u=x+xv
            v=y+yv
            matrixC[v][u]=matrix[y][x]
    return matrixC


def translate2(matrix,xv,yv,cols,rows):
    rowsm,colsm,xm = image.shape
    matrixC=np.zeros([rows/2+rowsm,cols/2+colsm,3],dtype=np.uint8)
    for y in range(len(matrix)):
        for x in range(len(matrix[y])):
            u=x+xv
            v=y+yv
            matrixC[v][u]=matrix[y][x]
    return matrixC


import cv2
import numpy as np

def printmt(matrix):
    print(matrix[0][0],matrix[0][1],matrix[1][0],matrix[1][1])
    
def dot(matrix,x,y,image) : 
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

image = cv2.imread('anka.jpg')


yc,xc=image.shape[:2]
xc=int(xc/2)
yc=int(yc/2)


deg=120 #-30

TransMatrix=[[np.cos(deg),-np.sin(deg),0],[np.sin(deg),np.cos(deg),0],[0,0,1]]
InverseTransMatrix=[[np.cos(-deg),-np.sin(-deg),0],[np.sin(-deg),np.cos(-deg),0],[0,0,1]]

(h,w)=image.shape[:2]  #height and width values of the image


printmt(TransMatrix)
imageCopy=image.copy()
imageCopy[:,:,0]=0
imageCopy[:,:,1]=0
imageCopy[:,:,2]=0
        
    
for y in range(len(image)):   #backward transform rotation, center rotation
    for x in range(len(image[y])):
        a=dot(InverseTransMatrix,x,y,image)[0]
        b=dot(InverseTransMatrix,x,y,image)[1]
        #print(a,b)
        if int(a)<w and int(b)<h and int(a)>=0 and int(b)>=0:

            imageCopy[y][x][0]=image[int(b)][int(a)][0]
            imageCopy[y][x][1]=image[int(b)][int(a)][1]
            imageCopy[y][x][2]=image[int(b)][int(a)][2]


cv2.imshow("Center Rotation",imageCopy)




cv2.waitKey(0)
cv2.destroyAllWindows()


rows,cols,x = image.shape
transIm=translate(image,cols,rows,cols*2,rows*2)



(h,w)=transIm.shape[:2]
imageCopy=transIm.copy()

for y in range(len(transIm)):   #backward transform rotation, left corner rotation
    for x in range(len(transIm[y])):
        a=dot(InverseTransMatrix,x,y,transIm)[0]
        b=dot(InverseTransMatrix,x,y,transIm)[1]
        if int(a)<w and int(b)<h and int(a)>=0 and int(b)>=0:

            imageCopy[y][x][0]=transIm[int(b)][int(a)][0]
            imageCopy[y][x][1]=transIm[int(b)][int(a)][1]
            imageCopy[y][x][2]=transIm[int(b)][int(a)][2]

cv2.imshow("Roataion With Respect To Left Up Point",imageCopy)




cv2.waitKey(0)
cv2.destroyAllWindows()








