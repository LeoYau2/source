public class week01 {
    public static void main(String[] args) {
        int num1, num2, num3, max, min;
        java.util.Scanner sc =
            new java.util.Scanner(System.in);
        num1 = sc.nextInt();
        num2 = sc.nextInt();
        num3 = sc.nextInt();
        max = num1;
        if(num2>num1)
        {
            max = num2;
        }
        if(num3>num2)
        {
            max = num3;
        }
        min =num1;
        if(num2<num1)
        {
            max = num2;
        }
        if(num3<num2)
        {
            max = num3;
        }
        System.out.println("1. 總和 = " + (num1+num2+num3) + "\n2. 乘積 = " + (num1*num2*num3) + "\n3. 最大值 = " + max + "\n4. 最小值 = " + min);
    }
}