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

ev = -2
L = 2**ev
#L = 1/12
print("L           = %.28f" %(L))
#sRGB alpha=0.055
if L <= 0.00304:
    V_sRGB = 12.92*L
else:
    V_sRGB = (1+0.055)*L**(1/2.4)-0.055

if V_sRGB <= 0.0392768:
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