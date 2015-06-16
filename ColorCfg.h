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

#if !defined(AFX_COLORCFG_H__83E03A95_C63A_4FC9_9938_C2CE47CCD964__INCLUDED_)
#define AFX_COLORCFG_H__83E03A95_C63A_4FC9_9938_C2CE47CCD964__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCfgColor dialog

#include "CfgDlg.h"
#include "ColorStatic.h"
#include "afxwin.h"

class CCfgColor : public CCfgDlg
{
// Construction
public:
	CCfgColor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCfgColor)
	enum { IDD = IDD_CFG_COLORS };
	CColorStatic	m_stHiLite;
	CColorStatic	m_stHiLiteTxt;
	CColorStatic	m_stGrid;
	CColorStatic	m_stGridTxt;
	CColorStatic	m_stFocus;
	CColorStatic	m_stPend;
	CColorStatic	m_stOK;
	CColorStatic	m_stErr;
	CColorStatic	m_stLine;
	CColorStatic	m_stFiles;
	CColorStatic	m_stIP;
	CColorStatic	m_stPort;
	CColorStatic	m_stBg;
	CColorStatic	m_stMsg;
	CColorStatic	m_stMsgEnc;
	CColorStatic	m_stJoin;
	CColorStatic	m_stMotd;
	CColorStatic	m_stPart;
	CColorStatic	m_stTopic;
	CColorStatic	m_stActEnc;
	CColorStatic	m_stAction;
	CColorStatic	m_stDocHi;
	CColorStatic	m_stOpMsg;
	CColorStatic	m_stLink;
	CColorStatic	m_stName;
	CColorStatic	m_stTime;
	CString	m_strActionEnd;
	CString	m_strActionFront;
	CString	m_strMsgEnd;
	CString	m_strMsgFront;
	BOOL    m_bBlockMCMA;
	BOOL	m_bFocus;
	//}}AFX_DATA
	void Apply();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCfgColor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCfgColor)
	afx_msg void OnEncMsgColor();
	afx_msg void OnColorMessage();
	afx_msg void OnEncActionColor();
	afx_msg void OnColorAction();
	afx_msg void OnTopic();
	afx_msg void OnMotd();
	afx_msg void OnJoin();
	afx_msg void OnPart();
	afx_msg void OnBackg();
	afx_msg void OnLine();
	afx_msg void OnFiles();
	afx_msg void OnNodeip();
	afx_msg void OnPort();
	afx_msg void OnPending();
	afx_msg void OnSuccess();
	afx_msg void OnError();
	afx_msg void OnColorfocus();
	afx_msg void OnBnClickedTime();
	afx_msg void OnBnClickedHilite();
	afx_msg void OnBnClickedEdithilite();
	afx_msg void OnBnClickedDochi();
	afx_msg void OnBnClickedOpmsg();
	afx_msg void OnBnClickedLightDark();
	afx_msg void OnBnClickedDarkLight();
	afx_msg void OnBnClickedWinmxLike();
	afx_msg void OnBnClickedHiliteTxt();
	afx_msg void OnBnClickedGridBg();
	afx_msg void OnBnClickedGridTxt();
	afx_msg void OnBnClickedColorName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLink();
	afx_msg void OnBnClickedBlue();
	afx_msg void OnBnClickedImport();
	afx_msg void OnBnClickedExport();
	afx_msg void OnBnClickedBlockmcma();
	CButton m_btRawname;
	BOOL m_bRawname;
	afx_msg void OnBnClickedMcmaraw();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORCFG_H__83E03A95_C63A_4FC9_9938_C2CE47CCD964__INCLUDED_)
