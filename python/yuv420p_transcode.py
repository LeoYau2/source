#在我的電腦上這比原本的版本在處理1920*1080的圖片時總體快了剛好100倍，我的CPU是i7-9750h，記憶體是32Gb 2400Mhz雙通道

#我剛好以前有寫過色彩空間轉換的程式，一個8bpc的sRGB的彩色圖片果要轉換成黑白圖片，
#需要先把sRGB轉成線性sRGB，再轉換成線性的黑白圖片
#如果要把線性黑白圖片轉成sRGB曲線的黑白圖片需要套用sRGB的OETF

import cv2
import numpy as np
import time
#import gc
#import hdr_parser
import av
import imageio

#Perceptual Quantizer
#c1=c3-c2+1=107/128=0.8359375,c2=2413/128=18.8515625,c3=2392/128=18.6875,m1=1305/8192=0.1593017578125,m2=2523/32=78.84375
#L**(1305/8192) = -((128*V_PQ**(32/2523)-107)/(2392*V_PQ**(32/2523)-2413))
#PQ_L = (-((128*V_PQ**0.0126833135-107)/(2392*V_PQ**0.0126833135-2413)))**6.2773946360
#V_PQ = ((0.8359375+18.8515625*L**0.1593017578125)/(1+18.6875*L**0.1593017578125))**78.84375

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

def PQ_EOTF_vectorized(V_PQ):
    return np.real(np.where(V_PQ == 0, 0, (-((128*V_PQ**0.0126833135-107)/(2392*V_PQ**0.0126833135-2413)))**6.2773946360))

def sqrt_to_PQ_OETF_vectorized(square):
    return ((0.8359375+18.8515625*square**0.3186035156)/(1+18.6875*square**0.3186035156))**78.84375

def generate_sRGB_to_linear_lookup_table_8bit_vectorized(): #製作一個sRGB轉linear查找表，輸入範圍是0~255,輸出範圍是0~1，以後直接讀取，這樣速度會快很多
    sRGB_values = np.arange(0, 256) / 255.0 #產生0-255的自增數組,並歸一化到0-1作為所有可能的輸入值
    sRGB_to_linear_vectorized = sRGB_EOTF_vectorized(sRGB_values)
    return sRGB_to_linear_vectorized

def generate_linear_to_sRGB_lookup_table_16bit_vectorized(): #製作一個linear轉sRGB查找表，輸入範圍是0~65535,輸出範圍是0~1，這個查找表可以保留16EV動態範圍
    linear_values = np.arange(0, 65536) / 65535.0 #跟上一個函數差不多
    linear_to_sRGB_vectorized = sRGB_OETF_vectorized(linear_values)
    return linear_to_sRGB_vectorized

def generate_PQ_to_linear_lookup_table_16bit_vectorized():
    PQ_values = np.arange(0, 65536) / 65535.0
    PQ_to_linear_vectorized = PQ_EOTF_vectorized(PQ_values)
    return PQ_to_linear_vectorized

def generate_sqrt_to_PQ_lookup_table_16bit_vectorized():
    sqrt_values = np.arange(0, 65536) / 65535.0
    sqrt_to_PQ_vectorized = sqrt_to_PQ_OETF_vectorized(sqrt_values)
    return sqrt_to_PQ_vectorized

#print(cv2.getBuildInformation())
#out = cv2.VideoWriter('output.mkv', cv2.VideoWriter_fourcc(*'HEVC'), fps, (width, height), isColor=True, apiPreference=cv2.CAP_BGRA)

path = 'av1_540p.ss120.mkv'
container = av.open(path)
output_container = av.open('output_10bit.mkv', 'w')
input_stream = container.streams.video[0]
width, height = input_stream.width, input_stream.height
# 设置输出视频流的参数
output_stream = output_container.add_stream('libaom-av1')
output_stream.width = width
output_stream.height = height
output_stream.pix_fmt = 'yuv420p10le'  # 10-bit pixel format

# 设置 x264 的 qp 值
#output_stream.options = {'preset':'1','crf': '16'}
output_stream.options = {'cpu-used':'8','crf': '16'}
#start_frame_index = 1000
rgb_frames = []
for frame in container.decode(video=0):
    #yuv_frame = frame.to_ndarray(format='yuv420p')
    #rgb_frame = imageio.plugins.ffmpeg._convert_colorspace(yuv_frame, 'yuv420p', 'rgb')
    #rgb_frame = cv2.cvtColor(yuv_frame, cv2.COLOR_YUV2RGB_YV12)
    #yuv_frame = frame.to_ndarray(format='yuv420p10le')
    rgb_frame = frame.to_ndarray(format='yuv420p')
    #yuv_frame = 
    converted_frame = av.VideoFrame.from_ndarray(rgb_frame, format='yuv420p')
    encoded_frame = output_stream.encode(converted_frame)
    output_container.mux(encoded_frame)
    print(rgb_frame.dtype)

    # 將 10 位元的 YUV 轉換為 RGB
    #rgb_frame = cv2.cvtColor(yuv_frame.astype(np.uint16), cv2.COLOR_YUV2RGB_YV12)
    #print(frame[500][500])
    #print(rgb_frame.shape)
    #print(rgb_frame[500][500])
    #print('yuv',yuv_frame[500][500])
    print('yuv',rgb_frame.shape)
    
    
output_container.close()
container.close()
cv2.imshow('yuv_frame',rgb_frame)

