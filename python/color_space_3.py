import imageio
import math
V_sRGB=0
V_Rec709=0
V_Rec2020=0
V_HLG=0
V_PQ=0

sRGB_L=0
Rec709_L=0
Rec2020_L=0
HLG_L=0
PQ_L=0

CIE_RGB = [0,0,0]
CIE_xyY = [0,0,0]
CIE_XYZ = [0,0,0]
RGB_linear = [0,0,0]
RGB = [0,0,0]

mode = int(input("Mode = "))
if mode == 0: #ev
    ev = float(input("EV = "))
    L = 2**ev
    
if mode == 1: #L
    L = float(input("L = "))
    ev = math.log(L,2)

if mode == 2: #CIE_xyY
    CIE_xyY[0] = float(input("CIE_xyY\nx = "))
    CIE_xyY[1] = float(input("y = "))
    CIE_xyY[2] = float(input("Y = "))
    print("CIE xyY = ",CIE_xyY)
    CIE_XYZ[0] = CIE_xyY[2]*CIE_xyY[0]/CIE_xyY[1]
    CIE_XYZ[1] = CIE_xyY[2]
    CIE_XYZ[2] = CIE_xyY[2]*(1-CIE_xyY[0]-CIE_xyY[1])/CIE_xyY[1]
    print("CIE XYZ = ",CIE_XYZ)
    CIE_RGB[0] =  0.4185*CIE_XYZ[0] -0.1587*CIE_XYZ[1] -0.0828*CIE_XYZ[2]
    CIE_RGB[1] = -0.0912*CIE_XYZ[0] +0.2524*CIE_XYZ[1] +0.0157*CIE_XYZ[2]
    CIE_RGB[2] =  0.0009*CIE_XYZ[0] -0.0025*CIE_XYZ[1] -0.1786*CIE_XYZ[2]
    print("CIE RGB = ",CIE_RGB)
    L = CIE_XYZ[1]
    ev = math.log(L,2)

if mode == 3: #CIE_XYZ
    CIE_XYZ[0] = float(input("CIE_XYZ\nX = "))
    CIE_XYZ[1] = float(input("Y = "))
    CIE_XYZ[2] = float(input("Z = "))
    print("CIE XYZ = ",CIE_XYZ)
    CIE_RGB[0] =  0.4185*CIE_XYZ[0] -0.1587*CIE_XYZ[1] -0.0828*CIE_XYZ[2]
    CIE_RGB[1] = -0.0912*CIE_XYZ[0] +0.2524*CIE_XYZ[1] +0.0157*CIE_XYZ[2]
    CIE_RGB[2] =  0.0009*CIE_XYZ[0] -0.0025*CIE_XYZ[1] -0.1786*CIE_XYZ[2]
    print("CIE RGB = ",CIE_RGB)
    L = CIE_XYZ[1]
    ev = math.log(L,2)
    
if mode == 4: #CIE_RGB
    CIE_RGB[0] = float(input("CIE_RGB\nR = "))
    CIE_RGB[1] = float(input("G = "))
    CIE_RGB[2] = float(input("B = "))
    print("CIE RGB = ",CIE_RGB)
    CIE_XYZ[0] = 2.7688*CIE_RGB[0] +1.7517*CIE_RGB[1] +1.1301*CIE_RGB[2]
    CIE_XYZ[1] =      1*CIE_RGB[0] +4.5906*CIE_RGB[1] +0.0601*CIE_RGB[2]
    CIE_XYZ[2] =                   -0.0565*CIE_RGB[1] +5.5942*CIE_RGB[2]
    print("CIE XYZ = ",CIE_XYZ)
    L = CIE_XYZ[1]
    ev = math.log(L,2)

#XYZ to sRGB
RGB_linear[0] =  3.2404542*CIE_XYZ[0] -1.5371385*CIE_XYZ[1] -0.4985314*CIE_XYZ[2]
RGB_linear[1] = -0.9692660*CIE_XYZ[0] +1.8760108*CIE_XYZ[1] +0.0415560*CIE_XYZ[2]
RGB_linear[2] =  0.0556434*CIE_XYZ[0] -0.2040259*CIE_XYZ[1] +1.0572252*CIE_XYZ[2]
print("sRGB_linear",RGB_linear)


