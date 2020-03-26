package OutOfDate_Main;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.image.*;

/**
 * �ô�����ʵ���˶�λ��Ļ���<br>
 * ����ʵ�ֿ��Ը���LocatorPanel.generateImg()
 */
@SuppressWarnings({ "serial", "unused"})
public class UI_LocatorDemo extends JFrame {
	
	/**��λ����ʾ���**/
	LocatorPanel panel;
	
	UI_LocatorDemo() {
		//����ȫ��
		this.setTitle("��λ��DEMO-�����������Ŀһ");
		this.setDefaultCloseOperation(EXIT_ON_CLOSE);
		this.setUndecorated(true);
		this.setResizable(false);
        this.getGraphicsConfiguration().getDevice().setFullScreenWindow(this);

        //�������
        this.setLayout(null);
        panel = new LocatorPanel();
        this.add(panel);
        this.addKeyListener(panel);
		
        //��β
		this.setVisible(true);
	}
	
	public static void main(String args[]) {
		UI_LocatorDemo window = new UI_LocatorDemo();
		window.panel.repaint();
		System.out.println("(c)Copyright group mates and others 2020.");
	}
	
	public class LocatorPanel extends JPanel implements KeyListener {

		BufferedImage image;
		
		LocatorPanel() {
			Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize(); // ��ȡ��Ļ��С
			this.setSize(screenSize);
			
			image = generateImg();
		}
		
		public BufferedImage generateImg() {
			BufferedImage image = new BufferedImage(1920, 1080, BufferedImage.TYPE_4BYTE_ABGR);
			Graphics g = image.getGraphics();
			nextBase(g);
			return image;
		}
		
		/**
		 * ������1920*1080��Ļ�Ķ�λ��
		 */
		public void nextBase(Graphics g) {
			g.setColor(Color.white);
			g.fillRect(0, 0, 1920, 1080);

			// ����10x10����ʹ��Ǳ��θ�������
			if (false) {
				g.setColor(Color.black);
				for (int i = 20; i <= 1060; i+=10) {
					g.drawLine(20, i, 1900, i);
				}
				for (int i = 20; i <= 1900; i+=10) {
					g.drawLine(i, 20, i, 1060);
				}
				return;
			}
			
			// ���ƶ�λ��
			String data = "JBufferedInputStream(InputStream in)Creates a BufferedInputStream and saves its argument, the input stream inENDJBufferedInputStream(InputStream in)Creates a BufferedInputStream and saves its argument, the input stream intEND";
			Color clist[] = {new Color(0xff3333), Color.black, new Color(0x33ff33), new Color(0x3333ff)};
			g.setColor(Color.black);
			int x = 800, y = 350;
			g.fillRect(x, y, 310, 310);
			x += 5;
			y += 5;
//			System.out.println(data.length());
			for (int j = 0; j < 30; j++)
				for (int i = 0; i < 30; i++) {
//					System.out.println(""+j+" "+i+" "+(j*30+i)*2+" "+(j*30+i)*2/8);
					int c = data.charAt((j*30+i)*2/8);
					c = c >> 6-(j*30+i)*2%8;
					c = c & 3; //ֻ�������2bit��Ϣ
					System.out.printf(data.charAt((j*30+i)*2/8) + " " + c + " ");
					g.setColor(clist[c]);
					g.fillRect(x+i*10, y+j*10, 10, 10);
				}
		}

		// ���ÿ�ݼ�
		public void keyTyped(KeyEvent e) {
		}

		public void keyReleased(KeyEvent e) {
		}

		public void keyPressed(KeyEvent e) {
			if (e.getKeyCode() == KeyEvent.VK_ESCAPE) {
				System.exit(0);
			}
		}

		public void paint(Graphics g) {
			g.drawImage(image, 0, 0, null);
		}
	}

}
