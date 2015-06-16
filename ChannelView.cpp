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
#include "RoboMX.h"
#include "ChannelDoc.h"
#include "ChannelView.h"
#include "MainFrm.h"
#include "Settings.h"
#include "Clipboard.h"
#include "ping.h"
#include "Ini.h"
#include "FilterDlg.h"
#include "Tokenizer.h"
#include "AddCache.h"
#include "AddFavorite.h"
#include "rmx_qapi.h"
#include ".\channelview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSettings g_sSettings;
extern CArray<PLUGIN_DATA, PLUGIN_DATA> g_aPlugins;

extern UINT UWM_LOAD_SETTINGS;

/////////////////////////////////////////////////////////////////////////////
// CChannelView

IMPLEMENT_DYNCREATE(CChannelView, CFormView)

extern UINT UWM_WPNNOTIFY;
extern UINT UWM_ADDLISTENER;
extern UINT UWM_REMLISTENER;

CChannelView::CChannelView()
	: CFormView(CChannelView::IDD),
	m_bNoScroll(TRUE),
	m_nPerc(0),
	m_bInit(FALSE),
	m_strSearch(""),
	m_bFilter(TRUE),
	m_dwFiltered(0),
	m_fTotal(2000.f),
	m_bHilite(TRUE),
	m_bCache(TRUE),
	m_nCached(0),
	m_dwWinMX(0),
	m_dwRose(0),
	m_dwWCS(0),
	m_dwMCS(0),
	m_dwMCMA(0),
	m_dwRCMS(0),
	m_dwTotalUsers(0),
	m_dwFXServ(0)

{
}

CChannelView::~CChannelView()
{
	m_iImageList.DeleteImageList();
}

void CChannelView::DoDataExchange(CDataExchange* pDX) 
{

	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChannelView)
	DDX_Control(pDX, IDC_CHANNELS, m_lcList);
	DDX_Text(pDX, IDC_SEARCH, m_strSearch);
	DDX_Check(pDX, IDC_SELECTION_NOSCROLL, m_bNoScroll);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATUS, m_pgStatus);
	DDX_Control(pDX, IDC_SEARCH, m_eSearch);
}

BEGIN_MESSAGE_MAP(CChannelView, CFormView)
	//{{AFX_MSG_MAP(CChannelView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_SEARCH, OnChangeSearch)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CHANNELS, OnItemchangedChannels)
	ON_NOTIFY(NM_RCLICK, IDC_CHANNELS, OnRclickChannels)
	ON_NOTIFY(NM_DBLCLK, IDC_CHANNELS, OnDblclkChannels)
	ON_BN_CLICKED(IDC_REFRESH, OnRefresh)
	ON_BN_CLICKED(IDC_CLEAR_REFRESH, OnClearRefresh)
	ON_BN_CLICKED(IDC_SELECTION_NOSCROLL, OnSelectionNoscroll)
	ON_COMMAND(ID_POPUP_LOOPBACKJOIN, OnPopupLoopbackjoin)
	ON_COMMAND(ID_POPUP_COPYTOPIC, OnPopupCopytopic)
	ON_COMMAND(ID_POPUP_COPYROOMNAME, OnPopupCopyroomname)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_LIST_UPDATELIST, OnRefresh)
	ON_COMMAND(ID_LIST_RELOADLIST, OnClearRefresh)
	ON_REGISTERED_MESSAGE(UWM_LOAD_SETTINGS, OnReloadCfg)
	ON_REGISTERED_MESSAGE(UWM_WPNNOTIFY, WPNNotify)
	ON_COMMAND(ID_POPUP_PINGROOM, OnPopupPingroom)
	ON_COMMAND(ID_CHANNELS_FILTER, OnChannelsFilter)
	ON_UPDATE_COMMAND_UI(ID_CHANNELS_FILTER, OnUpdateChannelsFilter)
	ON_COMMAND(ID_CHANNELS_EDITFILTER, OnChannelsEditfilter)
	ON_UPDATE_COMMAND_UI(ID_LIST_LOCKFOCUSONSELECTION, OnUpdateSelectionNoscroll)
	ON_COMMAND(ID_LIST_LOCKFOCUSONSELECTION, OnListLockfocusonselection)
	ON_COMMAND(ID_LIST_HIGHLIGHT, OnListHighlight)
	ON_UPDATE_COMMAND_UI(ID_LIST_HIGHLIGHT, OnUpdateListHighlight)
	ON_COMMAND(ID_LIST_EDITHIGHLIGHTKEYWORDS, OnListEdithighlightkeywords)
	ON_UPDATE_COMMAND_UI(ID_POPUP_LOOPBACKJOIN, OnUpdatePopupLoopbackjoin)
	ON_COMMAND(ID_MYFILE_NEW, OnMyfileNew)
	ON_COMMAND(ID_CONFIGURECACHING_CACHEROOMLIST, OnConfigurecachingCacheroomlist)
	ON_UPDATE_COMMAND_UI(ID_CONFIGURECACHING_CACHEROOMLIST, OnUpdateConfigurecachingCacheroomlist)
	ON_COMMAND(ID_ADVANCED_CLEARCACHEDROOMS, OnAdcancedClearCache)
	ON_COMMAND(ID_ADVANCED_REMOVEFROMCACHE, OnAdvancedRemovefromcache)
	ON_COMMAND(ID_ADVANCED_REMOVECACHEDFLAG, OnAdvancedRemovecachedflag)
	ON_UPDATE_COMMAND_UI(ID_ADVANCED_REMOVEFROMCACHE, OnUpdateAdvancedRemovefromcache)
	ON_UPDATE_COMMAND_UI(ID_ADVANCED_REMOVECACHEDFLAG, OnUpdateAdvancedRemovecachedflag)
	ON_COMMAND(ID_ADVANCED_MANUALLYADDROOMTOCACHE, OnAdvancedManuallyaddroomtocache)
	ON_COMMAND(ID_LIST_ADDTOFAVORITES, OnListAddtofavorites)
	ON_UPDATE_COMMAND_UI(ID_LIST_ADDTOFAVORITES, OnUpdateListAddtofavorites)
	ON_COMMAND(ID_LIST_REMOVEFROMFAVORITES, OnListRemovefromfavorites)
	ON_UPDATE_COMMAND_UI(ID_LIST_REMOVEFROMFAVORITES, OnUpdateListRemovefromfavorites)
	ON_COMMAND(ID_LIST_GROUPFAVORITESATTOPOFLIST, OnListGroupfavoritesattopoflist)
	ON_UPDATE_COMMAND_UI(ID_LIST_GROUPFAVORITESATTOPOFLIST, OnUpdateListGroupfavoritesattopoflist)
	ON_COMMAND(ID_LIST_CACHEONLYFAVORITES, OnListCacheonlyfavorites)
	ON_UPDATE_COMMAND_UI(ID_LIST_CACHEONLYFAVORITES, OnUpdateListCacheonlyfavorites)
	ON_COMMAND(ID_POPUP_EDITFAVORITEENTRY, OnPopupEditfavoriteentry)
	ON_UPDATE_COMMAND_UI(ID_POPUP_EDITFAVORITEENTRY, OnUpdatePopupEditfavoriteentry)
	ON_COMMAND(ID_LIST_SHOWSTATISTICS, OnListShowstatistics)
	ON_COMMAND(ID_LIST_COPYSTATISTICS, OnListCopystatistics)
	ON_NOTIFY (UDM_TOOLTIP_DISPLAY, NULL, NotifyDisplayTooltip)
	ON_MESSAGE(WM_QAPI, OnPluginQueryInfo)
	ON_COMMAND(ID_DELETE_CHANNEL, OnDeleteChannel)
	ON_COMMAND(ID_RECONNECT, OnClearRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChannelView drawing

void CChannelView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CChannelView diagnostics

#ifdef _DEBUG
void CChannelView::AssertValid() const
{
	CView::AssertValid();
}

void CChannelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChannelView message handlers
#define TIMER_LOAD 10000

void CChannelView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
    GetParentFrame()->RecalcLayout();
    
	if(g_sSettings.GetMaxi() && !g_sSettings.GetSavePos()){

		if(!m_lcList.m_hWnd){

			return;
		}

		CRect	rcItem;
		CRect	rcParent;

		GetClientRect(rcParent);

		// resize userlist
		m_lcList.GetWindowRect(rcItem);
		ScreenToClient(rcItem);
		
		rcItem.right = rcParent.right - 1;
		rcItem.bottom = rcParent.bottom -1;

		m_lcList.MoveWindow(rcItem);
	}
	else{

		ResizeParentToFit(FALSE);
	}

	m_pStatusBar = (CColorStatusBar *)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	GetApp()->GetMainFrame()->m_wndDocSelector.AddButton(this, IDR_LIST);
	GetApp()->GetMainFrame()->m_dwChannelList = (DWORD)m_hWnd;

	m_fFont.CreateFont(15, 6, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_STRING_PRECIS, 
		CLIP_STROKE_PRECIS, PROOF_QUALITY, FF_DONTCARE, g_sSettings.GetFont());

	CBitmap		bitmap;
	bitmap.LoadMappedBitmap(IDB_CHANNELLIST);
	m_iImageList.Create(16, 16, ILC_COLORDDB|ILC_MASK, 4, 1);
	m_iImageList.Add(&bitmap, RGB(255,0,255));
	
	m_lcList.SetImageList(&m_iImageList, LVSIL_SMALL);
	m_lcList.SetHeadings(IDS_ROOMLIST);
	m_lcList.LoadColumnInfo();
	if(g_sSettings.GetUserUseOldTools()){

		m_lcList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_LABELTIP);
	}
	else{

		m_lcList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

		if(m_ToolTip.Create(this, TRUE)){

			m_ToolTip.SetNotify();
			m_ToolTip.EnableEscapeSequences(FALSE);
			m_ToolTip.SetEffectBk(g_sSettings.GetUserToolEffect(), g_sSettings.GetUserToolGran());
			m_ToolTip.SetBehaviour(PPTOOLTIP_MULTIPLE_SHOW);
			m_ToolTip.SetColorBk(g_sSettings.GetUserToolBg1(), g_sSettings.GetUserToolBg2(), g_sSettings.GetUserToolBg3());
			m_ToolTip.SetSize(CPPToolTip::PPTTSZ_MARGIN_ANCHOR, 0);
			m_ToolTip.SetSize(CPPToolTip::PPTTSZ_WIDTH_ANCHOR, 0);
			m_ToolTip.SetSize(CPPToolTip::PPTTSZ_HEIGHT_ANCHOR, 0);
			m_ToolTip.HideBorder();
			m_ToolTip.AddTool(&m_lcList);
		}
	}
	m_lcList.SetFont(&m_fFont);
	m_lcList.m_bChannelList = TRUE;

	m_lcList.SetColors(
		g_sSettings.GetRGBNormalMsg(),
		g_sSettings.GetRGBFiles(),
		g_sSettings.GetRGBLine(),
		g_sSettings.GetRGBNormalMsg(),
		g_sSettings.GetRGBIP()
		);
	m_lcList.SetBkColor(g_sSettings.GetRGBBg());
	m_lcList.SetHiLite(m_bHilite);

	m_bCache = g_sSettings.GetCacheRooms();
	LoadCache();

	m_eSearch.SetExtended();
	m_eSearch.SetBkColor(g_sSettings.GetRGBBg());
	m_eSearch.SetFont(&m_fFont);
	m_eSearch.Invalidate();
	m_eSearch.SetFocus();

	m_pgStatus.SetRange(0, 100);

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	m_bFilter = ini.GetValue("ChannelFilter", "Activate", TRUE);

	::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_ADDLISTENER, 0, (LPARAM)this);

	if(GetApp()->GetMainFrame()->m_bWPN){

		GetApp()->GetMainFrame()->m_mxClient.SendQueStatus(1, 1, 0);
		GetApp()->GetMainFrame()->m_mxClient.SendFormatMessage(0x238D, "D", 0);
		m_nLastItem = GetTickCount();
		SetTimer(TIMER_LOAD, 500, 0);
		m_tTime.ReInit();
		m_pgStatus.SetPos(0);
	}
	
	CString strTmp;
	strTmp.LoadString(IDS_CONNECTING);
	GetDlgItem(IDC_STATIC_ROOMS)->SetWindowText(strTmp);
}

