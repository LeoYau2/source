#include <iostream>
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

        Pokemon(char *n, int h, int a, int d, int v);

        ~Pokemon();
        {
            cout << "Destructor:" << Name << endl;    
        };
};

Pokemon::Pokemon(char *n, int h, int a, int d, int v)
{
    strcpy(name,n);
    HP = h,
    Attack = a;
    Defence = f;
    Vitality = v,
    cout << "=====here is your pokemon=====" << endl;
    cout << "Name:" << Name << endl;
    cout << "HP:" << HP << endl;
    cout << "Attack:" << Attack << endl;
    cout << "Defence:" << Defence << endl;
    cout << "Vitality:" << Vitality << endl;
    cout << "==============================" << endl;
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
    cout << "=====Not satisfied enough?=====\n=====set value by yourself=====" << endl;

    cout << "Pokemon name:";
    gets(n);
    cout << "Name:" << n << endl;
    cout << "HP:";
    cin >> h;
    cout << "Attack:";
    cin >> a;
    cout << "Defence:";
    cin >> d;
    cout << "Vitality:";
    cin >> v;
    Pokemon p1(n, h, a, d ,v);

    system("pause");
    return 0;
}