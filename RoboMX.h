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

#if !defined(AFX_ROBOMX_H__25DFE5AD_544B_4E6F_89A8_1C06FF7D7318__INCLUDED_)
#define AFX_ROBOMX_H__25DFE5AD_544B_4E6F_89A8_1C06FF7D7318__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRoboMXApp:
// See RoboMX.cpp for the implementation of this class
//
class CChatView;
class CMainFrame;

class CRoboMXApp : public CWinApp
{
public:
	void ApplyPic(void);
	CString m_strAppTitle;
	void SetRandomAppTitle();
	CRoboMXApp();

	inline CMainFrame* GetMainFrame(){

		ASSERT(m_pMainWnd);
		return (CMainFrame*)m_pMainWnd;
	}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRoboMXApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
	
	int    m_nWSA;
	BOOL   m_bPicLoaded;
	CImage m_iBgImage;
	CFont				m_gdiFont;
	WNDPROC m_pOldWndProc;
	static LRESULT CALLBACK NewWndProc(HWND hwnd, UINT uMsg, WPARAM wParam,LPARAM lParam);

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

inline CRoboMXApp *GetApp()
{
	return (CRoboMXApp *)AfxGetApp();
}


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROBOMX_H__25DFE5AD_544B_4E6F_89A8_1C06FF7D7318__INCLUDED_)
