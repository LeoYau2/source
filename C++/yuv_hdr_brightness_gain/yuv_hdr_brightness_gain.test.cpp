//g++ -std=c++17 .\yuv_hdr_brightness_gain.cpp -o .\yuv_hdr_brightness_gain.exe
#include <stdio.h>
#include <immintrin.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <vector>
#include <filesystem>
#include <cmath>
//#include <Eigen/Dense>
#include <string>
#include <cstring>
#include <dirent.h>
#include <sstream>
#include <iomanip>


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

void array_size_double(unsigned short* input, float* output, int width, int height){
    for (int y=0;y<height/2;y++) {
        for (int x=0;x<width/2;x++) {
            output[(y*2)*width + x*2] = input[y*width/2 + x];
            output[(y*2)*width + x*2 + 1] = input[y*width/2 + x];
            output[(y*2+1)*width + x*2] = input[y*width/2 + x];
            output[(y*2+1)*width + x*2 + 1] = input[y*width/2 + x];
        }
    }
    /*for (int y=0;y<height/2;y++) {
        int x = 0;
        for (;x<width/2;x+=8) {
            __m256i input_vec = _mm256_loadu_si256((__m256i*)&input[y*width/2 + x]);
            __m256i input_low = _mm256_unpacklo_epi16(input_vec, _mm256_setzero_si256());
            __m256i input_high = _mm256_unpackhi_epi16(input_vec, _mm256_setzero_si256());

            __m256 float_vec_low = _mm256_cvtepi32_ps(input_low);
            __m256 float_vec_high = _mm256_cvtepi32_ps(input_high);
            //__m256 float_vec = _mm256_castsi256_ps(input_vec);
            _mm256_storeu_ps(&output[(y*2)*width + x*2], float_vec);
            _mm256_storeu_ps(&output[(y*2)*width + x*2 + 1], float_vec);
            _mm256_storeu_ps(&output[(y*2+1)*width + x*2], float_vec);
            _mm256_storeu_ps(&output[(y*2+1)*width + x*2 + 1], float_vec);
        }
        for (;x<width/2;x++) {
            output[(y*2)*width + x*2] = float(input[y*width/2 + x]);
            output[(y*2)*width + x*2 + 1] = float(input[y*width/2 + x]);
            output[(y*2+1)*width + x*2] = float(input[y*width/2 + x]);
            output[(y*2+1)*width + x*2 + 1] = float(input[y*width/2 + x]);
        }
    }*/
    /*int remainder = (width/2)%8;
    int adjustedSize = width/2 - remainder;
    int x, y;
    __m128i zero = _mm_setzero_si128();
    for (y = 0; y < height/2; y++) {
        for (x = 0; x < width/2; x+=8) {
        __m128i in = _mm_loadu_si128((__m128i*)(input + y * width + x));
        __m128i lo = _mm_unpacklo_epi16(in, zero);
        __m128i hi = _mm_unpackhi_epi16(in, zero);
        __m128 out1 = _mm_cvtepi32_ps(lo);
        __m128 out2 = _mm_cvtepi32_ps(hi);
        _mm_storeu_ps(output + y*width + x*2, out1);
        _mm_storeu_ps(output + y*width + x*2 + 4, out2);
        _mm_storeu_ps(output + (y*2+1)*width/2 + x*2, out1);
        _mm_storeu_ps(output + (y*2+1)*width/2 + x*2 + 4, out2);
        }
        for (x = width/2 - remainder; x < width/2; x++) {
            output[(y*2)*width + x*2] = float(input[y*width/2 + x]);
            output[(y*2)*width + x*2 + 1] = float(input[y*width/2 + x]);
            output[(y*2+1)*width + x*2] = float(input[y*width/2 + x]);
            output[(y*2+1)*width + x*2 + 1] = float(input[y*width/2 + x]);
        }
    }*/
}
/*
void array_size_supersample_half(float* input, unsigned short* output, int width, int height){
    for (int y=0;y<height/2;y++) {
        for (int x=0;x<width/2;x++) {
            output[y*width/2 + x] = (input[y*width + x*2]+input[y*width + x*2 + 1]+input[(y+1)*width + x*2]+input[(y+1)*width + x*2 + 1])/4
        }
    }
}*/

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

std::string intToFourDigitString(int number) {
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << number;
    return ss.str();
}

using namespace std;