void CChannelView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if(m_pStatusBar){

		CString strOut;
		strOut.LoadString(IDS_CHANNELLIST);
		m_pStatusBar->SetPaneText(2, strOut);
		CString strText;
		strText.Format(IDS_FILTEREDCHANNELS, m_lcList.GetItemCount(), m_dwFiltered, m_dwFiltered+m_lcList.GetItemCount());
		//m_pStatusBar->SetLagColor(0, strText);
		m_pStatusBar->SetPaneText(3, strText);
		m_pStatusBar->SetPaneText(4, "");
	}
}

LRESULT CChannelView::OnPluginQueryInfo(WPARAM wParam, LPARAM lParam)
{

	if(lParam == 0) return 1;

	EXT_MSG_STRUCT* msg = (EXT_MSG_STRUCT*)lParam;
	
	LRESULT lResult = 1;

	
	switch(wParam){

		case RMX_GETNUMCHANNELS:
			lResult = (LRESULT)m_lcList.GetItemCount();
			break;
		case RMX_GETNUMFILTERCHANNELS:
			lResult = (LRESULT)m_dwFiltered;
			break;
		case RMX_GETNUMWINMX:
			lResult = (LRESULT)m_dwWinMX;
			break;
		case RMX_GETNUMRCMS:
			lResult = (LRESULT)m_dwRCMS;
			break;
		case RMX_GETNUMROSE:
			lResult = (LRESULT)m_dwRose;
			break;
		case RMX_GETNUMMCMA:
			lResult = (LRESULT)m_dwMCMA;
			break;
		case RMX_GETNUMMCS:
			lResult = (LRESULT)m_dwMCS;
			break;
		case RMX_GETNUMWCS:
			lResult = (LRESULT)m_dwWCS;
			break;
		case RMX_GETNUMFXSERV:
			lResult = (LRESULT)m_dwFXServ;
			break;
		case RMX_GETNUMWPNUSERS:
			lResult = (LRESULT)m_dwTotalUsers;
			break;
		case RMX_LOADCHANNELS:
			lResult = TRUE;
			if(wParam == 1) OnClearRefresh();
			else OnRefresh();
			break;
	}

	return lResult;
}

LRESULT CChannelView::OnReloadCfg(WPARAM w, LPARAM l)
{


	m_fFont.DeleteObject();
	m_fFont.CreateFont(15, 6, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_STRING_PRECIS, 
		CLIP_STROKE_PRECIS, PROOF_QUALITY, FF_DONTCARE, g_sSettings.GetFont());

	m_lcList.SetColors(
		g_sSettings.GetRGBNormalMsg(),
		g_sSettings.GetRGBFiles(),
		g_sSettings.GetRGBLine(),
		g_sSettings.GetRGBIP(),
		g_sSettings.GetRGBPort()
		);
	m_lcList.SetBkColor(g_sSettings.GetRGBBg());
	m_lcList.SetFont(&m_fFont);
	m_lcList.Invalidate();

	m_eSearch.SetBkColor(g_sSettings.GetRGBBg());
	m_eSearch.SetFont(&m_fFont);
	m_eSearch.Invalidate();

	Invalidate();

	return 1;
}

