import java.util.*;

import java.util.Scanner;

public class stddraw{

    private static char[][] canvas = null;

    private static int weight = -1;

    private static int height = -1;

    public static void main(String[] args) {

        createCanvas(8,4);

        System.out.println(canvas[0].length);

        System.out.println(canvas.length);

        render();

        filledAll('A');

        render();

        setPoint(3,2,'X');

        render();

        setAround(3,2,'O');

        render();

        drawSquare(5,0,7,3,'*');

        render();

        setAround(0,0,'#');

        render();

        while(true){

            Scanner scanner = new Scanner(System.in);

            

            // Use Scanner to choose Function for work

            // input "-1" exit

        }

    }

    public static boolean createCanvas(int w,int h){

        if(weight == -1 || height == -1){

            weight = w;

            height = h;

            canvas = new char[height][weight];

            return true;

        }

        return false;

    }

    public static void filledAll(char c){

        for(int i=0;i<canvas.length;i++)

        {

            for(int j=0;j<canvas[i].length;j++)

            {

                setPoint(j,i,c);

            }

        }

        // use for loop and `setPoint` function to complete

    }

    public static boolean setPoint(int x,int y,char c){

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

    public static int getPoint(int x,int y){

        if(isOutSide(x,y)) return -1;

        else{

            return canvas[y][x];

        }

    }

    public static boolean isOutSide(int x,int y){

        if(x < 0 || y < 0) return true;

        if(x >= weight || y >= height) return true;

        return false;

    }

    public static void drawSquare(int x1,int y1,int x2,int y2,char c){

        for(int i=x1;i<x2+1;i++)

        {

            for(int j=y1;j<y2+1;j++)

            {

                setPoint(i,j,c);

            }

        }

        //use `setPoint()` to complete

    }

    public static void setAround(int x,int y,char c){

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

    public static void render(){

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

    public static void renderInt(){

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

/*

class Canvas{

    public static Canvas(){

    }

}

*/