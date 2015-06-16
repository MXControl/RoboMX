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
#include "mylistctrl.h"
#include "afxwin.h"
#include "ColorStatusBar.h"
#include "MyEdit.h"

// CBrowseView form view
class CBrowseDoc;

typedef struct TAG_BROWSEITEM {

	CString strPath;
	CString strFile;
	CString strType;
	CString strSize;
	CString strBitrate;
	CString strSamplerate;
	CString strLength;
	int     nIcon;
}  BROWSEITEM, *PBROWSEITEM;


typedef CArray<BROWSEITEM, BROWSEITEM> CBrowse;

class CBrowseView : public CFormView
{
	DECLARE_DYNCREATE(CBrowseView)

protected:
	CBrowseView();           // protected constructor used by dynamic creation
	virtual ~CBrowseView();

public:
	enum { IDD = IDD_BROWSE_VIEW };
	CMyListCtrl m_lcFiles;
	CMyEdit     m_eSearch;
 	
	//void SetAni(CString strName);
	void Layout(void);

	//CRITICAL_SECTION m_AniLock;

	//ImageEx*		 m_iAni;
	CStatic m_wndStat;
	CColorStatusBar* m_pStatusBar;
	CFont			 m_fFont;
	CStatic			 m_stAni;
	CString			 m_strSearch;
	CBrowse			 m_aBrowse;
	BOOL			 m_bFullPath;
	CButton			 m_btCheck;
	BOOL			 m_bWorking;
	int				m_nAsciiAni;
	DWORD           m_dwNumAudio;
	DWORD			m_dwNumVideo;
	DWORD			m_dwNumImg;
	DWORD			m_dwNumOther;

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	CBrowseDoc* GetDocument();
	CImageList	m_iImageList;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate();
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

	afx_msg void OnEnChangeSearch();
	afx_msg void OnBrowseCopyfilepath();
	afx_msg void OnBrowseCopyfilename();
	afx_msg void OnNMRclickFileList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBrowseCopyfileinformation();
	afx_msg void OnBnClickedPath();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnReloadCfg(WPARAM w, LPARAM l);
	afx_msg LRESULT OnWPNNotify(WPARAM w, LPARAM l);
	afx_msg void OnDestroy();
	afx_msg void OnActionsReloadfiles();
	afx_msg void OnActionsSendprivatemessage();
	afx_msg void OnUpdateActionsReloadfiles(CCmdUI *pCmdUI);
 	afx_msg void NotifyDisplayTooltip(NMHDR * pNMHDR, LRESULT * result);
   DECLARE_MESSAGE_MAP()
};
 
#ifndef _DEBUG  // debug version in RoboMXView.cpp
inline CBrowseDoc* CBrowseView::GetDocument()
   { return (CBrowseDoc*)m_pDocument; }
#endif