void CChannelView::OnDestroy() 
{

	g_sSettings.SetCacheRooms(m_bCache);
	SaveCache();
	m_lcList.SaveColumnInfo();
	m_fFont.DeleteObject();
	m_bInit = FALSE;
	GetApp()->GetMainFrame()->m_wndDocSelector.RemoveButton(this);
	GetApp()->GetMainFrame()->m_dwChannelList = 0;
	::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_REMLISTENER, 0, (LPARAM)this);
	GetApp()->GetMainFrame()->m_bChannelList = FALSE;
	GetApp()->GetMainFrame()->m_strRoom.Empty();
	CFormView::OnDestroy();
}

void CChannelView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	if(!m_lcList.m_hWnd){

		return;
	}

	CRect	rcItem;
	CRect	rcParent;

	GetClientRect(rcParent);

	// resize userlist
	m_lcList.GetWindowRect(rcItem);
	ScreenToClient(rcItem);
	
	rcItem.right = rcParent.right - 1;
	rcItem.bottom = rcParent.bottom -1;
	m_lcList.MoveWindow(rcItem);
	int nWidth = m_lcList.GetColumnWidth(0);
	nWidth +=  m_lcList.GetColumnWidth(1);
	nWidth +=  m_lcList.GetColumnWidth(2);
	nWidth +=  m_lcList.GetColumnWidth(3);
	nWidth +=  m_lcList.GetColumnWidth(4);
	m_lcList.SetColumnWidth(5, rcParent.right > 540 ? rcParent.right - 20 - nWidth: 540 - nWidth);
	
	ShowScrollBar(SB_BOTH , FALSE);
}

void CChannelView::OnClearRefresh() 
{

	CWaitCursor wc;

	m_dwWinMX      = 0;
	m_dwRose       = 0;
	m_dwWCS        = 0;
	m_dwMCS        = 0;
	m_dwMCMA       = 0;
	m_dwRCMS	   = 0;
	m_dwTotalUsers = 0;
	m_dwFXServ     = 0;
	int nTotal     = m_lcList.GetItemCount();
	for(int i = 0; i < m_lcList.GetItemCount(); i++){

		if((m_lcList.GetItemMode(i) == 0)){

			m_lcList.DeleteItem(i);
			i--;
		}
		else{

			m_lcList.SetItemCached(i, TRUE);
		}
	}
	if(m_lcList.GetItemCount()){

		m_lcList.Sort();
	}

	wc.Restore();

	m_dwFiltered = 0;
	m_nLastItem = GetTickCount();
	m_fTotal		 = 2000.f;
	SetTimer(TIMER_LOAD, 500, 0);
	m_tTime.ReInit();
	m_pgStatus.SetPos(0);
	GetApp()->GetMainFrame()->m_mxClient.SendQueStatus(1, 1, 0);
	GetApp()->GetMainFrame()->m_mxClient.SendFormatMessage(0x238D, "D", 0);
}

void CChannelView::OnRefresh() 
{

	m_nLastItem		= GetTickCount();
	m_dwFiltered	= 0;
	m_fTotal		= 2000.f;
	SetTimer(TIMER_LOAD, 500, 0);
	m_tTime.ReInit();
	m_pgStatus.SetPos(0);

	((CMainFrame*)AfxGetMainWnd())->m_mxClient.SendQueStatus(1, 1, 0);
	((CMainFrame*)AfxGetMainWnd())->m_mxClient.SendFormatMessage(0x238D, "D", 0);
}

LRESULT CChannelView::WPNNotify(WPARAM wParam, LPARAM lParam)
{

	switch(wParam){

		case WPN_PARENTDISCONNECTED:
		case WPN_ERROR :
			break;

		case WPN_WPNCONNECTED :
			m_nLastItem = GetTickCount();
			GetApp()->GetMainFrame()->m_mxClient.SendFormatMessage(0x238D, "D", 0);
			SetTimer(TIMER_LOAD, 500, 0);
			m_tTime.ReInit();
			m_pgStatus.SetPos(0);
			return TRUE;

		case WPN_CHATROOMINFO: 
			CHATROOMINFO *pRoomInfo = (CHATROOMINFO *)lParam;
			UpdateRoomItem(pRoomInfo->lpszRoomName, (WORD)pRoomInfo->dwUsers, (WORD)pRoomInfo->dwLimit, pRoomInfo->lpszTopic);
			return TRUE;
		}

	return FALSE;
}

void CChannelView::UpdateRoomItem(LPTSTR lpszRoomName, WORD wUsers, WORD wLimit, LPTSTR lpszTopic)
{

	m_nLastItem = GetTickCount();
	
	
	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());
	int nFilters = ini.GetValue("ChannelFilter", "Num", 0);
	
	CString strFilter;

	for(int i = 0; (i < nFilters) && m_bFilter; i++){

		strFilter.Format("Filter_%d", i);
		strFilter = ini.GetValue("ChannelFilter", strFilter, "");
		if(strFilter.IsEmpty()) continue;

		if(Util::WildcardMatch(lpszRoomName, strFilter, TRUE, FALSE) || Util::WildcardMatch(lpszTopic, strFilter, TRUE, FALSE)){

			m_dwFiltered++;
			return;
		}

	}
	CHATROOMINFOBUFFER room;
	room.wLimit = wLimit;
	room.wUsers = wUsers;
	room.strRoomName = lpszRoomName;
	room.strTopic    = lpszTopic;

	m_aRoomBuffer.Add(room);

	if(m_aRoomBuffer.GetSize() >= 50){

		WriteBufferToList();
	}
}

void CChannelView::OnTimer(UINT nIDEvent) 
{

	if(nIDEvent == TIMER_LOAD){

		if(m_pStatusBar == NULL){

			m_pStatusBar = (CColorStatusBar *)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
		}

		m_bInit = TRUE;

		if((GetTickCount() - m_nLastItem) >= 30000){

			WriteBufferToList();
			CString strOut;
			strOut.LoadString(IDS_FINISHED);
			m_pStatusBar->SetPaneText(2, strOut);
			m_fTotal		 = (float)m_lcList.GetItemCount();
			m_pgStatus.SetPos(100);
			KillTimer(TIMER_LOAD);
		}
		
		CString strNum;
		m_nPerc = (int)(((float)m_lcList.GetItemCount()+m_dwFiltered-m_nCached) / m_fTotal * 100.f);
		m_pgStatus.SetPos(m_nPerc);
		m_tTime.Calculate(m_nPerc);
		strNum.Format(IDS_ROOMS_ETA, m_lcList.GetItemCount(), m_tTime.GetRemainingString(), m_tTime.GetElapsedString(), m_tTime.GetEstimateString());
		GetDlgItem(IDC_STATIC_ROOMS)->SetWindowText(strNum);

		if(m_pStatusBar){

			if(GetApp()->GetMainFrame()->GetActiveFrame() == GetParentFrame()){
				CString strOut;
				strOut.LoadString(IDS_CHANNELLIST);
				m_pStatusBar->SetPaneText(2, strOut);
				CString strText;
				strText.Format(IDS_FILTEREDCHANNELS, m_lcList.GetItemCount(), m_dwFiltered, m_dwFiltered+m_lcList.GetItemCount());
				m_pStatusBar->SetPaneText(3, strText);
				m_pStatusBar->SetPaneText(4, "");
			}
		}
	}
	CFormView::OnTimer(nIDEvent);
}


void CChannelView::OnChangeSearch() 
{

	UpdateData(TRUE);

	m_nLastItem = GetTickCount();

	int nStart = m_lcList.GetSelectionMark();

	int nFound = m_lcList.SafeSearch(m_strSearch, nStart+1, FALSE, TRUE);
	if((nFound == -1) && (nStart != 0)){

		nFound = m_lcList.SafeSearch(m_strSearch, 0, FALSE, TRUE);
	}
	if(nFound != -1){

		m_lcList.SetItemState(nFound, LVNI_SELECTED, LVNI_SELECTED);
		m_lcList.EnsureVisible(nFound, FALSE);
	}

}

