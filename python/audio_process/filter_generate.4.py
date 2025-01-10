#from scipy.io import wavefile

import scipy
from scipy.io.wavfile import read
from scipy.io.wavfile import write
import matplotlib.pyplot as plt
from scipy.fftpack import fft, ifft, fftfreq
import numpy as np
import math
from scipy import signal
from scipy.fftpack import fft2, ifft2
from scipy.signal import firwin, freqz

def design_parametric_fir(freq, q, gain, fs, order, numtaps):
   
   nyq = 0.5 * fs
   freq_norm = freq / nyq  # 规范化中心频率
   transition_ratio = q / (np.pi * freq_norm)  # 过渡带比
   
   # 设计窗函数
   window = signal.windows.kaiser(numtaps, np.pi * transition_ratio)
   
   # 计算规范化 FIR 系数
   fir_coeff = signal.firwin(numtaps, 
                              [freq_norm - transition_ratio, freq_norm + transition_ratio], 
                              pass_zero=False, 
                              window=window)
   # 施加增益
   fir_coeff *= gain
   
   return fir_coeff

def write_fir_coeffs_to_wav(fir_coeffs, output_file, sample_rate=44100, num_channels=2, dtype=np.int16):
    """
    將FIR濾波器係數寫入wav文件

    參數:
        fir_coeffs (ndarray): FIR濾波器係數
        output_file (str): 輸出wav文件路徑
        sample_rate (int): 采樣率(Hz)
        num_channels (int): 聲道數
        dtype (dtype): 輸出數據類型
    """
    num_taps = len(fir_coeffs)
    output_data = np.tile(fir_coeffs, (num_channels, 1)).T.astype(dtype)

    # 創建一個全零矩陣,用於填充輸出數據
    output_data = np.pad(output_data, ((0, 0), (0, sample_rate - num_taps)), mode='constant')

    # 寫入wav文件
    wavfile.write(output_file, sample_rate, output_data)

def gaussian(x, mu, sigma):
    """
    計算高斯分布概率密度函數
    
    參數:
    x (float或numpy陣列): 自變量值
    mu (float): 均值
    sigma (float): 標準差
    
    返回:
    高斯分布概率密度值(float或numpy陣列)
    """
    return (1 / (sigma * np.sqrt(2 * np.pi))) * np.exp(-(x - mu)**2 / (2 * sigma**2))

rate, inputWav = read("300hz.2.wav")

inputWav = np.zeros((10000,2))
#rate, inputWav = read("02. NEXT COLOR PLANET.wav")
gain = 0.0015
rateRatio = 1
subStep = 8
dt = 1/rate/rateRatio/subStep
print(dt)
force = float(100000)
free_oscillate_frequency = 18000     #w=(k/m)^0.5
k_m_ratio = (free_oscillate_frequency/2/math.pi)**2
damping = 10000
maxError = 0.00001

xl = float(0)
xr = float(0)
vl = float(0)
vr = float(0)
al = float(0)
ar = float(0)
substepCount = 0
findValue = 0

#print("Sample rate: {} Hz".format(rate))
#print("Data type: {}".format(inputWav.dtype))
#print("Duration:",len(inputWav)/rate,len(inputWav),"samples")

if format(inputWav.dtype) == "int16":
   print(16)
   sampleRange = 32768
if format(inputWav.dtype) == "int32":
   print(32)
   sampleRange = 2147483648
#for i in range (1,100):
#    print(i,inputWav[i*1][0],inputWav[i*1][1])

sample_rate = 192000
fir_length = 10000
outputWav = np.zeros((10000,2))
filter_order = 20
freq = 1000
q = 2


#fade = 0.999
# 計算濾波器階數
#order = np.ceil(np.log10(2) / (2 * np.log10(transition_ratio))) 
#order = int(order)
# 計算濾波器長度 
#numtaps = order * 20 
#if numtaps % 2 == 0:
#    numtaps += 1

#fir_coeffs = design_parametric_fir(center_freq, q_factor, gain, sample_rate, filter_order, fir_length)
'''
for i in range (len(inputWav)):
   outputL = math.cos((i/fs)*freq*2*math.pi)*(fade**(i))*32767*gain
   #outputR = math.cos((i/rate)*freq*2*math.pi-2)*(fade**(i))*32767
   #output = (fade**(i))*32767
   outputWav[i][0] = outputL
   outputWav[i][1] = outputL
'''
gain = 0.42
gain = 1
frequency_response = np.zeros((1024))
frequency_response[:] = gain
center_freq = 1000
q_factor = 10

sample_rate = 192000
freq_min = 20
freq_max = 20000
gain_min = 0.01
gain_max = 100
fir_length = 65
#freq_bins = np.logspace(np.log10(freq_min), np.log10(freq_max), num=1024)
gain_db = np.linspace(-40, 10, num=1024)
amplitude = 10 ** (gain_db / 20)
#print(gain_db.size)

