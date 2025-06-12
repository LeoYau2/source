#include <iostream>
#include <cstdlib>
using namespace std;

class circle
{
	public:
		int x, y, radius;
		void show_message();
		void displacement(int,int);
		void read_in_data();
};

void circle::show_message()
{
	cout << "Center is (" << x << ", " << y << "), radius = " << radius << endl;	
}

void circle::displacement(int a, int b)
{
	x += a;
	y += b;
}

void circle::read_in_data()
{
	cout <<"Input x, y and radius" << endl;
	cin >> x >> y >> radius;
}


int main()
{
	int *p;
	circle a, c1, c2,*c;
	//p = p
	//c1.x = 20;
	//c1.y = 30;
	//c1.radius = 8;
	c1.read_in_data();
	c1.show_message();
	c1.displacement(-20, 10);
	c1.show_message();
	a.radius = 3;
	a.show_message();
	cout << a.radius << endl;
	cout << p << endl;
	cout << c << endl;
	system("pause");
	return 0;
}
