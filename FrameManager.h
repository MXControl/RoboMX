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

class CFrameManager
{
public:
	CFrameManager(CFrameWnd* pWnd = NULL, LPCTSTR szHeading = NULL, BOOL bBarStates = FALSE);
	virtual ~CFrameManager(void);

	virtual	void	Load(BOOL bUseWindowTitle = FALSE);
	virtual	void	Save();

	UINT GetCmdShow(){ return m_nCmdShow; }

	LPWINDOWPLACEMENT GetWindowsPlacement() { return &m_wndpl;}

	void SetWindow(CFrameWnd* pWnd){ m_pWnd = pWnd;	}

	void SetProfileHeading(LPCTSTR szHeading){ m_profileHeading = szHeading; }

	void SetManageBarStates(BOOL bManage){ m_bManageBarStates = bManage;  }

private:
	CFrameWnd*		m_pWnd;
	UINT			m_nCmdShow;
    WINDOWPLACEMENT	m_wndpl;

	CString		m_profileHeading;			//	The assigned Profile Heading
	BOOL		m_bManageBarStates;			//	True if we should manage bar states too
	BOOL		m_bLoaded;					//  True if the position has already been loaded;

    static TCHAR	s_profileHeading[];		//	Default profile heading
    static TCHAR	s_profileRect[];
    static TCHAR	s_profileIcon[];
    static TCHAR	s_profileMax[];
	static TCHAR	s_profileBarState[];
};
