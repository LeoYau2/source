//g++ .\pipe_rgb_hdr_brightness_gain.cpp -o .\pipe_rgb_hdr_brightness_gain.exe -std=c++11 -O3 -march=native -fopenmp
#include <stdio.h>
#include <immintrin.h>
#include <iostream>
#include <chrono>
#include <thread>
//#include <fstream>
#include <vector>
//#include <filesystem>
#include <cmath>
//#include <Eigen/Dense>
#include <string>
#include <cstring>
#include <dirent.h>
//#include <sstream>
//#include <iomanip>
#include <cstdint>
#include <unistd.h>
#include <fcntl.h>

void array_scalar_add_40(float* outarray, float* array, float scalar, int size) {
    int remainder = size % 40;
    int adjustedSize = size - remainder;
    __m256 scalarVector = _mm256_set1_ps(scalar);
    for (int i = 0; i < adjustedSize; i += 8) {
        __m256 dataVector_1 = _mm256_loadu_ps(&array[i]);
        __m256 dataVector_2 = _mm256_loadu_ps(&array[i+8]);
        __m256 dataVector_3 = _mm256_loadu_ps(&array[i+16]);
        __m256 dataVector_4 = _mm256_loadu_ps(&array[i+24]);
        __m256 dataVector_5 = _mm256_loadu_ps(&array[i+32]);
        __m256 resultVector_1 = _mm256_add_ps(dataVector_1, scalarVector);
        __m256 resultVector_2 = _mm256_add_ps(dataVector_2, scalarVector);
        __m256 resultVector_3 = _mm256_add_ps(dataVector_3, scalarVector);
        __m256 resultVector_4 = _mm256_add_ps(dataVector_4, scalarVector);
        __m256 resultVector_5 = _mm256_add_ps(dataVector_5, scalarVector);
        _mm256_storeu_ps(&outarray[i], resultVector_1);
        _mm256_storeu_ps(&outarray[i+8], resultVector_2);
        _mm256_storeu_ps(&outarray[i+16], resultVector_3);
        _mm256_storeu_ps(&outarray[i+24], resultVector_4);
        _mm256_storeu_ps(&outarray[i+32], resultVector_5);
    }
    for (int i = adjustedSize; i < size; ++i) {
        array[i] += scalar;
    }
}

void array_add_40(float* outarray, float* array1, float* array2, int size) {
    int remainder = size % 40;
    int adjustedSize = size - remainder;
    for (int i = 0; i < adjustedSize; i += 40) {
        __m256 vector1_1 = _mm256_loadu_ps(&array1[i]);
        __m256 vector1_2 = _mm256_loadu_ps(&array1[i + 8]);
        __m256 vector1_3 = _mm256_loadu_ps(&array1[i + 16]);
        __m256 vector1_4 = _mm256_loadu_ps(&array1[i + 24]);
        __m256 vector1_5 = _mm256_loadu_ps(&array1[i + 32]);
        __m256 vector2_1 = _mm256_loadu_ps(&array2[i]);
        __m256 vector2_2 = _mm256_loadu_ps(&array2[i + 8]);
        __m256 vector2_3 = _mm256_loadu_ps(&array2[i + 16]);
        __m256 vector2_4 = _mm256_loadu_ps(&array2[i + 24]);
        __m256 vector2_5 = _mm256_loadu_ps(&array2[i + 32]);
        vector1_1 = _mm256_add_ps(vector1_1, vector2_1);
        vector1_2 = _mm256_add_ps(vector1_2, vector2_2);
        vector1_3 = _mm256_add_ps(vector1_3, vector2_3);
        vector1_4 = _mm256_add_ps(vector1_4, vector2_4);
        vector1_5 = _mm256_add_ps(vector1_5, vector2_5);
        _mm256_storeu_ps(&outarray[i], vector1_1);
        _mm256_storeu_ps(&outarray[i + 8], vector1_2);
        _mm256_storeu_ps(&outarray[i + 16], vector1_3);
        _mm256_storeu_ps(&outarray[i + 24], vector1_4);
        _mm256_storeu_ps(&outarray[i + 32], vector1_5);
    }
    for (int i = adjustedSize; i < size; ++i) {
        outarray[i] = array1[i] + array2[i];
    }
}

