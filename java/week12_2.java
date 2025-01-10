import java.io.*;
import java.util.Scanner;
public class week12_2 {

    static char in_letters(char c)
    {
        char [] letters = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z','A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};

        int x = 0;
        for(int i=0;i<letters.length;i++)
        {
            if(c==letters[i])
            {
                x++;
            }
        }
        if(x==1)
        {
            return '1';
        }
        else
        {
            return '0';
        }
    }
    public static void main(String[] args) throws Exception
    {
        String file = "article.txt";
        File article = new File(file);
        BufferedReader input = new BufferedReader(new FileReader(article));
        String str;
        String txt = "1";

        while((str = input.readLine()) != null)
        {
            //System.out.println(str);
            txt = txt.concat(str);
        }

        //System.out.println(txt);

        input.close();
        int n;
        Scanner sc = new Scanner(System.in);
        n = sc.nextInt();

        //System.out.println(txt.charAt(n));
        int index = 1, word = 0;
        while(word<=(n-2))
        {
            while(in_letters(txt.charAt(index))=='1')
            {
                index++;
            }
            while(in_letters(txt.charAt(index))=='0')
            {
                word++;
                index++;
            }

        }
        //System.out.println("word" + word);

        //System.out.println("index" + index + " " + txt.charAt(index));

        while(txt.charAt(index)!=' '&&txt.charAt(index)!='.')
        {
            if(in_letters(txt.charAt(index))=='1')
                System.out.print(txt.charAt(index));
            index++;
        }
        char c = txt.charAt(index);
    }
}
