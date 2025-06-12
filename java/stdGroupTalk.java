public class stdGroupTalk{
    public static void main(String[] args) {
        // A<-B->C
        Person A = new Person("A");
        Person B = new Person("B");
        Person C = new Person("C");
        Person[] A_C = {A,C};
        B.talkTo(A_C);
        B.talk("Hello World");
        System.out.println("Stage 1");
        System.out.println("A<-"+A.listen());
        System.out.println("B<-"+B.listen());
        System.out.println("C<-"+C.listen());
        // A B C
        A.closeListen();
        B.closeListen();
        C.closeListen();
        System.out.println("Stage 2");
        System.out.println("A<-"+A.listen());
        System.out.println("B<-"+B.listen());
        System.out.println("C<-"+C.listen());
        // (A->(B->C))
        B.talkTo(C);
        A.talkTo(B);
        B.talk("Hello C");
        A.talk("Hello B");
        System.out.println("Stage 3");
        System.out.println("A<-"+A.getFocusName()+" : "+A.listen());
        System.out.println("B<-"+B.getFocusName()+" : "+B.listen());
        System.out.println("C<-"+C.getFocusName()+" : "+C.listen());
        // (A->(B->(C-<A)))
        C.closeListen();
        C.listenTo(A);
        System.out.println("Stage 4");
        System.out.println("A<-"+A.getFocusName()+" : "+A.listen());
        System.out.println("B<-"+B.getFocusName()+" : "+B.listen());
        System.out.println("C<-"+C.getFocusName()+" : "+C.listen());
        // (A-<(B->(C-<A)))
        A.closeListen();
        A.listenTo(B);
        System.out.println("Stage 5");
        System.out.println("A<-"+A.getFocusName()+" : "+A.listen());
        System.out.println("B<-"+B.getFocusName()+" : "+B.listen());
        System.out.println("C<-"+C.getFocusName()+" : "+C.listen());
        // (reset All)
        A.closeListen();
        B.closeListen();
        C.closeListen();
        System.out.println("");
        // HW : 
        // 1. Complete the simulation below 
        // 2. The contents for talk must read in Stdin
        //      And Talk should be like :
        //          A.talk("A:....")
        //          B.talk("B:....")
        // 3. Relationship diagram placed in iClass
        // 4. Print Everyone Final listened content like :
        //      A : B:....
        //      B : C:....
        //      C : ?:....
        // 5. 5 people
        Person D = new Person("D");
        Person E = new Person("E");
        Person[] group = {A,B,C,D,E};
        System.out.println("Stage 1");
        // A talk to everyone
        A.listen();
        B.listen();
        C.listen();
        D.listen();
        E.listen();
        //-------------------------------
        System.out.println("Stage 2");
        // E and D talk to each other
        // B talk to C
        // C listen to A
        // E and D talk to each other
        B.talk("B2");
        D.talk("D2");
        E.talk("E2");
        A.listen();
        B.listen();
        C.listen();
        D.listen();
        E.listen();
        //-------------------------------
        System.out.println("Stage 3");
        // C and D Listen to B 
        // B talk to C
        // E talk to D
        // A talk to everyone
        A.talk("A3");
        B.talk("B3");
        E.talk("E3");
        //-------------------------------
        System.out.println("Final Listened Contents :");
        System.out.println("A : "+A.listen());
        System.out.println("B : "+B.listen());
        System.out.println("C : "+C.listen());
        System.out.println("D : "+D.listen());
        System.out.println("E : "+E.listen());
    }
}
