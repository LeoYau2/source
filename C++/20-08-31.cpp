#include <iostream>
using namespace std;

int count = 0;

void move(int n, int s ,int d, int t)
{
	//cout << "move " << n << " disk from peg " << s << " to peg " << d << endl;
	if(n==1)
	{
		cout << "move " << 1 << " disk from peg " << s << " to peg " << d << endl;
		count++;
		return;
	}else
	{
		cout << "move " << n << " disk from peg " << s << " to peg " << d << endl;
		count++;
		move(n-1, s, t, d);
		move(n-1, t, d, s);
	}
 }

int main()
{
    int n, x, p[64][3]={0};
    cin >> n;
    move(n, 1, 2, 3); //move n disks from peg 1 to peg 2, using peg 3 as temporary peg
    
	cout << count << endl;
    x = n;
    for(int i=0;i<n;i++)
    {
        p[i][0] = n-i;
        cout << n-i <<endl;
    }

    for(int i=n-1;i>-1;i--)
    {
        if(p[i][0]==0)
        {
            cout << "a\t";
        }else
        {
            cout << p[i][0] << "a\t";
        }
        if(p[i][1]==0)
        {
            cout << "b\t";
        }else
        {
            cout << p[i][1] << "b\t";
        }
        if(p[i][2]==0)
        {
            cout << "c\t" << endl;
        }else
        {
            cout << p[i][2] << endl;
        }
    }
    cout << endl;
    system("pause");
    return 0;
}
