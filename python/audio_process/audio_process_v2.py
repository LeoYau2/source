#from scipy.io import wavefile

import scipy
from scipy.io.wavfile import read
from scipy.io.wavfile import write
import matplotlib.pyplot as plt
from scipy.fftpack import fft
import numpy as np
import math

rate, inputWav = read("300hz.2.wav")
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

print("Sample rate: {} Hz".format(rate))
print("Data type: {}".format(inputWav.dtype))
print("Duration:",len(inputWav)/rate,len(inputWav),"samples")

if format(inputWav.dtype) == "int16":
   print(16)
   sampleRange = 32768
if format(inputWav.dtype) == "int32":
   print(32)
   sampleRange = 2147483648
for i in range (1,100):
    print(i,inputWav[i*1][0],inputWav[i*1][1])

outputWav = np.zeros((len(inputWav)*rateRatio,2))

gainDivideSampleRange = gain/sampleRange
print(gainDivideSampleRange)
testInput = 0
testvl = 0
testxl = 0
testvr = 0
testxr = 0
progressReportStep = 1000
progressStep = 0
for i in range(0,len(inputWav)):
   testInput = 0
   adjust = 0.5
   while adjust > maxError:
      testvl = vl
      testxl = xl
      for k in range(subStep):
         al = (testInput-xl)*force - testxl*k_m_ratio - testvl*damping
            #ar = (inputWav[i][1]*gain/sampleRange-xr)*force - xr*k_m_ratio - vr*damping
         testvl += al*dt
            #vr += ar*dt
         testxl += testvl*dt
            #xr += vr*dt
      if (testxl > inputWav[i][0]*gainDivideSampleRange):
         testInput -= adjust
      else:
         testInput += adjust
      adjust /= 2
      #print("testInput",testInput,"target",inputWav[i][1]*gain/sampleRange,"xl",testxl)
   vl = testvl
   xl = testxl
   outputWav[i][0] = int(testInput*sampleRange)

   testInput = 0
   adjust = 0.5
   while adjust > maxError:
      testvr = vr
      testxr = xr
      
      for k in range(subStep):
         ar = (testInput-xr)*force - testxr*k_m_ratio - testvr*damping
         testvr += ar*dt
         testxr += testvr*dt

      if (testxr > inputWav[i][1]*gainDivideSampleRange):
         testInput -= adjust
      else:
         testInput += adjust
      adjust /= 2
   vr = testvr
   xr = testxr
   outputWav[i][1] = int(testInput*sampleRange)
   progressStep += 1
   if progressStep >= progressReportStep:
      progressStep = 0
      print("sample",i,i/len(inputWav)*100,"%")
   #print(i)
             
      #if outputWav[i*rateRatio+j][0] > 32767:
      #   outputWav[i*rateRatio+j][0] = 32767
      #if outputWav[i*rateRatio+j][1] > 32767:
      #   outputWav[i*rateRatio+j][1] = 32767
      #if outputWav[i*rateRatio+j][0] < -32768:
      #   outputWav[i*rateRatio+j][0] = -32767
      #if outputWav[i*rateRatio+j][1] < -32768:
      #  outputWav[i*rateRatio+j][1] = -32768
write("output.wav", rate*rateRatio, outputWav.astype(np.int16))
#write("300hz.gain0.5.fix.2.wav", rate*rateRatio, outputWav.astype(np.int16))
print("done")
plt.figure(figsize=(16, 5))
plt.plot(outputWav[0:len(inputWav)*rateRatio])
plt.show()
plt.figure(figsize=(16, 5))
plt.plot(inputWav[0:len(inputWav)])
plt.show()

#write("300hz.gain0.5.wav", 48000, out.astype(np.int16))
