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

#if !defined(AFX_MESSAGECFG_H__3A3A53D8_B4E9_468F_816D_55865AC11A75__INCLUDED_)
#define AFX_MESSAGECFG_H__3A3A53D8_B4E9_468F_816D_55865AC11A75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCfgMessage dialog
#include "CfgDlg.h"
//#include "afxwin.h"

class CCfgMessage : public CCfgDlg
{
// Construction
public:
	CCfgMessage(CWnd* pParent = NULL);   // standard constructor
	void TestPlugin(void);
	void Apply();

// Dialog Data
	//{{AFX_DATA(CCfgMessage)
	enum { IDD = IDD_CFG_MESSAGES };
	CString	m_strJoin;
	CString	m_strWatch;
	CString	m_strListen;
	CString	m_strPart;
	CStatic m_wndWAPlugin;
	CButton m_wndInstall;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCfgMessage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	CPPToolTip	m_mToolTip;
	// Generated message map functions
	//{{AFX_MSG(CCfgMessage)
	afx_msg void OnEnChangeJoin();
	afx_msg void OnEnChangePart();
	afx_msg void OnEnChangeNowlistening();
	afx_msg void OnEnChangeNowWatching();
	afx_msg void OnBnClickedInstall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MESSAGECFG_H__3A3A53D8_B4E9_468F_816D_55865AC11A75__INCLUDED_)
