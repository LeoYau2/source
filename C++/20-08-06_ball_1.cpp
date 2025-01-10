#include <iostream>
//#include <cstdio>
using namespace std;

int main()
{
    float m1=1,x1=2,y1=2,z1=2,Vx1=2,Vy1=0,Vz1=0,radius=0.5,gravity=10,Rx=4,Ry=4,Rz=4,dt=0.0001;
    int step = 32,f=0;

    for(int i=0;i<100;i++)
    {
        Vz1 = Vz1-gravity*dt;
        x1 += Vx1*dt;
        y1 += Vy1*dt;
        z1 += Vz1*dt;
        if(x1<0|x1>Rx)
            Vx1=Vx1*-1;
        if(y1<0|y1>Ry)
            Vy1=Vy1*-1;
        if(z1<0|z1>Rz)
            Vz1=Vz1*-1;
        if(f==step)
            cout <<"x:"<< x1 <<" y:"<< y1 <<" z:"<< z1 <<" "<<f<< endl;
            f = 0;
        f++;
    }
    system("pause");
    return 0;
}