// NCURoboteamDlg.h : ͷ�ļ�
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


// CNCURoboteamDlg �Ի���
class CNCURoboteamDlg : public CDialog
{
	// ����
public:
	//////////////////LK����������//////////////////////////////////
	string window_name = "optical flow tracking";
	Mat gray;	// ��ǰͼƬ
	Mat gray_prev;	// Ԥ��ͼƬ
	vector<Point2f> points[2];	// point0Ϊ�������ԭ��λ�ã�point1Ϊ���������λ��
	vector<Point2f> initial;	// ��ʼ�����ٵ��λ��
	vector<Point2f> features;	// ��������
	int maxCount;	// �������������
	double qLevel;	// �������ĵȼ�
	double minDist;	// ��������֮�����С����
	vector<uchar> Status;	// ����������״̬��������������Ϊ1������Ϊ0
	vector<float> err;
	///////////////////////////////////////////////////////////////
	Mat frame;
	Mat result;
	///////////////////////////////////////////////////////////////
	Mat fore_frame;
	Mat back_frame;
	Mat current_gray;
	Mat last_gray;
	Mat closerect = getStructuringElement(MORPH_RECT, Size(3, 3)); //���нṹ��������
	Mat first;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	//TLD framework
	int x, y;
	VideoCapture capture;//����ͷ��׽
	vector<Point2f> pts1;
	vector<Point2f> pts2;
	vector<Point2f> Trajectory_P;
	bool status;
	CvPoint point0;//������¼�ұ߲�ɫͼ�������ԲȦ���ĵ�
	CvPoint point1;//������¼��߱�����֡�ǰ����ȡͼ�����ë��ԲȦ���ĵ�
	float Point_Angle;//��ĽǶ�
	CString str_data;
	CString face_data;
	int frames;//��¼�ѹ�ȥ֡�� 
	int detections;//��¼�ɹ���⵽��Ŀ��box��Ŀ
	int init_number;
	CRect videorect;
	CPoint m_p, m_ptold, m_ptstart;
	BOOL m_d;//����������õ���void CNCURoboteamDlg::OnMouseMove(UINT nFlags, CPoint point)
	BOOL Data_Send_Flag;//���ڷ������ݱ�־λ
	BOOL Data_Receive_Flag;//���ڽ������ݱ�־λ
	int largestComp;
	Point2f zuobiao_P[4];
	//	BackgroundSubtractorMOG bgSubtractor;
	BackgroundSubtractorMOG2 bgSubtractor2;
	void ImageToDC(Mat &Frame, UINT ID);//��Matͼ��ת��Picture�ؼ�����ʾ����
	void Processing();//ͼ������

	void tracking(Mat &frame, Mat &output);	
	bool addNewPoints();
	bool acceptTrackedPoint(int i);
	void drawCornerOnImage(Mat& image, const Mat&binary);
	CNCURoboteamDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CNCURoboteamDlg();
	// �Ի�������
	enum { IDD = IDD_NCUROBOTEAM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


	// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
