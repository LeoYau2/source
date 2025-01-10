import imageio
import math
V_sRGB=0
V_Rec709=0
V_Rec2020=0
V_HLG=0
V_PQ=0
V_Gamma22=0
V_Gamma26=0
V_BT_1886=0
BT_1886_variable = [1, 1, 0, 0.5, 2.4, 0, 0] #L, Lw, Lb, V, gamma, a, b

sRGB_L=0
Rec709_L=0
Rec2020_L=0
HLG_L=0
PQ_L=0
Gamma22_L=0
Gamma26_L=0
BT_1886_L=0

CIE_RGB = [0,0,0] #[B,G,R]
CIE_xyY = [0,0,0] #[x,y,Y]
CIE_XYZ = [0,0,0] #[Z,Y,X]
sRGB_linear = [0,0,0] #[B,G,R]
RGB = [0,0,0]

"""mode = int(input("Mode = "))
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
    CIE_XYZ[2] = CIE_xyY[2]*CIE_xyY[0]/CIE_xyY[1]
    CIE_XYZ[1] = CIE_xyY[2]
    CIE_XYZ[0] = CIE_xyY[2]*(1-CIE_xyY[0]-CIE_xyY[1])/CIE_xyY[1]
    print("CIE XYZ = ",CIE_XYZ)
    CIE_RGB[0] =  0.4185*CIE_XYZ[0] -0.1587*CIE_XYZ[1] -0.0828*CIE_XYZ[2]
    CIE_RGB[1] = -0.0912*CIE_XYZ[0] +0.2524*CIE_XYZ[1] +0.0157*CIE_XYZ[2]
    CIE_RGB[2] =  0.0009*CIE_XYZ[0] -0.0025*CIE_XYZ[1] -0.1786*CIE_XYZ[2]
    print("CIE RGB = ",CIE_RGB)
    L = CIE_XYZ[1]
    ev = math.log(L,2)

if mode == 3: #CIE_XYZ
    CIE_XYZ[2] = float(input("CIE_XYZ\nX = "))
    CIE_XYZ[1] = float(input("Y = "))
    CIE_XYZ[0] = float(input("Z = "))
    print("CIE XYZ = ",CIE_XYZ)
    CIE_xyY[0] = CIE_XYZ[2]/(CIE_XYZ[2]+CIE_XYZ[1]+CIE_XYZ[0])
    CIE_xyY[1] = CIE_XYZ[1]
    CIE_xyY[2] = CIE_XYZ[1]/(CIE_XYZ[2]+CIE_XYZ[1]+CIE_XYZ[0])
    print("CIE xyY = ",CIE_xyY)
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
    CIE_xyY[0] = CIE_XYZ[2]/(CIE_XYZ[2]+CIE_XYZ[1]+CIE_XYZ[0])
    CIE_xyY[1] = CIE_XYZ[1]
    CIE_xyY[2] = CIE_XYZ[1]/(CIE_XYZ[2]+CIE_XYZ[1]+CIE_XYZ[0])
    print("CIE xyY = ",CIE_xyY)
    L = CIE_XYZ[1]
    ev = math.log(L,2)

#CIE_XYZ to sRGB
sRGB_linear[2] =  3.2404542*CIE_XYZ[2] -1.5371385*CIE_XYZ[1] -0.4985314*CIE_XYZ[0]
sRGB_linear[1] = -0.9692660*CIE_XYZ[2] +1.8760108*CIE_XYZ[1] +0.0415560*CIE_XYZ[0]
sRGB_linear[0] =  0.0556434*CIE_XYZ[2] -0.2040259*CIE_XYZ[1] +1.0572252*CIE_XYZ[0]
print("sRGB_linear",sRGB_linear)



for i in range(3):
    if sRGB_linear[i] <= 0.00304:
        RGB[i] = 12.92*sRGB_linear[i]
    else:
        RGB[i] = (1+0.055)*sRGB_linear[i]**(1/2.4)-0.055

if mode == 5: #sRGB
    sRGB_linear[0] = float(input("sRGB\nR = "))
    sRGB_linear[1] = float(input("G = "))
    sRGB_linear[2] = float(input("B = "))
            
print("sRGB = ",RGB)
#inverse sRGB to CIE_XYZ
CIE_XYZ[2] = 0.4124564*sRGB_linear[2] +0.3575761*sRGB_linear[1] +0.1804375*sRGB_linear[0]
CIE_XYZ[1] = 0.2126729*sRGB_linear[2] +0.7151522*sRGB_linear[1] +0.0721750*sRGB_linear[0]
CIE_XYZ[0] = 0.0193339*sRGB_linear[2] +0.1191920*sRGB_linear[1] +0.9503041*sRGB_linear[0]
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

#Gamma 2.2
V_Gamma22 = L**(1/2.2)

#Gamma 2.6
V_Gamma26 = L**(1/2.6)

#BT.1886
#Lb = a*b**gamma
#Lw = a*(1+b)**gamma
BT_1886_variable[5] = (BT_1886_variable[1]**(1/BT_1886_variable[4])-BT_1886_variable[2]**(1/BT_1886_variable[4]))**BT_1886_variable[4] #a = (Lw**(1/gamma)-Lb**(1/gamma))**gamma
BT_1886_variable[6] = (BT_1886_variable[2]**(1/BT_1886_variable[4]))/(BT_1886_variable[1]**(1/BT_1886_variable[4])-BT_1886_variable[2]**(1/BT_1886_variable[4])) #b = (Lb**(1/gamma)/(Lw**(1/gamma)-Lb**(1/gamma))
BT_1886_L = BT_1886_variable[5]*max((BT_1886_variable[3]+BT_1886_variable[6]),0)**BT_1886_variable[4] #L = a*(max[(V+b),0)**gamma

    
print("%.3f EV" % (ev))
print("V_sRGB      = %.7f  *255 = %.2f *1023 = %.2f *4095 = %.2f" % (V_sRGB, V_sRGB*255, V_sRGB*1023, V_sRGB*4095))
print("V_Rec. 709  = %.7f  *255 = %.2f *1023 = %.2f *4095 = %.2f" % (V_Rec709, V_Rec709*255, V_Rec709*1023, V_Rec709*4095))
print("V_Rec. 2020 = %.7f  *255 = %.2f *1023 = %.2f *4095 = %.2f" % (V_Rec2020, V_Rec2020*255, V_Rec2020*1023, V_Rec2020*4095))
print("V_HLG       = %.7f  *255 = %.2f *1023 = %.2f *4095 = %.2f" % (V_HLG, V_HLG*255, V_HLG*1023, V_HLG*4095))
print("V_PQ        = %.7f  *255 = %.2f *1023 = %.2f *4095 = %.2f" % (V_PQ, V_PQ*255, V_PQ*1023, V_PQ*4095))
print("V_Gamma 2.2 = %.7f  *255 = %.2f *1023 = %.2f *4095 = %.2f" % (V_Gamma22, V_Gamma22*255, V_Gamma22*1023, V_Gamma22*4095))
print("V_Gamma 2.6 = %.7f  *255 = %.2f *1023 = %.2f *4095 = %.2f" % (V_Gamma26, V_Gamma26*255, V_Gamma26*1023, V_Gamma26*4095))
print("Inverse Function")
print("sRGB_L      = %.28f" % (sRGB_L))
print("Rec. 709_L  = %.28f" % (Rec709_L))
print("Rec. 2020_L = %.28f" % (Rec2020_L))
print("HLG_L       = %.28f" % (HLG_L))
print("PQ_L        = %.28f" % (PQ_L))
print("BT.1886_L   = %.28f" % (BT_1886_L))
print(BT_1886_variable)
#print(BT_1886_variable[6])
#print(BT_1886_variable[6]*max((BT_1886_variable[3]+BT_1886_variable[6]),0))
"""

