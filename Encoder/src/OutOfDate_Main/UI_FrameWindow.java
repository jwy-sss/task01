package OutOfDate_Main;
import javax.swing.*;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.BufferedImage;

@SuppressWarnings({ "serial", "unused" })
public class UI_FrameWindow extends JFrame {
	
	public class PixelPanel extends JPanel implements Runnable, KeyListener {
		/**
		 * ����ԭ��Ϊ(0,0) fillRect(int x , int y , int width , int height)
		 */
		BufferedImage nextImg() {
			if (spaninsert)
				return spanimage;
			BufferedImage next = new BufferedImage(1920, 1080, BufferedImage.TYPE_4BYTE_ABGR);
			Graphics g = next.getGraphics();
			g.setColor(new Color(0xffbe76));
			g.fillRect(0, 0, 1920, 1080);
			g.setColor(Color.black);
			g.fillRect(60, 90, 100, 100);
			g.fillRect(1760, 90, 100, 100);
			g.fillRect(60, 890, 100, 100);
			g.fillRect(1760, 890, 100, 100);
			g.setColor(new Color(0x334433));
			g.setFont(new Font("Arial", Font.BOLD, 200));
			g.drawString("" + serial, 860, 600);

			return next;
		}
		
		boolean spaninsert = false;
		BufferedImage spanimage;
		int serial = 0;

		PixelPanel() {
			Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize(); // ��ȡ��Ļ��С
			this.setSize(screenSize);
			
			//��ʼ��
			spanimage = new BufferedImage(1920, 1080, BufferedImage.TYPE_4BYTE_ABGR);
			Graphics g = spanimage.getGraphics();
			g.setColor(new Color(0xffbe76));
			g.fillRect(0, 0, 1920, 1080);
		}

		// ���ÿ�ݼ�
		public void keyTyped(KeyEvent e) {
		}

		public void keyReleased(KeyEvent e) {
		}

		public void keyPressed(KeyEvent e) {
			if (e.getKeyCode() == KeyEvent.VK_SPACE) {
				Thread t = new Thread(this);
				t.start();
			}
		}

		public void paint(Graphics g) {
			g.drawImage(nextImg(), 0, 0, null);
			g.drawImage(nextImg(), 0, 0, null);
		}

		// ʵ��1��60֡
		public void run() {
			for (int i = 1; i <= 3; i++) {
				long stime = System.currentTimeMillis();
				double span = 1000.0/15.0;
				long pre = stime;
				serial++;
				while (serial <= 15) {
					long now = System.currentTimeMillis();
					if (now < stime + (long)(serial * span) )
						continue;
					//spaninsert = true;
					//this.repaint();
					//spaninsert = false;
					this.repaint();
					System.out.print(serial + ":" + (now - pre) + "; ");
					pre = now;
					serial++;
				}
				System.out.println("\nTotal(" + i + "): " + (System.currentTimeMillis() - stime));
				serial = 0;
			}
			System.exit(0);
		}
	}

	
	/**������ʾ���**/
	PixelPanel panel;
	
	UI_FrameWindow() {
		//����ȫ��
		this.setTitle("�ļ�������-�����������Ŀһ");
		this.setDefaultCloseOperation(EXIT_ON_CLOSE);
		this.setUndecorated(true);  
		this.setResizable(false);
        this.getGraphicsConfiguration().getDevice().setFullScreenWindow(this);
        
        //�������
        this.setLayout(null);
        panel = new PixelPanel();
        this.add(panel);
        this.addKeyListener(panel);
		
        //��β
		this.setVisible(true);
	}
	
	public static void main(String args[]) {
		UI_FrameWindow window = new UI_FrameWindow();
		window.panel.repaint();
		System.out.println("(c)Copyright group mates and others 2020.");
	}
}
