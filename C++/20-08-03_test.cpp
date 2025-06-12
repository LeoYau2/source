#include <iostream>
using namespace std;

int add(int,int);

int main()
{
    int int_a;
    cin >> int_a;
    int *p_a = &int_a;
    int array[int_a];
    cout << "abc\ndef\n";
    cout << add(1,2) << endl;
    cout << p_a << "-->" << int_a << endl;
    for(int i=0;i<int_a;i++)
    {
        array[i] = i;
        cout << array[i] << " ";
    }
    return 0;
}

int add(int a,int b)
{
    return(a+b);
}