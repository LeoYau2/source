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

using namespace std;

int main(int argc, char *argv[]){
    string input_file_path;
    string output_file_path;
    unsigned int sample_frequency[1];
    if (argc == 4) {
        input_file_path = argv[1];
        output_file_path = argv[2];
        sample_frequency[0] = stoi(argv[3]);
        cout << "input:" << input_file_path << ", output:" << output_file_path << endl;
    } else {
        std::cerr << "Usage: " << argv[0] << " [input_path] [output_path] [sample_frequency]" << std::endl;
        //this_thread::sleep_for(chrono::seconds(10));
        return 1;
    }

    bool found_file;

    ifstream input_file(input_file_path, ios::binary);
    if (!input_file.is_open()) {
        found_file = false;
        cerr << strerror(errno) << endl;
        //this_thread::sleep_for(chrono::seconds(1));
    }else{
        found_file = true;
    }

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
        unsigned char* data = new unsigned char[filesize];

        auto start_time = chrono::high_resolution_clock::now();
        unsigned int input_sample_frequency[1];
        input_file.seekg(24, ios::beg);
        input_file.read(reinterpret_cast<char*>(input_sample_frequency), 4);
        cout << input_sample_frequency[0] << " hz -> " << sample_frequency[0] << " hz" << endl;
        input_file.seekg(0, ios::beg);
        input_file.read(reinterpret_cast<char*>(data), filesize);
        output_file.seekp(0, ios::beg);
        output_file.write(reinterpret_cast<const char*>(data), filesize);
        output_file.seekp(24, ios::beg);
        output_file.write(reinterpret_cast<const char*>(sample_frequency), 4);
    }
    return 0;
}
