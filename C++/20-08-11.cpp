#include <iostream>
using namespace std;

int main()
{
	char s1[8];
	//cout << "\"/\\/\\/\\/\\\"\n\"Double quote \" needs to be expressed by \\\" \"\n\"Single quote \' needs to be expressed by \\\' \"\n\"Backslash \\ needs to be expressed by \\\\\"\n///Thanks God, this class is over!\\\\\\" << endl;
	cout << "\"/\\/\\/\\/\\\"\n\"Double quote \" needs to be expressed by \\\" \"\n";
	cout << "\"Single quote \' needs to be expressed by \\\' \"\n";
	cout << "\"Backslash \\ needs to be expressed by \\\\\"\n";
	cout << "///Thanks God, this class is over!\\\\\\" << endl;
	system("pause");
	
	scanf("%s",&s1);
	printf("%s",s1);
	return 0;
}
