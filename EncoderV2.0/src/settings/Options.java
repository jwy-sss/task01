package settings;

import java.awt.Color;
import java.io.*;

public class Options {
	
	// 路径设定区
	// Area Of Paths
	private static String PATHPrefix = "E:\\1\\";
	public static String INPUT_FILENAME = PATHPrefix + "NetIn.G24";
	public static String ENC_OUTPUTPATH = PATHPrefix + "EncResult\\";
	public static String ENC_OUTMP4NAME = "Enc00Out.mp4";
	public static String FFMPEGPATH = "E:\\ffmpeg-4.2.2-win64-static\\bin\\ffmpeg.exe";
	// 由static {}块进行的一步，如果内存盘Q盘存在，会自动修改目录至Q盘
	static {
		File Q = new File("Q:/");
		if (Q.exists()) {
			PATHPrefix = "Q" + PATHPrefix.substring(1);
			ENC_OUTPUTPATH = "Q" + ENC_OUTPUTPATH.substring(1);
			File Q_OUT = new File(ENC_OUTPUTPATH);
			if (!Q_OUT.exists())
				Q_OUT.mkdirs();
			System.out.println("RAMDisk directory is created: "+PATHPrefix);
		}
	}
	
	// 色彩编码集设定区
	// Area Of Color Set                               灰                                                                                蓝
	public static Color[] colorset = {Color.red, new Color(70,70,70), Color.green, new Color(0,0,240)};
	
	// 视频参数常量设置区
	// Area Of Video Consts
	public static int FPS = 10;
	public static String Str_FPS = "9.87";
	public static int preframe = 12;
	public static int tailframe = 5;
	public static int randomseed = 24; // 二十四组专用随机数！乌拉！
	
	// 单帧参数常量设定区
	// Area Of Single Frame Consts
	/**每个Cell格子的边长10*10px*/
	public static int cellsize = 10;
	/**每个超小Cell格子的边长5*5px*/
	public static int smallcellsize = 5;
	/**每个Block方块的边框宽度*/
	public static int border = 5;
	
	/**每个Block在X方向的列数*/
	public static int xsplit  = 30;
	
	/**超小Block在X方向的列数*/
	public static int smallxsplit=60;
	
	/**每个Block在Y方向的行数*/
	public static int ysplit  = 34;
	
	/**超小Block在Y方向的行数*/
	public static int smallysplit  = 68;
	
	/**第一个Block方块左上角的X坐标*/
	public static int xlocation = 15;
	
	/**第一个Block方块左上角的Y坐标*/
	public static int ylocation = 9;
	
	/**各个Block方块左上角坐标的X差值*/
	public static int xspan = 2*border + xsplit*cellsize + 6;
	/**各个Block方块左上角坐标的Y差值*/
	public static int yspan = 2*border + ysplit*cellsize + 6;
	
	/**4字节容量 表示文件长度（按字节计）的偏移量*/
	public static int offset = 16;
	/**checksum size 校验和长度	4字节容量 共计16*2bit格子*/
	public static int csSize = 16;
	/**frame id size 帧编号 3个一循环 2字节容量 共计8*2bit格子*/
	public static int frmIDSize = 8;
	public static int[] frmidlist = {0x0, 0xAAAA, 0xFFFF};
	
	/**block capacity 方块容量 每block能够容纳多少个2bit格子*/
	public static int blockCap
		= Options.xsplit * Options.ysplit;
	
	/**small block capacity 方块容量 每超小block能够容纳多少个2bit格子*/
	public static int smallblockCap
		= Options.smallxsplit * Options.smallysplit;
	
	/**frame capacity 帧容量 每帧能够容纳多少个2bit格子*/
	public static int frameCap
		= 10 * blockCap+8*smallblockCap;
	
	//Out Of Date
	public static String OUTPUTPATH = ENC_OUTPUTPATH;
}
