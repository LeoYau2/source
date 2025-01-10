#include <iostream>
using namespace std;

void f(int *a)
{
    *a = 0;
}

class c
{
    public:
        //ptr *p
        void show()
        {

        }
};

int main()
{
    int *p1;
    int x = 1;
    p1 = &x;
    //p1 = &x;
    f(&x);
    cout << &x << " " << x << p1 << endl;
    system("pause");
}
