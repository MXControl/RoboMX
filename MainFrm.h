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


#if !defined(AFX_MAINFRM_H__4EE0D859_D847_4259_AA7D_531CB4E1928B__INCLUDED_)
#define AFX_MAINFRM_H__4EE0D859_D847_4259_AA7D_531CB4E1928B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "docselect.h"
#include "ColorStatusBar.h"
#include "Winsock2.h"
#include "WPNPClient.h"
#include "FrameManager.h"
#include "RoboEx.h"

typedef struct	TAG_PLUGIN_DATA
{

	CString		strFilename;
	LONG		lApiVersion;
	HINSTANCE	hDLL;
	CRoboEx*	pEx;

}PLUGIN_DATA, *PLUGINDATA;

class Emoticon;

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:

	CMainFrame();
	virtual ~CMainFrame();

	// Public interface
	void LoadPlugins(void);
	void UnloadPlugins(void);
	BOOL DeletePlugin(CString strName);
	void ReloadPlugins(void);
	void CheckUpdate(void);
	void ExecuteAutoJoins(void);
	void ModifyTrayIcon(HICON hIcon);
	void StopAni(void);
	void StartAni(void);
	void InitWPNP(void);
	void OpenPMView(CString strName, DWORD dwIP, WORD wPort, CString strInitMsg = "");
	void OpenBrowse(CString strName, DWORD dwIP, WORD wPort);
	int IncomingPM(WPARAM wParam, LPARAM lParam);
	void LoadToTray(HWND hWnd, UINT uCallbackMessage, CString strInfoTitle, CString strInfo, CString strTip, int uTimeout, HICON icon, DWORD dwIcon);
	void DisplayToolTip(CString strMessage, UINT uTimeout, DWORD dwIcon = NIIF_INFO, BOOL bPM = FALSE);

	void StartPMAni(void);
	void StopPMAni(void);
	void OnTimer(UINT nIDEvent);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle);
	afx_msg LRESULT OnWPNNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnChannelChannellist();
	void JoinChannel();
	
	CMenu* GetMenu() const;
	// WPNP Client
	CWPNPClient m_mxClient;
	CEvent		m_eWPNConnected;
	
	// Temporary variables for quickjoin and incoming PM's
	CString			m_strRoom;
	CString         m_strQuickLogin;
	BOOL			m_bChannelList;
	DWORD			m_dwChannelList;
	BOOL			m_bSettings;
	WORD			m_wClientID;
	BOOL			m_bWPN;
	BOOL			m_bDisconnected;
	BOOL	        m_bAway;
	BOOL		    m_bIdleAway;
	CString			m_strAway;
	CString			m_strPMAway;
	CString		    m_strAwayPrefix;
	CString			m_strAwaySuffix;
	CString			m_strAwayNickBck;
	UINT			m_uAwayStart;
	BOOL			m_bQuickJoin;
	BOOL			m_bUpdate;
	DWORD			m_dwLastActivity;
	// 
	// mdofied: of course this need to be volatile ;)
	// volatile is programmers best friend - right after coffee :D
	volatile  int	m_nUnreadPMs;
	CArray<CWnd*, CWnd*>m_aWPNListeners;

	// Initialization
	CEvent		m_eStartupDone;
	CWinThread* m_pThread;
	static UINT StartupThread(LPVOID lpParam);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Controls that need to be puclic accessible
	CDocSelector	m_wndDocSelector;

	//	Set the ini section where the state info to be saved/loaded from
	void SetProfileHeading(LPCTSTR szHeading){ 
		
		m_frameManager.SetProfileHeading(szHeading);
	}
	
	//	Specify whether positions of control bars should be saved/restored too.
	void SetManageBarStates(BOOL bManage){

		m_frameManager.SetManageBarStates(bManage);
	}

	virtual	BOOL ShowWindow(int nCmdShow);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CFrameManager	m_frameManager;
	
protected:  
	
	// other controls
	CColorStatusBar  m_wndStatusBar;
	CToolBar		 m_wndToolBarStd;
	CMenu           m_cmSystray;
	
	// Attributes
	// used for full-screen mode
	BOOL	  m_bFullScreenMode;
	//BOOL	  m_bStatusBarWasVisible;
	//BOOL	  m_bToolBarWasVisible;
	CRect	  m_mainRect;
	CToolBar* m_pwndFullScreenBar;
	bool	  m_bChildMax;
	
	BOOL	  m_bMinimized;
	BOOL	  m_bMuted;
	//CArray<HINSTANCE, HINSTANCE> m_aMods;
	CFont m_fFont;
	HICON m_hIcon;
	HICON m_hIcon2;
	HICON m_hPM1;
	HICON m_hPM2;
	HICON m_hConnect;
	HICON m_hDisconnect;
	int   m_nIcon;
	int   m_nMaxAni;
	int   m_nPMAni;
	NOTIFYICONDATA m_nIconData;
	NOTIFYICONDATA m_nPMIcon;

	void FullScreenModeOn();
	void FullScreenModeOff();
	void ConnectWPN(void);

	// Message Mapping
	afx_msg LRESULT OnLoadSettings(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetAway(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetBack(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMute();
	afx_msg void OnUpdateMute(CCmdUI *pCmdUI);
	afx_msg void OnClose();
	afx_msg LRESULT OnAddListener(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRemoveListener(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBrowse(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReBrowse(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnNewPM(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewOptions();
	afx_msg void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnUpdateChannelChannellist(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewChannelBar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWPNRefresh(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWPNConnect(CCmdUI* pCmdUI);
	afx_msg void OnFileNew();
	afx_msg void OnViewFullScreen();
	afx_msg void OnUpdateViewFullScreen(CCmdUI* pCmdUI);
	afx_msg void OnIdsScnextwindow();
	afx_msg void OnViewChannelbar();
	afx_msg LRESULT OnPluginQueryInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPluginSendMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPluginInput(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPluginEcho(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPluginSysEcho(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTooltip(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChannelTooltip(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFinishStart(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWhois(WPARAM wParam, LPARAM lParam);
	afx_msg void OnWPNConnect();
	afx_msg void OnWPNRefresh();
	afx_msg void OnSystrayRestore();
	afx_msg void OnReconnectAll();
	afx_msg void OnRename();
	afx_msg void OnViewIgnoredusers();
	afx_msg void OnViewTrustedusers();
	afx_msg void OnViewAutojoinmanager();
	afx_msg void OnHelpVisithomepage();
	afx_msg void OnCtrltab();
	afx_msg void OnCtrlshifttab();
	afx_msg void OnHelpVisitonlineforum();
	afx_msg void OnHelpSubscribetomailinglist();
	afx_msg void OnHelpLincense();
	afx_msg void OnHelpChangelog();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__4EE0D859_D847_4259_AA7D_531CB4E1928B__INCLUDED_)
