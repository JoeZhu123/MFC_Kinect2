
// NCURoboteam.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include <opencv2/opencv.hpp>
#include <string> 
#include <iostream>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>



// CNCURoboteamApp: 
// �йش����ʵ�֣������ NCURoboteam.cpp
//

class CNCURoboteamApp : public CWinApp
{
public:
	CNCURoboteamApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CNCURoboteamApp theApp;



