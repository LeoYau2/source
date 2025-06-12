import java.util.*;

import java.util.Scanner;

private boolean switchTo(int x,int y,int src,char dest){
    if(getPoint(x,y)==src) return setPoint(x,y,dest);
    else return false;
}

private void filledInBy(int x,int y,int src,char dest){
    if(switchTo(x,y,src,dest)){
        filledInBy(x-1,y-1,src,dest);
        filledInBy(x,y-1,src,dest);
        filledInBy(x+1,y-1,src,dest);
        filledInBy(x-1,y,src,dest);
        filledInBy(x+1,y,src,dest);
        filledInBy(x-1,y+1,src,dest);
        filledInBy(x,y+1,src,dest);
        filledInBy(x+1,y+1,src,dest);
    }
}
public void filledIn(int x,int y,char c){
    if(isAnchor(x,y)) return;
    filledInBy(x,y,getPoint(x,y),c);
}

public Minemap(int size_x,int size_y,int mine_num){
        map = new draw(size_x,size_y);
        // random your mine on your map
        // set Anchor with something you don't want to be modified
}

public class Minesweeper{
    public static void main (String[] args) {
       //new Scanner let player can create random map
       Minemap new_game = new Minemap(8,5,5);
       new_game.showMapStatus();
    }
}

class Minemap{
    draw map;
    public Minemap(int size_x,int size_y,int mine_num){
        map = new draw(size_x,size_y);
        // random your mine on your map
        // set Anchor with something you don't want to be modified
    }
    private void setBomb(int x,int y){
        if(map.setPoint(x,y,/*number you define for bomb*/)){
            map.sumAround(x,y,(char)1);
        }
    }
    public void showMapStatus(){
        map.renderInt();
    }
}

public int touchEvent(int x,int y){
    if(display_map.isOutSide(x,y)) return /*something you define*/;
    int status = map.getPonit(x,y);
    if(status >= 0 && status <= 8){
        /*do someing*/
        return status;
    }
    else if(/*...if is bomb...*/) {
        /*do something*/
        return result;
    }
    else if(/*...if is space...*/){
    /*openMap(x,y,status)*/
    }
}

public void showMap(){
    display_map.render();
}

private boolean setFlag(int x,int y){
    if(display_map.getPoint(x,y)==0){
        /*do something*/
    }
}

class draw{

}

public class week10_1{
private static char[][] canvas = null;
private static int weight = -1;
private static int height = -1;
public static void main(String[] args) {
int number;
draw canvas = new draw(8,4);
//createCanvas(8,4);
canvas.renderInt();
canvas.filledAll((char)0);
canvas.renderInt();
canvas.setPoint(3,2,'X');
canvas.renderInt();
canvas.setAround(3,2,(char)1);
canvas.addAnchor('X');
canvas.addAnchor('O');
canvas.renderInt();
canvas.setPoint(4,2,'O');
canvas.renderInt();
canvas.sumAround(4,2,(char)1);
canvas.renderInt();
canvas.setPoint(5,2,'X');
canvas.renderInt();
canvas.sumAround(5,2,(char)1);
canvas.renderInt();
canvas.filledIn(0,0,'A');
canvas.renderInt();

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
