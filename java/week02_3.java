import java.util.Scanner;

public class week02_3
{
    public static void main(String[] args)
    {
        int[] numbers = new int[1000];
        int n=0 ;
        Scanner sc = new Scanner(System.in);
        numbers[n] = sc.nextInt();
        while(numbers[n]!=-1)
        {
            n++;
            numbers[n] = sc.nextInt();
        }
        for(int i=0;i<n;i++)
        {
            System.out.print((i+1) + ":" + (i+1) + " ");
            while(numbers[i]!=1)
            {
                if(numbers[i]%2==0)
                {
                    numbers[i] = numbers[i]/2;
                }
                else
                {
                    numbers[i] = 3*numbers[i]+1;
                }
                System.out.print(numbers[i] + " ");
            }
            System.out.print("\n");
        }
    }
}
