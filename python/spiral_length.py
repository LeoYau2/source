import math

diameter = float(input("圓柱直徑："))
screw_pitch = float(input("螺距："))
height = float(input("圓柱高："))
spiral_length_ratio = math.sqrt((math.pi*diameter)**2+screw_pitch*screw_pitch)/screw_pitch
pitch_length = screw_pitch * spiral_length_ratio
spiral_length = height * spiral_length_ratio
print(f"螺旋線長:{spiral_length:3f}, 一圈{pitch_length:3f}")
