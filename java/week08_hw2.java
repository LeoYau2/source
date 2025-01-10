class Person {
    private int id;
    private String name;
    private double height;

    public void setID(int id) {
        this.id = id;
    }

    public void setName(String n) {
        name = n;
    }

    public void setHeight(double h) {
        height = h;
    }

    public void personInfo() {
        System.out.println("字號 : " + id);
        System.out.println("姓名 : " + name);
        System.out.println("身高 : " + height);
    }
}

class Student extends Person {
    private int score;

    public Student(int id, String n, double h, int score) {
        setID(id);
        setName(n);
        setHeight(h);
        this.score = score;
    }

    public void studentInfo() {
        System.out.println("[ 學生資料 ]=====");
        personInfo();
        System.out.println("成績 : " + score);
    }

}

public class CH12_1_2 {
    public static void main(String[] args) {
        Student joe = new Student(1234, " 陳會安 ", 175.0, 85);
        Student jane = new Student(3456, " 江小魚 ", 150.0, 65);
        jane.setHeight(158.0);
        joe.studentInfo();
        jane.studentInfo();
    }
}
