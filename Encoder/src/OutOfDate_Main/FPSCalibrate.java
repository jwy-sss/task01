package OutOfDate_Main;

import java.awt.*;
import java.awt.image.*;
import java.io.*;
import javax.imageio.*;

import tools.*;
import settings.*;
/**
 * FPS Calibrate������У��֡��<br>
 * һ��˵ С��8 �ֻ�����Ƶ¼��֡��Ϊ30֡��60֡�Ǻ��ںϳɵ�<br>
 * ��ͨ��ʾ����ˢ����Ϊ 60Hz<br>
 * <br>
 * ������Դ�main�������ܹ�ʵ������15��30��60֡����Ƶ<br>
 * ע���������Ƶ��FPS=60�ģ�ͨ��2�Ż����ظ���ʵ��30֡��Ч��<br>
 * ����ͨ���޸ĳ��� outputpath ���ı����Ŀ¼
 */
@SuppressWarnings("unused")
public class FPSCalibrate {
	public static String outputpath = Options.OUTPUTPATH;
	
	public static void main(String args[]) {
		
		// Ϊ�˲��� main ����̫��į�������˼�������
		
		(new FPSCalibrate()).Output();
		
		System.out.println("[FPSCalibrate] MP4 Ready!");
		
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
		System.out.println("[FPSCalibrate] Output Starts.");
		
		//����һ����������Ŀ¼���ͼƬ
		long timestamp = System.currentTimeMillis() % 1000;
		String pathname = outputpath+"\\FPSCali"+timestamp;
		File dir = new File(pathname);
		while (dir.exists()) {
			timestamp= ++timestamp % 1000;
			pathname = outputpath+"\\FPSCali"+timestamp;
			dir = new File(pathname);
		}
		dir.mkdirs();
		String prefix = pathname + "\\";
		
		//���ͼƬ
		frameoutput = 0;
		int FPSArr[] = {60,30,20,15};
		for (int arrP = 0; arrP < 4; arrP++) {
			
			FPS = FPSArr[arrP];
			makePreframe = true;
			frameID = 0;
			for (int i = 1; i <= 90; i++) {
				BufferedImage image = generateImg();
				try {
					frameoutput++;
					File outfile = new File(prefix+frameoutput+".png");
					ImageIO.write(image, "png", outfile);
				} catch (Exception e) {
					System.out.println("[FPSCalibrate] Met an error when writing into PNG. Exit.");
					System.exit(0);
				}
				if (i > preframeAmount) frameID++;
				if (frameoutput % 40 == 0)
					System.out.println("[FPSCalibrate] "+frameoutput+" PNG Files Done.");
			}
			
		}
		System.out.println("[FPSCalibrate] Output "+ frameoutput +" PNG Files.");
		frameID = 0;
		
		//���MP4
		try {
			FFMPEG.generateVideo(prefix+"%d.png", ""+timestamp+".mp4", 60);
		} catch (Exception e) {
			System.out.println("[FPSCalibrate] Met an error when generate MP4 file. Exit.");
			e.printStackTrace();
			System.exit(0);
		}
		System.out.println("[FPSCalibrate] MP4 Path:\n\t\t"+Options.OUTPUTPATH+timestamp+".mp4");
	}
	
	private int preframeLeft = 0; 
	public BufferedImage generateImg() {
		BufferedImage image = new BufferedImage(1920, 1080, BufferedImage.TYPE_4BYTE_ABGR);
		Graphics g = image.getGraphics();
		if (makePreframe) {
			if (preframeLeft == 0)
				preframeLeft = preframeAmount;
			nextBase(g);
			preframeLeft--;
			if (preframeLeft != 0)
				return image;
			makePreframe = false;
			return image;
		}
		nextBase(g);
		encode(g);
		return image;
	}
	
	public void nextBase(Graphics g) {
		g.setColor(Color.white);
		g.fillRect(0, 0, 1920, 1080);
		g.setColor(Color.black);
		g.drawRect(59,89,1801,901);
		g.fillRect(60,90+frameoutput%(900/50)*50,100,50); //��60֡��ˢ��
		g.fillRect(1760,90+
				(makePreframe ? 850 : (frameID/(60/FPS)%(900/50)*50))
				,100,50); //��FPS�趨��֡��ˢ��
		
		g.setColor(new Color(0xdfdfdf));
		g.fillRect(160, 890, 1600, 100);
		g.setColor(Color.black);
		g.drawString("Suposed FPS:"+FPS, 200, 940);
	}
	
	public void encode(Graphics g) {
		g.setFont(new Font("Arial", Font.BOLD, 200));
		int x = 460+frameID/(60/FPS)/4%2*800;
		int y = 260+frameID/(60/FPS)%4*200;
		g.drawString("" + (frameID/(60/FPS)+1), x, y);
	}
}
