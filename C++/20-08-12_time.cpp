#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <chrono>
using namespace std;

int add(int,int);

int main()
{

    int time_step = 25, x1=2, y1=2, vx1=1,vy1=1,size_x = 64, size_y = 28;
    int step=0, pixel=0, t_start,frame_time;
    bool wait;
    int frame[size_x*size_y];
    cin >> time_step;
	auto begin = chrono::high_resolution_clock::now();
    int x;
    //cin >> x;      // wait for user input
    auto end = chrono::high_resolution_clock::now();
    auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << ms << endl;
	while(step<10000)
	{
        //auto begin = chrono::high_resolution_clock::now();
	    auto end = chrono::high_resolution_clock::now();
	    auto dur = end - begin;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
        //cout << ms << endl;
        if(wait==1)
            frame_time = ms - t_start;

        if(ms/time_step > step)
        {
            t_start = ms;
            //for(int i=0;i<size_y;i++)
            //{
            //    cout << endl;
            //}
            for(int y=0;y<size_y;y++)
            {
                for(int x=0;x<size_x;x++)
                {
                    if(x==x1 && y==y1)
                    {
                        printf("O");
                        //cout << "O";
                        //frame[pixel] = 0;
                    }else
                    {
                        if(x==0 || x==size_x-1)
                        {

                            if(y!=y1 && y!=0 && y!= size_y-1)
                            {
                                /*
                                cout << "|";
                                for(int i=0;i<size_x-2;i++)
                                {
                                    cout << " ";
                                }
                                cout << "|";*/
                                cout << "|                                                              |";
                                break;
                            }
                            //printf("|");
                            cout << "|";
                        }else if(y==0 || y==size_y-1)
                        {
                            //printf("-");
                            cout << "--------------------------------------------------------------|";
                            break;
                        }else
                        {
                            //printf(" ");
                            cout << " ";

                        }
                        //frame[pixel] = 1;
                    }
                    pixel++;
                }
                cout << endl;
            }
            //cout << frame << endl;
            x1+=vx1;
            y1+=vy1;
            if(x1>size_x-3 || x1<2)
                vx1 = -vx1;
            if(y1>size_y-3 || y1<2)
                vy1 = -vy1;
            cout  << "x:" << x1 << " y:" << y1 << " frame:" << step <<  " time:" << ms << "ms frame time:" << frame_time << "ms" << endl;
            pixel = 0;
            wait = 1;
            step++;
        }
	}
    cout << ms << endl;
	//long long time;
	//time = long long getCurrentTime()
	//cout << time << endl;

/*
	char s1[800], s2[800], s3[800];
	gets(s1);
	gets(s3);
	strcpy(s2,s1);
	for(int i=0;i<2;i++)
	{
		cout << "w" << endl;
		strcat(s1,s3);
		cout << s1 << "  " << s3 << endl;
		strcat(s3,s1);
		cout << s1 << "  " << s3 << endl;
	}
	strcat(s1,s3);
	cout << s1 << endl;
	cout << sizeof(s2) << endl;*/
	system("pause");
	return 0;
}

int add(int a,int b)
{
	return a+b;
}
/*
long long getCurrentTime()
{
struct cc_timeval tv;
CCTime::gettimeofdayCocos2d(&tv, NULL);
long long time = ((long long)tv.tv_sec) * 1000+ tv.tv_usec / 1000;
return time;
}*/
