#include <iostream>
#include <cstdio>
using namespace std;

class Cpolygon
{
protected:
    int width;
public:
    void set_values(int a)
    {
        width = a;
    }
};

class CRectangle: public Cpolygon
{
public:
    int area()
    {
        return width*width;
    }

    void PrintRectangle()
    {
        for(int i=0;i<width;i++)
        {
            for(int i=0;i<width;i++)
            {
                cout << "$";
            }
                cout << endl;
        }
    }
};

class CTriangle: public Cpolygon
{
public:
    float area()
    {
        return 0.5*width*width;
    }

    void PrintTriangle()
    {
        for(int i=0;i<width;i++)
        {
            for(int j=0;j<i+1;j++)
            {
                cout << "$";
            }
                cout << endl;
        }
    }
};

int main()
{
    CRectangle rect;
    CTriangle trgl;
    int N;
    cout << "input a number:";
    cin >> N;
    rect.set_values(N);
    trgl.set_values(N);
    cout << "Rectangle area = " << rect.area() << endl;
    rect.PrintRectangle();
    cout << "Triangle area = " << trgl.area() << endl;
    trgl.PrintTriangle();
    return 0;
}