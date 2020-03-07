package encoder;

import java.awt.*;
import java.awt.image.*;
import java.io.*;
import java.nio.channels.FileChannel;
import java.util.Random;

import javax.imageio.*;

import settings.*;
import tools.*;

@SuppressWarnings("unused")
public class Enc {
	public static void main(String args[]) {
		new Enc();
	}
	public Enc() {
		this(Options.INPUT_FILENAME);
	}
	public Enc(String FilePath) {
		
		// �ļ����
		File file = new File(FilePath);
		if (!file.isFile() || !file.exists()) {
			System.out.println("File not available: " + FilePath);
			System.exit(0);
		}
		if (file.length() > 10*(int)Math.pow(2,20)) {
			System.out.println("Filesize is over 10MB: " + FilePath);
			System.exit(0);
		}
		File dir = new File(Options.ENC_OUTPUTPATH);
		if (!dir.exists()) {
			dir.mkdirs();
		}
		for(File f:dir.listFiles()){
			if(!f.isDirectory())
				f.delete();
		}
		System.out.println("Output directory prepared: " + dir.getPath());
		
		
		// �ļ���ȡ
		byte[] content = null; // һ���Զ�ȡ10M����
		try {
			DataInputStream datais = new DataInputStream(new FileInputStream(file));
			content = datais.readAllBytes();
			datais.close();
		} catch (Exception e) {
			System.out.println("Reading Error: " + FilePath);
			System.exit(0);
		}
		
		// �ļ�����
		// �����ȡԽ�磬����ȡһ����ɫ����010101...��ASCII : U
		int filesizeByte = content.length; 	// �ļ��������Byte����
		int offset = Options.offset;		// 4�ֽ����� ��ʾ�ļ����ȣ����ֽڼƣ���ƫ����
		int csSize = Options.csSize;		// checksum size У��ͳ���	4�ֽ����� ����16*2bit����
		int frameCap = Options.frameCap;	// frame capacity ֡���� ÿ֡�ܹ����ɶ��ٸ�2bit����
		int frmIDSize = Options.frmIDSize;	// frame id size ֡��� 3��һѭ�� 2�ֽ����� ����8*2bit����
		int framecount = (int)Math.ceil( 	// �ܵ�֡��
			(double)(offset + filesizeByte * 4) / (frameCap - csSize - frmIDSize)
		);
		// ����ǰ��֡
		for (int frame = 0; frame < Options.preframe; frame++) {
			encodeImgWhite(frame);
		}
		// ������Ч�ؼ�֡
		for (int frame = 0; frame < framecount; frame++) {
			if (frame % 50 == 0 && frame > 0)
				System.out.println(""+frame+" frame have been generated.");
			byte[] cells = new byte[frameCap];	// java��byte��ʼֵΪ0
			for (int i = csSize; i < frameCap; i++) {
				int p2bit = 0;
				if (frame == 0 && i >= csSize+frmIDSize  && i < offset + csSize+frmIDSize) {
					// ��filesize��Ϣд����
					cells[i] = (byte)((filesizeByte >> (offset+csSize+frmIDSize-1-i)*2) & 0x0003);
				} else if (i >= csSize && i < csSize + frmIDSize) {
					// д��frame ID
					cells[i] = (byte)((Options.frmidlist[frame%3] >> (frmIDSize+csSize-1-i)*2) & 0x0003);
				} else {
					// ���ļ���Ϣд����
					p2bit = (frameCap - csSize - frmIDSize)*frame + i - csSize - frmIDSize - offset; // ��ȡ�ļ���λ��ָ��(��2bit��)
					cells[i] = (byte)((content[p2bit/4] >> (3 - p2bit % 4)*2) & 0x0003);
				}
				cells[i%csSize] = (byte)((cells[i%csSize] + cells[i]) % 4);
				if (p2bit == filesizeByte * 4 - 1) { // ʹ��-1��ֹ��������ռ������ҳʱ����Խ������
					break;
				}
			}
			this.encodeImg(frame, cells, filesizeByte);
		}
		// ������β֡��
		for (int frame = framecount+Options.preframe; frame < framecount+Options.preframe+Options.tailframe; frame++) {
			encodeImgWhite(frame);
		}
		System.out.println("Encoder: "+(framecount+Options.preframe+Options.tailframe)*2+" frames output.");
		
		
		// �ϳ���Ƶ 
		try {
			FFMPEG.generateVideoV2(Options.ENC_OUTPUTPATH+"Enc%d.png", Options.ENC_OUTMP4NAME, Options.Str_FPS);
		} catch (Exception e) {
			System.out.println("Met Error when Generated Video: "+Options.ENC_OUTMP4NAME);
			System.exit(0);
		}
		System.out.println("Video Generated: "+Options.ENC_OUTMP4NAME);
		return;
	}
	private void encodeImg(int frame, byte[] cells, int filesizeByte) {
		int offset = Options.offset;		// 4�ֽ����� ��ʾ�ļ����ȣ����ֽڼƣ���ƫ����
		int csSize = Options.csSize;		// checksum size У��ͳ���	4�ֽ����� ����16*2bit����
		int blockCap = Options.blockCap;	// block capacity �������� ÿ֡�ܹ����ɶ��ٸ�2bit����
		int frameCap = Options.frameCap;	// frame capacity ֡���� ÿ֡�ܹ����ɶ��ٸ�2bit����
		int frmIDSize = Options.frmIDSize;	// frame id size ֡��� 3��һѭ�� 2�ֽ����� ����8*2bit����
		
		BufferedImage image = new BufferedImage(1920, 1080, BufferedImage.TYPE_3BYTE_BGR);
		Graphics g = image.getGraphics();
		g.setColor(Color.white);
		g.fillRect(0, 0, 1920, 1080);
		for (int block = 0; block < 18; block++) {
			int x = Options.xlocation + (block) % 6 * Options.xspan;
			int y = Options.ylocation + (block) / 6 * Options.yspan;
			g.setColor(Color.black);
			g.fillRect(x, y,
					2*Options.border+Options.xsplit*Options.cellsize,
					2*Options.border+Options.ysplit*Options.cellsize);
			for (int i = 0; i < blockCap; i++) {
				int p2bit = (frameCap - csSize - frmIDSize)*frame + block*blockCap + i - csSize - frmIDSize - offset; // ��ȡ�ļ���λ��ָ��(��2bit��)
				if (p2bit/4 >= filesizeByte) {
					break;
				}
				g.setColor(Options.colorset[cells[block*(Options.xsplit*Options.ysplit)+i]]);
				g.fillRect(
						x + Options.border + Options.cellsize*(i%Options.xsplit),
						y + Options.border + Options.cellsize*(i/Options.xsplit),
						Options.cellsize, Options.cellsize);
			}
		}
		try {
			ImageIO.write(image, "PNG", new File(Options.ENC_OUTPUTPATH+"Enc"+(frame+Options.preframe)+".png"));
//			copyFile(
//					new File(Options.ENC_OUTPUTPATH+"Enc"+((frame+Options.preframe)*2)+".png"),
//					new File(Options.ENC_OUTPUTPATH+"Enc"+((frame+Options.preframe)*2+1)+".png")
//					);
		} catch (IOException e) {
			System.out.println("Function encodeImg error: writing into "+"Enc"+(frame+Options.preframe)+".png");
			System.exit(0);
		}
		return;
	}
	/**������������ڷ�֮һ�ĸ��ʱ�ʶ��Ϊ�ؼ�֡*/
	private void encodeImgWhite(int frame) {
		int blockCap = Options.blockCap;	// block capacity �������� ÿ֡�ܹ����ɶ��ٸ�2bit����
		Color[] clist = {Color.red, Color.green, Color.blue, Color.black};
		Random r = new Random(Options.randomseed+(new Random()).nextInt(1000));
		
		BufferedImage image = new BufferedImage(1920, 1080, BufferedImage.TYPE_3BYTE_BGR);
		Graphics g = image.getGraphics();
		g.setColor(Color.white);
		g.fillRect(0, 0, 1920, 1080);
		for (int block = 0; block < 18; block++) {
			int x = Options.xlocation + (block) % 6 * Options.xspan;
			int y = Options.ylocation + (block) / 6 * Options.yspan;
			g.setColor(Color.black);
			g.fillRect(x, y,
					2*Options.border+Options.xsplit*Options.cellsize,
					2*Options.border+Options.ysplit*Options.cellsize);
			for (int i = 0; i < blockCap; i++) {
				g.setColor(clist[(r.nextInt(clist.length))%clist.length]);
				g.fillRect(
						x + Options.border + Options.cellsize*(i%Options.xsplit),
						y + Options.border + Options.cellsize*(i/Options.xsplit),
						Options.cellsize, Options.cellsize);
			}
		}
		try {
			ImageIO.write(image, "PNG", new File(Options.ENC_OUTPUTPATH+"Enc"+frame+".png"));
//			copyFile(
//					new File(Options.ENC_OUTPUTPATH+"Enc"+((frame)*2)+".png"),
//					new File(Options.ENC_OUTPUTPATH+"Enc"+((frame)*2+1)+".png")
//					);
		} catch (IOException e) {
			e.printStackTrace();
			System.out.println("Function encodeImg error: writing into "+"Enc"+frame+".png");
			System.exit(0);
		}
		return;
	}
	/**
	 * Copy from
	 * https://www.cnblogs.com/zq-boke/p/8523710.html
	 */
	private static void copyFile(File source, File dest) throws IOException {    
		FileInputStream fin = new FileInputStream(source);
		FileOutputStream fout = new FileOutputStream(dest);
        FileChannel inputChannel = null;    
        FileChannel outputChannel = null;    
        try {
        	inputChannel = fin.getChannel();
        	outputChannel = fout.getChannel();
        	outputChannel.transferFrom(inputChannel, 0, inputChannel.size());
        } finally {
        	inputChannel.close();
        	outputChannel.close();
        	fin.close();
        	fout.close();
        }
        return;
	}
}
