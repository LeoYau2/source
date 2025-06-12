import java.util.Scanner;

public class week04_hw1_1
{
    public static void main(String[] args)
    {
        int[] array = {0,1,2,3,4,5,6,7,8,9,10,11};
        int sum = 0;
        for(int i=0;i<12;i++)
        {
            sum += array[i];
        }
        System.out.println("總和 = " + sum);
        System.out.println("平均 = " + sum/12);
    }
}