void array_subtract_40(float* outarray, float* array1, float* array2, int size) {
    int remainder = size % 40;
    int adjustedSize = size - remainder;
    for (int i = 0; i < adjustedSize; i += 40) {
        __m256 vector1_1 = _mm256_loadu_ps(&array1[i]);
        __m256 vector1_2 = _mm256_loadu_ps(&array1[i + 8]);
        __m256 vector1_3 = _mm256_loadu_ps(&array1[i + 16]);
        __m256 vector1_4 = _mm256_loadu_ps(&array1[i + 24]);
        __m256 vector1_5 = _mm256_loadu_ps(&array1[i + 32]);
        __m256 vector2_1 = _mm256_loadu_ps(&array2[i]);
        __m256 vector2_2 = _mm256_loadu_ps(&array2[i + 8]);
        __m256 vector2_3 = _mm256_loadu_ps(&array2[i + 16]);
        __m256 vector2_4 = _mm256_loadu_ps(&array2[i + 24]);
        __m256 vector2_5 = _mm256_loadu_ps(&array2[i + 32]);
        vector1_1 = _mm256_sub_ps(vector1_1, vector2_1);
        vector1_2 = _mm256_sub_ps(vector1_2, vector2_2);
        vector1_3 = _mm256_sub_ps(vector1_3, vector2_3);
        vector1_4 = _mm256_sub_ps(vector1_4, vector2_4);
        vector1_5 = _mm256_sub_ps(vector1_5, vector2_5);
        _mm256_storeu_ps(&outarray[i], vector1_1);
        _mm256_storeu_ps(&outarray[i + 8], vector1_2);
        _mm256_storeu_ps(&outarray[i + 16], vector1_3);
        _mm256_storeu_ps(&outarray[i + 24], vector1_4);
        _mm256_storeu_ps(&outarray[i + 32], vector1_5);
    }
    for (int i = adjustedSize; i < size; ++i) {
        outarray[i] = array1[i] - array2[i];
    }
}

void array_scalar_multiply_40(float* outarray, float* array, float scalar, int size) {
    int remainder = size % 40;
    int adjustedSize = size - remainder;
    __m256 scalarVector = _mm256_set1_ps(scalar);
    for (int i = 0; i < adjustedSize; i += 8) {
        __m256 dataVector_1 = _mm256_loadu_ps(&array[i]);
        __m256 dataVector_2 = _mm256_loadu_ps(&array[i+8]);
        __m256 dataVector_3 = _mm256_loadu_ps(&array[i+16]);
        __m256 dataVector_4 = _mm256_loadu_ps(&array[i+24]);
        __m256 dataVector_5 = _mm256_loadu_ps(&array[i+32]);
        __m256 resultVector_1 = _mm256_mul_ps(dataVector_1, scalarVector);
        __m256 resultVector_2 = _mm256_mul_ps(dataVector_2, scalarVector);
        __m256 resultVector_3 = _mm256_mul_ps(dataVector_3, scalarVector);
        __m256 resultVector_4 = _mm256_mul_ps(dataVector_4, scalarVector);
        __m256 resultVector_5 = _mm256_mul_ps(dataVector_5, scalarVector);
        _mm256_storeu_ps(&outarray[i], resultVector_1);
        _mm256_storeu_ps(&outarray[i+8], resultVector_2);
        _mm256_storeu_ps(&outarray[i+16], resultVector_3);
        _mm256_storeu_ps(&outarray[i+24], resultVector_4);
        _mm256_storeu_ps(&outarray[i+32], resultVector_5);
    }
    for (int i = adjustedSize; i < size; ++i) {
        outarray[i] *= scalar;
    }
}

void array_multiply_40(float* outarray, float* array1, float* array2, int size) {
    int remainder = size % 40;
    int adjustedSize = size - remainder;
    for (int i = 0; i < adjustedSize; i += 40) {
        __m256 vector1_1 = _mm256_loadu_ps(&array1[i]);
        __m256 vector1_2 = _mm256_loadu_ps(&array1[i + 8]);
        __m256 vector1_3 = _mm256_loadu_ps(&array1[i + 16]);
        __m256 vector1_4 = _mm256_loadu_ps(&array1[i + 24]);
        __m256 vector1_5 = _mm256_loadu_ps(&array1[i + 32]);
        __m256 vector2_1 = _mm256_loadu_ps(&array2[i]);
        __m256 vector2_2 = _mm256_loadu_ps(&array2[i + 8]);
        __m256 vector2_3 = _mm256_loadu_ps(&array2[i + 16]);
        __m256 vector2_4 = _mm256_loadu_ps(&array2[i + 24]);
        __m256 vector2_5 = _mm256_loadu_ps(&array2[i + 32]);
        vector1_1 = _mm256_mul_ps(vector1_1, vector2_1);
        vector1_2 = _mm256_mul_ps(vector1_2, vector2_2);
        vector1_3 = _mm256_mul_ps(vector1_3, vector2_3);
        vector1_4 = _mm256_mul_ps(vector1_4, vector2_4);
        vector1_5 = _mm256_mul_ps(vector1_5, vector2_5);
        _mm256_storeu_ps(&outarray[i], vector1_1);
        _mm256_storeu_ps(&outarray[i + 8], vector1_2);
        _mm256_storeu_ps(&outarray[i + 16], vector1_3);
        _mm256_storeu_ps(&outarray[i + 24], vector1_4);
        _mm256_storeu_ps(&outarray[i + 32], vector1_5);
    }
    for (int i = adjustedSize; i < size; ++i) {
        outarray[i] = array1[i] * array2[i];
    }
}