def sRGB_EOTF_8U(V):
    if V <= 11: #0.040449936*255 = 10.31473368
        L = V/3294.6
    else:
        L = ((V/255+0.055)/1.055)**2.4
    return L

def sRGB_EOTF(V):
    if V <= 0.040449936:
        L = V/12.92
    else:
        L = ((V+0.055)/1.055)**2.4
    return L

def sRGB_OETF_8U(L):
    if L <= 0.0031308:
        return 255 * (12.92 * L)
    else:
        return round(255 * (1.055 * pow(L, 0.41666666) - 0.055))

while(1):
    hex_8U3C = input("8U3C_V_sRGB_HEX = ")
    max_value = 0
    max_id = 0
    multiplier = 0
    print(hex_8U3C)
    #print(int(hex_8U3C, 16) / 65536)

    string_hex_3C = [hex_8U3C[0:2], hex_8U3C[2:4], hex_8U3C[4:6]]
    dec_8U3C = [int(string_hex_3C[0],16), int(string_hex_3C[1],16), int(string_hex_3C[2],16)]
    L_3C = [sRGB_EOTF_8U(dec_8U3C[0]), sRGB_EOTF_8U(dec_8U3C[1]), sRGB_EOTF_8U(dec_8U3C[2])]
    print(string_hex_3C)
    print(dec_8U3C)
    print(L_3C)
    output = "#" + hex_8U3C + " [" + str(int(dec_8U3C[0])).rjust(3) + "," + str(int(dec_8U3C[1])).rjust(3) + "," + str(int(dec_8U3C[2])).rjust(3) + "]"
    for i in range(3):
        if(int(string_hex_3C[i],16) > max_value):
            max_value = int(string_hex_3C[i],16)
            max_id = i
    #print(max_value)
    #print(max_id)
    multiplier = 1/L_3C[max_id]
    L_3C = [L_3C[0]* multiplier, L_3C[1]* multiplier, L_3C[2]* multiplier]
    print("L * " + str(multiplier))
    print(L_3C)
    dec_8U3C = [sRGB_OETF_8U(L_3C[0]), sRGB_OETF_8U(L_3C[1]), sRGB_OETF_8U(L_3C[2])]
    print(dec_8U3C)
    hex_8U3C = [hex(int(dec_8U3C[0])), hex(int(dec_8U3C[1])), hex(int(dec_8U3C[2]))] 
    output = output + " x" + '%.3f'%multiplier + " #" + hex_8U3C[0][2:4].zfill(2) + hex_8U3C[1][2:4].zfill(2) + hex_8U3C[2][2:4].zfill(2) + " [" + str(int(dec_8U3C[0])).rjust(3) + "," + str(int(dec_8U3C[1])).rjust(3) + "," + str(int(dec_8U3C[2])).rjust(3) + "]"
    print(output)
    print("#" + hex_8U3C[0][2:4] + hex_8U3C[1][2:4] + hex_8U3C[2][2:4])

