import java.util.Date;
import java.util.Scanner;

class Time
{
    public int hour;
    public int minute;
    public int second;
    public void setXXX()
    {
        Scanner sc = new Scanner(System.in);
        System.out.println("hour");
        hour = sc.nextInt();
        System.out.println("minute");
        minute = sc.nextInt();
        System.out.println("second");
        second = sc.nextInt();
    }
    public void getXXX()
    {
        //Date date = new Date();
    }
    public void printTime()
    {
        if(hour<10)
        {
            System.out.print("0");
        }
        System.out.print(hour + ":");
        if(minute<10)
        {
            System.out.print("0");
        }
        System.out.print(minute + ":");
        if(second<10)
        {
            System.out.print("0");
        }
        System.out.print(second + "\n");
    }
    public void validateTime()
    {
        if(hour<0 | hour>23 | minute<0 | minute>60 | second<0 | second>60)
        {
            System.out.println("error");
        }
    }
}

public class week06_hw1
{
    public static void main(String[] args)
    {
        Time time;
        time = new Time();
        time.setXXX();
        time.getXXX();
        time.printTime();
        time.validateTime();
    }
}
