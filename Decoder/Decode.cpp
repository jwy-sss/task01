#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <io.h> //_access(...)

#include "Settings.h"
#include "BlockSplit.h"

using namespace cv;
using namespace std;
using namespace BS;

namespace DC {
	typedef vector<vector<vector<char>>> arrayOfCharMatrix;

	// @imgname 图片名
	// @imgindex 图片编号
	// @supposedFrame 对应的源文件帧编号
	// @isfinal 是否是最后一帧
	// @debug 调试开关
	// 返回值，大于等于0表示解析成功，返回值是源帧的编号；小于零代表解析不成功。
	int decodeSingleImg(string imgname, int imgindex, int supposedFrame, char outcharlist[], int filesizeByte = 204800, bool debug = false) {
		/* 全函数变量定义区 */
		int lastframe = (filesizeByte + G_offset) * 4 / (G_frameCap - G_csSize - G_frmIDSize);

		// 创建目录
		if (_access(DEC_OUTPUTPATH.c_str(), 0) != 0) {
			string command;
			command = "mkdir -p " + DEC_OUTPUTPATH;
			system(command.c_str());
		}

		/* 完成图像加载与定位 */
		Mat src = imread(imgname);
		vector<vector<vector<Point>>> arrayOfmatrix = getAllInfoCenters(src, imgindex);
		/* cout << "图像解析定位完成\n"; */

		/* 颜色解析 */
		// {R G B}
		int ColorThreshold[3] = {110, 110, 150};
		arrayOfCharMatrix cm = arrayOfCharMatrix(arrayOfmatrix.size());
		/*cout << setw(3) << arrayOfmatrix.size() << setw(3) << arrayOfmatrix[0].size() << setw(3) << arrayOfmatrix[0][0].size();*/
		for (int a = 0; a < arrayOfmatrix.size(); a++) {
			cm[a] = vector<vector<char>>(arrayOfmatrix[a].size());
			for (int j = 0; j < G_YSPLIT; j++) {
				cm[a][j] = vector<char>(arrayOfmatrix[a][j].size());
				for (int i = 0; i < G_XSPLIT; i++) {

					int r, g, b;
					r = g = b = 0;
					for (int loc = 0; loc < 9; loc++) {
						Vec3b s = src.at<Vec3b>(Point(arrayOfmatrix[a][j][i].x-1+loc%3, arrayOfmatrix[a][j][i].y - 1 + loc / 3));
						r += s[2];
						g += s[1];
						b += s[0];
					}
					if (r > g&& r > b&& r > ColorThreshold[0] * 9) {
						cm[a][j][i] = 0;
					} else if (g > r&& g > b&& g > ColorThreshold[1] * 9) {
						cm[a][j][i] = 2;
					} else if (b > g&& b > r&& b > ColorThreshold[2] * 9) {
						cm[a][j][i] = 3;
					} else {
						cm[a][j][i] = 1;
					}

				}
			}
		}

		/* 校验帧的完整性 */
		// 临时写法，后期要改
		// frame id，帧循环校验，对应G_frmIDSize
		int frameID = cm[0][0][G_csSize + 1] > 1 ? cm[0][0][G_csSize + 1] - 1 : 0;
		if (supposedFrame % 3 !=  frameID){
			cout << setw(3) << imgindex << ":supposedFrame=";
			cout << right << setw(4) << supposedFrame << " wanted sf%3=" << supposedFrame % 3 << ". Got frameID=" << frameID << endl;
			return -1;
		}
		// is valid frame 校验和检验是否通过
		bool isvalidframe = true;
		// check sum，帧的校验和
		char cs[G_csSize];
		for (int i = 0; i < G_csSize; i++) cs[i] = 0;
		int k = G_csSize;
		bool atfileend = false;
		for (int a = 0; a < cm.size(); a++) {
			int startindex = (a == 0) ? k : 0;
			for (int i = startindex; i < G_blockCap; i++) {
				cs[k % G_csSize] = (cs[k % G_csSize]+cm[a][i / G_xsplit][i % G_xsplit]) % 4;
				k++;
				if (supposedFrame == lastframe &&
					lastframe * (G_frameCap - G_csSize - G_frmIDSize) + G_blockCap * a + i - G_csSize - G_frmIDSize - G_offset == filesizeByte * 4 - 1) {
					atfileend = true;
					break;
				}
			}
			if (atfileend) break;
		}
		for (int i = 0; i < G_csSize; i++) {
			if (cs[i] != cm[0][i / G_xsplit][i % G_xsplit]) {
				isvalidframe = false;
				break;
			}
		}
		if (isvalidframe) {
			cout << setw(4) << imgindex << " valid. frameID: ";
			cout << setw(2) << frameID;
			cout << endl;
		}
		if (debug || !isvalidframe) {
			cout << setw(3) << imgindex << " ";
			cout << "OriginCS:";
			for (int i = 0; i < G_csSize; i++) {
				cout << (int)cm[0][i / G_xsplit][i % G_xsplit];
			}
			cout << " ";
			cout << "CalculCS:";
			for (int i = 0; i < G_csSize; i++) {
				cout << (int)cs[i];
			}
			cout << endl;
		}
		/* 校验帧的完整性 END */

		/* Debug模式输出不同的帧  ↓ */
		if (debug) {
			Mat out = Mat::zeros(1080, 1920, CV_8UC3);
			rectangle(
				out,
				Point(0, 0),
				Point(1920 - 1, 1080 - 1),
				Scalar(255, 255, 255),
				FILLED
			);
			Scalar colors[4] = { Scalar(0, 0, 255) , Scalar(0, 0, 0) , Scalar(0, 255, 0) , Scalar(255, 0, 0) };
			for (int a = 0; a < cm.size(); a++) {
				rectangle(
					out,
					Point(G_xlocation + G_xspan * (a % 6), G_ylocation + G_yspan * (a / 6)),
					Point(G_xlocation + G_xspan * (a % 6) + G_xsplit * G_cellsize + 2 * G_border - 1,
						G_ylocation + G_yspan * (a / 6) + G_ysplit * G_cellsize + 2 * G_border - 1),
					Scalar(0, 0, 0),
					FILLED
				);
				int x = G_xlocation + G_xspan * (a % 6) + G_border,
					y = G_ylocation + G_yspan * (a / 6) + G_border;
				for (int j = 0; j < G_ysplit; j++) {
					for (int i = 0; i < G_xsplit; i++) {
						rectangle(
							out,
							Point(x + i * G_cellsize, y + j * G_cellsize),
							Point(x + (i + 1) * G_cellsize - 1, y + (j + 1) * G_cellsize - 1),
							colors[cm[a][j][i]],
							FILLED
						);
					}
				}
			}
			imwrite(DEC_OUTPUTPATH + "out" + to_string(imgindex) + ".png", out);
			Mat originsrc = imread("E:\\NET-Program-Output\\v1.0\\EncResult\\Enc54.png");
			bitwise_xor(out, originsrc, out);
			imwrite(DEC_OUTPUTPATH + "xor" + to_string(imgindex) + ".png", out);
		}

		// 写入文件，并返回消息
		if (isvalidframe) {
			int startindex = G_csSize + G_frmIDSize;
			if (supposedFrame == 0) {
				startindex += G_offset;
			}
			for (int i = startindex; i < G_frameCap; i++) {
				int p2bit = supposedFrame * (G_frameCap - G_csSize - G_frmIDSize) + i - G_csSize - G_frmIDSize - G_offset;
				outcharlist[p2bit / 4] = (outcharlist[p2bit / 4] << 2)
					+ cm[i / G_blockCap]
					[(i - i / G_blockCap * G_blockCap) / G_xsplit]
					[(i - i / G_blockCap * G_blockCap) % G_xsplit];
				if (supposedFrame == lastframe &&
					p2bit == filesizeByte * 4 - 1) {
					break;
				}
			}
			return supposedFrame;
		} else {
			return -1;
		}
		// 完结撒花
	}

	void decode() {
		char* outcharlist = (char *)malloc(204800 * sizeof(char));
		if (!outcharlist) {
			cout << "连个内存空间都分配不成功？！已退出。" << endl;
			return;
		}
		for (int i = 0; i < 204800; i++) {
			outcharlist[i] = 0;
		}
		//int debuglist[] = {58,59,82,83,134,135,149,150};
		int debuglist[] = { -1 };
		int supposedFrame = 0;
		for (int imgindex = 40; imgindex < 184; imgindex++) {
			bool debug = false;
			for (int i = 0; i < sizeof(debuglist) / sizeof(debuglist[0]); i++)
				if (imgindex == debuglist[i]) {
					debug = true;
					break;
				}
			//if (debug)
			int ret = decodeSingleImg(DEC_INPUTPATH + "Dec" + to_string(imgindex) + ".jpg", imgindex, supposedFrame, outcharlist, 204800, debug);
			if (ret >= 0) supposedFrame++;
		}
		ofstream outFile(DEC_OUTFILENAME, ios::out | ios::binary);
		outFile.write(outcharlist, 204800);
		free(outcharlist);
	}
}