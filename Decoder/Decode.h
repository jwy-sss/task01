#pragma once
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
	void decode();
}