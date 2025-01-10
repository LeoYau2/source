class Card
{
    /* 欄位 */
    private String name;
    private int age;
    private PhoneList phone;
    private String email;

    /* 建構方法 */
    public Card(String name, int age, PhoneList phone, String email)
    {
        this.name = name;
        this.age = age;
        this.phone = phone;
        this.email = email;
    }

    public Card() { this("", 0, new PhoneList(), "@"); }

    /* 存取方法 */
    public void setName(String name) { this.name = name; }
    public void setAge(int age) { this.age = age; }
    public void setPhone(PhoneList phone) { this.phone = phone; }
    public void setEmail(String email) { this.email = email; }

    public String getName() { return this.name; }
    public int getAge() { return this.age; }
    public PhoneList getPhone() { return this.phone; }
    public String getEmail() {return this.email; }

    /* 其他方法 */
    public void printCard()
    {
        System.out.println("name:\t" + this.name);
        System.out.println("age:\t" + this.age);
        System.out.println("homephone:\t" + this.phone.getHomePhone());
        System.out.println("officehone:\t" + this.phone.getOfficePhone());
        System.out.println("cellphone:\t" + this.phone.getCellPhone());
        System.out.println("email:\t" + this.email);
    }
}

class PhoneList
{
    /* 欄位 */
    private String homephone;
    private String officehone;
    private String cellphone;

    /* 建構方法 */
    public PhoneList(String home, String office, String cell)
    {
        this.homephone = home;
        this.officehone = office;
        this.cellphone = cell;
    }

    public PhoneList() { this("0","0","0"); }

    /* 存取方法 */
    public void setHomePhone(String s) { this.homephone = s; }
    public void setOfficePhone(String s) { this.officehone = s; }
    public void setCellPhone(String s) { this.cellphone = s; }

    public String getHomePhone() {return this.homephone; }
    public String getOfficePhone() {return this.officehone; }
    public String getCellPhone() {return this.cellphone; }
}

public class week07_hw1_2
{
    public static void main(String[] args)
    {
        Card card_1 = new Card("name_1",18,phone,"email_1");
        PhoneList phoneList_1 = new PhoneList("0000000000","1111111111","2222222222");
        Card_1.printCard();
    }
}
