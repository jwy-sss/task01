#pragma once
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/types_c.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

namespace BS {
	using namespace std;
	using namespace cv;

	vector<vector<vector<Point>>> getAllInfoCenters(const Mat& src, int frame);
}