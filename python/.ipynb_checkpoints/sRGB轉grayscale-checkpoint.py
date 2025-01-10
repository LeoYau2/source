#我剛好以前有寫過色彩空間轉換的程式，一個8bpc的sRGB的彩色圖片果要轉換成黑白圖片，
#需要先把sRGB轉成線性sRGB，再轉換成線性的黑白圖片
#如果要把線性黑白圖片轉成sRGB曲線的黑白圖片需要套用sRGB的OETF

import cv2
import numpy

def sRGB_EOTF(V_sRGB): #把sRGB訊號轉換為線性訊號，輸入和輸出範圍都是0~1
    if V_sRGB <= 0.040449936:
        L = V_sRGB/12.92
    else:
        L = ((V_sRGB+0.055)/1.055)**2.4
    return L

def sRGB_OETF(L): #把線性訊號轉換為sRGB訊號，輸入和輸出範圍都是0~1
    if L <= 0.0031308:
        V_sRGB = 12.92*L
    else:
        V_sRGB = (1+0.055)*L**(1/2.4)-0.055
    return V_sRGB
    
def generate_sRGB_to_linear_lookup_table_8bit(): #製作一個sRGB轉linear查找表，輸入範圍是0~255,輸出範圍是0~1，以後直接讀取，這樣速度會快很多，但是我不會寫多執行緒程式
    for i in range(256):
        sRGB_to_linear[i]=sRGB_EOTF(i/255)

def generate_linear_to_sRGB_lookup_table_16bit(): #製作一個linear轉sRGB查找表，輸入範圍是0~65535,輸出範圍是0~1，這個查找表可以保留16EV動態範圍
    for i in range(65536):
        linear_to_sRGB[i]=sRGB_OETF(i/65535)

def linear_sRGB_to_linear_grayscale(rgb_values): #把線性sRGB訊號轉換為線性黑白訊號，輸入和輸出範圍都是0~1
    return(rgb_values[0] * 0.299 + rgb_values[1] * 0.587 + rgb_values[2] * 0.114)

input_img = cv2.imread('input.png',cv2.IMREAD_UNCHANGED) #讀取一張8bpc的sRGB圖片
height, width, channels = input_img.shape #查看圖片寬高
print("width:",width,",height:",height,",channels:",channels)
cv2.imshow('input_img',input_img) #顯示輸入圖片
linear_sRGB_img = numpy.zeros((height,width,3), dtype=numpy.float32) #用來儲存線性sRGB彩色圖片，範圍0~1
linear_grayscale_img = numpy.zeros((height,width), dtype=numpy.float32) #用來儲存線性灰階圖片，範圍0~1
grayscale_img = numpy.zeros((height,width), dtype=numpy.uint8) #用來儲存8位元深度sRGB曲線灰階圖片，範圍0~255
sRGB_to_linear = numpy.zeros((256), dtype=numpy.float32) #用來儲存8bit查找表，需要256*4bytes=1kb記憶體
linear_to_sRGB = numpy.zeros((65536), dtype=numpy.float32) #用來儲存16bit查找表，需要65535*4bytes=256kb記憶體
generate_sRGB_to_linear_lookup_table_8bit() #製作sRGB查找表
generate_linear_to_sRGB_lookup_table_16bit() #製作linear轉sRGB查找表
for i in range(width):
    for j in range(height):
        input_img[j,i,:] = [int(input_img[j,i,0]/257),int(input_img[j,i,1]/257),int(input_img[j,i,2]/257)] #輸入的圖片值的範圍是0~65535，要轉換到0~255
        linear_sRGB_img[j,i,:] = [sRGB_to_linear[input_img[j,i,0]],sRGB_to_linear[input_img[j,i,1]],sRGB_to_linear[input_img[j,i,2]]] #把sRGB轉成線性sRGB

for i in range(width):
    for j in range(height):
        linear_grayscale_img[j,i] = linear_sRGB_to_linear_grayscale(linear_sRGB_img[j,i,:]) #把線性sRGB轉換成線性的黑白圖片

for i in range(width):
    for j in range(height):
        grayscale_img[j,i] = int(linear_to_sRGB[int(linear_grayscale_img[j,i]*65535)]*255) #把線性黑白圖片轉換成8位元深度sRGB曲線灰階圖片

cv2.imshow('linear_sRGB_img',linear_sRGB_img) #顯示線性sRGB圖片
cv2.imshow('linear_grayscale_img',linear_grayscale_img) #顯示線性黑白圖片
cv2.imshow('grayscale_img',grayscale_img) #顯示8位元深度sRGB曲線灰階圖片
