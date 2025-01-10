#include <iostream>
#include <cstdio>
using namespace std;

class Pokemon
{
private:
    string Name;
    int Hp, Defence, Attack, Vitality;
public:
    void InputPokemon()
    {
        cout <<"=====input=====\nPokemon name:";
        cin >> Name;
        cout << "HP:";
        cin >> Hp;
        cout << "Defence:";
        cin >> Defence;
        cout << "Attack:";
        cin >> Attack;
        cout << "Vitality:";
        cin >> Vitality;
    }
    void OutputPokemon()
    {
        cout << "\n=====output=====\nyour CP value is:" << CPvalue() << endl;
        cout << "HP:" << Hp << endl;
        cout << "Defence:" << Defence << endl;
        cout << "Attack:" << Attack << endl;
        cout << "Vitality:" << Vitality << endl;
    }
    float CPvalue()
    {
        return 0.25*(Hp + Defence + Attack + Vitality);
    }
};

int main()
{
    Pokemon pokemon_0;
    pokemon_0.InputPokemon();
    pokemon_0.OutputPokemon();
}