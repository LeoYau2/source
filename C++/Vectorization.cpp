//g++ Vectorization.cpp -o Vectorization.exe -std=c++11 -O3 -march=native -fopenmp
#include <stdio.h>
#include <immintrin.h>
#include <iostream>
#include <algorithm> // 包含 std::copy 函数
#include <chrono>
#include <cstdlib>
#include <thread>
#include <omp.h>
//#include <math.h>
#include <opencv2/opencv.hpp>
//#include <opencv2/imgcodecs.hpp>
//#include "opencv2/core.hpp"
//#include "opencv2/core/version.hpp"
//#include <C:\opencv\build\include\opencv2\core.hpp>
//#include "opencv2/highgui.hpp"

using namespace std;
//using namespace cv;

void array_scalar_multiply(float* array, float scalar, int size) {
    // Make sure the size is a multiple of 8 (32-bit floats in a 256-bit AVX register)
    int remainder = size % 8;
    int adjustedSize = size - remainder;
    // Load the scalar into a vector register
    __m256 scalarVector = _mm256_set1_ps(scalar);
    for (int i = 0; i < adjustedSize; i += 8) {
        // Load 8 floats from the array into a vector register
        __m256 dataVector = _mm256_loadu_ps(&array[i]);
        // Multiply each element in the vector by the scalar
        __m256 resultVector = _mm256_mul_ps(dataVector, scalarVector);
        // Store the result back to the array
        _mm256_storeu_ps(&array[i], resultVector);
    }
    // Handle any remaining elements
    for (int i = adjustedSize; i < size; ++i) {
        array[i] *= scalar;
    }
}

int sum = 0;
//__attribute__ ((optimize("O0")))
void array_multiply(float* outarray, float* array1, float* array2, int size) {
    // Make sure the size is a multiple of 8 (32-bit floats in a 256-bit AVX register)
    int remainder = size % 8;
    int adjustedSize = size - remainder;
    for (int i = 0; i < adjustedSize; i += 8) {
        // Load 8 floats from each array into vector registers
        __m256 vector1 = _mm256_loadu_ps(&array1[i]);
        __m256 vector2 = _mm256_loadu_ps(&array2[i]);
        // Multiply each element in the vectors
        __m256 resultVector = _mm256_mul_ps(vector1, vector2);
        // Store the result back to one of the arrays (you can choose either array1 or array2)
        _mm256_storeu_ps(&outarray[i], resultVector);
        sum += 1;
    }
    // Handle any remaining elements
    for (int i = adjustedSize; i < size; ++i) {
        outarray[i] = array1[i] * array2[i];
    }
}

void array_multiply_modify(float* array1, float* array2, int size) {
    // Make sure the size is a multiple of 8 (32-bit floats in a 256-bit AVX register)
    int remainder = size % 8;
    int adjustedSize = size - remainder;
    for (int i = 0; i < adjustedSize; i += 8) {
        // Load 8 floats from each array into vector registers
        __m256 vector1 = _mm256_loadu_ps(&array1[i]);
        __m256 vector2 = _mm256_loadu_ps(&array2[i]);
        // Multiply each element in the vectors
        __m256 resultVector = _mm256_mul_ps(vector1, vector2);
        // Store the result back to one of the arrays (you can choose either array1 or array2)
        _mm256_storeu_ps(&array1[i], resultVector);
    }
    // Handle any remaining elements
    for (int i = adjustedSize; i < size; ++i) {
        array1[i] *= array2[i];
    }
}

void array_multiply_16(float* outarray, float* array1, float* array2, int size) {
    int remainder = size % 16;
    int adjustedSize = size - remainder;
    for (int i = 0; i < adjustedSize; i += 16) {
        __m256 vector1_1 = _mm256_loadu_ps(&array1[i]);
        __m256 vector1_2 = _mm256_loadu_ps(&array1[i + 8]);
        __m256 vector2_1 = _mm256_loadu_ps(&array2[i]);
        __m256 vector2_2 = _mm256_loadu_ps(&array2[i + 8]);
        vector1_1 = _mm256_mul_ps(vector1_1, vector2_1);
        vector1_2 = _mm256_mul_ps(vector1_2, vector2_2);
        _mm256_storeu_ps(&outarray[i], vector1_1);
        _mm256_storeu_ps(&outarray[i + 8], vector1_2);
    }
    for (int i = adjustedSize; i < size; ++i) {
        outarray[i] = array1[i] * array2[i];
    }
}

