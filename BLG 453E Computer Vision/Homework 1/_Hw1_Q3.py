
import cv2
import numpy as np

points1=[]  #points from original picture
points2=[]  #points from affined picture
nextIm=False
def PickCorrespondig(event, x, y, flags, param):  #choosing 5 corresponding points 
	global nextIm,points1,points2

	if event == cv2.EVENT_LBUTTONDOWN and nextIm==True:  #affiined picture
		points2.append([x, y])
		print("points 2 ",len(points2),points2)
    
	if event == cv2.EVENT_LBUTTONDOWN and nextIm==False: #original picture
		points1.append([x, y])
		print("points 1 ",len(points1),points1)
		if len(points1) ==5:
			nextIm=True
    

		cv2.imshow("image", image)
        
        
image = cv2.imread("cameraman.jpg")
(h,w)=image.shape[:2]  #height and width values of the image

clone = image.copy()
cv2.namedWindow("image")
cv2.setMouseCallback("image", PickCorrespondig)
 
while True:
    
	cv2.imshow("image", image)
	key = cv2.waitKey(1) & 0xFF
	if len(points1)==5:
		image=cv2.imread("cameramanAffine.jpg")       
                  
	if len(points1)+len(points2)==10:
		break


    
#Affine
#A.x=b
b=[]
for a in range(len(points1)):
        b.append(points1[a][0])
        b.append(points1[a][1])
        
A=[] 
for a in range(len(points2)):
        x=points2[a][0]
        y=points2[a][1]
        A.append([points2[a][0],points2[a][1],0,0])
        A.append([0,0,points2[a][0],points2[a][1]])
An=np.asarray(A)

x=np.dot(np.dot(   np.linalg.inv(     np.dot(An.transpose(), An)  ) ,An.transpose()),b)                 

print(x)

TransMat=[[x[0],x[1],0],[x[2],x[3],0],[0,0,1]] #transfrom matrix

image2=cv2.imread("cameraman.jpg")

myOtherResult=image2.copy()
myOtherResult[:,:,0]=0
myOtherResult[:,:,1]=0
myOtherResult[:,:,2]=0

             
for y in range(len(image2)): #affine calculation with backward
    for x in range(len(image2[y])):
        xx,yy,cc=np.dot(TransMat,[x,y,1])
        
        if int(xx)>=0 and int(xx)<w and int(yy) >=0 and int(yy)<h:
            myOtherResult[y][x][0]=image2[int(yy)][int(xx)][0]
            myOtherResult[y][x][1]=image2[int(yy)][int(xx)][1]
            myOtherResult[y][x][2]=image2[int(yy)][int(xx)][2]




cv2.imshow("Affine Implemented",myOtherResult)
cv2.waitKey(0)
cv2.destroyAllWindows()

#Reverse Affine

bR=[]
for a in range(len(points2)):
        bR.append(points2[a][0])
        bR.append(points2[a][1])
        
AR=[] 
for a in range(len(points1)):
        x=points1[a][0]
        y=points1[a][1]
        AR.append([points1[a][0],points1[a][1],0,0])
        AR.append([0,0,points1[a][0],points1[a][1]])
AnR=np.asarray(AR)

xR=np.dot(np.dot(   np.linalg.inv(     np.dot(AnR.transpose(), AnR)  ) ,AnR.transpose()),bR)                 

print(xR)

TransMatR=[[xR[0],xR[1],0],[xR[2],xR[3],0],[0,0,1]]

image2=cv2.imread("cameramanAffine.jpg")

myOtherResult=image2.copy()
myOtherResult[:,:,0]=0
myOtherResult[:,:,1]=0
myOtherResult[:,:,2]=0

             
for y in range(len(image2)):  #reverse affine calculation with backward
    for x in range(len(image2[y])):
        xx,yy,cc=np.dot(TransMatR,[x,y,1])
        
        if int(xx)>=0 and int(xx)<w and int(yy) >=0 and int(yy)<h:
            myOtherResult[y][x][0]=image2[int(yy)][int(xx)][0]
            myOtherResult[y][x][1]=image2[int(yy)][int(xx)][1]
            myOtherResult[y][x][2]=image2[int(yy)][int(xx)][2]




cv2.imshow("Affined Reversed",myOtherResult)
cv2.waitKey(0)
cv2.destroyAllWindows()





