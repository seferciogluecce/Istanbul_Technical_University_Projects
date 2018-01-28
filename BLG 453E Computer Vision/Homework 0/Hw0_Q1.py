import cv2

Icolor = cv2.imread('picture.JPG')


r = Icolor.copy() #red channel, block green and blue
r[:, :, 0] = 0
r[:, :, 1] = 0


g = Icolor.copy() #green channel, block red and blue
g[:, :, 0] = 0
g[:, :, 2] = 0

b = Icolor.copy() #blue channel, block red and green
b[:, :, 1] = 0
b[:, :, 2] = 0

cv2.imwrite('Q1_R_Channel.jpg',r)
cv2.imwrite('Q1_G_Channel.jpg',g)
cv2.imwrite('Q1_B_Channel.jpg',b)


rb = Icolor.copy() #red-blue channel, block green 
rb[:, :, 1] = 0


rg = Icolor.copy() #red-green channel, block blue
rg[:, :, 0] = 0

gb = Icolor.copy() #green-blue channel, block red
gb[:, :, 2] = 0


cv2.imwrite('Q1_RB_Channel.jpg',rb)
cv2.imwrite('Q1_RG_Channel.jpg',rg)
cv2.imwrite('Q1_BG_Channel.jpg',gb)

