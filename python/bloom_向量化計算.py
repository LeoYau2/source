#在我的電腦上這比原本的版本在處理1920*1080的圖片時總體快了剛好100倍，我的CPU是i7-9750h，記憶體是32Gb 2400Mhz雙通道

#我剛好以前有寫過色彩空間轉換的程式，一個8bpc的sRGB的彩色圖片果要轉換成黑白圖片，
#需要先把sRGB轉成線性sRGB，再轉換成線性的黑白圖片
#如果要把線性黑白圖片轉成sRGB曲線的黑白圖片需要套用sRGB的OETF


import cv2
import numpy as np
import time
#import cupy as cp
#import gc
#print(cv2.__version__)

def sRGB_EOTF_vectorized(V_sRGB): #把sRGB訊號轉換為線性訊號，輸入和輸出範圍都是0~1
    small_indices = V_sRGB <= 0.040449936 #建立一個布林類型的掩碼數組，標識V_sRGB 中哪些元素滿足條件V_sRGB <= 0.040449936
    L = np.empty_like(V_sRGB) #建立一個和V_sRGB形狀相同的空數組L，根據不同的條件，將計算結果填入這個陣列中
    L[small_indices] = V_sRGB[small_indices] / 12.92
    L[~small_indices] = ((V_sRGB[~small_indices] + 0.055 )/ 1.055) ** 2.4
    return L

def sRGB_OETF_vectorized(L): #把線性訊號轉換為sRGB訊號，輸入和輸出範圍都是0~1，使用numpy進行向量化計算
    small_indices = L <= 0.0031308 #跟上一個函數差不多
    V_sRGB = np.empty_like(L)
    V_sRGB[small_indices] = 12.92 * L[small_indices]
    V_sRGB[~small_indices] = (1 + 0.055) * L[~small_indices] ** (1/2.4) - 0.055
    return V_sRGB

def generate_sRGB_to_linear_lookup_table_8bit_vectorized(): #製作一個sRGB轉linear查找表，輸入範圍是0~255,輸出範圍是0~1，以後直接讀取，這樣速度會快很多
    sRGB_values = np.arange(0, 256) / 255.0 #產生0-255的自增數組,並歸一化到0-1作為所有可能的輸入值
    sRGB_to_linear_vectorized = sRGB_EOTF_vectorized(sRGB_values)
    return sRGB_to_linear_vectorized

def generate_linear_to_sRGB_lookup_table_16bit_vectorized(): #製作一個linear轉sRGB查找表，輸入範圍是0~65535,輸出範圍是0~1，這個查找表可以保留16EV動態範圍
    linear_values = np.arange(0, 65536) / 65535.0 #跟上一個函數差不多
    linear_to_sRGB_vectorized = sRGB_OETF_vectorized(linear_values)
    return linear_to_sRGB_vectorized

def weight(L): #把線性訊號轉換為sRGB訊號，輸入和輸出範圍都是0~1，使用numpy進行向量化計算
    small_indices = L <= 0.0031308 #跟上一個函數差不多
    V_sRGB = np.empty_like(L)
    V_sRGB[small_indices] = 12.92 * L[small_indices]
    V_sRGB[~small_indices] = (1 + 0.055) * L[~small_indices] ** (1/2.4) - 0.055
    return V_sRGB

def generate_weight_vectorized():
    linear_values = np.arange(0, 65536) / 65535.0
    linear_to_sRGB_vectorized = sRGB_OETF_vectorized(linear_values)
    return linear_to_sRGB_vectorized

use_gpu = 0
bloom_process = 1
bloom_range = 50
bloom_size = 2
bloom_strength = 0.5
weight = np.zeros((bloom_range*2+1,bloom_range*2+1,3), dtype=np.float32)
for y in range(bloom_range*2+1):
    for x in range(bloom_range*2+1):
        if (x!=bloom_range) or (y!=bloom_range):
            weight[y,x,:] = ((y-bloom_range)*(y-bloom_range)*bloom_size*bloom_size+(x-bloom_range)*(x-bloom_range)*bloom_size*bloom_size)**-1
