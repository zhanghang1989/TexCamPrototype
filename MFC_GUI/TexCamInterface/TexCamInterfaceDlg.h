
// TexCamInterfaceDlg.h : header file
//

#pragma once
#include <string>
using std::string;

// CTexCamInterfaceDlg dialog
class CTexCamInterfaceDlg : public CDialogEx
{
// Construction
public:
	CTexCamInterfaceDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TEXCAMINTERFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString material;
	int step;
	int shutter;
	int PointIndex;
	int instance;
};
