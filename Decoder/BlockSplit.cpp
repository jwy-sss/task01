// https://blog.csdn.net/nick123chao/article/details/77573675
// opencv��غ������÷����Բο���������
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#include "Settings.h"


namespace BS {
	using namespace std;
	using namespace cv;

	typedef vector<vector<Point>> InfoCenterMatrix_TYPE; //�洢����Cell��������ľ��� [��][��]

	// ���ߺ������ڻ����ϱ�ǵ�
	void noteDot(Mat src, Point p) {
		rectangle(
			src,
			Point(p.x - 2, p.y - 2),
			Point(p.x + 2, p.y + 2),
			Scalar(0, 255, 255)
		);
		rectangle(
			src,
			Point(p.x, p.y),
			Point(p.x, p.y),
			Scalar(0, 255, 255)
		);
	}

	//[3] xb> [2]
	//^       ^
	//ya      yb
	//[0] xa> [1]
	//��ȡ����ֱ�ߵĽ��㣬ֱ����������ȷ��
	Point getCrossoverPoint(Point2d xa, Point2d xb, Point2d ya, Point2d yb) {
		if (abs(xa.x - xb.x) < 0.1) {
			int x = round(0.5 * (xa.x + xb.x));
			double r = (yb.y - ya.y) / (yb.x - ya.x);
			int y = round((0.5 * (xa.x + xb.x) - ya.x) * r + ya.y);
			return Point(x, y);
		}
		if (abs(ya.y - yb.y) < 0.1) {
			int y = round(0.5 * (ya.y + yb.y));
			double s = (xb.x - xa.x) / (xb.y - xa.y);
			int x = round((0.5 * (ya.y + yb.y) - xb.y) * s + xb.x);
			return Point(x, y);
		}
		double r = (yb.y - ya.y) / (yb.x - ya.x);
		double s = (xb.x - xa.x) / (xb.y - xa.y);
		//x���㹫ʽ���ݸ屾
		double X_double = 1.0 / (r - 1.0 / s) * (r * ya.x - ya.y - 1.0 / s * xb.x + xb.y);
		int x = round(X_double);
		int y = round((X_double - ya.x) * r + ya.y);
		return Point(x, y);
	}

	// ��ȡ������������
	// ԭ����� b = y + x��ȡ b ����������
	// ������ o �� max ��չ���������ĸ��ǵ�����
	// ���� MAX( (y + o*x)*max )
	void getCorners_getTop3(Point t[3], vector<Point> c, int o, int max) {
		t[0] = c[0], t[1] = c[1], t[2] = c[2];
		for (int i = 0; i < 2; i++)
			for (int j = 2; j > i; j--)
				if ((t[j].y + o * t[j].x) * max > (t[j - 1].y + o * t[j - 1].x)* max) {
					Point temp = t[j];
					t[j] = t[j - 1];
					t[j - 1] = temp;
				}
		for (int i = 3; i < c.size(); i++)
			for (int j = 2; j >= 0; j--)
				if ((c[i].y + o * c[i].x) * max > (t[j].y + o * t[j].x)* max) {
					if (j == 2) t[j] = c[i];
					else if (j == 1) {
						t[2] = t[1];
						t[1] = c[i];
					} else {
						t[1] = t[0];
						t[0] = c[i];
					}
				}
		return;
	}

