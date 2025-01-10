#include <iostream>
#include <cstdlib>
using namespace std;

struct Student
{
	int id, score;
};

struct Circle
{
	float x, y, radius;
};

int main()
{
	struct Student s1, s2, *sptr ;
	s1.id = 5;
	s1.score = 92;
	cout << "Input id and score of s2" << endl;
	cin >> s2.id >> s2.score;
	sptr = &s2;
	cout << "id = " << sptr->id << " score = " << sptr->score << endl;
	cout << sizeof(s1) << endl << sizeof(struct Student) << endl;
	
	struct Circle c1, c2;
	cout << "Input x, y and radius of c1" << endl;
	cin >> c1.x >> c1.y >> c1.radius;
	c2.x = c1.x + 20;
	c2.y = c1.y + 30;
	c2.radius = c1.radius * 2;
	cout << "c1 : x = " << c1.x << ", y = " << c1.y << ", radius = " << c1.radius << endl;
	cout << "c2 : x = " << c2.x << ", y = " << c2.y << ", radius = " << c2.radius << endl;
	
	system("pause");
	return 0;
} 
