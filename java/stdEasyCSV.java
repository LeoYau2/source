import java.util.Scanner;
import java.io.*;

public class stdEasyCSV{
    private static String file_name = null;
    public static void main (String[] args) {
       file_name = "weather.csv";
       String[] strs = getCsvRow(7);
       // show function todo
       // check and refresh output file
       checkOutputFile("output.txt");
       // How to split string in Java
       // refer to : https://www.runoob.com/java/java-string-split.html
        int[] array = new int[49];
        int j = 0, sum=0;

        for(int i=1;i<7;i++){
            for (String retval: strs[i].split(",")){
                array[j] = Integer.parseInt(retval);
                //System.out.println(retval+array[j]);
                j++;
            }
        }
        for(int i=0;i<49;i++){
                //System.out.println(array[i]);
        }
       // And How to Convert string to int
       // refer to : http://tw.gitbook.net/java/lang/integer_parseint.html
       for(int i=0;i<strs.length-1;i++){
           // Do Something to Finish HomeWork
           for(int k=1;k<8;k++)
           {
               sum += array[i*8+k];
           }
           writeStrTo("output.txt",(sum/7.0 + "\n")); // write content to fie
           sum = 0;
       }
    }
    private static String[] getCsvRow(int read_row_num){
        int array_len = read_row_num;
        String[] str_array = new String[array_len];
        if(file_name==null){
            Scanner sc = new Scanner(System.in);
            file_name = sc.next();
            sc.close();
        }
        try{
            File f = new File(file_name);
            InputStream f_in = new FileInputStream(f);
            InputStreamReader f_reader = new InputStreamReader(f_in);
            BufferedReader br = new BufferedReader(f_reader);

            for(int i=0;i<array_len;i++){
                str_array[i] = br.readLine();
                if(str_array[i] == null){
                    break;
                }
            }
            br.close();
        }catch(IOException e){
            e.printStackTrace();
            return null;
        }
        return str_array;
    }

    private static boolean checkOutputFile(String output_file_name){
        try{
            File f = new File(output_file_name);
            if(!f.exists()){
                f.createNewFile();
            }
            OutputStream f_out = new FileOutputStream(f);
            OutputStreamWriter f_writer = new OutputStreamWriter(f_out);
            BufferedWriter bw = new BufferedWriter(f_writer);
            bw.write("");
            bw.flush();
            bw.close();
        }
        catch(IOException e){
            e.printStackTrace();
            return false;
        }
        return true;
    }

    private static boolean writeStrTo(String output_file_name,String data){
        System.out.println(data);
        try{
            File f = new File(output_file_name);
            // About Java's append and write
            // Refer to : https://stackoverflow.com/questions/8544771/how-to-write-data-with-fileoutputstream-without-losing-old-data
            // or : https://stackoverflow.com/questions/1625234/how-to-append-text-to-an-existing-file-in-java
            OutputStream f_out = new FileOutputStream(f,true);
            OutputStreamWriter f_writer = new OutputStreamWriter(f_out);
            BufferedWriter bw = new BufferedWriter(f_writer);

            bw.write(data);
            bw.flush();
            bw.close();

        }catch(IOException e){
            e.printStackTrace();
            return false;
        }
        return true;
    }
}