	//[3] xb> [2]
	//^       ^
	//ya      yb
	//[0] xa> [1]
	// ���ҷ��ε��ĸ����㣬���㲻���Ե�ʱ����Զ�����
	// �ڷ���Ľ��ر����Ե�ʱ�򣨱���4���������ص�y��xֵ��ͬ�����ᶪʧ����
	vector<Point2d> getCorners(vector<Point> contour) {
		vector<Point2d> corners = vector<Point2d>();
		Point top3[3];

		// �����ĸ���
		for (int i = 0; i < 4; i++) {
			//o, max�÷����ݸ�ֽ
			int o = (i == 0 || i == 2) ? -1 : 1;
			int max = (i < 2) ? 1 : -1;
			getCorners_getTop3(top3, contour, o, max);

			//OO����������
			//O
			//|
			if ((top3[0].y + o * top3[0].x) * max > (top3[1].y + o * top3[1].x)* max) {
				corners.push_back(top3[0]);
				continue;
			}

			// O����ƽ����
			//O
			//|
			if ((top3[1].y + o * top3[1].x) * max > (top3[2].y + o * top3[2].x)* max) {
				int x, y;
				if (top3[0].x < top3[1].x) {
					x = top3[0].x + ((i == 0 || i == 3) ? 0 : 1);
					y = top3[0].y + ((i == 0 || i == 3) ? o * -1 : 0);
				} else {
					x = top3[1].x + ((i == 0 || i == 3) ? 0 : 1);
					y = top3[1].y + ((i == 0 || i == 3) ? o * -1 : 0);
				}
				corners.push_back(Point2d(x, y));
				continue;
			}

			//  O��������ƽ���ͣ������Ѿ���ʼȱʧ
			// O
			//O
			//|
			for (int i = 0; i < 2; i++) {
				for (int j = 2; j > i; j--) {
					if ((i < 2 ? 1 : -1) * top3[j].x < (i < 2 ? 1 : -1) * top3[j - 1].x) {
						Point temp = top3[j];
						top3[j] = top3[j - 1];
						top3[j - 1] = temp;
					}
				}
			}
			int x, y;
			x = top3[0].x + ((i == 0 || i == 2) ? 0 : 1);
			y = top3[0].y + ((i == 0 || i == 2) ? o * -1 : 0);
			corners.push_back(Point2d(x, y));
			continue;
		}
		// �����ĸ��� END.

		return corners;
	}

	//[3] xb> [2]
	//^       ^
	//ya      yb
	//[0] xa> [1]
	// ��������info cell������(info cell : ���������С����)
	// ����һ��G_ysplist * G_xsplist��Point����
	// �������ѧ���㣬�Ҳ�����дһ����
	// ��ǣ��ðɣ���������ˣ���Ҫ��ʼdebug��
	InfoCenterMatrix_TYPE getInfoCenters(const Mat& src, vector<Point> contour, double b, double size, int xsplit, int ysplit) {
		/**/Mat cornerMat = src;
		double bxRatio = b / (2 * b + size * xsplit),
			byRatio = b / (2 * b + size * ysplit);
		vector<Point2d> c = getCorners(contour);
		vector<Point2d> vxa, vxb, vya, vyb;
		double ra = (c[1].y - c[0].y) / (c[1].x - c[0].x); //y=rx+b -> r of a.
		double rb = (c[2].y - c[3].y) / (c[2].x - c[3].x);
		for (int i = 0; i < xsplit; i++) {
			b = bxRatio * (c[1].x - c[0].x);
			double xax = b + (c[1].x - c[0].x - 2 * b) * (0.5 + 1.0 * i) / xsplit + c[0].x;
			double xay = (xax - c[0].x) * ra + c[0].y;
			vxa.push_back(Point2d(xax, xay));
			b = bxRatio * (c[2].x - c[3].x);
			double xbx = b + (c[2].x - c[3].x - 2 * b) * (0.5 + 1.0 * i) / xsplit + c[3].x;
			double xby = (xbx - c[3].x) * rb + c[3].y;
			vxb.push_back(Point2d(xbx, xby));

			noteDot(cornerMat, Point2d(xax, xay));
			noteDot(cornerMat, Point2d(xbx, xby));
		}
		double sa = (c[0].x - c[3].x) / (c[0].y - c[3].y); //x=sy+b -> r of a.
		double sb = (c[1].x - c[2].x) / (c[1].y - c[2].y);
		for (int i = ysplit - 1; i >= 0; i--) {
			b = byRatio * (c[0].y - c[3].y);
			double yay = b + (c[0].y - c[3].y - 2 * b) * (0.5 + 1.0 * i) / ysplit + c[3].y;
			double yax = (yay - c[3].y) * sa + c[3].x;
			vya.push_back(Point2d(yax, yay));
			b = bxRatio * (c[1].y - c[2].y);
			double yby = b + (c[1].y - c[2].y - 2 * b) * (0.5 + 1.0 * i) / ysplit + c[2].y;
			double ybx = (yby - c[2].y) * sb + c[2].x;
			vyb.push_back(Point2d(ybx, yby));

			noteDot(cornerMat, Point2d(yax, yay));
			noteDot(cornerMat, Point2d(ybx, yby));
		}
		InfoCenterMatrix_TYPE ICMatrix = InfoCenterMatrix_TYPE(ysplit);
		for (int y = 0; y < ysplit; y++) {
			ICMatrix[y] = vector<Point>(xsplit);
			for (int x = 0; x < xsplit; x++) {
				ICMatrix[y][x] = getCrossoverPoint(vxa[x], vxb[x], vya[ysplit - 1 - y], vyb[ysplit - 1 - y]); //���ͼ��y�ᷭ����ȥŪ������
			}
		}
		for (int i = 0; i < 4; i++) {
			noteDot(cornerMat, c[i]);
		}
		/*imwrite(DEC_OUTPUTPATH + "centers_"+to_string(888)+"corner.jpg", cornerMat);*/

		//getInfoCenters ends.
		return ICMatrix;
	}


