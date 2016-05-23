///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	2015.3.17
//	Author:朱幸尔
//	Content:用来做跟踪初始化， 和调用TLD算法执行跟踪
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////


#pragma once

#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <string> 
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <cv.h>
#include <cxcore.h>
#include <cvaux.h>
#include <highgui.h>

using namespace cv;
using namespace std;



class NCURoboteam_D_T
{
public:
	Point NCURoboteam_D_T::Finding(Mat& Image);
	void NCURoboteam_D_T::TrackInit(Mat& frame);
	NCURoboteam_D_T();
	virtual	~NCURoboteam_D_T();
private:

};


