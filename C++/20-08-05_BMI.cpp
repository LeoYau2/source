#include <iostream>
#include <cstdio>
using namespace std;

int main()
{
    float hight, weight, BMI;
    cout << "��J����(cm):" ;
    cin >> hight;
    cout << "��J�魫(kg):" ;
    cin >> weight;
    hight = hight / 100;
    BMI = weight/(hight*hight);
    cout << "�z��BMI��:" << BMI << endl ;
    if (BMI<18.5)
    {
    	cout << "�魫�L��" << endl;
	}
	else if(BMI<24)
	{
		cout << "���`" << endl;
	}
	else if(BMI<27)
	{
		cout << "�L��" << endl;
	}
	else if(BMI<30)
	{
		cout << "���תέD" << endl;
	}
	else if(BMI<35)
	{
		cout << "���תέD" << endl;
	}
	else
	{
		cout << "���תέD" << endl;
	}

    system("pause");
    return 0;
}
