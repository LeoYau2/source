import math
import time
import cv2
import matplotlib
import numpy

#img = cv2.imread('linear.png',-1)
img = cv2.imread('crop_-13ev_PQ_2.png',cv2.IMREAD_UNCHANGED)
#img = cv2.imread('crop -8ev_XYZ_7.png',cv2.IMREAD_UNCHANGED)
size = (1080,1920)
sample = (200,500,2)
x=0
print(img[0,0])
#print(img[0,855])
print(img[200,0],"200 0")
print(img[0,200],"0 200")


print(img[200,100])
print(img[200,150])
print(img[100,100])
print(img[479,959])
#crop = (3200,11000)
crop = (0,0)
frame = numpy.zeros((size[0], size[1] ,3), numpy.float32)
CIE_XYZ = numpy.zeros((size[0], size[1] ,3), numpy.float32)
YUV = numpy.zeros((size[0], size[1] ,3), numpy.float32)
out = numpy.zeros((size[0], size[1] ,3), numpy.uint16)

im_size = frame.shape
print(img.dtype)
print(img.shape)
print(img.size)

for i in range(im_size[0]):
    
    for j in range(im_size[1]):
        """
        #frame[i,j] = img[crop[0]+i,crop[1]+j]/8192
        """
        #PQ EOTF
        
        frame[i,j] = 1*(-((128*(img[crop[0]+i,crop[1]+j]/65535)**0.0126833135-107)/(2392*(img[crop[0]+i,crop[1]+j]/65535)**0.0126833135-2413)))**6.2773946360
        
        #CIE_XYZ to sRGB
        """
        frame[i,j,2] =  3.2404542*img[crop[0]+i,crop[1]+j,2]/65535 -1.5371385*img[crop[0]+i,crop[1]+j,1]/65535 -0.4985314*img[crop[0]+i,crop[1]+j,0]/65535
        frame[i,j,1] = -0.9692660*img[crop[0]+i,crop[1]+j,2]/65535 +1.8760108*img[crop[0]+i,crop[1]+j,1]/65535 +0.0415560*img[crop[0]+i,crop[1]+j,0]/65535
        frame[i,j,0] =  0.0556434*img[crop[0]+i,crop[1]+j,2]/65535 -0.2040259*img[crop[0]+i,crop[1]+j,1]/65535 +1.0572252*img[crop[0]+i,crop[1]+j,0]/65535
        """
        #sRGB EOTF
        #0.00392768*65535=2574.005
        """
        if img[i,j,0] <= 2650:
            frame[i,j,0] = img[crop[0]+i,crop[1]+j,0]/65535/12.92
        else:
            frame[i,j,0] = ((img[crop[0]+i,crop[0]+j,0]/65535+0.055)/1.055)**2.4
        if img[i,j,1] <= 2650:
            frame[i,j,1] = img[crop[0]+i,crop[1]+j,1]/65535/12.92
        else:
            frame[i,j,1] = ((img[crop[0]+i,crop[1]+j,1]/65535+0.055)/1.055)**2.4
        if img[i,j,2] <= 2650:
            frame[i,j,2] = img[crop[0]+i,crop[1]+j,2]/65535/12.92
        else:
            frame[i,j,2] = ((img[crop[0]+i,crop[1]+j,2]/65535+0.055)/1.055)**2.4
        """
        #HLG EOTF
        """
        if img[crop[0]+i,crop[1]+j,0] <= 0.5:
            frame[i,j,0] = img[crop[0]+i,crop[0]+j,0]**2/3*65535
        else:
            frame[i,j,0] = (math.exp(((img[crop[0]+i,crop[1]+j,0]-0.55991073)/0.17883277))+0.28466892)/12*65535
        
        if img[crop[0]+i,crop[1]+j,1] <= 0.5:
            frame[i,j,1] = img[crop[0]+i,crop[1]+j,1]**2/3*65535
        else:
            frame[i,j,1] = (math.exp(((img[crop[0]+i,crop[1]+j,1]-0.55991073)/0.17883277))+0.28466892)/12*65535
        
        if img[crop[0]+i,crop[1]+j,2] <= 0.5:
            frame[i,j,2] = img[crop[0]+i,crop[1]+j,2]**2/3*65535
        else:
            frame[i,j,2] = (math.exp(((img[crop[0]+i,crop[1]+j,2]-0.55991073)/0.17883277))+0.28466892)/12*65535
            """
        """
        #frame[200,500,2] = ((img[200,500,2]/65535+0.055)/1.055)**2.4


        
        #frame[i,j,0] = (img[crop[0]+i,crop[1]+j,0]/65535)**2.2
        #frame[i,j,1] = (img[crop[0]+i,crop[1]+j,1]/65535)**2.2
        #frame[i,j,2] = (img[crop[0]+i,crop[1]+j,2]/65535)**2.2
        #frame[i,j,1] = img[crop[0]+i,crop[1]+j,1]
        #frame[i,j,2] = img[crop[0]+i,crop[1]+j,2]
        """
        """
        if frame[i,j,0]>1:
            frame[i,j,0]=1
        if frame[i,j,1]>1:
            frame[i,j,1]=1
        if frame[i,j,2]>1:
            frame[i,j,2]=1
        """
