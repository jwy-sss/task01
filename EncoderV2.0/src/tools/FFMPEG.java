package tools;

import java.util.*;
import settings.*;

public class FFMPEG {
	/**
	 * 参考了：https://github.com/SuperAbee/Mailman 中的ffmpeg.java文件<br>
	 * 参考者注：Mailman的generateVideo函数参数不足<br>
	 * 后期添加了许多视频质量控制参数
	 * @param inPattern 图片名称模式
	 * @param MP4name 输出的文件名
	 * @param fps 帧率（一般为60）
	 * @throws Exception 调用 FFMPEG.exe 可能抛出异常
	 */
	public static void generateVideo(String inPattern, String MP4name, int fps) throws Exception {
		List<String> command = new ArrayList<String>();
		//-f image2 -r 5 -i %d.png -vcodec mpeg4 t.mp4
		command.add(Options.FFMPEGPATH);
		command.add("-r"); command.add(""+fps);
		command.add("-i"); command.add(inPattern);
		command.add("-vcodec"); command.add("mpeg4");
		command.add("-b"); command.add(""+20000); //比特率，好像没什么用
		command.add("-qscale"); command.add(""+0.01); //指定视频质量，数值越小越好（最小0.01）
		command.add("-y"); //直接覆盖已有文件
		command.add(Options.OUTPUTPATH+MP4name);
		
		ProcessBuilder builder = new ProcessBuilder();
		builder.command(command);
		builder.start(); //此处可抛出异常
	}
	public static void generateVideoV2(String inPattern, String MP4name, String fps) throws Exception {
		List<String> command = new ArrayList<String>();
		//-f image2 -r 5 -i %d.png -vcodec mpeg4 t.mp4
		command.add(Options.FFMPEGPATH);
		command.add("-r"); command.add(fps);
		command.add("-i"); command.add(inPattern);
		command.add("-vcodec"); command.add("mpeg4");
		command.add("-b"); command.add(""+20000); //比特率，好像没什么用
		command.add("-qscale"); command.add(""+0.01); //指定视频质量，数值越小越好（最小0.01）
		command.add("-y"); //直接覆盖已有文件
		command.add(Options.OUTPUTPATH+MP4name);
		
		ProcessBuilder builder = new ProcessBuilder();
		builder.command(command);
		builder.start(); //此处可抛出异常
	}
}