for i in range(3):
    if RGB_linear[i] <= 0.00304:
        RGB[i] = 12.92*RGB_linear[i]
    else:
        RGB[i] = (1+0.055)*RGB_linear[i]**(1/2.4)-0.055

if mode == 5: #sRGB
    RGB_linear[0] = float(input("sRGB\nR = "))
    RGB_linear[1] = float(input("G = "))
    RGB_linear[2] = float(input("B = "))
            
print("sRGB = ",RGB)
#inverse
CIE_XYZ[0] = 0.4124564*RGB_linear[0] +0.3575761*RGB_linear[1] +0.1804375*RGB_linear[2]
CIE_XYZ[1] = 0.2126729*RGB_linear[0] +0.7151522*RGB_linear[1] +0.0721750*RGB_linear[2]
CIE_XYZ[2] = 0.0193339*RGB_linear[0] +0.1191920*RGB_linear[1] +0.9503041*RGB_linear[2]
print("inverse CIE_XYZ = ",CIE_XYZ)
    
#L = 1/12
print("L           = %.28f" %(L))
#sRGB alpha=0.055
if L <= 0.0031308:
    V_sRGB = 12.92*L
else:
    V_sRGB = (1+0.055)*L**(1/2.4)-0.055

if V_sRGB <= 0.040449936:
    sRGB_L = V_sRGB/12.92
else:
    sRGB_L = ((V_sRGB+0.055)/1.055)**2.4

#Rec. 709
#color space    xW	yW	xR	yR	xG	yG	xB	yB
#ITU-R BT.709	0.3127	0.3290	0.64	0.33	0.30	0.60	0.15	0.06
if L <= 0.018:
    V_Rec709 = 4.5*L
else:
    V_Rec709 = (1+0.099)*L**0.45-0.099

if V_Rec709 <= 0.081:
    Rec709_L = V_Rec709/4.5
else:
    Rec709_L = ((V_Rec709+0.099)/1.099)**(1/0.45)
    
#Rec. 2020
#alpha=1.09929682680944,beta=0.018053968510807
#color space    xW	yW	xR	yR	xG	yG	xB	yB
#ITU-R BT.2020	0.3127	0.3290	0.708	0.292	0.17	0.797	0.131	0.046
if L <= 0.018053968510807:
    V_Rec2020 = 4.5*L
else:
    V_Rec2020 = (1+0.09929682680944)*L**0.45-0.09929682680944

if V_Rec2020 <= 4.5*0.018053968510807:
    Rec2020_L = V_Rec2020/4.5
else:
    Rec2020_L = ((V_Rec2020+0.09929682680944)/1.09929682680944)**(1/0.45)

#Hybrid Log-Gamma
#a = 0.17883277, b = 0.28466892, c = 0.55991073
#V_HLG<0.5:L = V_HLG**2/3,V_HLG>0.5:L = (e*((V_HLG-0.55991073)/0.17883277)+0.28466892)/12
if L <= 1/12:
    V_HLG = 3**0.5*L**0.5
else:
    V_HLG = 0.17883277*math.log(12*L-0.28466892)+0.55991073

if V_HLG <= 0.5:
    #HLG_L = (V_HLG/3**0.5)**2
    HLG_L = V_HLG**2/3
else:
    HLG_L = (math.exp(((V_HLG-0.55991073)/0.17883277))+0.28466892)/12

#Perceptual Quantizer
#c1=c3-c2+1=107/128=0.8359375,c2=2413/128=18.8515625,c3=2392/128=18.6875,m1=1305/8192=0.1593017578125,m2=2523/32=78.84375
#L**(1305/8192) = -((128*V_PQ**(32/2523)-107)/(2392*V_PQ**(32/2523)-2413))
#L = (-((128*V_PQ**(32/2523)-107)/(2392*V_PQ**(32/2523)-2413)))**(8192/1305)
#L = (-((128*V_PQ**0.0126833135-107)/(2392*V_PQ**0.0126833135-2413)))**6.2773946360
V_PQ = ((0.8359375+18.8515625*L**0.1593017578125)/(1+18.6875*L**0.1593017578125))**78.84375

