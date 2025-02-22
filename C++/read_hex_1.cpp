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
            ch = ~ch;     // 負數取補數 
        }
        cout << setw(2) << hex << static_cast<int>(ch) << " "; 
        count++; 
        if(count > 16) {  // 換行 
            cout << endl; 
            count = 0; 
        } 
    } 
    cout << endl; 
}

int main(int argc, char* argv[]) { 
    ifstream in(argv[1], ios::in | ios::binary); 
    if(!in) { 
        cout << "無法讀取檔案" << endl; 
        return 1; 
    } 

    print(in);
    in.close(); 

    return 0; 
}
