//NCURoboteamDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "NCURoboteam.h"
#include "NCURoboteamDlg.h"
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
#include "NCURoboteam_Deal_Track.h"
#include "DataControlDialog.h"
#include <vector>



#define Frame_Height 240
#define Frame_Width 340

using namespace cv;
using namespace std;

//Global variables ȫ�ֱ����Ķ���
extern Rect box;
extern bool drawing_box;
extern bool gotBB;
extern bool tl;
extern string video;
/** Global variables */
String face_cascade_name = "haarcascade_frontalface_default.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
RNG rng(12345);


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

/*********************************************************************************/
// CNCURoboteamDlg �Ի���



CNCURoboteamDlg::CNCURoboteamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNCURoboteamDlg::IDD, pParent)
	, m_facedata(_T(""))
	, m_facedata2(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CNCURoboteamDlg::~CNCURoboteamDlg()
{
	delete pTD;
}


void CNCURoboteamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_Face, m_facedata);
	DDX_Control(pDX, IDC_Face, m_FaceControl);
	DDX_Control(pDX, IDC_ComNum, m_ComXuan);
	DDX_Control(pDX, IDC_ComBaud, m_BodXuan);
	DDX_Control(pDX, IDC_MSCOMM1, m_MSCom);
	DDX_Control(pDX, IDC_Face2, m_FaceControl2);
	DDX_Text(pDX, IDC_Face2, m_facedata2);
	DDX_Control(pDX, IDC_ComNum2, m_ComXuan2);
	DDX_Control(pDX, IDC_ComBaud2, m_BodXuan2);
	DDX_Control(pDX, IDC_MSCOMM2, m_MSCom2);
}

BEGIN_MESSAGE_MAP(CNCURoboteamDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_Close, &CNCURoboteamDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_Open, &CNCURoboteamDlg::OnBnClickedOpen)
	ON_BN_CLICKED(IDOK, &CNCURoboteamDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CNCURoboteamDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_StartProcessed, &CNCURoboteamDlg::OnBnClickedStartprocessed)
	ON_BN_CLICKED(IDC_TrackInit, &CNCURoboteamDlg::OnBnClickedTrackinit)
	ON_BN_CLICKED(IDC_TrackStart, &CNCURoboteamDlg::OnBnClickedTrackstart)
	ON_BN_CLICKED(IDC_DataPanel, &CNCURoboteamDlg::OnBnClickedDatapanel)
	ON_BN_CLICKED(IDC_COMKOU, &CNCURoboteamDlg::OnBnClickedComkou)
	ON_BN_CLICKED(IDC_COMSend, &CNCURoboteamDlg::OnBnClickedComsend)
	ON_BN_CLICKED(IDC_COMKOU2, &CNCURoboteamDlg::OnBnClickedComkou2)
	ON_BN_CLICKED(IDC_COMSend2, &CNCURoboteamDlg::OnBnClickedComsend2)
	ON_EN_CHANGE(IDC_Face, &CNCURoboteamDlg::OnEnChangeFace)
END_MESSAGE_MAP()


// CNCURoboteamDlg ��Ϣ�������

