#include <iostream>
#include <string.h>
using namespace std;

class Employee
{
	public:
		char name[20];
		int id, salary;
		void input_information();
		void input_information(char *, int, int);
		void show_message();
		void update(int);
};

void Employee::input_information()
{
	cout << "input name, id and salary ";
	cin >> name >> id >> salary;
}

void Employee::input_information(char name[20],int id,int salary)
{
	strcpy((*this).name, name);
	(*this).id = id;
	(*this).salary = salary;
}

void Employee::show_message()
{
	cout << name << "  id = " << id << ", salary = " << salary << endl;
}

void Employee::update(int x)
{
	salary += x;
}

int main()
{
	Employee e1, e2;
	e1.input_information();
	e1.show_message();
	e2.input_information("Julie", 8, 50000);
	e2.show_message();
	return 0;
}
