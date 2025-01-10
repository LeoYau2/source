import java.util.Scanner;

public class week04_01
{
    public static void main(String[] args)
    {
        Scanner sc = new Scanner(System.in);
        String input = sc.nextLine();
        System.out.println(decoder(encoder(input)));
    }
    public static String encoder(String str)
    {
        char[] char_array = str.toCharArray();
        int size = str.length();
        for(int i=0;i<size;i++)
        {
            char_array[i] += 3;
        }
        str = String.valueOf(char_array);
        return(str);
    }
    public static String decoder(String str)
    {
        char[] char_array = str.toCharArray();
        int size = str.length();
        for(int i=0;i<size;i++)
        {
            char_array[i] += -3;
        }
        str = String.valueOf(char_array);
        return(str);
    }
}
