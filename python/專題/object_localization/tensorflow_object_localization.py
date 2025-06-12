#pip install object_detection

import tensorflow as tf
import cv2
import numpy as np

def detect(img, score_threshold):
    input_tensor = tf.convert_to_tensor(img, dtype=tf.float32)[tf.newaxis, ...]
    input_tensor = tf.cast(input_tensor, dtype=tf.uint8)  # 將數據類型轉換為 tf.uint8
    
    detections = model(input_tensor) # 執行推論
    # 解析輸出
    boxes = detections['detection_boxes'].numpy() #儲存方框座標
    scores = detections['detection_scores'].numpy() #儲存score
    #print(boxes)
    #print(scores)
    num_objects = 0  # 初始化物體數量
    # 輸出方框
    for i in range(boxes.shape[1]):
        #if np.any(scores[0,i] > scores[0,0]*score_ratio_threshold) and np.any(scores[0,i] > score_threshold): #score大於最高score*score_ratio_threshold且大於score_threshold，備用選項
        if np.any(scores[0,i] > score_threshold): #score大於score_threshold
            box = boxes[0,i]
            ymin, xmin, ymax, xmax = box[0], box[1], box[2], box[3]
            x, y, w, h = int(xmin * img.shape[1]), int(ymin * img.shape[0]), int((xmax - xmin) * img.shape[1]), int((ymax - ymin) * img.shape[0])
            result_array[i] = [x, y, x + w, y + h]
            cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2) #這一行用來畫出方框，會修改圖片，之後可以去掉
            num_objects += 1  # 每找到一個物體，物體數量增加
    return num_objects #返回方框數量

model = tf.saved_model.load("ssd_mobilenet_v2") # 載入模型
img = cv2.imread("apple_1.600px.png")
#img = cv2.imread("orange_2.jpg")
#img = cv2.imread("orange_3.600px.png")
#img = cv2.imread("tomato_2.jpg")
#img = cv2.imread("food_1.jpg")
#img = cv2.imread("food_4.jpg") # 載入圖片
result_array = np.zeros((100, 4)) #存放方框座標
num_objects = detect(img, 0.3) #接收方框數量，輸入圖片和最低score倍率和最低score，最低score越高偵測出數量越少

# 顯示物體數量和每個方框的座標
print("Number of objects:", num_objects)
print("Bounding box coordinates:")
for i in range(num_objects):
    print(result_array[i])

# 顯示結果
cv2.imshow("Image", img)
cv2.waitKey(0)
cv2.destroyAllWindows()
