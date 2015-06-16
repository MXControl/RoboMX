/*
** Copyright (C) 2004 Bender
**  
** RoboMX is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** RoboMX is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/


#include "stdafx.h"
#include "RoboMX.h"
#include "MainFrm.h"
#include "ChannelDoc.h"
#include "ChildFrm.h"
#include "ChannelView.h"
#include "ChatDoc.h"
#include "ChatView.h"
#include "Settings.h"
#include <mmsystem.h>
#include "Tokenizer.h"
#include "SystemInfo.h"
#include "ini.h"
#include "rmx_qapi.h"
#include "RoboEx.h"
#include "EmoticonManager.h"
#include "BrowseDoc.h"
#include "MessageDoc.h"
#include "MessageView.h"
#include "RenameDlg.h"
#include ".\mainfrm.h"
#include "IgnoredUsers.h"
#include "TrustedUsers.h"
#include "ManageAutojoins.h"
#include "ExitConfirmation.h"
#include "InputRequest.h"

#ifdef _ROBO_READER
#include "VoiceDlg.h"
CVoiceDlg m_dlgVoice;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//CPtrArray g_aPlugins;
CArray<PLUGIN_DATA, PLUGIN_DATA> g_aPlugins;

// Plugin defines
#define WM_ECHOTEXT			WM_APP+1
#define WM_ECHOSYSTEXT		WM_APP+2
#define WM_TEXT_INPUT			WM_APP+3
#define WM_DPLTOOLTIP		WM_APP+4
#define WM_PMSG				WM_APP+5
#define WM_CHANNELTIP		WM_APP+7

#define WM_INCOMMING		WM_APP+10
#define WM_FINISH_START		WM_APP+20	 // ;-)

extern UINT UWM_TEXT_INPUT;
extern UINT UWM_RENAMECL;
extern CSystemInfo  g_sSystem;
extern CSettings    g_sSettings;

CEmoticonManager    g_eEmoticons;


/////////////////////////////////////////////////////////////////////////////
// CMainFrame
#define WM_TRAY_ICON_NOTIFY_MESSAGE (WM_USER + 1)
#define TIMER_ANIMATE   101010
#define TIMER_PING	    111010
#define TIMER_PMANI     111111
#define TIMER_IDLECHECK 111112

extern UINT UWM_SETBACK;
extern UINT UWM_SETAWAY;
extern UINT UWM_WPNNOTIFY;
extern UINT UWM_LOAD_SETTINGS;
UINT UWM_WHOIS		 = ::RegisterWindowMessage("UWM_WHOIS-{494D99C1-03BE-49e3-8A47-D0D17C6D4ACE}");
UINT UWM_BROWSE		 = ::RegisterWindowMessage("UWM_BROWSE-{494D99C1-03BE-49e3-8A47-D0D17C6D4ACE}");
UINT UWM_REBROWSE	 = ::RegisterWindowMessage("UWM_REBROWSE-{494D99C1-03BE-49e3-8A47-D0D17C6D4ACE}");
UINT UWM_NEWPM		 = ::RegisterWindowMessage("UWM_NEWPM-{494D99C1-03BE-49e3-8A47-D0D17C6D4ACE}");
UINT UWM_ADDLISTENER = ::RegisterWindowMessage("UWM_ADDLISTENER-{494D99C1-03BE-49e3-8A47-D0D17C6D4ACE}");
UINT UWM_REMLISTENER = ::RegisterWindowMessage("UWM_REMLISTENER-{494D99C1-03BE-49e3-8A47-D0D17C6D4ACE}");

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_OPTIONS, OnViewOptions)
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_CHANNEL_CHANNELLIST, OnChannelChannellist)
	ON_UPDATE_COMMAND_UI(ID_CHANNEL_CHANNELLIST, OnUpdateChannelChannellist)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_VIEW_FULL_SCREEN, OnViewFullScreen)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FULL_SCREEN, OnUpdateViewFullScreen)
	ON_COMMAND(AFX_IDS_SCNEXTWINDOW, OnIdsScnextwindow)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CHANNELBAR, OnUpdateViewChannelBar)
	ON_COMMAND(ID_VIEW_CHANNELBAR, OnViewChannelbar)
	ON_COMMAND(ID_MYFILE_NEW, OnFileNew)
	ON_MESSAGE(WM_TRAY_ICON_NOTIFY_MESSAGE,OnTrayNotify)
	ON_MESSAGE(WM_CHANNELTIP, OnChannelTooltip)
	ON_MESSAGE(WM_QAPI, OnPluginQueryInfo)
	ON_MESSAGE(WM_TEXT_INPUT, OnPluginInput)
	ON_MESSAGE(WM_ECHOTEXT, OnPluginEcho)
	ON_MESSAGE(WM_DPLTOOLTIP, OnTooltip)
	ON_MESSAGE(WM_ECHOSYSTEXT, OnPluginSysEcho)	
	ON_MESSAGE(WM_PMSG, OnPluginSendMsg)
	ON_COMMAND(ID_SYSTRAY_RESTORE, OnSystrayRestore)
	ON_COMMAND(ID_RECONNECT_ALL, OnReconnectAll)
	ON_UPDATE_COMMAND_UI(ID_WPN_REFRESH, OnUpdateWPNRefresh)
	ON_UPDATE_COMMAND_UI(ID_WPN_STATUS, OnUpdateWPNConnect)
	ON_COMMAND(ID_WPN_STATUS, OnWPNConnect)
	ON_COMMAND(ID_WPN_REFRESH, OnWPNRefresh)
	ON_REGISTERED_MESSAGE(UWM_WPNNOTIFY, OnWPNNotify)
	ON_MESSAGE(WM_FINISH_START, OnFinishStart)
	ON_REGISTERED_MESSAGE(UWM_WHOIS, OnWhois)
	ON_REGISTERED_MESSAGE(UWM_BROWSE, OnBrowse)
	ON_REGISTERED_MESSAGE(UWM_REBROWSE, OnReBrowse)
	ON_REGISTERED_MESSAGE(UWM_ADDLISTENER, OnAddListener)
	ON_REGISTERED_MESSAGE(UWM_REMLISTENER, OnRemoveListener)
	ON_REGISTERED_MESSAGE(UWM_NEWPM, OnNewPM)
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
	ON_COMMAND(ID_MUTE, OnMute)
	ON_UPDATE_COMMAND_UI(ID_MUTE, OnUpdateMute)
	ON_REGISTERED_MESSAGE(UWM_SETAWAY, OnSetAway)
	ON_REGISTERED_MESSAGE(UWM_SETBACK, OnSetBack)
	ON_COMMAND(ID_RENAME, OnRename)
	ON_COMMAND(ID_VIEW_IGNOREDUSERS, OnViewIgnoredusers)
	ON_COMMAND(ID_VIEW_TRUSTEDUSERS, OnViewTrustedusers)
	ON_COMMAND(ID_VIEW_AUTOJOINMANAGER, OnViewAutojoinmanager)
	ON_COMMAND(ID_HELP_VISITHOMEPAGE, OnHelpVisithomepage)
	ON_COMMAND(ID_CTRLTAB, OnCtrltab)
	ON_COMMAND(ID_CTRLSHIFTTAB, OnCtrlshifttab)
	ON_COMMAND(ID_HELP_VISITONLINEFORUM, OnHelpVisitonlineforum)
	ON_COMMAND(ID_HELP_SUBSCRIBETOMAILINGLIST, OnHelpSubscribetomailinglist)
	ON_COMMAND(ID_HELP_LINCENSE, OnHelpLincense)
	ON_COMMAND(ID_HELP_CHANGELOG, OnHelpChangelog)
	ON_REGISTERED_MESSAGE(UWM_LOAD_SETTINGS, OnLoadSettings)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,           // status line indicator
	ID_SEPARATOR,			// status line indicator
	ID_SEPARATOR,			// status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL
};


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{

	m_bIdleAway		  = FALSE;
	m_bChannelList    = FALSE;
	m_dwChannelList   = 0;
	m_bFullScreenMode = FALSE;
	m_bSettings       = FALSE;
	m_bAway			  = FALSE;
	m_bQuickJoin	  = FALSE;
	m_hIcon			  = NULL;
	m_hIcon2		  = NULL;
	m_hPM1			  = NULL;
	m_hPM2			  = NULL;
	m_bMinimized	  = FALSE;
	m_nIcon			  = 0;
	m_nMaxAni		  = 0;
	m_wClientID		  = 0;
	m_bWPN			  = FALSE;
	m_eStartupDone.ResetEvent();
	m_eWPNConnected.ResetEvent();
	m_pThread		  = NULL;
	m_bDisconnected	  = FALSE;
	m_frameManager.SetWindow(this);
	m_frameManager.SetProfileHeading("Main Frame");
	m_bUpdate		 = FALSE;
	m_nPMAni         = 0;
	m_nUnreadPMs     = 0;
	m_dwLastActivity = 0;
	m_bMuted         = FALSE;
	m_hConnect		 = 0;
	m_hDisconnect    = 0;
}


CMainFrame::~CMainFrame()
{

	UnloadPlugins();
	g_eEmoticons.Free();

	if(m_hIcon)
		DeleteObject(m_hIcon);
	if(m_hIcon2)
		DeleteObject(m_hIcon2);
	if(m_hConnect)
		DeleteObject(m_hConnect);
	if(m_hDisconnect)
		DeleteObject(m_hDisconnect);
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_fFont.CreateFont(15, 6, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_STRING_PRECIS, 
		CLIP_STROKE_PRECIS, PROOF_QUALITY, FF_DONTCARE, g_sSettings.GetFont());

	m_dwLastActivity = GetTickCount();

	m_hIcon  = (HICON)LoadImage(GetApp()->m_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR );
	m_hIcon2 = (HICON)LoadImage(GetApp()->m_hInstance, MAKEINTRESOURCE(IDI_ANI), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR );
	
	m_hPM1  = (HICON)LoadImage(GetApp()->m_hInstance, MAKEINTRESOURCE(IDI_PM_SYSTRAY), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR );
	m_hPM2  = (HICON)LoadImage(GetApp()->m_hInstance, MAKEINTRESOURCE(IDI_PM2_SYSTRAY), IMAGE_ICON,16, 16, LR_DEFAULTCOLOR );

	m_hConnect		 = (HICON)LoadImage(GetApp()->m_hInstance, MAKEINTRESOURCE(IDI_CONNECTED), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR );;
	m_hDisconnect    = (HICON)LoadImage(GetApp()->m_hInstance, MAKEINTRESOURCE(IDI_DISCONNECTED), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR );;

	m_cmSystray.LoadMenu(IDR_SYSTRAY);
	LoadToTray(
		m_hWnd,
		WM_TRAY_ICON_NOTIFY_MESSAGE, 
		GetApp()->m_strAppTitle, 
		"",
		GetApp()->m_strAppTitle, 
		1, 
		m_hIcon,
		NIIF_INFO
	); 

	m_strTitle = GetApp()->m_strAppTitle;

	if (!m_wndToolBarStd.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBarStd.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// Check wether graphics adapter supports true color :-P
	if((::GetDeviceCaps(GetDC()->m_hDC,BITSPIXEL) > 8)) 
	{
	
		CImageList	imageList;
		CBitmap		bitmap;

		// Set up toolbar image lists.
		// Create and set the normal toolbar image list.
		bitmap.LoadMappedBitmap(IDB_TOOLBAR256);
		imageList.Create(16, 16, ILC_COLORDDB|ILC_MASK, 7, 1);
		imageList.Add(&bitmap, RGB(255,0,255));
		m_wndToolBarStd.SendMessage(TB_SETIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
		imageList.Detach();
		bitmap.Detach();

		// Create and set the disabled toolbar image list.
		bitmap.LoadMappedBitmap(IDB_TOOLBAR256_DEACT);
		imageList.Create(16, 16, ILC_COLORDDB|ILC_MASK, 7, 1);
		imageList.Add(&bitmap, RGB(255,0,255));
		m_wndToolBarStd.SendMessage( TB_SETDISABLEDIMAGELIST, 0, (LPARAM)imageList.m_hImageList);
		imageList.Detach();
		bitmap.Detach();
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	m_wndToolBarStd.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBarStd);

	m_wndDocSelector.Create(NULL, NULL, WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS | CBRS_BOTTOM | CBRS_TOP,
		         CRect(0,0,0,0), this, AFX_IDW_STATUS_BAR);

	m_wndDocSelector.SetBarStyle(CBRS_ALIGN_BOTTOM|CBRS_ALIGN_TOP);
	
	if(!g_sSettings.GetBarTop()){ // should bar dock on top or at bottom of the window

		m_wndDocSelector.EnableDocking(CBRS_ALIGN_BOTTOM|CBRS_ALIGN_TOP);
		DockControlBar(&m_wndDocSelector, AFX_IDW_DOCKBAR_BOTTOM);
	}

	m_wndStatusBar.SetPaneInfo(0,ID_SEPARATOR,SBPS_POPOUT,100);
	m_wndStatusBar.SetPaneInfo(1,ID_SEPARATOR,SBPS_STRETCH,120);
    m_wndStatusBar.SetPaneInfo(2,ID_SEPARATOR,SBPS_NORMAL,240);
	m_wndStatusBar.SetPaneInfo(3,ID_SEPARATOR,SBPS_NORMAL,200);
	m_wndStatusBar.SetPaneInfo(4,ID_SEPARATOR,SBPS_NORMAL,100);

	m_wndStatusBar.GetStatusBarCtrl().SetText("", 0, SBT_OWNERDRAW); 
	m_wndStatusBar.GetStatusBarCtrl().SetText("", 2, SBT_OWNERDRAW); 

	RecalcLayout();

	SetWindowText(GetApp()->m_pszAppName);

	m_bDisconnected = !g_sSettings.GetAutoWPN();

	m_pThread = AfxBeginThread(StartupThread, (LPVOID)this, THREAD_PRIORITY_NORMAL);

	if(g_sSettings.GetSoundFX()){

		if(!g_sSettings.GetSfxStart().IsEmpty())
			PlaySound(g_sSettings.GetSfxStart(), 0, SND_FILENAME|SND_ASYNC|SND_NODEFAULT );
	}

	m_bMuted = g_sSettings.GetSfxChatSfx();
	g_eEmoticons.Load();

#ifdef _ROBO_READER
	m_dlgVoice.Create(IDD_VOICE, this);
	m_dlgVoice.ShowWindow(SW_SHOWNORMAL);
	CString strWelcome, strName = g_sSettings.GetNickname();
	CVoiceDlg::FixName(strName);
	strWelcome.Format("Hello there, %s", strName);
	m_dlgVoice.SpeakOut(strWelcome);
#endif
	SetTimer(TIMER_IDLECHECK, 1000, 0);

	return 0;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	cs.style |= FWS_PREFIXTITLE;

	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	CRect rect;
	GetDesktopWindow()->GetWindowRect(rect);
	if(rect.Width() >= 1024){

		cs.cx = 800;
		cs.cy = 700;
	}


	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::StartPMAni(void)
{

	if(m_nUnreadPMs <= 0){

		m_nUnreadPMs = 0;
		ZeroMemory( &m_nPMIcon, sizeof( NOTIFYICONDATA ) );

		m_nPMIcon.cbSize		      = sizeof( NOTIFYICONDATA );
		m_nPMIcon.hWnd			      = m_hWnd;
		m_nPMIcon.uID			      = 120133;
		
		// Flag Description:
		// - NIF_ICON	 The hIcon member is valid.  
		// - NIF_MESSAGE The uCallbackMessage member is valid. 
		// - NIF_TIP	 The szTip member is valid. 
		// - NIF_STATE	 The dwState and dwStateMask members are valid. 
		// - NIF_INFO	 Use a balloon ToolTip instead of a standard ToolTip. The szInfo, uTimeout, szInfoTitle, and dwInfoFlags members are valid. 
		// - NIF_GUID	 Reserved. 
		m_nPMIcon.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

		m_nPMIcon.dwInfoFlags      = NIIF_INFO; // add an icon to a balloon ToolTip

		m_nPMIcon.uCallbackMessage = WM_TRAY_ICON_NOTIFY_MESSAGE;  
		m_nPMIcon.uTimeout         = 5000;
		m_nPMIcon.hIcon		   	   = m_hPM1;
		strcpy( m_nPMIcon.szInfoTitle, Util::LoadString(IDS_NEW_PM_TOOL));
		strcpy( m_nPMIcon.szInfo,      Util::LoadString(IDS_NEW_PM_TEXT));
		strcpy( m_nPMIcon.szTip,      Util::LoadString(IDS_NEW_PM_TITLE));

		Shell_NotifyIcon( NIM_ADD, &m_nPMIcon); // add to the taskbar's status area
		SetTimer(TIMER_PMANI, 1000, 0);
	}
	m_nUnreadPMs++;
}

void CMainFrame::StopPMAni(void)
{

	m_nUnreadPMs = 0;
	if((m_nUnreadPMs <= 0) && m_nPMIcon.hWnd && m_nPMIcon.uID>0)
	{
		Shell_NotifyIcon(NIM_DELETE,&m_nPMIcon);
		m_nPMIcon.uID   = 0;
		m_nPMIcon.hIcon = 0;
	}
	KillTimer(TIMER_PMANI);
}

void CMainFrame::StopAni(void)
{

	if(m_nIcon < 2){

		KillTimer(TIMER_ANIMATE);
		m_nIcon = 2;
		ModifyTrayIcon(m_hIcon);
	}
}

void CMainFrame::StartAni(void)
{
	if(m_nIcon == 2){

		m_nIcon = 0;
		SetTimer(TIMER_ANIMATE, 500, 0);
	}
}

void CMainFrame::OnTimer(UINT nIDEvent)
{

	if(nIDEvent == TIMER_ANIMATE){

		m_nMaxAni++;
		if(m_nIcon == 0){

			m_nIcon = 1;
			ModifyTrayIcon(m_hIcon);
		}
		else{

            m_nIcon = 0;
			ModifyTrayIcon(m_hIcon2);
		}
		if(!m_bWPN && !m_bDisconnected){

			m_wndStatusBar.SetWPNStatus(2);
			//m_wndToolBarStd.Invalidate();

		}
		if(m_nMaxAni >= 60) StopAni();
	}
	else if(nIDEvent == TIMER_PING){

		// Send queue status to node every 60 seconds...
		// as keep alive signal (winmx does that too :P)
		// Pretty stupid and bandwith-wasting for the 
		// Poor primary users...
		m_mxClient.SendQueStatus(1, 1, 0);
		TRACE("WPNP Ping\n");
		
		m_nIconData.uFlags = NIF_ICON;
		if(Shell_NotifyIcon(NIM_MODIFY, &m_nIconData) == NULL){

			LoadToTray(
				m_hWnd,
				WM_TRAY_ICON_NOTIFY_MESSAGE, 
				GetApp()->m_strAppTitle, 
				"",
				GetApp()->m_strAppTitle, 
				1, 
				m_hIcon,
				NIIF_INFO
			); 
		}
	}
	else if(nIDEvent == TIMER_PMANI){

		m_nPMIcon.uFlags	= NIF_ICON;
		m_nPMIcon.hIcon	= m_nPMAni  ? m_hPM1 : m_hPM2;
		m_nPMAni = !m_nPMAni;

		Shell_NotifyIcon(NIM_MODIFY, &m_nPMIcon);
	}
	else if(nIDEvent == TIMER_IDLECHECK){

		if(g_sSettings.GetAwayIfIdle() && (GetTickCount() - m_dwLastActivity > g_sSettings.GetIdleDuration())){

			OnSetAway(0, 1);
		}
	}
	else{

		CMDIFrameWnd::OnTimer(nIDEvent);
	}

}

void CMainFrame::DisplayToolTip(CString strMessage, UINT uTimeout, DWORD dwIcon, BOOL bPM)
{

	if(bPM){

		m_nPMIcon.uFlags		     = NIF_TIP | NIF_INFO;
		m_nPMIcon.dwInfoFlags      = dwIcon; // add an icon to a balloon ToolTip
		m_nPMIcon.uTimeout         = uTimeout * 1000;
		strcpy(m_nPMIcon.szInfo, strMessage);
		Shell_NotifyIcon(NIM_MODIFY, &m_nPMIcon); // add to the taskbar's status area
	}
	else{

		m_nIconData.uFlags		     = NIF_TIP | NIF_INFO;
		m_nIconData.dwInfoFlags      = dwIcon; // add an icon to a balloon ToolTip
		m_nIconData.uTimeout         = uTimeout * 1000;
		strcpy( m_nIconData.szInfo, strMessage);
		Shell_NotifyIcon(NIM_MODIFY, &m_nIconData); // add to the taskbar's status area
	}
}

void CMainFrame::ModifyTrayIcon(HICON hIcon)
{

	m_nIconData.uFlags	= NIF_ICON;
	m_nIconData.hIcon	= hIcon;

	Shell_NotifyIcon(NIM_MODIFY, &m_nIconData);
}

// dwIcon may be
// - NIIF_ERROR     An error icon. 
// - NIIF_INFO      An information icon. 
// - NIIF_NONE      No icon. 
// - NIIF_WARNING   A warning icon. 
// - NIIF_ICON_MASK Version 6.0. Reserved. 
// - NIIF_NOSOUND   Version 6.0. Do not play the associated sound. Applies only to balloon ToolTips 
// UTimeOut
//  min 10 secs, max 30 secs (enforced by windows :-( )
void CMainFrame::LoadToTray(HWND hWnd, UINT uCallbackMessage, CString strInfoTitle, CString strInfo, CString strTip, int uTimeout, HICON icon, DWORD dwIcon)
{

	//NOTIFYICONDATA contains information that the system needs to process taskbar status area messages

	ZeroMemory( &m_nIconData, sizeof( NOTIFYICONDATA ) );

	m_nIconData.cbSize		      = sizeof( NOTIFYICONDATA );
	m_nIconData.hWnd			  = hWnd;
	m_nIconData.uID			      = 120132;
	
	if(strInfo.IsEmpty()){

		m_nIconData.uFlags		      = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	}
	else{

		m_nIconData.uFlags		      = NIF_MESSAGE | NIF_ICON | NIF_TIP | NIF_INFO;
	}
			// Flag Description:
			// - NIF_ICON	 The hIcon member is valid.  
	        // - NIF_MESSAGE The uCallbackMessage member is valid. 
	        // - NIF_TIP	 The szTip member is valid. 
	        // - NIF_STATE	 The dwState and dwStateMask members are valid. 
	        // - NIF_INFO	 Use a balloon ToolTip instead of a standard ToolTip. The szInfo, uTimeout, szInfoTitle, and dwInfoFlags members are valid. 
            // - NIF_GUID	 Reserved. 

	m_nIconData.dwInfoFlags      = dwIcon; // add an icon to a balloon ToolTip

	m_nIconData.uCallbackMessage = uCallbackMessage;  
	m_nIconData.uTimeout         = uTimeout * 1000;
	m_nIconData.hIcon		   	 = icon;
	strcpy( m_nIconData.szInfoTitle, strInfoTitle );
	strcpy( m_nIconData.szInfo,      strInfo      );
	strcpy( m_nIconData.szTip,       strTip       );

	Shell_NotifyIcon( NIM_ADD, &m_nIconData ); // add to the taskbar's status area
}


LRESULT CMainFrame::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{

    UINT uID; 
    UINT uMsg; 
 
    uID = (UINT) wParam; 
    uMsg = (UINT) lParam; 
 
	if(uID == 120133){

		if(uMsg == WM_LBUTTONDBLCLK){

			ShowWindow(SW_RESTORE);
			SetFocus();
			SetForegroundWindow();
		}
		return 1;
	}
	else if (uID != 120132)
		return 0;
	
	CPoint pt;	

    switch (uMsg ) 
	{ 
	case WM_LBUTTONDBLCLK:
		ShowWindow(SW_RESTORE);
		SetFocus();
		SetForegroundWindow();
		break;
	
	case WM_RBUTTONDOWN:
	case WM_CONTEXTMENU:
		GetCursorPos(&pt);
		m_cmSystray.GetSubMenu(0)->TrackPopupMenu(TPM_BOTTOMALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,pt.x,pt.y,this);
		m_cmSystray.GetSubMenu(0)->SetDefaultItem(0,TRUE);
		break;
	case 0x00000405:
		if(m_bUpdate){
			::ShellExecute(	0,
					"open",
					"http://www.mxcontrol.org",
					0,
					0,
					SW_SHOW
				);
			m_bUpdate = FALSE;
		}
		break;
    } 
	return 1;
}

void CMainFrame::OnDestroy() 
{
	CMDIFrameWnd::OnDestroy();

	m_fFont.DeleteObject();
}

void CMainFrame::OnClose()
{

	if(g_sSettings.GetShowExitConfirm() && !m_wndDocSelector.m_Buttons.IsEmpty()){

		 ExitConfirmation dlg;
		 if(dlg.DoModal() == IDCANCEL){

			 return;
		 }
	}
	if(g_sSettings.GetSavePos()){
	
		m_frameManager.Save();
	}
	if(m_bFullScreenMode){

		FullScreenModeOff();
	}
	m_mxClient.m_hWnd = NULL;
	m_aWPNListeners.RemoveAll();
	m_mxClient.RemoveListener(m_hWnd);
	m_mxClient.Disconnect();
	
	m_eWPNConnected.SetEvent();
	DWORD n = WaitForSingleObject(m_eStartupDone.m_hObject, 1000);
	if(((n == WAIT_TIMEOUT) || (n == WAIT_FAILED)) && (m_pThread != 0)){

		TerminateThread(m_pThread->m_hThread, 0);
	}
	m_pThread = NULL;


	m_nUnreadPMs = 1;
	StopPMAni();	

	KillTimer(TIMER_IDLECHECK);

	if(m_bAway && (m_strAwayPrefix.GetLength() || m_strAwaySuffix.GetLength())){

		Util::MakeValidUserName(m_strAwayNickBck, TRUE, m_wClientID);
		g_sSettings.SetNickname(m_strAwayNickBck);
	}

	if(m_nIconData.hWnd && m_nIconData.uID>0){

		Shell_NotifyIcon(NIM_DELETE, &m_nIconData);
	}

	g_sSettings.Save();
	g_sSettings.SaveUsers();

	CMDIFrameWnd::OnClose();
}

BOOL CMainFrame::ShowWindow(int nCmdShow)
{

	if(g_sSettings.GetSavePos() && (nCmdShow != SW_MINIMIZE) && (nCmdShow != SW_HIDE)){

		m_frameManager.Load();
		return	CMDIFrameWnd::ShowWindow(m_frameManager.GetCmdShow());
	}
	return	CMDIFrameWnd::ShowWindow(nCmdShow);
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{

	CMDIFrameWnd::OnSysCommand(nID, lParam);

 	if(((nID & 0xFFF0) == SC_MINIMIZE) && g_sSettings.GetMiniTray())
	{

		if(m_bFullScreenMode){

			FullScreenModeOff();
		}
		this->ShowWindow(SW_HIDE);
	}
}

void CMainFrame::OnMute()
{

	m_bMuted = !m_bMuted;
	g_sSettings.SetSfxChatSfx(m_bMuted);
}

void CMainFrame::OnUpdateMute(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(!m_bMuted);
}

void CMainFrame::OnUpdateChannelChannellist(CCmdUI* pCmdUI) 
{

	pCmdUI->Enable(m_bWPN);
}

void CMainFrame::OnChannelChannellist() 
{
	
	if(!m_bChannelList){

		// Create a new ChannelListClient (only one allowed)  #0
		POSITION pos = GetApp()->GetFirstDocTemplatePosition();
		CDocTemplate* pTemplate = GetApp()->GetNextDocTemplate(pos);
		pTemplate->OpenDocumentFile(NULL);
		m_bChannelList = TRUE;
	}
	else{

		CString strText;
		CString strList;
		strList.LoadString(IDS_LIST);
		MDIGetActive()->GetWindowText(strText);
		while(strText.Compare(strList) != 0){
			
			this->MDINext();
			MDIGetActive()->GetWindowText(strText);
		}
	}

}

void CMainFrame::OnFileNew() 
{

	// Create a new ChatClientDocument	#1
	POSITION pos = GetApp()->GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate = GetApp()->GetNextDocTemplate(pos);
	pTemplate = GetApp()->GetNextDocTemplate(pos);
	pTemplate->OpenDocumentFile(NULL);
}

void CMainFrame::OnViewOptions() 
{

	// Create a new SettingsDocument #2
	if(!m_bSettings){

		POSITION pos = GetApp()->GetFirstDocTemplatePosition();
		CDocTemplate* pTemplate = GetApp()->GetNextDocTemplate(pos);
		pTemplate = GetApp()->GetNextDocTemplate(pos);
		pTemplate = GetApp()->GetNextDocTemplate(pos);
		pTemplate = GetApp()->GetNextDocTemplate(pos);
		pTemplate->OpenDocumentFile(NULL); 
		m_bSettings = TRUE;
	}
	else{

		CString strText;
		CString strSettings;
		strSettings.LoadString(IDS_SETTINGS);
		MDIGetActive()->GetWindowText(strText);
		while(strText.Compare(strSettings) != 0){
			
			this->MDINext();
			MDIGetActive()->GetWindowText(strText);
		}
	}
}

void CMainFrame::OpenPMView(CString strName, DWORD dwIP, WORD wPort, CString strInitMsg)
{

	POSITION pos = GetApp()->GetFirstDocTemplatePosition(); // #4
	CDocTemplate* pTemplate = GetApp()->GetNextDocTemplate(pos);
	pTemplate = GetApp()->GetNextDocTemplate(pos);
	pTemplate = GetApp()->GetNextDocTemplate(pos);
	//pTemplate = GetApp()->GetNextDocTemplate(pos);
	CString strTitle;
	strTitle.Format(IDS_SENDPM, strName); 
	CMessageDoc* pDoc = (CMessageDoc*)pTemplate->OpenDocumentFile(NULL);	
	pDoc->m_strUser = strName;
	pDoc->m_dwIP    = dwIP;
	pDoc->m_wPort   = wPort;
	pDoc->m_strMsg  = strInitMsg;
	pDoc->SetTitle(strTitle);
}

void CMainFrame::OpenBrowse(CString strName, DWORD dwIP, WORD wPort)
{

	POSITION pos = GetApp()->GetFirstDocTemplatePosition();	// #5
	CDocTemplate* pTemplate = GetApp()->GetNextDocTemplate(pos);
	pTemplate = GetApp()->GetNextDocTemplate(pos);
	pTemplate = GetApp()->GetNextDocTemplate(pos);
	pTemplate = GetApp()->GetNextDocTemplate(pos);
	pTemplate = GetApp()->GetNextDocTemplate(pos);
	CBrowseDoc* pDoc = (CBrowseDoc*)pTemplate->OpenDocumentFile(NULL);
	CString strTitle;
	strTitle.Format(IDS_BROWSE_TITLE, strName);
	pDoc->SetTitle(strTitle);
	pDoc->m_strUser = strName;
	pDoc->m_dwIP    = dwIP;
	pDoc->m_wPort   = wPort;
	m_mxClient.SendBrowseRequest(dwIP, wPort, strName);
}


void CMainFrame::JoinChannel()
{

	OnFileNew();
}


void CMainFrame::FullScreenModeOn()
{

	// available only if there is an active doc
	CMDIChildWnd* pChild=MDIGetActive();
	if(!pChild) return;

	//m_bToolBarWasVisible=(m_wndToolBarStd.IsWindowVisible()!=0);
	//m_wndToolBarStd.ShowWindow(SW_HIDE);
	// first create the new toolbar
	// this will contain the full-screen off button
	m_pwndFullScreenBar=new CToolBar;
	m_pwndFullScreenBar->Create(this);
	m_pwndFullScreenBar->LoadToolBar(IDR_FULLSCREEN);
	m_pwndFullScreenBar->SetBarStyle(m_pwndFullScreenBar->GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	// to look better:
	m_pwndFullScreenBar->ModifyStyle(0, TBSTYLE_FLAT);
	m_pwndFullScreenBar->EnableDocking(0);
	// place the full-screen off button somewhere:
	CPoint pt(300,200);
	FloatControlBar(m_pwndFullScreenBar,pt);
	
	// now save the old positions of the main and child windows
	GetWindowRect(&m_mainRect);
	// remove the caption of the mainWnd:
	LONG style=::GetWindowLong(m_hWnd,GWL_STYLE);
	style&=~WS_CAPTION;
	::SetWindowLong(m_hWnd,GWL_STYLE,style);
	int screenx=GetSystemMetrics(SM_CXSCREEN);
	int screeny=GetSystemMetrics(SM_CYSCREEN);
	// resize:
	SetWindowPos(NULL,-4,-4,screenx+8,screeny+8,SWP_NOZORDER);
	style=::GetWindowLong(pChild->m_hWnd,GWL_STYLE);
	m_bChildMax=(style & WS_MAXIMIZE)?true:false;
	// note here: m_bMainMax is not needed since m_hWnd only
	// changed its caption...

	// and maximize the child window
	// it will remove its caption, too.
	pChild->ShowWindow(SW_SHOWMAXIMIZED);
	RecalcLayout();
	m_bFullScreenMode=true;

}

void CMainFrame::FullScreenModeOff()
{

	// You can use SaveBarState() in OnClose(),
	// so remove the newly added toolbar entirely
	// in order SaveBarState() not
	// to save its state. That is why I used dynamic
	// allocation
	delete m_pwndFullScreenBar;
	LONG style=::GetWindowLong(m_hWnd,GWL_STYLE);
	style|=WS_CAPTION;
	::SetWindowLong(m_hWnd,GWL_STYLE,style);
	//if(m_bToolBarWasVisible)
	//	m_wndToolBarStd.ShowWindow(SW_SHOW);
	MoveWindow(&m_mainRect);
	RecalcLayout();
	CMDIChildWnd* pChild=MDIGetActive();
	// pchild can be NULL if the USER closed all the
	// childs during Full Screen Mode:
	if(pChild){
		if(m_bChildMax)
			MDIMaximize(pChild);
		else MDIRestore(pChild);
	}
	m_bFullScreenMode=false;
}


void CMainFrame::OnViewFullScreen() 
{

	if(m_bFullScreenMode){

		FullScreenModeOff();
	}
	else{
		
		FullScreenModeOn();
	}
}

void CMainFrame::OnUpdateViewFullScreen(CCmdUI* pCmdUI) 
{

	pCmdUI->SetCheck(m_bFullScreenMode);	
}

void CMainFrame::OnIdsScnextwindow() 
{

	MDINext();
}

void CMainFrame::OnViewChannelbar() 
{

	m_wndDocSelector.ShowWindow(m_wndDocSelector.IsWindowVisible() ? SW_HIDE : SW_SHOW);
	RecalcLayout();
}

void CMainFrame::OnUpdateViewChannelBar(CCmdUI* pCmdUI)
{

	pCmdUI->SetCheck(m_wndDocSelector.IsWindowVisible());
}

void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{

	CMDIFrameWnd::OnUpdateFrameTitle(bAddToTitle);
}

UINT CMainFrame::StartupThread(LPVOID lpParam)
{

	CMainFrame* pFrame = (CMainFrame*)lpParam;
	ASSERT(pFrame);

	// Animate Systemtray icon
	pFrame->StartAni();
	
	while(AfxGetApp() == 0 && !::IsWindow(pFrame->m_hWnd)){

		// wait until mainwindow is initialized
		// because the plugin interface relies
		// on a valid handle to the nain window
		Sleep(5);
	}
	
	// Load Plugins
	pFrame->LoadPlugins();

	// Connect to the WinMX Peer Network
	pFrame->InitWPNP();

	if(!pFrame->m_bDisconnected){

		WaitForSingleObject(pFrame->m_eWPNConnected.m_hObject, INFINITE);
	}
	
	if(!pFrame->m_bDisconnected){

		::SendMessage(pFrame->m_hWnd, WM_FINISH_START, 0, 0);
	}
	
	return 0;
}

LRESULT CMainFrame::OnFinishStart(WPARAM wParam, LPARAM lParam)
{

	if(g_sSettings.GetAutoList()){

		OnChannelChannellist();
	}
	
	if(GetKeyState(VK_SHIFT) >= 0){

		ExecuteAutoJoins();
	}

#ifndef _DEBUG
	if(g_sSettings.GetUpdate()){

		CheckUpdate();
	}
#endif

	m_eStartupDone.SetEvent();
	StopAni();

	return 0;
}

void CMainFrame::LoadPlugins(void)
{

	int i = 0;
	CFileFind finder;

	BOOL bResult = finder.FindFile(g_sSettings.GetWorkingDir() + _T("\\Plugins\\*.r2x"));

	PLUGIN_DATA plugin;
	plugin.hDLL		= NULL;
	plugin.pEx		= NULL;
	plugin.lApiVersion	= 0;

	while(bResult){

		bResult = finder.FindNextFile();
		
		plugin.hDLL			= NULL;
		plugin.pEx			= NULL;
		plugin.lApiVersion	= 0;
		plugin.strFilename	= finder.GetFilePath();

		plugin.hDLL = LoadLibrary(plugin.strFilename);
		if(plugin.hDLL == NULL){
		
			CString strError;
			strError.Format(IDS_ERROR_PLUGIN_LOAD, plugin.strFilename);
			DisplayToolTip(strError, 10000, NIIF_ERROR);
			continue;
		}
		
		REGISTERPLUGIN pRegister = (REGISTERPLUGIN)GetProcAddress(plugin.hDLL, "RegisterExtensionEx");

		if(pRegister == NULL){
		
			CString strError;
			strError.Format(IDS_ERROR_INVALID_PLUGIN, finder.GetFileName());
			DisplayToolTip(strError, 10000, NIIF_ERROR);
			continue;
		}
	
		plugin.pEx = pRegister(&plugin.lApiVersion);

		if(plugin.lApiVersion < API_VERSION){

			CString strError;
			strError.Format(IDS_ERROR_PLUGIN_OLDAPI, finder.GetFileName(), plugin.lApiVersion, API_VERSION);
			DisplayToolTip(strError, 10000, NIIF_ERROR);
			continue;
		}

		if(plugin.pEx == NULL){

			CString strError;
			strError.Format(IDS_ERROR_PLUGIN_INIT, finder.GetFileName());
			DisplayToolTip(strError, 10000, NIIF_ERROR);
			continue;
		}

		plugin.pEx->Init();
		g_aPlugins.Add(plugin);
	}
}

void CMainFrame::UnloadPlugins(void)
{

	for(int i = 0; i < g_aPlugins.GetSize(); i++){

		g_aPlugins[i].pEx->Quit();
		FreeLibrary(g_aPlugins[i].hDLL);	
	}

	g_aPlugins.RemoveAll();
}

BOOL CMainFrame::DeletePlugin(CString strName)
{
	
	for(int i = 0; i < g_aPlugins.GetSize(); i++){

		if(g_aPlugins[i].pEx->m_strName == strName){

			g_aPlugins[i].pEx->Quit();
			FreeLibrary(g_aPlugins[i].hDLL);

			TRY{

				CFile::Remove(g_aPlugins[i].strFilename);
			}
			CATCH(CFileException, e){

				return 0;
			}END_CATCH;

			g_aPlugins.RemoveAt(i);

			return 1;
		}
	}
	return 0;
}


void CMainFrame::ReloadPlugins(void)
{

	UnloadPlugins();
	LoadPlugins();
	if(g_aPlugins.GetSize() == 0) return;

	for(int i = 0; i < m_wndDocSelector.m_Buttons.GetSize(); i++){

		if(((CSwitcherButton*)m_wndDocSelector.m_Buttons[i])->m_bIsChatClient){

			for(int j = 0; j < g_aPlugins.GetSize(); j++){

				g_aPlugins[j].pEx->OnJoinChannel((DWORD)((CSwitcherButton*)m_wndDocSelector.m_Buttons[i])->m_AttachedView->m_hWnd, 
						((CChatView*)((CSwitcherButton*)m_wndDocSelector.m_Buttons[i])->m_AttachedView)->GetDocument()->m_strRoom,
						&((CChatView*)((CSwitcherButton*)m_wndDocSelector.m_Buttons[i])->m_AttachedView)->m_aUsers,
						((CChatView*)((CSwitcherButton*)m_wndDocSelector.m_Buttons[i])->m_AttachedView)->m_mxClient.m_strServerString,
						((CChatView*)((CSwitcherButton*)m_wndDocSelector.m_Buttons[i])->m_AttachedView)->m_mxClient.m_strServerVer
						
						);
					
			}
		}
	}
}

LRESULT CMainFrame::OnTooltip(WPARAM wParam, LPARAM lParam)
{

	EXT_MSG_STRUCT* msg = (EXT_MSG_STRUCT*)lParam;
	DisplayToolTip(msg->lpszMsg, (UINT)msg->wParam, (DWORD)msg->lParam);

	return 1;
}

LRESULT CMainFrame::OnPluginQueryInfo(WPARAM wParam, LPARAM lParam)
{

	EXT_MSG_STRUCT* msg = (EXT_MSG_STRUCT*)lParam;

	LRESULT lResult = 1;
	if(wParam == RMX_GETVERSION){

		lResult = VERSION_NUMBER;
	}
	else if(wParam == RMX_LISTSTATUS){

		switch(msg->wParam){

			case 0:
				if(m_bChannelList){

					m_wndDocSelector.CloseChannellist(m_dwChannelList);
				}
				break;
			case 1:
				OnChannelChannellist();
				break;
			case 2:
				lResult = (LRESULT)m_bChannelList;
				break;
		}
	}
	else if(wParam >= 100 && wParam < 200 && m_bChannelList){

		lResult = m_wndDocSelector.DeliverMessage(m_dwChannelList, WM_QAPI, wParam, lParam);
	}
	else if(wParam >= 200 && wParam < 300){

		lResult = m_wndDocSelector.DeliverMessage((DWORD)msg->wParam, WM_QAPI, wParam, lParam);
	}
	else if(wParam >= 300 && wParam < 400){

		switch(wParam){
			case RMX_GET_CONFIG:
				{
					int nLen = g_sSettings.GetIniFile().GetLength();
					lResult = (LRESULT)nLen;
					if(nLen < (int)msg->wParam){

						strcpy((char*)msg->lParam, g_sSettings.GetIniFile());
					}
				}
				break;
			case RMX_CFG_COLOR_BG:
				lResult = (LRESULT)g_sSettings.GetRGBBg();
				break;
			case RMX_CFG_COLOR_NAME:
				lResult = (LRESULT)g_sSettings.GetRGBNormalName();
				break;
			case RMX_CFG_COLOR_MSG:
				lResult = (LRESULT)g_sSettings.GetRGBNormalMsg();
				break;
			case RMX_CFG_COLOR_ACTION:
				lResult = (LRESULT)g_sSettings.GetRGBActionMsg();
				break;
			case RMX_CFG_COLOR_TOPIC:
				lResult = (LRESULT)g_sSettings.GetRGBTopic();
				break;
			case RMX_CFG_COLOR_MOTD:
				lResult = (LRESULT)g_sSettings.GetRGBMotd();
				break;
			case RMX_CFG_COLOR_OPMSG:
				lResult = (LRESULT)g_sSettings.GetRGBOp();
				break;
			case RMX_CFG_COLOR_ENTER:
				lResult = (LRESULT)g_sSettings.GetRGBJoin();
				break;
			case RMX_CFG_COLOR_PART:
				lResult = (LRESULT)g_sSettings.GetRGBPart();
				break;
			case RMX_CFG_COLOR_LINK:
				lResult = (LRESULT)g_sSettings.GetRGBLink();
				break;
			case RMX_CFG_COLOR_ERROR:
				lResult = (LRESULT)g_sSettings.GetRGBErr();
				break;
			case RMX_CFG_COLOR_PENDING:
				lResult = (LRESULT)g_sSettings.GetRGBPend();
				break;
			case RMX_CFG_COLOR_OK:
				lResult = (LRESULT)g_sSettings.GetRGBOK();
				break;
		}
	}
	return lResult;
}

LRESULT CMainFrame::OnPluginSendMsg(WPARAM wParam, LPARAM lParam)
{

	EXT_MSG_STRUCT* msg = (EXT_MSG_STRUCT*)lParam;

	m_mxClient.SendPM((DWORD)msg->wParam, (WORD)msg->lParam, msg->lpszParam, msg->lpszMsg);
	return 1;
}


LRESULT CMainFrame::OnPluginInput(WPARAM wParam, LPARAM lParam)
{

	m_wndDocSelector.DeliverMessage((DWORD)wParam, UWM_TEXT_INPUT, 1, (LPARAM)((EXT_MSG_STRUCT*)lParam)->lpszMsg);
	return 1;
}

LRESULT CMainFrame::OnPluginEcho(WPARAM wParam, LPARAM lParam)
{

	m_wndDocSelector.DeliverMessage((DWORD)wParam, WM_ECHOTEXT, 0, lParam);
	return 1;
}

LRESULT CMainFrame::OnPluginSysEcho(WPARAM wParam, LPARAM lParam)
{

	m_wndDocSelector.DeliverMessage((DWORD)wParam, WM_ECHOSYSTEXT, 0, lParam);
	return 1;
}

LRESULT CMainFrame::OnChannelTooltip(WPARAM wParam, LPARAM lParam)
{

	m_wndDocSelector.DisplayBtnTool((HWND)wParam, ((EXT_MSG_STRUCT*)lParam)->lpszMsg);
	return 1;
}

void CMainFrame::OnSystrayRestore()
{

	ShowWindow(SW_RESTORE);
}

void CMainFrame::CheckUpdate(void)
{

	CInternetSession	is;
	CString				strTmp, strNewVersion;
	
	try{

		CHttpFile* pFile = (CHttpFile*) is.OpenURL("http://mxcontrol.sourceforge.net/update/robomx.info");
		// some ISPs interfear with a proxy to display adds when the first page is loaded
		// so we close and opem the file again   ;-)
		// screw those damned spammers :-P
		pFile->Close();
		delete pFile;
		pFile = 0;
		pFile = (CHttpFile*) is.OpenURL("http://mxcontrol.sourceforge.net/update/robomx.info");
		
		if(pFile != NULL){

			pFile->ReadString(strNewVersion);
			pFile->Close();
			delete pFile;
		}

		is.Close();
	}
	catch(CInternetException* pEx){
		
		TCHAR   szCause[255];
		CString strFormatted;

		pEx->GetErrorMessage(szCause, 255);
		strFormatted.Format("Error during Update Query: %s\n", szCause);
		TRACE(strFormatted);
		return;
	}

	CString strOldVersion = STR_VERSION_DLG;
	strOldVersion.Replace("\n", " ");
	strNewVersion.Replace("\\n", " ");

	if(strNewVersion != strOldVersion){

		m_bUpdate = TRUE;
		strTmp.Format(IDS_UPDATE_AVAILABLE, strOldVersion, strNewVersion);
		DisplayToolTip(strTmp, 30);
	}
}

void CMainFrame::ExecuteAutoJoins(void)
{

	CIni ini;
	int n = 0;
	CString strTmp, strRoom, strLogin;
	
	ini.SetIniFileName(g_sSettings.GetIniFile());
	n = ini.GetValue("AutoJoins", "Num", 0);

	for(int i = 1; i < n+1; i++){

		strTmp.Format("AutoJoin_%03d", i);
		strRoom = ini.GetValue("AutoJoins", strTmp, "");
		strTmp.Format("AutoJoinLogin_%03d", i);
		strLogin = ini.GetValue("AutoJoins", strTmp, "");
		if(!strRoom.IsEmpty()){

			m_strQuickLogin = strLogin;
			m_strRoom = strRoom;
			m_bQuickJoin = TRUE;
			OnFileNew();
			Sleep(100);
		}	
	}
}

void CMainFrame::OnReconnectAll()
{

	m_wndDocSelector.BroadcastMessage(WM_COMMAND, MAKEWPARAM(ID_RECONNECT, 0), 0);
}


void CMainFrame::OnRename()
{

	CRenameDlg dlg;
	dlg.m_bWPNOnly = TRUE;
	dlg.m_dwFiles  = g_sSettings.GetFiles();
	dlg.m_nLine    = g_sSettings.GetLine();
	dlg.m_strName  = Util::GetBaseName(g_sSettings.GetNickname());

	if(dlg.DoModal() == IDOK){

		m_mxClient.SendRename(dlg.m_strName, dlg.m_nLine);
		m_wndDocSelector.BroadcastMessage(UWM_RENAMECL, 0, 0);
	}
}

void CMainFrame::InitWPNP(void)
{


	m_wndStatusBar.SetWPNStatus(m_bDisconnected ? 0 : 2);
	//m_wndToolBarStd.Invalidate();
	m_mxClient.AddListener(m_hWnd);

	if(!m_bDisconnected){

		ConnectWPN();
	}
}


void CMainFrame::ConnectWPN(void)
{

	StartAni();
	CString strNick = g_sSettings.GetNickname();

	strNick = Util::GetBaseName(strNick);

	if(g_sSettings.GetForcePrimary()){

		WORD wPort;
		DWORD dwIP;
		CString strNode, strPort;
		CTokenizer token(g_sSettings.GetForcedNode(), ":");
		token.Next(strNode);
		token.Next(strPort);

		dwIP = CMySocket::GetIP(strNode);
		if(dwIP == 0){

			DisplayToolTip(Util::LoadString(IDS_ERROR_CONNECTFORCED), 10000, NIIF_ERROR);
			return;
		}
		wPort = atoi(strPort);

		PARENTNODEINFO NodeInfo;
		NodeInfo.bFreePri = 1;
		NodeInfo.bFreeSec = 1;
		NodeInfo.wReserved = 0;
		NodeInfo.wTCPPort = wPort;
		NodeInfo.dwNodeIP = dwIP;
		NodeInfo.wUDPPort = 0;

		m_mxClient.ConnectWPN(&NodeInfo, strNick, g_sSettings.GetLine(), 0);
	}
	else{

		m_mxClient.ConnectWPN(NULL, strNick, g_sSettings.GetLine(), 0);
	}
}

LRESULT CMainFrame::OnWPNNotify(WPARAM wParam, LPARAM lParam)
{

	// First we process this message ourselfs
	BOOL bReturn  = FALSE;
	BOOL bHandled = FALSE;

	switch(wParam){

		case WPN_ERROR:
			m_wndStatusBar.SetWPNStatus(WPN_STATUS_DISCONNECTED);
			if(!m_bDisconnected && !g_sSettings.GetForcePrimary()){

				m_mxClient.RefreshWPN();
			}
			//m_wndToolBarStd.Invalidate();
			break;

		case WPN_PARENTDISCONNECTED:
			TRACE("Disonnected\n");
			m_wndStatusBar.SetWPNStatus(WPN_STATUS_DISCONNECTED);
			if(!m_bDisconnected && !g_sSettings.GetForcePrimary()){

				m_mxClient.RefreshWPN();
			}
			//InitWPNP();
			m_bWPN = FALSE;
			KillTimer(TIMER_PING);
			bReturn = TRUE;
			//m_wndToolBarStd.Invalidate();
			break;

		case WPN_WPNCONNECTSTART:
		case WPN_CHANGEPARENTSTART:
			TRACE("Connecting...\n");
			m_wndStatusBar.SetWPNStatus(WPN_STATUS_CONNECTING);
			StartAni();
			m_bWPN = FALSE;
			bReturn = TRUE;
			//m_wndToolBarStd.Invalidate();
			break;

		case WPN_WPNCONNECTED:
		case WPN_CHANGEPARENTCOMP:
			TRACE("Connected\n");
			m_wndStatusBar.SetWPNStatus(WPN_STATUS_CONNECTED);
			StopAni();
			m_eWPNConnected.SetEvent();
			m_mxClient.SendDummyShares();
			SetTimer(TIMER_PING, 60000, NULL);
			m_bWPN = TRUE;
			bReturn = TRUE;
			//m_wndToolBarStd.Invalidate();
			break;

		case WPN_CLIENTID:
			{
				m_wndStatusBar.SetWPNStatus(WPN_STATUS_CONNECTED);
				m_wClientID = (WORD)lParam;
				CString strName = g_sSettings.GetNickname();
				Util::MakeValidUserName(strName, TRUE, m_wClientID);
				for(int i = 0; i < g_aPlugins.GetSize(); i++){

					if(g_aPlugins[i].lApiVersion >= 0x1002L){

						g_aPlugins[i].pEx->OnSetUserName(strName);
					}
				}
				bReturn = TRUE;
				//m_wndToolBarStd.Invalidate();
			}
			break;
		case WPN_INCOMING_PM:
			bReturn = IncomingPM(wParam, lParam);
			bHandled = bReturn != PM_NOTPROCESSED;
			break;
	}

	// then we broadcast it to all listeners
	for(int i = 0; i < m_aWPNListeners.GetSize() && !bHandled; i++){

		CWnd* pTmp = m_aWPNListeners[i];
		if(pTmp != NULL){

			pTmp->SendMessage(UWM_WPNNOTIFY, wParam, lParam);
		}
	}

	return (LRESULT)bReturn;
}

int CMainFrame::IncomingPM(WPARAM wParam, LPARAM lParam)
{

	PMINFO *pm = (PMINFO*)lParam;
	if(pm == NULL) return PM_NOTPROCESSED;

	CString strUser = pm->lpszName;
	CString strMsg  = pm->lpszMessage;
	DWORD   dwIP    = pm->dwIP;
	WORD	wPort	= pm->wPort;

	for(int i = 0; i < g_aPlugins.GetSize(); i++){

		g_aPlugins[i].pEx->OnPrivateMsg(&strUser, &strMsg, dwIP, wPort);
	}

	if(strMsg.IsEmpty() || strUser.IsEmpty()) return PM_NOTPROCESSED;	// message empty, bounce out

	// check if message is sent by ignored user
	for(i = 0; i < g_sSettings.m_aIgnored.GetSize(); i++){

		if(Util::WildcardMatch(strUser, g_sSettings.m_aIgnored[i])){

			return PM_IGNORED;
		}
	}

	if(g_sSettings.GetPMOnlyTrusted()){

		for(i = 0; i < g_sSettings.m_aTrusted.GetSize(); i++){

			if(Util::WildcardMatch(strUser, g_sSettings.m_aTrusted[i])){

				break;
			}
		}
		if(i >= g_sSettings.m_aTrusted.GetSize()){

			return PM_PRIVACY; // User is not to be trusted. ignore PM
		}
	}

	CString strTitle;
	strTitle.Format(IDS_SENDPM, strUser);

	BOOL bHandled = FALSE;
	for(int i = 0; i < m_aWPNListeners.GetSize() && !bHandled; i++){

		CWnd* pTmp = m_aWPNListeners[i];
		if(pTmp != NULL){

			bHandled = pTmp->SendMessage(UWM_WPNNOTIFY, wParam, lParam);
		}
	}

	if(!bHandled){

		OpenPMView(strUser, dwIP, wPort, pm->lpszMessage);
	}

	if(g_sSettings.GetPMToolTip()){

		WINDOWPLACEMENT wp;
		GetWindowPlacement(&wp);
		if(wp.showCmd == SW_SHOWMINIMIZED){

			CString strTooltip;
			strTooltip.Format(IDS_NEWPM, strUser);
			DisplayToolTip(strTooltip, 1000, NIIF_INFO, TRUE);
		}
	}

	return PM_RECIEVED;
}

void CMainFrame::OnUpdateWPNRefresh(CCmdUI *pCmdUI)
{

	pCmdUI->Enable(!m_bDisconnected && m_bWPN && !g_sSettings.GetForcePrimary());
}

void CMainFrame::OnWPNRefresh()
{

	if(!m_bDisconnected && !g_sSettings.GetForcePrimary()){

		StartAni();
		m_bWPN = FALSE;
		m_mxClient.RefreshWPN();
	}
}

void CMainFrame::OnUpdateWPNConnect(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(m_bWPN);
	m_wndToolBarStd.GetToolBarCtrl().GetImageList()->Replace(0, m_bWPN ? m_hDisconnect : m_hConnect);
}

void CMainFrame::OnWPNConnect()
{

	if(m_bDisconnected){

		m_bDisconnected = FALSE;
		m_wndStatusBar.SetWPNStatus(WPN_STATUS_CONNECTING);
		ConnectWPN();
		//m_wndToolBarStd.Invalidate();
	}
	else{

		m_bDisconnected = TRUE;
		m_wndStatusBar.SetWPNStatus(WPN_STATUS_DISCONNECTED);
		m_mxClient.Disconnect();
		//m_wndToolBarStd.Invalidate();
	}
}

LRESULT CMainFrame::OnAddListener(WPARAM wParam, LPARAM lParam)
{

	CWnd* pWnd = (CWnd*)lParam;

	for(int i = 0; i < m_aWPNListeners.GetSize(); i++){

		if(m_aWPNListeners[i] == pWnd){

			return 1;
		}
	}
	m_aWPNListeners.Add(pWnd);
	return 1;
}

LRESULT CMainFrame::OnRemoveListener(WPARAM wParam, LPARAM lParam)
{

	CWnd* pWnd = (CWnd*)lParam;

	for(int i = 0; i < m_aWPNListeners.GetSize(); i++){

		if(m_aWPNListeners[i] == pWnd){

			m_aWPNListeners.RemoveAt(i);
			return 1;
		}
	}

	return 0;
}

LRESULT CMainFrame::OnWhois(WPARAM wParam, LPARAM lParam)
{

	EXT_MSG_STRUCT* pMsg = (EXT_MSG_STRUCT*)lParam;
	m_mxClient.SendWhois((DWORD)pMsg->wParam, (WORD)pMsg->lParam, pMsg->lpszParam);

	return 1;
}

LRESULT CMainFrame::OnBrowse(WPARAM wParam, LPARAM lParam)
{

	EXT_MSG_STRUCT* pMsg = (EXT_MSG_STRUCT*)lParam;
	OpenBrowse(pMsg->lpszParam, (DWORD)pMsg->wParam, (WORD)pMsg->lParam);
	return 1;
}

LRESULT CMainFrame::OnReBrowse(WPARAM wParam, LPARAM lParam)
{

	EXT_MSG_STRUCT* pMsg = (EXT_MSG_STRUCT*)lParam;
	m_mxClient.SendBrowseRequest((DWORD)pMsg->wParam, (WORD)pMsg->lParam, pMsg->lpszParam);
	return 1;
}

LRESULT CMainFrame::OnNewPM(WPARAM wParam, LPARAM lParam)
{

	EXT_MSG_STRUCT* pMsg = (EXT_MSG_STRUCT*)lParam;
	OpenPMView(pMsg->lpszParam, (DWORD)pMsg->wParam, (WORD)pMsg->lParam);
	return 1;
}

LRESULT CMainFrame::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{

	// code below is taken from winfrm.cpp and modified to set the 
	// status text in pane 1 instead of pane 0

	UINT nIDLast = m_nIDLastMessage;
	m_nFlags &= ~WF_NOPOPMSG;

	CWnd* pMessageBar = GetMessageBar();
	if (pMessageBar != NULL)
	{
		LPCTSTR lpsz = NULL;
		CString strMessage;

		// set the message bar text
		if (lParam != 0)
		{
			ASSERT(wParam == 0);    // can't have both an ID and a string
			lpsz = (LPCTSTR)lParam; // set an explicit string
		}
		else if (wParam != 0)
		{
			// map SC_CLOSE to PREVIEW_CLOSE when in print preview mode
			if (wParam == AFX_IDS_SCCLOSE && m_lpfnCloseProc != NULL)
				wParam = AFX_IDS_PREVIEW_CLOSE;

			// get message associated with the ID indicated by wParam
		 //NT64: Assume IDs are still 32-bit
			GetMessageString((UINT)wParam, strMessage);
			lpsz = strMessage;
		}
		
		//pMessageBar->SetWindowText(lpsz);
		m_wndStatusBar.SetPaneText(1, lpsz);

		// update owner of the bar in terms of last message selected
		CFrameWnd* pFrameWnd = pMessageBar->GetParentFrame();
		if (pFrameWnd != NULL)
		{
			m_nIDLastMessage = (UINT)wParam;
			m_nIDTracking = (UINT)wParam;
		}
	}

	m_nIDLastMessage = (UINT)wParam;    // new ID (or 0)
	m_nIDTracking = (UINT)wParam;       // so F1 on toolbar buttons work
	return nIDLast;
}

void CMainFrame::OnViewIgnoredusers()
{

	IgnoredUsers dlg;
	dlg.DoModal();
}

void CMainFrame::OnViewTrustedusers()
{

	TrustedUsers dlg;
	dlg.DoModal();
}

void CMainFrame::OnViewAutojoinmanager()
{

	ManageAutojoins dlg;
	dlg.DoModal();
}

void CMainFrame::OnHelpVisithomepage()
{

	ShellExecute(NULL, "open", "http://www.mxcontrol.org", NULL, NULL, SW_SHOW);
}

void CMainFrame::OnCtrltab()
{

	m_wndDocSelector.SwitchToNext();
}

void CMainFrame::OnCtrlshifttab()
{

	m_wndDocSelector.SwitchToPrevious();
}

void CMainFrame::OnHelpVisitonlineforum()
{

	ShellExecute(NULL, "open", "http://www.mxcontrol.org/modules.php?name=Forums", NULL, NULL, SW_SHOW);
}

void CMainFrame::OnHelpSubscribetomailinglist()
{

	ShellExecute(NULL, "open", "http://lists.sourceforge.net/lists/listinfo/mxcontrol-general", NULL, NULL, SW_SHOW);
}

void CMainFrame::OnHelpLincense()
{

	ShellExecute(NULL, "open", g_sSettings.GetWorkingDir() + "\\license.txt", NULL, NULL, SW_SHOW);
}

void CMainFrame::OnHelpChangelog()
{

	ShellExecute(NULL, "open", g_sSettings.GetWorkingDir() + "\\change-log.txt", NULL, NULL, SW_SHOW);
}


BOOL CMainFrame::PreTranslateMessage(MSG *pMsg)
{

	switch(pMsg->message){

		case WM_MOUSEACTIVATE:
		case WM_MOUSEMOVE:
		case WM_CHAR:
			m_dwLastActivity = GetTickCount();
			if(m_bIdleAway && g_sSettings.GetAutoBack()){

				OnSetBack(0, 0);
			}
			break;
	}
	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

LRESULT CMainFrame::OnSetBack(WPARAM wParam, LPARAM lParam)
{

	CString strNickname = g_sSettings.GetNickname();
	
	if(m_strAwayPrefix.GetLength() || m_strAwaySuffix.GetLength()){

		strNickname = m_strAwayNickBck;
		Util::MakeValidUserName(strNickname, TRUE, m_wClientID);
		
		g_sSettings.SetNickname(strNickname);
		m_mxClient.SendRename(strNickname, g_sSettings.GetLine(), 0);
		m_wndDocSelector.BroadcastMessage(UWM_RENAMECL, 0, 0);
	}
	
	m_strAwayNickBck.Empty();
	CString strMessage, strReason, strTime;

	strReason = GetApp()->GetMainFrame()->m_strAway;
	
	DWORD dwAway = GetTickCount() - GetApp()->GetMainFrame()->m_uAwayStart;

	int nSec = dwAway / 1000;
	int nMin = dwAway / 60000;
	int nHour = dwAway / 3600000;
	strTime.Format("%02dh %02dm %02ds", nHour, nMin - nHour*60, nSec - nMin*60);

	strMessage.Format(wParam == 0 ? IDS_AWAY_BACK : IDS_AWAY_SILENTBACK, strReason, strNickname, strTime);
	
	if(wParam == 0){

		m_wndDocSelector.InputAll(strMessage);
	}
	else{

		EXT_MSG_STRUCT msg;
		msg.lpszMsg = strMessage;
		msg.wParam  = (WPARAM)g_sSettings.GetRGBNormalMsg();
		msg.lParam  = (LPARAM)g_sSettings.GetRGBBg();
		m_wndDocSelector.BroadcastMessage(WM_ECHOTEXT, 0, (LPARAM)&msg);
	}
	m_bAway = FALSE;
	m_bIdleAway = FALSE;
	m_strAway.Empty();
	m_strPMAway.Empty();
	m_strAwayPrefix.Empty();
	m_strAwaySuffix.Empty();
	m_uAwayStart = 0;

	if(m_hIcon) DeleteObject(m_hIcon);
	m_hIcon = GetApp()->LoadIcon(IDR_MAINFRAME);
	m_nIconData.uFlags = NIF_ICON;
	m_nIconData.hIcon  = m_hIcon;
	Shell_NotifyIcon(NIM_MODIFY, &m_nIconData);
	
	SetTimer(TIMER_IDLECHECK, 1000, NULL);
	return 1;
}

LRESULT CMainFrame::OnSetAway(WPARAM wParam, LPARAM lParam)
{

	UINT nAwayItem = (UINT)wParam;

	CString strLabel, strReason, strPrefix, strSuffix, strMessage;
	
	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());
	
	if(lParam == 0){

		strLabel.Format("Reason_%d", nAwayItem);
		strReason = ini.GetValue("AwayControl", strLabel, "");
		m_bIdleAway = FALSE;

		CInputRequest dlg;
		dlg.m_strInput = strReason;
		dlg.SetMode(6, &m_fFont);
		if(dlg.DoModal() == IDOK){

			strReason = dlg.m_strInput;
			strLabel.Format("Suffix_%d", nAwayItem);
			strSuffix = ini.GetValue("AwayControl", strLabel, "");
			strLabel.Format("Prefix_%d", nAwayItem);
			strPrefix = ini.GetValue("AwayControl", strLabel, "");
		}
		else strReason.Empty();
	}
	else{

		strReason = ini.GetValue("AwayControl", "AutoReason", "");
		strSuffix = ini.GetValue("AwayControl", "AutoSuffix", "");
		strPrefix = ini.GetValue("AwayControl", "AutoPrefix", "");
		m_bIdleAway = TRUE;
	}
	if(!strReason.IsEmpty()){

		KillTimer(TIMER_IDLECHECK);

		CString strMessage;
		strMessage.Format(IDS_AWAYCTRL, strReason);

		m_bAway		= TRUE;
		m_strAway	= strReason;
		m_strPMAway.Format(IDS_PM_AWAY, strReason);
		m_wndDocSelector.InputAll(strMessage);
		m_uAwayStart = GetTickCount();
		m_strAwayPrefix = strPrefix;
		m_strAwaySuffix = strSuffix;
		if(strPrefix.GetLength() || strSuffix.GetLength()){

			CString strAwayName = Util::GetBaseName(g_sSettings.GetNickname());
			
			m_strAwayNickBck = strAwayName;

			//strAwayName);
			strAwayName.Insert(0, strPrefix);
			if(strAwayName.GetLength()-3 > 0){

				strAwayName.Insert(strAwayName.GetLength()-3, strSuffix);
			}
			
			Util::MakeValidUserName(strAwayName, TRUE, m_wClientID);
			if(strAwayName.GetLength() > 44){

				//strAwayName = strAwayName.Left(38);
				strAwayName.Truncate(38);
				Util::MakeValidUserName(strAwayName, TRUE, m_wClientID);
			}
			g_sSettings.SetNickname(strAwayName);
			
			m_mxClient.SendRename(strAwayName, g_sSettings.GetLine(), 0);
			m_wndDocSelector.BroadcastMessage(UWM_RENAMECL, 0, 0);
		}

		if(m_hIcon) DeleteObject(m_hIcon);
		m_hIcon = GetApp()->LoadIcon(IDI_AWAY);
		m_nIconData.uFlags = NIF_ICON;
		m_nIconData.hIcon  = m_hIcon;
		Shell_NotifyIcon(NIM_MODIFY, &m_nIconData);
	}

	return 1;
}

LRESULT CMainFrame::OnLoadSettings(WPARAM wParam, LPARAM lParam)
{

	for(int i = 0; i < g_aPlugins.GetSize(); i++){

		g_aPlugins[i].pEx->OnReloadCfg();
	}
    m_wndDocSelector.BroadcastMessage(UWM_LOAD_SETTINGS, 0, 0);
	
	m_bMuted = g_sSettings.GetSfxChatSfx();
	return 1;
}