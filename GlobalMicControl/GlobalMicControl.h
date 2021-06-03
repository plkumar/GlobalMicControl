
// GlobalMicControl.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CGlobalMicControlApp:
// See GlobalMicControl.cpp for the implementation of this class
//

class CGlobalMicControlApp : public CWinApp
{
public:
	CGlobalMicControlApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CGlobalMicControlApp theApp;
