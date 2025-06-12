#include <iostream>
#include <stdlib.h>
using namespace std;

class CDummy
{
	public:
		static int n;
		CDummy() {n++;};
		~CDummy() {n--;};
		
};

class Baseball
{
	private:
		char name[20];
		int fire, safe;
	public:
		static int totalfire, totalsafe;
		Baseball();
		output();
};

int CDummy::n=0;
int Baseball::totalfire=0;
int Baseball::totalsafe=0;

void output()
{
	//cout << b[0].name;
};

int main()
{
	//CDummy a;
	//CDummy b[5];
	//CDummy *c = new CDummy;
	//cout << a.n << endl;
	//delete c;
	//cout << CDummy::n << endl;
	cout << "==========input Data==========\n";
	Baseball b[3];
	/*Baseball b1;
	Baseball b2;
	Baseball b3;*/
	
	cout << "==========output Data==========\n====================\n";
	for(int i=0;i<3;i++)
	{
		b[i].output(); 
	}
	cout << "totalfires:" << Baseball::totalfire << "\ntotalsafes:" << Baseball::totalsafe << "\ntotalcountsafe:" << float(Baseball::totalfire)/float(Baseball::totalsafe) << endl;
	
	system("pause");
	return 0;
}

Baseball::Baseball()
{
	int temp;
	cout << "Name:";
	cin >> name;
	cout << "Fires:";
	cin >> temp;
	fire = temp;
	totalfire += temp;
	cout << "Safes:";
	cin >> temp;
	safe = temp;
	totalsafe += temp;
	cout << "totalfires:" << totalfire << endl;
	cout << "totalsafes:" << totalsafe << endl << "====================" << endl;
}

Baseball::output()
{
	cout << "Name:" << name << "\nFires:" << fire << "\nSafes:" << safe << "\n====================" << endl;
	//cout << name << endl;
}
/*int Input()
{
	int temp;
	cout << "==========input Data==========\nName:";
	cin >> temp;
	fire += temp;
	cout << "totalfires:" << fire;
	
}*/