print(weight[0][0])
print(np.sum(weight))

linear_to_sRGB = generate_linear_to_sRGB_lookup_table_16bit_vectorized()

#generate_weight_vectorized()
weight = weight * (bloom_strength / np.sum(weight))
print(weight[0][0])
#weight = weight * ((np.sum(weight) / bloom_strength))
#print(weight[0][0])
cv2.imshow('weight',weight)
gain = 128

#crop = np.array([3300,11500,1080,1920], dtype=np.int32)
crop = np.array([3300,11500,400,400], dtype=np.int32)
crop_img = np.zeros((crop[2],crop[3],3), dtype=np.float32)
input_img = cv2.imread('linear.png',cv2.IMREAD_UNCHANGED) #讀取一張8bpc的sRGB圖片
crop_img = input_img[crop[0]:crop[0]+crop[2],crop[1]:crop[1]+crop[3],0:3]
height, width, channels = input_img.shape #查看圖片寬高
print("width:",width,",height:",height,",channels:",channels)
print("width:",crop[3],",height:",crop[2],",channels:",3)

#cv2.imshow('crop_img',crop_img)
sRGB_8bpc = np.zeros((crop[2],crop[3],3), dtype=np.uint8)
#linear_sRGB_bloom_img = np.zeros((height,width,3), dtype=np.float32)
linear_sRGB_bloom_img = np.zeros((crop[2]+2*bloom_range+1,crop[3]+2*bloom_range+1, channels), dtype=np.float32)
#linear_sRGB_bloom_result_img = np.zeros((crop[2],crop[3], channels), dtype=np.float32)
#linear_sRGB_bloom_img = np.zeros((1500, 2500, channels), dtype=np.float32)
linear_sRGB_img = np.zeros((height,width,3), dtype=np.float32) #用來儲存線性sRGB彩色圖片，範圍0~1
linear_grayscale_img = np.zeros((height,width), dtype=np.float32) #用來儲存線性灰階圖片，範圍0~1
grayscale_img = np.zeros((height,width), dtype=np.uint8) #用來儲存8位元深度sRGB曲線灰階圖片，範圍0~255
sRGB_to_linear = np.zeros((256), dtype=np.float32) #用來儲存8bit查找表，需要256*4bytes=1kb記憶體
linear_to_sRGB = np.zeros((65536), dtype=np.float32) #用來儲存16bit查找表，需要65535*4bytes=256kb記憶體

start_time = time.perf_counter() #開始計時

start_time_section = start_time
sRGB_to_linear = generate_sRGB_to_linear_lookup_table_8bit_vectorized() #製作sRGB查找表
print(f"generate_sRGB_to_linear_lookup_table_8bit:{time.perf_counter()-start_time_section:.5f}s")

start_time_section = time.perf_counter()
linear_to_sRGB = generate_linear_to_sRGB_lookup_table_16bit_vectorized() #製作linear轉sRGB查找表
print(f"generate_linear_to_sRGB_lookup_table_16bit:{time.perf_counter()-start_time_section:.5f}s")

print(np.max(crop_img))
temp_int32 = np.zeros((crop[2],crop[3],3), dtype=np.uint8) #建立暫存index的陣列
print(crop_img.shape)
print(temp_int32.shape)
temp = np.zeros((257,257,3), dtype=np.float32)
temp = crop_img[0, 0] * weight
print(temp.shape)
print(temp[0][0])
print(linear_sRGB_bloom_img.shape)
print(crop_img.shape)

