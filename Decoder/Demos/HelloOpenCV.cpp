#include<opencv2/opencv.hpp>
#include<iostream>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2\imgproc\types_c.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../Settings.h"

using namespace cv;
using namespace std;


Mat src; Mat src_gray;


RNG rng(12345);
//Scalar colorful = CV_RGB(rng.uniform(0,255),rng.uniform(0,255),rng.uniform(0,255));

//获取轮廓的中心点
Point Center_cal(vector<vector<Point> > contours, int i) {
    int centerx = 0, centery = 0, n = contours[i].size();
    //在提取的小正方形的边界上每隔周长个像素提取一个点的坐标，
    //求所提取四个点的平均坐标（即为小正方形的大致中心）
    centerx = (contours[i][n / 4].x + contours[i][n * 2 / 4].x + contours[i][3 * n / 4].x + contours[i][n - 1].x) / 4;
    centery = (contours[i][n / 4].y + contours[i][n * 2 / 4].y + contours[i][3 * n / 4].y + contours[i][n - 1].y) / 4;
    Point point1 = Point(centerx, centery);
    return point1;
}

int main_free(int argc, char** argv[]) {
    src = imread(PATH_PROC_PNG);
    Mat src_all = src.clone();


    //对图像进行平滑处理
    blur(src, src, Size(2, 2));
    //彩色图转灰度图
    cvtColor(src, src_gray, CV_BGR2GRAY);
    /*删去直方图均衡化操作，防止红色被去除*/
    //使灰度图象直方图均衡化
    //equalizeHist(src_gray, src_gray);
    //imwrite(string(PATH_HELLO_OUT)+string("src_gray.png"), src_gray);


    Scalar color = Scalar(1, 1, 255);
    Mat threshold_output;
    vector<vector<Point> > contours, contours2;
    vector<Vec4i> hierarchy;
    Mat drawing = Mat::zeros(src.size(), CV_8UC3);
    Mat drawing2 = Mat::zeros(src.size(), CV_8UC3);
    Mat drawingAllContours = src.clone();

    /*设置75为阈值获得最佳二值化图像*/
    //指定112阀值进行二值化
    //threshold(src_gray, threshold_output, 40, 255, THRESH_BINARY);
    //imwrite(string(PATH_HELLO_OUT) + string("threshold_output040.png"), threshold_output);
    //指定112阀值进行二值化
    threshold(src_gray, threshold_output, 75, 255, THRESH_BINARY);
    imwrite(string(PATH_HELLO_OUT) + string("threshold_output075.png"), threshold_output);
    //threshold(src_gray, threshold_output, 100, 255, THRESH_BINARY);
    //imwrite(string(PATH_HELLO_OUT) + string("threshold_output100.png"), threshold_output);
    //threshold(src_gray, threshold_output, 112, 255, THRESH_BINARY);
    //imwrite(string(PATH_HELLO_OUT) + string("threshold_output112.png"), threshold_output);

    //exit(0);

    /*查找轮廓
     *  参数说明
        输入图像image必须为一个2值单通道图像
        contours参数为检测的轮廓数组，每一个轮廓用一个point类型的vector表示
        hiararchy参数和轮廓个数相同，每个轮廓contours[ i ]对应4个hierarchy元素hierarchy[ i ][ 0 ] ~hierarchy[ i ][ 3 ]，
            分别表示后一个轮廓、前一个轮廓、父轮廓、内嵌轮廓的索引编号，如果没有对应项，该值设置为负数。
        mode表示轮廓的检索模式
            CV_RETR_EXTERNAL 表示只检测外轮廓
            CV_RETR_LIST 检测的轮廓不建立等级关系
            CV_RETR_CCOMP 建立两个等级的轮廓，上面的一层为外边界，里面的一层为内孔的边界信息。如果内孔内还有一个连通物体，这个物体的边界也在顶层。
            CV_RETR_TREE 建立一个等级树结构的轮廓。具体参考contours.c这个demo
        method为轮廓的近似办法
            CV_CHAIN_APPROX_NONE 存储所有的轮廓点，相邻的两个点的像素位置差不超过1，即max（abs（x1-x2），abs（y2-y1））==1
            CV_CHAIN_APPROX_SIMPLE 压缩水平方向，垂直方向，对角线方向的元素，只保留该方向的终点坐标，例如一个矩形轮廓只需4个点来保存轮廓信息
            CV_CHAIN_APPROX_TC89_L1，CV_CHAIN_APPROX_TC89_KCOS 使用teh-Chinl chain 近似算法
        offset表示代表轮廓点的偏移量，可以设置为任意值。对ROI图像中找出的轮廓，并要在整个图像中进行分析时，这个参数还是很有用的。
     */
    findContours(threshold_output, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_NONE, Point(0, 0));

    int c = 0, ic = 0, k = 0, area = 0;

    //通过黑色定位角作为父轮廓，有两个子轮廓的特点，筛选出三个定位角
    int parentIdx = -1;
    cout << src.size().height << " " << src.size().width << "\n";
    int contourOutputID = 0;
    for (int i = 0; i < contours.size(); i++) {
        if (contours[i].size() > 1000) {
            /*void drawContours(InputOutputArray image, InputArrayOfArrays contours, int contourIdx, const Scalar & color,
                int thickness = 1, int lineType = 8, InputArray hierarchy = noArray(), int maxLevel = INT_MAX, Point offset = Point())
                函数参数详解：
                其中第一个参数image表示目标图像
                第二个参数contours表示输入的轮廓组，每一组轮廓由点vector构成，
                第三个参数contourIdx指明画第几个轮廓，如果该参数为负值，则画全部轮廓，
                第四个参数color为轮廓的颜色，
                第五个参数thickness为轮廓的线宽，如果为负值或CV_FILLED表示填充轮廓内部，
                第六个参数lineType为线型，
                第七个参数为轮廓结构信息，
                第八个参数为maxLevel*/
            std::cout << "contours " << i << " size : " << (int64)(contours[i].size()) << endl;
            Scalar colors[] = { CV_RGB(255,100,100), CV_RGB(100,255,100), CV_RGB(100,100,255) };
            drawContours(drawingAllContours, contours, i, colors[contourOutputID++%3], 1);
        }
    }

    //填充的方式画出三个黑色定位角的轮廓
    //for (int i = 0; i < contours2.size(); i++)
    //    drawContours(drawing2, contours2, i, CV_RGB(rng.uniform(100, 255), rng.uniform(100, 255), rng.uniform(100, 255)), -1, 4, hierarchy[k][2], 0, Point());

    imwrite(string(PATH_HELLO_OUT) + string("src_drawingAllContours.png"), drawingAllContours);
    exit(0);

    //获取三个定位角的中心坐标
    Point point[3];
    for (int i = 0; i < contours2.size(); i++) {
        point[i] = Center_cal(contours2, i);
    }

    //计算轮廓的面积，计算定位角的面积，从而计算出边长
    area = contourArea(contours2[1]);
    int area_side = cvRound(sqrt(double(area)));
    for (int i = 0; i < contours2.size(); i++) {
        //画出三个定位角的中心连线
        line(drawing2, point[i % contours2.size()], point[(i + 1) % contours2.size()], color, area_side / 2, 8);
    }

    namedWindow("DrawingAllContours");
    imshow("DrawingAllContours", drawingAllContours);

    namedWindow("Drawing2");
    imshow("Drawing2", drawing2);

    namedWindow("Drawing");
    imshow("Drawing", drawing);


    //接下来要框出这整个二维码
    Mat gray_all, threshold_output_all;
    vector<vector<Point> > contours_all;
    vector<Vec4i> hierarchy_all;
    cvtColor(drawing2, gray_all, CV_BGR2GRAY);


    threshold(gray_all, threshold_output_all, 45, 255, THRESH_BINARY);
    findContours(threshold_output_all, contours_all, hierarchy_all, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point(0, 0));//RETR_EXTERNAL表示只寻找最外层轮廓


    Point2f fourPoint2f[4];
    //求最小包围矩形
    RotatedRect rectPoint = minAreaRect(contours_all[0]);

    //将rectPoint变量中存储的坐标值放到 fourPoint的数组中
    rectPoint.points(fourPoint2f);


    for (int i = 0; i < 4; i++) {
        line(src_all, fourPoint2f[i % 4], fourPoint2f[(i + 1) % 4]
            , Scalar(20, 21, 237), 3);
    }

    namedWindow("Src_all");
    imshow("Src_all", src_all);

    //框出二维码后，就可以提取出二维码，然后使用解码库zxing，解出码的信息。
    //或者研究二维码的排布规则，自己写解码部分

    waitKey(0);
    return(0);
}

//————————————————
//版权声明：本文为CSDN博主「nickchao的IT」的原创文章，遵循 CC 4.0 BY - SA 版权协议，转载请附上原文出处链接及本声明。
//原文链接：https ://blog.csdn.net/nick123chao/article/details/77573675