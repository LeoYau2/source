class Card
{
    private String name;
    private int age;
    private PhoneList phone;
    private String email;
    public Card(String name, int age, PhoneList phone, String email)
    {
        this.name = name;
        this.age = age;
        this.phone = phone;
        this.email = email;
    }
    public void printCard()
    {
        System.out.println("name: " + name);
        System.out.println("age: " + age);
        System.out.println("homephone: ");
        System.out.println("officehone: ");
        System.out.println("cellphone: ");
        System.out.println("email: ");
    }
}

class PhoneList
{
    private String homephone;
    private String officehone;
    private String cellphone;
    public PhoneList(String home, String office, String cell)
    {
        this.homephone = home;
        this.officehone = office;
        this.cellphone = cell;
    }
}

public class week07_hw1
{
    public static void main(String[] args)
    {
        //Card card_1 = new Card("name_1",18,phone,"email_1");
        PhoneList phoneList_1 = new PhoneList("0000000000","1111111111","2222222222");
        //Card_1.printCard();
    }
}
