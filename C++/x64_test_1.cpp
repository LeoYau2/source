#include <iostream>
#include <cstdio>
#include <stdlib.h>
using namespace std;

int main()
{
	int count = 330000, rand_seed = 1;  //-march=x86-64
    int score[count], sort[count], n = 0,index = 0 ,k ,max ,score_limit = 100;
    bool mark[count];
    for(int i=0;i<count;i++)
    {
        //score[i] = (i*i)%100;
        //srand(rand_seed);
        //score[i] = rand()%score_limit;
        score[i] = 0;
        //rand_seed++;
        //mark[i] = 0;
        cout << score[i] << " ";          //
    }

	cout << endl;
    system("pause");
	for(int i=0;i<count;i++)
	{
		//cout << sort[i] << " ";        //
	}

    system("pause");
    return 0;
}
