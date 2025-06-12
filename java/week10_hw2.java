public class week10_hw2 {
    static int square(int intput)
    {
        return intput*intput;
    }
    static double square(double intput)
    {
        return intput*intput;
    }
    static int getMax(int intput_0,int intput_1,int intput_2)
    {
        int max = intput_0;
        if(intput_1>max)
        {
            max = intput_1;
        }
        if(intput_2>max)
        {
            max = intput_2;
        }
        return max;
    }
    static int getMax(int intput_0,int intput_1,int intput_2,int intput_3)
    {
        int max = intput_0;
        if(intput_1>max)
        {
            max = intput_1;
        }
        if(intput_2>max)
        {
            max = intput_2;
        }
        if(intput_3>max)
        {
            max = intput_3;
        }
        return max;
    }
    public static void main(String[] args)
    {
        System.out.println("square 4 = " + square(4));
        System.out.println("square 4.7 = " + (4.7));
        System.out.println("Max of 1,2,3 = " + getMax(1,2,3));
        System.out.println("Max of 1,2,3,4 = " + getMax(1,2,3,4));
    }
}