void CChannelView::OnItemchangedChannels(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(pNMListView->uNewState & LVNI_SELECTED){

		m_lcList.GetItemText(pNMListView->iItem, 0);
		GetApp()->GetMainFrame()->m_strRoom = m_lcList.GetItemText(pNMListView->iItem, 0);
	}
	*pResult = 0;
}

void CChannelView::OnRclickChannels(NMHDR* pNMHDR, LRESULT* pResult) 
{

	CMenu mContextMenu;
	mContextMenu.LoadMenu(IDR_LISTRMENU);

	POINT point;
	GetCursorPos(&point);

	if(m_lcList.GetNextItem(-1, LVNI_SELECTED) >= 0){

		CString strRoom = Util::GetBaseRoomName(m_lcList.GetItemText(m_lcList.GetNextItem(-1, LVNI_SELECTED), 0));
		strRoom.Replace("&", "&&");
		if(strRoom.GetLength() > 40)
			strRoom.Truncate(40);
			//strRoom = strRoom.Left(40);
		mContextMenu.GetSubMenu(0)->InsertMenu(0, MF_STRING|MF_BYPOSITION|MF_DISABLED, 0, strRoom);
		mContextMenu.GetSubMenu(0)->InsertMenu(1, MF_SEPARATOR|MF_BYPOSITION, 0, (LPCSTR)0);
		mContextMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
											point.x,
											point.y,
											AfxGetMainWnd());
	}

	*pResult = 0;
}

void CChannelView::OnDblclkChannels(NMHDR* pNMHDR, LRESULT* pResult) 
{

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(pNMListView->iItem >= 0){

		GetApp()->GetMainFrame()->m_bQuickJoin = TRUE;
		GetApp()->GetMainFrame()->m_strRoom = m_lcList.GetItemText(pNMListView->iItem, 0);
		GetApp()->GetMainFrame()->m_strQuickLogin = m_lcList.GetItemText(pNMListView->iItem, 3);
		GetApp()->GetMainFrame()->JoinChannel();
	}
	*pResult = 0;
}

void CChannelView::OnPopupLoopbackjoin() 
{

	int nSel = m_lcList.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

	    ((CMainFrame*)GetApp()->m_pMainWnd)->m_bQuickJoin = TRUE;
		((CMainFrame*)GetApp()->m_pMainWnd)->m_strRoom = m_lcList.GetItemText(nSel, 0);
		((CMainFrame*)GetApp()->m_pMainWnd)->m_strQuickLogin = m_lcList.GetItemText(nSel, 3);
		((CMainFrame*)GetApp()->m_pMainWnd)->JoinChannel();
		m_lcList.SetItemState(nSel, ~LVNI_SELECTED, LVNI_SELECTED);
	}
}

void CChannelView::OnMyfileNew()
{

	int nSel = m_lcList.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

	    GetApp()->GetMainFrame()->m_bQuickJoin = FALSE;
		GetApp()->GetMainFrame()->m_strRoom = m_lcList.GetItemText(nSel, 0);
		GetApp()->GetMainFrame()->m_strQuickLogin = m_lcList.GetItemText(nSel, 3);
		m_lcList.SetItemState(nSel, ~LVNI_SELECTED, LVNI_SELECTED);
	}
	GetApp()->GetMainFrame()->JoinChannel();
}

void CChannelView::OnPopupCopytopic() 
{

	int nSel = m_lcList.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		CClipboard::SetText((LPTSTR)(LPCTSTR)m_lcList.GetItemText(nSel, 5));
	}	
}

void CChannelView::OnPopupCopyroomname() 
{

	int nSel = m_lcList.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		CClipboard::SetText((LPTSTR)(LPCTSTR)m_lcList.GetItemText(nSel, 0));
	}	
}

void CChannelView::OnSelectionNoscroll() 
{

	UpdateData(TRUE);
}

void CChannelView::OnAdvancedRemovecachedflag()
{

	int nSel = m_lcList.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		if(m_lcList.GetItemCached(nSel) == 1){

			m_lcList.SetItemCached(nSel, FALSE);
		}
	}
}

void CChannelView::OnUpdateAdvancedRemovecachedflag(CCmdUI *pCmdUI)
{

	pCmdUI->Enable(m_lcList.GetSelectedCount());
}

void CChannelView::WriteBufferToList(void)
{
	

	int nSearch = -1;
	BOOL bIsFav = FALSE;
	CString strNum, strLimit, strTmp;
	CString strLoginTmp;
	LVFINDINFO lvi;
	lvi.flags = LVFI_STRING|LVFI_PARTIAL;
	
	BOOL bUpdateItem = FALSE;

	for(int i = 0; i < m_aRoomBuffer.GetSize(); i++){

		// Is the item already listed? then we only
		// update the info
		// otherwise we add it as "new"
		strTmp    = Util::GetBaseRoomName(m_aRoomBuffer[i].strRoomName);
		lvi.psz   = strTmp;
		nSearch	  = m_lcList.FindItem(&lvi, -1);

		if(nSearch == -1){
			
			// room is not listed yet so add it :P
			bUpdateItem = FALSE;
		}
		else{ // the baseroomname is on the list already

			CString strRoom = m_lcList.GetItemText(nSearch, 0);
			
			if(strRoom == m_aRoomBuffer[i].strRoomName){

				// The roomnames are identical. Update the item
				// if it was previously cached
				bUpdateItem = TRUE;
			}
			else{

				// The basenames seem to be identical
				// If the only the IP is different and the Port is also identical
				// it is safe to assume the server has only switched IPs
				// then we can update the item with new IP
				// Otherwise we add is as a new room
				// ROOMNAME_AABBCCDDPPPP			
				strTmp = Util::GetBaseRoomName(m_aRoomBuffer[i].strRoomName) + "_*";
				int nStart = m_aRoomBuffer[i].strRoomName.GetLength() - 4;
				if(nStart > 0){

					strTmp += m_aRoomBuffer[i].strRoomName.Mid(nStart);
				}
				if(Util::WildcardMatch(strRoom, strTmp, FALSE, TRUE)){

					TRACE("%s\n%s\n", strRoom, m_aRoomBuffer[i].strRoomName);
					// bingo, only update
					bUpdateItem = TRUE;
				}
				else{

					// no, we need to add it
					bUpdateItem = FALSE;
				}
			}
		}

		strTmp.Empty();

		if(bUpdateItem){

			// Update item
			if(m_lcList.GetItemCached(nSearch)){

				if(m_nCached > 0) m_nCached--;
			}
			strNum.Format("%d", m_aRoomBuffer[i].wUsers);
			strLimit.Format("%d", m_aRoomBuffer[i].wLimit);
			m_lcList.SetItemText(nSearch, 0, m_aRoomBuffer[i].strRoomName) ;
			m_lcList.SetItemText(nSearch, 1, strNum) ;
			m_lcList.SetItemText(nSearch, 2, strLimit) ;
			m_lcList.SetItemText(nSearch, 5, m_aRoomBuffer[i].strTopic) ;
			m_lcList.SetItemCached(nSearch, FALSE);
		}
		else{

			// Add Item to list
			strNum.Format("%d", m_aRoomBuffer[i].wUsers);
			strLimit.Format("%d", m_aRoomBuffer[i].wLimit);
			bIsFav = IsFavorite(m_aRoomBuffer[i].strRoomName, strLoginTmp);
			m_lcList.AddItem(bIsFav, (LPCTSTR)m_aRoomBuffer[i].strRoomName, (LPCTSTR)strNum, (LPCTSTR)strLimit, (LPCTSTR)strLoginTmp, (LPCTSTR)strTmp, (LPCTSTR)m_aRoomBuffer[i].strTopic);
			strLoginTmp.Empty();
			UpdateStats(m_aRoomBuffer[i].strTopic);
			int nUsers = atoi(strNum);
			if(nUsers < 600){
				
				// if this is higher, it is likely to be a fake user setting ;)
				m_dwTotalUsers+=nUsers;
			}
		}
	}

	m_lcList.Sort();

	if(m_bNoScroll){

		int n = m_lcList.GetNextItem(-1, LVNI_SELECTED);
		int nMax = m_lcList.GetItemCount();
		if(n >= 0){

			m_lcList.EnsureVisible((n +5  < nMax ? n + 5 : n), FALSE);
		}
	}
	m_aRoomBuffer.RemoveAll();

}

