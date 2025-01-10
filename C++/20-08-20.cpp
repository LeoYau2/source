#include <iostream>
using namespace std;

struct Student
{
	char name[20];
	int grade;
	struct Student *next;
};

int main()
{
	int n = 4;
	struct Student s[n], *ptr;
	int i;
	cout << sizeof(i) << endl;
	cout << sizeof(s) << endl;
	
	cout << ptr << endl;
	
	for(int i=0;i<n;i++)
	{
		cout << "Input No." << i << " Name and grade  " << endl;
		cin >> s[i].name >> s[i].grade;
		cout <<ptr << endl;
	}
	for(int i=0;i<n;i++)
	{
		s[i].next = &s[i+1];
		cout << ptr << endl;
	}
	s[n-1].next = NULL;
	ptr = s; //ptr = &s[0];
	while(ptr != NULL)
	{
		cout << ptr->name << " =  " << ptr->grade <<endl;
		ptr = ptr->next;
		cout << ptr << endl;
	} 
	system("pause");
	return 0;
} 
