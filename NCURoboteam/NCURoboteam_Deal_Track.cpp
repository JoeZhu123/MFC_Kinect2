///////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	2015.3.17
//	Author:���Ҷ�
//	Content:���������ٳ�ʼ���� �͵���TLD�㷨ִ�и���
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////


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
#include "NCURoboteam_Deal_Track.h"



//Global variables
Rect box;
bool drawing_box;
bool gotBB;
bool tl;
string video;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
NCURoboteam_D_T::NCURoboteam_D_T()
{

}

NCURoboteam_D_T::~NCURoboteam_D_T()
{

}

Point NCURoboteam_D_T::Finding(Mat& Image)
{
	Point pt;
	int distinct_p[8] = { 0 };//�����ڱ�����ǰ�����ص���������
	cvtColor(Image,Image,CV_RGB2GRAY);
	int h = Image.rows;//�����У���ͼ���
	int w = Image.cols;//�����У���ͼ���
	uchar* Data = Image.data;
	int step = Image.step;
	//ͼ��ɨ��
	for (int y = 2; y < (h-2); y++)
	{
		for (int x = 2; x < (w-2); x++)
		{
			distinct_p[0] = Data[(y-2)*step + x];
			distinct_p[1] = Data[(y-1)*step + x];
			distinct_p[2] = Data[y*step + (x-2)];
			distinct_p[3] = Data[y*step + (x-1)];
			distinct_p[4] = Data[y*step + (x+1)];
			distinct_p[5] = Data[y*step + (x+2)];
			distinct_p[6] = Data[(y+1)*step + x];
			distinct_p[7] = Data[(y+2)*step + x];
			if ((distinct_p[0] > 10) && (distinct_p[1] > 10) && (distinct_p[2] > 10) && (distinct_p[3] > 10) && (distinct_p[4] > 10) && (distinct_p[5] > 10) && (distinct_p[6] > 10) && (distinct_p[7] > 10))
			{
				pt.x = x;
				pt.y = y;
				return pt;
			}
		}
	}
	pt.x = 0;
	pt.y = 0;
	return pt;
}

void NCURoboteam_D_T::TrackInit(Mat& frame)
{
	
}
















