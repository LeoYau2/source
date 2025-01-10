#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <cstdlib>
using namespace std;
//C
/*int main()
{
	int a, b;
	printf("input 2 integers ");
	scanf("%d %d",&a , &b);
	printf("a = %d, b = %d\n",a ,b);
	system("pause");
	
	float f1;
	scanf(%f,&f1);
	printf("%f" ,f1);
	return 0;
}*/

//C++
int main()
{
	int a, b;
	cout << "input 2 integers ";
	cin >> a >> b;
	cout << "a = " << a << ", b = " << b << endl;
	system("pause");
	
	float f1;
	cin >> f1;
	cout << f1;
	return 0;
}
