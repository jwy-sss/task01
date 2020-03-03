// https://blog.csdn.net/nick123chao/article/details/77573675
// opencv相关函数的用法可以参考上述连接
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


	typedef vector<vector<Point>> INFOCENTER_TYPE; //[行][列]
	typedef vector<Point> CONTOUR_TYPE;

	INFOCENTER_TYPE getInfoCenters(const Mat& src, CONTOUR_TYPE contour, double b, double size, int xsplit, int ysplit);
	vector<Point2d> getCorners(CONTOUR_TYPE contour);
	void getCorners_getTop3(Point t[3], CONTOUR_TYPE c, int o, int max);
	Point getCrossoverPoint(Point2d xa, Point2d xb, Point2d ya, Point2d yb);

	vector<INFOCENTER_TYPE> getAllInfoCenters(const Mat& src) {
		
		vector<INFOCENTER_TYPE> invalid = vector<INFOCENTER_TYPE>();

		// 图像二值化
		Mat src_gray = src.clone();
		Mat src_threshold;
		// 彩色图转灰度图
		cvtColor(src, src_gray, CV_BGR2GRAY);
		// 指定75阀值进行二值化
		threshold(src_gray, src_threshold, 75, 255, THRESH_BINARY);
		imwrite(string(PATH_HELLO_OUT) + string("black.png"), src_threshold);

		// 轮廓操作
		vector<CONTOUR_TYPE> contours, contoursOfBlock;
		vector<Vec4i> familyTree;
		int rootContourIndex = 0;
		findContours(src_threshold, contours, familyTree, CV_RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));
		for (int i = 0, rootContourNum = 0; i < contours.size(); i++) {
			if (familyTree[i][3] == -1 && contours[i].size() > 5000) {
				rootContourNum++;
				rootContourIndex = i;
			}
			if (rootContourNum > 1) {
				return invalid;
			}
		}
		/**/cout << "Before contoursOfBlock.push_back()" << endl;
		for (int i = 0; i < contours.size(); i++) {
			if (contours[i].size() > 600 && familyTree[i][3] == rootContourIndex)
				contoursOfBlock.push_back(contours[i]);
		}
		/**/cout << "contoursOfBlock.size() = " << contoursOfBlock.size() << endl;

		// 查找每个info cell的中心(info cell : 黑红绿蓝的小格子)
		vector<INFOCENTER_TYPE> infocenters = vector<INFOCENTER_TYPE>();
		for (int i = 0; i < contoursOfBlock.size(); i++) {
			infocenters.push_back(
				getInfoCenters(src, contoursOfBlock[i], G_BORDERWIDE, G_CELLSIZE, G_XSPLIT, G_YSPLIT)
			);
		}
		/**/cout << "infocenters.size() = " << infocenters.size() << endl;

		return infocenters;
	}


	void noteDot(Mat* src, Point p) {
		rectangle(
			*src,
			Point(p.x - 2, p.y - 2),
			Point(p.x + 2, p.y + 2),
			Scalar(0, 255, 255)
		);
		rectangle(
			*src,
			Point(p.x, p.y),
			Point(p.x, p.y),
			Scalar(0, 255, 255)
		);
	}

	// 查找info cell的中心(info cell : 黑红绿蓝的小格子)
	// 返回一个G_YSPLIT x G_XSPLIT的阵列
	INFOCENTER_TYPE getInfoCenters(const Mat& src, CONTOUR_TYPE contour, double b, double size, int xsplit, int ysplit) {
		/**/cout << "GetInfoCenters..." << endl;
		/**/Mat cornerMat = src.clone();
		double bxRatio = b / (2 * b + size * xsplit),
			byRatio = b / (2 * b + size * ysplit);
		vector<Point2d> c = getCorners(contour);
		vector<Point2d> vxa, vxb, vya, vyb;
		double ra = (c[1].y - c[0].y) / (c[1].x - c[0].x); //y=rx+b -> r of a.
		double rb = (c[2].y - c[3].y) / (c[2].x - c[3].x);
		for (int i = 0; i < xsplit; i++) {
			b = bxRatio * (c[1].x - c[0].x);
			double xax = b + (c[1].x - c[0].x - 2 * b) * (0.5 + 1.0 * i) / 30.0 + c[0].x;
			double xay = (xax - c[0].x) * ra + c[0].y;
			vxa.push_back(Point2d(xax, xay));
			b = bxRatio * (c[2].x - c[3].x);
			double xbx = b + (c[2].x - c[3].x - 2 * b) * (0.5 + 1.0 * i) / 30.0 + c[3].x;
			double xby = (xbx - c[3].x) * rb + c[3].y;
			vxb.push_back(Point2d(xbx, xby));

			noteDot(&cornerMat, Point2d(xax, xay));
			noteDot(&cornerMat, Point2d(xbx, xby));
		}
		double sa = (c[0].x - c[3].x) / (c[0].y - c[3].y); //x=sy+b -> r of a.
		double sb = (c[1].x - c[2].x) / (c[1].y - c[2].y);
		for (int i = ysplit-1; i >= 0; i--) {
			b = byRatio * (c[0].y - c[3].y);
			double yay = b + (c[0].y - c[3].y - 2 * b) * (0.5 + 1.0 * i) / 30.0 + c[3].y;
			double yax = (yay - c[3].y) * sa + c[3].x;
			vya.push_back(Point2d(yax, yay));
			b = bxRatio * (c[1].y - c[2].y);
			double yby = b + (c[1].y - c[2].y - 2 * b) * (0.5 + 1.0 * i) / 30.0 + c[2].y;
			double ybx = (yby - c[2].y) * sb + c[2].x;
			vyb.push_back(Point2d(ybx, yby));

			noteDot(&cornerMat, Point2d(yax, yay));
			noteDot(&cornerMat, Point2d(ybx, yby));
		}
		INFOCENTER_TYPE ic = INFOCENTER_TYPE(ysplit);
		for (int y = 0; y < ysplit; y++) {
			ic[y] = vector<Point>(xsplit);
			for (int x = 0; x < xsplit; x++) {
				ic[y][x] = getCrossoverPoint(vxa[x], vxb[x], vya[ysplit - 1 - y], vyb[ysplit - 1 - y]); //这边图像y轴翻来翻去弄混乱了
			}
		}
		/**/for (int i = 0; i < 4; i++) {
			noteDot(&cornerMat, c[i]);
		}
		/**/imwrite(string(PATH_OUTPUT) + string("centers_corner.png"), cornerMat);
		/**/cout << "GetInfoCenters Done." << endl;

		/*cout << ic[0][0] << endl;
		cout << ic[0][29] << endl;
		cout << ic[29][29] << endl;
		cout << ic[29][0] << endl;*/
		return ic;
	}


	//[3] xb> [2]
	//^       ^
	//ya      yb
	//[0] xa> [1]
	// 查找方形的四个顶点，顶点不明显得时候会自动补点
	// 在方块的角特别不明显的时候（比如4个以上像素的y±x值相同），精度会丢失
	vector<Point2d> getCorners(CONTOUR_TYPE contour) {
		/**/cout << "getCorners..." << endl;
		vector<Point2d> cs = vector<Point2d>();
		Point top3[3];
		for (int i = 0; i < 4; i++) {
			//o, max用法见草稿纸
			int o = (i == 0 || i == 2) ? -1 : 1;
			int max = (i < 2) ? 1 : -1;
			getCorners_getTop3(top3, contour, o, max);
			/**/cout << top3[0] << "," << top3[1] << "," << top3[2] << endl;
			//OO--正角型
			//O
			//|
			if ((top3[0].y + o * top3[0].x) * max > (top3[1].y + o * top3[1].x) * max) {
				cs.push_back(top3[0]);
				continue;
			}
			// O--平角型
			//O
			//|
			if ((top3[1].y + o * top3[1].x) * max > (top3[2].y + o * top3[2].x) * max) {
				int x, y;
				if (top3[0].x < top3[1].x) {
					x = top3[0].x + ((i == 0 || i == 3)?0:1);
					y = top3[0].y + ((i == 0 || i == 3)?o*-1:0);
				} else {
					x = top3[1].x + ((i == 0 || i == 3) ? 0 : 1);
					y = top3[1].y + ((i == 0 || i == 3) ? o * -1 : 0);
				}
				cs.push_back(Point2d(x,y));
				continue;
			}
			//  O--超级平角型，精度已经开始缺失
			// O
			//O
			//|
			for (int i = 0; i < 2; i++)
				for (int j = 2; j > i; j--) {
					if (
						(i < 2 ? 1 : -1) * top3[j].x < (i < 2 ? 1 : -1) * top3[j - 1].x
						) {
						Point temp = top3[j];
						top3[j] = top3[j - 1];
						top3[j - 1] = temp;
					}
				}
			int x, y;
			x = top3[0].x + ((i == 0 || i == 2) ? 0 : 1);
			y = top3[0].y + ((i == 0 || i == 2) ? o * -1 : 0);
			cs.push_back(Point2d(x, y));
			continue;
		}
		/**/cout << cs[0] << "," << cs[1] << "," << cs[2] << "," << cs[3] << endl;
		/**/cout << "getCorners Done." << endl;
		return cs;
	}


	// 获取最靠角落的几个点
	void getCorners_getTop3(Point t[3], CONTOUR_TYPE c, int o, int max) {
		t[0] = c[0], t[1] = c[1], t[2] = c[2];
		for (int i = 0; i < 2; i++)
			for (int j = 2; j > i; j--) {
				if ((t[j].y + o * t[j].x) * max > (t[j-1].y + o * t[j-1].x) * max) {
					Point temp = t[j];
					t[j] = t[j - 1];
					t[j - 1] = temp;
				}
			}
		for (int i = 3; i < c.size(); i++) {
			for (int j = 2; j >= 0; j--) {
				if ((c[i].y + o * c[i].x) * max > (t[j].y + o * t[j].x) * max) {
					if (j == 2) t[j] = c[i];
					else if (j == 1) {
						t[2] = t[1];
						t[1] = c[i];
					} else {
						t[1] = t[0];
						t[0] = c[i];
					}
				}
			}
		}
	}


	//[3] xb> [2]
	//^       ^
	//ya      yb
	//[0] xa> [1]
	// 获取两条直线的交点，直线由点确定
	Point getCrossoverPoint(Point2d xa, Point2d xb, Point2d ya, Point2d yb) {
		if (abs(xa.x - xb.x) < 0.1) {
			int x = round(0.5 * (xa.x + xb.x));
			double r = (yb.y - ya.y) / (yb.x - ya.x);
			int y = round((0.5 * (xa.x + xb.x) - ya.x)*r + ya.y);
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
		//x计算公式见草稿本
		int x = round(
			1.0 / (r - 1.0 / s)
			* (r * ya.x - ya.y - 1.0 / s * xb.x + xb.y)
		);
		int y = round(
			(1.0 / (r - 1.0 / s) * (r * ya.x - ya.y - 1.0 / s * xb.x + xb.y)
				- ya.x) * r + ya.y
		);
		return Point(x, y);
	}
}