void array_sqrt_40(float* outarray, float* array1, int size) {
    int remainder = size % 40;
    int adjustedSize = size - remainder;
    for (int i = 0; i < adjustedSize; i += 40) {
        __m256 vector1 = _mm256_loadu_ps(&array1[i]);
        __m256 vector2 = _mm256_loadu_ps(&array1[i + 8]);
        __m256 vector3 = _mm256_loadu_ps(&array1[i + 16]);
        __m256 vector4 = _mm256_loadu_ps(&array1[i + 24]);
        __m256 vector5 = _mm256_loadu_ps(&array1[i + 32]);
        __m256 resultVector1 = _mm256_sqrt_ps(vector1);
        __m256 resultVector2 = _mm256_sqrt_ps(vector2);
        __m256 resultVector3 = _mm256_sqrt_ps(vector3);
        __m256 resultVector4 = _mm256_sqrt_ps(vector4);
        __m256 resultVector5 = _mm256_sqrt_ps(vector5);
        _mm256_storeu_ps(&outarray[i], resultVector1);
        _mm256_storeu_ps(&outarray[i + 8], resultVector2);
        _mm256_storeu_ps(&outarray[i + 16], resultVector3);
        _mm256_storeu_ps(&outarray[i + 24], resultVector4);
        _mm256_storeu_ps(&outarray[i + 32], resultVector5);
    }
    for (int i = adjustedSize; i < size; ++i) {
        outarray[i] = sqrt(array1[i]);
    }
}

void float_clip_min_8(float* array, float min, int size) {
    int remainder = size % 8;
    int adjustedSize = size - remainder;
    __m256 Vector_min = _mm256_set1_ps(min);
    for (int i = 0; i < adjustedSize; i += 8) {
        __m256 Vector1 = _mm256_load_ps(&array[i]);
        Vector1 = _mm256_max_ps(Vector1, Vector_min);
        _mm256_store_ps(&array[i], Vector1);
    }
    for (int i = adjustedSize; i < size; ++i) {
        if(array[i] < min){
            array[i] = min;
        }
    }
}

void float_clip_min_40(float* array, float min, int size) {
    int remainder = size % 40;
    int adjustedSize = size - remainder;
    __m256 Vector_min = _mm256_set1_ps(min);
    for (int i = 0; i < adjustedSize; i += 40) {
        __m256 Vector1 = _mm256_load_ps(&array[i]);
        __m256 Vector2 = _mm256_load_ps(&array[i+8]);
        __m256 Vector3 = _mm256_load_ps(&array[i+16]);
        __m256 Vector4 = _mm256_load_ps(&array[i+24]);
        __m256 Vector5 = _mm256_load_ps(&array[i+32]);
        Vector1 = _mm256_max_ps(Vector1, Vector_min);
        Vector2 = _mm256_max_ps(Vector2, Vector_min);
        Vector3 = _mm256_max_ps(Vector3, Vector_min);
        Vector4 = _mm256_max_ps(Vector4, Vector_min);
        Vector5 = _mm256_max_ps(Vector5, Vector_min);
        _mm256_store_ps(&array[i], Vector1);
        _mm256_store_ps(&array[i+8], Vector2);
        _mm256_store_ps(&array[i+16], Vector3);
        _mm256_store_ps(&array[i+24], Vector4);
        _mm256_store_ps(&array[i+32], Vector5);
    }
    for (int i = adjustedSize; i < size; ++i) {
        if(array[i] < min){
            array[i] = min;
        }
    }
}

void array_size_double(unsigned short* input, float* output, int width, int height){
    for (int y=0;y<height/2;y++) {
        for (int x=0;x<width/2;x++) {
            output[(y*2)*width + x*2] = input[y*width/2 + x];
            output[(y*2)*width + x*2 + 1] = input[y*width/2 + x];
            output[(y*2+1)*width + x*2] = input[y*width/2 + x];
            output[(y*2+1)*width + x*2 + 1] = input[y*width/2 + x];
        }
    }
}

float PQ_EOTF(float V_PQ){
    if(V_PQ==0){
        return 0;
    }else{
        return pow(-((128 * pow(V_PQ, 0.0126833135) - 107) / (2392 * pow(V_PQ, 0.0126833135) - 2413)), 6.2773946360);
    }
}

float PQ_OETF(float L){
    return pow((0.8359375 + 18.8515625 * pow(L, 0.1593017578125)) / (1 + 18.6875 * pow(L, 0.1593017578125)), 78.84375);
}

