package settings;

import java.awt.Color;
import java.io.*;

public class Options {
	
	// ·���趨��
	// Area Of Paths
	private static String PATHPrefix = "E:\\NET-Program-Output\\v0.2.00\\";
	public static String INPUT_FILENAME = PATHPrefix + "NetIn.G24";
	public static String ENC_OUTPUTPATH = PATHPrefix + "EncResult\\";
	public static String ENC_OUTMP4NAME = "Enc00Out.mp4";
	public static String FFMPEGPATH = "D:\\Program-Folders\\ffmpeg\\bin\\ffmpeg.exe";
	// ��static {}����н�һ��������ڴ���Q�̴��ڣ����Զ��޸�Ŀ¼��Q��
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
	
	// ɫ�ʱ��뼯�趨��
	// Area Of Color Set
	public static Color[] colorset = {Color.red, Color.black, Color.green, Color.blue}; 
	
	// ��Ƶ��������������
	// Area Of Video Consts
	public static int FPS = 15;
	
	// ��֡���������趨��
	// Area Of Single Frame Consts
	/**ÿ��Cell���ӵı߳�*/
	public static int cellsize = 10;
	
	/**ÿ��Block����ı߿����*/
	public static int border = 4;
	
	/**ÿ��Block��X���������*/
	public static int xsplit  = 30;
	/**ÿ��Block��Y���������*/
	public static int ysplit  = 34;
	
	/**��һ��Block�������Ͻǵ�X����*/
	public static int xlocation = 16;
	/**��һ��Block�������Ͻǵ�Y����*/
	public static int ylocation = 10;
	
	/**����Block�������Ͻ������X��ֵ*/
	public static int xspan = 2*border + xsplit*cellsize + 8;
	/**����Block�������Ͻ������Y��ֵ*/
	public static int yspan = 2*border + ysplit*cellsize + 8;
	
	/**4�ֽ����� ��ʾ�ļ����ȣ����ֽڼƣ���ƫ����*/
	public static int offset = 16;
	/**checksum size У��ͳ���	4�ֽ����� ����16*2bit����*/
	public static int csSize = 16;
	/**frame id size ֡��� 3��һѭ�� 2�ֽ����� ����8*2bit����*/
	public static int frmIDSize = 8;
	public static int[] frmidlist = {0x0, 0xAAAA, 0xFFFF};
	//public static int[] frmidlist = {0x2CB2, 0xB2CB, 0xCB2C};
	
	/**block capacity �������� ÿ֡�ܹ����ɶ��ٸ�2bit����*/
	public static int blockCap
		= Options.xsplit * Options.ysplit;
	/**frame capacity ֡���� ÿ֡�ܹ����ɶ��ٸ�2bit����*/
	public static int frameCap
		= 18 * blockCap;
	
	//Out Of Date
	public static String OUTPUTPATH = ENC_OUTPUTPATH;
}