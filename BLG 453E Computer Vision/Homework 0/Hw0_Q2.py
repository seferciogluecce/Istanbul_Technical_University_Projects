import cv2
import numpy as np


Igray = cv2.imread('picture.JPG')
weightedgray=Igray.copy() #graysacle with weighted channels

for x in range(len(Igray)):
    for y in range(len(Igray[x])):
        weightedgray[x][y]=Igray[x][y][0]*0.114+Igray[x][y][1]*0.578+Igray[x][y][2]*0.299


cv2.imwrite('Q2_WeightedGray.jpg',weightedgray)

averageGray=Igray.copy()  #grayscale by averaging channel values

for x in range(len(Igray)): 
    for y in range(len(Igray[x])):
        averageGray[x][y][0]=(int(Igray[x][y][0])+int(Igray[x][y][1])+int(Igray[x][y][2]))/3
        averageGray[x][y][1]=(int(Igray[x][y][0])+int(Igray[x][y][1])+int(Igray[x][y][2]))/3
        averageGray[x][y][2]=(int(Igray[x][y][0])+int(Igray[x][y][1])+int(Igray[x][y][2]))/3



cv2.imwrite('Q2_AverageGray.jpg',averageGray)


Max=np.amax(Igray) #max value in image
Min=np.amin(Igray) #min value in image

print("Max value ",Max)
print("Min value ",Min)



#0-255, 8 bit