// audio_process_1.cpp : ���ɮץ]�t 'main' �禡�C�{���|��ӳB�}�l����ε�������C
//
/*
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

int main() {
    std::vector<std::string> names;
    std::vector<int> scores;
    std::ifstream ifs("input.txt", std::ios::in);
    if (!ifs.is_open()) {
        cout << "Failed to open file.\n";
    }
    else {
        std::string name;
        int score;
        while (ifs >> name >> score) {
            cout << name << " " << score << "\n";
            names.push_back(name);
            scores.push_back(score);
        }
        ifs.close();
    }
    ifs.close();
    cout << name << " " << score << "\n";
    return 0;
}*/
#include <iostream>
#include <fstream>
using namespace std;

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

void show_parameters(int substep){
    cout << "substep " << substep << "\n";
}

int main()
{
    ifstream fin("config.txt");
    string filename;
    string parameter_name;
    float var;
    int substep = 8;
    fin >> parameter_name >> filename;
    cout << parameter_name << " " << filename << "\n";
    while (fin >> parameter_name >> var)
    {
        /*switch(str2int(parameter_name)){
            case str2int("substep"):
                substep = int(var);
            default:
                cout << "Unrecognized parameter '" << parameter_name << "'";
                break;
        }*/
        if(parameter_name=="substep"){
            substep = (int)var;
        }else{
            cout << "Unrecognized parameter '" << parameter_name << "'";
        }


        cout << parameter_name << " " << var << "\n";

    }
    show_parameters(substep);
}
/*
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
    ifstream fin("marks.txt");

    string name;
    int var1;
    int var2;
    int var3;

    while (fin >> name >> var1 >> var2 >> var3)
    {

        cout << name << var1 << var2 << var3 << "\n";
    }
}
*/
