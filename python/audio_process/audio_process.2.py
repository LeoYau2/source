#from scipy.io import wavefile
import scipy
from scipy.io.wavfile import read
from scipy.io.wavfile import write
import matplotlib.pyplot as plt
from scipy.fftpack import fft
import numpy as np

gain = 0.5
substep = 4
damping = 0.1
xl = 0
xr = 0
Vl = 0
Vr = 0

#rate, inputWav = read("test-8000Hz-le-2ch-1byteu.wav")
rate, inputWav = read("normal-hitclap.wav")
#rate, inputWav = read("test24b.wav")

if format(inputWav.dtype) == "int16":
   print(16)
   range = 32768
if format(inputWav.dtype) == "int32":
   print(32)
   range = 2147483648

print("Sample rate: {} Hz".format(rate))
print("Data type: {}".format(inputWav.dtype))
time = np.arange(0, len(inputWav)) / rate
#sampleCount = len(inputWav)
print("Duration:",len(inputWav)/rate,len(inputWav),"samples")

for i in range (1,64):
    print(i,inputWav[i*1][0],inputWav[i*1][1])
    
dataFFT = fft(inputWav[0:1024])
dataFFTAbs = abs(dataFFT[1:512])
#print("len: {}".len(inputWav))


outputWav = inputWav*0.5
write("out.wav", rate, outputWav.astype(np.int16))
out = np.zeros((320,2))

for i in range(0,1):
   out[i][0] = 1*gain*range
   out[i][1] = -1*gain*range
#for i in range(0,1):
#if 1:
#    count = 0
#    while count <160:
#        if count<80:
#            out[+count][0]=1*gain*range
#            out[+count][1]=1*gain*range
#        else:
#            out[+count][0]=-1*gain*range
#            out[+count][1]=-1*gain*range
#        count+=1
#       write("out.wav", 48000, out.astype(np.int16))
#plt.figure(figsize=(15, 5))
#plt.plot(outputWav[0:1024])
#plt.show()
plt.figure(figsize=(15, 5))
plt.plot(inputWav[0:64])
plt.show()
plt.figure(figsize=(15, 5))
plt.plot(dataFFTAbs, 'r')
plt.show()

plotA = plt.subplot(211)
plotA.plot(time, inputWav)
plotA.set_ylabel("Amplitude")
plotA.set_xlim(0, len(inputWav) / rate)

# 繪製時頻譜圖
plotB = plt.subplot(212)
#plotB.specgram(inputWav, NFFT=1024, Fs=rate, noverlap=900)
plotB.set_ylabel("Frequency")
plotB.set_xlabel("Time")

plt.show()
