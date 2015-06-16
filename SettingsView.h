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

#if !defined(AFX_SETTINGSVIEW_H__F523F13F_A0C7_4D12_B86F_D4475D140E65__INCLUDED_)
#define AFX_SETTINGSVIEW_H__F523F13F_A0C7_4D12_B86F_D4475D140E65__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSettingsViewh : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSettingsView view

#include "ColorStatusBar.h"
#include "CfgDlg.h"
#include "CfgGeneral.h"
#include "CfgWelcome.h"
#include "ColorCfg.h"
#include "CfgFormat.h"
#include "MessageCfg.h"
#include "SfxCfg.h"
#include "CfgSounds.h"
#include "FunCfg.h"
#include "ExtCfg.h"
#include "CfgQuickMsg.h"
#include "CfgChannelBar.h"
#include "CfgSelector.h"
#include "CfgUserTool.h"
#include "CfgEmoticons.h"
#include "CfgUserlist.h"
#include "WPNSetup.h"
#include "CfgAway.h"
#include "afxwin.h"

class CSettingsView : public CFormView
{
protected:
	CSettingsView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSettingsView)

// Attributes
public:
	enum { IDD = IDD_SETTINGS };
	void AddCategory(CCfgDlg* pCat); //, int nICon, UINT nID);

// Operations
public:
	CColorStatusBar*	m_pStatusBar;

protected:
	virtual void OnInitialUpdate();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void DoDataExchange(CDataExchange* pDX);

// Implementation
protected:
	virtual ~CSettingsView();

	BOOL m_bExpert;
	CButton m_btOK;
	CButton m_btApply;
	CButton m_btCancel;
	//CPtrArray m_aDlgs;
	CMap<CString, LPCTSTR, CCfgDlg*, CCfgDlg*> m_aDlgs;
	CCfgSelector	m_lcCat;
	CImageList m_cImageList;
	CButton m_btExpert;

	CCfgUserTool	cfgUserTool;
	CCfgEmoticons	cfgEmo;
	CCfgFormat		cfgFormat;
	CCfgColor		cfgColor;
	CCfgQuickMsg	cfgQuick;
	CCfgGeneral		cfgGeneral;
	CCfgWelcome		cfgWelcome;
	CCfgMessage		cfgMsg;
	CExtCfg			cfgExt;
	CSfxCfg			cfgSfx;
	CCfgSounds		cfgChatSfx;
	CCfgFun			cfgFun;
	CCfgWPN			cfgWPN;
	CCfgChannelBar	cfgBar;
	CCfgUserlist    cfgUserList;
	CCfgAway		cfgAway;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	void Layout(void);

	virtual void OnOK();
	afx_msg void OnApply();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnItemchangedCat(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void NotifyDisplayTooltip(NMHDR * pNMHDR, LRESULT * result);
	afx_msg void OnBnClickedExpert();
	DECLARE_MESSAGE_MAP()
};


#endif // !defined(AFX_SETTINGSVIEW_H__F523F13F_A0C7_4D12_B86F_D4475D140E65__INCLUDED_)