	// ���getAllInfoCenters(xxx).size() == 0��������޷���������Ч�����ݿ�
	// ��������һ�����е����飬�� 0...17 �����걸
	vector<InfoCenterMatrix_TYPE> getAllInfoCenters(const Mat& src, int frame) {
		
		// �޷���������Ч�����ݿ�ʱ�ķ���ֵ
		vector<InfoCenterMatrix_TYPE> invalid = vector<InfoCenterMatrix_TYPE>();
		// ���ڼ�¼�������ݵ�mat
		Mat mat4log = src.clone();


		// OpenCV ��ʼͼ�������������
		// ��¡��ת�Ҷ�ͼ��75��ֵ��ֵ��
		Mat src_gray = src.clone();
		Mat src_threshold;
		cvtColor(src, src_gray, CV_BGR2GRAY);
		threshold(src_gray, src_threshold, 110, 255, THRESH_BINARY);
		/*imwrite(DEC_OUTPUTPATH + "black"+to_string(frame)+".jpg", src_threshold);*/


		// ��������
		// �ҵ�������ʾ�����������������ݿ��������
		vector<vector<Point>> contours, contoursOfBlock;
		vector<Vec4i> familyTree;
		findContours(src_threshold, contours, familyTree, CV_RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));
		vector<int> childNUMBER = vector<int>(contours.size());
		int rootContourIndex = -1;
		for (int i = 0; i < contours.size(); i++) {
			childNUMBER[i] = 0;
		}
		for (int i = 0; i < contours.size(); i++) {
			if (familyTree[i][3] != -1)
				childNUMBER[familyTree[i][3]]++;
		}