void CChannelView::LoadCache(void)
{

	if(!m_bCache) return;

	CWaitCursor wc;
	
	CStdioFile file;
	TRY{

		if(!file.Open(g_sSettings.GetWorkingDir() + "\\roomlist.ccf", CFile::modeCreate|CFile::modeRead|CFile::modeNoTruncate|CFile::typeText, 0)) return;

		CString strLine, strRoom, strTopic, strNum, strLimit, strTmp, strLogin;
		int nPos;
		BOOL bIsFav = FALSE;

		int nCnt = 0;
		while(file.ReadString(strLine)){

			nCnt++;
			CTokenizer token(strLine, "¨");
			if(!token.Next(strNum)) continue;
			if(!token.Next(strLimit)) continue;
			if(!token.Next(strRoom)) continue;
			if(!token.Next(strLogin)) continue;
			if(strLogin == " ") strLogin.Empty();
			
			strTopic = token.Tail();
			
			UpdateStats(strTopic);
			int nUsers = atoi(strNum);
			
			if(nUsers < 600){
				
				// if this is higher, it is likely to be a fake user setting ;)
				m_dwTotalUsers+=nUsers;
			}
			bIsFav = IsFavorite(strRoom, strLogin);
			nPos = m_lcList.AddItem(bIsFav, (LPCTSTR)strRoom, (LPCTSTR)strNum, (LPCTSTR)strLimit, (LPCTSTR)strLogin, (LPCTSTR)strTmp, (LPCTSTR)strTopic);
			m_lcList.SetItemCached(nPos, TRUE);
			m_nCached++;
		}

	}
	CATCH(CFileException, e){

	}END_CATCH;
	m_lcList.Sort(0, TRUE);
}

void CChannelView::SaveCache(void)
{

	if(!m_bCache) return;
	m_lcList.Sort(0, TRUE);
	CStdioFile file;
	TRY{

		CString strLine, strRoom, strTopic, strNum, strLimit, strLogin;
		if(!file.Open(g_sSettings.GetWorkingDir() + "\\roomlist.ccf", CFile::modeCreate|CFile::modeWrite|CFile::typeText, 0)) return;

		int nNum = m_lcList.GetItemCount();
		for(int i = 0; i < nNum; i++){

			if(g_sSettings.GetCacheFavs() && (m_lcList.GetItemMode(i) == 0)){

				continue;
			}
			strRoom  = m_lcList.GetItemText(i, 0);
			strNum   = m_lcList.GetItemText(i, 1);
			strLimit = m_lcList.GetItemText(i, 2);
			strLogin = m_lcList.GetItemText(i, 3);
			strTopic = m_lcList.GetItemText(i, 5);
			if(strRoom.IsEmpty())	continue;
			if(strNum.IsEmpty())	strNum	 = "0";
			if(strLimit.IsEmpty())	strLimit = "0";
			if(strLogin.IsEmpty())  strLogin = " ";
			if(strTopic.IsEmpty())	strTopic = " ";
				
			strLine	 = strNum + "¨" + strLimit + "¨" + strRoom + "¨" + strLogin + "¨" + strTopic + "\n";
			file.WriteString(strLine);
		}

	}
	CATCH(CFileException, e){

	}END_CATCH;
}

CString CChannelView::PingRoom(CString& strRoom, int bInitPing)
{

	CPing ping;
	CPingReply pr;
	CString strPing;

	if(ping.Ping(Util::GetIPFromRoom(strRoom), pr, 50, (bInitPing ? 500 : 2000), 32)){

		strPing.Format("%u", pr.RTT);
	}
	else{

		strPing = (bInitPing ? "" : " - ");
	}
	return strPing;
}

void CChannelView::OnPopupPingroom()
{

	int nSel = m_lcList.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		CString strRoom = m_lcList.GetItemText(nSel, 0);
		m_lcList.SetItemText(nSel, 4, PingRoom(strRoom, FALSE));
	}
}

void CChannelView::OnChannelsFilter()
{

	m_bFilter = !m_bFilter;

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	ini.SetValue("ChannelFilter", "Activate", m_bFilter);

	if(m_bFilter){

		CWaitCursor wc;

		int nFilters = ini.GetValue("ChannelFilter", "Num", 0);
		
		CString strFilter;
		int nSearch = -1;

		for(int i = 0; i < nFilters; i++){

			strFilter.Format("Filter_%d", i);
			strFilter = ini.GetValue("ChannelFilter", strFilter, "");
			if(strFilter.IsEmpty()) continue;

			while((nSearch = m_lcList.SafeSearch(strFilter, -1, FALSE, TRUE)) != -1){

				m_lcList.DeleteItem(nSearch);
				m_dwFiltered++;
			}
		}
	}
	else{

		OnClearRefresh();
	}
}

void CChannelView::OnUpdateChannelsFilter(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(m_bFilter);
}

void CChannelView::OnChannelsEditfilter()
{

	FilterDlg dlg;
	if((dlg.DoModal() == IDOK) && m_bFilter){

		CIni ini;
		ini.SetIniFileName(g_sSettings.GetIniFile());
		int nFilters = ini.GetValue("ChannelFilter", "Num", 0);
		
		CString strFilter;
		int nSearch = -1;

		for(int i = 0; i < nFilters; i++){

			strFilter.Format("Filter_%d", i);
			strFilter = ini.GetValue("ChannelFilter", strFilter, "");
			if(strFilter.IsEmpty()) continue;

			while((nSearch = m_lcList.SafeSearch(strFilter, -1, FALSE, TRUE)) != -1){

				m_lcList.DeleteItem(nSearch);
				m_dwFiltered++;
			}
		}
	}
}

void CChannelView::OnUpdateSelectionNoscroll(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(m_bNoScroll);
}

void CChannelView::OnListLockfocusonselection()
{

	m_bNoScroll = !m_bNoScroll;
	UpdateData(FALSE);
}

void CChannelView::OnListHighlight()
{
	// TODO: Add your command handler code here
	m_bHilite = !m_bHilite;
	m_lcList.SetHiLite(m_bHilite);
	m_lcList.Invalidate();
}

void CChannelView::OnUpdateListHighlight(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(m_bHilite);
}

void CChannelView::OnListEdithighlightkeywords()
{

	char buffer[MAX_PATH+1];
	GetWindowsDirectory((LPSTR)&buffer, MAX_PATH+1);

	CString strExec, strParam;
	strExec.Format("%s\\notepad.exe", buffer);
	strParam.Format("%s\\hilite.ini", g_sSettings.GetWorkingDir());
	Util::ShellExecuteWait(strExec, strParam);
	g_sSettings.LoadHiLite();
	AfxMessageBox(IDS_HILITE_LOADED, MB_ICONINFORMATION);
}

void CChannelView::OnUpdatePopupLoopbackjoin(CCmdUI *pCmdUI)
{

	pCmdUI->Enable(m_lcList.GetSelectedCount());
}


void CChannelView::OnConfigurecachingCacheroomlist()
{

	m_bCache = !m_bCache;
}

void CChannelView::OnUpdateConfigurecachingCacheroomlist(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(m_bCache);
}

