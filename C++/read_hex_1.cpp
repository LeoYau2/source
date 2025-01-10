#include <iostream> 
#include <fstream> 
#include <iomanip> 
using namespace std; 

void print(ifstream &in) {
    char ch; 
    int count = 0;     
    while(!in.eof()) { 
        in.get(ch); 
        if(ch < 0) {
            ch = ~ch;     // �t�ƨ��ɼ� 
        }
        cout << setw(2) << hex << static_cast<int>(ch) << " "; 
        count++; 
        if(count > 16) {  // ���� 
            cout << endl; 
            count = 0; 
        } 
    } 
    cout << endl; 
}

int main(int argc, char* argv[]) { 
    ifstream in(argv[1], ios::in | ios::binary); 
    if(!in) { 
        cout << "�L�kŪ���ɮ�" << endl; 
        return 1; 
    } 

    print(in);
    in.close(); 

    return 0; 
}
