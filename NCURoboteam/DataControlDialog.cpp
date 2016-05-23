// DataControlDialog.cpp : implementation file
//

#include "stdafx.h"
#include "NCURoboteam.h"
#include "NCURoboteamDlg.h"
#include "DataControlDialog.h"
#include "afxdialogex.h"
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
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
// CDataControlDialog dialog

IMPLEMENT_DYNAMIC(CDataControlDialog, CDialogEx)

CDataControlDialog::CDataControlDialog(CWnd* pParent /*=NULL*/)
: CDialogEx(CDataControlDialog::IDD, pParent)
, m_Data(_T(""))
{

}

CDataControlDialog::~CDataControlDialog()
{

}

void CDataControlDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Data, m_Data);
	DDX_Control(pDX, IDC_COM_Choice, m_ComChoice);
	DDX_Control(pDX, IDC_COMBOD, m_Bod);
	DDX_Control(pDX, IDC_MSCOMM1, m_ComControl);
	DDX_Control(pDX, IDC_Data, m_Data_Control);
}


BEGIN_MESSAGE_MAP(CDataControlDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDataControlDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDataControlDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_GetData, &CDataControlDialog::OnBnClickedGetdata)
	ON_BN_CLICKED(IDC_CloseCOM, &CDataControlDialog::OnBnClickedClosecom)
	ON_BN_CLICKED(IDC_ClearImage, &CDataControlDialog::OnBnClickedClearimage)
	ON_BN_CLICKED(IDC_ClearData, &CDataControlDialog::OnBnClickedCleardata)
	ON_BN_CLICKED(IDC_LoadImage, &CDataControlDialog::OnBnClickedLoadimage)
END_MESSAGE_MAP()


BOOL CDataControlDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化代码
	//以下是DataControlDialog的初始化程序
	CString str;
	int com_i, com_j;
	for (com_i = 0; com_i < 15; com_i++)
	{
		str.Format(_T("COM %d"), com_i + 1);
		m_ComChoice.InsertString(com_i, str);
	}
	m_ComChoice.SetCurSel(0);//预置COM口
	CString str1[] = { _T("300"), _T("600"), _T("1200"), _T("2400"), _T("4800"), _T("9600"), _T("19200"), _T("38400"), _T("43000"), _T("56000"), _T("57600"), _T("115200") };
	for (com_j = 0; com_j < 12; com_j++)
	{
		int judge_tf = m_Bod.AddString(str1[com_j]);
		if ((judge_tf == CB_ERR) || judge_tf == CB_ERRSPACE)
		{
			MessageBox(_T("Build Baud Error !"));
		}
	}
	m_Bod.SetCurSel(11);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
// CDataControlDialog message handlers


void CDataControlDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (m_ComControl.get_PortOpen())
	{
		m_ComControl.put_PortOpen(false);						//如果串口是打开的，则关闭
	}
	CDialogEx::OnOK();
	DestroyWindow();//非模态：只是隐藏，没有销毁，需要重写基类的OnOK虚函数，并且调用DestroyWindow函数
}


void CDataControlDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	if (m_ComControl.get_PortOpen())
	{
		m_ComControl.put_PortOpen(false);						//如果串口是打开的，则关闭
	}
	CDialogEx::OnCancel();
	DestroyWindow();//取消时，也要重写OnCancel，然后在重写函数中DestroyWindow函数，并且不能再调用基类的OnOK 和OnCancel
}


void CDataControlDialog::OnBnClickedGetdata()
{
	// TODO: Add your control notification handler code here	
	SetTimer(3, 25, NULL);//定时器1，定时时间和摄像机帧率一致	
	//UpdateData(false);		
	//this->SetDlgItemText(IDC_Data, m_Data);//将字符串str中内容显示到ID为IDC_EDIT_RECV的编辑框
	//m_Data_Control.LineScroll(m_Data_Control.GetLineCount());//m_recvshw为该编辑框的成员变量（也就是control类型的变量）//使用控件变量的一些成员函数设置滚动条的位置//更新数据
}


void CDataControlDialog::OnBnClickedSenddata()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);								//更新控件数据
	m_ComControl.put_Output(COleVariant(m_Data));	//把发送编辑框的数据发送出去
}


void CDataControlDialog::OnBnClickedClosecom()
{
	// TODO: Add your control notification handler code here
	CString str, str1, n;
	GetDlgItemText(IDC_CloseCOM, str);
	CWnd *h1;
	h1 = GetDlgItem(IDC_CloseCOM);		//指向控件的Caption
	if (!m_ComControl.get_PortOpen())
	{
		m_Bod.GetLBText(m_Bod.GetCurSel(), str1);//取得所选的字符，并放在str1里面
		str1 = str1 + ',' + 'n' + ',' + '8' + ',' + '1';//增加校验位，如果不是“n，8，1”，直接在程序里改

		m_ComControl.put_CommPort(m_ComChoice.GetCurSel() + 1);	//选择串口
		m_ComControl.put_InputMode(1);							//设置输入方式为二进制方式
		m_ComControl.put_Settings(str1);						//波特率为（无校验位，8数据位，1个停止位）
		m_ComControl.put_InputLen(65536);						//设置当前接受区数据长度为65536
		m_ComControl.put_RThreshold(1);							//缓冲区一个字符引发事件
		m_ComControl.put_RTSEnable(1);							//设置RT允许

		m_ComControl.put_PortOpen(true);						//打开串口
		if (m_ComControl.get_PortOpen())
		{
			str = _T("关闭串口");
			UpdateData(true);
			h1->SetWindowTextW(str);							//改变按钮名称为“关闭串口”
		}
		else
		{
			m_ComControl.put_PortOpen(false);
			if (str != _T("打开串口"))
			{
				str = _T("打开串口");
				UpdateData(true);
				h1->SetWindowTextW(str);						//改变按钮名称为打开串口
			}
		}

	}
}


