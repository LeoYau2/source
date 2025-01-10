import java.util.Scanner;

public class week05_hw1
{
    public static void main(String[] args)
    {
        int[] array = new int[5];
        Scanner sc = new Scanner(System.in);
        for(int i=0;i<5;i++)
        {
            array[i] = sc.nextInt();
        }
        System.out.println("Max = " + arrayMax(array));
        System.out.println("Min = " + arrayMin(array));
    }
    static int arrayMax(int[] input)
    {
        int max = 0;
        for(int i=0;i<5;i++)
        {
            if(input[i] > max)
            {
                max = input[i];
            }
        }
        return(max);
    }
    static int arrayMin(int[] input)
    {
        int min = 1000;
        for(int i=0;i<5;i++)
        {
            if(input[i] < min)
            {
                min = input[i];
            }
        }
        return(min);
    }
}
