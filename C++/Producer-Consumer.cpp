#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <chrono>

using namespace std;

mutex mtx; //定義互斥鎖，用來同步對共享緩衝區queue的訪問
condition_variable cv; //條件變數，用來同步生產者和消費者執行緒
queue<int> buffer; //共享的隊列，用作生產者和消費者之間的緩衝區

void display_buffer() {
    unique_lock<mutex> display_lock(mtx); //取得互斥鎖
    cout << "Buffer contents: ";
    for(int i=0; i<buffer.size(); ++i) { //遍歷buffer資料
        cout << buffer.front() << " "; //顯示buffer資料
        buffer.push(buffer.front()); //將元素推回隊尾，保持順序
        buffer.pop(); //彈出元素但不消耗
    }
    cout << endl;
    display_lock.unlock(); //釋放互斥鎖
}

void producer() {
    while (true) {
        unique_lock<std::mutex> produce_lock(mtx); //取得互斥鎖
        int random = rand() % 100 + 1; //生產資料
        cout << "Producing " << random << endl;
        buffer.push(random); //放入緩衝區
        produce_lock.unlock(); //釋放互斥鎖
        display_buffer(); //顯示buffer内容
        cv.notify_one(); //喚醒等待的消費者執行緒
        this_thread::sleep_for(chrono::milliseconds(500)); //睡眠500ms
    }
}

void consumer() {
    while (true) {
        unique_lock<mutex> consume_lock(mtx); //取得互斥鎖
        cv.wait(consume_lock); //等待取得互斥鎖
        int val = buffer.front(); //取得緩衝區中的資料
        buffer.pop(); //從前端移除pop第一個元素
        cout << "Consuming " << val << endl; //消費資料
        consume_lock.unlock(); //釋放互斥鎖
        display_buffer(); //顯示buffer内容
        this_thread::sleep_for(chrono::milliseconds(1000)); //睡眠1000ms，比producer長
    }
}

int main() {
    thread producer_thread(producer); //建立生產者執行緒
    thread consumer_thread(consumer); //建立消費者執行緒
    producer_thread.join();  //等待生產者執行緒結束
    consumer_thread.join();  //等待消費者執行緒結束
    return 0;
}
