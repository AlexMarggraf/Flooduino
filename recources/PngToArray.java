import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
import java.io.*;
import java.util.Arrays;

public class PngToArray{

    public String imageName;
    public BufferedImage image;
    public String[] values;

    public PngToArray(String path, String imageName) {
        try {
            this.imageName = imageName;
            this.image = ImageIO.read(new File(path));
            this.values = new String[2*4096];
        } catch (IOException e) {
            e.printStackTrace();
            this.image = null;
            this.values = null;
        }
    }
    

    public void convertPngToArray(){
        for(int i = 0; i < image.getHeight(); i++){
            for(int j = 0; j < image.getWidth(); j++){
                int pixel = image.getRGB(i, j);

                int red = (pixel >> 16) & 0xff;
                int green = (pixel >> 8) & 0xff;
                int blue = pixel & 0xff;

                red = (int)Math.floor((red*31)/255);
                green = (int)Math.floor((green*63)/255);
                blue = (int)Math.floor((blue*31)/255);

                int rgb1 = (red << 3) + (green >> 3);
                int mask = 0b111;
                int rgb2 = ((green & mask) << 5) + blue;

                String hex1 = String.format("0X%02X", rgb1);
                String hex2 = String.format("0X%02X", rgb2);

                values[2*64*i + 127-(2*j)] = hex1;
                values[2*64*i + 127-(2*j + 1)] = hex2;
            
            }
        }

        try (FileWriter writer = new FileWriter(imageName + ".csv")) {
            int valueMax = 0;
            for (String item : values) {
                writer.write(item + ",");

                if(valueMax >= 39){
                    writer.write("\n");
                    valueMax = -1;
                }

                valueMax += 1;
            }
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args){
        PngToArray pngArrStartScreen = new PngToArray("start_screen.png", "start_screen");
        PngToArray pngArrGameOver = new PngToArray("game_over.png", "game_over");
        PngToArray pngArrYouWon = new PngToArray("you_won.png", "you_won");


        pngArrStartScreen.convertPngToArray();
        pngArrGameOver.convertPngToArray();
        pngArrYouWon.convertPngToArray();
    }
}