void array_multiply_24(float* outarray, float* array1, float* array2, int size) {
    int remainder = size % 24;
    int adjustedSize = size - remainder;
    for (int i = 0; i < adjustedSize; i += 24) {
        __m256 vector1_1 = _mm256_loadu_ps(&array1[i]);
        __m256 vector1_2 = _mm256_loadu_ps(&array1[i + 8]);
        __m256 vector1_3 = _mm256_loadu_ps(&array1[i + 16]);
        __m256 vector2_1 = _mm256_loadu_ps(&array2[i]);
        __m256 vector2_2 = _mm256_loadu_ps(&array2[i + 8]);
        __m256 vector2_3 = _mm256_loadu_ps(&array2[i + 16]);
        vector1_1 = _mm256_mul_ps(vector1_1, vector2_1);
        vector1_2 = _mm256_mul_ps(vector1_2, vector2_2);
        vector1_3 = _mm256_mul_ps(vector1_3, vector2_3);
        _mm256_storeu_ps(&outarray[i], vector1_1);
        _mm256_storeu_ps(&outarray[i + 8], vector1_2);
        _mm256_storeu_ps(&outarray[i + 16], vector1_3);
    }
    for (int i = adjustedSize; i < size; ++i) {
        outarray[i] = array1[i] * array2[i];
    }
}

