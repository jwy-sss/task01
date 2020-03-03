package OutOfDate_Main;

import java.awt.*;
import java.awt.image.*;
import java.io.*;
import javax.imageio.*;

//import tools.*;
import settings.*;

public class PixelSizeCalibrate {
	public static String outputpath = Options.OUTPUTPATH;
	
	
	public static void main(String args[]) {
		
		// Ϊ�˲��� main ����̫��į�������˼�������
		
		(new PixelSizeCalibrate()).Output();
		
		System.out.println("[PixelSizeCalibrate] MP4 Ready!");
		
	}
	
	/**frameID���������ͬ֡���еı���*/
	int frameID = 0;
	/**frameoutput�����Ѿ������ͼ�������*/
	int frameoutput = 0;
	int FPS = 30;
	int preframeAmount = 30; //�൱��0.5��
	boolean makePreframe = false;
	
	/**
	 * ���ͼ�񲢺ϳ�mp4�ļ�
	 */
	public void Output() {
		System.out.println("[PixelSizeCalibrate] Output Starts.");
		
		//����һ����������Ŀ¼���ͼƬ
		long timestamp = System.currentTimeMillis() % 1000;
		String pathname = outputpath+"\\PixelSizeCali"+timestamp;
		File dir = new File(pathname);
		while (dir.exists()) {
			timestamp= ++timestamp % 1000;
			pathname = outputpath+"\\PixelSizeCali"+timestamp;
			dir = new File(pathname);
		}
		dir.mkdirs();
		String prefix = pathname + "\\";
		
		//���ͼƬ
		frameoutput = 0;
		int SizeArr[] = {40,5,6,7,8,9,10,11,12};
		for (int arrP = 0; arrP < 9; arrP++) {
			int PixelSize = SizeArr[arrP];
			makePreframe = true;
			BufferedImage image = generateImg(PixelSize);
			try {
				frameoutput++;
				File outfile = new File(prefix+frameoutput+".png");
				ImageIO.write(image, "png", outfile);
			} catch (Exception e) {
				System.out.println("[PixelSizeCalibrate] Met an error when writing into PNG. Exit.");
				System.exit(0);
			}
		}
		System.out.println("[PixelSizeCalibrate] Output "+ frameoutput +" PNG Files.");
		frameID = 0;
	}
	
	public BufferedImage generateImg(int PixelSize) {
		BufferedImage image = new BufferedImage(1920, 1080, BufferedImage.TYPE_4BYTE_ABGR);
		Graphics g = image.getGraphics();
		encode(g, PixelSize);
		return image;
	}
	
	public void encode(Graphics g, int PixelSize) {
		g.setColor(Color.white);
		g.fillRect(0, 0, 1920, 1080);
		
		//������ϽǶ�λ��
		g.setColor(new Color(0x666666));
		g.fillRect(10, 10, 100, 100);
		g.setColor(Color.black);
		g.fillRect(10, 10, 50, 50);
		
		int x = 110;
		int y = 110;
		Color linebegin = Color.black;
		Color c;
		while (y <= 1080-100-10-PixelSize) {
			x = 110;
			c = linebegin;
			while (x <= 1920-100-10-PixelSize) {
				g.setColor(c);
				g.fillRect(x, y, PixelSize, PixelSize);
				c = c == Color.black ? Color.white : Color.black;
				x += PixelSize;
			}
			linebegin = linebegin == Color.black ? Color.white : Color.black;
			y += PixelSize;
		}
		
		g.setColor(new Color(0x666666));
		g.fillRect(x, y, 100, 100);
		g.fillRect(10, y, 100, 100);
		g.fillRect(x, 10, 100, 100);
	}
}
