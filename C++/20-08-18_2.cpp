#include <iostream>
using namespace std;

int plus_5(int *score)
{
    *score += 5;
}

int minus_3(int *score)
{
    *score += -3;
}

int plus_2(int *score)
{
    *score += 2;
}

int main()
{
    int score_1, score_2;
    cin >> score_1 >> score_2;
    if(score_1 > score_2)
    {
        plus_5(&score_1);
        minus_3(&score_2);
    }else if(score_2 > score_1)
    {
        plus_5(&score_1);
        minus_3(&score_2);
    }else
    {
        plus_2(&score_1);
        plus_2(&score_2);
    }
    cout << "score(1) = " << score_1 << "\nscore(2) = " << score_2;
    system("pause");
    return 0;
}