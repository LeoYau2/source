#include <iostream>
using namespace std;

int main()
{
	int count = 13;
	int score[count], temp, n = 0, flag;
	for(int i=0;i<count;i++)
	{
		cout << "Input No." << i << " score ";
		cin >> score[i];
	}
	for(int i=0;i<count-1;i++)
	{
		flag = 0;
		for(int j=i;j<count;j++)
		{
			if(score[i]>score[j])
			{
				temp = score[i];
				score[i] = score[j];
				score[j] = temp;
				n++;
				flag = 1;
			}
			//if(flag == 0)
				//break;
		}
		//n++;
	}
	for(int i=0;i<count;i++)
	{
		cout << "No." << i << " score = " << score[i] << endl;	
	}
	cout << endl << n << endl;	
	system("pause");
	return 0;
}