BOOL CNCURoboteamDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//��ʼ����־λ	
	Data_Send_Flag = false;
	Data_Receive_Flag = false;
	videoflag = false;
	frameload = false;
	processedflag = false;
	initflag = false;
	trackedflag = false;
	drawing_box = false;
	gotBB = false;
	tl = true;
	capture = NULL;
	status = true;
	frames = 1;//��¼�ѹ�ȥ֡�� 
	detections = 1;//��¼�ɹ���⵽��Ŀ��box��Ŀ
	init_number = 0;
	//-- 1. Load the cascade
	face_cascade.load(face_cascade_name);
	eyes_cascade.load(eyes_cascade_name);
	largestComp = -1;
	point0.x = 0;
	point0.y = 0;
	point1.x = 0;
	point1.y = 0;
	Point_Angle = 0;
	Trajectory_P.reserve(10);//�����Ȳ��䣬��������Ϊ10
	///////////////////////////�������������ĵ������ʼ��/////////////////////////
	zuobiao_P[0].x = 12;
	zuobiao_P[0].y = 34;
	zuobiao_P[1].x = 12;
	zuobiao_P[1].y = 34;
	zuobiao_P[2].x = 12;
	zuobiao_P[2].y = 34;
	zuobiao_P[3].x = 12;
	zuobiao_P[3].y = 34;
	/////////////////////////////////////////////////////////////////////////////
	maxCount = 500;	// �������������
	qLevel = 0.01;	// �������ĵȼ�
	minDist = 10.0;	// ��������֮�����С����
	//BackgroundSubtractorMOG::BackgroundSubtractorMOG(int history, int nmixtures,double backgroundRatio, double noiseSigma = 0)
	//����historyΪʹ����ʷ֡����Ŀ��nmixturesΪ��ϸ�˹������backgroundRatioΪ����������noiseSigmaΪ����Ȩ�ء�
	//	BackgroundSubtractorMOG::BackgroundSubtractorMOG(20, 10, 0.5 , false);

	//BackgroundSubtractorMOG2::BackgroundSubtractorMOG2(int history, float varThreshold, bool bShadowDetection = true )
	//historyͬ�ϣ�varThreshold��ʾ����ƽ��������ʹ�õ����ж��Ƿ�Ϊ��������ֵ����ֵ��Ӱ�챳���������ʣ���
	//	bShadowDetection��ʾ�Ƿ�ʹ����Ӱ��⣨���������Ӱ��⣬��mask��ʹ��127��ʾ��Ӱ����
	BackgroundSubtractorMOG2::BackgroundSubtractorMOG2(20, 16, false);
	/////////////////////////////////////���ʹ��ڳ�ʼ��////////////////////////////////////////////////////////
	CString str;
	int com_i, com_j;
	for (com_i = 0; com_i < 15; com_i++)
	{
		str.Format(_T("COM %d"), com_i + 1);
		m_ComXuan.InsertString(com_i, str);
	}
	m_ComXuan.SetCurSel(0);//Ԥ��COM��
	CString str1[] = { _T("300"), _T("600"), _T("1200"), _T("2400"), _T("4800"), _T("9600"), _T("19200"), _T("38400"), _T("43000"), _T("56000"), _T("57600"), _T("115200") };
	for (com_j = 0; com_j < 12; com_j++)
	{
		int judge_tf = m_BodXuan.AddString(str1[com_j]);
		if ((judge_tf == CB_ERR) || judge_tf == CB_ERRSPACE)
		{
			MessageBox(_T("Build Baud Error !"));
		}
	}
	m_BodXuan.SetCurSel(11);	
	/////////////////////////////////////���մ��ڳ�ʼ��////////////////////////////////////////////////////////
	CString str2;
	int com_i2, com_j2;
	for (com_i2 = 0; com_i2 < 15; com_i2++)
	{
		str2.Format(_T("COM %d"), com_i2 + 1);
		m_ComXuan2.InsertString(com_i2, str2);
	}
	m_ComXuan2.SetCurSel(0);//Ԥ��COM��
	CString str3[] = { _T("300"), _T("600"), _T("1200"), _T("2400"), _T("4800"), _T("9600"), _T("19200"), _T("38400"), _T("43000"), _T("56000"), _T("57600"), _T("115200") };
	for (com_j2 = 0; com_j2 < 12; com_j2++)
	{
		int judge_tf2 = m_BodXuan2.AddString(str3[com_j2]);
		if ((judge_tf2 == CB_ERR) || judge_tf2 == CB_ERRSPACE)
		{
			MessageBox(_T("Build Baud Error !"));
		}
	}
	m_BodXuan2.SetCurSel(11);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CNCURoboteamDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CNCURoboteamDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CNCURoboteamDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CNCURoboteamDlg::OnBnClickedClose()
{
	// TODO: Add your control notification handler code here
	videoflag = false;//�ر�����ͷ��������ͷ��־λ
	if (processedflag == true)
	{
		processedflag = false;
	}
	
	KillTimer(1);
	KillTimer(2);//�����2������Timer2�ı�ʶ
}


void CNCURoboteamDlg::OnBnClickedOpen()
{
	// TODO: Add your control notification handler code here
	HWND NCUWin = ::GetDlgItem(m_hWnd, IDC_VideoImage);
	capture.open(0);//��������ͷ
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 340);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 240);

	if (!capture.isOpened())
	{
		//��������ͷδ����
		AfxMessageBox(_T("����ͷδ������"), MB_OKCANCEL | MB_ICONEXCLAMATION);
	}
	else
	{
		videoflag = true;
		SetTimer(1, 25, NULL);//��ʱ��1����ʱʱ��������֡��һ��	
		m_facedata = _T(" ");
		UpdateData(false);//��������
	}
}

