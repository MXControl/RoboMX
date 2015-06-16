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

#if !defined(AFX_CHANNELVIEW_H__F523F13F_A0C7_4D12_B86F_D4475D140E65__INCLUDED_)
#define AFX_CHANNELVIEW_H__F523F13F_A0C7_4D12_B86F_D4475D140E65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChannelView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChannelView view

//#include "ListClient.h"
#include "WPNPClient.h"
#include "ColorStatusBar.h"
#include "MyListCtrl.h"
#include "afxcmn.h"
#include "TimeStatus.h"
#include "myedit.h"
#include "PPTooltip.h"


typedef struct TAG_CHATROOMINFOBUFFER {

	WORD	wUsers;
	WORD	wLimit;
	CString strRoomName;
	CString strTopic;
} CHATROOMINFOBUFFER, *PCHATROOMINFOBUFFER;

class CChannelView : public CFormView
{
protected:
	CChannelView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CChannelView)

// Attributes
public:
	//{{AFX_DATA(CChannelView)
	enum { IDD = IDD_CHANNELLIST };
	CMyListCtrl m_lcList;
	CString   m_strSearch;
	CString   m_strMsg;
	BOOL m_bNoScroll;
	//}}AFX_DATA

	BOOL m_bFilter;
	DWORD m_dwFiltered;
	float m_fTotal;
	BOOL m_bHilite;
	int  m_nCached;
	CImageList m_iImageList;
	CProgressCtrl m_pgStatus;
	int 		  m_nPerc;
	CTimeStatus   m_tTime;
	BOOL          m_bInit;
	BOOL		  m_bCache;
	CFont		  m_fFont;
	CMyEdit m_eSearch;
	DWORD m_dwWinMX;
	DWORD m_dwRose;
	DWORD m_dwWCS;
	DWORD m_dwMCS;
	DWORD m_dwMCMA;
	DWORD m_dwRCMS;
	DWORD m_dwFXServ;
	DWORD m_dwTotalUsers;
	CPPToolTip		m_ToolTip;
public:

	void UpdateStats(const CString strTopic)
	{

		if(strTopic.Find("[roSe") == 0){

			m_dwRose++;
		}
		else if(strTopic.Find("[WCS]") == 0){

			m_dwWCS++;
		}
		else if(strTopic.Find("[MCS]") == 0){

			m_dwMCS++;
		}
		else if(strTopic.Find("[MCMA]") == 0){

			m_dwMCMA++;
		}
		else if(strTopic.Find("[RCMS]") == 0){

			m_dwRCMS++;
		}
		else if(strTopic.Find("[FXServ]") == 0){

			m_dwFXServ++;
		}
		else{

			m_dwWinMX++;
		}
	}

// Operations
public:
	void UpdateRoomItem(LPTSTR lpszRoomName, WORD wUsers, WORD wLimit, LPTSTR lpszTopic);
	afx_msg LRESULT WPNNotify(WPARAM wParam, LPARAM lParam);

	int					m_nLastItem;
	CColorStatusBar*	m_pStatusBar;
	CArray<CHATROOMINFOBUFFER, CHATROOMINFOBUFFER> m_aRoomBuffer;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChannelView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CChannelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

protected:
	void LoadCache(void);
	void SaveCache(void);
	BOOL IsFavorite(const CString strRoom, CString& strLogin) const;
	void AddToAutoJoin(const CString strChannel, const CString strLogin);
	void WriteBufferToList(void);
	CString PingRoom(CString& strRoom, BOOL bInitPing = TRUE);

	// Generated message map functions
protected:
	afx_msg LRESULT OnPluginQueryInfo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReloadCfg(WPARAM w, LPARAM l);
	afx_msg void OnRefresh();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnClearRefresh();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeSearch();
	afx_msg void OnItemchangedChannels(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickChannels(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkChannels(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelectionNoscroll();
	afx_msg void OnPopupLoopbackjoin();
	afx_msg void OnPopupCopytopic();
	afx_msg void OnPopupCopyroomname();
	afx_msg void OnPopupPingroom();
	afx_msg void OnChannelsFilter();
	afx_msg void OnUpdateChannelsFilter(CCmdUI *pCmdUI);
	afx_msg void OnChannelsEditfilter();
	afx_msg void OnUpdateSelectionNoscroll(CCmdUI *pCmdUI);
	afx_msg void OnConfigurecachingCacheroomlist();
	afx_msg void OnUpdateConfigurecachingCacheroomlist(CCmdUI *pCmdUI);
	afx_msg void OnAdcancedClearCache();
	afx_msg void OnAdvancedRemovefromcache();
	afx_msg void OnAdvancedRemovecachedflag();
	afx_msg void OnUpdateAdvancedRemovefromcache(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAdvancedRemovecachedflag(CCmdUI *pCmdUI);
	afx_msg void OnAdvancedManuallyaddroomtocache();
	afx_msg void OnListLockfocusonselection();
	afx_msg void OnListHighlight();
	afx_msg void OnUpdateListHighlight(CCmdUI *pCmdUI);
	afx_msg void OnListEdithighlightkeywords();
	afx_msg void OnUpdatePopupLoopbackjoin(CCmdUI *pCmdUI);
	afx_msg void OnMyfileNew();
	afx_msg void OnListAddtofavorites();
	afx_msg void OnUpdateListAddtofavorites(CCmdUI *pCmdUI);
	afx_msg void OnListRemovefromfavorites();
	afx_msg void OnUpdateListRemovefromfavorites(CCmdUI *pCmdUI);
	afx_msg void OnListGroupfavoritesattopoflist();
	afx_msg void OnUpdateListGroupfavoritesattopoflist(CCmdUI *pCmdUI);
	afx_msg void OnListCacheonlyfavorites();
	afx_msg void OnUpdateListCacheonlyfavorites(CCmdUI *pCmdUI);
	afx_msg void OnPopupEditfavoriteentry();
	afx_msg void OnUpdatePopupEditfavoriteentry(CCmdUI *pCmdUI);
	afx_msg void NotifyDisplayTooltip(NMHDR * pNMHDR, LRESULT * result);
	afx_msg void OnListShowstatistics();
	afx_msg void OnListCopystatistics();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeleteChannel();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANNELVIEW_H__F523F13F_A0C7_4D12_B86F_D4475D140E65__INCLUDED_)
