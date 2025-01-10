import math

def awg_to_diameter(awg):
    # 根據AWG標準計算直徑（英寸）
    diameter = 0.005 * (92 ** ((36 - awg) / 39))
    return diameter / 39.37  # 將直徑轉換為米
#36-(log(diameter / 0.005 * 39) / log (92)) = awg

def diameter_to_awg(diameter):
    # 根據直徑計算AWG編號
    awg = 36 - (math.log(diameter * 39.37 / 0.005) * 39) / math.log (92)
    #awg = round(36 - 39 * math.log10(0.005 * diameter * 39.37 / 92))
    return awg

def skin_effect(resistivity, skin_depth, depth):
    return resistivity*(1+(depth/skin_depth)**2) #計算趨膚效應下不同深度的電阻率

def calculate_branch_diameter(original_diameter, branches):
    # 計算分支後的直徑（米）
    branch_diameter = original_diameter * math.sqrt(1/branches)
    return branch_diameter

# 選擇材質
input_type = input("請輸入 'c' (copper) 或 's' (silver) 以選擇材質: ").lower()

if input_type == 'c':
    resistivity = 1.7e-8
    permeability = 1.256629e-6
elif input_type == 's':
    resistivity = 1.59e-8
    permeability = 1.256637e-6
else:
    exit()

print(f"電阻率:{resistivity:.6e}Ωm, 磁導率:{permeability:.6e}H/m")

# 選擇輸入方式
input_type = input("請輸入 'a' (awg) 或 'd' (diameter) 以選擇輸入方式: ").lower()

if input_type == 'a':
    awg = int(input("請輸入 AWG 編號："))
    original_diameter = awg_to_diameter(awg)
    print(f"AWG {awg} 對應的直徑為 {original_diameter:.8f} m")
elif input_type == 'd':
    original_diameter = float(input("請輸入線的直徑(m)："))
    #print(f"直徑 {original_diameter:.8f} m")
    corresponding_awg = diameter_to_awg(original_diameter)
    print(f"直徑 {original_diameter:.8f} m 對應的 AWG 編號為 {corresponding_awg:.4f}")
else:
    print("無效的輸入類型，請選擇 'a' (awg) 或 'd' (diameter)")
    exit()

# 提示用戶輸入並聯的分支數
branches = int(input("請輸入並聯的分支數："))

# 計算分支後的直徑
branch_diameter = original_diameter / math.sqrt(branches)
print(f"分支數 {branches}，分支直徑 {branch_diameter:.8f} m")

# 輸入線的長度
length = float(input("請輸入線的長度(m)："))

while True:
# 輸入訊號頻率
    frequency = float(input("請輸入訊號頻率（赫茲，Hz）："))
    if frequency == 0:
        break 

# 計算並聯電阻
    dc_resistence = resistivity*length/(math.pi*(original_diameter/2)**2)
    print("直流電阻:",f"{dc_resistence:.4f}Ω")
    radius = original_diameter/2/math.sqrt(branches) # 計算分支半徑
    skin_depth = math.sqrt(resistivity/(math.pi*frequency*permeability)) #計算趨膚深度
    resistence = 0
    for i in range(4096):
        area = math.pi*(radius**2 * (i+1)**2/4096**2 - radius**2*i**2/4096**2)
        resistence += 1/(skin_effect(resistivity, skin_depth, radius/4096*(4096-i))*length/area)
    resistence = 1 / resistence
    resistence = resistence / branches
    ratio = resistence/dc_resistence*100
    print(f"趨膚效應電阻為:{resistence:.4f}Ω, {ratio:.4f}%")
