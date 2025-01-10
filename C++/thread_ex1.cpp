#include <iostream>
#include <thread>
#include <functional>

using namespace std;

void addAndShow(int a, int b, char* name)
{
    //下面這兩行迴圈只是延遲用
    for (int i = 1; i <= 10000; i++)
        for (int j = 1; j <= 10000; j++)
            ;



    cout << name << ":" << (a + b) << endl;
}

int main()
{
    char name[] = "Number from Main";
    thread t1(bind(&addAndShow, 100, 50, name));
    thread t2(bind(&addAndShow, 100, 50, name));
    thread t3(bind(&addAndShow, 100, 50, name));
    cout << "\n";
    t1.join();
    t2.join();
    t3.join();

    system("pause");
    return 0;
}
