import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import fsolve

from scipy.interpolate import Rbf
'''
from sklearn.linear_model import Ridge
from sklearn.preprocessing import PolynomialFeatures
from sklearn.pipeline import make_pipeline'''

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


for i in range(14):
    for j in range(10):
        size_ratio[j*14+i] = size_ratio[j*14+i] / size[i]
        print(f"{size_ratio[j*14+i]:.5f}, ", end="")
    print("")

print(size_ratio)

import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

# 數據輸入
data = np.array([
    [1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000, 1.00000],
    [1.00363, 1.01177, 1.01107, 1.01041, 1.00982, 1.00977, 1.01003, 1.01906, 1.01799, 1.01971, 1.02347, 1.03993, 1.06192, 1.06153],
    [1.00709, 1.01806, 1.01745, 1.01611, 1.01388, 1.01438, 1.01318, 1.02051, 1.01886, 1.01645, 1.02079, 1.03563, 1.05510, 1.06008],
    [1.01176, 1.02290, 1.02227, 1.02079, 1.01850, 1.01801, 1.01665, 1.02472, 1.02372, 1.01877, 1.02196, 1.03659, 1.05432, 1.05220],
    [1.00991, 1.03832, 1.03809, 1.03638, 1.03462, 1.03352, 1.03231, 1.03828, 1.03456, 1.02921, 1.02422, 1.02822, 1.03660, 1.04083],
    [1.01069, 1.04248, 1.04233, 1.04068, 1.03817, 1.03761, 1.03642, 1.04343, 1.03942, 1.03447, 1.02959, 1.03144, 1.03894, 1.03966],
    [1.03510, 1.08062, 1.07746, 1.07440, 1.07226, 1.06723, 1.06507, 1.07208, 1.06845, 1.06237, 1.05792, 1.06492, 1.09579, 1.14027],
    [1.03530, 1.14361, 1.14957, 1.15405, 1.16034, 1.17026, 1.18387, 1.20849, 1.22153, 1.22246, 1.20945, 1.21401, 1.26343, 1.30563],
    [1.03692, 1.16324, 1.15958, 1.14920, 1.13950, 1.13511, 1.13402, 1.14354, 1.14448, 1.13932, 1.13394, 1.13187, 1.15284, 1.00408],
    [1.11493, 1.40779, 1.37723, 1.35699, 1.34280, 1.32857, 1.31188, 1.30237, 1.27476, 1.23278, 1.18657, 1.15244, 1.15635, 1.01896]
])

x = np.arange(data.shape[0])
y = np.arange(data.shape[1])
X, Y = np.meshgrid(x, y, indexing='ij')

# 將數據攤平，以便於curve_fit處理
xdata = np.vstack((X.ravel(), Y.ravel()))
zdata = data.ravel()


def func2(xy, a, b, c, d, e, f):
    x, y = xy
    return a*x**2 + b*y**2 + c*x*y + d*x + e*y + f

def func3(xy, a, b, c, d, e, f, g, h, i, j):
    x, y = xy
    return a*x**3 + b*y**3 + c*x**2*y + d*x*y**2 + e*x**2 + f*y**2 + g*x*y + h*x + i*y + j

def func4(xy, a, b, c, d, e, f, g, h, i, j, k, l, m, n, o):
    x, y = xy
    return (a*x**4 + b*y**4 + c*x**3*y + d*x*y**3 + e*x**3 + f*y**3 +
            g*x**2*y**2 + h*x**2*y + i*x*y**2 + j*x**2 + k*y**2 + 
            l*x*y + m*x + n*y + o)



'''params, _ = curve_fit(func4, xdata, zdata)
print(f'擬合參數: {params}')

Z_fit = func4((X, Y), *params).reshape(data.shape)'''

x = np.linspace(0, 13, 14)
y = np.linspace(0, 9, 10)
print(x)
print(y)
x, y = np.meshgrid(x, y)
rbf = Rbf(x, y, data, function='multiquadric')  # 可以嘗試不同的 function 參數
Z_fit = rbf(x, y)

plt.figure(figsize=(12, 6))

plt.subplot(1, 3, 1)
plt.contourf(X, Y, data, levels=20)
plt.title('data')
plt.colorbar()

plt.subplot(1, 3, 2)
plt.contourf(X, Y, Z_fit, levels=20)
plt.title('fit')
plt.colorbar()

plt.subplot(1, 3, 3)
plt.contourf(X, Y, Z_fit-data, levels=20)
plt.title('error')
plt.colorbar()

plt.show()

'''
degree = 4  # 這裡可以調整為更高階的多項式
model = make_pipeline(PolynomialFeatures(degree), Ridge(alpha=1e-3))

xdata = np.vstack((x.ravel(), y.ravel())).T
zdata = data.ravel()

model.fit(xdata, zdata)
z_fit = model.predict(xdata).reshape(data.shape)'''


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