"""
'%.intf'%float
str.rjust(int)
str.zfill(int)
"""

"""
V_sRGB = float(input("V_sRGB [0,255] = "))/255
if V_sRGB <= 0.040449936:
    sRGB_L = V_sRGB/12.92
else:
    sRGB_L = ((V_sRGB+0.055)/1.055)**2.4

t1=sRGB_L

V_sRGB = float(input("V_sRGB_2 [0,255] = "))/255
if V_sRGB <= 0.040449936:
    sRGB_L = V_sRGB/12.92
else:
    sRGB_L = ((V_sRGB+0.055)/1.055)**2.4

sRGB_L = sRGB_L/t1

if sRGB_L <= 0.0031308:
    V_sRGB = 12.92*sRGB_L
else:
    V_sRGB = (1+0.055)*sRGB_L**(1/2.4)-0.055
print(V_sRGB)
print(V_sRGB*255)
print(hex(round(V_sRGB*255)))

V_sRGB = float(input("V_sRGB_2 [0,255] = "))/255
if V_sRGB <= 0.040449936:
    sRGB_L = V_sRGB/12.92
else:
    sRGB_L = ((V_sRGB+0.055)/1.055)**2.4

sRGB_L = sRGB_L/t1

if sRGB_L <= 0.0031308:
    V_sRGB = 12.92*sRGB_L
else:
    V_sRGB = (1+0.055)*sRGB_L**(1/2.4)-0.055
print(V_sRGB)
print(V_sRGB*255)
print(hex(round(V_sRGB*255)))

"""

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

EOTF_1
if(V>=0.332238):
    L = (tan(V*pi/2)-0.10146692)/4
else:
    L =

611586,0.332238
#OETF_2
if L <= 0.0031308:
        V = L*12.92
    elif L <= 0.237335:
        V = 1.055*L**(1/2.4)-0.055
    else:
        V = (math.atan(6*frame[i,j,0]+1.1274908)*4/math.pi)-1
"""
"""
2.76 0        0.1775
0    1.92     0
0    0.041875 1.06

0.84687 0.01161 0.01764

0.4124564*3.2404542 +0.21267293*-1.5371385+0.01933393*-0.4985314
"""
