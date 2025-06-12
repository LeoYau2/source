import numpy as np

def cov(a,b):
    mean_a = np.mean(a)
    mean_b = np.mean(b)
    convarience = np.sum((a - mean_a) * (b - mean_b)) / (len(a) - 1)
    return convarience

x = [2.0, 3.0, 3.1, 5.2, 6.5, 7.0, 8.2, 9.3, 10.2, 11.0]
y = [12.0, 10.8, 2.9, 0.5, -2.0, -4.2, -6.0, -8.3, -10.2, -12.1]
z = [7.0, 3.0, 2.0, 6.0, 0.0, 8.0, -1.0, 4.0, 1.0, 5.0]

convarience_matrix = [[cov(x,x),cov(x,y),cov(x,z)],
                      [cov(y,x),cov(y,y),cov(y,z)],
                      [cov(z,x),cov(z,y),cov(z,z)]]

print(convarience_matrix[0])
print(convarience_matrix[1])
print(convarience_matrix[2])
