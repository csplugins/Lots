import java.io.*;
import java.awt.*;
import javax.swing.*;
import java.util.Scanner;
import static java.lang.Math.*;
import javax.imageio.*;
import java.awt.image.BufferedImage;

/**
 * @author Cody Skala
 * @dateCreated 04/09/2016 14:17
 * This program will read pgm files and remove seems based off lowest significance level
 */
public class SeemCarving{
    public static void main(String[] args) throws FileNotFoundException, IOException{
      if(args.length != 3){
        System.out.println("Usage: java SeemCarving filename x y");
        System.exit(0);
      }

      ToDraw td;
        JFrame frame = new JFrame("Seem Carving");

        String t = args[0];
        t = t.substring(t.length() - 3, t.length());
        if(t.equals("pbm") || t.equals("pgm") || t.equals("ppm")){
          td = new ToDraw(args[0], Integer.parseInt(args[1]), Integer.parseInt(args[2]));
          JScrollPane scroll = new JScrollPane(td,JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
          frame.add(scroll);
        }
        else if(t.equals("png") || t.equals("jpg") || t.equals("gif") || t.equals("bmp")){
          td = new ToDraw(args[0], Integer.parseInt(args[1]), Integer.parseInt(args[2]), 0);
          JScrollPane scroll = new JScrollPane(td,JScrollPane.VERTICAL_SCROLLBAR_AS_NEEDED,JScrollPane.HORIZONTAL_SCROLLBAR_AS_NEEDED);
          frame.add(scroll);
        }
        else{
          System.out.println("Error: Invalid file format!");
          System.exit(0);
        }

        frame.setSize(500, 500);
        frame.setExtendedState(JFrame.MAXIMIZED_BOTH);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setVisible(true);
    }
}

final class ToDraw extends JPanel{
    int arrR[][], arrG[][], arrB[][];
    int newArrR[][], newArrG[][], newArrB[][];
    int arr[][];
    int vertical, horizontal;
    int x, y, largest;
    ToDraw(String str, int vertical1, int horizontal1) throws FileNotFoundException, IOException{
        Scanner in = new Scanner(new FileReader(str));
        String[] comment = new String[2];
        String toSave;
        String type = in.nextLine();
        toSave = type + "\n";
        comment[0] = in.nextLine();
        while(comment[0].startsWith("#")){
            toSave += comment[0] + "\n";
            comment[0] = in.nextLine();
        }
        comment = comment[0].split(" ");
        x = Integer.parseInt(comment[0]);
        y = Integer.parseInt(comment[1]);
        if(type.equals("P1")){
            largest = 1;
        }
        else largest = in.nextInt();
        arr = new int[x][y];
        arrR = new int[x][y];
        arrG = new int[x][y];
        arrB = new int[x][y];
        vertical = vertical1;
        horizontal = horizontal1;
        toSave += (x-vertical) + " " + (y-horizontal) + "\n";
        if(!type.equals("P1"))
            toSave += largest + "\n";
        if(vertical == x && horizontal != 0){
          System.out.println("Error: Can't remove all vertical seems followed by a horizontal!");
          System.exit(0);
        }
        if(vertical > x){
            System.out.println("Error: Not enough horizontal seems to remove!");
            System.exit(0);
        }
        if(horizontal > y){
            System.out.println("Error: Not enough vertical seems to remove!");
            System.exit(0);
        }
        for(int j = 0; j < y; j++){
            for(int i = 0; i < x; i++){
                arrR[i][j] = in.nextInt();
                if(type.equals("P3")){
                    arrG[i][j] = in.nextInt();
                    arrB[i][j] = in.nextInt();
                }
                else{
                    arrG[i][j] = arrR[i][j];
                    arrB[i][j] = arrR[i][j];
                }
                arr[i][j] += arrR[i][j];
                arr[i][j] += arrG[i][j];
                arr[i][j] += arrB[i][j];
            }
        }
        in.close();
        removeSeems(horizontal, vertical, arr, arrR, arrG, arrB);
        String file = str.substring(0, str.length()-4) + "_processed.";
        if(type.equals("P1")) file += "pbm";
        else if(type.equals("P2")) file += "pgm";
        else file += "ppm";
        System.out.println("Writing processed file: " + file);
        File outFile = new File (file);
        FileWriter fWriter = new FileWriter (outFile);
        PrintWriter pWriter = new PrintWriter (fWriter);
        pWriter.print(toSave);
        for(int j = 0; j < y-horizontal; ++j){
            for(int i = 0; i < x-vertical; ++i){
                pWriter.print(newArrR[i][j] + " ");
                if(type.equals("P3")){
                    pWriter.print(newArrG[i][j] + " ");
                    pWriter.print(newArrB[i][j] + " ");
                }
            }
            if(j != y-horizontal-1)
                pWriter.println("");
        }
        pWriter.close();
        System.out.println("Done writing.");
    }

