import math

def skin_effect(resistivity, skin_depth, depth):
    return resistivity*(1+(depth/skin_depth)**2)

frequency = 17000
resistivity = 1.7e-8
permeability = 1.256629e-6
diameter = 0.0003
radius = diameter / 2
length = 1.2
depth = 0.001
print("frequency:",frequency,"hz")
skin_depth = math.sqrt(resistivity/(math.pi*frequency*permeability))
print("skin_depth:",skin_depth, "m")
skin_effect_resistivity = skin_effect(resistivity, skin_depth, depth)
print("depth:",depth,"m, skin_effect_resistivity:",skin_effect_resistivity, "Ωm")

dc_resistence = resistivity*length/(math.pi*(diameter/2)**2)
print("DC resistence:",f"{dc_resistence:.4f}Ω")
resistence = 0
for i in range(4096):
    area = math.pi*(radius**2 * (i+1)**2/4096**2 - radius**2*i**2/4096**2)
    resistence += 1/(skin_effect(resistivity, skin_depth, radius/4096*(4096-i))*length/area)
resistence = 1/resistence
ratio = resistence/dc_resistence*100
print(frequency,"hz, resistence:",f"{resistence:.4f}Ω, {ratio:.2f}%")
