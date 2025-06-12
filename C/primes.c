#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
    int m, n, count, t;
    FILE *txt;
    txt = fopen("primes.txt","w");
    printf("Input integers m, n\n");
    scanf("%d%d",&m,&n);
    for(int i=m;i<=n;i++)
    {
        count = 0;
        for(int j=2;j<i/2;j++)
        {
            if(i%j == 0)
            {
                count++;
            }
        }
        if(count == 0)
        {
            fprintf(txt,"%d\n",i);
        }
    }
}
