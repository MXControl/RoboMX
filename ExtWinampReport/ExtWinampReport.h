// ExtWinampReport.h : main header file for the ExtWinampReport DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "MsgDummy.h"
#include "wa_ipc.h"

// CExtWinampReportApp
// See ExtWinampReport.cpp for the implementation of this class
//
typedef struct {
	int version;
	char *description;
	int (*init)();
	void (*config)();
	void (*quit)();
	HWND hwndParent;
	HINSTANCE hDllInstance;
} winampGeneralPurposePlugin;

#define GPPHDR_VER 0x10

//extern winampGeneralPurposePlugin *gen_plugins[256];
//typedef winampGeneralPurposePlugin * (*winampGeneralPurposePluginGetter)();


class CExtWinampReportApp : public CWinApp
{
public:
	CExtWinampReportApp();

public:
	//static UINT WorkerThread(LPVOID pParam);
	//CWinThread* m_pThread;
	//CEvent      m_eStop;
	//BOOL		m_bRun;
	//BOOL		m_bWinampRun;
	//void StartThread(void);

	CMsgDummy *m_pDummy;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance(); 
	DECLARE_MESSAGE_MAP()
	
};
