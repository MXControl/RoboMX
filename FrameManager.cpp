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
#include ".\framemanager.h"
#include "settings.h"
#include "ini.h"
#include "MyDocument.h"

extern CSettings g_sSettings;

TCHAR CFrameManager::s_profileHeading[]		= _T("Window size");
TCHAR CFrameManager::s_profileRect[]		= _T("Rectangle");
TCHAR CFrameManager::s_profileIcon[]		= _T("Iconized");
TCHAR CFrameManager::s_profileMax[]			= _T("Maximized");
TCHAR CFrameManager::s_profileBarState[]	= _T("BarState");

CFrameManager::CFrameManager(CFrameWnd* pWnd, LPCTSTR szHeading, BOOL bBarStates)
{

	m_pWnd = pWnd;
	m_bLoaded = FALSE;

	m_wndpl.length = sizeof(WINDOWPLACEMENT);
	
	if(szHeading != NULL){

		SetProfileHeading(szHeading);
	}
	else{

		SetProfileHeading(s_profileHeading);
	}

	m_bManageBarStates = bBarStates;
}

CFrameManager::~CFrameManager(void)
{
}

void CFrameManager::Load(BOOL bUseWindowTitle)
{

	ASSERT(m_pWnd != NULL);

	if(m_bLoaded) return;

	if(bUseWindowTitle){

		CString strOut, strTitle, strType;
		strTitle = m_pWnd->GetActiveDocument()->GetTitle();
		strType     = ((CMyDocument*)m_pWnd->GetActiveDocument())->GetDocumentString();

		if(!strTitle.IsEmpty()){

			int nIndex = strTitle.Find(" ", 1);
			if(nIndex > 0){

				strTitle = strTitle.Left(nIndex);
			}
			strOut.Format("%s_%s", strType, strTitle);
			SetProfileHeading(strOut);
		}
	}

    WINDOWPLACEMENT	m_wndpl;
	CString			strText;

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	BOOL	bRet = m_pWnd->GetWindowPlacement(&m_wndpl);
	CRect	rect = m_wndpl.rcNormalPosition;

	strText = ini.GetValue(m_profileHeading, s_profileRect, "");

	if(! strText.IsEmpty()){

		rect.left	= _ttoi((const char*) strText.Mid(0,4));
		rect.top	= _ttoi((const char*) strText.Mid(5,4));
		rect.right	= _ttoi((const char*) strText.Mid(10,4));
		rect.bottom = _ttoi((const char*) strText.Mid(15,4));
	}

	BOOL	bIconic = ini.GetValue(m_profileHeading, s_profileIcon, 0);
	BOOL	bMaximized = ini.GetValue(m_profileHeading, s_profileMax, 0);   
    UINT flags;

	if(bIconic){

		m_nCmdShow = SW_SHOWMINNOACTIVE;
		if(bMaximized){

			flags = WPF_RESTORETOMAXIMIZED;
		}
		else{

			flags = WPF_SETMINPOSITION;
		}
	}
	else{

		if(bMaximized){

			m_nCmdShow = SW_SHOWMAXIMIZED;
			flags = WPF_RESTORETOMAXIMIZED;
		}
		else{

			m_nCmdShow = SW_NORMAL;
			flags = WPF_SETMINPOSITION;
		}
	}

	//	Make the window have new settings
	m_wndpl.showCmd = m_nCmdShow;
	m_wndpl.flags = flags;
	m_wndpl.ptMinPosition = CPoint(0, 0);
	m_wndpl.ptMaxPosition = CPoint(-::GetSystemMetrics(SM_CXBORDER),
									-::GetSystemMetrics(SM_CYBORDER));
	m_wndpl.rcNormalPosition = rect;

	m_pWnd->SetWindowPlacement(&m_wndpl);

	if(m_bManageBarStates){

		CString	cTmp;
		cTmp.Format("%s\\%s", m_profileHeading, s_profileBarState);
		m_pWnd->LoadBarState(cTmp);
	}

	m_bLoaded = TRUE;
}

void CFrameManager::Save()
{

	ASSERT(m_pWnd != NULL);

	CString			strText;
	BOOL			bIconic = FALSE;
	BOOL			bMaximized = FALSE;

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	BOOL	bRet = m_pWnd->GetWindowPlacement(&m_wndpl);
	if(m_wndpl.showCmd == SW_SHOWNORMAL){

		bIconic = FALSE;
		bMaximized = FALSE;
	}
	else if(m_wndpl.showCmd == SW_SHOWMAXIMIZED){

		bIconic = FALSE;
		bMaximized = TRUE;
	} 
	else if(m_wndpl.showCmd == SW_SHOWMINIMIZED){

		bIconic = TRUE;
		if(m_wndpl.flags){

			bMaximized = TRUE;
		}
		else{

			bMaximized = FALSE;
		}
	}
	
	//	Save the state into Registry
	strText.Format("%04d %04d %04d %04d",
			m_wndpl.rcNormalPosition.left,
			m_wndpl.rcNormalPosition.top,
			m_wndpl.rcNormalPosition.right,
			m_wndpl.rcNormalPosition.bottom);
	ini.SetValue(m_profileHeading, s_profileRect, strText);
	ini.SetValue(m_profileHeading, s_profileIcon, bIconic);
	ini.SetValue(m_profileHeading, s_profileMax, bMaximized);

	if(m_bManageBarStates){

		CString	cTmp;
		cTmp.Format("%s\\%s", m_profileHeading, s_profileBarState);
		m_pWnd->SaveBarState(cTmp);
	}
}