void CNCURoboteamDlg::OnTimer(UINT nIDEvent)
{
	switch (nIDEvent)
	{
	case 1:
	{
		if (videoflag == true)
		{
			//��Picture�ؼ�IDC_VideoImage����ʾ��Ƶ
			capture >> frame;
			std::vector<Rect> faces;
			Mat frame_gray;
			//////////////////////////Harris �ǵ���///////////////////////////////////////////////
			Mat gray;
			cvtColor(frame, gray, CV_BGR2GRAY);

			Mat cornerStrength;
			cornerHarris(gray, cornerStrength, 3, 3, 0.01);

			double maxStrength;
			double minStrength;
			// �ҵ�ͼ���е������Сֵ
			minMaxLoc(cornerStrength, &minStrength, &maxStrength);

			Mat dilated;
			Mat locaMax;
			// ����ͼ�����ҳ�ͼ����ȫ���ľֲ����ֵ��
			dilate(cornerStrength, dilated, Mat());
			// compare��һ���߼��ȽϺ�������������ͼ���ж�Ӧ����ͬ�Ķ�ֵͼ��
			compare(cornerStrength, dilated, locaMax, CMP_EQ);

			Mat cornerMap;
			double qualityLevel = 0.01;
			double th = qualityLevel*maxStrength; // ��ֵ����
			threshold(cornerStrength, cornerMap, th, 255, THRESH_BINARY);
			cornerMap.convertTo(cornerMap, CV_8U);
			// ����λ����
			bitwise_and(cornerMap, locaMax, cornerMap);

			drawCornerOnImage(frame, cornerMap);
			////////////////////////////////////////////////////////////////////////////////////
			point0.x = 0;
			point0.y = 0;
			cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
			equalizeHist(frame_gray, frame_gray);

			//-- Detect faces
			face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0, Size(80, 80));

			for (size_t i = 0; i < faces.size(); i++)
			{
				Mat faceROI = frame_gray(faces[i]);
				std::vector<Rect> eyes;

				//-- In each face, detect eyes
				eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
				if (eyes.size() == 2)
				{
					//-- Draw the face
					Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
					ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 0), 2, 8, 0);
					//�������ݴ��͵�DataControl-Dialog/////
					point0.x = center.x;
					point0.y = center.y;
					///////////////////////////////////////
					for (size_t j = 0; j < eyes.size(); j++)
					{ //-- Draw the eyes
						Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
						int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
						circle(frame, eye_center, radius, Scalar(255, 0, 255), 3, 8, 0);
					}
				}
			}
			/*
			if ((point0.x != 0) && (point0.y != 0))
			{
				face_data.Format(_T("X = %d		Y = %d		"), point0.x, point0.y);
				m_facedata += face_data;
				this->SetDlgItemText(IDC_Face, m_facedata);//���ַ���str��������ʾ��IDΪIDC_EDIT_RECV�ı༭��
				m_FaceControl.LineScroll(m_FaceControl.GetLineCount()-1);//Ϊ�ñ༭��ĳ�Ա������Ҳ����control���͵ı�����//ʹ�ÿؼ�������һЩ��Ա�������ù�������λ��
				UpdateData(false);//��������
			}			
			*/
			if (pTD != NULL && point0.x != 0)
			{
				str_data.Format(_T("X = %d		Y = %d		"), point0.x, point0.y);
				pTD->m_Data += str_data + _T("Z = 0 \r\n");
			}
			if (!frame.empty())
			{
				tracking(frame, result);
			}
			ImageToDC(result, IDC_VideoImage);
		}
	}
	break;
	case 2:
	{
		//Timer2��OnTimer��Ҫ��������
		if (processedflag == true)
		{
			point1.x = 0;
			point1.y = 0;
			capture >> frame;
			cvtColor(frame, frame, CV_RGB2GRAY);
			Processing();
			morphologyEx(fore_frame, fore_frame, MORPH_OPEN, closerect);//������̬ѧ������
			/**********************************����ƥ��*******************************************************/
			/*
			vector<vector<Point>> w, w1;
			vector<Vec4i> hierarchy, hierarchy1;
			findContours(fore_frame, w, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);//��ȡ����Ԫ��
			//findContours(f1, w1, hierarchy1, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
			FileStorage fs("f.dat", FileStorage::WRITE);
			fs << "f" << w1[0];
			int idx = 0;
			double ffff = matchShapes(w[0], w1[0], CV_CONTOURS_MATCH_I3, 1.0);//��������ƥ��
			std::cout << ffff << std::endl;
			*/
			/***********************************************************************************************/
			//bgSubtractor2.getBackgroundImage(back_ground);				
			//findContours(fore_frame, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
			findContours(fore_frame, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
			vector<RotatedRect> minRect(contours.size());
			/// �����
			vector<Moments> mu(contours.size());
			///  �������ľ�:
			vector<Point2f> mc(contours.size());
			//iterate through all the top-level contours
			//draw each connected component with its own random color		
			for (int idx = 0; idx < contours.size(); idx++)
			{
				//��ȥ̫������̫�̵�����(ע��ֻ��һ���ǳ��򵥵��˳�����)  
				int cmin = 0;
				int cmax = 100;
				double whRatio_max = 10.0;//3//2.0//3.0
				double whRatio_min = 0.5;//1.5

				double dAreaMax = 280.0;//280
				double dAreaMin = 0.0;//50	

				double dConArea = fabs(contourArea(contours[idx]));
				if (dConArea > dAreaMax)
				{
					contours.erase(contours.begin() + idx);
					continue;
				}
				if (dConArea < dAreaMin)
				{
					contours.erase(contours.begin() + idx);
					continue;
				}
				Rect aRect = boundingRect(contours[idx]);
				if (((aRect.width / aRect.height) > whRatio_max) || ((aRect.width / aRect.height) < whRatio_min))
				{
					contours.erase(contours.begin() + idx);
					continue;
				}
				double cLength = arcLength(contours[idx], true);
				if ((cLength > cmax) || (cLength < cmin))
				{
					contours.erase(contours.begin() + idx);
					continue;
				}
			}
			Mat drawing = Mat::zeros(fore_frame.size(), CV_8UC3);
			for (int idx = 0; idx < contours.size(); idx++)
			{

				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				minRect[idx] = minAreaRect(Mat(contours[idx]));
				mu[idx] = moments(contours[idx], false);// �����
				mc[idx] = Point2f(mu[idx].m10 / mu[idx].m00, mu[idx].m01 / mu[idx].m00);//  �������ľ�

				drawContours(drawing, contours, idx, color, 2, 8, vector<Vec4i>(), 0, Point());
				circle(drawing, mc[idx], 20, color, 2, 8, 0);
				point1.x = mc[idx].x;
				point1.y = mc[idx].y;
				Point_Angle = minRect[idx].angle;

				Point2f rect_points[4];
				minRect[idx].points(rect_points);
				for (int j = 0; j < 4; j++)
				{
					line(drawing, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
				}
				largestComp++;
				Trajectory_P.resize(largestComp + 10);
				Trajectory_P[largestComp] = mc[idx];
				for (int i = 0; i < largestComp; i++)
				{
					if (i == 0)
					{
						line(drawing, Trajectory_P[i], Trajectory_P[i], color, 2, 8);
					}
					else if ((Trajectory_P[i].x > 100) && (Trajectory_P[i].y < 380))
					{
						line(drawing, Trajectory_P[i - 1], Trajectory_P[i], color, 2, 8);
					}
					else
					{
						largestComp = -1;
						Trajectory_P.clear();
					}
				}
			}
			//��Picture�ؼ�IDC_ProcessedImage����ʾ��Ƶ
			ImageToDC(drawing, IDC_ProcessedImage);//��ʾ�����˳����ͼ��	

			if ((point1.x != 0) && (point1.y != 0))
			{
				face_data.Format(_T("Y=%d     Z=%d     J=%0.f"), point1.x, point1.y, Point_Angle);
				m_facedata += face_data;
				m_facedata += "\r\n";
				this->SetDlgItemText(IDC_Face, m_facedata);//���ַ���str��������ʾ��IDΪIDC_EDIT_RECV�ı༭��
				m_FaceControl.LineScroll(m_FaceControl.GetLineCount(), 0);//Ϊ�ñ༭��ĳ�Ա������Ҳ����control���͵ı�����//ʹ�ÿؼ�������һЩ��Ա�������ù�������λ��
				UpdateData(false);//��������
				if (Data_Send_Flag)
				{
					UpdateData(true);								//���¿ؼ�����
					m_MSCom.put_Output(COleVariant(m_facedata));	//�ѷ��ͱ༭������ݷ��ͳ�ȥ
				}
			}
		}					
	}
	break;
	case 3:
	{
		this->SetDlgItemText(IDC_Data, pTD->m_Data);//���ַ���str��������ʾ��IDΪIDC_EDIT_RECV�ı༭��
		pTD->m_Data_Control.LineScroll(pTD->m_Data_Control.GetLineCount());//m_recvshwΪ�ñ༭��ĳ�Ա������Ҳ����control���͵ı�����//ʹ�ÿؼ�������һЩ��Ա�������ù�������λ��//��������
		UpdateData(false);											//��������
	}
	break;
	default:
		break;
	}
	CDialog::OnTimer(nIDEvent);
}

void CNCURoboteamDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	capture.release();
	KillTimer(1);
	KillTimer(2);//�����2������Timer2�ı�ʶ
	CDialog::OnOK();
}


void CNCURoboteamDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	capture.release();
	KillTimer(1);//�����1������Timer1�ı�ʶ
	KillTimer(2);//�����2������Timer2�ı�ʶ
	CDialog::OnCancel();
}

void CNCURoboteamDlg::ImageToDC(Mat &Frame, UINT ID_Picture)
{
	IplImage* Ima;
	CRect rect;
	CStatic* pStc;//��ʶͼ����ʾ��Picture�ؼ�
	CDC* pDC;//��Ƶ��ʾ�ؼ��豸������
	HDC  hDC;//��Ƶ��ʾ�ؼ��豸���
	pStc = (CStatic *)GetDlgItem(ID_Picture);//ID_PictureΪ���⴫��Picture�ؼ�ID
	pStc->GetClientRect(&rect);//��CWind��ͻ���������㴫������
	pDC = pStc->GetDC();//�õ�Picture�ؼ��豸������
	hDC = pDC->GetSafeHdc();//�õ�Picture�ؼ��豸�����ĵľ��

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
			RGBQUAD* palette = bmi->bmiColors;//palette:��ɫ��
			int IMA_i;
			for (IMA_i = 0; IMA_i < 255; IMA_i++)
			{
				palette[IMA_i].rgbBlue = palette[IMA_i].rgbGreen = palette[IMA_i].rgbRed = (BYTE)IMA_i;
				palette[IMA_i].rgbReserved = 0;
			}
		}
		if (rect.right - rect.left + 1 < bmp_w)
		{
			SetStretchBltMode(pDC->m_hDC, HALFTONE);//handle to device context;device context:�豸������
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

void CNCURoboteamDlg::OnBnClickedStartprocessed()
{
	// TODO: Add your control notification handler code here
	if (videoflag == true)
	{
		//���������ͷ�󣬲�������
		if (!frame.empty())
		{
			processedflag = true;
			SetTimer(2, 25, NULL);//��ʱ��2����ʱʱ��������֡��һ��	
		}
	}
	else
	{
		//��������ͷδ����
		AfxMessageBox(_T("����ͷδ������"), MB_OKCANCEL | MB_ICONEXCLAMATION);
	}

}

void CNCURoboteamDlg::Processing()
{
	/*opencv������ģ���˶�������   ��Ƶ���б�����ģ��Ȼ���ֵó��˶�����*/
	/******************************opencv1.x�汾�ı�����ַ�****************************************/
	//	back_frame.create(frame.rows, frame.cols, CV_8UC1);
	//fore_frame.create(frame.rows, frame.cols, CV_8UC1);
	//�ȸ�˹�˲�����ƽ��ͼ��
	//GaussianBlur(frame, frame, frame.size(), 0);
	//��ǰ֡�ͱ���ͼ���
	//absdiff(frame,back_frame,fore_frame );
	//��ֵ��ǰ��ͼ�����˶����壩
	//threshold(fore_frame, fore_frame, 60, 255.0, CV_THRESH_BINARY);
	//		CvMat frame_t = frame; CvMat back_frame_t = back_frame;
	//		cvRunningAvg(&frame_t,&back_frame_t,0.003, 0);
	//		Mat::Mat(&back_frame_t,false); /*����IplImage -> Mat����ѡ���Ƿ�������*/
	/******************************opencv1.x�汾�ı�����ַ�****************************************/
	//�˶�ǰ����⣬�����±���

	//	bgSubtractor(frame,back_frame,0.01);
	bgSubtractor2(frame, back_frame, 0.01);//�������ʣ�0.001ʱ�Ƚ���

	//������̬ѧ�˲���ȥ������
	erode(back_frame, fore_frame, Mat());
	dilate(fore_frame, fore_frame, Mat());
}


void CNCURoboteamDlg::OnBnClickedTrackinit()
{
	// TODO: Add your control notification handler code here
	initflag = true;
	init_number = 1;
}


void CNCURoboteamDlg::OnBnClickedTrackstart()
{
	// TODO: Add your control notification handler code here	
	if (gotBB == true)
	{
		trackedflag = true;
		initflag = false;

	}
	else
	{
		//����û�г�ʼ��׷��
		AfxMessageBox(_T("û�г�ʼ��׷�٣�"), MB_OKCANCEL | MB_ICONEXCLAMATION);
	}
}

void CNCURoboteamDlg::OnBnClickedDatapanel()
{
	// TODO: Add your control notification handler code here
	//���ó�Ա��������һ����ģ̬�Ի���������������ʾ�ͷ��Ϳ���
	if (pTD == NULL)
	{
		pTD = new CDataControlDialog(); //��ָ������ڴ�  
	}
	pTD->Create(IDD_DataControl, this); //����һ����ģ̬�Ի��� 	
	pTD->ShowWindow(SW_SHOW); //��ʾ��ģ̬�Ի��� 
}

//bounding box mouse callback 
//������Ӧ���ǵõ�Ŀ������ķ�Χ�������ѡ��bounding box��
void CNCURoboteamDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	GetDlgItem(IDC_VideoImage)->GetWindowRect(videorect);
	ScreenToClient(&videorect);
	if (point.x >= videorect.left && point.x <= videorect.right && point.y >= videorect.top && point.y <= videorect.bottom)
	{
		if (drawing_box)
		{
			CClientDC dc(this);
			CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
			CPen *penold = dc.SelectObject(&pen);
			CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
			CBrush *pbrold = dc.SelectObject(pbrush);
			if (m_d = true)
			{
				dc.SetROP2(R2_NOT);//����ģʽ
				dc.Rectangle(CRect(m_ptstart, m_ptold));
				dc.MoveTo(m_ptstart);
				dc.Rectangle(CRect(m_ptstart, point));
				dc.SelectObject(penold);
				dc.SelectObject(pbrold);
				m_ptold = point;
			}

			//AfxMessageBox(_T("MouseMove��"), MB_OKCANCEL | MB_ICONEXCLAMATION);
			x = point.x;
			y = point.y;
			box.width = x - box.x;
			box.height = y - box.y;
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CNCURoboteamDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	GetDlgItem(IDC_VideoImage)->GetWindowRect(videorect);
	ScreenToClient(&videorect);
	if (point.x >= videorect.left && point.x <= videorect.right && point.y >= videorect.top && point.y <= videorect.bottom)
	{
		//AfxMessageBox(_T("LButtonDown��"), MB_OKCANCEL | MB_ICONEXCLAMATION);
		m_ptstart = point;
		m_ptold = point;
		m_d = true;

		drawing_box = true;
		x = point.x;
		y = point.y;
		box = Rect(x, y, 0, 0);
	}
	CDialog::OnLButtonDown(nFlags, point);
}

void CNCURoboteamDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	GetDlgItem(IDC_VideoImage)->GetWindowRect(videorect);
	ScreenToClient(&videorect);
	if (point.x >= videorect.left && point.x <= videorect.right && point.y >= videorect.top && point.y <= videorect.bottom)
	{
		//AfxMessageBox(_T("LButtonUp��"), MB_OKCANCEL | MB_ICONEXCLAMATION);
		drawing_box = false;
		CClientDC dc(this);
		CPen pen(PS_SOLID, 2, RGB(255, 0, 0));
		CPen *penold = dc.SelectObject(&pen);
		CBrush *pbrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		CBrush *pbrold = dc.SelectObject(pbrush);
		if (m_d = true)
		{
			dc.SetROP2(R2_NOT);
			dc.Rectangle(CRect(m_ptstart, m_ptold));
			dc.SetROP2(R2_COPYPEN);
			dc.Rectangle(CRect(m_ptstart, point));
			dc.SelectObject(penold);
			dc.SelectObject(pbrold);
		}

		if (box.width < 0)
		{
			box.x += box.width;
			box.width *= -1;
		}
		if (box.height < 0)
		{
			box.y += box.height;
			box.height *= -1;
		}
		CString str;
		str.Format(_T("%d,%d,%d,%d"), box.x, box.y, box.height, box.width);
		MessageBox(str);
		gotBB = true;//�Ѿ����bounding box		
	}
	CDialog::OnLButtonUp(nFlags, point);
}


void CNCURoboteamDlg::OnBnClickedComkou()
{
	// TODO: Add your control notification handler code here
	CString str, str1, n;
	GetDlgItemText(IDC_COMKOU, str);
	CWnd *h1;
	h1 = GetDlgItem(IDC_COMKOU);		//ָ��ؼ���Caption
	if (!m_MSCom.get_PortOpen())
	{
		m_BodXuan.GetLBText(m_BodXuan.GetCurSel(), str1);//ȡ����ѡ���ַ���������str1����
		str1 = str1 + ',' + 'n' + ',' + '8' + ',' + '1';//����У��λ��������ǡ�n��8��1����ֱ���ڳ������

		m_MSCom.put_CommPort(m_ComXuan.GetCurSel() + 1);	//ѡ�񴮿�
		m_MSCom.put_InputMode(1);							//�������뷽ʽΪ�����Ʒ�ʽ
		m_MSCom.put_Settings(str1);						//������Ϊ����У��λ��8����λ��1��ֹͣλ��
		m_MSCom.put_InputLen(65536);						//���õ�ǰ���������ݳ���Ϊ65536
		m_MSCom.put_RThreshold(1);							//������һ���ַ������¼�
		m_MSCom.put_RTSEnable(1);							//����RT����

		m_MSCom.put_PortOpen(true);						//�򿪴���
		if (m_MSCom.get_PortOpen())
		{
			str = _T("�رշ��ʹ���");
			UpdateData(true);
			h1->SetWindowTextW(str);							//�ı䰴ť����Ϊ���رմ��ڡ�
		}
		else
		{
			m_MSCom.put_PortOpen(false);
			if (str != _T("�򿪷��ʹ���"))
			{
				Data_Send_Flag = false;
				str = _T("�򿪷��ʹ���");
				UpdateData(true);
				h1->SetWindowTextW(str);						//�ı䰴ť����Ϊ�򿪴���
			}
		}

	}
	else
	{
		m_MSCom.put_PortOpen(false);
		if (str != _T("�򿪷��ʹ���"))
		{
			Data_Send_Flag = false;
			str = _T("�򿪷��ʹ���");
			UpdateData(true);
			h1->SetWindowTextW(str);						//�ı䰴ť����Ϊ�򿪴���
		}
	}
}


void CNCURoboteamDlg::OnBnClickedComsend()
{
	// TODO: Add your control notification handler code here
	if (m_MSCom.get_PortOpen())
	{
		Data_Send_Flag = true;
	}
	else
	{
		//����û�г�ʼ��׷��
		AfxMessageBox(_T("û�д򿪷��ʹ��ڣ�"), MB_OKCANCEL | MB_ICONEXCLAMATION);
	}
}
BEGIN_EVENTSINK_MAP(CNCURoboteamDlg, CDialog)
	ON_EVENT(CNCURoboteamDlg, IDC_MSCOMM1, 1, CNCURoboteamDlg::OnCommMscomm1, VTS_NONE)
	ON_EVENT(CNCURoboteamDlg, IDC_MSCOMM2, 1, CNCURoboteamDlg::OnCommMscomm2, VTS_NONE)
END_EVENTSINK_MAP()


void CNCURoboteamDlg::OnCommMscomm1()
{
	// TODO: Add your message handler code here
	if (m_MSCom.get_CommEvent() == 2)
	{
		char str[65536] = { 0 };
		long k;
		VARIANT InputData = m_MSCom.get_Input();			//��������
		COleSafeArray fs;
		fs = InputData;											//VARIANT�ͱ���ת��ΪCOleSafeArray�ͱ���
		for (k = 0; k < fs.GetOneDimSize(); k++)
		{
			fs.GetElement(&k, str + k);							//ת��ΪBYTE������
		}
		m_facedata += str;											//���յ��༭����

		UpdateData(false);
	}
}


void CNCURoboteamDlg::OnCommMscomm2()
{
	// TODO: Add your message handler code here
	if (m_MSCom2.get_CommEvent() == 2)
	{
		char str[65536] = { 0 };
		long k;
		VARIANT InputData = m_MSCom2.get_Input();			//��������
		COleSafeArray fs;
		fs = InputData;											//VARIANT�ͱ���ת��ΪCOleSafeArray�ͱ���
		for (k = 0; k < fs.GetOneDimSize(); k++)
		{
			fs.GetElement(&k, str + k);							//ת��ΪBYTE������
		}
		m_facedata2 += str;											//���յ��༭����
		m_facedata2 += "\r\n";

		UpdateData(false);
	}
}


void CNCURoboteamDlg::OnBnClickedComkou2()
{
	// TODO: Add your control notification handler code here
	CString str, str1, n;
	GetDlgItemText(IDC_COMKOU2, str);
	CWnd *h1;
	h1 = GetDlgItem(IDC_COMKOU2);		//ָ��ؼ���Caption
	if (!m_MSCom2.get_PortOpen())
	{
		m_BodXuan2.GetLBText(m_BodXuan2.GetCurSel(), str1);//ȡ����ѡ���ַ���������str1����
		str1 = str1 + ',' + 'n' + ',' + '8' + ',' + '1';//����У��λ��������ǡ�n��8��1����ֱ���ڳ������

		m_MSCom2.put_CommPort(m_ComXuan2.GetCurSel() + 1);	//ѡ�񴮿�
		m_MSCom2.put_InputMode(1);							//�������뷽ʽΪ�����Ʒ�ʽ
		m_MSCom2.put_Settings(str1);						//������Ϊ����У��λ��8����λ��1��ֹͣλ��
		m_MSCom2.put_InputLen(65536);						//���õ�ǰ���������ݳ���Ϊ65536
		m_MSCom2.put_RThreshold(1);							//������һ���ַ������¼�
		m_MSCom2.put_RTSEnable(1);							//����RT����

		m_MSCom2.put_PortOpen(true);						//�򿪴���
		if (m_MSCom2.get_PortOpen())
		{
			str = _T("�رս��մ���");
			UpdateData(true);
			h1->SetWindowTextW(str);							//�ı䰴ť����Ϊ���رմ��ڡ�
		}
		else
		{
			m_MSCom2.put_PortOpen(false);
			if (str != _T("�򿪽��մ���"))
			{
				Data_Receive_Flag = false;
				str = _T("�򿪽��մ���");
				UpdateData(true);
				h1->SetWindowTextW(str);						//�ı䰴ť����Ϊ�򿪴���
			}
		}

	}
	else
	{
		m_MSCom2.put_PortOpen(false);
		if (str != _T("�򿪽��մ���"))
		{
			Data_Receive_Flag = false;
			str = _T("�򿪽��մ���");
			UpdateData(true);
			h1->SetWindowTextW(str);						//�ı䰴ť����Ϊ�򿪴���
		}
	}
}


void CNCURoboteamDlg::OnBnClickedComsend2()
{
	// TODO: Add your control notification handler code here
	if (m_MSCom2.get_PortOpen())
	{
		Data_Receive_Flag = true;
	}
	else
	{
		//����û�г�ʼ��׷��
		AfxMessageBox(_T("û�д򿪽��մ��ڣ�"), MB_OKCANCEL | MB_ICONEXCLAMATION);
	}
}

//////////////////////////////////////////////////////////////////////////
// function: addNewPoints
// brief: ����µ��Ƿ�Ӧ�ñ����
// parameter:
// return: �Ƿ���ӱ�־
//////////////////////////////////////////////////////////////////////////
bool CNCURoboteamDlg::addNewPoints()
{
	return points[0].size() <= 10;
}

//////////////////////////////////////////////////////////////////////////
// function: acceptTrackedPoint
// brief: ������Щ���ٵ㱻����
// parameter:
// return:
//////////////////////////////////////////////////////////////////////////
bool CNCURoboteamDlg::acceptTrackedPoint(int i)
{
	return Status[i] && ((abs(points[0][i].x - points[1][i].x) + abs(points[0][i].y - points[1][i].y)) > 2);
}
//////////////////////////////////////////////////////////////////////////
// function: tracking
// brief: ����
// parameter: frame	�������Ƶ֡
//			  output �и��ٽ������Ƶ֡
// return: void
//////////////////////////////////////////////////////////////////////////
void CNCURoboteamDlg::tracking(Mat &frame, Mat &output)
{
	cvtColor(frame, gray, CV_BGR2GRAY);
	frame.copyTo(output);
	// ���������
	if (addNewPoints())
	{
		goodFeaturesToTrack(gray, features, maxCount, qLevel, minDist);
		points[0].insert(points[0].end(), features.begin(), features.end());
		initial.insert(initial.end(), features.begin(), features.end());
	}

	if (gray_prev.empty())
	{
		gray.copyTo(gray_prev);
	}
	// l-k�������˶�����
	calcOpticalFlowPyrLK(gray_prev, gray, points[0], points[1], Status, err);
	// ȥ��һЩ���õ�������
	int k = 0;
	for (size_t i = 0; i<points[1].size(); i++)
	{
		if (acceptTrackedPoint(i))
		{
			initial[k] = initial[i];
			points[1][k++] = points[1][i];
		}
	}
	points[1].resize(k);
	initial.resize(k);
	// ��ʾ��������˶��켣
	for (size_t i = 0; i<points[1].size(); i++)
	{
		line(output, initial[i], points[1][i], Scalar(0, 0, 255));
		circle(output, points[1][i], 3, Scalar(255, 0, 0), -1);
	}

	// �ѵ�ǰ���ٽ����Ϊ��һ�˲ο�
	swap(points[1], points[0]);
	swap(gray_prev, gray);

}

void CNCURoboteamDlg::drawCornerOnImage(Mat& image, const Mat&binary)
{
	Mat_<uchar>::const_iterator it = binary.begin<uchar>();
	Mat_<uchar>::const_iterator itd = binary.end<uchar>();
	for (int i = 0; it != itd; it++, i++)
	{
		if (*it)
			circle(image, Point(i%image.cols, i / image.cols), 3, Scalar(0, 255, 0), 1);
	}
}

void CNCURoboteamDlg::OnEnChangeFace()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
