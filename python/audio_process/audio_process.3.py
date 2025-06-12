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
gain = 0.015
substep = 2
dt = 1/rate/substep
print(dt)
force = float(100000)
free_oscillate_frequency = 600     #w=(k/m)^0.5
k_m_ratio = (free_oscillate_frequency/2/math.pi)**2/substep
damping = 10000
errors = 0.00001

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
#for i in range (1,100):
#    print(i,inputWav[i*1][0],inputWav[i*1][1])
    
outputWav = np.zeros((len(inputWav)*substep,2))
for i in range(0,len(inputWav)):
    for j in range(substep):
        al = ((inputWav[i][0]*gain-xl)*force/sampleRange - xl*k_m_ratio - vl*damping)*dt
        ar = ((inputWav[i][1]*gain-xr)*force/sampleRange - xr*k_m_ratio - vr*damping)*dt
        vl += al
        vr += ar
        xl += vl
        xr += vr
        outputWav[i*substep+j][0] = int(xl*sampleRange)
        outputWav[i*substep+j][1] = int(xr*sampleRange)
    #al = ((inputWav[i][0]-xl)*force*gain - xl*k_m_ratio - vl*damping)*dt/sampleRange
    #ar = ((inputWav[i][1]-xr)*force*gain - xr*k_m_ratio - vr*damping)*dt/sampleRange
    
        if outputWav[i*substep+j][0] > 32767:
            outputWav[i*substep+j][0] = 32767
        if outputWav[i*substep+j][1] > 32767:
            outputWav[i*substep+j][1] = 32767
        if outputWav[i*substep+j][0] < -32768:
            outputWav[i*substep+j][0] = -32767
        if outputWav[i*substep+j][1] < -32768:
            outputWav[i*substep+j][1] = 32768
write("output.wav", rate*substep, outputWav.astype(np.int16))
#write("300hz.gain0.5.wav", rate*substep, outputWav.astype(np.int16))

plt.figure(figsize=(16, 5))
plt.plot(outputWav[0:len(inputWav)*substep])
plt.show()
plt.figure(figsize=(16, 5))
plt.plot(inputWav[0:len(inputWav)])
plt.show()

#write("300hz.gain0.5.wav", 48000, out.astype(np.int16))
