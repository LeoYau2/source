#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;

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
    double p;

    cout << "p = ";
    cin >> p;

    int n, r;
    double p_sum=0, x, c=1;
    printf("n = ");
    scanf("%d",&n);
    printf("r = ");
    scanf("%d",&r);
    //P(X=x) = C(n,x) * p^x * (1-p)^(n-x)
    for(int i=r;i<=n;i++)
    {
        x = c*combination(n,i)*pow(p,i)*pow(1-p,n-i);
        p_sum += x;
        printf("P(%d,%d)=%.16f\n",n,i,x);
    }
    printf("Tested %d times, correct %d times\n",n,r);
    printf("Correct rate = %.5f = %.2f%%\n",(float)r/n,(float)r/n*100);
    printf("Binomial Distribution (%d~%d) = %.10f = %.8f%%\n",r,n,p_sum,p_sum*100);
    system("pause");
}
