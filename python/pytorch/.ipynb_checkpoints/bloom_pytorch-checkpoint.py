import cv2
import pyexr
import numpy as np
import time
import os
import torch

#import cupy as cp
#import gc

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

def pytorch_convolution(input_data, kernel, device):
    # 將 NumPy 轉換為 PyTorch tensor，並移動到指定設備 (CPU/GPU)
    input_tensor = torch.from_numpy(input_data).float().to(device)  # 使用 torch.from_numpy()
    kernel_tensor = torch.from_numpy(kernel).float().to(device)  # 使用 torch.from_numpy()
    
    # 獲取卷積核的維度資訊
    out_channels, in_channels, kernel_height, kernel_width = kernel.shape
    print("kernel", kernel.shape)
    
    # 計算 padding 大小，確保輸出與輸入大小相同
    padding_height = (kernel_height - 1) // 2
    padding_width = (kernel_width - 1) // 2
    
    # 建立卷積層，指定輸入/輸出通道和卷積核大小
    conv2d = torch.nn.Conv2d(
        in_channels=in_channels,  # 輸入通道數，來自卷積核的第二維
        out_channels=out_channels,  # 輸出通道數，來自卷積核的第一維
        kernel_size=(kernel_height, kernel_width),  # 卷積核大小
        stride=(1, 1),  # 步長為 1
        padding=(padding_height, padding_width),  # 使用計算出的 padding
        bias=False  # 不使用偏置項，因為原始程式中沒有
    )
    
    # 將卷積層的權重替換為自定義的卷積核
    conv2d.weight.data = kernel_tensor
    
    # 將卷積層移到指定設備 (CPU/GPU)
    conv2d = conv2d.to(device)
    
    # 執行卷積操作
    output = conv2d(input_tensor)
    return output

use_gpu = 1  # 設定為 1 表示使用 GPU，設定為 0 表示強制使用 CPU

# 根據 use_gpu 和系統是否支援 GPU 來設定裝置 (device)
if use_gpu and torch.cuda.is_available():
    device = torch.device("cuda")  # 使用 GPU
    print("Using GPU")
else:
    device = torch.device("cpu")  # 使用 CPU
    print("Using CPU")

use_crop = 0 #是否啟用裁剪
# 定義裁剪範圍 (crop)，包含裁剪區域的起始點與寬高
crop = np.array([3300, 11500, 1080, 1920], dtype=np.int32)

# 是否啟用 bloom 處理
use_bloom_process = 1
bloom_range = 100  # 光暈的方形範圍
bloom_size_factor = 0.85  # 光暈大小的影響因子
bloom_strength = 0.25  # 變成光暈的亮度比例 0~1

# 定義亮度增益，透過曝光值 (EV) 的指數計算
brightness_EV = 10
brightness_gain = 2**brightness_EV

# 初始化卷積核 (kernel)
kernel = np.zeros((3, 3, bloom_range*2+1, bloom_range*2+1))

'''
以下是使用 for 迴圈的方式計算權重
針對光暈範圍內的每個像素進行計算，根據距離中心的距離設定權重值。
這段程式碼目前被註解掉，作為參考。

for y in range(bloom_range*2+1):
    for x in range(bloom_range*2+1):
        if (x != bloom_range) or (y != bloom_range):
             weight[y, x, :] = ((y - bloom_range) * (y - bloom_range) + (x - bloom_range) * (x - bloom_range)) ** (-1 / bloom_size_factor)
'''
# 計算卷積核 (kernel) 的值，針對 R、G、B 三個通道
for y in range(bloom_range*2+1):
    for x in range(bloom_range*2+1):
        if (x != bloom_range) or (y != bloom_range):  # 排除中心點
            value = ((y - bloom_range) * (y - bloom_range) + (x - bloom_range) * (x - bloom_range)) ** (-1 / bloom_size_factor)
            kernel[0, 0, x, y] = value  # 設定藍色通道的值
            kernel[1, 1, x, y] = value  # 設定綠色通道的值
            kernel[2, 2, x, y] = value  # 設定紅色通道的值

