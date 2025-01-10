import time
import cv2
import matplotlib
import numpy

#img = cv2.imread('linear.png',-1)
img = cv2.imread('crop.png',cv2.IMREAD_UNCHANGED)

x=127
crop = (3200,11000)
frame = numpy.zeros((1080, 1920 ,3), numpy.float32)
out = numpy.zeros((1080, 1920 ,3), numpy.uint16)
"""im_size = frame.shape
for i in range(im_size[0]):
    
    for j in range(im_size[1]):
        #frame[i,j,0] = img[i,j,0]*1000
        frame[i,j] = (img[i,j]/65535)**2.2
        #frame[i,j,0] = (img[crop[0]+i,crop[1]+j,0]/65535)**2.2
        #frame[i,j,1] = (img[crop[0]+i,crop[1]+j,1]/65535)**2.2
        #frame[i,j,2] = (img[crop[0]+i,crop[1]+j,2]/65535)**2.2
        #frame[i,j,1] = img[crop[0]+i,crop[1]+j,1]
        #frame[i,j,2] = img[crop[0]+i,crop[1]+j,2]
"""
"""for i in range(im_size[0]):
    
    for j in range(im_size[0]):
        if img[i,j,0] > x:
            frame[i,j,0]=255
        if img[i,j,1] > x:
            frame[i,j,1]=255
        if img[i,j,2] > x:
            frame[i,j,2]=255
"""
"""frame = numpy.zeros((256, 128, 3), numpy.uint8)
for i in range(256):
    #frame[i,j,2] = i
    cv2.imshow('frame',frame)
    #time.sleep(0.1)
    #cv2.destroyAllWindows()
    #print("".format(i))
    for j in range(128):
        frame[i,j,2] = i

cv2.line(frame,(16,24),(64,90),(240,240,240),8)
cv2.circle(frame,(64,90),32,(240,240,240),8)

x=1"""
print(img.dtype)
print(img.shape)
print(img.size)
#print("".format(x))
#cv2.blur(InputArray src, OutputArray dst, Size ksize, Point anchor=Point(-1,-1), int borderType=BORDER_DEFAULT )
"""
frame = cv2.GaussianBlur(frame,(5,5),0)
frame = cv2.GaussianBlur(frame,(5,5),0)
frame = cv2.GaussianBlur(frame,(5,5),0)
frame = cv2.GaussianBlur(frame,(5,5),0)
frame = cv2.GaussianBlur(frame,(5,5),0)
frame = cv2.GaussianBlur(frame,(5,5),0)
"""
#median = cv2.medianBlur(img,5)
#blur = cv2.blur(img,(16,16))
#cv2.imshow('frame',blur)
"""
for i in range(im_size[0]):
    for j in range(im_size[1]):
        out[i,j] = frame[i,j]**(1/2.2)*65535
"""
cv2.imshow('image',img)
#cv2.imwrite('blur_6.png',out)
