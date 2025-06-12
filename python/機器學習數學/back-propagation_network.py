import numpy as np


def sigmoid(x): #sigmoid函數
    return 1 / (1 + np.exp(-x))

def sigmoid_derivative(x): #sigmoid微分
    return x * (1 - x)

train_data_input = np.zeros((100 ,2), dtype=np.int32)
train_data_output = np.zeros((100 ,1), dtype=np.int32)
test_data_input = np.zeros((100 ,2), dtype=np.int32)
test_data_output = np.zeros((100 ,1), dtype=np.int32)

for i in range(100): #生成訓練資料集和測試資料集
    train_data_input[i][0] = np.random.randint(0, 2)  #生成0或1
    train_data_input[i][1] = np.random.randint(0, 2)
    test_data_input[i][0] = np.random.randint(0, 2)
    test_data_input[i][1] = np.random.randint(0, 2)
    train_data_output[i] = (train_data_input[i][0] ^ train_data_input[i][1]) #計算xor
    test_data_output[i] = (test_data_input[i][0] ^ test_data_input[i][1])

input_neurons = 2  #input layer神經元數
hidden_neurons = 2 #hidden layer神經元數
output_neurons = 1 #output layer神經元數
learning_rate = 0.3

weights_input_to_hidden = np.random.uniform(-1, 1, (input_neurons, hidden_neurons)) #隨機初始weight
weights_hidden_to_output = np.random.uniform(-1, 1, (hidden_neurons, output_neurons))
bias_hidden = np.zeros((hidden_neurons)) #初始bias
bias_output = np.zeros((output_neurons))

epochs = 100000 #訓練次數

for i in range(epochs):
    #前向傳播
    hidden_layer_input = np.dot(train_data_input, weights_input_to_hidden) + bias_hidden
    hidden_layer_output = sigmoid(hidden_layer_input)
    
    output_layer_input = np.dot(hidden_layer_output, weights_hidden_to_output) + bias_output
    predicted_output = sigmoid(output_layer_input)

    error = train_data_output - predicted_output #計算誤差

    #反向傳播
    delta_predicted_output = error * sigmoid_derivative(predicted_output)
    error_hidden_layer = np.dot(delta_predicted_output, weights_hidden_to_output.T) #.T是轉置矩陣
    delta_hidden_layer = error_hidden_layer * sigmoid_derivative(hidden_layer_output)

    #更新weight和bias
    weights_hidden_to_output += np.dot(hidden_layer_output.T, delta_predicted_output) * learning_rate
    weights_input_to_hidden += np.dot(train_data_input.T, delta_hidden_layer) * learning_rate
    bias_output += np.sum(delta_predicted_output, axis=0) * learning_rate
    bias_hidden += np.sum(delta_hidden_layer, axis=0) * learning_rate

    if i % 1000 == 0:
        loss = np.mean(np.abs(error))
        print(f"Epoch {i}, Loss: {loss:.4f}")

#顯示訓練結果
print("hidden layer 到 output layer 權重")
print(weights_hidden_to_output)
print("input layer 到 hidden layer 權重")
print(weights_input_to_hidden)
print("output layer 偏置")
print(bias_output)
print("hidden layer 偏置")
print(bias_hidden)

#用test data測試正確率
hidden_layer_input_test = np.dot(test_data_input, weights_input_to_hidden) + bias_hidden
hidden_layer_output_test = sigmoid(hidden_layer_input_test)

output_layer_input_test = np.dot(hidden_layer_output_test, weights_hidden_to_output) + bias_output
predicted_output_test = sigmoid(output_layer_input_test)

predicted_classes = (predicted_output_test > 0.5).astype(int)  #predicted值以0.5為界線分成0或1
accuracy = np.mean(predicted_classes == test_data_output)  #計算正確率

print(f"正確率: {accuracy * 100:.2f}%")


