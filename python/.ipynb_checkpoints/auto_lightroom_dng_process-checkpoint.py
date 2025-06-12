import rawpy
import numpy as np
from PIL import Image
from PIL.PngImagePlugin import PngInfo
#import colour
import imageio
import cv2
import os
from pathlib import Path
import shutil
import subprocess
#import time

# 定義輸入、輸出和已完成資料夾
input_folder = "C:/Users/Leo/Pictures/photo/nef/DxO"
png_output_folder = "C:/Users/Leo/Pictures/photo/nef/DxO/PNG_output"
done_folder = "C:/Users/Leo/Pictures/photo/nef/DxO/DNG_done"
avif_output_folder = "C:/Users/Leo/Pictures/photo/nef/DxO/avif.2"

# 確保輸出和已完成資料夾存在
os.makedirs(input_folder, exist_ok=True)
os.makedirs(png_output_folder, exist_ok=True)
os.makedirs(done_folder, exist_ok=True)
os.makedirs(avif_output_folder, exist_ok=True)


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

# 重試移動檔案函數
def move_file_with_retry(src, dst, retries=5, delay=0.5):
    for attempt in range(retries):
        try:
            shutil.move(src, dst)
            return True
        except PermissionError as e:
            print(f"移動 {src} 失敗，嘗試 {attempt + 1}/{retries}: {e}")
            time.sleep(delay)
    print(f"無法移動 {src}，已放棄")
    return False

#PQ_to_linear = generate_PQ_to_linear_lookup_table_16bit_vectorized()

sqrt_to_PQ = generate_sqrt_to_PQ_lookup_table_16bit_vectorized()

#PQ_to_sqrt = generate_PQ_to_sqrt_lookup_table_16bit_vectorized()

# 定義轉換矩陣
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
    [0.8951, 0.2664, -0.1614],
    [-0.7502, 1.7135, 0.0367],
    [0.0389, -0.0685, 1.0296]
])
M_bradford_inv = np.linalg.inv(M_bradford)

# 白點
XYZ_D50 = np.array([0.9642, 1.0000, 0.8252])
XYZ_D65 = np.array([0.9505, 1.0000, 1.0890])

# 計算適配矩陣
LMS_D50 = np.dot(M_bradford, XYZ_D50)
LMS_D65 = np.dot(M_bradford, XYZ_D65)
scale = np.diag(LMS_D65 / LMS_D50)
M_adapt = np.dot(M_bradford_inv, np.dot(scale, M_bradford))





# 處理資料夾中的所有 DNG 檔案
for dng_file in Path(input_folder).glob("*.dng"):
    print(f"正在處理: {dng_file.name}")
    
    # Step 1: 讀取 DNG 檔案
    try:
        with rawpy.imread(str(dng_file)) as raw:  # 使用 with 語句
            rgb = raw.postprocess(
                output_color=rawpy.ColorSpace.ProPhoto,
                output_bps=16,
                no_auto_bright=True,
                use_auto_wb=False,
                user_wb=[0.4, 0.35, 0.5, 0.35],
                gamma=(1, 1),
                fbdd_noise_reduction=rawpy.FBDDNoiseReductionMode.Off,
                exp_shift=-10.0
            )
    except Exception as e:
        print(f"無法讀取 {dng_file.name}: {e}")
        continue
    
    print(f"raw Shape: {rgb.shape}")
    print(f"raw Dtype: {rgb.dtype}")
    print(f"raw Min: {rgb.min()}, Max: {rgb.max()}")

    # Step 3: 將 16 位元整數轉為浮點（範圍 0-1）
    raw_max_brightness = 10000  # 假設值，請根據實際需求調整
    linear_rgb = rgb.astype(np.float32) / 65535.0
    linear_rgb *= raw_max_brightness / 10000
    print(f"Min: {linear_rgb.min()}, Max: {linear_rgb.max()}")

    # Step 4: 色域轉換
    linear_rgb_bt2020 = np.dot(linear_rgb, ProPhoto_RGB_to_XYZ)  # 到 XYZ (D50)
    linear_rgb_bt2020 = np.dot(linear_rgb_bt2020, M_adapt)       # D50 到 D65
    linear_rgb_bt2020 = np.dot(linear_rgb_bt2020, XYZ_to_BT2020) # 到 BT.2020
    linear_rgb_bt2020 = np.clip(linear_rgb_bt2020, 0, 1)

    # Step 5: PQ 轉換
    linear_rgb = np.sqrt(linear_rgb_bt2020)
    print(f"Min: {linear_rgb.min()}, Max: {linear_rgb.max()}")
    temp_int32 = (linear_rgb * 65535).astype(np.int32)
    pq_float = sqrt_to_PQ[temp_int32]
    pq_16bpc = (pq_float * 65535).astype(np.uint16)
    print(f"pq Min: {pq_16bpc.min()}, pq Max: {pq_16bpc.max()}")
    print(f"Min: {PQ_EOTF_vectorized(pq_16bpc.min()/65535)*10000} nits, "
          f"pq Max: {PQ_EOTF_vectorized(pq_16bpc.max()/65535)*10000} nits")

    # Step 6: 儲存 PNG
    bgr = pq_16bpc[:, :, [2, 1, 0]]
    png_output_path = os.path.join(png_output_folder, f"{dng_file.stem}.png")
    cv2.imwrite(png_output_path, bgr)
    print(f"已儲存 PNG: {png_output_path}")

    # Step 7: 將 PNG 轉換為 AVIF
    avif_output_path = os.path.join(avif_output_folder, f"{dng_file.stem}.avif")
    avifenc_cmd = [
        "avifenc",
        "--cicp", "9/16/9",  # BT.2020, PQ, non-linear
        "-y", "420",         # YUV 4:2:0
        "-r", "full",        # Full range YUV
        "--depth", "10",     # 10-bit depth
        "--speed", "6",      # Encoding speed
        "-j", "all",         # Use all CPU cores
        "--max", "4",        # Max quantizer
        "--min", "4",        # Min quantizer
        png_output_path,     # 修正：使用具體 PNG 檔案路徑
        avif_output_path     # 修正：使用具體 AVIF 檔案路徑
    ]
    try:
        result = subprocess.run(avifenc_cmd, capture_output=True, text=True, check=True)
        print(f"已生成 AVIF: {avif_output_path}")
        # 刪除 PNG 檔案
        os.remove(png_output_path)
        print(f"已刪除 PNG: {png_output_path}")
    except subprocess.CalledProcessError as e:
        print(f"轉換 {dng_file.name} 到 AVIF 失敗: {e.stderr}")
        continue

    # Step 8: 移動 DNG 檔案到已完成資料夾
    done_path = os.path.join(done_folder, dng_file.name)
    if move_file_with_retry(str(dng_file), done_path):
        print(f"已移動 DNG 到: {done_path}")
    else:
        print(f"跳過移動 {dng_file.name}")

print("所有 DNG 檔案處理完成！")