void array_multiply_32(float* outarray, float* array1, float* array2, int size) {
    int remainder = size % 32;
    int adjustedSize = size - remainder;
    for (int i = 0; i < adjustedSize; i += 32) {
        __m256 vector1_1 = _mm256_loadu_ps(&array1[i]);
        __m256 vector1_2 = _mm256_loadu_ps(&array1[i + 8]);
        __m256 vector1_3 = _mm256_loadu_ps(&array1[i + 16]);
        __m256 vector1_4 = _mm256_loadu_ps(&array1[i + 24]);
        __m256 vector2_1 = _mm256_loadu_ps(&array2[i]);
        __m256 vector2_2 = _mm256_loadu_ps(&array2[i + 8]);
        __m256 vector2_3 = _mm256_loadu_ps(&array2[i + 16]);
        __m256 vector2_4 = _mm256_loadu_ps(&array2[i + 24]);
        vector1_1 = _mm256_mul_ps(vector1_1, vector2_1);
        vector1_2 = _mm256_mul_ps(vector1_2, vector2_2);
        vector1_3 = _mm256_mul_ps(vector1_3, vector2_3);
        vector1_4 = _mm256_mul_ps(vector1_4, vector2_4);
        _mm256_storeu_ps(&outarray[i], vector1_1);
        _mm256_storeu_ps(&outarray[i + 8], vector1_2);
        _mm256_storeu_ps(&outarray[i + 16], vector1_3);
        _mm256_storeu_ps(&outarray[i + 24], vector1_4);
    }
    for (int i = adjustedSize; i < size; ++i) {
        outarray[i] = array1[i] * array2[i];
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

void array_multiply_48(float* outarray, float* array1, float* array2, int size) {
    int remainder = size % 48;
    int adjustedSize = size - remainder;
    for (int i = 0; i < adjustedSize; i += 48) {
        __m256 vector1_1 = _mm256_loadu_ps(&array1[i]);
        __m256 vector1_2 = _mm256_loadu_ps(&array1[i + 8]);
        __m256 vector1_3 = _mm256_loadu_ps(&array1[i + 16]);
        __m256 vector1_4 = _mm256_loadu_ps(&array1[i + 24]);
        __m256 vector1_5 = _mm256_loadu_ps(&array1[i + 32]);
        __m256 vector1_6 = _mm256_loadu_ps(&array1[i + 40]);
        __m256 vector2_1 = _mm256_loadu_ps(&array2[i]);
        __m256 vector2_2 = _mm256_loadu_ps(&array2[i + 8]);
        __m256 vector2_3 = _mm256_loadu_ps(&array2[i + 16]);
        __m256 vector2_4 = _mm256_loadu_ps(&array2[i + 24]);
        __m256 vector2_5 = _mm256_loadu_ps(&array2[i + 32]);
        __m256 vector2_6 = _mm256_loadu_ps(&array2[i + 40]);
        vector1_1 = _mm256_mul_ps(vector1_1, vector2_1);
        vector1_2 = _mm256_mul_ps(vector1_2, vector2_2);
        vector1_3 = _mm256_mul_ps(vector1_3, vector2_3);
        vector1_4 = _mm256_mul_ps(vector1_4, vector2_4);
        vector1_5 = _mm256_mul_ps(vector1_5, vector2_5);
        vector1_6 = _mm256_mul_ps(vector1_6, vector2_6);
        _mm256_storeu_ps(&outarray[i], vector1_1);
        _mm256_storeu_ps(&outarray[i + 8], vector1_2);
        _mm256_storeu_ps(&outarray[i + 16], vector1_3);
        _mm256_storeu_ps(&outarray[i + 24], vector1_4);
        _mm256_storeu_ps(&outarray[i + 32], vector1_5);
        _mm256_storeu_ps(&outarray[i + 40], vector1_6);
    }
    for (int i = adjustedSize; i < size; ++i) {
        outarray[i] = array1[i] * array2[i];
    }
}

void array_multiply_56(float* outarray, float* array1, float* array2, int size) {
    int remainder = size % 56;
    int adjustedSize = size - remainder;
    for (int i = 0; i < adjustedSize; i += 56) {
        __m256 vector1_1 = _mm256_loadu_ps(&array1[i]);
        __m256 vector1_2 = _mm256_loadu_ps(&array1[i + 8]);
        __m256 vector1_3 = _mm256_loadu_ps(&array1[i + 16]);
        __m256 vector1_4 = _mm256_loadu_ps(&array1[i + 24]);
        __m256 vector1_5 = _mm256_loadu_ps(&array1[i + 32]);
        __m256 vector1_6 = _mm256_loadu_ps(&array1[i + 40]);
        __m256 vector1_7 = _mm256_loadu_ps(&array1[i + 48]);
        __m256 vector2_1 = _mm256_loadu_ps(&array2[i]);
        __m256 vector2_2 = _mm256_loadu_ps(&array2[i + 8]);
        __m256 vector2_3 = _mm256_loadu_ps(&array2[i + 16]);
        __m256 vector2_4 = _mm256_loadu_ps(&array2[i + 24]);
        __m256 vector2_5 = _mm256_loadu_ps(&array2[i + 32]);
        __m256 vector2_6 = _mm256_loadu_ps(&array2[i + 40]);
        __m256 vector2_7 = _mm256_loadu_ps(&array2[i + 48]);
        vector1_1 = _mm256_mul_ps(vector1_1, vector2_1);
        vector1_2 = _mm256_mul_ps(vector1_2, vector2_2);
        vector1_3 = _mm256_mul_ps(vector1_3, vector2_3);
        vector1_4 = _mm256_mul_ps(vector1_4, vector2_4);
        vector1_5 = _mm256_mul_ps(vector1_5, vector2_5);
        vector1_6 = _mm256_mul_ps(vector1_6, vector2_6);
        vector1_7 = _mm256_mul_ps(vector1_7, vector2_7);
        _mm256_storeu_ps(&outarray[i], vector1_1);
        _mm256_storeu_ps(&outarray[i + 8], vector1_2);
        _mm256_storeu_ps(&outarray[i + 16], vector1_3);
        _mm256_storeu_ps(&outarray[i + 24], vector1_4);
        _mm256_storeu_ps(&outarray[i + 32], vector1_5);
        _mm256_storeu_ps(&outarray[i + 40], vector1_6);
        _mm256_storeu_ps(&outarray[i + 48], vector1_7);
    }
    for (int i = adjustedSize; i < size; ++i) {
        outarray[i] = array1[i] * array2[i];
    }
}

void array_multiply_64(float* outarray, float* array1, float* array2, int size) {
    // Make sure the size is a multiple of 64
    int remainder = size % 64;
    int adjustedSize = size - remainder;
    for (int i = 0; i < adjustedSize; i += 64) {
        // Load 64 floats from each array into vector registers
        __m256 vector1_1 = _mm256_loadu_ps(&array1[i]);
        __m256 vector1_2 = _mm256_loadu_ps(&array1[i + 8]);
        __m256 vector1_3 = _mm256_loadu_ps(&array1[i + 16]);
        __m256 vector1_4 = _mm256_loadu_ps(&array1[i + 24]);
        __m256 vector1_5 = _mm256_loadu_ps(&array1[i + 32]);
        __m256 vector1_6 = _mm256_loadu_ps(&array1[i + 40]);
        __m256 vector1_7 = _mm256_loadu_ps(&array1[i + 48]);
        __m256 vector1_8 = _mm256_loadu_ps(&array1[i + 56]);
        __m256 vector2_1 = _mm256_loadu_ps(&array2[i]);
        __m256 vector2_2 = _mm256_loadu_ps(&array2[i + 8]);
        __m256 vector2_3 = _mm256_loadu_ps(&array2[i + 16]);
        __m256 vector2_4 = _mm256_loadu_ps(&array2[i + 24]);
        __m256 vector2_5 = _mm256_loadu_ps(&array2[i + 32]);
        __m256 vector2_6 = _mm256_loadu_ps(&array2[i + 40]);
        __m256 vector2_7 = _mm256_loadu_ps(&array2[i + 48]);
        __m256 vector2_8 = _mm256_loadu_ps(&array2[i + 56]);
        // Multiply each element in the vectors
        vector1_1 = _mm256_mul_ps(vector1_1, vector2_1);
        vector1_2 = _mm256_mul_ps(vector1_2, vector2_2);
        vector1_3 = _mm256_mul_ps(vector1_3, vector2_3);
        vector1_4 = _mm256_mul_ps(vector1_4, vector2_4);
        vector1_5 = _mm256_mul_ps(vector1_5, vector2_5);
        vector1_6 = _mm256_mul_ps(vector1_6, vector2_6);
        vector1_7 = _mm256_mul_ps(vector1_7, vector2_7);
        vector1_8 = _mm256_mul_ps(vector1_8, vector2_8);
        // Store the result back to array1
        _mm256_storeu_ps(&outarray[i], vector1_1);
        _mm256_storeu_ps(&outarray[i + 8], vector1_2);
        _mm256_storeu_ps(&outarray[i + 16], vector1_3);
        _mm256_storeu_ps(&outarray[i + 24], vector1_4);
        _mm256_storeu_ps(&outarray[i + 32], vector1_5);
        _mm256_storeu_ps(&outarray[i + 40], vector1_6);
        _mm256_storeu_ps(&outarray[i + 48], vector1_7);
        _mm256_storeu_ps(&outarray[i + 56], vector1_8);
    }
    // Handle any remaining elements
    for (int i = adjustedSize; i < size; ++i) {
        outarray[i] = array1[i] * array2[i];
    }
}

void array_add(float* array1, float* array2, int size) {
    // Make sure the size is a multiple of 8 (32-bit floats in a 256-bit AVX register)
    int remainder = size % 8;
    int adjustedSize = size - remainder;
    for (int i = 0; i < adjustedSize; i += 8) {
        // Load 8 floats from each array into vector registers
        __m256 vector1 = _mm256_loadu_ps(&array1[i]);
        __m256 vector2 = _mm256_loadu_ps(&array2[i]);
        // Add each element in the vectors
        __m256 resultVector = _mm256_add_ps(vector1, vector2);
        // Store the result back to array1 (you can choose either array1 or array2)
        _mm256_storeu_ps(&array1[i], resultVector);
    }
    // Handle any remaining elements
    for (int i = adjustedSize; i < size; ++i) {
        array1[i] += array2[i];
    }
}

void array_divide(float* array1, float* array2, int size) {
    // Make sure the size is a multiple of 8 (32-bit floats in a 256-bit AVX register)
    int remainder = size % 8;
    int adjustedSize = size - remainder;
    for (int i = 0; i < adjustedSize; i += 8) {
        // Load 8 floats from each array into vector registers
        __m256 vector1 = _mm256_loadu_ps(&array1[i]);
        __m256 vector2 = _mm256_loadu_ps(&array2[i]);
        // Divide each element in the vectors
        __m256 resultVector = _mm256_div_ps(vector1, vector2);
        // Store the result back to array1 (you can choose either array1 or array2)
        _mm256_storeu_ps(&array1[i], resultVector);
    }
    // Handle any remaining elements
    for (int i = adjustedSize; i < size; ++i) {
        array1[i] /= array2[i];
    }
}

void array_multiply_no_avx(float* outarray, float* array1, float* array2, int size) {
    for (int i = 0; i < size; ++i) {
        outarray[i] = array1[i] * array2[i];
    }
}

void array_multiply_multithread(float* outarray, float* array1, float* array2, int size, int num_threads) {
    thread threads[num_threads];

    for (int i = 0; i < num_threads-1; ++i) {
        threads[i] = thread(array_multiply_32, outarray+size/num_threads*i, array1+size/num_threads*i, array2+size/num_threads*i, size/num_threads);
    }
    threads[num_threads-1] = thread(array_multiply_32, outarray+size/num_threads*(num_threads-1), array1+size/num_threads*(num_threads-1), array2+size/num_threads*(num_threads-1), size-size/num_threads*(num_threads-1));
    for (int i = 0; i < num_threads; ++i) {
        threads[i].join();
    }
}

void array_multiply_multithread_2(float* outarray, float* array1, float* array2, int size, int floats_num) {
    thread threads[size/floats_num+1];
    for (int i = 0; i < size/floats_num; i++) {
        threads[i] = thread(array_multiply_32, outarray+i*floats_num, array1+i*floats_num, array2+i*floats_num, floats_num);
    }
    threads[size/floats_num] = thread(array_multiply_32, outarray+size/floats_num*floats_num, array1+size/floats_num*floats_num, array2+size/floats_num*floats_num, size-size/floats_num*floats_num);
    for (int i = 0; i < size/floats_num+1; ++i) {
        threads[i].join();
    }
    //array_multiply_32(outarray+size/floats_num*floats_num, array1+size/floats_num*floats_num, array2+size/floats_num*floats_num, size-size/floats_num*floats_num);
}

int main() {
    const int width = 1000;
    const int height = 1000;
    const int channels = 3;
    const int arraySize = width * height * channels;
    //float inputArray[width][height][channels];
    //float array2[width][height][channels];
    //float outarray[width][height][channels];
    float* array3 = new float[arraySize];
    float* array4 = new float[arraySize];
    float* array5 = new float[arraySize];
    int num_threads = 8;

    srand(time(NULL));
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            for (int c = 0; c < channels; ++c) {
                array3[3*height*x + 3*y + c] = 3*height*x + 3*y + c;
                array4[3*height*x + 3*y + c] = rand();
                //array4[3*height*x + 3*y + c] = 3*height*x + 3*y + c;
            }
        }
    }
    //float* inputArrayptr = &inputArray[0][0][0];
    //float* array2ptr = &array2[0][0][0];
    //float* outarrayptr = &outarray[0][0][0];

    //array_scalar_multiply(floatArray, 10.0f, arraySize);
    //std::copy(inputArray.begin(), inputArray.end(), array2.begin());
    auto start_time = chrono::high_resolution_clock::now();
    for(int i=0;i<10000;i++)
    {
        //array_multiply_modify(array5, array4, arraySize);
    }
    auto stop_time = chrono::high_resolution_clock::now();
    auto durationMicro = chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    //auto durationNano = chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time);

    cout << "array_multiply_modify:   " << durationMicro.count() << " microseconds" << endl;
    //cout << durationNano.count() << " nanoseconds" << endl;

    start_time = chrono::high_resolution_clock::now();
    for(int i=0;i<10000;i++)
    {
        array_multiply_no_avx(array5, array3, array4, arraySize);
    }
    stop_time = chrono::high_resolution_clock::now();
    durationMicro = chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    cout << "array_multiply_no_avx:   " << durationMicro.count() << " microseconds" << endl;

    start_time = chrono::high_resolution_clock::now();
    for(int i=0;i<10000;i++)
    {
        //array_multiply(array5, array3, array4, arraySize);
    }
    stop_time = chrono::high_resolution_clock::now();
    durationMicro = chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    cout << "array_multiply:          " << durationMicro.count() << " microseconds" << endl;

    start_time = chrono::high_resolution_clock::now();
    for(int i=0;i<10000;i++)
    {
        //array_multiply_16(array5, array3, array4, arraySize);
    }
    stop_time = chrono::high_resolution_clock::now();
    durationMicro = chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    cout << "array_multiply_16:       " << durationMicro.count() << " microseconds" << endl;

    start_time = chrono::high_resolution_clock::now();
    for(int i=0;i<10000;i++)
    {
        //array_multiply_24(array5, array3, array4, arraySize);
    }
    stop_time = chrono::high_resolution_clock::now();
    durationMicro = chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    cout << "array_multiply_24:       " << durationMicro.count() << " microseconds" << endl;

    start_time = chrono::high_resolution_clock::now();
    //#pragma omp parallel for num_threads(4)
    for(int i=0;i<1000;i++)
    {
        //array_multiply_32(array5, array3, array4, arraySize);
    }
    stop_time = chrono::high_resolution_clock::now();
    durationMicro = chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    cout << "array_multiply_32:       " << durationMicro.count() << " microseconds" << endl;

    start_time = chrono::high_resolution_clock::now();
    for(int i=0;i<10000;i++)
    {
        //array_multiply_40(array5, array3, array4, arraySize);
    }
    stop_time = chrono::high_resolution_clock::now();
    durationMicro = chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    cout << "array_multiply_40:       " << durationMicro.count() << " microseconds" << endl;

    for(int t=num_threads;t<num_threads+1;t++){ //發生False Sharing，慢了20倍以上，試過1到8線程，除了1線程慢了不到10%，其他的時間都差不多
        start_time = chrono::high_resolution_clock::now();
        for(int i=0;i<1000;i++)
        {
            array_multiply_multithread(array5, array3, array4, arraySize, 4);
        }
        stop_time = chrono::high_resolution_clock::now();
        durationMicro = chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
        cout << "array_multiply " << t << " threads:" << durationMicro.count() << " microseconds" << endl;
    }

    for(int t=num_threads;t<num_threads+1;t++){ //cache line 64bytes
        start_time = chrono::high_resolution_clock::now();
        for(int i=0;i<1000;i++)
        {
            array_multiply_multithread_2(array5, array3, array4, arraySize, 524288);
        }
        stop_time = chrono::high_resolution_clock::now();
        durationMicro = chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
        cout << "array_multiply " << t << " threads_2:" << durationMicro.count() << " microseconds" << endl;
    }


    start_time = chrono::high_resolution_clock::now();
    for(int i=0;i<10000;i++)
    {
        //array_multiply_48(array5, array3, array4, arraySize);
    }
    stop_time = chrono::high_resolution_clock::now();
    durationMicro = chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    cout << "array_multiply_48:       " << durationMicro.count() << " microseconds" << endl;

    start_time = chrono::high_resolution_clock::now();
    for(int i=0;i<10000;i++)
    {
        //array_multiply_56(array5, array3, array4, arraySize);
    }
    stop_time = chrono::high_resolution_clock::now();
    durationMicro = chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    cout << "array_multiply_56:       " << durationMicro.count() << " microseconds" << endl;

    start_time = chrono::high_resolution_clock::now();
    for(int i=0;i<10000;i++)
    {
        //array_multiply_64(array5, array3, array4, arraySize);
    }
    stop_time = chrono::high_resolution_clock::now();
    durationMicro = chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time);
    cout << "array_multiply_64:       " << durationMicro.count() << " microseconds" << endl;

    //array_add(inputArrayptr, array2ptr, arraySize);

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 4; y++) {
            //printf("%f %f %f\n",array5[3*height*x + 3*y],array5[3*height*x + 3*y+1],array5[3*height*x + 3*y+2]);
        }
    }

    //system("pause");

    delete[] array3;
    delete[] array4;
    delete[] array5;
    system("pause");
    return 0;
}
