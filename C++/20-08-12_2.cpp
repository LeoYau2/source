#include <iostream>
#include <string.h>
#include <chrono>
using namespace std;

int add(int,int);

int main()
{
	auto begin = chrono::high_resolution_clock::now();    
    int x;
    cin >> x;      // wait for user input
    auto end = chrono::high_resolution_clock::now();    
    auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << ms << endl;
	for(int i=0;i<200;i++)
	{ 
    //auto begin = chrono::high_resolution_clock::now();
	auto end = chrono::high_resolution_clock::now();  
	auto dur = end - begin;
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    cout << ms << endl;
	}
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