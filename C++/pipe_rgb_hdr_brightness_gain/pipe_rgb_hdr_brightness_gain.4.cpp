#include <iostream>
#include <cstdio>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <width> <height> <gain>\n";
        return 1;
    }

    int width = std::atoi(argv[1]);
    int height = std::atoi(argv[2]);
    int gain = std::atoi(argv[3]); // 目前不使用 gain 参数

    // 从标准输入读取数据
    const size_t bufferSize = 3840 * 1600 * 6; // 每幀的大小
    char buffer[bufferSize];

    while (true) {
        size_t bytesRead = fread(buffer, 1, bufferSize, stdin);
        if (bytesRead == 0) {
            if (feof(stdin)) {
                std::cout << "End of input stream" << std::endl;
                break;
            }
            if (ferror(stdin)) {
                std::cerr << "Error reading from input pipe" << std::endl;
                break;
            }
        }
        size_t bytesWritten = fwrite(buffer, 1, bytesRead, stdout);
        if (bytesWritten != bytesRead) {
            std::cerr << "Error writing to output pipe" << std::endl;
            break;
        }
    }

    return 0;
}
