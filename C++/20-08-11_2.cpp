#include <iostream>
using namespace std;

int main()
{
	char name[0];
	int score;
	cout << "Input name ";
	gets(name);
	//cin >> name;
	cout << "Input score ";
	cin >> score;
	cout << "Name is " << name << ",score = " << score;
    system("pause");
	return 0;
}
