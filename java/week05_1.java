import java.util.Scanner;

public class week05_1
{
    public static void main(String[] args)
    {
        int m = 2, n = 3, p = 2;
        Scanner sc = new Scanner(System.in);
        int[] m1 = new int[m*n],m2 = new int[n*p],m3 = new int[n*n];
        for(int i=0;i<m*n;i++)
        {
            m1[i] = sc.nextInt();
        }
        for(int i=0;i<n*p;i++)
        {
            m2[i] = sc.nextInt();
        }
        for(int i=0;i<m;i++)
        {
            for(int j=0;j<p;j++)
            {
                for(int k=0;k<n;k++)
                {
                    m3[j+i*2] += m1[i*3+k] * m2[j+k*2];
                }
            }
        }
        for(int i=0;i<m*p;i++)
        {
            System.out.print(m3[i] + " ");
        }
    }
}