int main(int argc, char *argv[]){
    string input_file_path;
    string output_file_path;
    int width, height;
    int min = 512, max = 512 ,frame_min, frame_max;
    float gain;
    if (argc == 6) {
        input_file_path = argv[1];
        output_file_path = argv[2];
        width = stoi(argv[3]);
        height = stoi(argv[4]);
        gain = stof(argv[5]);
        cout << "input:" << input_file_path << ", output:" << output_file_path << endl;
        cout << "width:" << width << ", height:" << height << ", gain:" << gain << endl;
    } else {
        std::cerr << "Usage: " << argv[0] << " input_path output_path width height brightness_gain" << std::endl;
        //this_thread::sleep_for(chrono::seconds(10));
        return 1;
    }
    //int del = 1;
    bool found_file;
    int frame_index = 0, total_frame_count, yuv_size = width*height*1.5, y_size = width*height, u_size = width*height/4, v_size = width*height/4;
    //string input_file_path = "X:\\yuv\\002.yuv";
    //string input_path = "X:\\yuv\\input\\";
    //string output_file_path = "C:\\Users\\Leo\\Sync\\source\\C++\\yuv_hdr_brightness_gain\\output.yuv";
    //string output_path = "X:\\yuv\\gained\\";
    //string filename;
    //string na;
        //string na = intToFourDigitString(file_count);
        //input_file_path = input_path + na + ".yuv";
        //output_file_path = output_path + na + ".yuv";

    ifstream input_file(input_file_path, ios::binary);
    if (!input_file.is_open()) {
        found_file = false;
        cerr << strerror(errno) << endl;
        //this_thread::sleep_for(chrono::seconds(1));
    }else{
        found_file = true;
    }

        /*if (yuvFiles.empty()) {
            std::cout << "在指定路徑中未找到任何 YUV 檔案。" << std::endl;
            found_file = false;
            cout << "waiting" << endl;
        } else {
            std::cout << "找到以下 YUV 檔案：" << std::endl;
            for (const auto& fileName : yuvFiles) {
                std::cout << fileName << std::endl;
                found_file = true;
            }
        }*/

    if(found_file){
        /*
        input_file.close();
        int result_1 = std::remove(input_file_path.c_str());
        if (result_1 == 0) {
            cout << input_file_path << "deleted" << endl;
        } else {
            cout << "can't delete " << input_file_path << endl;
        }*/
        //cout << "Processing "<< input_file_path << endl;
        ofstream output_file(output_file_path, ios::binary);
        if (!output_file.is_open()) {
            cerr << "Failed to open the file for writing." << endl;
            //this_thread::sleep_for(chrono::seconds(2));
            return 1;
        }
        ifstream in(input_file_path, ifstream::ate | ifstream::binary);
        streampos filesize = in.tellg();
        cout << "File size: " << filesize << " bytes" << endl;
        cout << filesize/yuv_size/sizeof(unsigned short) << " frames" << endl;
        total_frame_count = filesize/yuv_size/sizeof(unsigned short);
        unsigned short* Y = new unsigned short[y_size];
        unsigned short* U = new unsigned short[u_size];
        unsigned short* V = new unsigned short[v_size];
        float* Y_float = new float[y_size];
        float* U_float = new float[u_size];
        float* V_float = new float[v_size];
        float* U_full = new float[y_size];
        float* V_full = new float[y_size];
        float* R = new float[y_size];
        float* G = new float[y_size];
        float* B = new float[y_size];
        float* temp_1 = new float[y_size];
        float* temp_2 = new float[y_size];
        float* temp_3 = new float[y_size];
        float* temp_4 = new float[y_size];
        float* temp_5 = new float[y_size];
        float* temp_6 = new float[y_size];
        float* PQ_to_linear = new float[65536];
        float* linear_to_PQ = new float[65536];
        float* sqrt_to_PQ_table = new float[65536];
        float* PQ_gain_table_10bit = new float[1024];
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
        for (int i = 0; i < 1024; ++i) {
            L = PQ_EOTF(float(i)/1023);
            L *= gain;
            PQ_gain_table_10bit[i] = PQ_OETF(L);
        }
        //cout<<"step 2" << endl;

        int remainder = y_size % 8;
        int adjustedSize = y_size - remainder;
        auto start_time = chrono::high_resolution_clock::now();
        for(int i=0;i<total_frame_count;i++){
            auto section_time = chrono::high_resolution_clock::now();
            input_file.seekg(static_cast<std::streamoff>(yuv_size) * static_cast<std::streamoff>(frame_index) * sizeof(unsigned short), ios::beg);
            input_file.read(reinterpret_cast<char*>(Y), y_size * sizeof(unsigned short));

            input_file.seekg((static_cast<std::streamoff>(yuv_size) * static_cast<std::streamoff>(frame_index)+y_size) * sizeof(unsigned short), ios::beg);
            input_file.read(reinterpret_cast<char*>(U), u_size * sizeof(unsigned short));

            input_file.seekg((static_cast<std::streamoff>(yuv_size) * static_cast<std::streamoff>(frame_index)+static_cast<std::streamoff>(y_size)+static_cast<std::streamoff>(u_size)) * sizeof(unsigned short), ios::beg);
            input_file.read(reinterpret_cast<char*>(V), v_size * sizeof(unsigned short));
            //cout<<yuv_size<<endl;
            //cout<<"seek:" << yuv_size*frame_index*sizeof(unsigned short) << ", " << (yuv_size*frame_index+y_size)*2 << ", " << (yuv_size*frame_index+y_size+u_size)* sizeof(unsigned short) << endl;
            //cout<<"step 3" << endl;

            array_size_double(U,U_full,width,height);//U放大兩倍
            array_size_double(V,V_full,width,height);//V放大兩倍
            //cout<<"step 4" << endl;
            /*for (int i = 539; i < 540; ++i) {
                for (int j = 0; j < 960; ++j) {
                    cout << U_full[i*960+j] << " ";
                }
                cout << endl;
            }*/
            for (size_t i = 0; i < y_size; ++i) {
                Y_float[i] = static_cast<float>(Y[i]);
            }
            /*frame_max = 512;
            frame_min = 512;
            for (size_t i = 0; i < y_size; ++i) {
                Y_float[i] = static_cast<float>(Y[i]);
                if (Y[i] > frame_max) {
                    frame_max = Y[i];
                }

                if (Y[i] < frame_min) {
                    frame_min = Y[i];
                }
            }
            if (frame_max > max) {
                max = frame_max;
            }
            if (frame_min < min) {
                min = frame_min;
            }
            cout<<"min:"<<frame_min<<", max:"<<frame_max<<endl;*/
            //cout<<"step 5" << endl;

            //BT.2020 limted range YUV轉RGB
            //R = 1.1643 * (Y-16) + 1.6853 * (V-128)
            //G = 1.1643 * (Y-16) - 0.1881 * (U-128) - 0.6529 * (V-128)
            //B = 1.1643 * (Y-16) + 2.1501 * (U-128)
            //BT.2020 full range YUV轉RGB
            //R = Y + 1.4746 * (V-128)
            //G = Y - 0.1645 * (U-128) - 0.5713 * (V-128)
            //B = Y + 1.8814 * (U-128)

            //array_scalar_add_40(temp_4, Y_float, -64.1882, y_size);
            //array_scalar_multiply_40(temp_1, temp_4, 1.1643, y_size);
            array_scalar_add_40(temp_2, U_full, -512, y_size);
            array_scalar_add_40(temp_3, V_full, -512, y_size);

            array_scalar_multiply_40(temp_4, temp_3, 1.4746, y_size);
            array_add_40(R, Y_float, temp_4, y_size);

            array_scalar_multiply_40(temp_4, temp_2, -0.1645, y_size);
            array_scalar_multiply_40(temp_5, temp_3, -0.5713, y_size);
            array_add_40(temp_6, Y_float, temp_4, y_size);
            array_add_40(G, temp_6, temp_5, y_size);

            array_scalar_multiply_40(temp_4, temp_2, 1.8814, y_size);
            array_add_40(B, Y_float, temp_4, y_size);
            //cout<<"step 6" << endl;
    /*
            for (size_t i = 0; i < y_size; i++) {
                R[i] = PQ_to_linear[(static_cast<unsigned short>(R[i]*64.06158358))];
                G[i] = PQ_to_linear[(static_cast<unsigned short>(G[i]*64.06158358))];
                B[i] = PQ_to_linear[(static_cast<unsigned short>(B[i]*64.06158358))];
            }*/

            for (int i = 0; i < y_size; ++i) {
                if (R[i] > 1023) {
                    cout << "clip R " << R[i]<<",Y:"<<Y[i] << endl;
                    R[i] = 1023;
                }
            }
            for (int i = 0; i < u_size; ++i) {
                if (G[i] > 1023) {
                    cout << "clip G " << G[i]<< endl;
                    G[i] = 1023;
                }
                if (B[i] > 1023) {
                    cout << "clip B " << B[i] << endl;
                    B[i] = 1023;
                }
            }

            for (int i = 0; i < adjustedSize; i+=8) {
                __m256 R_vec = _mm256_loadu_ps(&R[i]);
                __m256 G_vec = _mm256_loadu_ps(&G[i]);
                __m256 B_vec = _mm256_loadu_ps(&B[i]);
                __m256i R_index = _mm256_cvtps_epi32(R_vec);
                __m256i G_index = _mm256_cvtps_epi32(G_vec);
                __m256i B_index = _mm256_cvtps_epi32(B_vec);
                __m256 R_gain = _mm256_i32gather_ps(PQ_gain_table_10bit, R_index, 4);
                __m256 G_gain = _mm256_i32gather_ps(PQ_gain_table_10bit, G_index, 4);
                __m256 B_gain = _mm256_i32gather_ps(PQ_gain_table_10bit, B_index, 4);
                _mm256_storeu_ps(&R[i], R_gain);
                _mm256_storeu_ps(&G[i], G_gain);
                _mm256_storeu_ps(&B[i], B_gain);
            }
            for (int i = adjustedSize; i < y_size; i++) {
                R[i] = PQ_gain_table_10bit[(static_cast<unsigned short>(R[i]))];
                G[i] = PQ_gain_table_10bit[(static_cast<unsigned short>(G[i]))];
                B[i] = PQ_gain_table_10bit[(static_cast<unsigned short>(B[i]))];
            }
            //cout<<"step 7" << endl;
    /*
            __m256 factor = _mm256_set1_ps(64.06158358);
            int remainder = y_size % 8;
            int adjustedSize = y_size - remainder;
            for (int i = 0; i < adjustedSize; i+=8) {
                __m256 R_vec = _mm256_loadu_ps(&R[i]);
                __m256 G_vec = _mm256_loadu_ps(&G[i]);
                __m256 B_vec = _mm256_loadu_ps(&B[i]);
                __m256i R_index = _mm256_cvtps_epi32(_mm256_mul_ps(R_vec, factor));
                __m256i G_index = _mm256_cvtps_epi32(_mm256_mul_ps(G_vec, factor));
                __m256i B_index = _mm256_cvtps_epi32(_mm256_mul_ps(B_vec, factor));
                __m256 R_linear = _mm256_i32gather_ps(PQ_to_linear, R_index, 4);
                __m256 G_linear = _mm256_i32gather_ps(PQ_to_linear, G_index, 4);
                __m256 B_linear = _mm256_i32gather_ps(PQ_to_linear, B_index, 4);
                _mm256_storeu_ps(&R[i], R_linear);
                _mm256_storeu_ps(&G[i], G_linear);
                _mm256_storeu_ps(&B[i], B_linear);
            }
            for (int i = adjustedSize; i < y_size; i++) {
                R[i] = PQ_to_linear[(static_cast<unsigned short>(R[i]*64.06158358))];
                G[i] = PQ_to_linear[(static_cast<unsigned short>(G[i]*64.06158358))];
                B[i] = PQ_to_linear[(static_cast<unsigned short>(B[i]*64.06158358))];
            }

            for (size_t i = 0; i < y_size; i++) {
                if(R[i]>max){
                    max = R[i];
                }
            }
            //cout <<"max1 "<< max << endl;
            max = 0;
            array_scalar_multiply_40(temp_1, R, gain, y_size);
            array_scalar_multiply_40(temp_2, G, gain, y_size);
            array_scalar_multiply_40(temp_3, B, gain, y_size);
            for (size_t i = 0; i < y_size; i++) {
                if(temp_1[i]>max){
                    max = R[i];
                }
            }*/
            //cout <<"max2 "<< max << endl;

    /*
            array_sqrt_40(R, R, y_size);
            array_sqrt_40(G, G, y_size);
            array_sqrt_40(B, B, y_size);
            for(int i=0;i<1;i++){
                cout << R[i] << endl;
            }
            for(int i=0;i<1;i++){
                cout << G[i] << endl;
            }
            for(int i=0;i<1;i++){
                cout << B[i] << endl;
            }

            for (size_t i = 0; i < y_size; i++) {
                R[i] = sqrt_to_PQ_table[(static_cast<unsigned short>(R[i]*65535))];
                G[i] = sqrt_to_PQ_table[(static_cast<unsigned short>(G[i]*65535))];
                B[i] = sqrt_to_PQ_table[(static_cast<unsigned short>(B[i]*65535))];
            }*/
            /*
            for (size_t i = 0; i < y_size; i++) {
                R[i] = linear_to_PQ[(static_cast<unsigned short>(temp_1[i]*65535))];
                G[i] = linear_to_PQ[(static_cast<unsigned short>(temp_2[i]*65535))];
                B[i] = linear_to_PQ[(static_cast<unsigned short>(temp_3[i]*65535))];
            }*/
            /*
            factor = _mm256_set1_ps(65535);
            remainder = y_size % 8;
            adjustedSize = y_size - remainder;
            for (int i = 0; i < adjustedSize; i+=8) {
                __m256 R_vec = _mm256_loadu_ps(&temp_1[i]);
                __m256 G_vec = _mm256_loadu_ps(&temp_2[i]);
                __m256 B_vec = _mm256_loadu_ps(&temp_3[i]);
                __m256i R_index = _mm256_cvtps_epi32(_mm256_mul_ps(R_vec, factor));
                __m256i G_index = _mm256_cvtps_epi32(_mm256_mul_ps(G_vec, factor));
                __m256i B_index = _mm256_cvtps_epi32(_mm256_mul_ps(B_vec, factor));
                __m256 R_pq = _mm256_i32gather_ps(linear_to_PQ, R_index, 4);
                __m256 G_pq = _mm256_i32gather_ps(linear_to_PQ, G_index, 4);
                __m256 B_pq = _mm256_i32gather_ps(linear_to_PQ, B_index, 4);
                _mm256_storeu_ps(&R[i], R_pq);
                _mm256_storeu_ps(&G[i], G_pq);
                _mm256_storeu_ps(&B[i], B_pq);
            }
            for (int i = adjustedSize; i < y_size; i++) {
                R[i] = PQ_to_linear[(static_cast<unsigned short>(temp_1[i]*65535))];
                G[i] = PQ_to_linear[(static_cast<unsigned short>(temp_2[i]*65535))];
                B[i] = PQ_to_linear[(static_cast<unsigned short>(temp_3[i]*65535))];
            }
            */
            //cout <<"max "<< max << endl;
            //max = 0;
            /*
            for(int i=0;i<1;i++){
                cout << "R"<< R[i] << endl;
            }
            for(int i=0;i<1;i++){
                cout << "G"<< G[i] << endl;
            }
            for(int i=0;i<1;i++){
                cout << "B"<< B[i] << endl;
            }*/

            //BT.2020 limted range RGB轉YUV
            //Y = 0.2256*R + 0.5823*G + 0.05093*B
            //U = -0.1222*R -0.3154*G + 0.4375*B + 0.5
            //V = 0.4375*R - 0.4023*G - 0.0352*B + 0.5

            //BT.2020 full range RGB轉YUV
            //Y = 0.2627*R + 0.6780*G + 0.0593*B
            //U = -0.1396*R -0.3604*G + 0.5*B + 0.5
            //V = 0.5*R - 0.4598*G - 0.0402*B + 0.5

            array_scalar_multiply_40(temp_1, R, 0.2627, y_size);
            array_scalar_multiply_40(temp_2, G, 0.6780, y_size);
            array_scalar_multiply_40(temp_3, B, 0.0593, y_size);
            array_add_40(temp_4, temp_1, temp_2, y_size);
            array_add_40(Y_float, temp_4, temp_3, y_size);

            array_scalar_multiply_40(temp_1, R, -0.1396, y_size);
            array_scalar_multiply_40(temp_2, G, -0.3604, y_size);
            array_scalar_multiply_40(temp_3, B, 0.5, y_size);
            array_add_40(temp_4, temp_1, temp_2, y_size);
            array_add_40(temp_5, temp_4, temp_3, y_size);
            array_scalar_add_40(U_full, temp_5, 0.5, y_size);

            array_scalar_multiply_40(temp_1, R, 0.5, y_size);
            array_scalar_multiply_40(temp_2, G, -0.4598, y_size);
            array_scalar_multiply_40(temp_3, B, -0.0402, y_size);
            array_add_40(temp_4, temp_1, temp_2, y_size);
            array_add_40(temp_5, temp_4, temp_3, y_size);
            array_scalar_add_40(V_full, temp_5, 0.5, y_size);
            //cout<<"step 8" << endl;
    /*
            for(int i=0;i<1;i++){
                cout << "y"<< Y_float[i] << endl;
            }
            for(int i=0;i<1;i++){
                cout << "u"<< U_full[i] << endl;
            }
            for(int i=0;i<1;i++){
                cout << "v"<< V_full[i] << endl;
            }*/

            for (int y=0;y<height/2;y++) { //U超採樣
                for (int x=0;x<width/2;x++) {
                    U_float[y*width/2 + x] = (U_full[(y*2)*width + x*2]+U_full[(y*2)*width + x*2 + 1]+U_full[(y*2+1)*width + x*2]+U_full[(y*2+1)*width + x*2 + 1]);
                }
            }
            //cout << U_full[0] << endl;
            for (int y=0;y<height/2;y++) { //V超採樣
                for (int x=0;x<width/2;x++) {
                    V_float[y*width/2 + x] = (V_full[(y*2)*width + x*2]+V_full[(y*2)*width + x*2 + 1]+V_full[(y*2+1)*width + x*2]+V_full[(y*2+1)*width + x*2 + 1]);
                }
            }
            //cout<<"step 9" << endl;

            for (size_t i = 0; i <u_size; i++) {
                U[i] = static_cast<unsigned short>(U_float[i]*1023/4);
                V[i] = static_cast<unsigned short>(V_float[i]*1023/4);
            }
            for (size_t i = 0; i <y_size; i++) {
                Y[i] = static_cast<unsigned short>(Y_float[i]*1023);
            }
            //cout<<"step 10" << endl;
/*
            for (int i = 0; i < y_size; ++i) {
                if (Y[i] > 1023) {
                    cout << "clip Y " << Y[i];
                    Y[i] = 1023;
                }
            }
            for (int i = 0; i < u_size; ++i) {
                if (U[i] > 1023) {
                    cout << "clip U " << U[i];
                    U[i] = 1023;
                }
                if (V[i] > 1023) {
                    cout << "clip V " << V[i];
                    V[i] = 1023;
                }
            }*/

            output_file.write(reinterpret_cast<const char*>(Y), y_size * sizeof(unsigned short));
            output_file.write(reinterpret_cast<const char*>(U), u_size * sizeof(unsigned short));
            output_file.write(reinterpret_cast<const char*>(V), v_size * sizeof(unsigned short));
            //cout<<"step 11" << endl;
            frame_index++;
            auto now_time = chrono::high_resolution_clock::now();
            auto durationMicro = chrono::duration_cast<std::chrono::microseconds>(now_time - section_time);

            cout << "frame " << frame_index << ", " << float(durationMicro.count())/1000000 << " s, " << 1/float(durationMicro.count())*1000000 << "fps" << endl;
        }

        /*output_file.close();
        this_thread::sleep_for(chrono::seconds(1));
        //cout<<"step 12" << endl;
        while(del==1){
            input_file.close();

            if (input_file.is_open()) {
                cout << "File is opened" << endl;
                input_file.close();
                if (input_file.fail()) {
                    cout << "Error closing file: " << input_file_path << endl;
                    return 1;
                } else {
                    cout << "File closed successfully" << endl;
                }
            }
            //del = std::remove(input_file_path.c_str());
            ifstream fileStream(input_file_path);
            if (fileStream.is_open()) {
                std::cout << "File exists." << std::endl;
                del = 1;
            } else {
                std::cout << "File does not exist." << std::endl;
                del = 0;
            }
            if (del == 0) {
                cout << input_file_path << " deleted" << endl;
            } else {
                cout << "can't delete " << input_file_path << endl;
                cerr << strerror(errno) << endl;
                del = 1;
            }
            this_thread::sleep_for(chrono::seconds(2));
        }*/
        //cout<<"step 13" << endl;
        //cout<<"total min:"<<min<<", total max:"<<max<<endl;
        //this_thread::sleep_for(chrono::seconds(30));
    }
    return 0;
}
