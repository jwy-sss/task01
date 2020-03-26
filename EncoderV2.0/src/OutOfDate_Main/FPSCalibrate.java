package OutOfDate_Main;

import java.awt.*;
import java.awt.image.*;
import java.io.*;
import javax.imageio.*;

import tools.*;
import settings.*;
/**
 * FPS Calibrate类用于校正帧率<br>
 * 一般说 小米8 手机的视频录制帧率为30帧，60帧是后期合成的<br>
 * 普通显示器的刷新率为 60Hz<br>
 * <br>
 * 这个类自带main函数，能够实现生成15、30、60帧的视频<br>
 * 注：输出的视频是FPS=60的，通过2张画面重复来实现30帧的效果<br>
 * 可以通过修改常数 outputpath 来改变输出目录
 */
@SuppressWarnings("unused")
public class FPSCalibrate {
	public static String outputpath = Options.OUTPUTPATH;
	
	public static void main(String args[]) {
		
		// 为了不让 main 函数太寂寞，多留了几个空行
		
		(new FPSCalibrate()).Output();
		
		System.out.println("[FPSCalibrate] MP4 Ready!");
		
	}
	
	/**frameID代表各个不同帧率中的编码*/
	int frameID = 0;
	/**frameoutput代表已经输出的图像的总数*/
	int frameoutput = 0;
	int FPS = 30;
	int preframeAmount = 30; //相当于0.5秒
	boolean makePreframe = false;
	
	/**
	 * 输出图像并合成mp4文件
	 */
	public void Output() {
		System.out.println("[FPSCalibrate] Output Starts.");
		
		//创建一个不重名的目录存放图片
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
		
		//输出图片
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
		
		//输出MP4
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
		g.fillRect(60,90+frameoutput%(900/50)*50,100,50); //按60帧率刷新
		g.fillRect(1760,90+
				(makePreframe ? 850 : (frameID/(60/FPS)%(900/50)*50))
				,100,50); //按FPS设定的帧率刷新
		
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
