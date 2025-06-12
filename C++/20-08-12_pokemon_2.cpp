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

        Pokemon(char *Name,int HP, int Attack, int Defence, int Vitality);

        ~Pokemon()
        {
            cout << "Destructor:" << Name << endl;    
        };
};

Pokemon::Pokemon(char *Name,int HP, int Attack, int Defence, int Vitality)
{
    //strcpy(name,n);

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
    char Name[20];
    int HP, Attack, Defence, Vitality; 
    cout << "==========create your pokemon==========" << endl;
    //srand(time(NULL));
    srand(0);
    //Pokemon pokemon[4];
    //for(int i=0;i<3;i++)
    //{

        //pokemon[i].Create();
    //}

    //Pokemon p1;
    cout << "=====Not satisfied enough?=====\n=====set value by yourself=====" << endl;

    cout << "Pokemon name:";
    gets(Name);
    cout << "Name:" << Name << endl;
    cout << "HP:";
    cin >> HP;
    cout << "Attack:";
    cin >> Attack;
    cout << "Defence:";
    cin >> Defence;
    cout << "Vitality:";
    cin >> Vitality;
    Pokemon p1(Name, HP, Attack, Defence, Vitality);

    system("pause");
    return 0;
}