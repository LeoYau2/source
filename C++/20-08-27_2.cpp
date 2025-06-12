#include <iostream>
using namespace std;

class Rectangle
{
	Rectangle
};

Rectangle::Rectangle(int x, int y)
{
	this->x = 
}

Rectangle::Rectangle()
{
	cout << "Input x, y, lenth and width ";
	cin >> x >> y >> lenth >> width;
}

Rectangle::Rectangle()
{
	x = y = length = width = 1;
}

int Rectangle::area()
{
	return lenth * width;
}

void Rectangle::show_Rectangle()
{
	cout << "x = " << x << ",y = " << y << endl;
	cout << "lenth = " << lenth << ", width = " << width << endl;
}

int main()
{
	Rectangle r1(75, 60, 10, 8);
	r1.show_Rectangle();
	Rectangle r2(50,40);
	r2.
}
