#include <iostream>
#include <chrono>
using namespace std;

int time_step = 33, x1=2, y1=2, vx1=1,vy1=1,size_x = 118, size_y = 28, q=1;
int step=0, frames=0, pixel=0, t_start, frame_time = 100, input, qx1, qx2, t_s = 0, t_f = 0;
bool wait=1,wait_f=1,skip;
//char frame[(size_x+1)*size_y];

void test();
void draw();

int main()
{
    auto begin = chrono::high_resolution_clock::now();
    int x;
    //cin >> x;      // wait for user input
    auto end = chrono::high_resolution_clock::now();
    auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << ms << endl;
    test();
    while(step<100)
    {
        
        auto end = chrono::high_resolution_clock::now();
	    auto dur = end - begin;
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
        
        if(wait == 1)
        {
            wait = 0;
            t_s = ms;
        }

        if(wait_f == 1)
        {
            wait_f = 0;
            t_f = ms;
        }

        if(ms/time_step > step)
        {
            /*
            x1+=vx1;
            y1+=vy1;
            if(x1>size_x-3 || x1<2)
                vx1 = -vx1;
            if(y1>size_y-3 || y1<2)
                vy1 = -vy1;
                */
            //if(ms/frame_time > frames)
            //{
                
                
            //}
            step++;
            
            test();
            wait = 1;
        }

        if(ms-t_f < 2*frame_time)
            {
                skip = 0;
            }else
            {
                skip = 1;
            }

        cout << " " << ms-t_f;
        if(ms-t_f > frame_time)
        {
            cout << endl << frames <<endl;
            if(skip == 0)
            {  
                draw();
            }
            frames++;
            wait_f = 1;
        }
    }
}

void test()
{
    cout << "test " << step << endl;
}

void draw()
{
    cout << "draw " << "frames" << endl ;
    for(int i=0;i<10000;i++)
    {
        for(int j=0;j<10000;j++)
        {
            q = -q;
        }
        
    }
}