import cv2
import numpy as np



def transformMatrix(bPoints,Apoints): #solving A.x=b for x for matching points
    b=[]
    for a in range(len(bPoints)):
        b.append(bPoints[a][0])
        b.append(bPoints[a][1])
    A=[]
    for a in range(len(Apoints)):
        A.append([Apoints[a][0],Apoints[a][1],0,0])
        A.append([0,0,Apoints[a][0],Apoints[a][1]])
    An=np.asarray(A)
    x=np.dot(np.dot(   np.linalg.inv(     np.dot(An.transpose(), An)  ) ,An.transpose()),b)                 
    TransMat=[[x[0],x[1],0],[x[2],x[3],0],[0,0,1]] #transfrom matrix
    print("a11 = ",str(x[0]),"a12 =",str(x[1]),"a21 =",str(x[2]),"a22 =",str(x[3]))
    return TransMat
        
def applyMatrix(image,image2,transMat,matchType):        

    (h,w)=image.shape[:2]  #height and width values of the image
    image2=np.zeros(image2.shape[:2],dtype=np.uint8)
    for y in range(len(image)): #affine calculation with backward
        for x in range(len(image2[y])):
            xx,yy,cc=np.dot(transMat,[x,y,1])        
            if int(xx)>=0 and int(xx)<w and int(yy) >=0 and int(yy)<h:
                image2[y][x]=image[int(yy)][int(xx)]

    cv2.imshow("Source "+matchType,image)
    cv2.imshow("Affined "+matchType,image2)
    

   



originalImg = cv2.imread('cameraman1.jpg',0) # original image   
affinedImg = cv2.imread('cameraman2.jpg',0)  # affined image

sift = cv2.xfeatures2d.SIFT_create()  # sift detector

originalKeyPoints, descriptorOriginal = sift.detectAndCompute(originalImg,None) # key points and descriptors 
affinedKeyPoints, descriptorAffined = sift.detectAndCompute(affinedImg,None)


bf = cv2.BFMatcher()  #brute force matched
matches = bf.knnMatch(descriptorOriginal,descriptorAffined,k=2) # points are matched with knnmatch with two best matches




matchedPoints_original=[originalKeyPoints[a[0].queryIdx].pt for a in matches] #matches points are appended to list
matchedPoints_affined=[affinedKeyPoints[a[0].trainIdx].pt for a in matches]



out=originalImg.copy() 
matched= cv2.drawMatchesKnn(originalImg,originalKeyPoints,affinedImg,affinedKeyPoints,matches,out,flags=2) #drawing matched points between images


cv2.imshow("Matches",matched) #matched points image 


  
affineMat=transformMatrix(matchedPoints_original,matchedPoints_affined) # affine matrix calculation for all matched points

applyMatrix(originalImg,affinedImg,affineMat, "All Matched Points") # found affine matrix is applied to original image



goodMatches = [] 
for m,n in matches: # from the 2 best matches the best one is appended to list 
    if m.distance < 0.63*n.distance:
        goodMatches.append([m])

bestMatches=2 #first two elements of good matches is really the best matches with very small error

bestMatched = cv2.drawMatchesKnn(originalImg,originalKeyPoints,affinedImg,affinedKeyPoints,goodMatches[:bestMatches],out,flags=2)

cv2.imshow("Best Matches", bestMatched) #best matched points image 

matchedPoints_original_good=[originalKeyPoints[a[0].queryIdx].pt for a in goodMatches[:bestMatches]]
matchedPoints_affined_good=[affinedKeyPoints[a[0].trainIdx].pt for a in goodMatches[:bestMatches]]




affineMat=transformMatrix(matchedPoints_original_good,matchedPoints_affined_good) # affine matrix calculation for best matched points

applyMatrix(originalImg,affinedImg,affineMat,"Best Matched Points") # found affine matrix is applied to original image




cv2.waitKey(0)
cv2.destroyAllWindows()
    
  