void CDataControlDialog::OnBnClickedClearimage()
{
	// TODO: Add your control notification handler code here
}


void CDataControlDialog::OnBnClickedCleardata()
{
	// TODO: Add your control notification handler code here
	m_Data = _T("");											//给编辑框发送空格符
	KillTimer(3);
	UpdateData(false);											//更新数据
}


void CDataControlDialog::OnBnClickedLoadimage()
{
	// TODO: Add your control notification handler code here
	Mat court = imread("badminton_court.jpg");
	imageToDC(court, IDC_3D_Badminton);
}

BEGIN_EVENTSINK_MAP(CDataControlDialog, CDialogEx)
	ON_EVENT(CDataControlDialog, IDC_MSCOMM1, 1, CDataControlDialog::OnCommMscomm1, VTS_NONE)
END_EVENTSINK_MAP()


void CDataControlDialog::OnCommMscomm1()
{
	// TODO: Add your message handler code here
	if (m_ComControl.get_CommEvent() == 2)
	{
		char str[65536] = { 0 };
		long k;
		VARIANT InputData = m_ComControl.get_Input();			//读缓冲区
		COleSafeArray fs;
		fs = InputData;											//VARIANT型变量转换为COleSafeArray型变量
		for (k = 0; k < fs.GetOneDimSize(); k++)
		{
			fs.GetElement(&k, str + k);							//转换为BYTE型数组
		}
		m_Data += str;											//接收到编辑框里

		UpdateData(false);
	}
}

void CDataControlDialog::imageToDC(Mat &Frame, UINT ID_Picture)
{
	IplImage* Ima;
	CRect rect;
	CStatic* pStc;//标识图像显示的Picture控件
	CDC* pDC;//视频显示控件设备上下文
	HDC  hDC;//视频显示控件设备句柄
	pStc = (CStatic *)GetDlgItem(ID_Picture);//ID_Picture为任意传入Picture控件ID
	pStc->GetClientRect(&rect);//将CWind类客户区的坐标点传给矩形
	pDC = pStc->GetDC();//得到Picture控件设备上下文
	hDC = pDC->GetSafeHdc();//得到Picture控件设备上下文的句柄

	if (pDC&&rect&&!Frame.empty())
	{
		uchar buffer[sizeof(BITMAPINFOHEADER) + 1024];
		BITMAPINFO* bmi = (BITMAPINFO*)buffer;
		int bmp_w = Frame.cols, bmp_h = Frame.rows;

		BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);
		memset(bmih, 0, sizeof(*bmih));
		bmih->biSize = sizeof(BITMAPINFOHEADER);
		bmih->biWidth = bmp_w;
		bmih->biHeight = -bmp_h;//-abs(Height)
		bmih->biPlanes = 1;
		bmih->biBitCount = IPL_DEPTH_8U*Frame.channels();
		bmih->biCompression = BI_RGB;

		if (IPL_DEPTH_8U*Frame.channels() == 8)
		{
			RGBQUAD* palette = bmi->bmiColors;//palette:调色板
			int IMA_i;
			for (IMA_i = 0; IMA_i < 255; IMA_i++)
			{
				palette[IMA_i].rgbBlue = palette[IMA_i].rgbGreen = palette[IMA_i].rgbRed = (BYTE)IMA_i;
				palette[IMA_i].rgbReserved = 0;
			}
		}
		if (rect.right - rect.left + 1 < bmp_w)
		{
			SetStretchBltMode(pDC->m_hDC, HALFTONE);//handle to device context;device context:设备上下文
		}
		else
		{
			SetStretchBltMode(pDC->m_hDC, COLORONCOLOR);//handle to device context
		}
		if (Frame.cols % 4 != 0 && Frame.isContinuous())
		{
			Ima = cvCreateImage(Frame.size(), 8, Frame.channels());
			Frame.copyTo(Mat(Ima));
			::StretchDIBits(pDC->m_hDC, rect.left, rect.top, rect.right - rect.left + 1, rect.bottom - rect.top + 1, 0, 0, bmp_w, bmp_h, Ima->imageData, bmi, DIB_RGB_COLORS, SRCCOPY);
			cvReleaseImage(&Ima);
		}
		else
		{
			::StretchDIBits(pDC->m_hDC, rect.left, rect.top, rect.right - rect.left + 1, rect.bottom - rect.top + 1, 0, 0, bmp_w, bmp_h, Frame.data, bmi, DIB_RGB_COLORS, SRCCOPY);
		}
	}

	ReleaseDC(pDC);
	return;
}