    ToDraw(String str, int vertical1, int horizontal1, int placeholder) throws FileNotFoundException, IOException{
        BufferedImage bimg = ImageIO.read(new File(str));
        largest = 255;
        x = bimg.getWidth();
        y = bimg.getHeight();
        vertical = vertical1;
        horizontal = horizontal1;
        arr = new int[x][y];
        arrR = new int[x][y];
        arrG = new int[x][y];
        arrB = new int[x][y];
        if(vertical > x){
            System.out.println("Error: Not enough horizontal seems to remove!");
            System.exit(0);
        }
        if(horizontal > y){
            System.out.println("Error: Not enough vertical seems to remove!");
            System.exit(0);
        }
        for(int j = 0; j < y; j++){
            for(int i = 0; i < x; i++){
              Color c = new Color(bimg.getRGB(i,j));
                arrR[i][j] = c.getRed();
                arrG[i][j] = c.getGreen();
                arrB[i][j] = c.getBlue();
                arr[i][j] += arrR[i][j];
                arr[i][j] += arrG[i][j];
                arr[i][j] += arrB[i][j];
            }
        }
        removeSeems(horizontal, vertical, arr, arrR, arrG, arrB);
        String file = str.substring(0, str.length() - 4) + "_processed.";
        file += str.substring(str.length() - 3, str.length());
        System.out.println("Writing processed file: " + file);
        BufferedImage bi = new BufferedImage(x-vertical, y-horizontal, BufferedImage.TYPE_INT_ARGB);
        Graphics2D ig2 = bi.createGraphics();
        for(int j = 0; j < y-horizontal; ++j){
            for(int i = 0; i < x-vertical; ++i){
                ig2.setPaint(new Color(newArrR[i][j],newArrG[i][j],newArrB[i][j]));
                ig2.drawLine(i,j,i,j);
            }
        }
        ImageIO.write(bi, "PNG", new File(file));
      }

