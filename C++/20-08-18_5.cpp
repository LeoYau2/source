#include <iostream>
using namespace std;

int main()
{
    int n, count = 0, p[1000];
    cin >> n;
    for(int i=2;i<n-1;i++)
    {
        if(n%i==0)
        {
            p[count] = i;
            count++;
        }
    }
    for(int i=0;i<count;i++)
    {
        cout << p[i] << endl;
    }
    system("pause");
    return 0;
}