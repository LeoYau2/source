#include <iostream>
using namespace std;

int main()
{
	int count = 18;
	int score[count], temp;
	for(int i=0;i<count;i++)
	{
		cin >> score[i];
	}
	
	for(int i=0;i<count-1;i++)
	{
		for(int j=i+1;j<count;j++)
		{
			if(score[i]>score[j])
			{
				temp = score[i];
				score[i] = score[j];
				score[j] = temp;
			}
		}
	}
	
	for(int i=0;i<count;i++)
	{
		cout << score[i] << " ";
	}
	system("pause");
	return 0;
} 