# 開啟影片輸入串流
#cap = cv2.VideoCapture('input.mkv')
cap = cv2.VideoCapture('input.mkv', cv2.CAP_FFMPEG)
cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc('I', 'Y', 'U', 'V'))


# 取得影片屬性
width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
fps = cap.get(cv2.CAP_PROP_FPS) 
print("width:",width,",height:",height,",fps:",fps)
# 定義輸出影片設定
fourcc = cv2.VideoWriter_fourcc(*'XVID')  
out = cv2.VideoWriter('output.mkv', fourcc, fps, (width, height))

if not out.isOpened():
    print("输出文件打开失败")

print(PQ_EOTF_vectorized(144/255),PQ_EOTF_vectorized(0.1),PQ_EOTF_vectorized(0.57),PQ_EOTF_vectorized(0.9),PQ_EOTF_vectorized(1))
print('sq',sqrt_to_PQ_OETF_vectorized(0),sqrt_to_PQ_OETF_vectorized(1))
gain = 6
input_img = cv2.imread('bt2020_pq_2.png',cv2.IMREAD_UNCHANGED) #讀取一張8bpc的sRGB圖片
print(np.max(input_img))
#cv2.imshow('input_img',input_img)
height, width, channels = input_img.shape #查看圖片寬高
print("width:",width,",height:",height,",channels:",channels)
linear_sRGB_img = np.zeros((height,width,3), dtype=np.float32) #用來儲存線性sRGB彩色圖片，範圍0~1
linear_bt2020_img = np.zeros((height,width,3), dtype=np.float32)
pq_float_img = np.zeros((height,width,3), dtype=np.float32)
linear_grayscale_img = np.zeros((height,width), dtype=np.float32) #用來儲存線性灰階圖片，範圍0~1
grayscale_img = np.zeros((height,width), dtype=np.uint8) #用來儲存8位元深度sRGB曲線灰階圖片，範圍0~255
sRGB_to_linear = np.zeros((256), dtype=np.float32) #用來儲存8bit查找表，需要256*4bytes=1kb記憶體
linear_to_sRGB = np.zeros((65536), dtype=np.float32) #用來儲存16bit查找表，需要65535*4bytes=256kb記憶體
PQ_to_linear = np.zeros((65536), dtype=np.float32)
sqrt_to_PQ = np.zeros((65536), dtype=np.float32)

start_time = time.perf_counter() #開始計時

start_time_section = start_time
PQ_to_linear = generate_PQ_to_linear_lookup_table_16bit_vectorized()
print(f"generate_PQ_to_linear_lookup_table_16bit:{time.perf_counter()-start_time_section:.5f}s")

start_time_section = start_time
sqrt_to_PQ = generate_sqrt_to_PQ_lookup_table_16bit_vectorized()
print(f"generate_sqrt_to_PQ_lookup_table_16bit:{time.perf_counter()-start_time_section:.5f}s")

start_time_section = time.perf_counter()
temp_int32 = np.zeros((height,width), dtype=np.int32)
temp_int32 = (input_img * 257).astype(np.int32)
linear_bt2020_img = PQ_to_linear[temp_int32]
print(np.max(linear_bt2020_img))

print(f"pq_to_linear:{time.perf_counter()-start_time_section:.5f}s")
#cv2.imshow('linear_bt2020_img',linear_bt2020_img)
print(np.max(linear_bt2020_img))
linear_bt2020_img = linear_bt2020_img * gain
print(np.max(linear_bt2020_img))
linear_bt2020_img = np.sqrt(linear_bt2020_img)
temp_int32 = (linear_bt2020_img * 65535).astype(np.int32)
pq_float_img = sqrt_to_PQ[temp_int32]
'''
# 逐幀處理輸入影片  
while(cap.isOpened()):
    ret, frame = cap.read()
    print(ret)
    if ret == True:
    
        # 影像處理程式碼
        # ......
        
        # 寫入影格到輸出影片 
        out.write(frame)

    else:
        break
        
# 釋放資源
cap.release()
out.release()'''

'''
if not out.isOpened():
    print("輸出文件打開失敗，嘗試使用不同的編碼器或檢查路徑設定。")
else:
    print("輸出文件成功打開。")

# 確保在使用完畢後釋放資源
#cap.release()
#out.release()
#cv2.destroyAllWindows()

while True:
    ret, frame = cap.read()
    #ret, frame = cap.read(cv2.IMREAD_UNCHANGED)
    print(frame[500][500])
    print(frame.shape)
    #frame = frame[:,:,0]*0+frame[:,:,1]*0+frame[:,:,2]*1
    print("Data type of frame:", frame.dtype)
    cv2.imshow('frame',frame)
    break
    if not ret:
        break
        
    # 不做處理,直接寫出輸入的frame    
    out.write(frame)
    
cap.release()
out.release()
'''
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
print(f"total time:{time.perf_counter()-start_time:.3f}s")'''

#cv2.imshow('input_img',input_img) #顯示輸入圖片
#cv2.imshow('linear_sRGB_img',linear_sRGB_img) #顯示線性sRGB圖片
#cv2.imshow('linear_grayscale_img',linear_grayscale_img) #顯示線性黑白圖片
#cv2.imshow('grayscale_img_numpy',grayscale_img) #顯示8位元深度sRGB曲線灰階圖片
#cv2.imshow('sqrt_img',linear_bt2020_img)
#cv2.imshow('pq_float_img',pq_float_img)
