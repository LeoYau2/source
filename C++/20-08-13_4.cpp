#include <iostream>
using namespace std;

int a(int n)
{
	cout << n << endl;
	return a(n-1)+n;
}

int main()
{
	int sum = 0 ,y = 1, n=2;
	sum = a(n);
	cout << "n =" << n << ", f(" << n << ") = " << sum;
	return 0;
}
