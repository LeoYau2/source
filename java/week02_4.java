import java.util.Scanner;

public class week02_4
{
    public static void main(String[] args)
    {
        int sum=0, n;
        for(int i=1;i<=500;i++)
        {
            for(int j=1;j<i;j++)
            {
                if(i%j==0)
                {
                    sum+=j;
                }
            }
            if(sum==i)
            {
                System.out.println(i);
            }
            sum=0;
        }
    }
}