freq_bins = np.linspace(freq_min, freq_max, num=1024)

sigma = 40
for i in range (1024):
   #frequency_response[i] = gaussian(i, 500, 20)*20*(2*math.pi)**0.5*-3
   frequency_response[i] *= 10**(gaussian(i, 500, sigma)*sigma*(2*math.pi)**0.5*(-12)/20)
   #frequency_reponse[i] = 10
   #if i%100==0:
   #   print(frequency_response[i], i)
#frequency_response = np.array([1.0, 1.0, 0.8, 0.5, 0.4, 0.7, 1.0])
freq_bins = np.linspace(freq_min, freq_max, num=len(frequency_response))
plt.figure(figsize=(16, 5))
plt.plot(frequency_response)
plt.show()

# 生成对应的归一化频率采样点
#freq_norm = np.fft.fftfreq(fir_length, 1/sample_rate)[:fir_length//2+1]
freq_norm = np.fft.fftfreq(fir_length, 1/sample_rate)[:fir_length//2+1]
print("freq_norm:", freq_norm)

# 使用线性插值获得完整的频率响应
freq_norm_scaled = (freq_norm * sample_rate - freq_min) / (freq_max - freq_min)
freq_norm_scaled = (freq_bins - freq_min) / (freq_max - freq_min)
print("freq_norm_scaled:", freq_norm_scaled)
freq_response_full = np.interp(freq_norm_scaled, np.linspace(0, 1, num=len(frequency_response)), frequency_response)
freq_response_full = np.concatenate((freq_response_full, freq_response_full[:0:-1]))

print("freq_response_full:", freq_response_full, freq_response_full.size)
print("frequency_response:", frequency_response)
for i in range (freq_response_full.size):
   if i%20==0:
      print(freq_response_full[i], i)
complex_response = np.exp(1j * np.angle(frequency_response)) * np.abs(frequency_response)
fir_filter = np.fft.ifftshift(np.fft.ifft(complex_response))
fir_filter_real = np.real(fir_filter)
#fir_filter_2 = firwin(1024, [20, 20000], pass_zero=False, fs=sample_rate, window='hamming', gain=frequency_response)

#frequency_response = amplitude * np.exp(1j * np.zeros_like(freq_bins))
#kernel_freq_domain = np.concatenate((frequency_response, np.conj(frequency_response[::-1])))

# 执行反傅里叶变换并获取 FIR 滤波器系数
#fir_filter = np.fft.ifft(kernel_freq_domain).real

  # 補全為全頻譜
kernel = ifft(freq_response_full) * fir_length  # 反傅里葉變換
print("kernel:", kernel)
# 截短並加窗
kernel = kernel[None, :fir_length]  # 截短為FIR長度
print("kernel:", kernel)
window = np.hamming(fir_length)  # 加漢明窗
print(window)
fir_coeffs = kernel.squeeze() * window / np.sum(window)  # 規範化

'''# 生成對數頻率採樣點和線性尺度的頻率響應
freq_bins = freq_min * (freq_max / freq_min) ** (np.arange(1024) / 1023)
freq_response = 10 ** (frequency_response / 20)  # dB 轉換為線性尺度

# 對頻率響應進行反傅里葉變換
kernel = np.concatenate((freq_response, np.conj(freq_response[:0:-1])))  # 補全為全頻譜
kernel = ifft(kernel) * len(kernel)  # 反傅里葉變換

# 截短並加窗
kernel = kernel[None, :fir_length]  # 截短為 FIR 長度
window = np.hamming(fir_length)  # 加漢明窗
fir_coeffs = kernel.squeeze() * window / np.sum(window)  # 規範化'''

fir_coeffs *= 32767
fir_filter *= 32767
#fir_filter[480] = 20000

plt.figure(figsize=(16, 5))
#plt.plot(fir_coeffs)
plt.plot(fir_filter)

#plt.plot(window)
plt.show()
write("C:\Program Files\EqualizerAPO\config\EqualizerAPO\outfilter.2.wav", 192000, fir_filter_real.astype(np.int16))

#fir_coeffs = design_parametric_fir(center_freq, q_factor, gain, sample_rate, filter_order, fir_length)
#write_fir_coeffs_to_wav(fir_coeff, "C:\Program Files\EqualizerAPO\config\EqualizerAPO\filter.wav")
#write("C:\Program Files\EqualizerAPO\config\EqualizerAPO\filter.wav", rate*rateRatio, outputWav.astype(np.int16))
#write("300hz.gain0.5.fix.2.wav", rate*rateRatio, outputWav.astype(np.int16))
print("done")
#plt.figure(figsize=(16, 5))
#plt.plot(outputWav[0:len(inputWav)*rateRatio])
#plt.show()
#plt.figure(figsize=(16, 5))
#plt.plot(inputWav[0:len(inputWav)])
#plt.show()

#write("300hz.gain0.5.wav", 48000, out.astype(np.int16))