#for y in range(height):
#    for x in range(width):
start_time_section = time.perf_counter()
center_strength = 1-bloom_strength
full_range = 2*bloom_range+1
for y in range(crop[2]):
    for x in range(crop[3]):
        #linear_sRGB_bloom_img[y+bloom_range, x+bloom_range, :] += crop_img[y, x]
        linear_sRGB_bloom_img[y+bloom_range, x+bloom_range, :] += crop_img[y, x] * center_strength
        linear_sRGB_bloom_img[y:y+full_range, x:x+full_range, :] += crop_img[y, x] * weight

print(f"bloom_process:{time.perf_counter()-start_time_section:.5f}s")

#linear_sRGB_bloom_result_img = linear_sRGB_bloom_img[bloom_range:bloom_range+crop[2], bloom_range:bloom_range+crop[3], :]
linear_sRGB_bloom_img = linear_sRGB_bloom_img[bloom_range:bloom_range+crop[2], bloom_range:bloom_range+crop[3], :]
temp_int32 = (np.clip(np.clip(linear_sRGB_bloom_img, 0, 65535/gain) * gain, 0, 65535)).astype(np.int32)
#temp_int32 = (np.clip(linear_sRGB_bloom_result_img, 0, 65535/gain) * gain).astype(np.int32)
#print(crop_img[171,212],temp_int32[171,212])
#print(crop_img[730,1030],temp_int32[730,1030])
#sRGB_8bpc = temp_int32 /65535
sRGB_8bpc = (np.clip(linear_to_sRGB[temp_int32]*255, 0, 255)).astype(np.uint8)
temp_int32 = None
#print(sRGB_8bpc[171,212])
#print(sRGB_8bpc[730,1030])
cv2.imshow('crop_img',crop_img)
cv2.imshow('sRGB_8bpc',sRGB_8bpc)

#cv2.imwrite('crop_sRGB.png', sRGB_8bpc)
cv2.imwrite('crop_sRGB_bloom_+7EV.png', sRGB_8bpc)

'''
start_time_section = start_time
sRGB_to_linear = generate_sRGB_to_linear_lookup_table_8bit_vectorized() #製作sRGB查找表，使用numpy進行向量化計算，速度跟原方法差不多
print(f"generate_sRGB_to_linear_lookup_table_8bit:{time.perf_counter()-start_time_section:.5f}s")

start_time_section = time.perf_counter()
linear_to_sRGB = generate_linear_to_sRGB_lookup_table_16bit_vectorized() #製作linear轉sRGB查找表，使用numpy進行向量化計算，快了7倍
print(f"generate_linear_to_sRGB_lookup_table_16bit:{time.perf_counter()-start_time_section:.5f}s")

start_time_section = time.perf_counter()
linear_sRGB_img = sRGB_to_linear[input_img] #把線性sRGB訊號轉換為線性黑白訊號，使用NumPy向量化查表，速度很快
print(f"sRGB_to_linear:{time.perf_counter()-start_time_section:.5f}s")

start_time_section = time.perf_counter()
linear_grayscale_img = linear_sRGB_img[:,:,0]*0.299+linear_sRGB_img[:,:,1]*0.587+linear_sRGB_img[:,:,2]*0.114 #把線性sRGB轉換成線性的黑白圖片，使用numpy向量-純量乘法，速度很快
#linear_grayscale_img = np.dot(linear_sRGB_img[:, :, :3], [0.299, 0.587, 0.114])
print(f"linear_sRGB_to_linear_grayscale:{time.perf_counter()-start_time_section:.5f}s")

start_time_section = time.perf_counter()
temp_int32 = np.zeros((height,width), dtype=np.int32) #建立暫存index的陣列
temp_int32 = (linear_grayscale_img * 65535).astype(np.int32) #把float32量化成16bit離散訊號，所以會有一點誤差，使用numpy向量-純量乘法和類型轉換，速度很快
grayscale_img = linear_to_sRGB[temp_int32] #把16bit線性黑白圖片轉換成8位元深度sRGB曲線灰階圖片，使用NumPy向量化查表，速度很快
temp_int32 = None #把temp_int32設為None，讓temp_int32成為垃圾回收的候選對象
#gc.collect() #手動垃圾回收，不是必要的，釋放temp_int32之前占用的記憶體
print(f"linear_to_sRGB:{time.perf_counter()-start_time_section:.5f}s")
print(f"total time:{time.perf_counter()-start_time:.3f}s")
'''
#cv2.imshow('input_img',input_img) #顯示輸入圖片
#cv2.imshow('linear_sRGB_img',linear_sRGB_img) #顯示線性sRGB圖片
#cv2.imshow('linear_grayscale_img',linear_grayscale_img) #顯示線性黑白圖片
#cv2.imshow('grayscale_img_numpy',grayscale_img) #顯示8位元深度sRGB曲線灰階圖片

