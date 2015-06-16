// ExtWinampReport.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ExtWinampReport.h"
#include "MsgDummy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CExtWinampReportApp

BEGIN_MESSAGE_MAP(CExtWinampReportApp, CWinApp)
END_MESSAGE_MAP()

void config();
void quit();
int init();
void config_write();
void config_read();

#define PLUGIN_DESC "RoboMX Advanced Playbackreport Plugin"

winampGeneralPurposePlugin plugin =
{
	GPPHDR_VER,
	PLUGIN_DESC " v1.1",
	init,
	config,
	quit,
};

__declspec( dllexport ) winampGeneralPurposePlugin * winampGetGeneralPurposePlugin()
{
	return &plugin;
}


// The one and only CExtWinampReportApp object

CExtWinampReportApp theApp;

void config()
{
	
	MessageBox(plugin.hwndParent, "Nothing to configure\n" PLUGIN_DESC "\n(c) 2005 by Thees Schwab\nDistributed under the terms of the GNU General Public License\nhttp://mxcontrol.sf.net", "About " PLUGIN_DESC, MB_ICONINFORMATION);
}

int init()
{

	return 0;
}

void quit()
{
}

// CExtWinampReportApp construction

CExtWinampReportApp::CExtWinampReportApp()
{
	
	m_pDummy = NULL;
}




// CExtWinampReportApp initialization

BOOL CExtWinampReportApp::InitInstance()
{
	CWinApp::InitInstance();

	m_pDummy = new CMsgDummy(NULL);

	m_pDummy->Create(IDD_MSGDUMMY, 0);
	m_pDummy->ShowWindow(SW_HIDE);

	return TRUE;
}

int CExtWinampReportApp::ExitInstance()
{

/*	m_bRun = FALSE;
	DWORD n = WaitForSingleObject(m_eStop.m_hObject, 1000);
	if(n == WAIT_TIMEOUT || n == WAIT_FAILED){

		TerminateThread(m_pThread->m_hThread, 0);
	}
	m_pThread = NULL;	
	*/

	while(m_pDummy->m_bIsUpdating) Sleep(10);
	delete m_pDummy;
	m_pDummy = 0;
	return CWinApp::ExitInstance();
}
/*
UINT CExtWinampReportApp::WorkerThread(LPVOID pParam)
{

	CExtWinampReportApp* pApp = (CExtWinampReportApp*)pParam;

	ASSERT(pParam);
	extendedFileInfoStruct fileInfo;
	fileInfo.ret      = new char[1024];
	fileInfo.retlen   = 1024;
	ZeroMemory(fileInfo.ret, fileInfo.retlen);

	CString strCurrent, strPrev, strAlbum, strArtist, strSong, strYear, strGenre, strTrack, strComment;
	int nCurrent = -1, nLength = 0;

	while(pApp->m_bRun){

		if(!pApp->m_bWinampRun){

			Sleep(100);
			continue;
		}
		nLength = (int)::SendMessage(plugin.hwndParent, WM_WA_IPC, 0, IPC_GETLISTLENGTH);
		if(nLength <= 0) continue;

		::SendMessage(plugin.hwndParent,WM_WA_IPC,0,IPC_REFRESHPLCACHE);
		::SendMessage(plugin.hwndParent, WM_WA_IPC, 0, IPC_UPDTITLE);
	
		nCurrent = (int)::SendMessage(plugin.hwndParent, WM_WA_IPC, 0, IPC_GETLISTPOS);
		if(nCurrent >= 0){

			fileInfo.filename = (LPSTR)::SendMessage(plugin.hwndParent, WM_WA_IPC, nCurrent, IPC_GETPLAYLISTFILE);
			strCurrent = fileInfo.filename;
		}
		else continue;

		fileInfo.metadata = "Album";
		if(SendMessage(plugin.hwndParent, WM_WA_IPC, (WPARAM)&fileInfo, IPC_GET_EXTENDED_FILE_INFO)){

			strAlbum = fileInfo.ret;
		}
		else{ 

			strAlbum	= "Unknown Album"; 
		}
;

		fileInfo.metadata = "Artist";
		if(SendMessage(plugin.hwndParent, WM_WA_IPC, (WPARAM)&fileInfo, IPC_GET_EXTENDED_FILE_INFO)){

			strArtist = fileInfo.ret;
		}
		else{
			
			strArtist.Empty();
		}
		fileInfo.metadata = "Title";
		if(SendMessage(plugin.hwndParent, WM_WA_IPC, (WPARAM)&fileInfo, IPC_GET_EXTENDED_FILE_INFO)){

			strSong = fileInfo.ret;
		}
		else{

			strSong.Empty();
		}

		fileInfo.metadata = "Year";
		if(SendMessage(plugin.hwndParent, WM_WA_IPC, (WPARAM)&fileInfo, IPC_GET_EXTENDED_FILE_INFO)){

			strYear = fileInfo.ret;
		}
		else{
			
			strYear		= "Unknown Year"; 
		}

		fileInfo.metadata = "Genre";
		if(SendMessage(plugin.hwndParent, WM_WA_IPC, (WPARAM)&fileInfo, IPC_GET_EXTENDED_FILE_INFO)){

			strGenre = fileInfo.ret;
		}
		else{

			strGenre	= "Other"; 
		}

		fileInfo.metadata = "Track";
		if(SendMessage(plugin.hwndParent, WM_WA_IPC, (WPARAM)&fileInfo, IPC_GET_EXTENDED_FILE_INFO)){

			strTrack = fileInfo.ret;
		}
		else{

			strTrack	= "0"; 
		}

		fileInfo.metadata = "Comment";
		if(SendMessage(plugin.hwndParent, WM_WA_IPC, (WPARAM)&fileInfo, IPC_GET_EXTENDED_FILE_INFO)){

			strComment = fileInfo.ret;
		}
		else{

			strComment	= "";
		}

		if(strSong.IsEmpty() || strArtist.IsEmpty()){

			::SendMessage(plugin.hwndParent, WM_WA_IPC,0,IPC_UPDTITLE);
			TCHAR *buff = new TCHAR[250];
			::GetWindowText(plugin.hwndParent, buff, 250);
			CString strTmp = buff;
			strTmp = strTmp.Mid(strTmp.Find(" ", 0)+1, strTmp.Find(" - Winamp") - strTmp.Find(" ", 0)-1);
            int nIndex = strTmp.Find("-", 0);
			if(nIndex > 0){

				if(strArtist.IsEmpty()){

					strArtist	= strTmp.Left(nIndex);
					strArtist.TrimRight();
					strArtist.TrimLeft();
				}
				if(strSong.IsEmpty()){

					strSong		= strTmp.Mid(nIndex+1);
					strSong.TrimLeft();
					strSong.TrimRight();
				}
			}
			delete buff;
			buff = 0;
		}

		strPrev	= strCurrent;
		pApp->SetSongInfo(strCurrent, strArtist, strSong, strAlbum, strYear, strGenre, strTrack, strComment);
		Sleep(100);
	}

	delete fileInfo.ret;
	fileInfo.ret = NULL;

	pApp->m_eStop.SetEvent();
	return 0;
}

void CExtWinampReportApp::StartThread(void)
{
	m_bRun = TRUE;
	m_eStop.ResetEvent();
	m_pThread = AfxBeginThread(WorkerThread, (LPVOID)this, THREAD_PRIORITY_NORMAL);
}
*/
