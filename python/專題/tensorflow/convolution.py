import numpy as np
import tensorflow as tf
import time

# 检查 TensorFlow 是否能够识别到 GPU
print("Num GPUs Available: ", len(tf.config.experimental.list_physical_devices('GPU')))
#input()

# 创建随机输入数据和卷积核
input_data = np.random.rand(1, 1920, 1080, 3)  # 1张128x128的3通道图片
kernel = np.random.rand(255, 255, 3, 1)         # 卷积核：3x3大小，3通道，32个filter

# NumPy实现卷积
def numpy_convolution(input_data, kernel):
    batch, height, width, channels = input_data.shape
    kh, kw, kc, num_filters = kernel.shape
    assert channels == kc, "输入通道数和卷积核通道数必须一致"
    
    output_height = height - kh + 1
    output_width = width - kw + 1
    output = np.zeros((batch, output_height, output_width, num_filters))
    
    for b in range(batch):
        for f in range(num_filters):
            for i in range(output_height):
                for j in range(output_width):
                    for c in range(channels):
                        output[b, i, j, f] += np.sum(
                            input_data[b, i:i+kh, j:j+kw, c] * kernel[:, :, c, f]
                        )
    return output

# TensorFlow实现卷积
def tensorflow_convolution(input_data, kernel):
    input_tensor = tf.constant(input_data, dtype=tf.float32)
    kernel_tensor = tf.constant(kernel, dtype=tf.float32)
    output = tf.nn.conv2d(input_tensor, kernel_tensor, strides=[1, 1, 1, 1], padding="SAME")
    return output

def tensorflow_gpu_convolution(input_data, kernel):
    with tf.device('/GPU:0'):
        input_tensor = tf.constant(input_data, dtype=tf.float32)
        kernel_tensor = tf.constant(kernel, dtype=tf.float32)
        output = tf.nn.conv2d(input_tensor, kernel_tensor, strides=[1, 1, 1, 1], padding="SAME")
        return output


# 测试NumPy速度
#start_time = time.time()
#numpy_output = numpy_convolution(input_data, kernel)
#numpy_time = time.time() - start_time

# 测试TensorFlow速度
start_time = time.time()
tensorflow_output = tensorflow_convolution(input_data, kernel)
tf_time = time.time() - start_time

# 输出结果
#print(f"NumPy卷积耗时: {numpy_time:.5f} 秒")
print(f"TensorFlow: {tf_time:.5f} 秒")
