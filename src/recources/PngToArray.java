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

                String hex = String.format("0X%02X%02X%02X", red, green, blue);
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
        PngToArray pngArrEndScreen = new PngToArray("end_screen.png", "end_screen");


        pngArrStartScreen.convertPngToArray();
        pngArrEndScreen.convertPngToArray();
    }
}