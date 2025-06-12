import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import fsolve

# 定義數字列表
numbers = np.array([
    14028619, 10215759, 9035483, 8222898, 7674947,
    7209680, 5831198, 5060127, 4516278,
    4003896, 3426044, 2816047, 2250617, 1738946,
    1292521, 1211790, 1132802, 1057054, 984109,
    912093, 842204, 707106, 774377, 641281,
    576333, 511374, 431525, 400378
])

size = np.array([13235.2, 6515.4, 5285.1, 4599.3, 4113.2, 3663.8, 3141.3, 2565.1, 2057.1, 1598.5, 1193.1, 836.4, 513.6, 342.9,
                 13283.2, 6592.1, 5343.6, 4647.2, 4153.6, 3699.6, 3172.8, 2614, 2094.1, 1630, 1221.1, 869.8, 545.4, 364,
                 13329, 6633.1, 5377.3, 4673.4, 4170.3, 3716.5, 3182.7, 2617.7, 2095.9, 1624.8, 1217.9, 866.2, 541.9, 363.5,
                 13390.9, 6664.6, 5402.8, 4694.9, 4189.3, 3729.8, 3193.6, 2628.5, 2105.9, 1628.5, 1219.3, 867, 541.5, 360.8,
                 13366.3, 6765.1, 5486.4, 4766.6, 4255.6, 3786.6, 3242.8, 2663.3, 2128.2, 1645.2, 1222, 860, 532.4, 356.9,
                 13376.7, 6792.2, 5508.8, 4786.4, 4270.2, 3801.6, 3255.7, 2676.5, 2138.2, 1653.6, 1228.4, 862.7, 533.6, 356.5,
                 13699.8, 7040.7, 5694.5, 4941.5, 4410.4, 3910.1, 3345.7, 2750, 2197.9, 1698.2, 1262.2, 890.7, 562.8, 391,
                 13702.4, 7451.1, 6075.6, 5307.8, 4772.7, 4287.6, 3718.9, 3099.9, 2512.8, 1954.1, 1443, 1015.4, 648.9, 447.7,
                 13723.9, 7579, 6128.5, 5285.5, 4687, 4158.8, 3562.3, 2933.3, 2354.3, 1821.2, 1352.9, 946.7, 592.1, 344.3,
                 14756.3, 9172.3, 7278.8, 6241.2, 5523.2, 4867.6, 4121, 3340.7, 2622.3, 1970.6, 1415.7, 963.9, 593.9, 349.4])

size_ratio = size.copy()


for j in range(10):
    for i in range(14):
        size_ratio[j*14+i] = size_ratio[j*14+i] / size[i]
        print(f"{size_ratio[j*14+i]:.5f}, ", end="")
    print("")

print(size_ratio)


# 計算自然對數
ln_values = np.log(numbers)
a = ln_values[0]
ln_values -= a

# 輸出結果為 numpy array
print(ln_values)

# 給定的數列
x = np.array([0,1,2,3,4,5, 10, 15, 20, 25, 30, 35, 40, 45, 50,51,52,53,54,55,56,57,58,59,60,61,62,63])
a = ln_values[0]
y = np.array = ln_values

# 擬合多項式的次數
degree = 4 # 改成你需要的次數

# 擬合多項式
coefficients = np.polyfit(x, y, degree)
polynomial = np.poly1d(coefficients)

# 輸出擬合的係數
print(f"Coefficients: {coefficients}")

# 創建用於繪圖的 x 值
x_fit = np.linspace(min(x), max(x), 100)
y_fit = polynomial(x_fit)
'''
# 繪圖
plt.figure(figsize=(8, 6))  # 確保這裡的 figsize 是正確的
plt.scatter(x, y, color='red', label='Data Points')
plt.plot(x_fit, y_fit, label=f'{degree}-degree Polynomial Fit')
plt.xlabel('x')
plt.ylabel('y')
plt.legend()
plt.show()'''

for i in range(len(ln_values)):
    print(f"{x[i]}, {y[i]:.3f}, {polynomial(x[i]):.3f}, {(polynomial(x[i])-y[i]):.3f}")
    
# 定義原始多項式
def polynomial(x):
    #return -2.537e-05 * x**3 + 0.002053 * x**2 - 0.08316 * x - 0.1392
    return 5.48043369e-07 * x**4 -9.44538185e-05 * x**3 + 4.78633490e-03 * x**2 -1.18258206e-01 * x -6.65678822e-02

# 定義反函數
def inverse_polynomial(y, guess):
    return fsolve(lambda x: polynomial(x) - y, guess)

# 示例：計算給定 y = 0.5 的 x
y = -1
initial_guess = 0  # 初始猜測值
x_value = inverse_polynomial(y, initial_guess)

print(f'The value of x for y = {y} is approximately: {x_value[0]}')

