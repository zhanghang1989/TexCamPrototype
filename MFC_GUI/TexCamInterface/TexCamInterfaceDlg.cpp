
// TexCamInterfaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TexCamInterface.h"
#include "TexCamInterfaceDlg.h"
#include "afxdialogex.h"

#include <string>
#include <Windows.h>
#include <shellapi.h>
#include <fstream>
#include <vcclr.h>
#include <time.h>
#include <stdlib.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace System;
using namespace System::IO;
using namespace System::Windows::Forms;
using namespace System::Drawing;
using namespace System::Drawing::Imaging;
using namespace System::Runtime::InteropServices;
using namespace System::Diagnostics;
using namespace std;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTexCamInterfaceDlg dialog



CTexCamInterfaceDlg::CTexCamInterfaceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTexCamInterfaceDlg::IDD, pParent)
	, step(0)
	, material(_T(""))
	, shutter(0)
	, PointIndex(0)
	, instance(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTexCamInterfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, material);
	DDX_Text(pDX, IDC_EDIT1, step);
	DDX_Text(pDX, IDC_EDIT3, shutter);
	DDX_Text(pDX, IDC_EDIT4, PointIndex);
	DDX_Text(pDX, IDC_EDIT5, instance);
}

BEGIN_MESSAGE_MAP(CTexCamInterfaceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CTexCamInterfaceDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTexCamInterfaceDlg message handlers

BOOL CTexCamInterfaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTexCamInterfaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTexCamInterfaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTexCamInterfaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTexCamInterfaceDlg::OnBnClickedButton1()
{
	UpdateData(true);
	String ^path = Application::StartupPath + "\\Camera1394.exe";
	//String ^para = System::Runtime::InteropServices::Marshal::PtrToStringAnsi((IntPtr) (LPSTR) (LPCTSTR)material) + " " + step;
	String ^para = gcnew String(material) + " " + instance + " " + step + " " + shutter + " " + PointIndex;
	Process ^process = gcnew Process();
	ProcessStartInfo ^ startInfo = gcnew ProcessStartInfo(path, para);
	process->StartInfo = startInfo;
	process->Start();
	
	// TODO: Add your control notification handler code here
}
