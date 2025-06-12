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
            ch = ~ch;     // 璽计干计 
        }
        cout << setw(2) << hex << static_cast<int>(ch) << " "; 
        count++; 
        if(count > 16) {  // 传︽ 
            cout << endl; 
            count = 0; 
        } 
    } 
    cout << endl; 
}

int main(int argc, char* argv[]) { 
    ifstream in(argv[1], ios::in | ios::binary); 
    if(!in) { 
        cout << "礚猭弄郎" << endl; 
        return 1; 
    } 

    print(in);
    in.close(); 

    return 0; 
}
