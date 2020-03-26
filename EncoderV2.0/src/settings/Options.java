package settings;

import java.awt.Color;
import java.io.*;

public class Options {
	
	// ·���趨��
	// Area Of Paths
	private static String PATHPrefix = "E:\\1\\";
	public static String INPUT_FILENAME = PATHPrefix + "NetIn.G24";
	public static String ENC_OUTPUTPATH = PATHPrefix + "EncResult\\";
	public static String ENC_OUTMP4NAME = "Enc00Out.mp4";
	public static String FFMPEGPATH = "E:\\ffmpeg-4.2.2-win64-static\\bin\\ffmpeg.exe";
	// ��static {}����е�һ��������ڴ���Q�̴��ڣ����Զ��޸�Ŀ¼��Q��
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
	// Area Of Color Set                               ��                                                                                ��
	public static Color[] colorset = {Color.red, new Color(70,70,70), Color.green, new Color(0,0,240)};
	
	// ��Ƶ��������������
	// Area Of Video Consts
	public static int FPS = 10;
	public static String Str_FPS = "9.87";
	public static int preframe = 12;
	public static int tailframe = 5;
	public static int randomseed = 24; // ��ʮ����ר���������������
	
	// ��֡���������趨��
	// Area Of Single Frame Consts
	/**ÿ��Cell���ӵı߳�10*10px*/
	public static int cellsize = 10;
	/**ÿ����СCell���ӵı߳�5*5px*/
	public static int smallcellsize = 5;
	/**ÿ��Block����ı߿���*/
	public static int border = 5;
	
	/**ÿ��Block��X���������*/
	public static int xsplit  = 30;
	
	/**��СBlock��X���������*/
	public static int smallxsplit=60;
	
	/**ÿ��Block��Y���������*/
	public static int ysplit  = 34;
	
	/**��СBlock��Y���������*/
	public static int smallysplit  = 68;
	
	/**��һ��Block�������Ͻǵ�X����*/
	public static int xlocation = 15;
	
	/**��һ��Block�������Ͻǵ�Y����*/
	public static int ylocation = 9;
	
	/**����Block�������Ͻ������X��ֵ*/
	public static int xspan = 2*border + xsplit*cellsize + 6;
	/**����Block�������Ͻ������Y��ֵ*/
	public static int yspan = 2*border + ysplit*cellsize + 6;
	
	/**4�ֽ����� ��ʾ�ļ����ȣ����ֽڼƣ���ƫ����*/
	public static int offset = 16;
	/**checksum size У��ͳ���	4�ֽ����� ����16*2bit����*/
	public static int csSize = 16;
	/**frame id size ֡��� 3��һѭ�� 2�ֽ����� ����8*2bit����*/
	public static int frmIDSize = 8;
	public static int[] frmidlist = {0x0, 0xAAAA, 0xFFFF};
	
	/**block capacity �������� ÿblock�ܹ����ɶ��ٸ�2bit����*/
	public static int blockCap
		= Options.xsplit * Options.ysplit;
	
	/**small block capacity �������� ÿ��Сblock�ܹ����ɶ��ٸ�2bit����*/
	public static int smallblockCap
		= Options.smallxsplit * Options.smallysplit;
	
	/**frame capacity ֡���� ÿ֡�ܹ����ɶ��ٸ�2bit����*/
	public static int frameCap
		= 10 * blockCap+8*smallblockCap;
	
	//Out Of Date
	public static String OUTPUTPATH = ENC_OUTPUTPATH;
}
