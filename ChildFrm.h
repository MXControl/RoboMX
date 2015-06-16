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


#if !defined(AFX_CHILDFRM_H__D67C8487_2E21_4707_B709_6EEC1347609F__INCLUDED_)
#define AFX_CHILDFRM_H__D67C8487_2E21_4707_B709_6EEC1347609F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FrameManager.h"

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:

	//	Set the ini section where the state info to be saved/loaded from
	void SetProfileHeading(LPCTSTR szHeading){

		m_frameManager.SetProfileHeading(szHeading);
	}
	
	//	Specify whether positions of control bars should be saved/restored too.
	void SetManageBarStates(BOOL bManage){

		m_frameManager.SetManageBarStates(bManage);
	}

	CFrameManager	m_frameManager;
	
// Generated message map functions
protected:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd*, CWnd*);
	afx_msg LRESULT OnSetMessageString(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__D67C8487_2E21_4707_B709_6EEC1347609F__INCLUDED_)
