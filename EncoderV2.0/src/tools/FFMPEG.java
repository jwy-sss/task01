package tools;

import java.util.*;
import settings.*;

public class FFMPEG {
	/**
	 * �ο��ˣ�https://github.com/SuperAbee/Mailman �е�ffmpeg.java�ļ�<br>
	 * �ο���ע��Mailman��generateVideo������������<br>
	 * ��������������Ƶ�������Ʋ���
	 * @param inPattern ͼƬ����ģʽ
	 * @param MP4name ������ļ���
	 * @param fps ֡�ʣ�һ��Ϊ60��
	 * @throws Exception ���� FFMPEG.exe �����׳��쳣
	 */
	public static void generateVideo(String inPattern, String MP4name, int fps) throws Exception {
		List<String> command = new ArrayList<String>();
		//-f image2 -r 5 -i %d.png -vcodec mpeg4 t.mp4
		command.add(Options.FFMPEGPATH);
		command.add("-r"); command.add(""+fps);
		command.add("-i"); command.add(inPattern);
		command.add("-vcodec"); command.add("mpeg4");
		command.add("-b"); command.add(""+20000); //�����ʣ�����ûʲô��
		command.add("-qscale"); command.add(""+0.01); //ָ����Ƶ��������ֵԽСԽ�ã���С0.01��
		command.add("-y"); //ֱ�Ӹ��������ļ�
		command.add(Options.OUTPUTPATH+MP4name);
		
		ProcessBuilder builder = new ProcessBuilder();
		builder.command(command);
		builder.start(); //�˴����׳��쳣
	}
	public static void generateVideoV2(String inPattern, String MP4name, String fps) throws Exception {
		List<String> command = new ArrayList<String>();
		//-f image2 -r 5 -i %d.png -vcodec mpeg4 t.mp4
		command.add(Options.FFMPEGPATH);
		command.add("-r"); command.add(fps);
		command.add("-i"); command.add(inPattern);
		command.add("-vcodec"); command.add("mpeg4");
		command.add("-b"); command.add(""+20000); //�����ʣ�����ûʲô��
		command.add("-qscale"); command.add(""+0.01); //ָ����Ƶ��������ֵԽСԽ�ã���С0.01��
		command.add("-y"); //ֱ�Ӹ��������ļ�
		command.add(Options.OUTPUTPATH+MP4name);
		
		ProcessBuilder builder = new ProcessBuilder();
		builder.command(command);
		builder.start(); //�˴����׳��쳣
	}
}
