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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//������DataControlDialog�ĳ�ʼ������
	CString str;
	int com_i, com_j;
	for (com_i = 0; com_i < 15; com_i++)
	{
		str.Format(_T("COM %d"), com_i + 1);
		m_ComChoice.InsertString(com_i, str);
	}
	m_ComChoice.SetCurSel(0);//Ԥ��COM��
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
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
// CDataControlDialog message handlers


void CDataControlDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	if (m_ComControl.get_PortOpen())
	{
		m_ComControl.put_PortOpen(false);						//��������Ǵ򿪵ģ���ر�
	}
	CDialogEx::OnOK();
	DestroyWindow();//��ģ̬��ֻ�����أ�û�����٣���Ҫ��д�����OnOK�麯�������ҵ���DestroyWindow����
}


void CDataControlDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	if (m_ComControl.get_PortOpen())
	{
		m_ComControl.put_PortOpen(false);						//��������Ǵ򿪵ģ���ر�
	}
	CDialogEx::OnCancel();
	DestroyWindow();//ȡ��ʱ��ҲҪ��дOnCancel��Ȼ������д������DestroyWindow���������Ҳ����ٵ��û����OnOK ��OnCancel
}


void CDataControlDialog::OnBnClickedGetdata()
{
	// TODO: Add your control notification handler code here	
	SetTimer(3, 25, NULL);//��ʱ��1����ʱʱ��������֡��һ��	
	//UpdateData(false);		
	//this->SetDlgItemText(IDC_Data, m_Data);//���ַ���str��������ʾ��IDΪIDC_EDIT_RECV�ı༭��
	//m_Data_Control.LineScroll(m_Data_Control.GetLineCount());//m_recvshwΪ�ñ༭��ĳ�Ա������Ҳ����control���͵ı�����//ʹ�ÿؼ�������һЩ��Ա�������ù�������λ��//��������
}


void CDataControlDialog::OnBnClickedSenddata()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);								//���¿ؼ�����
	m_ComControl.put_Output(COleVariant(m_Data));	//�ѷ��ͱ༭������ݷ��ͳ�ȥ
}


void CDataControlDialog::OnBnClickedClosecom()
{
	// TODO: Add your control notification handler code here
	CString str, str1, n;
	GetDlgItemText(IDC_CloseCOM, str);
	CWnd *h1;
	h1 = GetDlgItem(IDC_CloseCOM);		//ָ��ؼ���Caption
	if (!m_ComControl.get_PortOpen())
	{
		m_Bod.GetLBText(m_Bod.GetCurSel(), str1);//ȡ����ѡ���ַ���������str1����
		str1 = str1 + ',' + 'n' + ',' + '8' + ',' + '1';//����У��λ��������ǡ�n��8��1����ֱ���ڳ������

		m_ComControl.put_CommPort(m_ComChoice.GetCurSel() + 1);	//ѡ�񴮿�
		m_ComControl.put_InputMode(1);							//�������뷽ʽΪ�����Ʒ�ʽ
		m_ComControl.put_Settings(str1);						//������Ϊ����У��λ��8����λ��1��ֹͣλ��
		m_ComControl.put_InputLen(65536);						//���õ�ǰ���������ݳ���Ϊ65536
		m_ComControl.put_RThreshold(1);							//������һ���ַ������¼�
		m_ComControl.put_RTSEnable(1);							//����RT����

		m_ComControl.put_PortOpen(true);						//�򿪴���
		if (m_ComControl.get_PortOpen())
		{
			str = _T("�رմ���");
			UpdateData(true);
			h1->SetWindowTextW(str);							//�ı䰴ť����Ϊ���رմ��ڡ�
		}
		else
		{
			m_ComControl.put_PortOpen(false);
			if (str != _T("�򿪴���"))
			{
				str = _T("�򿪴���");
				UpdateData(true);
				h1->SetWindowTextW(str);						//�ı䰴ť����Ϊ�򿪴���
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
	m_Data = _T("");											//���༭���Ϳո��
	KillTimer(3);
	UpdateData(false);											//��������
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
		VARIANT InputData = m_ComControl.get_Input();			//��������
		COleSafeArray fs;
		fs = InputData;											//VARIANT�ͱ���ת��ΪCOleSafeArray�ͱ���
		for (k = 0; k < fs.GetOneDimSize(); k++)
		{
			fs.GetElement(&k, str + k);							//ת��ΪBYTE������
		}
		m_Data += str;											//���յ��༭����

		UpdateData(false);
	}
}

void CDataControlDialog::imageToDC(Mat &Frame, UINT ID_Picture)
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

