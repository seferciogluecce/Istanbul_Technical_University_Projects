import cv2

Iimage = cv2.imread('picture.JPG')

vertical = Iimage.copy()
horizontal = Iimage.copy()

temp = Iimage.copy()

(h,w)=Iimage.shape[:2]  #height and widt values of the image

twoPieces = Iimage.copy() 

twoPieces[0:h,0:w/2]=temp[0:h,w/2:w]   #two piece effect operations
twoPieces[0:h,w/2:w]=temp[0:h,0:w/2]

cv2.imwrite('Q3_TwoPieces.jpg', twoPieces)



fourPieces = Iimage.copy()

fourPieces[0:h/2,0:w]=twoPieces[h/2:h,0:w]   #four piece effect operations
fourPieces[h/2:h,0:w]=twoPieces[0:h/2,0:w]

cv2.imwrite('Q3_FourPieces.jpg', fourPieces)


vertical= Iimage[:,::-1]  #vertical flip by reversing each row in Iimage


trans=Iimage.T
horizontal= trans[...,::-1].T   #horizontal flip effet by  transpose, reverse each row, transpose Iimage

cv2.imwrite('Q3_Vertical.jpg',vertical)
cv2.imwrite('Q3_Horizontal.jpg',horizontal)