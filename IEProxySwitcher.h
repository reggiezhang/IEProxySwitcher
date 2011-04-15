// IEProxySwitcher.h : main header file for the PROJECT_NAME application
// $Id: IEProxySwitcher.h 230 2007-12-10 16:15:22Z ezhenzh $

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CIEProxySwitcherApp:
// See IEProxySwitcher.cpp for the implementation of this class
//

class CIEProxySwitcherApp : public CWinApp
{
public:
	CIEProxySwitcherApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CIEProxySwitcherApp theApp;