float sqrt_to_PQ(float sqrt_L){
    return pow((0.8359375 + 18.8515625 * pow(sqrt_L, 0.3186035156)) / (1 + 18.6875 * pow(sqrt_L, 0.3186035156)), 78.84375);
}

void process_frame(std::vector<uint16_t>& frame, int width, int height) {
    // 示例处理：降低亮度，将每个像素值乘以 0.5
    for (size_t i = 0; i < frame.size(); ++i) {
        frame[i] = static_cast<uint16_t>(frame[i] * 0.5);
    }
}


using namespace std;

int main(int argc, char *argv[]){
    string input_file_path;
    string output_file_path;
    int width, height;
    int min = 512, max = 512 ,frame_min, frame_max;
    float gain;
    if (argc == 4) {
        //input_file_path = argv[1];
        //output_file_path = argv[2];
        width = stoi(argv[1]);
        height = stoi(argv[2]);
        gain = stof(argv[3]);
        //cout << "input:" << input_file_path << ", output:" << output_file_path << endl;
        cout << "width:" << width << ", height:" << height << ", gain:" << gain << endl;
    } else {
        std::cerr << "Usage: " << argv[0] << "width height brightness_gain" << std::endl;
        //this_thread::sleep_for(chrono::seconds(10));
        return 1;
    }

    float* PQ_to_linear = new float[65536];
    float* linear_to_PQ = new float[65536];
    float* sqrt_to_PQ_table = new float[65536];

    unsigned short* PQ_gain_table_16bit = new unsigned short[65535];
    //cout<<"step 1" << endl;
    for (int i = 0; i < 65536; ++i) {
        PQ_to_linear[i] = static_cast<float>(PQ_EOTF(static_cast<float>(i)/65535));
    }
    for (int i = 0; i < 65536; ++i) {
        linear_to_PQ[i] = static_cast<float>(PQ_OETF(static_cast<float>(i)/65535));
    }
    for (int i = 0; i < 65536; ++i) {
        sqrt_to_PQ_table[i] = static_cast<float>(sqrt_to_PQ(static_cast<float>(i)/65535));
    }
    float L, V_PQ;
    for (int i = 0; i < 65535; ++i) {
        L = PQ_EOTF(float(i)/65535);
        L *= gain;
        if (L>1){
            L = 1;
        }
        PQ_gain_table_16bit[i] = static_cast<unsigned short>(PQ_OETF(L)*65535);
    }
    for (int i = 0; i < 65535; i+=256) {
        cout<<PQ_gain_table_16bit[i] << endl;
    }
    //cout<<"step 2" << endl;

    int frame_size = width * height * 3;  // RGB48 = 3 通道，每通道 16-bit (2 bytes)
    int frame_index = 0;

    /*PipeProcessor processor(width, height);

    while (processor.processFrame()) {
        // 持續處理
    }*/
    // 用于存储一帧数据的缓冲区
    std::vector<uint16_t> frame_buffer(frame_size);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "start" << endl;
    /*while(true){
        while (std::cin.read(reinterpret_cast<char*>(frame_buffer.data()), frame_size * sizeof(uint16_t))) {
            // 输出当前帧的第一个 uint16_t 值
            uint16_t* data_as_uint16 = reinterpret_cast<uint16_t*>(frame_buffer.data());
            std::cout << "Frame " << frame_index << ": First uint16_t value = " << data_as_uint16[0] << std::endl;
            frame_index++;

            // 如果读取数据不足一帧，检测并停止
            if (std::cin.gcount() < static_cast<std::streamsize>(frame_size * sizeof(uint16_t))) {
                std::cerr << "Error: Incomplete frame detected!" << std::endl;
                break;
            }
        }
    }*/


    while (std::cin.read(reinterpret_cast<char*>(frame_buffer.data()), frame_size * sizeof(uint16_t))) {
        cout << frame_index << endl;
        frame_index++;
        if (std::cin.gcount() < static_cast<std::streamsize>(frame_size * sizeof(uint16_t))) {
            std::cerr << "Error: Incomplete frame detected!\n";
            break;
        }
        // 处理单帧
        //process_frame(frame_buffer, width, height);
        uint16_t* data_as_uint16 = reinterpret_cast<uint16_t*>(frame_buffer.data());
        cout<< data_as_uint16[0] << endl;
        // 将处理后的帧写回 stdout
        std::cout.write(reinterpret_cast<char*>(frame_buffer.data()), frame_size * sizeof(uint16_t));
        // 检查写入是否成功
        if (!std::cout) {
            std::cerr << "Error: Failed to write frame to stdout!\n";
            break;
        }
    }
    cout << "end" << endl;
    // 检查是否由于流中断退出
    if (!std::cin.eof()) {
        std::cerr << "Error: Unexpected input stream error!\n";
    }

    return 0;
}
