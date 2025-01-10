#from scipy.io import wavefile

import scipy
from scipy.io.wavfile import read
from scipy.io.wavfile import write
import matplotlib.pyplot as plt
from scipy.fftpack import fft
import numpy as np
import math

rate, inputWav = read("300hz.wav")

gain = 0.5
substep = 4
dt = 1/rate/substep
xl = 0
xr = 0
vl = 0
vr = 0
al = 0
ar = 0
free_oscillate_frequency = 1600     #w=(k/m)^0.5
k_m_ratio = (free_oscillate_frequency/2/math.pi)**2
damping = 0.1


print("Sample rate: {} Hz".format(rate))
print("Data type: {}".format(inputWav.dtype))
print("Duration:",len(inputWav)/rate,len(inputWav),"samples")
for i in range (1,100):
    print(i,inputWav[i*1][0],inputWav[i*1][1])
outputWav = ((len(inputWav)),2)
for i in range(0,len(inputWav)):
    #al = (inputWav[i][0]-outputWav[i][0])*k_m_ratio - vl*damping
    #ar = (inputWav[i][1]-outputWav[i][1])*k_m_ratio - vr*damping
    vl += al
    vr += ar
    xl += vl
    xr += vr
    #outputWav[i][0] = xl
    #outputWav[i][1] = xr
#write("300hz.out.gain0.5.wav", 48000, out.astype(np.int16))

out = np.zeros((640,2))

for i in range(0,80):
    out[i][0] = 1*gain*32768
    out[i][1] = 1*gain*32768
    if out[i][0] > 32767:
        out[i][0] = 32767
    if out[i][1] > 32767:
        out[i][1] = 32767
    if out[i][0] < -32768:
        out[i][0] = -32767
    if out[i][1] < -32768:
        out[i][1] = 32768
for i in range(80,160):
    out[i][0] = -1*gain*32768
    out[i][1] = -1*gain*32768
    if out[i][0] > 32767:
        out[i][0] = 32767
    if out[i][1] > 32767:
        out[i][1] = 32767
    if out[i][0] < -32768:
        out[i][0] = -32767
    if out[i][1] < -32768:
        out[i][1] = 32768
for i in range(160,240):
    out[i][0] = 1*gain*32768
    out[i][1] = 1*gain*32768
    if out[i][0] > 32767:
        out[i][0] = 32767
    if out[i][1] > 32767:
        out[i][1] = 32767
    if out[i][0] < -32768:
        out[i][0] = -32767
    if out[i][1] < -32768:
        out[i][1] = 32768
for i in range(240,320):
    out[i][0] = -1*gain*32768
    out[i][1] = -1*gain*32768
    if out[i][0] > 32767:
        out[i][0] = 32767
    if out[i][1] > 32767:
        out[i][1] = 32767
    if out[i][0] < -32768:
        out[i][0] = -32767
    if out[i][1] < -32768:
        out[i][1] = 32768
for i in range(320,400):
    out[i][0] = 1*gain*32768
    out[i][1] = 1*gain*32768
    if out[i][0] > 32767:
        out[i][0] = 32767
    if out[i][1] > 32767:
        out[i][1] = 32767
    if out[i][0] < -32768:
        out[i][0] = -32767
    if out[i][1] < -32768:
        out[i][1] = 32768
for i in range(400,480):
    out[i][0] = -1*gain*32768
    out[i][1] = -1*gain*32768
    if out[i][0] > 32767:
        out[i][0] = 32767
    if out[i][1] > 32767:
        out[i][1] = 32767
    if out[i][0] < -32768:
        out[i][0] = -32767
    if out[i][1] < -32768:
        out[i][1] = 32768
for i in range(480,560):
    out[i][0] = 1*gain*32768
    out[i][1] = 1*gain*32768
    if out[i][0] > 32767:
        out[i][0] = 32767
    if out[i][1] > 32767:
        out[i][1] = 32767
    if out[i][0] < -32768:
        out[i][0] = -32767
    if out[i][1] < -32768:
        out[i][1] = 32768
for i in range(560,640):
    out[i][0] = -1*gain*32768
    out[i][1] = -1*gain*32768
    if out[i][0] > 32767:
        out[i][0] = 32767
    if out[i][1] > 32767:
        out[i][1] = 32767
    if out[i][0] < -32768:
        out[i][0] = -32767
    if out[i][1] < -32768:
        out[i][1] = 32768

write("300hz.2.wav", 48000, out.astype(np.int16))
