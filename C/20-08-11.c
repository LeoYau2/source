#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main()
{
	char s1[6];

	
	scanf("%s",&s1);
	printf("%s\n",s1);
    printf("%d\n",strlen(s1));
    for(int i=0;i<strlen(s1);i++)
    {
        printf("%c",s1[i]);
    }
    //printf("%c",s1[3]);
    char s2[strlen(s1)];
    //printf("\ns2[0]=%c\n",s2[0]);
    for(int i=0;i<strlen(s1);i++)
    {
        s2[i] = s1[i];
    }
    //scanf("%s",&s2);
    printf("\nstrlen(s2)=%d\n",strlen(s2));
    for(int i=0;i<strlen(s2);i++)
    {
        printf("%c",s2[i]);
    }
    printf("\ns2=%s\n",s2);
    system("pause");
	return 0;
}