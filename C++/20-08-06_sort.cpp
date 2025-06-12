#include <iostream>
#include <cstdio>
using namespace std;

int status(int min, int index, int temp)
{
    cout << min << " " << index << " " << temp << endl;
}

int main()
{
    int Score[13], Sort[13], max = 0, index, min ,temp;
    for(int i=0;i<13;i++)
    {
        cout << "Input No." << i << " score ";
        cin >> Score[i];
    }

    /*for(int i=0;i<13;i++)
    {
        cout << Score[i] << " ";
    }*/

    for(int i=0;i<12;i++)
    {
        
        min = Score[i];
        //index = i;
        for(int j=i+1;j<13;j++)
        {
            if(Score[j]<Score[i])
            {
                //min = Score[j];
                //index = j;

                temp = Score[i];
                Score[i] = Score[j];
                Score[j] = temp;
            }
        }
        //status(min,index,temp);
    }

    /*for(int i=0;i<12;i++)
    {
        
        min = Score[i];
        index = i;
        for(int j=i+1;j<13;j++)
        {
            if(Score[j]<min)
            {
                min = Score[j];
                index = j;
            }
        }
        if(i != index)
        {
            temp = Score[i];
            Score[i] = Score[index];
            Score[index] = temp;
        }
        status(min,index,temp);
    }*/

    /*for(int i=0;i<13;i++)
    {
        for(int i=0;i<13;i++)
        {
            if(Score[i]>max)
            {
                max = Score[i];
                index = i;
            }
        }
        Sort[i] = max;
        Score[index] = 0;
        max = 0;
    }*/

    cout << endl;
    for(int i=0;i<13;i++)
    {
        cout << Score[i] << " ";
    }
    system("pause");
    return 0;
}