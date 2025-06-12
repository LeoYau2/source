import rawpy
import numpy as np
from PIL import Image
from PIL.PngImagePlugin import PngInfo
import colour
import imageio

raw_max_brightness = 10000

def PQ_EOTF_vectorized(V_PQ):
    return np.real(np.where(V_PQ == 0, 0, (-((128*V_PQ**0.0126833135-107)/(2392*V_PQ**0.0126833135-2413)))**6.2773946360))

def PQ_OETF_vectorized(L):
    return ((0.8359375+18.8515625*L**0.1593017578125)/(1+18.6875*L**0.1593017578125))**78.84375

def sqrt_to_PQ_OETF_vectorized(square):
    return ((0.8359375+18.8515625*square**0.3186035156)/(1+18.6875*square**0.3186035156))**78.84375

def generate_PQ_to_linear_lookup_table_16bit_vectorized():
    PQ_values = np.arange(0, 65536) / 65535.0
    PQ_to_linear_vectorized = PQ_EOTF_vectorized(PQ_values)
    return PQ_to_linear_vectorized

def generate_sqrt_to_PQ_lookup_table_16bit_vectorized():
    sqrt_values = np.arange(0, 65536) / 65535.0
    sqrt_to_PQ_vectorized = sqrt_to_PQ_OETF_vectorized(sqrt_values)
    return sqrt_to_PQ_vectorized

#PQ_to_linear = generate_PQ_to_linear_lookup_table_16bit_vectorized()

sqrt_to_PQ = generate_sqrt_to_PQ_lookup_table_16bit_vectorized()

#PQ_to_sqrt = generate_PQ_to_sqrt_lookup_table_16bit_vectorized()

# Step 1: 讀取 DNG 檔案
dng_path = "C:/Users/Leo/Pictures/photo/DSC_1166.dng"  # 替換為你的 DNG 檔案路徑
raw = rawpy.imread(dng_path)
img = imageio.imread("C:/Users/Leo/Pictures/photo/DSC_1166.dng")

# Step 2: 內插（Demosaic）為線性 RGB
# 使用 AHD 內插算法，輸出 16 位元整數，然後轉為浮點
'''rgb = raw.postprocess(
    demosaic_algorithm=rawpy.DemosaicAlgorithm.PPG,  # 高質量內插
    output_color=rawpy.ColorSpace.raw,          # 保留原始色域（不應用色彩空間）
    output_bps=16,                              # 16 位元輸出
    no_auto_bright=True,                        # 禁用自動亮度調整
    use_camera_wb=False,                        # 禁用相機白平衡
    use_auto_wb=False,                          # 禁用自動白平衡
    gamma=(1, 1),                               # 禁用伽瑪校正，保持線性
    user_wb=[1, 1, 1, 1],                      # 手動白平衡（單位矩陣）
    fbdd_noise_reduction=rawpy.FBDDNoiseReductionMode.Off  # 禁用降噪
)'''

# Step 3: 將 16 位元整數轉為浮點（範圍 0-1）
#linear_rgb = rgb.astype(np.float32) / 65535.0


# Step 4: 驗證數據
#print("Shape:", linear_rgb.shape)  # 應為 (4016, 6016, 3)
#print("Dtype:", linear_rgb.dtype)  # 應為 float32
#print("Min:", linear_rgb.min(), "Max:", linear_rgb.max())  # 範圍應為 0-1

metadata = PngInfo()

image = Image.fromarray(img, mode='RGB')

#cv2.imwrite("C:/Users/Leo/Pictures/photo/output.png", rgb)
image.save("C:/Users/Leo/Pictures/photo/output.png", bits=16, pnginfo=metadata)
