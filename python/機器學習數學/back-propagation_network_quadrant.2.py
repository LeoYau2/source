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

def calculate_accuracy(data_input, data_output, weights_input_to_hidden_1, bias_hidden_1, weights_hidden_1_to_hidden_2, bias_hidden_2, weights_hidden_2_to_output, bias_output):

    # 向前傳播
    hidden_layer_input_1 = np.dot(data_input, weights_input_to_hidden_1) + bias_hidden_1
    hidden_layer_output_1 = sigmoid(hidden_layer_input_1)

    hidden_layer_input_2 = np.dot(hidden_layer_output_1, weights_hidden_1_to_hidden_2) + bias_hidden_2
    hidden_layer_output_2 = sigmoid(hidden_layer_input_2)

    output_layer_input = np.dot(hidden_layer_output_2, weights_hidden_2_to_output) + bias_output
    predicted_output = sigmoid(output_layer_input)

    predicted_classes = np.argmax(predicted_output, axis=1)  # 預測類別
    true_classes = np.argmax(data_output, axis=1)            # 真實類別

    correct_count = np.sum(predicted_classes == true_classes)  # 計算正確數量
    accuracy = (correct_count / len(data_output))    # 計算正確率

    return accuracy

train_data_input = np.random.uniform(-1000, 1000, (4000, 2))
train_data_output = np.array([quadrant(x, y) for x, y in train_data_input])
test_data_input = np.random.uniform(-1000, 1000, (1000, 2))
test_data_output = np.array([quadrant(x, y) for x, y in test_data_input])


input_neurons = 2  #input layer神經元數
hidden_neurons_1 = 32 #第一層hidden layer神經元數，可調整
hidden_neurons_2 = 16 #第二層hidden layer神經元數，可調整
output_neurons = 4 #output layer神經元數

initial_learning_rate = 0.004 #初始學習率
decay = 80 #衰減係數，可調整

weights_input_to_hidden_1 = np.random.uniform(-1, 1, (input_neurons, hidden_neurons_1)) #隨機初始weight，範圍-1到1
weights_hidden_1_to_hidden_2 = np.random.uniform(-1, 1, (hidden_neurons_1, hidden_neurons_2))
weights_hidden_2_to_output = np.random.uniform(-1, 1, (hidden_neurons_2, output_neurons))

bias_hidden_1 = np.zeros((hidden_neurons_1)) #初始bias，數值0
bias_hidden_2 = np.zeros((hidden_neurons_2))
bias_output = np.zeros((output_neurons)) 

epochs = 5001 #訓練次數，可調整

for i in range(epochs):
    learning_rate = initial_learning_rate / (1 + (i / decay)) #計算學習率
    
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

        #用訓練資料集測試正確率
        train_accuracy = calculate_accuracy(
            train_data_input, train_data_output, 
            weights_input_to_hidden_1, bias_hidden_1, 
            weights_hidden_1_to_hidden_2, bias_hidden_2, 
            weights_hidden_2_to_output, bias_output
        )

        #用測試資料集測試正確率
        test_accuracy = calculate_accuracy(
            test_data_input, test_data_output, 
            weights_input_to_hidden_1, bias_hidden_1, 
            weights_hidden_1_to_hidden_2, bias_hidden_2, 
            weights_hidden_2_to_output, bias_output
        )
        #顯示迭帶次數、學習率、損失函數值、訓練資料集測試正確率、測試資料集測試正確率
        print(f"Epoch {i}, learning rate: {learning_rate:.6f}, Loss: {loss:.4f}, train accuracy: {train_accuracy * 100:.2f}%, test accuracy: {test_accuracy * 100:.2f}%")

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

     
print(f"訓練資料集正確率: {train_accuracy * 100:.2f}%, 測試資料集正確率: {test_accuracy * 100:.2f}%")


