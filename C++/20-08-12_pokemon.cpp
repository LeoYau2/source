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
        void Input();
        void Create();
        void Output();
        ~Pokemon()
        {
            cout << "Destructor:" << Name << endl;    
        };
};

void Pokemon::Create(void)
{
    HP = rand()%100;
    Attack = rand()%100;
    Defence = rand()%100;
    Vitality = rand()%100;
    
    cout << "Pokemon name:";
    gets(Name);
    cout << "=====here is your pokemon=====" << endl;
    cout << "Name:" << Name << endl;
    cout << "HP:" << HP << endl;
    cout << "Attack:" << Attack << endl;
    cout << "Defence:" << Defence << endl;
    cout << "Vitality:" << Vitality << endl;
    cout << "==============================" << endl;
}

void Pokemon::Input(void)
{
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
    cout << "==========create your pokemon==========" << endl;
    //srand(time(NULL));
    srand(0);
    Pokemon pokemon[4];
    for(int i=0;i<3;i++)
    {
        pokemon[i].Create();
    }
    
    cout << "=====Not satisfied enough?=====\n=====set value by yourself=====" << endl;
    pokemon[3].Input();

    //printf("RAND_MAX = %d\n", RAND_MAX);
    system("pause");
    cout << "===============Destructor===============" << endl;
    return 0;
}