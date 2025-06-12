#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;

class normal
{
	public:
		int health, agility, magic;
		virtual void getvalue()
		{
			health = rand()%100;
			agility = rand()%100;
			magic = rand()%100;
		}
		virtual void cp() = 0;
		virtual void show() = 0;
};

class Warrior:public normal
{
	private:
		int c;
	public:
		void getvalue()
		{
			health = 200 + rand()%100;
			agility = rand()%100;
			magic = rand()%100;
		}
		void cp()
		{
			c = 2*health + agility + magic;
		}
		void show()
		{
			cout << "I'm Warrior!!\nhealth : " << health << "\nagility : " << agility << "\nmagic : " << magic << "\nCP : " << c << endl;
		}
};

class Assitant:public normal
{
	private:
		int c;
	public:
		void getvalue()
		{
			health = rand()%100;
			agility = rand()%100;
			magic = 200 + rand()%100;
		}
		void cp()
		{
			c = health + agility + 2*magic;
		}
		void show()
		{
			cout << "I'm Assitant!!\nhealth : " << health << "\nagility : " << agility << "\nmagic : " << magic << "\nCP : " << c << endl;
		}
};

class Ranger:public normal
{
	private:
		int c;
	public:
		void getvalue()
		{
			health = rand()%100;
			agility = rand()%100;
			magic = rand()%100;
		}
		void cp()
		{
			c = health + agility + magic;
		}
		void show()
		{
			cout << "I'm Ranger!!\nhealth : " << health << "\nagility : " << agility << "\nmagic : " << magic << "\nCP : " << c << endl;
		}
};


int main()
{
	srand( time(NULL) );
	normal *n;
	Warrior w;
	Assitant a;
	Ranger r;
	n = &w;
	n -> getvalue();
	n -> cp();
	n -> show();
	n = &a;
	n -> getvalue();
	n -> cp();
	n -> show();
	n = &r;
	n -> getvalue();
	n -> cp();
	n -> show();
	
	system("pause");
	return 0;
} 
