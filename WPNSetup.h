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

#include "CfgDlg.h"

// CCfgWPN dialog

class CCfgWPN : public CCfgDlg
{
	DECLARE_DYNAMIC(CCfgWPN)

public:
	CCfgWPN(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCfgWPN();
	void Save();
	void Load();
	void Apply();

// Dialog Data
	enum { IDD = IDD_CFG_WPN };
	CEdit   m_eNodeName;
	CEdit   m_eExternal;
	CString m_strNodeName;
	CString m_strExternal;
	CButton m_btAutoNode;
	CButton m_btForcedNode;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGetNode();
	afx_msg void OnBnClickedOwnNode();
	afx_msg void OnBnClickedEditList();
	CString m_strPrivacy;
	CString m_strIgnored;
	afx_msg void OnBnClickedEditpcs();
};