		/* �޸Ķ�λԭ��ʱ�ض��޸Ĵ˴� */
		// �洢��������contours index
		int childIDUnsort[18];
		for (int i = 0; i < contours.size(); i++) {
			if (childNUMBER[i] >= 18) {
				double parentArea = contourArea(contours[i]);
				if (parentArea < src.size().width * src.size().height / 2 / 2) {
					continue;
				}
				/*cout << "ParentArea: " << parentArea << endl;*/
				int ValidChild = 0;
				for (int j = 0; j < contours.size(); j++) {
					if (familyTree[j][3] != i)
						continue;
					double childArea = contourArea(contours[j]);
					if (childArea > parentArea / 18 * 1.2 || childArea < parentArea / 18 / 1.5)
						continue;
					if (ValidChild > 18) {
						/*cout << "getAllInfoCenters: ̫����Ч��������" << endl;*/
						ValidChild = -1;
						break;
					}
					childIDUnsort[ValidChild++] = j;
					/*cout << j << "\tvalidchild("+to_string(ValidChild)+")\t"<< " Area=" << childArea << endl;*/
				}
				if (ValidChild == 18) {
					rootContourIndex = i;
					break;
				}
			}
		}
		if (rootContourIndex < 0) {
			/*cout << "getAllInfoCenters: rootContourIndex == " << rootContourIndex << endl;*/
			return invalid;
		}
		/*Ѱ�������������Ͻ�����*/
		// ���������Ͻ����꣬����childIDUnsort����
		Point childCorner[18];
		for (int c = 0; c < 18; c++) { // C++ !!
			int childCornerID = 0;
			for (int i = 1; i < contours[childIDUnsort[c]].size(); i++) {
				if (contours[childIDUnsort[c]][childCornerID].y + contours[childIDUnsort[c]][childCornerID].x
					> contours[childIDUnsort[c]][i].y + contours[childIDUnsort[c]][i].x)
					childCornerID = i;
			}
			childCorner[c] = contours[childIDUnsort[c]][childCornerID];
		}
		// parent corners ��λʾ��ͼ
		//ԭ��
		//[3]	[2]
		//[0]	[1]
		vector<Point2d> parentCorners = getCorners(contours[rootContourIndex]);
		int childIDSort[18];
		for (int c = 0; c < 18; c++) { // C++ !!
			bool islocated = false;
			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < 6; x++) {
					if (childCorner[c].x < parentCorners[3].x + (parentCorners[2].x - parentCorners[3].x) / 6.0 * (x + 0.5) &&
						childCorner[c].x > parentCorners[3].x + (parentCorners[2].x - parentCorners[3].x) / 6.0 * (x - 0.5) &&
						childCorner[c].y < parentCorners[3].y + (parentCorners[0].y - parentCorners[3].y) / 3.0 * (y + 0.5) &&
						childCorner[c].y > parentCorners[3].y + (parentCorners[0].y - parentCorners[3].y) / 3.0 * (y - 0.5)
						) {
						childIDSort[y * 6 + x] = childIDUnsort[c];
						/*cout << "child " << c << "\t" << childCorner[c] << "\t: ��"+to_string(y * 6 + x)+"\t" << "��λ���" << endl;*/
						drawContours(mat4log, contours, childIDSort[y * 6 + x], Scalar(0, 0, 255));
						islocated = true;
						break;
					}
				}
				if (islocated) break;
			}
			if (!islocated) {
				/*cout << "�������ĸ��ǣ�\n" << parentCorners[3] << parentCorners[2] << endl << parentCorners[0] << parentCorners[1] << endl;*/
				/*cout << "child " << c << ":\t" << childCorner[c] << " " << "�޷���λ" << endl;*/
				/*imwrite(DEC_OUTPUTPATH + "centers" + to_string(frame) + ".jpg", mat4log);*/
				return invalid;
			}
		}
		for (int i = 0; i < 18; i++) {
			contoursOfBlock.push_back(contours[childIDSort[i]]);
		}
		/*cout << "��λԭ���޸���ɣ�����" << endl;*/
		// �޸Ķ�λԭ��ʱ�ض��޸Ĵ˴� END
		// �������� END


		// ����ÿ��info cell������(info cell : �ں�������С����)
		// ����Ϊ����ֵ����
		vector<InfoCenterMatrix_TYPE> infocenters = vector<InfoCenterMatrix_TYPE>();
		for (int i = 0; i < contoursOfBlock.size(); i++) {
			infocenters.push_back(
				getInfoCenters(mat4log, contoursOfBlock[i], G_BORDERWIDE-1, G_CELLSIZE, G_XSPLIT, G_YSPLIT)
			);
		}
		// �����λ��
		for (int a = 0; a < infocenters.size(); a++) {
			vector<vector<Point>> matrix = infocenters[a];
			for (int j = 0; j < matrix.size(); j++)
				for (int i = 0; i < matrix[0].size(); i++) {
					rectangle(
						mat4log,
						Point(matrix[j][i].x - 2, matrix[j][i].y - 2),
						Point(matrix[j][i].x + 2, matrix[j][i].y + 2),
						Scalar(0, 255, 255)
					);
				}
		}
		/*imwrite(DEC_OUTPUTPATH + "centers"+to_string(frame)+".jpg", mat4log);*/
		/*cout << string("centers") + to_string(frame) + ".jpg out!" << endl;*/
		return infocenters;
	}	
}