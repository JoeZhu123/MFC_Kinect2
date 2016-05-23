// NCURoboteamDlg.h : 头文件
//

#pragma once

#include <opencv2/opencv.hpp>
#include <string> 
#include <iostream>
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "DataControlDialog.h"
#include "afxwin.h"
#include "mscomm1.h"
#include <vector>


using namespace cv;
using namespace std;


// CNCURoboteamDlg 对话框
class CNCURoboteamDlg : public CDialog
{
	// 构造
public:
	//////////////////LK光流法参数//////////////////////////////////
	string window_name = "optical flow tracking";
	Mat gray;	// 当前图片
	Mat gray_prev;	// 预测图片
	vector<Point2f> points[2];	// point0为特征点的原来位置，point1为特征点的新位置
	vector<Point2f> initial;	// 初始化跟踪点的位置
	vector<Point2f> features;	// 检测的特征
	int maxCount;	// 检测的最大特征数
	double qLevel;	// 特征检测的等级
	double minDist;	// 两特征点之间的最小距离
	vector<uchar> Status;	// 跟踪特征的状态，特征的流发现为1，否则为0
	vector<float> err;
	///////////////////////////////////////////////////////////////
	Mat frame;
	Mat result;
	///////////////////////////////////////////////////////////////
	Mat fore_frame;
	Mat back_frame;
	Mat current_gray;
	Mat last_gray;
	Mat closerect = getStructuringElement(MORPH_RECT, Size(3, 3)); //进行结构算子生成
	Mat first;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	//TLD framework
	int x, y;
	VideoCapture capture;//摄像头捕捉
	vector<Point2f> pts1;
	vector<Point2f> pts2;
	vector<Point2f> Trajectory_P;
	bool status;
	CvPoint point0;//用来记录右边彩色图像的人脸圆圈中心点
	CvPoint point1;//用来记录左边背景差分、前景提取图像的羽毛球圆圈中心点
	float Point_Angle;//球的角度
	CString str_data;
	CString face_data;
	int frames;//记录已过去帧数 
	int detections;//记录成功检测到的目标box数目
	int init_number;
	CRect videorect;
	CPoint m_p, m_ptold, m_ptstart;
	BOOL m_d;//这个函数里用到了void CNCURoboteamDlg::OnMouseMove(UINT nFlags, CPoint point)
	BOOL Data_Send_Flag;//串口发送数据标志位
	BOOL Data_Receive_Flag;//串口接收数据标志位
	int largestComp;
	Point2f zuobiao_P[4];
	//	BackgroundSubtractorMOG bgSubtractor;
	BackgroundSubtractorMOG2 bgSubtractor2;
	void ImageToDC(Mat &Frame, UINT ID);//将Mat图像转到Picture控件中显示函数
	void Processing();//图像处理函数

	void tracking(Mat &frame, Mat &output);	
	bool addNewPoints();
	bool acceptTrackedPoint(int i);
	void drawCornerOnImage(Mat& image, const Mat&binary);
	CNCURoboteamDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CNCURoboteamDlg();
	// 对话框数据
	enum { IDD = IDD_NCUROBOTEAM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedOpen();
	afx_msg void OnBnClickedStartprocessed();
	afx_msg void OnBnClickedTrackinit();
	afx_msg void OnBnClickedTrackstart();
	afx_msg void OnBnClickedDatapanel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
private:
	bool videoflag;
	bool processedflag;
	bool initflag;
	bool trackedflag;
	bool frameload;
	CDataControlDialog *pTD;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_facedata;
	CEdit m_FaceControl;
	CComboBox m_ComXuan;
	CComboBox m_BodXuan;
	afx_msg void OnBnClickedComkou();
	afx_msg void OnBnClickedComsend();
	CMscomm1 m_MSCom;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	CEdit m_FaceControl2;
	CString m_facedata2;
	CComboBox m_ComXuan2;
	CComboBox m_BodXuan2;
	CMscomm1 m_MSCom2;
	void OnCommMscomm2();
	afx_msg void OnBnClickedComkou2();
	afx_msg void OnBnClickedComsend2();
	afx_msg void OnEnChangeFace();
};
