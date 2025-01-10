#from scipy.io import wavefile

import scipy
from scipy.io.wavfile import read
from scipy.io.wavfile import write
import matplotlib.pyplot as plt
from scipy.fftpack import fft
import numpy as np
import math
from scipy import signal

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
fade = 0.999
gain = 0.1

#fade = 0.999
# 計算濾波器階數
#order = np.ceil(np.log10(2) / (2 * np.log10(transition_ratio))) 
#order = int(order)
# 計算濾波器長度 
#numtaps = order * 20 
#if numtaps % 2 == 0:
#    numtaps += 1

#fir_coeffs = design_parametric_fir(center_freq, q_factor, gain, sample_rate, filter_order, fir_length)

for i in range (len(inputWav)):
   outputL = math.cos((i/sample_rate)*freq*2*math.pi)*(fade**(i))*32767*gain
   #outputR = math.cos((i/rate)*freq*2*math.pi-2)*(fade**(i))*32767
   #output = (fade**(i))*32767
   outputWav[i][0] = outputL
   outputWav[i][1] = outputL

#fir_coeffs = design_parametric_fir(center_freq, q_factor, gain, sample_rate, filter_order, fir_length)
#write_fir_coeffs_to_wav(fir_coeff, "C:\Program Files\EqualizerAPO\config\EqualizerAPO\filter.wav")
write("C:\Program Files\EqualizerAPO\config\EqualizerAPO\output.wav", sample_rate, outputWav.astype(np.int16))
#write("300hz.gain0.5.fix.2.wav", rate*rateRatio, outputWav.astype(np.int16))
print("done")
plt.figure(figsize=(16, 5))
plt.plot(outputWav[0:len(inputWav)*rateRatio])
plt.show()
plt.figure(figsize=(16, 5))
plt.plot(inputWav[0:len(inputWav)])
plt.show()

#write("300hz.gain0.5.wav", 48000, out.astype(np.int16))
