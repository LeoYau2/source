import numpy as np


def sigmoid(x): #sigmoid函數
    return 1 / (1 + np.exp(-x))

def sigmoid_derivative(x): #sigmoid微分
    return x * (1 - x)

def quadrant(x ,y): #區分象限
    quadrant_output = np.zeros(4)
    if x > 0 and y > 0:
        quadrant_output = [1, 0, 0, 0]  # 第一象限
    elif x < 0 and y > 0:
        quadrant_output = [0, 1, 0, 0]  # 第二象限
    elif x < 0 and y < 0:
        quadrant_output = [0, 0, 1, 0]  # 第三象限
    else:
        quadrant_output = [0, 0, 0, 1]  # 第四象限
    
    return quadrant_output

train_data_input = np.zeros((4000 ,2), dtype=np.int32)
train_data_output = np.zeros((4000 ,4), dtype=np.int32)
test_data_input = np.zeros((1000 ,2), dtype=np.int32)
test_data_output = np.zeros((1000 ,4), dtype=np.int32)

for i in range(4000): #生成訓練資料集
    train_data_input[i][0] = np.random.uniform(-1000, 1000)  #生成-1000到1000隨機數
    train_data_input[i][1] = np.random.uniform(-1000, 1000)
    train_data_output[i] = quadrant(train_data_input[i][0], train_data_input[i][1])

for i in range(1000): #生成測試資料集
    test_data_input[i][0] = np.random.uniform(-1000, 1000)  #生成-1000到1000隨機數
    test_data_input[i][1] = np.random.uniform(-1000, 1000)
    test_data_output[i] = quadrant(test_data_input[i][0], test_data_input[i][1])

input_neurons = 2  #input layer神經元數
hidden_neurons_1 = 32 #第一層hidden layer神經元數，可調整
hidden_neurons_2 = 16 #第二層hidden layer神經元數，可調整
output_neurons = 4 #output layer神經元數
learning_rate = 0.0003

weights_input_to_hidden_1 = np.random.uniform(-1, 1, (input_neurons, hidden_neurons_1)) #隨機初始weight，範圍-1到1
weights_hidden_1_to_hidden_2 = np.random.uniform(-1, 1, (hidden_neurons_1, hidden_neurons_2))
weights_hidden_2_to_output = np.random.uniform(-1, 1, (hidden_neurons_2, output_neurons))

bias_hidden_1 = np.zeros((hidden_neurons_1)) #初始bias，數值0
bias_hidden_2 = np.zeros((hidden_neurons_2))
bias_output = np.zeros((output_neurons)) 

epochs = 10000 #訓練次數，可調整

for i in range(epochs):
    #向前傳播
    hidden_layer_input_1 = np.dot(train_data_input, weights_input_to_hidden_1) + bias_hidden_1 #dot是內積
    hidden_layer_output_1 = sigmoid(hidden_layer_input_1)

    hidden_layer_input_2 = np.dot(hidden_layer_output_1, weights_hidden_1_to_hidden_2) + bias_hidden_2
    hidden_layer_output_2 = sigmoid(hidden_layer_input_2)

    output_layer_input = np.dot(hidden_layer_output_2, weights_hidden_2_to_output) + bias_output
    predicted_output = sigmoid(output_layer_input)

    error = train_data_output - predicted_output #計算誤差

    #反向傳播
    delta_predicted_output = error * sigmoid_derivative(predicted_output)
    error_hidden_layer_2 = np.dot(delta_predicted_output, weights_hidden_2_to_output.T) #.T是轉置矩陣
    delta_hidden_layer_2 = error_hidden_layer_2 * sigmoid_derivative(hidden_layer_output_2)

    error_hidden_layer_1 = np.dot(delta_hidden_layer_2, weights_hidden_1_to_hidden_2.T)
    delta_hidden_layer_1 = error_hidden_layer_1 * sigmoid_derivative(hidden_layer_output_1)

    #更新weight和bias
    weights_hidden_2_to_output += np.dot(hidden_layer_output_2.T, delta_predicted_output) * learning_rate
    weights_hidden_1_to_hidden_2 += np.dot(hidden_layer_output_1.T, delta_hidden_layer_2) * learning_rate
    weights_input_to_hidden_1 += np.dot(train_data_input.T, delta_hidden_layer_1) * learning_rate

    bias_output += np.sum(delta_predicted_output) * learning_rate
    bias_hidden_2 += np.sum(delta_hidden_layer_2) * learning_rate
    bias_hidden_1 += np.sum(delta_hidden_layer_1) * learning_rate

    if i % 100 == 0:
        loss = np.mean(np.abs(error))
        print(f"Epoch {i}, Loss: {loss:.4f}")

print("")

#顯示訓練結果
print("hidden layer 2 到 output layer 權重")
print(weights_hidden_2_to_output)
print("hidden layer 1 到 hidden layer 2 權重")
print(weights_hidden_1_to_hidden_2)
print("input layer 1 到 hidden layer 1 權重")
print(weights_input_to_hidden_1)
print("output layer 偏置")
print(bias_output)
print("hidden layer 2 偏置")
print(bias_hidden_2)
print("hidden layer 2 偏置")
print(bias_hidden_1)
print("")

#用測試資料集測試正確率
hidden_layer_input_1 = np.dot(test_data_input, weights_input_to_hidden_1) + bias_hidden_1
hidden_layer_output_1 = sigmoid(hidden_layer_input_1)

hidden_layer_input_2 = np.dot(hidden_layer_output_1, weights_hidden_1_to_hidden_2) + bias_hidden_2
hidden_layer_output_2 = sigmoid(hidden_layer_input_2)

output_layer_input = np.dot(hidden_layer_output_2, weights_hidden_2_to_output) + bias_output
predicted_output = sigmoid(output_layer_input)

correct_count = 0

predicted_classes = np.argmax(predicted_output, axis=1) #取最大值的索引
true_classes = np.argmax(test_data_output, axis=1)

for i in range(1000):
    if predicted_classes[i] == true_classes[i]:
        correct_count += 1
     
print(f"測試資料集正確率: {correct_count / 1000 * 100:.2f}%")


