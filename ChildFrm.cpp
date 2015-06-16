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
#include "settings.h"
#include "ChildFrm.h"
#include "ColorStatusBar.h"
#include "SystemInfo.h"

extern CSystemInfo g_sSystem;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSettings g_sSettings;
/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_MDIACTIVATE()
	ON_MESSAGE(WM_SETMESSAGESTRING, OnSetMessageString)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{

	m_frameManager.SetWindow(this);
	m_frameManager.SetProfileHeading("Child Windows");
}

CChildFrame::~CChildFrame()
{
}

void CChildFrame::OnDestroy()
{

	if(g_sSettings.GetSavePos()){

		m_frameManager.Save();
	}
	CMDIChildWnd::OnDestroy();
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{

	if(g_sSettings.GetMaxi() && !g_sSettings.GetSavePos()){

		cs.style = WS_CHILD | WS_VISIBLE | WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU
			| FWS_ADDTOTITLE | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE;;
	}

	//cs.style |= FWS_PREFIXTITLE;

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeact)
{

	if(g_sSettings.GetSavePos() && !g_sSettings.GetMaxi()){
	
		m_frameManager.Load(TRUE);
	}

	CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeact);
}

void CChildFrame::OnSysCommand(UINT nID, LPARAM lParam) 
{

	CMDIChildWnd::OnSysCommand(nID, lParam);

 	if((nID & 0xFFF0) == SC_MINIMIZE){

		this->ShowWindow(SW_HIDE);
	}
}

LRESULT CChildFrame::OnSetMessageString(WPARAM wParam, LPARAM lParam)
{

	// code below is taken from winfrm.cpp and modified to set the 
	// status text in pane 1 instead of pane 0 :-)

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
		((CColorStatusBar*)pMessageBar)->SetPaneText(1, lpsz);
		//m_wndStatusBar.SetPaneText(1, lpsz);

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

