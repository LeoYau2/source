#include <iostream>
#include <thread>
#include <functional>
#include <chrono>
#include <atomic>
#include <mutex>

using namespace std;
mutex u_mutex;
atomic_int atomic_a(0);


void addAndShow(int a, int b, char* name)
{
    //下面這兩行迴圈只是延遲用
    for (int i = 1; i <= 10000; i++)
        for (int j = 1; j <= 10000; j++)
            ;



    cout << name << ":" << (a + b) << endl;
}

int increment_counter ()
{
	static std::atomic<int> counter(0);

	// increment is guaranteed to be done atomically
	int result = ++counter;

	return result;
}

void add_atomic_a(atomic_int* a)
{
    //unique_lock<mutex> lock(u_mutex);
    for (int i = 1; i <= 10000; i++)
        for (int j = 1; j <= 10000; j++)
        {
            atomic_a ++;
        }
}

void add_atomic_ax6(atomic_int* a)
{
    //unique_lock<mutex> lock(u_mutex);
    for (int i = 1; i <= 60000; i++)
        for (int j = 1; j <= 10000; j++)
        {
            atomic_a ++;
        }
}

void add_a(int* a)
{
    //unique_lock<mutex> lock(u_mutex);
    for (int i = 1; i <= 10000; i++)
        for (int j = 1; j <= 10000; j++)
        {
            a ++;
        }
}

void read_a(atomic_int* a)
{
    float b, c, d = 0;
    for (int i = 1; i <= 10000; i++)
        for (int j = 1; j <= 10000; j++)
            b = *a;
            c = *a;
            d = d + c - b;
    cout << d << endl;
}

void add_b(int* b, int id)
{
    b[id] ++;
}

int main()
{

    static std::atomic<int> counter(0);
    int a= 0;
	// increment is guaranteed to be done atomically
	int result = ++counter;
    int n = 0;
    cin >> n;
    int b[n] = {0};
    auto t1 = chrono::high_resolution_clock::now();
    auto t2 = chrono::high_resolution_clock::now();
    auto t3 = chrono::high_resolution_clock::now();
    auto t4 = chrono::high_resolution_clock::now();
    char name[] = "Number from Main";
    //thread thread_1(bind(&addAndShow, 100, 50, name));
    //thread thread_2(bind(&addAndShow, 100, 50, name));
    //thread thread_3(bind(&addAndShow, 100, 50, name));
    thread thread_1(add_atomic_a, &atomic_a);

    thread thread_2(bind(&add_atomic_a, &atomic_a));
    thread thread_3(add_atomic_a, &atomic_a);
    thread thread_4(add_atomic_a, &atomic_a);
    thread thread_5(add_atomic_a, &atomic_a);
    thread thread_6(add_atomic_a, &atomic_a);
    //thread thread_3(bind(&add_a, &a));
    increment_counter ();
    //add_a(&a);

    thread_1.join();
    thread_2.join();
    thread_3.join();
    thread_4.join();
    thread_5.join();
    thread_6.join();
    //thread_3.join();
    t2 = chrono::high_resolution_clock::now();
    cout << atomic_a << "\n";
    cout << " : " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;

    delete t1;

    t1 = chrono::high_resolution_clock::now();
    thread thread_7(add_atomic_ax6, &atomic_a);
    thread_7.join();
    cout << atomic_a << "\n";
    t2 = chrono::high_resolution_clock::now();
    cout << " : " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;

    t1 = chrono::high_resolution_clock::now();
    thread thread_8(add_a, &a);
    thread_8.join();
    cout << atomic_a << "\n";
    t2 = chrono::high_resolution_clock::now();
    cout << " a : " << chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << "ms" << endl;



    system("pause");
    return 0;
}
