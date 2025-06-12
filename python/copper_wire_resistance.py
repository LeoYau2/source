import math

def awg_to_diameter(awg):
    # 根據AWG標準計算直徑（英寸）
    diameter = 0.005 * (92 ** ((36 - awg) / 39))
    return diameter / 39.37  # 將直徑轉換為米

def calculate_resistance(length, diameter, resistivity):
    # 計算線的橫截面積（m²）
    area = math.pi * (diameter / 2) ** 2

    # 計算電阻（Ω）
    resistance = resistivity * (length / area)

    return resistance

# 提示用戶輸入線的材料

material = input("請輸入 'c' (copper) 或 's' (silver) 以選擇材料: ").lower()

# 根據材料選擇電阻率
if material == 'c':
    resistivity = 1.7e-8  # 銅的電阻率（Ω·m）
elif material == 's':
    resistivity = 1.59e-8  # 銀的電阻率（Ω·m）
else:
    print("無效的材料選擇，請選擇 'c' (copper) 或 's' (silver)")
    exit()

# 提示用戶輸入 AWG 編號或直徑
input_type = input("請輸入 'a' (awg) 或 'd' (diameter) 以選擇輸入方式: ").lower()

if input_type == 'a':
    awg = int(input("請輸入 AWG 編號："))
    diameter = awg_to_diameter(awg)
    print(f"AWG {awg} 對應的直徑為 {diameter:.7f} 米")
elif input_type == 'd':
    diameter = float(input("請輸入線的直徑（米）："))
    print(f"直徑 {diameter:.7f} 米")
else:
    print("無效的輸入類型，請選擇 'a'(awg) 或 'd'(diameter)")
    exit()

# 提示用戶輸入線的長度
length = float(input("請輸入線的長度（米）："))

# 計算電阻
result_resistance = calculate_resistance(length, diameter, resistivity)

# 輸出結果
print(f"圓柱 {material} 線長度 {length} 米，直徑 {diameter:.7f} 米的電阻為 {result_resistance:.7f} Ω")
