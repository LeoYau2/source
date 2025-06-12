import java.util.Scanner;

public class week04_hw2
{
    public static void main(String[] args)
    {
        int[] array = new int[20];
        int min_id = 0,number = 0;
        for(int i=0;i<20;i++)
        {
            array[i] += Math.floor(Math.random()*100);
            if(array[i] == 100)
            {
                array[i] = 99;
            }
        }
        System.out.print("原始:   ");
        for(int i=0;i<20;i++)
        {
            System.out.print(" " + array[i]);
        }
        for(int i=0;i<19;i++)
        {
            min_id = i;
            for(int j=i+1;j<20;j++)
            {
                if(array[min_id]>array[j])
                {
                    min_id = j;
                }
            }
            number = array[i];
            array[i] = array[min_id];
            array[min_id] = number;
            System.out.print("\nstep " + (i+1) + " :");
            for(int j=0;j<20;j++)
            {
                System.out.print(" " + array[j]);
            }
        }
    }
}
