import java.awt.image.BufferedImage;
import javax.imageio.ImageIO;
import java.io.*;

public class PngToArray{

    public String imageName;
    public BufferedImage image;
    public String[] values;

    public PngToArray(String path, String imageName) {
        try {
            this.imageName = imageName;
            this.image = ImageIO.read(new File(path));
            this.values = new String[4096];
        } catch (IOException e) {
            e.printStackTrace();
            this.image = null;
            this.values = null;
        }
    }
    

    public void convertPngToArray(){
        for(int i = 0; i < image.getWidth(); i++){
            for(int j = 0; j < image.getHeight(); j++){
                int pixel = image.getRGB(i, j);

                int red = (pixel >> 16) & 0xff;
                int green = (pixel >> 8) & 0xff;
                int blue = pixel & 0xff;

                red = (int)Math.floor((red*31)/255);
                green = (int)Math.floor((green*63)/255);
                blue = (int)Math.floor((blue*31)/255);

                int rgb = (red << 11) + (green << 5) + blue;

                String hex = String.format("0X%04X", rgb);
                values[64*j + i] = hex;
            }
        }

        try (FileWriter writer = new FileWriter(imageName + ".csv")) {
            for (String item : values) {
                writer.write(item + ",");
            }
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