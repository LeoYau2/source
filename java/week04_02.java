import java.util.Scanner;

public class week04_02
{
    public static void main(String[] args)
    {
        int input;
        Scanner sc = new Scanner(System.in);
        input = sc.nextInt();
        for(int i=0;i<input;i++)
        {
            for(int j=0;j<input-i;j++)
            {
                System.out.print("*");
            }
            for(int k=0;k<i*2;k++)
            {
                System.out.print(" ");
            }
            for(int j=0;j<input-i;j++)
            {
                System.out.print("*");
            }
            System.out.print("\n");
        }
    }
}