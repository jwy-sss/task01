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
#include <cstdio>
#include <io.h> //_access(...)

#include "Settings.h"
#include "BlockSplit.h"

using namespace cv;
using namespace std;
using namespace BS;

namespace DC {
	typedef vector<vector<vector<char>>> arrayOfCharMatrix;

	// Debug用的图片异或函数
	void debugLogImg(bool debug, arrayOfCharMatrix cm, int imgindex, int supposedFrame) {
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
			Scalar colors[4] = { Scalar(0, 0, 255) , Scalar(70, 70, 70) , Scalar(0, 255, 0) , Scalar(240, 0, 0) };
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
			imwrite(DEC_DEBUGPATH + "zout" + to_string(imgindex) + ".png", out);
			Mat originsrc = imread(PATHPrefix+"EncResult\\Enc"+to_string(supposedFrame+G_preframe)+".png");
			bitwise_xor(out, originsrc, out);
			imwrite(DEC_DEBUGPATH + "zxor" + to_string(imgindex) + ".png", out);
		}
	}

	char* mallocOutCharList(int size) {
		char* outcharlist;
		outcharlist = (char*)malloc(size * sizeof(char));
		if (!outcharlist) {
			cout << "内存空间未分配成功，已退出。" << endl;
			return 0;
		}
		for (int i = 0; i < size; i++) {
			outcharlist[i] = 0;
		}
		return outcharlist;
	}

	// decodeSingleImg函数，校验完整性、引用参数返回文件大小、写入输出流
	// @imgname 图片名
	// @imgindex 图片编号
	// @supposedFrame 对应的源文件帧编号
	// @isfinal 是否是最后一帧
	// @debug 调试开关
	// 返回值：≥0表示解析成功，返回值是源帧的编号；-1代表校验和不正确，-2代表帧序号不对，-3代表无法读取文件。
	int decodeSingleImg(string imgname, int imgindex, int supposedFrame, char* &outcharlist, int &filesizeByte, bool debug = false) {
		/* 全函数变量定义区 */
		int Illegal_CheckSum = -1;
		int Illegal_FrameID = -2;
		int Illegal_FileName = -3;
		/* 全函数变量定义区 END */

		/* 完成图像加载与定位 */
		Mat src = imread(imgname);
		if (src.empty()) {
			cout << right << setw(3) << imgindex << "::";
			cout << "无法读取文件\n";
			return Illegal_FileName;
		}
		vector<vector<vector<Point>>> arrayOfmatrix = getAllInfoCenters(src, imgindex);
		if (arrayOfmatrix.size() == 0) {
			cout << right << setw(3) << imgindex << "::";
			cout << "没找到完整的外轮廓（或有效内轮廓不足18个）\n";
			return Illegal_CheckSum;
		}
		/* 完成图像加载与定位 END */


		/* 颜色解析并匹配帧的校验和 */
		arrayOfCharMatrix cm = arrayOfCharMatrix(arrayOfmatrix.size());
		for (int a = 0; a < arrayOfmatrix.size(); a++) {
			cm[a] = vector<vector<char>>(arrayOfmatrix[a].size());
			for (int j = 0; j < G_YSPLIT; j++) {
				cm[a][j] = vector<char>(arrayOfmatrix[a][j].size());
			}
		}
		// is valid frame 校验和检验是否通过
		bool isvalidframe = false;
		// {R G B}
		int ColorThreshold[3] = {150, 130, 158};
		// check sum，帧的校验和
		char cs[G_csSize];
		
		int trycount = 1;
		while (!isvalidframe) {
			// 颜色解析
			for (int a = 0; a < arrayOfmatrix.size(); a++) {
				for (int j = 0; j < G_YSPLIT; j++) {
					for (int i = 0; i < G_XSPLIT; i++) {
						int r, g, b;
						r = g = b = 0;
						for (int loc = 0; loc < 9; loc++) {
							Vec3b s = src.at<Vec3b>(Point(
								arrayOfmatrix[a][j][i].x - 1 + loc % 3,
								arrayOfmatrix[a][j][i].y - 1 + loc / 3)
								);
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
			// 校验和计算
			for (int i = 0; i < G_csSize; i++) cs[i] = 0;
			for (int a = 0; a < cm.size(); a++) {
				int startindex = (a == 0) ? G_csSize : 0;
				for (int i = startindex; i < G_blockCap; i++) {
					int k = a * G_blockCap + i - G_csSize;
					cs[k % G_csSize] = (cs[k % G_csSize] + cm[a][i / G_xsplit][i % G_xsplit]) % 4;
					k++;
				}
			}
			isvalidframe = true;
			for (int i = 0; i < G_csSize; i++) {
				if (cs[i] != cm[0][i / G_xsplit][i % G_xsplit]) {
					isvalidframe = false;
					break;
				}
			}
			// Debug模式下可以针对特定的帧进行异或
			debugLogImg(debug, cm, imgindex, debug ? 44 - G_preframe : supposedFrame);
			if (!isvalidframe && trycount > 0) {
				ColorThreshold[2] -= 8;
				trycount--;
			} else {
				break;
			}
		}
		/* 颜色解析并匹配帧的校验和 END */


		/* 校验帧的有效性 */
		if (!isvalidframe) {
			cout << right << setw(3) << imgindex << "::";
			cout << "校验和不正确";
			if (debug) {
				cout << "帧头部CheckSum: ";
				for (int i = 0; i < G_csSize; i++) {
					cout << (int)cm[0][i / G_xsplit][i % G_xsplit];
				}
				cout << " ";
				cout << "实际CheckSum: ";
				for (int i = 0; i < G_csSize; i++) {
					cout << (int)cs[i];
				}
			}
			cout << endl;
			return Illegal_CheckSum;
		}
		// frame id，帧循环校验，对应G_frmIDSize
		int frameID = cm[0][G_csSize / G_xsplit][G_csSize % G_xsplit] > 1
			? cm[0][G_csSize / G_xsplit][G_csSize % G_xsplit] - 1 : 0;
		if (supposedFrame % 3 != frameID) {
			cout << right << setw(3) << imgindex << "::";
			cout << "等待第" << supposedFrame << "源帧。";
			cout << "目标：" << supposedFrame % 3 << " 当前：" << frameID << endl;
			return Illegal_FrameID;
		}
		// 最终校验正确
		cout << setw(3) << imgindex << "::valid. frameID: ";
		cout << setw(2) << frameID;
		cout << endl;
		/* 校验帧的有效性 END */

		/* 如果是第一帧，获取文件大小 */
		if (supposedFrame == 0) {
			filesizeByte = 0;
			for (int i = G_csSize + G_frmIDSize; i < G_csSize + G_frmIDSize + G_offset; i++) {
				filesizeByte = (filesizeByte << 2) + (int)cm[0][i / G_xsplit][i % G_xsplit];
			}
			cout << "Get File Size: " << filesizeByte << endl;
		}
		free(outcharlist);
		outcharlist = mallocOutCharList(filesizeByte);
		// -1 防止正好铺满所有帧
		int lastframe = (G_offset + filesizeByte * 4 - 1) / (G_frameCap - G_csSize - G_frmIDSize);
		/* 如果是第一帧，获取文件大小 END */

		// 写入文件，并返回消息
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
		// 完结撒花
	}

	void cleanDebugPATH() {
		for (int i = 1; i < 9999999; i++) {
			string path = DEC_PROC_PNG;
			path.replace(path.find("%d"), to_string(i).length(), to_string(i));
			if (_access(path.c_str(), 0) != 0) {
				remove(path.c_str());
			} else {
				return;
			}
		}
	}

	void decode() {
		/* Debug Area */
		bool debugswitch = false;
		int debugstartindex = 167;
		int debugendindex = 170;
		int debuglist[] = { 167,168,169 };

		// 调用FFMPEG
		bool useFFMPEG = USE_FFMPEG;
		if (debugswitch) useFFMPEG = false;
		if (useFFMPEG) {
			string command;
			command = FFMPEG_PATH + " " + DEC_PROC_PNG + " -i " + DEC_VIDEOPATH;
			system(command.c_str());
			cout << "FFMPEG 解析完成" << endl;
		}

		// 创建Debug目录
		if (_access(DEC_DEBUGPATH.c_str(), 0) != 0) {
			string command;
			command = "mkdir -p " + DEC_DEBUGPATH;
			system(command.c_str());
		}

		int startindex = 30 / 10 * G_preframe;
		if (debugswitch) startindex = debugstartindex;
		
		// 文件输出流
		char* outcharlist = (char*)malloc(1 * sizeof(char));
		// 当前的源帧编号
		int supposedFrame = 0;
		// 文件大小
		int filesizeByte = 0;

		// 正式解码
		for (int imgindex = startindex; imgindex < (debugswitch ? debugendindex : 999999999); imgindex++) {
			bool debug = false;
			for (int i = 0; i < sizeof(debuglist) / sizeof(debuglist[0]); i++)
				if (imgindex == debuglist[i] && debugswitch) {
					debug = true;
					break;
				}
			int ret = decodeSingleImg(DEC_INPUTPATH + "Dec" + to_string(imgindex) + ".jpg", imgindex, supposedFrame, outcharlist, filesizeByte, debug);
			
			// 正常返回
			if (ret >= 0) {
				// 检查是否最后一帧
				int lastframe = (G_offset + filesizeByte * 4 - 1) / (G_frameCap - G_csSize - G_frmIDSize);
				if (supposedFrame == lastframe) {
					cout << "Decoded All Images." << endl;
					break;
				}
				supposedFrame++;
			}
			// 读取错误
			if (ret == -3) {
				cout << "读取错误 或 已读完最后一张图片却没获得完整文件.\n";
				return;
			}
		}
		if (debugswitch) {
			cout << "在Debug模式下退出." << endl;
			return;
		}
		ofstream outFile(DEC_OUTFILENAME, ios::out | ios::binary);
		outFile.write(outcharlist, filesizeByte);
		free(outcharlist);
		cout << "Decoder : Everything Done!" << endl;
	}
}