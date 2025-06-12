#include <iostream>
#include <vector>
#include <cstdint>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "使用方法: " << argv[0] << " <寬度> <高度>" << std::endl;
        return 1;
    }

    int width = std::stoi(argv[1]);
    int height = std::stoi(argv[2]);
    int gain = std::stoi(argv[3]);
    size_t frame_size = width * height * 3;  // RGB48每像素3個uint16_t

    std::vector<uint16_t> frame_buffer(frame_size);
    size_t frame_index = 0;

    std::cout << "start" << std::endl;
    while (std::cin.read(reinterpret_cast<char*>(frame_buffer.data()), frame_size * sizeof(uint16_t))) {
        // 輸出當前幀的第一個 uint16_t 值
        std::cout << "Frame " << frame_index << ": First uint16_t value = "
                  << frame_buffer[0] << std::endl;
        frame_index++;

        // 將數據寫回標準輸出，以便管道傳遞
        std::cout.write(reinterpret_cast<char*>(frame_buffer.data()), frame_size * sizeof(uint16_t));
    }
    std::cout << "end" << std::endl;

    return 0;
}
