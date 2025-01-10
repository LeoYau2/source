#pip install object_detection

import tensorflow as tf
import cv2
import numpy as np

# 載入模型
model = tf.saved_model.load("ssd_mobilenet_v2")

# 載入圖片
img = cv2.imread("apple_1.600px.png")
input_tensor = tf.convert_to_tensor(img, dtype=tf.float32)[tf.newaxis, ...]
input_tensor = tf.cast(input_tensor, dtype=tf.uint8)  # 將數據類型轉換為 tf.uint8

# 執行推論
detections = model(input_tensor)

# 解析輸出
boxes = detections['detection_boxes'].numpy()
scores = detections['detection_scores'].numpy()
print(boxes.shape)
print(scores.shape)
# 用二維陣列存放方框座標
result_array = np.zeros((100, 4))

# 輸出方框
num_objects = 0  # 初始化物體數量

for i in range(boxes.shape[0]):
    if scores[i].any() > 0.5:
        box = boxes[num_objects,i]
        ymin, xmin, ymax, xmax = box[0], box[1], box[2], box[3]
        x, y, w, h = int(xmin * img.shape[1]), int(ymin * img.shape[0]), int((xmax - xmin) * img.shape[1]), int((ymax - ymin) * img.shape[0])
        result_array[i] = [x, y, x + w, y + h]
        cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)
        num_objects += 1  # 每找到一個物體，物體數量增加

# 顯示物體數量和每個方框的座標
print("boxes")
print(boxes)
print("scores")
print(scores)
print("Number of objects:", num_objects)
print("Bounding box coordinates:")
for i in range(num_objects):
    print(result_array[i])

# 顯示結果
cv2.imshow("Image", img)
cv2.waitKey(0)
cv2.destroyAllWindows()

# 輸出方框座標
#print(result_array)

'''
import tensorflow as tf
from object_detection.utils import label_map_util
from object_detection.utils import visualization_utils as viz_utils

#model = tf.saved_model.load('ssd_mobilenet_v2/')

# 1. 加载模型  

model_name = 'ssd_mobilenet_v2'
detection_model = tf.saved_model.load(f'./{model_name}/saved_model')
print(type(model))
print(dir(model))

# 2. 加载标签映射  

category_index = label_map_util.create_category_index_from_labelmap(PATH_TO_LABELS, use_display_name=True)

# 3. 读取图片 

image_np = load_image_into_numpy_array(PATH_TO_IMAGE)  

# 4. 运行推理

input_tensor = tf.convert_to_tensor(np.expand_dims(image_np, 0), dtype=tf.float32)
detections = detection_model(input_tensor)

# 5. 后处理结果
num_detections = int(detections.pop('num_detections'))  
detections = {key: value[0, :num_detections].numpy()  
              for key, value in detections.items()}
detections['num_detections'] = num_detections

# 6. 可视化
image_np_with_detections = image_np.copy()
viz_utils.visualize_boxes_and_labels_on_image_array(
          image_np_with_detections,
          detections['detection_boxes'],
          detections['detection_classes'],
          detections['detection_scores'],
          category_index,
          use_normalized_coordinates=True,
          max_boxes_to_draw=200,
          min_score_thresh=.30,
          agnostic_mode=False)
          
plt.imshow(image_np_with_detections)
'''