PQ_L = (-((128*V_PQ**(32/2523)-107)/(2392*V_PQ**(32/2523)-2413)))**(8192/1305)
#PQ_L = (-((128*V_PQ**0.0126833135-107)/(2392*V_PQ**0.0126833135-2413)))**6.2773946360
    
print("%.3f EV" % (ev))
print("V_sRGB      = %.7f  *255 = %.2f *1023 = %.2f *4095 = %.2f" % (V_sRGB, V_sRGB*255, V_sRGB*1023, V_sRGB*4095))
print("V_Rec. 709  = %.7f  *255 = %.2f *1023 = %.2f *4095 = %.2f" % (V_Rec709, V_Rec709*255, V_Rec709*1023, V_Rec709*4095))
print("V_Rec. 2020 = %.7f  *255 = %.2f *1023 = %.2f *4095 = %.2f" % (V_Rec2020, V_Rec2020*255, V_Rec2020*1023, V_Rec2020*4095))
print("V_HLG       = %.7f  *255 = %.2f *1023 = %.2f *4095 = %.2f" % (V_HLG, V_HLG*255, V_HLG*1023, V_HLG*4095))
print("V_PQ        = %.7f  *255 = %.2f *1023 = %.2f *4095 = %.2f" % (V_PQ, V_PQ*255, V_PQ*1023, V_PQ*4095))
print("Inverse Function")
print("sRGB_L      = %.28f" % (sRGB_L))
print("Rec. 709_L  = %.28f" % (Rec709_L))
print("Rec. 2020_L = %.28f" % (Rec2020_L))
print("HLG_L       = %.28f" % (HLG_L))
print("PQ_L        = %.28f" % (PQ_L))

"""
                        xW	yW	K	xR	yR	xG	yG	xB	yB
sRGB                    0.3127	0.3290	6500    0.64	0.33	0.30	0.60	0.15	0.06
ITU-R BT.709	        0.3127	0.3290	6500    0.64	0.33	0.30	0.60	0.15	0.06
ITU-R BT.2020	        0.3127	0.3290	6500    0.708	0.292	0.17	0.797	0.131	0.046
P3-D65 (Display)	0.3127	0.3290	6504	0.680	0.320	0.265	0.690	0.150	0.060
P3-DCI (Theater)	0.314	0.351	6300	0.680	0.320	0.265	0.690	0.150	0.060
P3-D60 (ACES Cinema)	0.32168	0.33767	6000	0.680	0.320	0.265	0.690	0.150	0.060
ACES AP0                                        0.7347  0.2653  0.0000  1.0000  0.0001  -0.0770
ACES AP1                                        0.7347  0.2653  0.0000  1.0000  0.0001  -0.0770
"""

"""
YUV <--> RGB

Y'= 0.299*R' + 0.587*G' + 0.114*B'
U'= -0.147*R' - 0.289*G' + 0.436*B' = 0.492*(B'- Y')
V'= 0.615*R' - 0.515*G' - 0.100*B' = 0.877*(R'- Y')

R' = Y' + 1.140*V'
G' = Y' - 0.394*U' - 0.581*V'
B' = Y' + 2.032*U'

SDTV-YCbCr <--> RGB
Y’ = 0.257*R' + 0.504*G' + 0.098*B' + 16
Cb' = -0.148*R' - 0.291*G' + 0.439*B' + 128
Cr' = 0.439*R' - 0.368*G' - 0.071*B' + 128

R' = 1.164*(Y’-16) + 1.596*(Cr'-128)
G' = 1.164*(Y’-16) - 0.813*(Cr'-128) - 0.392*(Cb'-128)
B' = 1.164*(Y’-16) + 2.017*(Cb'-128)
"""
