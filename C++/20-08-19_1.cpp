#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

class MyPokemon
{
public:
    char MyName[20];
    int MyHP, MyAttack, MyDefence, MyVitality;
    float MyCP;
    MyPokemon()
    {

    };
    My(char *n,int h,int a,int d,int v)
    {
        strcpy(MyName,n);
        MyHP = h;
        MyAttack = a;
        MyDefence = d;
        MyVitality = v;
        MyCP = 0.25*(MyHP + MyAttack + MyDefence + MyVitality);
        cout << "=====here is your pokemon=====" << endl;
        cout << "Name:" << MyName << endl;
        cout << "Your CP value is:" << MyCP << endl;
        cout << "HP:" << MyHP << endl;
        cout << "Attack:" << MyAttack << endl;
        cout << "Defence:" << MyDefence << endl;
        cout << "Vitality:" << MyVitality << endl;
    };
};

class EnemyPokemon
{
public:
    int EnemyHP, EnemyAttack, EnemyDefence, EnemyVitality;
    float EnemyCP;
    Enemy()
    {
        srand(time(NULL));
        EnemyHP = rand()%100;
        srand(time(NULL)+1);
        EnemyAttack = rand()%100;
        srand(time(NULL)+2);
        EnemyDefence = rand()%100;
        srand(time(NULL)+3);
        EnemyVitality = rand()%100;
        EnemyCP = 0.25*(EnemyHP + EnemyAttack + EnemyDefence + EnemyVitality);
        cout << "=====You Meet a Enemy pokemon!!=====" << endl;
        cout << "Name:EnemyName" << endl;
        cout << "Enemy CP value is:" << EnemyCP << endl;
        cout << "HP:" << EnemyHP << endl;
        cout << "Attack:" << EnemyAttack << endl;
        cout << "Defence:" << EnemyDefence << endl;
        cout << "Vitality:" << EnemyVitality << endl;
    };

};

class BattleSystem: public MyPokemon, public EnemyPokemon
{
public:
    Battle();
};

BattleSystem::Battle()
{
    cout << "*****************************" << endl;
    if(MyCP > EnemyCP)
    {
        cout << "you WIN!!" << endl;
    }else
    {
        cout << "you LOSS!!" << endl;
    }
    cout << "*****************************" << endl;
}
    

int main()
{
    int h,a,d,v;
    char n[20];
    //battleSystem b;
    MyPokemon();
    cout << "Ready for Battle!!\nYour Pokemon name:";
    cin >> n;
    cout << "HP:";
    cin >> h;
    cout << "Defence:";
    cin >> d;
    cout << "Attack:";
    cin >> a;
    cout << "Vitality:";
    cin >> v;
    BattleSystem b;
    b.My(n,h,a,d,v);
    b.Enemy();
    b.Battle();
    system("pause");
    return 0;
}
