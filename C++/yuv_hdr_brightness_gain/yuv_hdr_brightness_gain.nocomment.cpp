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

bool hasExtension(const std::string& fileName, const std::string& extension) {
    size_t pos = fileName.rfind('.');
    return (pos != std::string::npos) && (fileName.substr(pos) == extension);
}

std::string intToFourDigitString(int number) {
    std::stringstream ss;
    ss << std::setw(4) << std::setfill('0') << number;
    return ss.str();
}

using namespace std;

int main(){
    float max =0, gain = 8;
    int width = 3840, height = 2160, file_count = 0;
    int total_frame_count, frame_index = 0, yuv_size = width*height*1.5, y_size = width*height, u_size = width*height/4, v_size = width*height/4;
    string input_file_path = "X:\\yuv\\002.yuv";
    string input_path = "X:\\yuv\\input\\";
    //string input_file_path = "C:\\Users\\Leo\\Sync\\source\\C++\\yuv_hdr_brightness_gain\\output.yuv";
    string output_file_path = "C:\\Users\\Leo\\Sync\\source\\C++\\yuv_hdr_brightness_gain\\output.yuv";
    string output_path = "X:\\yuv\\gained\\";
    string filename;
    string na;
    bool foundYUV = false;
    while(true){
        string na = intToFourDigitString(file_count);
        input_file_path = input_path + na + ".yuv";
        output_file_path = output_path + na + ".yuv";

        ifstream input_file(input_file_path, ios::binary);
            if (!input_file.is_open()) {
                foundYUV = false;
                cout << "Waiting "<< input_file_path << endl;
                this_thread::sleep_for(chrono::seconds(1));
            }else{
                foundYUV = true;
            }


        if(foundYUV){
            cout << "Processing "<< input_file_path << endl;
            ifstream input_file(input_file_path, ios::binary);
            if (!input_file.is_open()) {
                cerr << "Failed to open the file." << endl;
                this_thread::sleep_for(chrono::seconds(2));
                return 1;
            }
            ofstream output_file(output_file_path, ios::binary);
            if (!output_file.is_open()) {
                cerr << "Failed to open the file for writing." << endl;
                this_thread::sleep_for(chrono::seconds(2));
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

            int remainder = y_size % 8;
            int adjustedSize = y_size - remainder;
            auto start_time = chrono::high_resolution_clock::now();
            for(int i=0;i<total_frame_count;i++){
                auto section_time = chrono::high_resolution_clock::now();
                input_file.seekg(yuv_size*frame_index*sizeof(unsigned short), ios::beg);
                input_file.read(reinterpret_cast<char*>(Y), y_size * sizeof(unsigned short));

                input_file.seekg((yuv_size*frame_index+y_size)*2, ios::beg);
                input_file.read(reinterpret_cast<char*>(U), u_size * sizeof(unsigned short));

                input_file.seekg((yuv_size*frame_index+y_size+u_size) * sizeof(unsigned short), ios::beg);
                input_file.read(reinterpret_cast<char*>(V), v_size * sizeof(unsigned short));

                array_size_double(U,U_full,width,height);//U放大兩倍
                array_size_double(V,V_full,width,height);//V放大兩倍
                /*for (int i = 539; i < 540; ++i) {
                    for (int j = 0; j < 960; ++j) {
                        cout << U_full[i*960+j] << " ";
                    }
                    cout << endl;
                }*/
                //BT.2020 limted range YUV轉RGB
                //R = 1.1643 * (Y-16) + 1.6853 * (V-128)
                //G = 1.1643 * (Y-16) - 0.1881 * (U-128) - 0.6529 * (V-128)
                //B = 1.1643 * (Y-16) + 2.1501 * (U-128)

                for (size_t i = 0; i < y_size; ++i) {
                    Y_float[i] = static_cast<float>(Y[i]);
                }

                array_scalar_add_40(temp_4, Y_float, -64.1882, y_size);
                array_scalar_multiply_40(temp_1, temp_4, 1.1643, y_size);
                array_scalar_add_40(temp_2, U_full, -513.5059, y_size);
                array_scalar_add_40(temp_3, V_full, -513.5059, y_size);

                array_scalar_multiply_40(temp_4, temp_3, 1.6853, y_size);
                array_add_40(R, temp_1, temp_4, y_size);

                array_scalar_multiply_40(temp_4, temp_2, -0.1881, y_size);
                array_scalar_multiply_40(temp_5, temp_3, -0.6529, y_size);
                array_add_40(temp_6, temp_1, temp_4, y_size);
                array_add_40(G, temp_6, temp_5, y_size);

                array_scalar_multiply_40(temp_4, temp_2, 2.1501, y_size);
                array_add_40(B, temp_1, temp_4, y_size);


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


                //BT.2020 limted range RGB轉YUV
                //Y = 0.2256*R + 0.5823*G + 0.05093*B
                //U = -0.1222*R -0.3154*G + 0.4375*B + 0.5
                //V = 0.4375*R - 0.4023*G - 0.0352*B + 0.5

                array_scalar_multiply_40(temp_1, R, 0.2256, y_size);
                array_scalar_multiply_40(temp_2, G, 0.5823, y_size);
                array_scalar_multiply_40(temp_3, B, 0.05093, y_size);
                array_add_40(temp_4, temp_1, temp_2, y_size);
                array_add_40(Y_float, temp_4, temp_3, y_size);

                array_scalar_multiply_40(temp_1, R, -0.1222, y_size);
                array_scalar_multiply_40(temp_2, G, -0.3154, y_size);
                array_scalar_multiply_40(temp_3, B, 0.4375, y_size);
                array_add_40(temp_4, temp_1, temp_2, y_size);
                array_add_40(temp_5, temp_4, temp_3, y_size);
                array_scalar_add_40(U_full, temp_5, 0.5, y_size);

                array_scalar_multiply_40(temp_1, R, 0.4375, y_size);
                array_scalar_multiply_40(temp_2, G, -0.4023, y_size);
                array_scalar_multiply_40(temp_3, B, -0.0352, y_size);
                array_add_40(temp_4, temp_1, temp_2, y_size);
                array_add_40(temp_5, temp_4, temp_3, y_size);
                array_scalar_add_40(V_full, temp_5, 0.5, y_size);


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

                for (size_t i = 0; i <u_size; i++) {
                    U[i] = static_cast<unsigned short>(U_float[i]*1023/4);
                    V[i] = static_cast<unsigned short>(V_float[i]*1023/4);
                }
                for (size_t i = 0; i <y_size; i++) {
                    Y[i] = static_cast<unsigned short>(Y_float[i]*1023);
                }

                output_file.write(reinterpret_cast<const char*>(Y), y_size * sizeof(unsigned short));
                output_file.write(reinterpret_cast<const char*>(U), u_size * sizeof(unsigned short));
                output_file.write(reinterpret_cast<const char*>(V), v_size * sizeof(unsigned short));
                frame_index++;
                auto now_time = chrono::high_resolution_clock::now();
                auto durationMicro = chrono::duration_cast<std::chrono::microseconds>(now_time - section_time);

                cout << "frame " << frame_index << ", " << float(durationMicro.count())/1000000 << " s, " << 1/float(durationMicro.count())*1000000 << "fps" << endl;

            }
            input_file.close();
            output_file.close();
            int result = std::remove(input_file_path.c_str());
            if (result == 0) {
                cout << input_file_path << "deleted" << endl;
            } else {
                cout << "can't delete " << input_file_path << endl;
            }
            file_count++;
        }

    }
    this_thread::sleep_for(chrono::seconds(10));
}
