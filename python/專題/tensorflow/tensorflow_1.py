import numpy as np
import cv2
import tensorflow as tf

model = tf.saved_model.load('ssd_mobilenet_v2/')
print(type(model))
print(dir(model))
# 定义目标类
class_names = ['tomato', 'lettuce', 'carrot']

# 加载图片
img = cv2.imread('input.png')
img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

# 图像预处理
img = tf.image.resize(img, (224, 224))
img = np.expand_dims(img, axis=0)
img = img / 127.5 - 1

# 模型预测
predictions = model.predict(img)

# 获取 tomato 的预测框
boxes = []
scores = predictions[0][:, class_names.index('tomato')]
idxs = tf.where(scores > 0.5)
for i in idxs:
  box = predictions[1][0, i] * [img.shape[1], img.shape[0], img.shape[1], img.shape[0]]
  boxes.append(box)

# 绘制预测框到图像上
for box in boxes:
  start_point = (int(box[1]), int(box[0]))
  end_point = (int(box[3]), int(box[2]))
  cv2.rectangle(img, start_point, end_point, (0, 255, 0), 2)

cv2.imshow('Detected Tomatoes', img)
cv2.waitKey(0)
