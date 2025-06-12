#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <chrono>
using namespace std;

int add(int,int);

int main()
{
    //float frame_time = 33.33, time_step = 10, x1=2, y1=2, vx1=-3,vy1=1;
    int time_step = 33, frame_time=100, x1=2, y1=2, vx1=1,vy1=1,size_x = 118, size_y = 28;
    int step=0, frames=0, pixel=0, t_start, input, qx1, qx2;
    bool wait=1, draw=1;
    char frame[(size_x+1)*size_y];
    cout << "time step:" << time_step << "ms" << "  size:x=" << size_x << ",y=" << size_y << "  initial coordinate:x=" << x1 << ",y=" << y1 << "  initial speed:Vx=" << vx1 << ",Vy=" << vy1 << endl;
    cout << "Press enter to start, input anything to set parameters.";

    if(cin.get() == '\n')
    {
        cout<<"ENTER WAS PRESSED"<<endl;
    }else
    {
        cout << "Press enter to skip." << endl << "time step:";
        cin >> time_step;
        if(cin.get() == '\n')
        {
            cout << time_step << endl;
            cout << "w" << endl;
            system("pause");
        }else
        {
            cout << "size x,y:";
            if(cin.get() == '\n')
            {
                cout << time_step << endl;
            }else
            {
                /* code */
            }
        }

    }
    cin >> time_step;
    /*cin >> input;
    if(input==0)
    {
        cout << "w";
    }else{
        cout << "q";
    }
    system("pause");*/
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
        if(draw==1)
        {
            frame_time = ms - t_start;
            draw == 0;
        }
            

        if(ms/time_step > step)
        {
            x1+=vx1;
            y1+=vy1;
            if(x1>size_x-3 || x1<2)
                vx1 = -vx1;
            if(y1>size_y-3 || y1<2)
                vy1 = -vy1;

            if(ms/frame_time > frames)
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
                            frame[pixel] = 'O';
                            //printf("O");
                            //cout << "O";
                        }else
                        {
                            if(x==0 || x==size_x-1)
                            {
                                //printf("|");
                                //cout << "|";
                                frame[pixel] = '|';
                            }else if(y==0 || y==size_y-1)
                            {
                                frame[pixel] = '-';
                                //cout << "-";
                                //printf("-");
                            }else
                            {
                                frame[pixel] = ' ';
                                //printf(" ");
                                //cout << " ";
                            }
                            //frame[pixel] = 1;
                        }
                        pixel++;
                    }
                    frame[pixel] = '\n';
                    pixel++;
                    //cout << endl;
                }
                frame[(size_x+1)*size_y-1] = ' ';
                cout << frame << endl;


                
                cout  << "x:" << x1 << " y:" << y1 << " frame:" << frames << " step:" << step <<  " time:" << ms << "ms frame time:" << frame_time << "ms" << endl;
                pixel = 0;
                draw = 1;
                frames++;
            }
            step++;
        }
	}
    cout << ms << endl;

	system("pause");
	return 0;
}