"""
for i in range(im_size[0]):
    for j in range(im_size[1]):
        if img[i,j,0] > x:
            frame[i,j,0]=255
        if img[i,j,1] > x:
            frame[i,j,1]=255
        if img[i,j,2] > x:
            frame[i,j,2]=255
"""
            
"""
frame = numpy.zeros((256, 128, 3), numpy.uint8)
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
##print('img(200,500,R) =',img[sample[0],sample[1],sample[2]])
##print('frame(200,500,R) =',frame[sample[0],sample[1],sample[2]])


for i in range(im_size[0]):
    for j in range(im_size[1]):
        #PQ OETF
        #out[i,j] = ((0.8359375+18.8515625*frame[i,j]**0.1593017578125)/(1+18.6875*frame[i,j]**0.1593017578125))**78.84375*65535
        #PQ EOTF
        #L = (-((128*V_PQ**(32/2523)-107)/(2392*V_PQ**(32/2523)-2413)))**(8192/1305)
        #L = (-((128*V_PQ**0.0126833135-107)/(2392*V_PQ**0.0126833135-2413)))**6.2773946360
        #out[i,j] = (-((128*frame[i,j]**0.0126833135-107)/(2392*frame[i,j]**0.0126833135-2413)))**6.2773946360
        #out[i,j] = ((0.8359375+18.8515625*(img[crop[0]+i,crop[1]+j]/1)**0.1593017578125)/(1+18.6875*(img[crop[0]+i,crop[1]+j]/1)**0.1593017578125))**78.84375*65535
    
        #HLG OETF
        """
        if frame[i,j,0] <= 1/12:
            out[i,j,0] = (1.7320508075688772*frame[i,j,0]**0.5)*65535
        else:
            out[i,j,0] = (0.17883277*math.log(12*frame[i,j,0]-0.28466892)+0.55991073)*65535
        if frame[i,j,1] <= 1/12:
            out[i,j,1] = (1.7320508075688772*frame[i,j,1]**0.5)*65535
        else:
            out[i,j,1] = (0.17883277*math.log(12*frame[i,j,1]-0.28466892)+0.55991073)*65535
        if frame[i,j,2] <= 1/12:
            out[i,j,2] = (1.7320508075688772*frame[i,j,2]**0.5)*65535
        else:
            out[i,j,2] = (0.17883277*math.log(12*frame[i,j,2]-0.28466892)+0.55991073)*65535
    """
        
        #sRGB OETF
        #12.92*65535=846712.2
        """
        if frame[i,j,0] <= 0.0031308:
            out[i,j,0] = frame[i,j,0]*846712.2#65535*12.92
        else:
            out[i,j,0] = (1.055*frame[i,j,0]**(1/2.4)-0.055)*65535#*65535
        if frame[i,j,1] <= 0.0031308:
            out[i,j,1] = frame[i,j,1]*846712.2#65535*12.92
        else:
            out[i,j,1] = (1.055*frame[i,j,1]**(1/2.4)-0.055)*65535#*65535
        if frame[i,j,2] <= 0.0031308:
            out[i,j,2] = frame[i,j,2]*846712.2#65535*12.92
        else:
            out[i,j,2] = (1.055*frame[i,j,2]**(1/2.4)-0.055)*65535#*65535
        """
        #OETF_1
        """
        if frame[i,j,0] <= 0.0031308:
            out[i,j,0] = frame[i,j,0]*846712.2#65535*12.92
        elif frame[i,j,0] <= 0.332238:
            out[i,j,0] = (1.055*frame[i,j,0]**(1/2.4)-0.055)*65535#*65535
        else:
            out[i,j,0] = math.atan(4*frame[i,j,0]+0.10146692)*2/math.pi*65535
            
        if frame[i,j,1] <= 0.0031308:
            out[i,j,1] = frame[i,j,1]*846712.2#65535*12.92
        elif frame[i,j,1] <= 0.332238:
            out[i,j,1] = (1.055*frame[i,j,1]**(1/2.4)-0.055)*65535#*65535
        else:
            out[i,j,1] = math.atan(4*frame[i,j,1]+0.10146692)*2/math.pi*65535
            
        if frame[i,j,2] <= 0.0031308:
            out[i,j,2] = frame[i,j,2]*846712.2#65535*12.92
        elif frame[i,j,2] <= 0.332238:
            out[i,j,2] = (1.055*frame[i,j,2]**(1/2.4)-0.055)*65535#*65535
        else:
            out[i,j,2] = math.atan(4*frame[i,j,2]+0.10146692)*2/math.pi*65535
        """
        #OETF_2
        """
        if frame[i,j,0] <= 0.0031308:
            out[i,j,0] = frame[i,j,0]*846712.2#65535*12.92
        elif frame[i,j,0] <= 0.237335:
            out[i,j,0] = (1.055*frame[i,j,0]**(1/2.4)-0.055)*65535#*65535
        else:
            out[i,j,0] = ((math.atan(6*frame[i,j,0]+1.1274908)*4/math.pi)-1)*65535
            
        if frame[i,j,1] <= 0.0031308:
            out[i,j,1] = frame[i,j,1]*846712.2#65535*12.92
        elif frame[i,j,1] <= 0.237335:
            out[i,j,1] = (1.055*frame[i,j,1]**(1/2.4)-0.055)*65535#*65535
        else:
            out[i,j,1] = ((math.atan(6*frame[i,j,1]+1.1274908)*4/math.pi)-1)*65535
            
        if frame[i,j,2] <= 0.0031308:
            out[i,j,2] = frame[i,j,2]*846712.2#65535*12.92
        elif frame[i,j,2] <= 0.237335:
            out[i,j,2] = (1.055*frame[i,j,2]**(1/2.4)-0.055)*65535#*65535
        else:
            out[i,j,2] = ((math.atan(6*frame[i,j,2]+1.1274908)*4/math.pi)-1)*65535
        """
        #sRGB to CIE XYZ
        
        CIE_XYZ[i,j,2] = 1*(0.4124564*frame[i,j,2] +0.3575761*frame[i,j,1] +0.1804375*frame[i,j,0])
        CIE_XYZ[i,j,1] = 1*(0.2126729*frame[i,j,2] +0.7151522*frame[i,j,1] +0.0721750*frame[i,j,0])
        CIE_XYZ[i,j,0] = 1*(0.0193339*frame[i,j,2] +0.1191920*frame[i,j,1] +0.9503041*frame[i,j,0])
        """
        if CIE_XYZ[i,j,2]>1:
            CIE_XYZ[i,j,2]=1
        if CIE_XYZ[i,j,1]>1:
            CIE_XYZ[i,j,1]=1
        if CIE_XYZ[i,j,0]>1:
            CIE_XYZ[i,j,0]=1
        out[i,j,2] = 65535*CIE_XYZ[i,j,2]
        out[i,j,1] = 65535*CIE_XYZ[i,j,2]
        out[i,j,0] = 65535*CIE_XYZ[i,j,2]
        """
        #RGB to YUV
        """
        YUV[i,j,0] = ( 0.299*out[i,j,2]+0.587*out[i,j,1]+0.114*out[i,j,0])/255#*65536
        YUV[i,j,1] = (-0.147*out[i,j,2]-0.289*out[i,j,1]+0.436*out[i,j,0])/255#*65536
        YUV[i,j,2] = ( 0.615*out[i,j,2]-0.515*out[i,j,1]-0.1  *out[i,j,0])/255#*65536
        
        #YUV to RGB
        out[i,j,2] = (YUV[i,j,0]                 +1.140*YUV[i,j,2])*65535
        out[i,j,1] = (YUV[i,j,0]-0.394*YUV[i,j,1]-0.581*YUV[i,j,2])*65535
        out[i,j,0] = (YUV[i,j,0]+2.032*YUV[i,j,1]                 )*65535
        """

print(frame[200,200])
print(out[200,200])
#print(YUV[200,200])
#out[200,500,2] = (1.055*frame[200,500,2]**(1/2.4)-0.055)*65535
#out[200,500,2] = (1.055*frame[200,500,2]*12.92*65535
#print('out(200,500,R) =',out[sample[0],sample[1],sample[2]])
cv2.imshow('out',out)
#cv2.imshow('YUV',YUV)
#cv2.imwrite('crop -5ev_OETF_2_1.png',out)
#cv2.imwrite('crop -8ev_XYZ_7.png',out)
#cv2.imwrite('crop -13ev_exr_1.exr',frame)
cv2.imwrite('crop -13ev_XYZ_1.exr',CIE_XYZ)


#((0.8359375+18.8515625*L**0.1593017578125)/(1+18.6875*L**0.1593017578125))**78.84375