#pragma once
#include "afxwin.h"
#include "mscomm1.h"
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

class CDataControlDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDataControlDialog)

public:
	CDataControlDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDataControlDialog();
	void imageToDC(Mat &Frame, UINT ID_Picture);
	// Dialog Data
	enum { IDD = IDD_DataControl };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedGetdata();
	afx_msg void OnBnClickedSenddata();
	afx_msg void OnBnClickedClosecom();
	afx_msg void OnBnClickedClearimage();
	afx_msg void OnBnClickedCleardata();
	afx_msg void OnBnClickedLoadimage();
	CString m_Data;
	CEdit m_Data_Control;
private:
	CComboBox m_ComChoice;
	CComboBox m_Bod;
public:
	CMscomm1 m_ComControl;
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm1();
	
};