    public void removeSeems(int h, int v, int[][] tempArr, int[][] createdR, int[][] createdG, int[][] createdB){
        if(h == 0 && v == 0) return;
        int[][] energy = new int[x-vertical+v][y-horizontal+h];
        for(int j = 0; j < y-horizontal+h; ++j){
            for(int i = 0; i < x-vertical+v; ++i){
                if(i - 1 > -1)
                    energy[i][j] += abs(tempArr[i][j] - tempArr[i-1][j]);
                if(j - 1 > -1)
                    energy[i][j] += abs(tempArr[i][j] - tempArr[i][j-1]);
                if(i + 1 < x-vertical+v)
                    energy[i][j] += abs(tempArr[i][j] - tempArr[i+1][j]);
                if(j + 1 < y-horizontal+h)
                    energy[i][j] += abs(tempArr[i][j] - tempArr[i][j+1]);
            }
        }
        if(v > 0){
            for(int j = 1; j < y-horizontal+h; ++j){
                for(int i = 0; i < x-vertical+v; ++i){
                    int min = energy[i][j-1];
                    if(i-1 > -1)
                        if(energy[i-1][j-1] < min)
                            min = energy[i-1][j-1];
                    if(i+1 < x-vertical+v)
                        if(energy[i+1][j-1] < min)
                            min = energy[i+1][j-1];
                    energy[i][j] += min;
                }
            }
            int minNum = energy[0][y-horizontal+h-1];
            int minPos = 0;
            for(int i = 1; i < x-vertical+v; ++i){
                if(energy[i][y-horizontal+h-1] < minNum){
                    minNum = energy[i][y-horizontal+h-1];
                    minPos = i;
                }
            }
            energy[minPos][y-horizontal+h-1] = -1;
            for(int j = y-horizontal+h-2; j > -1; --j){
                int min = energy[minPos][j];
                int tempMinPos = minPos;
                if(tempMinPos - 1 > -1)
                    if(energy[tempMinPos-1][j] < min){
                        min = energy[tempMinPos-1][j];
                        minPos = tempMinPos - 1;
                    }
                if(tempMinPos + 1 < x-vertical+v)
                    if(energy[tempMinPos+1][j] < min){
                        minPos = tempMinPos + 1;
                    }
                energy[minPos][j] = -1;
            }
            int[][] created = new int[x-vertical+v-1][y-horizontal+h];
            for(int j = 0; j < y-horizontal+h; ++j){
                int offset = 0;
                for(int i = 0; i < x-vertical+v; ++i){
                    if(energy[i][j] == -1)
                        offset = 1;
                    else created[i-offset][j] = tempArr[i][j];
                }
            }
            newArrR = new int[x-vertical+v-1][y-horizontal+h];
            newArrG = new int[x-vertical+v-1][y-horizontal+h];
            newArrB = new int[x-vertical+v-1][y-horizontal+h];
            for(int j = 0; j < y-horizontal+h; ++j){
                int offset = 0;
                for(int i = 0; i < x-vertical+v; ++i){
                    if(energy[i][j] == -1)
                        offset = 1;
                    else{
                        newArrR[i-offset][j] = createdR[i][j];
                        newArrG[i-offset][j] = createdG[i][j];
                        newArrB[i-offset][j] = createdB[i][j];
                    }
                }
            }
            removeSeems(h, v-1, created, newArrR, newArrG, newArrB);
        }
        else{
            for(int i = 1; i < x-vertical+v; ++i){
                for(int j = 0; j < y-horizontal+h; ++j){
                    int min = energy[i-1][j];
                    if(j-1 > -1)
                        if(energy[i-1][j-1] < min)
                            min = energy[i-1][j-1];
                    if(j+1 < y-horizontal+h)
                        if(energy[i-1][j+1] < min)
                            min = energy[i-1][j+1];
                    energy[i][j] += min;
                }
            }
            int minNum = energy[x-vertical+v-1][0];
            int minPos = 0;
            for(int i = 1; i < y-horizontal+h; ++i){
                if(energy[x-vertical+v-1][i] < minNum){
                    minNum = energy[x-vertical+v-1][i];
                    minPos = i;
                }
            }
            energy[x-vertical+v-1][minPos] = -1;
            for(int j = x-vertical+v-2; j > -1; --j){
                int min = energy[j][minPos];
                int tempMinPos = minPos;
                if(tempMinPos - 1 > -1)
                    if(energy[j][tempMinPos-1] < min){
                        min = energy[j][tempMinPos-1];
                        minPos = tempMinPos - 1;
                    }
                if(tempMinPos + 1 < y-horizontal+h)
                    if(energy[j][tempMinPos+1] < min){
                        minPos = tempMinPos + 1;
                    }
                energy[j][minPos] = -1;
            }
            int[][] created = new int[x-vertical+v][y-horizontal+h-1];
            for(int j = 0; j < x-vertical+v; ++j){
                int offset = 0;
                for(int i = 0; i < y-horizontal+h; ++i){
                    if(energy[j][i] == -1)
                        offset = 1;
                    else created[j][i-offset] = tempArr[j][i];
                }
            }
            newArrR = new int[x-vertical+v][y-horizontal+h-1];
            newArrG = new int[x-vertical+v][y-horizontal+h-1];
            newArrB = new int[x-vertical+v][y-horizontal+h-1];
            for(int j = 0; j < x-vertical+v; ++j){
                int offset = 0;
                for(int i = 0; i < y-horizontal+h; ++i){
                    if(energy[j][i] == -1)
                        offset = 1;
                    else{
                        newArrR[j][i-offset] = createdR[j][i];
                        newArrG[j][i-offset] = createdG[j][i];
                        newArrB[j][i-offset] = createdB[j][i];
                    }
                }
            }
            removeSeems(h-1, v, created, newArrR, newArrG, newArrB);
        }
    }

    @Override
    public void paintComponent(Graphics g){
        Graphics2D g2 = (Graphics2D) g;
        Rectangle reqt = new Rectangle(0,0,getWidth(),getHeight());
        g2.setColor(Color.YELLOW);
        g2.fill(reqt);
        g2.draw(reqt);
        g2.setColor(Color.BLUE);
        g2.drawString("Original", 20, 15);
        g2.drawString("Processed", x+100, 15);
        for(int j = 0; j < y; j++){
            for(int i = 0; i < x; i++){
                int R = arrR[i][j] *  255 / largest;
                int G = arrG[i][j] *  255 / largest;
                int B = arrB[i][j] *  255 / largest;
                g2.setColor(new Color(R,G,B));
                g2.drawLine(i+20,j+20,i+20,j+20);
            }
        }

        for(int j = 0; j < y-horizontal; j++){
            for(int i = 0; i < x-vertical; i++){
                int R = newArrR[i][j] *  255 / largest;
                int G = newArrG[i][j] *  255 / largest;
                int B = newArrB[i][j] *  255 / largest;
                g2.setColor(new Color(R,G,B));
                g2.drawLine(i+x+100,j+20,i+x+100,j+20);
            }
        }
    }

    @Override
    public Dimension getPreferredSize(){
        return new Dimension(x+x+120,y+40);
    }
}
