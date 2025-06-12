import java.util.*; 

public class Person{

    private String gender = null;
    private String name = null;
    private String job = null;
    private int age = -1;
    private String think_temp = null;
    private String[] memory_temp = new String[7];
    private ArrayList<String> notebook = new ArrayList<String>();
    private String talk_about = null;
    private boolean focus_flag = false;
    private Person focus_on = null;

    public Person(String name){
        this.name = name;
    }
    public Person(String name,int age,String gender,String job){
        this.name = name;
        setInfo(age,gender,job);
    }
    public void setInfo(int age,String gender,String job){
        this.age = age;
        this.gender = gender;
        this.job = job;
    }
    public String getName(){
        return this.name;
    }
    public String getFocusName(){
        if(focus_on == null) return "No one";
        else return focus_on.getName();
    }

    public boolean talkTo(Person person){
        beFocus(person);
        if(person.isFocusOn(this) || !person.isFocus()){
            person.listenTo(this);
            return true;
        }else{
            beRelax();
            return false;
        }
    }
    public boolean talkTo(Person[] group){
        boolean someone_listen = false;
        for(Person p:group){
            if(p == this) continue;
            if(p.listenTo(this)){
                someone_listen = true;
            }
        }
        return someone_listen;
    }
    public void talk(String str){
        talk_about = this.name+" : "+str;
    }
    public String talkAbout(){
        return talk_about;
    }

    public boolean listenTo(Person person){
        if(isFocusOn(person)) return true;
        if(isFocus()){
            return false;
        }else{
            beFocus(person);
            return true;
        }
    }
    public String listen(){
        String heard_temp = null;
        if(focus_on == null){
            if(think_temp == null) return "Nothing";
            else return think_temp;
        }
        else{
            heard_temp = focus_on.talkAbout();
            if(heard_temp != null) think_temp = heard_temp;
            if(think_temp == null) return "No one talk";
            else return think_temp;
        }
    }
    public String listen(Person person){
        think_temp = person.talkAbout();
        if(think_temp == null) return "Not talk";
        else return think_temp;
    }

    public boolean isFocus(){
        return this.focus_flag;
    }
    public boolean isFocusOn(Object something){
        if(something == focus_on) return true;
        else return false;
    }
    public void closeListen(){
        beRelax();
        think_temp = null;
    }

    private void beFocus(Person someone){
        focus_flag = true;
        focus_on = someone;
    }
    private void beRelax(){
        focus_on = null;
        focus_flag = false;
    }

}
