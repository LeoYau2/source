#include <iostream>
#include <cstdio>
#include <stdlib.h>
using namespace std;

/*int max()
{
    cout << "b";
    return 1;
}*/

int main()
{
	int count = 23021, rand_seed = 1;
    int score[count], sort[count], n = 0,index = 0 ,k ,max ,score_limit = 100;
    bool mark[count];
    for(int i=0;i<count;i++)
    {
        //score[i] = (i*i)%100;
        srand(rand_seed);
        score[i] = rand()%score_limit;
        rand_seed++;
        mark[i] = 0;
        cout << score[i] << " ";
    }
    
        for(int i=0;i<count;i++)
    {
		if(mark[i] == 0)
		{
			//cout << i << " ";
		}
    }
    cout << endl;
    system("pause");
    while(n<count)
    {
		//cout << max << " ";
    	for(int i=0;i<count;i++)
    	{
    		//cout << i << " ";
    		if(mark[i] == 0)
    		{
    			//cout << i << " ";
    			if(score[i]>max)
    			{
    				max = score[i];
    				//cout << "max="<< max << " ";
				}
			}	
		}
		cout << endl << max << " : ";
		//cout << "max=" << max << endl;
        
		for(int i=0;i<count;i++)
    	{
			if(mark[i] == 0)
    		{
    			if(score[i] == max)
    			{
    				sort[n] = score[i];
    				cout << "[" << i << "]";
    				mark[i] = 1;
    				n++;
    			}
    		}
		}
		max = 0;
		//system("pause");
	}
	
	cout << endl;
    //system("pause");
	for(int i=0;i<count;i++)
	{
		cout << sort[i] << " ";
	}
	//system("pause");

   /* while(n<20)
    {
    	if(score[n]>max)
    	{
    		max = score[n];

		}
		k = n;
    	while(k<20)
    	{
    			
		}
        //score[i] = (i*i)%10;
        cout << score[n] << " ";
        n++;
    }*/
    
    //max(score);
    //cout << "a";
    return 0;
}
