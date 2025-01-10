import java.util.Scanner;

public class week04_4
{
    public static void main(String[] args)
    {
        int[] point = new int[10000],count = new int[13],max = new int[13],min = new int[13],sum = new int[13];
        for(int i=2;i<13;i++)
        {
            min[i] = 1000000;
        }
        for(int i=0;i<100;i++)
        {
            for(int j=0;j<10000;j++)
            {
                point[j] = 0;
                point[j] += (Math.ceil(Math.random()*6));
                point[j] += (Math.ceil(Math.random()*6));
                //System.out.println(point[j]+" j");
            }
            for(int j=0;j<10000;j++)
            {
                count[point[j]]++;
                sum[point[j]]++;
            }
            for(int j=2;j<13;j++)
            {
                if(count[j]>max[j])
                {
                    max[j] = count[j];
                }
                if(count[j]<min[j])
                {
                    min[j] = count[j];
                }
            }
            count = new int[13];
        }
        for(int i=2;i<13;i++)
        {
            System.out.println("點數和 " + i + ", 平均次數(比例): " + sum[i]/100 + " (" + sum[i]/10000 + "%), 最高次數(比例): " + max[i] + " (" + max[i]/100 + "%), 最低次數(比例): " + min[i] + " (" + min[i]/100 + "%)");
        }
    }
}
