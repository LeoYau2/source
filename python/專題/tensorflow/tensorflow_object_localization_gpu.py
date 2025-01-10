#pip install object_detection
#需要安裝cudnn

import tensorflow as tf
import cv2
import numpy as np
import os

print("Num GPUs Available: ", len(tf.config.list_physical_devices('GPU')))

with tf.device('/GPU:0'):
    # 创建一个简单的张量运算
    a = tf.constant([[1.0, 2.0], [3.0, 4.0]])
    b = tf.constant([[5.0, 6.0], [7.0, 8.0]])
    c = tf.matmul(a, b)
    print("Result from GPU: ", c)

physical_devices = tf.config.list_physical_devices('GPU')
print('w')
for device in physical_devices:
    print(device)
if len(physical_devices) > 0:
    tf.config.experimental.set_memory_growth(physical_devices[0],True)

os.environ["CUDA_VISIBLE_DEVICES"]="0"
print(tf.__version__)
print(tf.test.is_gpu_available())
print(tf.test.gpu_device_name())

#檢查GPU
physical_devices = tf.config.list_physical_devices('GPU')
if physical_devices:
    tf.config.experimental.set_memory_growth(physical_devices[0], True)
    print("GPU")
else:
    print("no GPU")

input()

def detect(img, score_threshold, too_close_threshold):
    input_tensor = tf.convert_to_tensor(img, dtype=tf.float32)[tf.newaxis, ...]
    input_tensor = tf.cast(input_tensor, dtype=tf.uint8)  # 將數據類型轉換為 tf.uint8
    
    detections = model(input_tensor) # 執行推論
    # 解析輸出
    boxes = detections['detection_boxes'].numpy() #儲存方框座標
    scores = detections['detection_scores'].numpy() #儲存score
    num_objects = 0  # 初始化物體數量
    # 輸出方框
    for i in range(boxes.shape[1]):
        #if np.any(scores[0,i] > scores[0,0]*score_ratio_threshold) and np.any(scores[0,i] > score_threshold): #score大於最高score*score_ratio_threshold且大於score_threshold，備用方案
        if np.any(scores[0,i] > score_threshold): #score大於score_threshold
            box = boxes[0,i]
            too_close = 0
            ymin, xmin, ymax, xmax = box[0], box[1], box[2], box[3]
            x, y, w, h = int(xmin * img.shape[1]), int(ymin * img.shape[0]), int((xmax - xmin) * img.shape[1]), int((ymax - ymin) * img.shape[0]) #計算方框的座標，單位是像素
            for j in range(num_objects+1):
                if (abs(ymin-result_array[j,1]/img.shape[0])<too_close_threshold) and (abs(xmin-result_array[j,0]/img.shape[1])<too_close_threshold) and (abs(ymax-result_array[j,3]/img.shape[0])<too_close_threshold) and (abs(xmax-result_array[j,2]/img.shape[1])<too_close_threshold): #去除太相似的方框
                    too_close = 1
                if (ymin < 0.1) and (xmin < 0.1) and (ymax > 0.9) and (xmax > 0.9): #去除把整個畫面圍住的方框
                    too_close = 1
            if (too_close == 0):
                result_array[num_objects] = [x, y, x + w, y + h]
                cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2) #這一行用來畫出方框，會修改圖片，之後可以去掉
                num_objects += 1  # 每找到一個物體，物體數量增加
    return num_objects #返回方框數量

model = tf.saved_model.load("ssd_mobilenet_v2") # 載入模型
img = cv2.imread("food_1.jpg") # 載入圖片
result_array = np.zeros((100, 4)) #存放方框座標
num_objects = detect(img, 0.3, 0.05) #接收方框數量，輸入圖片 和 最低score倍率 和 最低score 和 判斷不同方框太接近的最小差異 ，值越小越容易判斷出太相似的方框，最低score越高偵測出數量越少

# 顯示物體數量和每個方框的座標
print("Number of objects:", num_objects)
print("Bounding box coordinates:")
for i in range(num_objects):
    print(result_array[i])

# 顯示結果
cv2.imshow("Image", img)
cv2.waitKey(0)
cv2.destroyAllWindows()
