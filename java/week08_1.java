import java.util.*;

import java.util.Scanner;

public class week08_1{
    
    //public char[][] canvas = null;
    //public int weight = -1;
    //public int height = -1;

    public void main(String[] args) {

        //System.out.println(canvas[0].length);

        //System.out.println(canvas.length);
        
        int number;
        /*createCanvas(8,4);
        renderInt();
        filledAll((char)0);
        renderInt();
        setPoint(3,2,'X');
        renderInt();
        setAround(3,2,(char)1);
        renderInt();
        setPoint(5,2,'O');
        renderInt();
        sumAround(5,2,(char)1);
        renderInt();*/
        int weight = -1;
        int height = -1;
        class_draw_array[0] = new Draw(w,h);

        //已存在五個實例A,B,C,D,E
//若存在一陣列class_draw_array
//class_draw_array內容為{A,B,C,D,E}
//Draw current_draw = null

//切換至0 如指標的操作
//current_draw = class_draw_array[0]
//其運作原理也等價於指標
//所以此時
// current_draw == A
//若使用current_draw調用方法
//current_draw.setPoint(x,y,'A')
//效果等價於
//A.setPoint(x,y,'A')

        /*while(true){
        Scanner scanner = new Scanner(System.in);

        }
        */
    }
}

class Draw{
    public boolean createCanvas(int w,int h){
        if(weight == -1 || height == -1){
            weight = w;
            height = h;
            canvas = new char[height][weight];
            return true;
        }
        return false;
    }

    public void filledAll(char c){
        for(int i=0;i<canvas.length;i++)
        {
            for(int j=0;j<canvas[i].length;j++)
            {
                setPoint(j,i,c);
            }
        }
        // use for loop and `setPoint` function to complete
    }

    public boolean setPoint(int x,int y,char c){
        if(isOutSide(x,y))
        {
            return false;
        }
        else
        {
            canvas[y][x] = c;
            return true;
        }

        // check Whether  `x` and `y` in canvas

        // outside return false

        // inside set c on canvas[y][x] and return true
    }

    public boolean sumPoint(int x,int y,char c){
        if(isOutSide(x,y))
        {
            return false;
        }
        else
        {
            int number = (int)canvas[y][x] + (int)c;
            //System.out.println(number);
            canvas[y][x] = (char)number;
            return true;
        }
    }

    public int getPoint(int x,int y){
        if(isOutSide(x,y)) return -1;
        else{
            return canvas[y][x];
        }
    }

    public boolean isOutSide(int x,int y){
        if(x < 0 || y < 0) return true;
        if(x >= weight || y >= height) return true;
        return false;
    }

    public void drawSquare(int x1,int y1,int x2,int y2,char c){
        for(int i=x1;i<x2+1;i++)
        {
            for(int j=y1;j<y2+1;j++)
            {
                setPoint(i,j,c);
            }
        }
        //use `setPoint()` to complete
    }

    public void setAround(int x,int y,char c){
        for(int i=x-1;i<x+2;i++)
        {
            for(int j=y-1;j<y+2;j++)
            {
                if(i!=x | y!=j)
                {
                    setPoint(i,j,c);
                }
            }
        }
        //use `setPoint()` to complete
    }

    public void sumAround(int x,int y,char c){
        for(int i=x-1;i<x+2;i++)
        {
            for(int j=y-1;j<y+2;j++)
            {
                if(i!=x | y!=j)
                {
                    sumPoint(i,j,c);
                }
            }
        }
    }

    public void render(){
        System.out.print(" ");
        for(int i=0;i<weight;i++) System.out.printf(" %d",i);
        System.out.println();
        for(int y=0;y<height;y++){
            System.out.print(y);
            for(int x=0;x<weight;x++){
                System.out.printf(" %c",getPoint(x,y));
            }
            System.out.println();
        }
        System.out.println();
    }

    public void renderInt(){
        System.out.print(" ");
        for(int i=0;i<weight;i++) System.out.printf(" %3d",i);
        System.out.println();
        for(int y=0;y<height;y++){

            System.out.print(y);
            for(int x=0;x<weight;x++){
                System.out.printf(" %3d",getPoint(x,y));
            }
            System.out.println();
        }
        System.out.println();
    }
}
