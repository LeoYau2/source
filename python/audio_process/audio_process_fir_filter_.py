#from scipy.io import wavefile

import scipy
from scipy.io.wavfile import read
from scipy.io.wavfile import write
import matplotlib.pyplot as plt
from scipy.fftpack import fft
import numpy as np
import math
import os

import numpy as np

def process_wav(input_wav, sample_range, power):
    # 将输入转换为NumPy数组以便进行向量化计算
    input_wav = np.array(input_wav)
    output_wav = np.zeros_like(input_wav)

    # 计算非零值的输出
    positive_indices = input_wav > 0
    negative_indices = ~positive_indices
    output_wav[positive_indices] = np.round(((input_wav[positive_indices] / sample_range) ** power) * sample_range)
    output_wav[negative_indices] = np.round(((input_wav[negative_indices] / sample_range * -1) ** power) * sample_range * -1)

    return output_wav


#rate, inputWav = read("input.wav")
#rate, inputWav = read("04. Blumenkranz.wav")
#rate, inputWav = read("D:\Music\試聽\SNoW - 逆さまの蝶.wav")
rate, inputWav = read("D:\Music\試聽\. 奈落の花.wav")
#gain = 0.125
#rateRatio = 1
#subStep = 8
#dt = 1/rate/rateRatio/subStep
#print(dt)
#force = float(180000000)
#free_oscillate_frequency = 18000     #w=(k/m)^0.5
#k_m_ratio = (free_oscillate_frequency/2/math.pi)**2
#damping = 7000
#maxError = 0.00001

#xl = float(0)
#xr = float(0)
#vl = float(0)
#vr = float(0)
#al = float(0)
#ar = float(0)
#substepCount = 0
#findValue = 0


# 繪製前 1024 點資料的波形圖
#plt.figure(figsize=(15, 5))
#plt.plot(inputWav[0:10000])
#plt.show()

print("Sample rate: {} Hz".format(rate))
print("Data type: {}".format(inputWav.dtype))
print("Duration:",len(inputWav)/rate,len(inputWav),"samples")

if format(inputWav.dtype) == "int16":
   print("16bit")
   sampleRange = 32768
if format(inputWav.dtype) == "int32":
   print("32bit")
   sampleRange = 2147483648
"""for i in range (1,100):
    print(i,inputWav[i][0],inputWav[i][1])"""

#power = float(input("Power = "))
gain = float(input("gain = "))
emf_gain = float(input("back emf gain = "))
#os.system("pause")

outputWav = np.zeros((len(inputWav),2))

#gainDivideSampleRange = gain/sampleRange
#testInput = 0
#testvl = 0
#testxl = 0
#testvr = 0
#testxr = 0

progressReportStep = 10000
progressStep = 0

shifted_wav = np.zeros_like(inputWav, dtype=np.float32)
shifted_wav[:-1, :] = inputWav[1:, :]
outputWav = (shifted_wav - inputWav) * emf_gain + inputWav * gain
referenceWav = inputWav * gain
emfWav = (shifted_wav - inputWav) * emf_gain

fir = np.zeros((3,2), dtype=np.float32)
fir[0][0] = round(32767 * gain)
fir[0][1] = round(32767 * gain)
fir[1][0] = round(32767 * emf_gain * -1)
fir[1][1] = round(32767 * emf_gain * -1)
fir[2][0] = 0
fir[2][1] = 0
#print(outputWav[100000])
#if sampleRange = 2147483648:
#clipped_wav = np.clip(outputWav, -32768, 32767)
"""for i in range(0,len(inputWav)):
   if inputWav[i][0] > 0:
      outputWav[i][0] = round(((float(inputWav[i][0])/sampleRange)**power)*sampleRange)
   else:
      outputWav[i][0] = round(((float(inputWav[i][0])/sampleRange*-1)**power)*sampleRange*-1)
   if inputWav[i][1] > 0:
      outputWav[i][1] = round(((float(inputWav[i][1])/sampleRange)**power)*sampleRange)
   else:
      outputWav[i][1] = round(((float(inputWav[i][1])/sampleRange*-1)**power)*sampleRange*-1)

   if outputWav[i][0] > 32767:
      outputWav[i][0] = 32767
   if outputWav[i][1] > 32767:
      outputWav[i][1] = 32767
   if outputWav[i][0] < -32768:
      outputWav[i][0] = -32767
   if outputWav[i][1] < -32768:
      outputWav[i][1] = -32767
#   print(i,inputWav[i][0],inputWav[i][1])

#   testInput = 0
#   adjust = 0.5
#   while adjust > maxError:
#      testvl = vl
#      testxl = xl
#      for k in range(subStep):
#         al = (testInput-xl)*force - testxl*k_m_ratio - testvl*damping
#         testvl += al*dt
#         testxl += testvl*dt
#      if (testxl > inputWav[i][0]*gainDivideSampleRange):
#         testInput -= adjust
#      else:
#         testInput += adjust
#      adjust /= 2
      #print("testInput",testInput,"target",inputWav[i][1]*gain/sampleRange,"xl",testxl)
#   vl = testvl
#   xl = testxl
#   outputWav[i][0] = int(testInput*sampleRange)
#
#   testInput = 0
#   adjust = 0.5
#   while adjust > maxError:
#      testvr = vr
#      testxr = xr
      
#      for k in range(subStep):
#         ar = (testInput-xr)*force - testxr*k_m_ratio - testvr*damping
#         testvr += ar*dt
#         testxr += testvr*dt

#      if (testxr > inputWav[i][1]*gainDivideSampleRange):
#         testInput -= adjust
#      else:
#         testInput += adjust
#      adjust /= 2
#   vr = testvr
#   xr = testxr
#   outputWav[i][1] = int(testInput*sampleRange)
   progressStep += 1
   if progressStep >= progressReportStep:
      progressStep = 0
      print("sample",i+1,round(i/len(inputWav)*100, 3),"%")"""
   #print(i)
             
      #if outputWav[i*rateRatio+j][0] > 32767:
      #   outputWav[i*rateRatio+j][0] = 32767
      #if outputWav[i*rateRatio+j][1] > 32767:
      #   outputWav[i*rateRatio+j][1] = 32767
      #if outputWav[i*rateRatio+j][0] < -32768:
      #   outputWav[i*rateRatio+j][0] = -32767
      #if outputWav[i*rateRatio+j][1] < -32768:
      #  outputWav[i*rateRatio+j][1] = -32768
print(np.max(outputWav))
print(np.min(outputWav))

if sampleRange == 2147483648:
   write("output.wav", rate, outputWav.astype(np.int32))
   write("reference.wav", rate, referenceWav.astype(np.int32))
   write("emf.wav", rate, emfWav.astype(np.int32))
else:
   write("output.wav", rate, outputWav.astype(np.int16))
   write("reference.wav", rate, referenceWav.astype(np.int16))
   write("emf.wav", rate, emfWav.astype(np.int16))
write("fir.wav", 192000, fir.astype(np.int16))
#write("300hz.gain0.5.fix.2.wav", rate*rateRatio, outputWav.astype(np.int16))
print("done")
#plt.figure(figsize=(16, 5))
#plt.plot(outputWav[0:len(inputWav)*rateRatio])
#plt.show()
#plt.figure(figsize=(16, 5))
#plt.plot(inputWav[0:len(inputWav)])
#plt.show()

#write("300hz.gain0.5.wav", 48000, out.astype(np.int16))
