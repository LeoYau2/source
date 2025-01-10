#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

class Pokemon
{
    private:
        char Name[20];
        int HP, Attack, Defence, Vitality;
    public:
        //Pokemon();
        Pokemon();
        Pokemon(char *Name,int h, int a, int d, int v);

        ~Pokemon()
        {
            cout << "Destructor:" << Name << endl;
        };
};

Pokemon::Pokemon(char *n,int h, int a, int d, int v)
{
    strcpy(Name,n);
    HP = h;
    Attack = a;
    Defence = d;
    Vitality = v;

    cout << "=====here is your pokemon=====" << endl;
    cout << "Name:" << Name << endl;
    cout << "HP:" << HP << endl;
    cout << "Attack:" << Attack << endl;
    cout << "Defence:" << Defence << endl;
    cout << "Vitality:" << Vitality << endl;
    cout << "==============================" << endl;
}

Pokemon::Pokemon()
{
	HP = rand()%100;
	Attack = rand()%100;
	Defence = rand()%100;
	Vitality = rand()%100;
	cout << "Pokemon name:";
    gets(Name);
    cout << "=====here is your pokemon=====" << endl;
    cout << "Name:" << Name <<endl;
    cout << "CP:" << (float(HP)+float(Attack)+float(Defence)+float(Vitality))/4 << endl;
    cout << "HP:" << HP <<endl;
    cout << "Defence:" << Defence <<endl;
    cout << "Attack:" << Attack <<endl;
    cout << "Vitality:" << Vitality <<endl;
    /*cout << "HP:";
    cin >> h;
    cout << "Attack:";
    cin >> a;
    cout << "Defence:";
    cin >> d;
    cout << "Vitality:";
    cin >> v;*/
}

int main()
{
    char n[20];
    int h, a, d, v;
    cout << "==========create your pokemon==========" << endl;
    //srand(time(NULL));
    srand(0);
    //Pokemon pokemon[4];
    //for(int i=0;i<3;i++)
    //{

        //pokemon[i].Create();
    //}

    //Pokemon p1;
    Pokemon p[3];
    cout << "=====Not satisfied enough?=====\n=====set value by yourself=====" << endl;

    cout << "Name:";
    gets(n);
    cout << "HP:";
    cin >> h;
    cout << "Attack:";
    cin >> a;
    cout << "Defence:";
    cin >> d;
    cout << "Vitality:";
    cin >> v;
    Pokemon p4(n, h, a, d, v);

    system("pause");
    return 0;
}
