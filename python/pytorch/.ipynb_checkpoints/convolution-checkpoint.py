import numpy as np
import torch
import time

# 检查是否可以使用 GPU
device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
print("Using device:", device)

# 仅执行一次来检查 CUDA 是否可用
torch.zeros(1, device="cuda")  

# 创建随机输入数据和卷积核
input_data = np.random.rand(1, 3, 1920, 1080)  # PyTorch 的输入格式是 [batch_size, channels, height, width]
kernel = np.random.rand(3, 3, 511, 511)       # 卷积核的格式是 [out_channels, in_channels, kernel_height, kernel_width]

# PyTorch 实现卷积
def pytorch_convolution(input_data, kernel):
    # 将 NumPy 转换为 PyTorch tensor，并移动到指定设备 (CPU/GPU)
    input_tensor = torch.from_numpy(input_data).float().to(device)  # 使用 torch.from_numpy()
    kernel_tensor = torch.from_numpy(kernel).float().to(device)  # 使用 torch.from_numpy()
    
    # 获取卷积核的维度信息
    out_channels, in_channels, kernel_height, kernel_width = kernel.shape
    
    # 计算 padding 大小，确保输出与输入大小相同
    padding_height = (kernel_height - 1) // 2
    padding_width = (kernel_width - 1) // 2
    
    # 创建卷积层，指定输入/输出通道和卷积核大小
    conv2d = torch.nn.Conv2d(
        in_channels=in_channels,  # 输入通道数，来自卷积核的第二维
        out_channels=out_channels,  # 输出通道数，来自卷积核的第一维
        kernel_size=(kernel_height, kernel_width),  # 卷积核大小
        stride=(1, 1),  # 步长为 1
        padding=(padding_height, padding_width),  # 使用计算出的 padding
        bias=False  # 不使用偏置项，因为原始程序中没有
    )
    
    # 将卷积层的权重替换为自定义的卷积核
    conv2d.weight.data = kernel_tensor
    
    # 将卷积层移到指定设备 (CPU/GPU)
    conv2d = conv2d.to(device)
    
    # 执行卷积操作
    output = conv2d(input_tensor)
    return output

# 测试 PyTorch 执行速度
start_time = time.time()
for i in range(4):
    pytorch_output = pytorch_convolution(input_data, kernel)
    torch_time = time.time() - start_time
    print(f"PyTorch {i}: {torch_time:.5f} 秒 {pytorch_output.shape} {pytorch_output[0][0][0][0]}")

# 输出最后一次执行的时间
print(f"PyTorch 总耗时: {torch_time:.5f} 秒")
