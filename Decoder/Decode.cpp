#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Settings.h"
#include "BlockSplit.h"

using namespace cv;
using namespace std;
using namespace BS;

namespace DC {
	typedef vector<vector<vector<char>>> arrayOfCharMatrix;

	void reoutput(arrayOfCharMatrix cm) {
		string sout = "";
		char temp = 0;
		for (int y = 0; y < cm[0].size(); y++)
			for (int x = 0; x < cm[0][y].size(); x++) {
				temp = temp << 2;
				temp += cm[0][y][x];
				if ((y * G_XSPLIT + x) % 4 == 3) {
					sout += temp;
					temp = 0;
				}
			}
		cout << sout;
		return;
	}

	void decode(string imgname) {
		// 完成图像读取与定位
		Mat src = imread(imgname);
		Mat src_centers = src.clone();
		vector<vector<vector<Point>>> arrayOfmatrix = getAllInfoCenters(src_centers);
		for (int a = 0; a < arrayOfmatrix.size(); a++) {
			vector<vector<Point>> matrix = arrayOfmatrix[a];
			for (int j = 0; j < matrix.size(); j++)
				for (int i = 0; i < matrix[0].size(); i++) {
					rectangle(
						src_centers,
						Point(matrix[j][i].x - 2, matrix[j][i].y - 2),
						Point(matrix[j][i].x + 2, matrix[j][i].y + 2),
						Scalar(0, 255, 255)
					);
				}
		}
		imwrite(string(PATH_OUTPUT) + string("centers.png"), src_centers);
		cout << "centers.png out!" << endl;
		
		/*这部分是颜色解析，在30FPS的源下要进行改写*/
		arrayOfCharMatrix cm = arrayOfCharMatrix(arrayOfmatrix.size());
		for (int a = 0; a < arrayOfmatrix.size(); a++) {
			cm.push_back(vector<vector<char>>(arrayOfmatrix[a].size()));
			for (int j = 0; j < G_YSPLIT; j++) {
				cm[a].push_back(vector<char>(G_XSPLIT));
				for (int i = 0; i < G_XSPLIT; i++) {

					int r, g, b;
					r = g = b = 0;
					//cout << arrayOfmatrix[a][j][i].x << " " << arrayOfmatrix[a][j][i].y << " ";
					for (int loc = 0; loc < 9; loc++) {
						Vec3b s = src.at<Vec3b>(Point(arrayOfmatrix[a][j][i].x-1+loc%3, arrayOfmatrix[a][j][i].y - 1 + loc / 3));
						r += s[2];
						g += s[1];
						b += s[0];
					}
					if (r > g&& r > b&& r > 75 * 9) {
						cm[a][j][i] = 0;
					} else if (g > r&& g > b&& g > 75 * 9) {
						cm[a][j][i] = 2;
					} else if (b > g&& b > r&& b > 75 * 9) {
						cm[a][j][i] = 3;
					} else {
						cm[a][j][i] = 1;
					}
					/*cout << r << " " << g << " " << b << " ";
					cout << i % 4 << "_data=" << (int)cm[a][j][i] << "\n";*/
				}
			}
		}
		reoutput(cm);
	}
}