# 生成線性到 sRGB 的查找表
linear_to_sRGB = generate_linear_to_sRGB_lookup_table_16bit_vectorized()

# 使用計算出的總權重對卷積核進行正規化以固定總亮度，並結合光暈強度 (bloom_strength)
kernel = kernel * (bloom_strength / np.sum(kernel[0, 0, :, :]))
#weight = weight * (bloom_strength / np.sum(weight[:, :, 0])

# 初始化裁剪後的影像矩陣
crop_img = np.zeros((crop[2], crop[3], 3), dtype=np.float32)

# 開啟 EXR 檔案，並取得其 RGB 資料，資料形狀為 (height, width, 3)
#exr_image = pyexr.open(r'World of worlds 3.4_08_sunx0.5+sky+emmitx4096_dn_02.exr')
exr_image = pyexr.open(r'crop.exr')
input_img_rgb = exr_image.get()  # 預設返回 RGB 資料
input_img = cv2.cvtColor(input_img_rgb, cv2.COLOR_RGB2BGR)  # 將 RGB 資料轉換為 BGR 格式

#input_img = cv2.imread('linear_bt709_16bpc.png',cv2.IMREAD_UNCHANGED)

# 輸出影像的寬、高和通道數
height, width, channels = input_img.shape  # 查看完整影像的寬高和通道數
if use_crop == 0: # 如果不啟用裁剪
    crop = np.array([0, 0, height, width], dtype=np.int32)  # 預設為整張圖片大小
print("input image channels:", channels, "width:", width, ",height:", height) # 查看輸入影像的寬高和通道數
print("crop image channels:", channels, "width:", crop[3], ",height:", crop[2])  # 查看裁剪影像的寬高和通道數

# 擷取裁剪範圍內的影像資料，並取出前三個通道 (BGR)
crop_img = input_img[crop[0]:crop[0]+crop[2], crop[1]:crop[1]+crop[3], 0:3]

# 將影像資料的形狀從 (height, width, channels) 轉為 (channels, height, width)
input_data = crop_img.transpose(2, 0, 1)
input_data = np.expand_dims(input_data, axis=0)  # 在最前面新增一個批量維度

# 初始化各種影像處理相關的陣列
sRGB_8bpc = np.zeros((crop[2], crop[3], 3), dtype=np.uint8)  # 用來儲存 8 位元 sRGB 彩色影像
linear_sRGB_bloom_img = np.zeros((crop[2]+2*bloom_range+1, crop[3]+2*bloom_range+1, channels), dtype=np.float32)  # 儲存處理後的線性 sRGB 光暈影像
linear_sRGB_img = np.zeros((height, width, 3), dtype=np.float32)  # 儲存完整線性 sRGB 彩色影像
linear_grayscale_img = np.zeros((height, width), dtype=np.float32)  # 儲存線性灰階影像
grayscale_img = np.zeros((height, width), dtype=np.uint8)  # 儲存 8 位元深度的 sRGB 灰階影像
sRGB_to_linear = np.zeros((256), dtype=np.float32)  # 儲存 sRGB 到線性空間的查找表 (8 位元)
linear_to_sRGB = np.zeros((65536), dtype=np.float32)  # 儲存線性空間到 sRGB 的查找表 (16 位元)

# 計時開始
start_time = time.perf_counter()  

# 生成 sRGB 到線性空間的查找表
start_time_section = start_time
sRGB_to_linear = generate_sRGB_to_linear_lookup_table_8bit_vectorized()
print(f"generate_sRGB_to_linear_lookup_table_8bit:{time.perf_counter()-start_time_section:.5f}s")

# 生成線性空間到 sRGB 的查找表
start_time_section = time.perf_counter()
linear_to_sRGB = generate_linear_to_sRGB_lookup_table_16bit_vectorized()
print(f"generate_linear_to_sRGB_lookup_table_16bit:{time.perf_counter()-start_time_section:.5f}s")

