#include <iostream>
#include <cstdio>
using namespace std;

int main()
{
    float hight, weight, BMI;
    cout << "輸入身高(cm):" ;
    cin >> hight;
    cout << "輸入體重(kg):" ;
    cin >> weight;
    hight = hight / 100;
    BMI = weight/(hight*hight);
    cout << "您的BMI為:" << BMI << endl ;
    if (BMI<18.5)
    {
    	cout << "體重過輕" << endl;
	}
	else if(BMI<24)
	{
		cout << "正常" << endl;
	}
	else if(BMI<27)
	{
		cout << "過重" << endl;
	}
	else if(BMI<30)
	{
		cout << "輕度肥胖" << endl;
	}
	else if(BMI<35)
	{
		cout << "中度肥胖" << endl;
	}
	else
	{
		cout << "重度肥胖" << endl;
	}

    system("pause");
    return 0;
}
