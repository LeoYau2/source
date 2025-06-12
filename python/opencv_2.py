import math
import time
import cv2
import matplotlib
import numpy

#img = cv2.imread('linear.png',-1)
img = cv2.imread('1_480p_16b.png',cv2.IMREAD_UNCHANGED)
size = (480,960)
sample = (200,500,2)
x=0
crop = (3200,11000)
frame = numpy.zeros((size[0], size[1] ,3), numpy.float32)
out = numpy.zeros((size[0], size[1] ,3), numpy.uint16)
im_size = frame.shape

for i in range(im_size[0]):
    
    for j in range(im_size[1]):
        #sRGB EOTF
        #0.00392768*65535=2574.005
        
        if img[i,j,0] <= 258:
            frame[i,j,0] = img[i,j,0]/65535/12.92
        else:
            frame[i,j,0] = ((img[i,j,0]/65535+0.055)/1.055)**2.4
        if img[i,j,1] <= 258:
            frame[i,j,1] = img[i,j,1]/65535/12.92
        else:
            frame[i,j,1] = ((img[i,j,1]/65535+0.055)/1.055)**2.4
        
        if img[i,j,2] <= 258:
            frame[i,j,2] = img[i,j,2]/65535/12.92
        else:
            frame[i,j,2] = ((img[i,j,2]/65535+0.055)/1.055)**2.4


#frame[200,500,2] = ((img[200,500,2]/65535+0.055)/1.055)**2.4

"""
        if frame[i,j,0]>1:
            frame[i,j,0]=1
        if frame[i,j,1]>1:
            frame[i,j,1]=1
        if frame[i,j,2]>1:
            frame[i,j,2]=1"""
        #frame[i,j,0] = (img[crop[0]+i,crop[1]+j,0]/65535)**2.2
        #frame[i,j,1] = (img[crop[0]+i,crop[1]+j,1]/65535)**2.2
        #frame[i,j,2] = (img[crop[0]+i,crop[1]+j,2]/65535)**2.2
        #frame[i,j,1] = img[crop[0]+i,crop[1]+j,1]
        #frame[i,j,2] = img[crop[0]+i,crop[1]+j,2]

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

print('sample =',sample)
print('img(200,500,R) =',img[sample[0],sample[1],sample[2]])
print('frame(200,500,R) =',frame[sample[0],sample[1],sample[2]])


for i in range(im_size[0]):
    for j in range(im_size[1]):
        #PQ OETF
        #out[i,j] = ((0.8359375+18.8515625*frame[i,j]**0.1593017578125)/(1+18.6875*frame[i,j]**0.1593017578125))**78.84375*65535

        #sRGB OETF
        #12.92*65535=846712.2
        if frame[i,j,0] <= 0.00304:
            out[i,j,0] = frame[i,j,0]*846712.2
        else:
            out[i,j,0] = (1.055*frame[i,j,0]**(1/2.4)-0.055)*65535
        if frame[i,j,1] <= 0.00304:
            out[i,j,1] = frame[i,j,1]*846712.2
        else:
            out[i,j,1] = (1.055*frame[i,j,1]**(1/2.4)-0.055)*65535
        if frame[i,j,2] <= 0.00304:
            out[i,j,2] = frame[i,j,2]*846712.2
        else:
            out[i,j,2] = (1.055*frame[i,j,2]**(1/2.4)-0.055)*65535



#out[200,500,2] = (1.055*frame[200,500,2]**(1/2.4)-0.055)*65535
#out[200,500,2] = (1.055*frame[200,500,2]*12.92*65535
print('out(200,500,R) =',out[sample[0],sample[1],sample[2]])
cv2.imshow('image',out)
#cv2.imwrite('out_3.png',out)

#((0.8359375+18.8515625*L**0.1593017578125)/(1+18.6875*L**0.1593017578125))**78.84375
