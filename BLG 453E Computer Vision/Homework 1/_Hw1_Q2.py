
import cv2
from matplotlib import pyplot as plt

Color1 = cv2.imread('color1.png')
Color2 = cv2.imread('color2.png')

(h,w)=Color1.shape[:2]  #height and width values of the image


# requested histograms of color1
print("Color 1 Graphics \n")

plt.hist(Color1[:,:,2].ravel(),256,[0,256],color = "red")
plt.title("R Channel")
plt.show()
plt.hist(Color1[:,:,1].ravel(),256,[0,256],color = "green")
plt.title("G Channel")
plt.show()
plt.hist(Color1[:,:,0].ravel(),256,[0,256],color = "blue")
plt.title("B Channel")
plt.show()


plt.hist([a for a in Color1[:,:,2].ravel()],256,[0,256],[0,1],color = "red")
plt.title("R Channel Normalized")
plt.show()
plt.hist([a for a in Color1[:,:,1].ravel()],256,[0,256],[0,1],color = "green")
plt.title("G Channel Normalized")
plt.show()
plt.hist([a for a in Color1[:,:,0].ravel()],256,[0,256],[0,1],color = "blue")
plt.title("B Channel  Normalized")
plt.show()



#clor1 cdf calculation
rCDF1,rBins,rPatches=plt.hist([a for a in Color1[:,:,2].ravel()],256,[0,256],[0,1],cumulative=True,color = "red")
plt.title("R Channel Normalized CDF")
gCDF1,gBins,gPatches=plt.hist([a for a in Color1[:,:,1].ravel()],256,[0,256],[0,1],cumulative=True,color = "green")
plt.title("G Channel Normalized CDF")
bCDF1,bBins,bPatches=plt.hist([a for a in Color1[:,:,0].ravel()],256,[0,256],[0,1],cumulative=True,color = "blue")
plt.title("B Channel  Normalized CDF")


# requested histograms of color2
print("\n\nColor 2 Graphics \n")

plt.hist(Color2[:,:,2].ravel(),256,[0,256],color = "red")
plt.title("R Channel")
plt.show()
plt.hist(Color2[:,:,1].ravel(),256,[0,256],color = "green")
plt.title("G Channel")
plt.show()
plt.hist(Color2[:,:,0].ravel(),256,[0,256],color = "blue")
plt.title("B Channel")
plt.show()

plt.hist([a for a in Color2[:,:,2].ravel()],256,[0,256],[0,1],color = "red")
plt.title("R Channel Normalized")
plt.show()
plt.hist([a for a in Color2[:,:,1].ravel()],256,[0,256],[0,1],color = "green")
plt.title("G Channel Normalized")
plt.show()
plt.hist([a for a in Color2[:,:,0].ravel()],256,[0,256],[0,1],color = "blue")
plt.title("B Channel  Normalized")
plt.show()


#clor2 cdf calculation


rCDF2,rBins,rPatches=plt.hist([a for a in Color2[:,:,2].ravel()],256,[0,256],[0,1],cumulative=True,color = "red")
plt.title("R Channel Normalized CDF")
gCDF2,gBins,gPatches=plt.hist([a for a in Color2[:,:,1].ravel()],256,[0,256],[0,1],cumulative=True,color = "green")
plt.title("G Channel Normalized CDF")
bCDF2,bBins,bPatches=plt.hist([a for a in Color2[:,:,0].ravel()],256,[0,256],[0,1],cumulative=True,color = "blue")
plt.title("B Channel  Normalized CDF")


#part d   look up table calculations for each channel

LUTR=[0 for a in range(0,256)]
gj=0
myBool=True
for gi in range(0,256):
    if gj==256:
        gj=255
        myBool=False
    while rCDF2[gj] < rCDF1[gi] and gj<256:
        gj=gj+1
        if gj==256:
            myBool=False
            break        
    LUTR[gi]=gj


LUTG=[0 for a in range(0,256)]
gj=0
myBool=True

for gi in range(0,256):
    if gj==256:
        gj=255
        myBool=False
    while gCDF2[gj] < gCDF1[gi] and gj<256 and myBool:
        gj=gj+1
        if gj==256:
            myBool=False
            break
    LUTG[gi]=gj

LUTB=[0 for a in range(0,256)]
gj=0
myBool=True
for gi in range(0,256):
    if gj==256:
        gj=255
    while bCDF2[gj] < bCDF1[gi] and gj<256 and myBool:
        gj=gj+1
        if gj==256:
            myBool=False
            break
    LUTB[gi]=gj


#Histogram matching
for x in range(len(Color1)):    
    for y in range(len(Color1[x])):
        Color1[x][y][0]=LUTB[Color1[x][y][0]]
        Color1[x][y][1]=LUTG[Color1[x][y][1]]
        Color1[x][y][2]=LUTR[Color1[x][y][2]]


#histogram tables of histogram matched new image
cv2.imshow("Histogram Matched Image",Color1)
cv2.waitKey(0)
cv2.destroyAllWindows()

cv2.imwrite("HistogramMatchedImage.jpg",Color1)

print("Histograms of The Matched Image")

plt.hist(Color1[:,:,2].ravel(),256,[0,256],color = "red")
plt.title("R Channel")
plt.show()
plt.hist(Color1[:,:,1].ravel(),256,[0,256],color = "green")
plt.title("G Channel")
plt.show()
plt.hist(Color1[:,:,0].ravel(),256,[0,256],color = "blue")
plt.title("B Channel")
plt.show()


plt.hist([a for a in Color1[:,:,2].ravel()],256,[0,256],[0,1],color = "red")
plt.title("R Channel Normalized")
plt.show()
plt.hist([a for a in Color1[:,:,1].ravel()],256,[0,256],[0,1],color = "green")
plt.title("G Channel Normalized")
plt.show()
plt.hist([a for a in Color1[:,:,0].ravel()],256,[0,256],[0,1],color = "blue")
plt.title("B Channel  Normalized")
plt.show()






