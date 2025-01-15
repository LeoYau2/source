# source

github網址 : https://github.com/LeoYau2?tab=repositories

以下標註部分檔案

反向傳遞神經網路:<br>
source/main/python/機器學習數學/back-propagation_network_quadrant.2.py 用反向傳播神經網路學習分辨象限<br>

物件分割:<br>
source/main/python/專題<br>

影像處理:<br>
source/main/python/pytorch/bloom_pytorch.py 對exr做bloom處理，輸出exr和調整過亮度的srgb影像，維持exr總亮度不變<br>
source/main/python/color_space_6.py 多種色彩空間的座標轉換<br>
source/main/python/hdr_video_gain.py 調整PQ hdr影片的亮度<br>
source/main/C++/yuv_hdr_brightness_gain.cpp 調整PQ hdr raw影片的亮度<br>
source/main/python/專題/sRGB轉grayscale_向量化計算 sRGB彩色轉灰階，使用sRGB的EOTF計算，維持感知亮度不變<br>

向量化計算:<br>
source/main/C++/yuv_hdr_brightness_gain/Vectorization.cpp 測試AVX計算速度<br>

多線程:<br>
source/main/C++/yuv_hdr_brightness_gain/Producer-Consumer.cpp 生產者-消費者模型<br>
source/main/powershell/avif/autojpg2resizedavifCopyTimeStampConcurrency.ps1 自動批次png轉avif，腳本會尋找接近目標平均檔案大小的壓縮品質，可設定目標平均檔案大小、檔案大小上下限、統一尺寸、並行數量、壓縮速度<br>