void CChannelView::OnAdcancedClearCache()
{

	CWaitCursor wc;

	for(int i = 0; i < m_lcList.GetItemCount(); i++){

		if(m_lcList.GetItemCached(i) && (m_lcList.GetItemMode(i) == 0)){

			m_lcList.DeleteItem(i);
			i--;
		}
	}
	

	m_nCached = 0;
	CString strNum;
	m_nPerc = (int)(((float)m_lcList.GetItemCount()+m_dwFiltered-m_nCached) / m_fTotal * 100.f);
	m_pgStatus.SetPos(m_nPerc);
	m_tTime.Calculate(m_nPerc);
	strNum.Format(IDS_ROOMS_ETA, m_lcList.GetItemCount(), m_tTime.GetRemainingString(), m_tTime.GetElapsedString(), m_tTime.GetEstimateString());
	GetDlgItem(IDC_STATIC_ROOMS)->SetWindowText(strNum);

	if(m_pStatusBar){

		if(GetApp()->GetMainFrame()->GetActiveFrame() == GetParentFrame()){
			CString strOut;
			strOut.LoadString(IDS_CHANNELLIST);
			m_pStatusBar->SetPaneText(2, strOut);
			CString strText;
			strText.Format(IDS_FILTEREDCHANNELS, m_lcList.GetItemCount(), m_dwFiltered, m_dwFiltered+m_lcList.GetItemCount());
			m_pStatusBar->SetPaneText(3, strText);
			m_pStatusBar->SetPaneText(4, "");
		}
	}
}

void CChannelView::OnAdvancedRemovefromcache()
{

	int nSel = m_lcList.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		if(m_lcList.GetItemCached(nSel)){

			m_lcList.DeleteItem(nSel);
		}
	}
}


void CChannelView::OnDeleteChannel()
{

	int nSel = m_lcList.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		m_lcList.DeleteItem(nSel);
	}
}


void CChannelView::OnListAddtofavorites()
{

	int nSel = m_lcList.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		//if(m_lcList.GetItemMode(nSel) == 0){

		CString strRoom  = m_lcList.GetItemText(nSel, 0); 
		CString strUsers = m_lcList.GetItemText(nSel, 1); 
		CString strLimit = m_lcList.GetItemText(nSel, 2); 
		CString strLogin = m_lcList.GetItemText(nSel, 3);
		CString strPing  = m_lcList.GetItemText(nSel, 4);
		CString strTopic = m_lcList.GetItemText(nSel, 5);
		BOOL bCached = m_lcList.GetItemCached(nSel);

		CAddFavorite dlg;
		dlg.m_strChannel = strRoom;
		dlg.m_strLogin   = strLogin;

		if(dlg.DoModal() == IDOK){

			nSel = m_lcList.GetNextItem(-1, LVNI_SELECTED);
			
			m_lcList.DeleteItem(nSel);

			strRoom  = dlg.m_strChannel;
			strLogin = dlg.m_strLogin; 
			
			nSel = m_lcList.AddItem(1, (LPCTSTR)strRoom, (LPCTSTR)strUsers, (LPCTSTR)strLimit, 
								(LPCTSTR)strLogin, (LPCTSTR)strPing, (LPCTSTR)strTopic);

			m_lcList.SetItemState(nSel, LVNI_SELECTED, LVNI_SELECTED);
			m_lcList.SetItemCached(nSel, bCached);
			m_lcList.Sort();
			nSel = m_lcList.GetNextItem(nSel, LVNI_SELECTED);
			m_lcList.EnsureVisible(nSel, FALSE);

			CIni ini;
			ini.SetIniFileName(g_sSettings.GetIniFile());
			
			CString strFav = Util::GetBaseRoomName(dlg.m_strChannel) + "*";
			CString strLabel, strValue;
			int nCnt = ini.GetValue("Favorites", "Count", 0);
			for(int i = 0; i < nCnt; i++){

				strLabel.Format("Room_%d", i);
				strValue = ini.GetValue("Favorites", strLabel, "");
				if(strValue == strFav){

					strLabel.Format("RoomLogin_%d", i);
					ini.SetValue("Favorites", strLabel, dlg.m_strLogin);
					break;
				}
			}
			
			if(i >= nCnt){

				strLabel.Format("Room_%d", nCnt);
				ini.SetValue("Favorites", strLabel, strFav);
				
				strLabel.Format("RoomLogin_%d", nCnt++);
				ini.SetValue("Favorites", strLabel, dlg.m_strLogin);
				
				ini.SetValue("Favorites", "Count", nCnt);
			}

			if(dlg.m_bAutoJoin){

				AddToAutoJoin(dlg.m_strChannel, dlg.m_strLogin);
            }
			m_lcList.Sort();
		}
	}
}

void CChannelView::OnUpdateListAddtofavorites(CCmdUI *pCmdUI)
{

	int nSel = m_lcList.GetNextItem(-1, LVNI_SELECTED);
	BOOL bActive = FALSE;
	if(nSel >= 0){

		bActive = m_lcList.GetItemMode(nSel) == 0;
	}

	pCmdUI->Enable(bActive);
}

void CChannelView::OnListRemovefromfavorites()
{

	int nSel = m_lcList.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		if(m_lcList.GetItemMode(nSel) == 1){

			CString strRoom  = m_lcList.GetItemText(nSel, 0); 
			CString strUsers = m_lcList.GetItemText(nSel, 1); 
			CString strLimit = m_lcList.GetItemText(nSel, 2); 
			CString strLogin = m_lcList.GetItemText(nSel, 3); 
			CString strPing  = m_lcList.GetItemText(nSel, 4);
			CString strTopic = m_lcList.GetItemText(nSel, 5);
			
			BOOL bCached = m_lcList.GetItemCached(nSel);
			m_lcList.DeleteItem(nSel);
			nSel = m_lcList.AddItem(0, (LPCTSTR)strRoom, (LPCTSTR)strUsers, (LPCTSTR)strLimit, 
								(LPCTSTR)strLogin, (LPCTSTR)strPing, (LPCTSTR)strTopic);
			m_lcList.SetItemCached(nSel, bCached);
			m_lcList.SetItemState(nSel, LVNI_SELECTED, LVNI_SELECTED);
			m_lcList.EnsureVisible(nSel, FALSE);

			CIni ini;
			ini.SetIniFileName(g_sSettings.GetIniFile());
			
			CString strFav = Util::GetBaseRoomName(strRoom) + "*";
            CString strTmp, strTmp2;
			CStringArray aRooms, aLogins;

			int nCnt = ini.GetValue("Favorites", "Count", 0);
			
			for(int i = 0; i < nCnt; i++){

				strTmp.Format("Room_%d", i);
				strTmp2 = ini.GetValue("Favorites", strTmp, "");
				if(strTmp2 != strFav){

					aRooms.Add(strTmp2);
					strTmp.Format("RoomLogin_%d", i);
					strTmp2 = ini.GetValue("Favorites", strTmp, "");
					aLogins.Add(strTmp2);
				}
			}

			nCnt = aRooms.GetSize();

			for(i = 0; i < nCnt; i++){

				strTmp.Format("Room_%d", i);
				ini.SetValue("Favorites", strTmp, aRooms[i]);
				strTmp.Format("RoomLogin_%d", i);
				ini.SetValue("Favorites", strTmp, aLogins[i]);
			}

			ini.SetValue("Favorites", "Count", nCnt);

			m_lcList.Sort();
			aRooms.RemoveAll(); aLogins.RemoveAll();
		}
	}
}

void CChannelView::OnPopupEditfavoriteentry()
{

	OnListAddtofavorites();
}

void CChannelView::OnUpdateAdvancedRemovefromcache(CCmdUI *pCmdUI)
{

	pCmdUI->Enable(m_lcList.GetSelectedCount());
}

void CChannelView::OnUpdateListRemovefromfavorites(CCmdUI *pCmdUI)
{

	pCmdUI->Enable(m_lcList.GetSelectedCount());
}