if use_bloom_process == 1: # 如果啟用光暈處理
    # 開始進行光暈處理計算
    start_time_section = time.perf_counter()
    center_strength = 1 - bloom_strength  # 計算中心強度
    full_range = 2 * bloom_range + 1  # 計算光暈範圍
    
    # 使用 PyTorch 進行卷積操作
    pytorch_output = pytorch_convolution(input_data, kernel, device)
    #print("pytorch_output", pytorch_output.shape)
    
    # 去掉批量維度
    if pytorch_output.dim() == 4:
        pytorch_output = pytorch_output[0, :, :, :]
    #print("pytorch_output", pytorch_output.shape)
    
    # 調整輸出維度順序，轉為 (height, width, channels)
    linear_sRGB_bloom_img = pytorch_output.permute(1, 2, 0)
    #print("linear_sRGB_bloom_img", linear_sRGB_bloom_img.shape)
    
    # 如果 PyTorch 的輸出是 Tensor，將其轉換為 NumPy 陣列
    if isinstance(linear_sRGB_bloom_img, torch.Tensor):
        linear_sRGB_bloom_img = linear_sRGB_bloom_img.detach().cpu().numpy()
    
    #print("linear_sRGB_bloom_img", linear_sRGB_bloom_img.shape)
    #print("crop_img", crop_img.shape)
    
    # 增加中心影像的影響，混合光暈和原始影像
    linear_sRGB_bloom_img += crop_img * center_strength
    # 計算光暈處理的執行時間，並輸出結果
    print(f"bloom_process:{time.perf_counter()-start_time_section:.5f}s")
    
elif use_bloom_process == 0:
    bloom_range = 0
    linear_sRGB_bloom_img = crop_img

# 將影像從 BGR 格式轉回 RGB 格式
rgb_img = cv2.cvtColor(linear_sRGB_bloom_img, cv2.COLOR_BGR2RGB)

# 將最終結果儲存為 EXR 檔案
if use_crop == 0:
    if use_bloom_process == 1:
        pyexr.write(f"bloom.range{bloom_range}.exr", rgb_img)
    elif use_bloom_process == 0:
        pyexr.write(f"bypass.exr", rgb_img)
elif use_crop == 1:
    if use_bloom_process == 1:
        pyexr.write(f"crop_bloom.range{bloom_range}.exr", rgb_img)
    elif use_bloom_process == 0:
        pyexr.write(f"crop.exr", rgb_img)

# 乘以亮度增益 (brightness_gain) 後裁切到 [0, 65535] 範圍內
# 最終將結果轉換為整數型別 (np.int32)
temp_int32 = (np.clip(linear_sRGB_bloom_img * brightness_gain, 0, 65535)).astype(np.int32)

# 使用線性到 sRGB 的查找表，將 16 位元線性顏色轉換為 8 位元 sRGB 顏色
# 並將範圍限制在 [0, 255]，最後轉換為 np.uint8 型別
sRGB_8bpc = (np.clip(linear_to_sRGB[temp_int32]*255, 0, 255)).astype(np.uint8)

# 清空暫存的整數陣列，釋放記憶體
temp_int32 = None

# 將處理後的 8 位元色深 sRGB 彩色影像儲存為 PNG 檔案
cv2.imwrite(f"crop_sRGB_bloom_+{brightness_EV}EV.range{bloom_range}.png", sRGB_8bpc)
if use_crop == 0:
    if use_bloom_process == 1:
        cv2.imwrite(f"sRGB_bloom_+{brightness_EV}EV.range{bloom_range}.png", sRGB_8bpc)
    elif use_bloom_process == 0:
        cv2.imwrite(f"sRGB_+{brightness_EV}EV.png", sRGB_8bpc)
elif use_crop == 1:
    if use_bloom_process == 1:
        cv2.imwrite(f"crop_sRGB_bloom_+{brightness_EV}EV.range{bloom_range}.png", sRGB_8bpc)
    elif use_bloom_process == 0:
            cv2.imwrite(f"crop_sRGB_+{brightness_EV}EV.png", sRGB_8bpc)


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
