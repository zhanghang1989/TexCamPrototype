
// TexCamInterface.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTexCamInterfaceApp:
// See TexCamInterface.cpp for the implementation of this class
//

class CTexCamInterfaceApp : public CWinApp
{
public:
	CTexCamInterfaceApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTexCamInterfaceApp theApp;