void CChannelView::OnAdvancedManuallyaddroomtocache()
{

	CAddCache dlg;
	if(dlg.DoModal() == IDOK){

		CString strTmp;
		BOOL bIsFav = IsFavorite(dlg.m_strRoom, strTmp);
		strTmp.Empty();
		int n = m_lcList.AddItem(bIsFav, (LPCTSTR)dlg.m_strRoom, (LPCTSTR)dlg.m_strUsers, 
							(LPCTSTR)dlg.m_strLimit, (LPCTSTR)dlg.m_strLogin, (LPCTSTR)strTmp, 
							(LPCTSTR)dlg.m_strTopic);
		m_lcList.SetItemState(n, LVNI_SELECTED, LVNI_SELECTED);
		m_lcList.SetItemCached(n, TRUE);
		m_lcList.Sort();
		n = m_lcList.GetNextItem(-1, LVNI_SELECTED);
		m_lcList.EnsureVisible(n, FALSE);

		//m_lcList.EnsureVisible(n, FALSE);
	}
}

BOOL CChannelView::IsFavorite(const CString strRoom, CString& strLogin) const
{

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());
	int nCount = ini.GetValue("Favorites", "Count", 0);

	CString strFav, strTmp;
	for(int i = 0; i < nCount; i++){

		strTmp.Format("Room_%d", i);
        strFav = ini.GetValue("Favorites", strTmp, "");
		
		if(strFav.IsEmpty()) continue;

		if(Util::WildcardMatch(strRoom, strFav, FALSE, FALSE)){

			strTmp.Format("RoomLogin_%d", i);
			strLogin = ini.GetValue("Favorites", strTmp, "");
			return TRUE;
		}
	}
	return FALSE;
}

void CChannelView::OnListGroupfavoritesattopoflist()
{

	g_sSettings.SetHiliteTop(!g_sSettings.GetHiliteTop());
	m_lcList.Sort();
}

void CChannelView::OnUpdateListGroupfavoritesattopoflist(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(g_sSettings.GetHiliteTop());
}

void CChannelView::OnListCacheonlyfavorites()
{

	g_sSettings.SetCacheFavs(!g_sSettings.GetCacheFavs());
	if(g_sSettings.GetCacheFavs()){

		CWaitCursor wc;
		
		for(int i = 0; i < m_lcList.GetItemCount(); i++){

			if(m_lcList.GetItemCached(i) && (m_lcList.GetItemMode(i) == 0)){

				m_lcList.DeleteItem(i--);
			}
		}
	}
}

void CChannelView::OnUpdateListCacheonlyfavorites(CCmdUI *pCmdUI)
{

	pCmdUI->Enable(m_bCache);
	pCmdUI->SetCheck(g_sSettings.GetCacheFavs() && m_bCache);
}

void CChannelView::AddToAutoJoin(const CString strChannel, const CString strLogin)
{

	CIni ini;
	int n = 0;
	CString strTmp;
	
	ini.SetIniFileName(g_sSettings.GetIniFile());
	n = ini.GetValue("AutoJoins", "Num", 0);

	for(int i = 1; i < n+1; i++){

		strTmp.Format("AutoJoin_%03d", i);
		if(ini.GetValue("AutoJoins", strTmp, "") == strChannel){

			ini.SetValue("AutoJoins", strTmp, strChannel);
			strTmp.Format("AutoJoinLogin_%03d", i);
			ini.SetValue("AutoJoins", strTmp, strLogin);
			return;
		}
	}

	for(i = 1; i < n+1; i++){

		strTmp.Format("AutoJoin_%03d", i);
		if(ini.GetValue("AutoJoins", strTmp, "").IsEmpty()) break;
	}

	strTmp.Format("AutoJoin_%03d", i);
	ini.SetValue("AutoJoins", "Num", (i <= n ? n : n+1));
	ini.SetValue("AutoJoins", strTmp, strChannel);
	strTmp.Format("AutoJoinLogin_%03d", i);
	ini.SetValue("AutoJoins", strTmp, strLogin);
}


void CChannelView::OnUpdatePopupEditfavoriteentry(CCmdUI *pCmdUI)
{

	BOOL bEnable = FALSE;
	int nSel = m_lcList.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		bEnable = m_lcList.GetItemMode(nSel) == 1;
	}
	pCmdUI->Enable(bEnable);
}

void CChannelView::OnListShowstatistics()
{

	CString strStat;

	DWORD dwTotal = m_dwWinMX+m_dwRCMS+m_dwWCS+m_dwMCS+m_dwMCMA+m_dwRose+m_dwFXServ;;
	
	if(dwTotal == 0) return;

	CString strUsers, strFiltered, strTotal;
	
	strUsers.Format("%04u", m_dwTotalUsers);
	strFiltered.Format("%04u", m_dwFiltered);
	strTotal.Format("%04u", dwTotal);
	
	NUMBERFMT fmt;
	fmt.Grouping      = 3;
	fmt.LeadingZero   = 1;
	fmt.lpDecimalSep  = ".";
	fmt.lpThousandSep = ",";
	fmt.NegativeOrder = 0;
	fmt.NumDigits     = 0;

	char *buffer = new char[256];
    
	ZeroMemory(buffer, 256);

	if(GetNumberFormat(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),SORT_DEFAULT), 0, 
						(LPCTSTR)strUsers, &fmt, buffer, 256) != 0){

		strUsers = buffer;
	}
	

	ZeroMemory(buffer, 256);
	if(GetNumberFormat(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),SORT_DEFAULT),
						0, (LPCTSTR)strFiltered, &fmt, buffer, 256) != 0){

		strFiltered = buffer;
	}

	ZeroMemory(buffer, 256);
	if(GetNumberFormat(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),SORT_DEFAULT),
						0, (LPCTSTR)strTotal, &fmt, buffer, 256) != 0){

		strTotal = buffer;
	}

	delete buffer;
	buffer = 0;

	strStat.Format("WPN Statistics:\n"
		           "++++++++++++++++++++++++++\n\n"
				   "+ %s\tusers in\n"
				   "+ %s\tchannels total\n"
				   "+ %s\tchannels filtered out by you\n\n"
				   "Servers:\n"
				   "++++++++++++++++++++++++++\n\n"
				   "+ %04u WinMX / Other\t(%.2f%%)\n"
				   "+ %04u RCMS\t\t(%.2f%%)\n"
				   "+ %04u MCMA\t\t(%.2f%%)\n"
				   "+ %04u MCS\t\t(%.2f%%)\n"
				   "+ %04u WCS\t\t(%.2f%%)\n"
				   "+ %04u roboServe\t\t(%.2f%%)\n"
				   "+ %04u FXServ\t\t(%.2f%%)\n",
				   strUsers,
				   strTotal, 
				   strFiltered, 
				   m_dwWinMX, ((float)m_dwWinMX/(float)dwTotal*100.f),
				   m_dwRCMS, ((float)m_dwRCMS/(float)dwTotal*100.f),
				   m_dwMCMA, ((float)m_dwMCMA/(float)dwTotal*100.f),
				   m_dwMCS, ((float)m_dwMCS/(float)dwTotal*100.f),
				   m_dwWCS, ((float)m_dwWCS/(float)dwTotal*100.f),
				   m_dwRose, ((float)m_dwRose/(float)dwTotal*100.f),
				   m_dwFXServ, ((float)m_dwFXServ/(float)dwTotal*100.f)
				   );

	MessageBox(strStat, "Channel Statistics", MB_ICONINFORMATION);


}

