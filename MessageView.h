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

#pragma once

#include "RichEditExCtrl.h"
#include "RichDocument.h"
#include "MyEdit.h"
//#include "ImageEx.h"
#include "ColorStatusBar.h"
#include "afxwin.h"

// CMessageView form view
class CMessageDoc;

class CMessageView : public CFormView
{

	DECLARE_DYNCREATE(CMessageView)

protected:
	CMessageView();           // protected constructor used by dynamic creation
	virtual ~CMessageView();

	CButton m_btOK;
	CButton m_btCancel;
	CButton m_btKeep;
	//ImageEx *m_iAni;
	DWORD	m_nWait;

	CColorStatusBar*	 m_pStatusBar;

	CMyEdit m_eInput;
	CRichEditExCtrl m_rOut;
	CRichDocument   m_rDoc;
	CString m_strText;
	CString m_strAwayMsg;
	CString m_strSelText;
	BOOL	m_bKeep;
	BOOL    m_bMsgPending;

	CStatic m_wndAni;
	int     m_nAway;
	CFont	m_fFont;
	CFont   m_fBold, m_fUnderline, m_fItalic;
	BOOL    m_bUnread;
	CButton m_btBold;
	CButton m_btUnderline;
	CButton m_btItalic;
	CButton m_btColor;
	CStatic m_wndNotics;
	DWORD  m_dwMsgCount;
	DWORD m_dwMsgSentCount;
	//CRITICAL_SECTION m_AniLock;

public:
	enum { IDD = IDD_MESSAGE_VIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CMessageDoc* GetDocument();

protected:
	void CheckAway(void);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

	void WriteOutgoing(CString strText);
	void WriteIncoming(CString strText);
	void Layout(void);
	void PMAck(BOOL bOK = TRUE);
	//void SetAni(CString strName);

	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnReloadCfg(WPARAM w, LPARAM l);
	afx_msg LRESULT OnWPNNotify(WPARAM w, LPARAM l);
	afx_msg void OnActionsReloadfiles();
	afx_msg void OnActionsSetawaymessage();
	afx_msg void OnActionsIgnoreuser();
	afx_msg void OnActionsTrustuser();
	afx_msg void OnShiftenter();
	afx_msg void OnChatroomDisplayinchannelWinampsong();
	afx_msg void OnUpdateChatroomDisplayinchannelWinampsong(CCmdUI *pCmdUI);
	afx_msg void OnDisplayinchannelOnlinetime();
	afx_msg void OnChatroomDisplayinchannelSystemuptime();
	afx_msg void OnChatroomDisplayinchannelSysteminfo();
	afx_msg void OnDisplayinchannelExtendedsysteminfo();
	afx_msg void OnRclickMsgs(RVN_ELEMENTEVENT* pNotify, LRESULT *pResult);
	afx_msg void OnOpenURL();
	afx_msg void OnOpenURLinnewbrowser();
	afx_msg void OnCopyAddress();
	afx_msg void NotifyDisplayTooltip(NMHDR * pNMHDR, LRESULT * result);
	afx_msg void OnBnClickedBold();
	afx_msg void OnBnClickedUnderline();
	afx_msg void OnBnClickedItalic();
	afx_msg void OnBnClickedColor();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RoboMXView.cpp
inline CMessageDoc* CMessageView::GetDocument()
   { return (CMessageDoc*)m_pDocument; }
#endif

