import rawpy
import numpy as np
from PIL import Image
from PIL.PngImagePlugin import PngInfo
#import colour
import imageio
import cv2


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
dng_path = "C:/Users/Leo/Pictures/photo/Dxo/DSC_1166-已增強-雜訊減少-dng_DxO_DeepPRIME.dng"  # 替換為你的 DNG 檔案路徑
dng_path = "C:/Users/Leo/Pictures/photo/nef/Dxo/DSC_2455-NEF_DxO_DeepPRIME.dng"
raw = rawpy.imread(dng_path)

# Step 2: 內插（Demosaic）為線性 RGB
# 使用 AHD 內插算法，輸出 16 位元整數，然後轉為浮點
rgb = raw.postprocess(
    #demosaic_algorithm=None,  # 禁用內插，Lightroom 已 demosaic
    output_color=rawpy.ColorSpace.ProPhoto,  # 保留原始色域
    output_bps=16,                      # 16 位元輸出
    no_auto_bright=True,                # 禁用自動亮度
    #use_camera_wb=True,
    exp_shift=-10.0,
    #user_wb=[0.5, 0.5, 1.0, 0.5],
    use_auto_wb=False,
    gamma=(1, 1),                       # 保持線性
    fbdd_noise_reduction=rawpy.FBDDNoiseReductionMode.Off,  # 禁用降噪
)

print("raw Shape:", rgb.shape)  # 應為 (4016, 6016, 3)
print("raw Dtype:", rgb.dtype)
print("raw Min:", rgb.min(), "Max:", rgb.max())

M = np.array([
    [1.4031, -0.2231, -0.1016],
    [-0.5266, 1.3582, 0.0248],
    [0.0290, -0.0786, 0.9642]
])

ProPhoto_RGB_to_XYZ = np.array([
    [0.7977, 0.1352, 0.0313],
    [0.2880, 0.7119, 0.0001],
    [0.0000, 0.0000, 0.8249]
])

XYZ_to_BT2020 = np.array([
    [1.7167, -0.3557, -0.2534],
    [-0.6667, 1.6165, 0.0158],
    [0.0176, -0.0428, 0.9421]
])

M_bradford = np.array([
    [ 0.8951,  0.2664, -0.1614],
    [-0.7502,  1.7135,  0.0367],
    [ 0.0389, -0.0685,  1.0296]
])
# LMS 到 XYZ 的逆矩陣
M_bradford_inv = np.linalg.inv(M_bradford)
# 白點
XYZ_D50 = np.array([0.9642, 1.0000, 0.8252])
XYZ_D65 = np.array([0.9505, 1.0000, 1.0890])

# 計算適配矩陣
LMS_D50 = np.dot(M_bradford, XYZ_D50)
LMS_D65 = np.dot(M_bradford, XYZ_D65)
scale = np.diag(LMS_D65 / LMS_D50)
M_adapt = np.dot(M_bradford_inv, np.dot(scale, M_bradford))

# Step 3: 將 16 位元整數轉為浮點（範圍 0-1）
linear_rgb = rgb.astype(np.float32) / 65535.0
linear_rgb *= raw_max_brightness/10000
print("Min:", linear_rgb.min(), "Max:", linear_rgb.max())

linear_rgb_bt2020 = np.dot(linear_rgb, ProPhoto_RGB_to_XYZ)  # 到 XYZ (D50)
linear_rgb_bt2020 = np.dot(linear_rgb_bt2020, M_adapt)       # D50 到 D65
linear_rgb_bt2020 = np.dot(linear_rgb_bt2020, XYZ_to_BT2020) # 到 BT.2020
print("Min:", linear_rgb_bt2020.min(), "Max:", linear_rgb_bt2020.max())
linear_rgb_bt2020 = np.clip(linear_rgb_bt2020, 0, 1)
print("Min:", linear_rgb_bt2020.min(), "Max:", linear_rgb_bt2020.max())

linear_rgb = np.sqrt(linear_rgb_bt2020)
print("Min:", linear_rgb.min(), "Max:", linear_rgb.max())
temp_int32 = (linear_rgb * 65535).astype(np.int32)
pq_float = sqrt_to_PQ[temp_int32]
pq_16bpc = (pq_float*65535).astype(np.uint16)
print("pq Min:", pq_16bpc.min(), "pq Max:", pq_16bpc.max())
print("Min:", PQ_EOTF_vectorized(pq_16bpc.min()/65535)*10000,"nits", "pq Max:", PQ_EOTF_vectorized(pq_16bpc.max()/65535)*10000,"nits")

#cv2.imwrite("C:/Users/Leo/Pictures/photo/output.png", rgb)
bgr = pq_16bpc[:, :, [2, 1, 0]]
cv2.imwrite("C:/Users/Leo/Pictures/photo/output.png", bgr)
