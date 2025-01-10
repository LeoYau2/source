#include <iostream>
#include <cstdlib>
using namespace std;


int x, y;
void t5()
{
	x = x*5;
	//cout << "test" << endl;
}

int main()
{
	
	cout << "Input 2 integers";
	cin >> x >> y;
	cout << "x = " << x << ", y = " << endl;
	t5();
	cout << "5*x = " << x;
	
}
