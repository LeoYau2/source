#include <iostream>
using namespace std;

class Warrior
{
	protected:
		int health;
	public:
		Warrior()
		{
			health = 0;
		}
		void levelup()
		{
			health += 10;
			//cout <<health <<endl;
		}
};

class Assitant
{
	protected:
		int magic;
	public:
		Assitant()
		{
			magic = 0;
		}
		void levelup()
		{
			magic += 10;
		}
};

class Ranger
{
	protected:
		int agility;
	public:
		Ranger()
		{
			agility = 0;
		}
		void levelup()
		{
			agility += 10;
		}
};

class Hero:public Warrior, public Assitant, public Ranger
{
	private:
		
	public:
		int level;
		Hero()
		{
			level = 0;
		}	

		void showplayer()
		{
			cout << "level : "<< level << "   " << endl;
			cout << "health : "<< health << endl;
			cout << "agility : "<< agility << endl;
			cout << "magic : "<< magic << endl;
		}
};

int main()
{
	//goto skip;
	int input;
	Warrior *w;
	Assitant *a;
	Ranger *r;
	Hero h1;
	w = &h1;
	r = &h1;
	a = &h1;
	for(int i=0;i<3;i++)
	{
		cout << "Create you own hero !!\n================================\nwhich Occupation you want to be?\n1:warrior\n2:ranger\n3:assistant\nEnter number:" ;
		cin >> input;
		h1.level += 1;
		if(input==1)
		{
			(*w).levelup(); //w ->levelup();
		}else if(input==2)
		{
			(*r).levelup(); //r ->levelup();
		}else if(input==3)
		{
			(*a).levelup(); //a ->levelup();
		}
		h1.showplayer();
	}
	cout << "================================\nadventure time!!!!\n";
	
	//skip:
	system("pause");
	return 0;
}
