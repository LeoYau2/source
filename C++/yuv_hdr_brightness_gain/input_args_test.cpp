#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

int main(int argc, char *argv[]) {
    // argc 表示命令行参数的数量
    // argv 是一个指向参数字符串数组的指针

    if (argc == 3) {
        // argv[1] 是第一个字符串
        // argv[2] 是第二个字符串
        std::cout << "First string: " << argv[1] << std::endl;
        std::cout << "Second string: " << argv[2] << std::endl;
        this_thread::sleep_for(chrono::seconds(10));
    } else {
        std::cerr << "Usage: " << argv[0] << " <string1> <string2>" << std::endl;
        return 1; // 返回非零表示程序异常结束
    }

    return 0;
}
