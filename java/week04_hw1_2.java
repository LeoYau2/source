import java.util.Scanner;

public class week04_hw1_2
{
    public static void main(String[] args)
    {
        int[] array = new int[6];
        int max = 0,index = 0,sum = 0;
        Scanner sc = new Scanner(System.in);
        for(int i=0;i<6;i++)
        {
            array[i] = sc.nextInt();
        }
        for(int i=0;i<6;i++)
        {
            if(array[i]>max)
            {
                max = array[i];
                index = i;
            }
        }
        System.out.println("最大的數字 = " + max);
        System.out.println("索引值 = " + index);
    }
}