#OETF_2
'''
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
        '''
'''
void Process_v1(Vec<float, 3>* __ptr64 im_3f_p, Vec<float, 3>* __ptr64 im_3f_2_p,int rows, int cols ,int* p, float* color_3f, float* size, int* range)
{
    float t;
    Vec3f* rxcols;
    Vec3f* rxcols_2;
    for (int r = p[0] - range[0]; r < p[0] + range[0] + 1; r++)
    {
        rxcols = im_3f_p + r * cols;
        rxcols_2 = im_3f_2_p + r * cols;
        for (int c = p[1] - range[1]; c < p[1] + range[1] + 1; c++)
        {
            if (r > -1 && r<rows && c>-1 && c < cols && (r != p[0] || c != p[1]))//(r != p[0] || c != p[1])
            {
                t = (pow((float(r) - p[0]) / size[0], 2) + pow((float(c) - p[1]) / size[1], 2));
                rxcols_2[c] = rxcols_2[c] + Vec3f(color_3f[0] / t, color_3f[1] / t, color_3f[2] / t);//(((r-p[0])^2)+ ((c - p[1]) ^ 2) )^-1
                //cout << p[0] << p[1] << endl;
            }
        }
    }
    //cout << *(im_3f_p + p[0] * cols + p[1]) << endl;
}

void Process_v2(Vec<float, 3>* __ptr64 im_3f_p, Vec<float, 3>* __ptr64 im_3f_2_p, int rows, int cols, int* p, float* color_3f, float* size, int* range)
{
    float t, dcolor_3f[3];
    Vec3f* position_p = im_3f_2_p + p[0] * cols + p[1];
    uint ixi;
    for (int r = 1; r < range[0] + 1; r++)
    {
        //rxcols = im_3f_p + r * im.cols;
        for (int c = 1; c < range[1] + 1; c++)
        {
            t = (pow(float(r) / size[0], 2) + pow(float(c) / size[1], 2));
            dcolor_3f[0] = color_3f[0] / t;
            dcolor_3f[1] = color_3f[1] / t;
            dcolor_3f[2] = color_3f[2] / t;
            //cout << "r=" << r << " c=" << c << " " << p[0] << "," << p[1] << endl;
            if (p[0] + r > -1 && p[0] + r <rows && p[1] + c>-1 && p[1] + c < cols)//&&(r != p[0] || c != p[1])
            {
                //cout << "A " << p[0] + r << " " << p[1] + c << endl;
                //t = 1/ (pow((float(r) - p[0]) / size[0], 2) + pow((float(c) - p[1]) / size[1], 2));
                *(position_p + r * cols + c) = *(position_p + r * cols + c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);
                //sum += color_3f[1] / (pow((float(r) - p[0]) / size, 2) + pow((float(c) - p[1]) / size, 2));
                //sum += t;
                //cout << r << "," << c << " s4 " << t << endl;
            }
            if (p[0] + r > -1 && p[0] + r <rows && p[1] - c>-1 && p[1] - c < cols)
            {
                //cout << "B " << p[0] + r << " " << p[1] - c << endl;
                *(position_p + r * cols - c) = *(position_p + r * cols - c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);//
            }
            if (p[0] - r > -1 && p[0] - r <rows && p[1] + c>-1 && p[1] + c < cols)
            {
                //cout << "C " << p[0] - r << " " << p[1] + c << endl;
                *(position_p - r * cols + c) = *(position_p - r * cols + c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);//
            }
            if (p[0] - r > -1 && p[0] - r <rows && p[1] - c>-1 && p[1] - c < cols)
            {
                //cout << "D " << p[0] - r << " " << p[1] - c << endl;
                *(position_p - r * cols - c) = *(position_p - r * cols - c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);//
            }
            //cout << "r=" << r << " c=" << c << " "<< p[0] << "," << p[1] << endl;
        }
    }

    for (int i = 1; i < range[0] + 1; i++)
    {
        ixi = i * i;
        //cout << im_3f_2_p << " " << p[0] * cols + p[1] << " " << prow << " " << im_3f_2_p + (p[0] + i) * cols + p[1] << " " << endl;
        if ( p[0] + i < rows)
        {
            *(position_p + i * cols) = *(position_p + i * cols) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
        }
        if ( p[0] - i > -1 )
        {
            *(position_p - i * cols) = *(position_p - i * cols) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
        }
    }
    for (int i = 1; i < range[1] + 1; i++)
    {
        ixi = i * i;
        if ( p[1] + i < cols)
        {
            *(position_p + i) = *(position_p + i) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
        }
        if ( p[1] - i > -1)
        {
            *(position_p - i) = *(position_p - i) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
        }
    }
}

void Process_v3(Vec<float, 3>* __ptr64 input, Vec<float, 3>* __ptr64 im_3f_p, Vec<float, 3>* __ptr64 im_3f_2_p, int rows, int cols, int* p, float* color_3f, float* size, int* range)
{
    float t, dcolor_3f[3];
    Vec3f* position_p = im_3f_2_p + p[0] * cols + p[1];
    uint ixi;
    for (int r = 1; r < range[0] + 1; r++)
    {
        //rxcols = im_3f_p + r * im.cols;
        for (int c = 1; c < range[1] + 1; c++)
        {
            t = (pow(float(r) / size[0], 2) + pow(float(c) / size[1], 2));
            //dcolor_3f[0] = color_3f[0] / t;
            //dcolor_3f[1] = color_3f[1] / t;
            //dcolor_3f[2] = color_3f[2] / t;
            //*(position_p + r * cols + c) = *(position_p + r * cols + c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);
            *(position_p + r * cols - c) = *(position_p + r * cols - c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);
            *(position_p - r * cols + c) = *(position_p - r * cols + c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);
            *(position_p - r * cols - c) = *(position_p - r * cols - c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);
            *(position_p + r * cols + c) = *(position_p + r * cols + c) + Vec3f(dcolor_3f[0], dcolor_3f[1], dcolor_3f[2]);
            //*(position_p + r * cols - c) = *(position_p + r * cols - c) + *(input + r * cols + c) / t;
            //*(position_p - r * cols + c) = *(position_p - r * cols + c) + *(input + r * cols + c) / t;
            //*(position_p - r * cols - c) = *(position_p - r * cols - c) + *(input + r * cols + c) / t;
            //*(position_p + r * cols + c) = *(position_p + r * cols + c) + *(input + r * cols + c) / t;
        }
    }

    for (int i = 1; i < range[0] + 1; i++)
    {
        ixi = i * i;
        *(position_p + i * cols) = *(position_p + i * cols) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
        *(position_p - i * cols) = *(position_p - i * cols) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
    }
    for (int i = 1; i < range[1] + 1; i++)
    {
        ixi = i * i;
        *(position_p + i) = *(position_p + i) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
        *(position_p - i) = *(position_p - i) + Vec3f(color_3f[0] / ixi, color_3f[1] / ixi, color_3f[2] / ixi);
    }
}
'''