void CChannelView::OnListCopystatistics()
{

	CString strStat;

	DWORD dwTotal = m_dwWinMX+m_dwRCMS+m_dwWCS+m_dwMCS+m_dwMCMA+m_dwRose+m_dwFXServ;;

	if(dwTotal == 0) return;

	CString strUsers, strTotal;
	
	strUsers.Format("%u", m_dwTotalUsers);
	strTotal.Format("%u", dwTotal);
	
	NUMBERFMT fmt;
	fmt.Grouping      = 3;
	fmt.LeadingZero   = 1;
	fmt.lpDecimalSep  = ".";
	fmt.lpThousandSep = ",";
	fmt.NegativeOrder = 0;
	fmt.NumDigits     = 0;

	char *buffer = new char[256];
    
	ZeroMemory(buffer, 256);

	if(GetNumberFormat(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),SORT_DEFAULT), 0, 
						(LPCTSTR)strUsers, &fmt, buffer, 256) != 0){

		strUsers = buffer;
	}
	
	ZeroMemory(buffer, 256);
	if(GetNumberFormat(MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),SORT_DEFAULT),
						0, (LPCTSTR)strTotal, &fmt, buffer, 256) != 0){

		strTotal = buffer;
	}

	delete buffer;
	buffer = 0;

	strStat.Format("WPN Statistics: %s users in %s channels total +++ Servers: %04u WinMX / Other (%.2f%%) +++ %04u RCMS (%.2f%%) +++ %04u MCMA (%.2f%%) +++ %04u MCS (%.2f%%) +++ %04u WCS (%.2f%%) +++ %04u roboServe (%.2f%%) +++ %04u FXServ (%.2f%%)",
		           strUsers, strTotal,
				   m_dwWinMX, ((float)m_dwWinMX/(float)dwTotal*100.f),
				   m_dwRCMS, ((float)m_dwRCMS/(float)dwTotal*100.f),
				   m_dwMCMA, ((float)m_dwMCMA/(float)dwTotal*100.f),
				   m_dwMCS, ((float)m_dwMCS/(float)dwTotal*100.f),
				   m_dwWCS, ((float)m_dwWCS/(float)dwTotal*100.f),
				   m_dwRose, ((float)m_dwRose/(float)dwTotal*100.f),
				   m_dwFXServ, ((float)m_dwFXServ/(float)dwTotal*100.f)
				   );

	CClipboard::SetText((LPTSTR)(LPCTSTR)strStat);
}

BOOL CChannelView::PreTranslateMessage(MSG* pMsg) 
{

	
	m_ToolTip.RelayEvent(pMsg);
	
	return CFormView::PreTranslateMessage(pMsg);
}

void CChannelView::NotifyDisplayTooltip(NMHDR * pNMHDR, LRESULT * result)
{

	*result = 0;
    NM_PPTOOLTIP_DISPLAY * pNotify = (NM_PPTOOLTIP_DISPLAY*)pNMHDR;
    
	if(pNotify->hwndTool == m_lcList.m_hWnd){

		CRect rcCtrl, rect;
		CPoint pt = *pNotify->pt;

		m_lcList.GetWindowRect(&rcCtrl);
		pt -= rcCtrl.TopLeft();
		LVHITTESTINFO li;
		li.pt = pt;
		int nItem = m_lcList.SubItemHitTest(&li);
		int nSubItem = li.iSubItem;
		UINT nFlags =   li.flags;
		
		if(nFlags & LVHT_ONITEM){
			
			pNotify->ti->nEffect		= g_sSettings.GetToolEffect();
			pNotify->ti->nGranularity	= g_sSettings.GetToolGran();
			pNotify->ti->nTransparency	= g_sSettings.GetToolTrans();
			pNotify->ti->crBegin		= g_sSettings.GetToolBg1();
			pNotify->ti->crMid			= g_sSettings.GetToolBg2();
			pNotify->ti->crEnd			= g_sSettings.GetToolBg3();

			CString strRoom    = m_lcList.GetItemText(li.iItem, 0);
			CString strUsers   = m_lcList.GetItemText(li.iItem, 1);
			CString strMax     = m_lcList.GetItemText(li.iItem, 2);
			CString strLogin   = m_lcList.GetItemText(li.iItem, 3);
			CString strPing    = m_lcList.GetItemText(li.iItem, 4);
			CString strTopic   = m_lcList.GetItemText(li.iItem, 5);
			
			CString strRoomShort = Util::GetBaseRoomName(strRoom);

			if(strTopic.GetLength() > 63){

				strTopic.Truncate(60);
				strTopic += "...";
			}

			CIni ini;
			ini.SetIniFileName(g_sSettings.GetIniFile());

			CString strLastJoin = ini.GetValue("LastJoins", strRoomShort, "");

			CString strLabel, strChannel, strImage;
			
			int nCnt = ini.GetValue("ChannelToolTips", "ChannelImageCnt", 0);

			for(int i = 0; i < nCnt; i++){

				strLabel.Format("ChannelName_%d", i);
				strChannel = ini.GetValue("ChannelToolTips", strLabel, "");

				if(strChannel.GetLength() && Util::WildcardMatch(strRoom, strChannel)){

					strLabel.Format("ChannelImage_%d", i);
					strImage = ini.GetValue("ChannelToolTips", strLabel, "");
					break;
				}
			}
			if(i >= nCnt){

				strImage = g_sSettings.GetChannelDefaultImage();
			}
			if(strImage.IsEmpty()){

				strLabel = " ";
			}
			else{

				strLabel.Format("<bmp file=\"%s\" mask=#FF00FF>", strImage);
			}


			pNotify->ti->sTooltip = g_sSettings.GetToolListHover();
			pNotify->ti->sTooltip.Replace("%JOINCOUNT%", Util::Formatint(ini.GetValue("LastJoins", strRoomShort + "_Count", 0)));
			pNotify->ti->sTooltip.Replace("%ROOMIMAGE%", strLabel);
			pNotify->ti->sTooltip.Replace("%ROOMNAME%", strRoom);
			pNotify->ti->sTooltip.Replace("%ROOMSHORT%", strRoomShort);
			pNotify->ti->sTooltip.Replace("%TOPIC%", strTopic);
			pNotify->ti->sTooltip.Replace("%LASTJOIN%", strLastJoin);
			pNotify->ti->sTooltip.Replace("%NUMUSERS%", strUsers);
			pNotify->ti->sTooltip.Replace("%MAXUSERS%", strMax);
			pNotify->ti->sTooltip.Replace("%LOGIN%", strLogin);
			pNotify->ti->sTooltip.Replace("%PING%", strPing);
		}
	}
	else{

		CString strImage;
		strImage.Format("<icon idres=%d width=16 height=16>", IDR_LIST);
		pNotify->ti->sTooltip = g_sSettings.GetToolList();
		pNotify->ti->sTooltip.Replace("%LISTICON%", strImage);
		pNotify->ti->sTooltip.Replace("%NUMCHANNELS%", Util::Formatint(m_lcList.GetItemCount()));
		pNotify->ti->sTooltip.Replace("%NUMFILTERED%", Util::Formatint(m_dwFiltered));
		pNotify->ti->sTooltip.Replace("%NUMWINMX%", Util::Formatint(m_dwWinMX));
		pNotify->ti->sTooltip.Replace("%NUMRCMS%", Util::Formatint(m_dwRCMS));
		pNotify->ti->sTooltip.Replace("%NUMMCMA%", Util::Formatint(m_dwMCMA));
		pNotify->ti->sTooltip.Replace("%NUMMCS%", Util::Formatint(m_dwMCS));
		pNotify->ti->sTooltip.Replace("%NUMWCS%", Util::Formatint(m_dwWCS));
		pNotify->ti->sTooltip.Replace("%NUMROSE%", Util::Formatint(m_dwRose));
		pNotify->ti->sTooltip.Replace("%NUMFXSERV%", Util::Formatint(m_dwFXServ));

		pNotify->ti->nEffect = g_sSettings.GetToolEffect();
		pNotify->ti->nGranularity = g_sSettings.GetToolGran();
		pNotify->ti->nTransparency = g_sSettings.GetToolTrans();
		pNotify->ti->crBegin = g_sSettings.GetToolBg1();
		pNotify->ti->crMid   = g_sSettings.GetToolBg2();
		pNotify->ti->crEnd   = g_sSettings.GetToolBg3();
	}
}
