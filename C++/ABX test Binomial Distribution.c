#include <stdio.h>
#include <math.h>

double combination(int n, int k) {
    //double r = lgamma(n + 1) - lgamma(k + 1) - lgamma(n - k + 1);
    //return exp(r);
    double r = 1;
    for (int i = 1; i <= k; i++)
    {
        r = r*(n - k + i) / i;
    }
    return r;
}

int main()
{
    int n, r;
    double p=0, x, c=1;
    printf("n = ");
    scanf("%d",&n);
    printf("r = ");
    scanf("%d",&r);
    //P(X=x) = C(n,x) * p^x * (1-p)^(n-x)
    for(int i=r;i<=n;i++)
    {
        x = c*combination(n,i)*pow(0.5,i)*pow(0.5,n-i);
        p += x;
        printf("P(%d,%d)=%.10f\n",n,i,x);
    }
    printf("Tested %d times, correct %d times\n",n,r);
    printf("Correct rate = %.5f = %.2f%%\n",(float)r/n,(float)r/n*100);
    printf("Binomial Distribution (%d~%d) = %.10f = %.8f%%\n",r,n,p,p*100);
    system("pause");
}
