from matplotlib import pyplot as plt
import cv2
import math


image = cv2.imread('underexposed.jpg')

cv2.imshow("Underexposed Image",image)
cv2.waitKey(0)
cv2.destroyAllWindows()

# part a   Mean and standart derivation calculations
(h,w)=image.shape[:2]  #height and width values of the image


rMean=0
gMean=0
bMean=0
rStandartDerivation=0
gStandartDerivation=0
bStandartDerivation=0


bSum= sum(map(sum, (image[:,:,0])))
gSum=sum(map(sum, (image[:,:,1])))
rSum=sum(map(sum, (image[:,:,2])))

rMean=rSum/(h*w)
gMean=gSum/(h*w)
bMean=bSum/(h*w)

rStaSum=0
gStaSum=0
bStaSum=0

for x in range(len(image)):
    for y in range(len(image[x])):
        rStaSum+=math.pow(rMean-image[x][y][2],2)
        gStaSum+=math.pow(rMean-image[x][y][1],2)
        bStaSum+=math.pow(rMean-image[x][y][0],2)

rStandartDerivation=math.sqrt(rStaSum/(h*w))
gStandartDerivation=math.sqrt(gStaSum/(h*w))
bStandartDerivation=math.sqrt(bStaSum/(h*w))

print("Underexposed, R channel mean:",rMean)
print("Underexposed, G channel mean:",gMean)
print("Underexposed, B channel mean:",bMean)

print("Underexposed, R channel standart derivation:",rStandartDerivation)
print("Underexposed, G channel standart derivation:",gStandartDerivation)
print("Underexposed, B channel standart derivation:",bStandartDerivation)


# part b    histograms

print("Histograms Of The Underexposed Image")


plt.hist(image[:,:,2].ravel(),256,[0,256],color = "red")
plt.title("R Channel")
plt.show()
plt.hist(image[:,:,1].ravel(),256,[0,256],color = "green")
plt.title("G Channel")
plt.show()
plt.hist(image[:,:,0].ravel(),256,[0,256],color = "blue")
plt.title("B Channel")
plt.show()


plt.hist([a for a in image[:,:,2].ravel()],256,[0,256],[0,1],color = "red")
plt.title("R Channel Normalized")
plt.show()
plt.hist([a for a in image[:,:,1].ravel()],256,[0,256],[0,1],color = "green")
plt.title("G Channel Normalized")
plt.show()
plt.hist([a for a in image[:,:,0].ravel()],256,[0,256],[0,1],color = "blue")
plt.title("B Channel  Normalized")
plt.show()

# part c  cdf histograms

print("CDFs Of The Underexposed Image")

plt.hist(image[:,:,2].ravel(),256,[0,256],cumulative=True,color = "red")
plt.title("R Channel CDF")
plt.show()
plt.hist(image[:,:,1].ravel(),256,[0,256],cumulative=True,color = "green")
plt.title("G Channel CDF")
plt.show()
plt.hist(image[:,:,0].ravel(),256,[0,256],cumulative=True,color = "blue")
plt.title("B Channel CDF")
plt.show()

rN,rBins,rPatches=plt.hist([a for a in image[:,:,2].ravel()],256,[0,256],[0,1],cumulative=True,color = "red")
plt.title("R Channel Normalized CDF")
plt.show()
gN,gBins,gPatches=plt.hist([a for a in image[:,:,1].ravel()],256,[0,256],[0,1],cumulative=True,color = "green")
plt.title("G Channel Normalized CDF")
plt.show()
bN,bBins,bPatches=plt.hist([a for a in image[:,:,0].ravel()],256,[0,256],[0,1],cumulative=True,color = "blue")
plt.title("B Channel  Normalized CDF")
plt.show()
 
#part d   histogram equalization

for x in range(len(image)):
    for y in range(len(image[x])):
        image[x][y][0]=255*bN[image[x][y][0]]
        image[x][y][1]=255*gN[image[x][y][1]]
        image[x][y][2]=255*rN[image[x][y][2]]



cv2.imshow("Brightness Equalized Image",image)
cv2.imwrite("BrigthnessEqualizedImage.jpg",image)
cv2.waitKey(0)
cv2.destroyAllWindows()



print("Histograms Of The Equalized Image")  #histograms of newly equalized image


plt.hist(image[:,:,2].ravel(),256,[0,256],color = "red")
plt.title("R Channel")
plt.show()
plt.hist(image[:,:,1].ravel(),256,[0,256],color = "green")
plt.title("G Channel")
plt.show()
plt.hist(image[:,:,0].ravel(),256,[0,256],color = "blue")
plt.title("B Channel")
plt.show()


plt.hist([a for a in image[:,:,2].ravel()],256,[0,256],[0,1],color = "red")
plt.title("R Channel Normalized")
plt.show()
plt.hist([a for a in image[:,:,1].ravel()],256,[0,256],[0,1],color = "green")
plt.title("G Channel Normalized")
plt.show()
plt.hist([a for a in image[:,:,0].ravel()],256,[0,256],[0,1],color = "blue")
plt.title("B Channel  Normalized")
plt.show()


hast=plt.hist(image[:,:,2].ravel(),256,[0,256],cumulative=True,color = "red")
plt.title("R Channel CDF")
plt.show()
plt.hist(image[:,:,1].ravel(),256,[0,256],cumulative=True,color = "green")
plt.title("G Channel CDF")
plt.show()
plt.hist(image[:,:,0].ravel(),256,[0,256],cumulative=True,color = "blue")
plt.title("B Channel CDF")
plt.show()


rN,rBins,rPatches=plt.hist([a for a in image[:,:,2].ravel()],256,[0,256],[0,1],cumulative=True,color = "red")
plt.title("R Channel Normalized CDF")
plt.show()
gN,gBins,gPatches=plt.hist([a for a in image[:,:,1].ravel()],256,[0,256],[0,1],cumulative=True,color = "green")
plt.title("G Channel Normalized CDF")
plt.show()
bN,bBins,bPatches=plt.hist([a for a in image[:,:,0].ravel()],256,[0,256],[0,1],cumulative=True,color = "blue")
plt.title("B Channel  Normalized CDF")
plt.show()



# part e  Mean and standart derivation of newly equalized image


rMean=0
gMean=0
bMean=0
rStandartDerivation=0
gStandartDerivation=0
bStandartDerivation=0


bSum= sum(map(sum, (image[:,:,0])))
gSum=sum(map(sum, (image[:,:,1])))
rSum=sum(map(sum, (image[:,:,2])))

rMean=rSum/(h*w)
gMean=gSum/(h*w)
bMean=bSum/(h*w)

rStaSum=0
gStaSum=0
bStaSum=0

for x in range(len(image)):
    for y in range(len(image[x])):
        rStaSum+=math.pow(rMean-image[x][y][2],2)
        gStaSum+=math.pow(rMean-image[x][y][1],2)
        bStaSum+=math.pow(rMean-image[x][y][0],2)

rStandartDerivation=math.sqrt(rStaSum/(h*w))
gStandartDerivation=math.sqrt(gStaSum/(h*w))
bStandartDerivation=math.sqrt(bStaSum/(h*w))

print("Equalized, R channel mean:",rMean)
print("Equalized, G channel mean:",gMean)
print("Equalized, B channel mean:",bMean)

print("Equalized, R channel standart derivation:",rStandartDerivation)
print("Equalized, G channel standart derivation:",gStandartDerivation)
print("Equalized, B channel standart derivation:",bStandartDerivation)


















