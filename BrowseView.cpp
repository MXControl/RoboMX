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
#include "BrowseDoc.h"
#include "BrowseView.h"
#include "MainFrm.h"
#include "Settings.h"
#include "Clipboard.h"
#include "roboex.h"
#include "util.h"
#include "Ini.h"

// CBrowseView

IMPLEMENT_DYNCREATE(CBrowseView, CFormView)

extern UINT UWM_WPNNOTIFY;
extern UINT UWM_LOAD_SETTINGS;
extern UINT UWM_ADDLISTENER;
extern UINT UWM_REMLISTENER;
extern UINT UWM_NEWPM;
extern UINT UWM_REBROWSE;

extern CSettings g_sSettings;

//#define TIMER_STOP_ANI 101010
#define TIMER_START    101011
#define TIMER_ASCIIANI 101012

CBrowseView::CBrowseView()
	: CFormView(CBrowseView::IDD)
	, m_strSearch(_T(""))
{

//	m_iAni = NULL;
	m_pStatusBar = NULL;
	m_bFullPath  = FALSE;
	m_bWorking	 = FALSE;
	m_nAsciiAni  = 0;
	m_dwNumAudio = 0;
	m_dwNumVideo = 0;
	m_dwNumImg   = 0;
	m_dwNumOther = 0;
}

CBrowseView::~CBrowseView()
{

	m_iImageList.DeleteImageList();
	m_fFont.DeleteObject();
}

void CBrowseView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_LIST, m_lcFiles);
	DDX_Control(pDX, IDC_STATIC_ANI, m_stAni);
	DDX_Control(pDX, IDC_SEARCH, m_eSearch);
	DDX_Text(pDX, IDC_SEARCH, m_strSearch);
	DDX_Check(pDX, IDC_PATH, m_bFullPath);
	DDX_Control(pDX, IDC_PATH, m_btCheck);
	DDX_Control(pDX, IDC_STATIC1, m_wndStat);
}

BEGIN_MESSAGE_MAP(CBrowseView, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_REGISTERED_MESSAGE(UWM_WPNNOTIFY, OnWPNNotify)
	ON_REGISTERED_MESSAGE(UWM_LOAD_SETTINGS, OnReloadCfg)
	ON_EN_CHANGE(IDC_SEARCH, OnEnChangeSearch)
	ON_COMMAND(ID_BROWSE_COPYFILEPATH, OnBrowseCopyfilepath)
	ON_COMMAND(ID_BROWSE_COPYFILENAME, OnBrowseCopyfilename)
	ON_NOTIFY(NM_RCLICK, IDC_FILE_LIST, OnNMRclickFileList)
	ON_COMMAND(ID_BROWSE_COPYFILEINFORMATION, OnBrowseCopyfileinformation)
	ON_BN_CLICKED(IDC_PATH, OnBnClickedPath)
	ON_COMMAND(ID_ACTIONS_RELOADFILES, OnActionsReloadfiles)
	ON_COMMAND(ID_ACTIONS_SENDPRIVATEMESSAGE, OnActionsSendprivatemessage)
	ON_UPDATE_COMMAND_UI(ID_ACTIONS_RELOADFILES, OnUpdateActionsReloadfiles)
	ON_NOTIFY (UDM_TOOLTIP_DISPLAY, NULL, NotifyDisplayTooltip)
END_MESSAGE_MAP()


// CBrowseView diagnostics

#ifdef _DEBUG
void CBrowseView::AssertValid() const
{
	CFormView::AssertValid();
}

void CBrowseView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CBrowseDoc* CBrowseView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBrowseDoc)));
	return (CBrowseDoc*)m_pDocument;
}
#endif //_DEBUG

// CBrowseView message handlers
void CBrowseView::OnInitialUpdate()
{

	CFormView::OnInitialUpdate();

    GetParentFrame()->RecalcLayout();
	//::InitializeCriticalSection(&m_AniLock);

	CString strTitle;
	strTitle.Format(IDS_BROWSE_TITLE, GetDocument()->m_strUser);

	GetDocument()->SetTitle(strTitle);

	GetApp()->GetMainFrame()->m_wndDocSelector.AddButton(this, IDR_BROWSEVIEW_TMPL);
	m_pStatusBar = (CColorStatusBar *)GetApp()->GetMainFrame()->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	
	if(m_pStatusBar){

		m_pStatusBar->SetPaneText(2, "");
		m_pStatusBar->SetPaneText(3, "");
		m_pStatusBar->SetPaneText(4, "");
	}

	::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_ADDLISTENER, 0, (LPARAM)this);

	m_fFont.CreateFont(15, 6, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_STRING_PRECIS, 
		CLIP_STROKE_PRECIS, PROOF_QUALITY, FF_DONTCARE, g_sSettings.GetFont());

	m_lcFiles.SetHeadings(IDS_BROWSE_HEAD);
	m_lcFiles.SetColors(
		g_sSettings.GetRGBNormalMsg(),
		g_sSettings.GetRGBFiles(),
		g_sSettings.GetRGBLine(),
		g_sSettings.GetRGBIP(),
		g_sSettings.GetRGBPort()
		);

	CBitmap		bitmap;
	bitmap.LoadMappedBitmap(IDB_FILETYPES);
	m_iImageList.Create(16, 16, ILC_COLORDDB|ILC_MASK, 4, 1);
	m_iImageList.Add(&bitmap, RGB(255,0,255));
	m_lcFiles.SetImageList(&m_iImageList, LVSIL_SMALL);

	m_lcFiles.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_UNDERLINEHOT);
	m_lcFiles.SetBkColor(g_sSettings.GetRGBBg());	  
	m_lcFiles.SetFont(&m_fFont);

	m_eSearch.SetExtended();
	m_eSearch.SetBkColor(g_sSettings.GetRGBBg());
	m_eSearch.SetFont(&m_fFont);

	LVCOLUMN lvCol;
	lvCol.mask = LVCF_FMT;
	lvCol.fmt  = LVCFMT_RIGHT;
	m_lcFiles.SetColumn(2, &lvCol);
	m_lcFiles.SetColumn(3, &lvCol);
	m_lcFiles.SetColumn(4, &lvCol);
	m_lcFiles.SetColumn(5, &lvCol);

	m_aBrowse.RemoveAll();
	m_bWorking = TRUE;

	Layout();
	SetTimer(TIMER_START, 100, 0);
	SetTimer(TIMER_ASCIIANI, 1000, 0);
	//SetAni("WAIT");
}

void CBrowseView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if(m_pStatusBar){

		m_pStatusBar->SetPaneText(2, GetDocument()->m_strUser);
		m_pStatusBar->SetPaneText(4, Util::Formatint(m_lcFiles.GetItemCount()));
		m_pStatusBar->SetPaneText(3, "");
	}

	m_lcFiles.Sort();
}

void CBrowseView::OnDestroy()
{

	m_aBrowse.RemoveAll();
	KillTimer(TIMER_ASCIIANI);
	KillTimer(TIMER_START);
	//SetAni("");
	//::DeleteCriticalSection(&m_AniLock);
	GetApp()->GetMainFrame()->m_wndDocSelector.RemoveButton(this);
	::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_REMLISTENER, 0, (LPARAM)this);

	CFormView::OnDestroy();
}

LRESULT CBrowseView::OnWPNNotify(WPARAM w, LPARAM l)
{


	if(w == WPN_BROWSE_ENTRY){


		if(GetDocument()->m_dwIP == ((PBROWSE_INFO)l)->dwIP && GetDocument()->m_wPort == ((PBROWSE_INFO)l)->wPort){

			if(GetDocument()->m_strUser.Compare(((PBROWSE_INFO)l)->lpszName) != 0) return 0;
        	
			BROWSEITEM b;
			b.strPath = ((PBROWSE_INFO)l)->lpszFile;
			if(b.strPath.ReverseFind('\\') >= 0){

				b.strFile = b.strPath.Mid(b.strPath.ReverseFind('\\')+1);
			}
			else{

				b.strFile = b.strPath.Mid(b.strPath.ReverseFind('/')+1);
			}

			b.strSize.Format("%.03f", (float)((PBROWSE_INFO)l)->dwFileSize/1024.f);

			if(((PBROWSE_INFO)l)->wBitrate > 0)
				b.strBitrate.Format("%u", ((PBROWSE_INFO)l)->wBitrate);
			if(((PBROWSE_INFO)l)->wSampleFQ > 0)
				b.strSamplerate.Format("%u", ((PBROWSE_INFO)l)->wSampleFQ);
			if(((PBROWSE_INFO)l)->dwTime > 0)
				b.strLength.Format("%u:%02u", ((PBROWSE_INFO)l)->dwTime/60, (((PBROWSE_INFO)l)->dwTime)%60);

			b.nIcon = 2;
			b.strType = b.strFile;
			b.strType.MakeLower();
			int nDot = b.strFile.ReverseFind('.');
			if(nDot > 0){

				b.strType = b.strType.Mid(nDot+1);
			}

			if((b.strType == "mp3") || (b.strType == "wma") || (b.strType == "ogg") || (b.strType == "wav")){
				b.nIcon = 0;
				m_dwNumAudio++;
			}
			else if((b.strType == "mpg") || (b.strType == "mpeg") || (b.strType == "avi") || (b.strType == "vob")){
				
				b.nIcon = 1;
				m_dwNumVideo++;
			}
			else if((b.strType == "jpg") || (b.strType == "bmp") || (b.strType == "gif") || (b.strType == "jpeg")){
				
				b.nIcon = 3;
				m_dwNumImg++;
			}
			else m_dwNumOther++;

			m_lcFiles.AddItem(
						b.nIcon, 
						(m_bFullPath ? (LPCTSTR)b.strPath : (LPCTSTR)b.strFile), 
						b.strType,
						(LPCTSTR)b.strSize, 
						(LPCTSTR)b.strBitrate, 
						(LPCTSTR)b.strSamplerate, 
						(LPCTSTR)b.strLength
					);

			if(m_pStatusBar && (GetApp()->GetMainFrame()->GetActiveFrame() == GetParentFrame())){

				CString strTmp;
				strTmp.Format("%d", m_lcFiles.GetItemCount());
				m_pStatusBar->SetPaneText(4, (LPCTSTR)strTmp);
			}
			m_aBrowse.Add(b);

			return 1;
		}
	}
	else if(w == WPN_BROWSE_DONE){

		CString strFile;
		if(GetDocument()->m_dwIP == ((PBROWSE_INFO)l)->dwIP && GetDocument()->m_wPort == ((PBROWSE_INFO)l)->wPort){

			if(GetDocument()->m_strUser.Compare(((PBROWSE_INFO)l)->lpszName) != 0) return 0;
        	
			// We are done :-)
			KillTimer(TIMER_ASCIIANI);
			if(m_pStatusBar && (GetApp()->GetMainFrame()->GetActiveFrame() == GetParentFrame())){

				//m_pStatusBar->SetLagColor(RGB(0, 200, 0), "OK.");
				m_pStatusBar->SetPaneText(3, "OK", RGB(255,255,255), RGB(0, 200, 0));
			}
			CString strText;
			strText.Format(IDS_BROWSE_CMPLT, GetDocument()->m_strUser, m_lcFiles.GetItemCount());
			m_stAni.SetWindowText(strText);
			::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_REMLISTENER, 0, (LPARAM)this);
			m_bWorking = FALSE;
			m_lcFiles.Sort();
			return 1;
		}
	}
	else if(w == WPN_BROWSE_ERROR){

		CString strFile;
		if(GetDocument()->m_dwIP == ((PBROWSE_INFO)l)->dwIP && GetDocument()->m_wPort == ((PBROWSE_INFO)l)->wPort){

			if(GetDocument()->m_strUser.Compare(((PBROWSE_INFO)l)->lpszName) != 0) return 0;
        	
			// Primary node sent an error :-(
			KillTimer(TIMER_ASCIIANI);
			if(m_pStatusBar && (GetApp()->GetMainFrame()->GetActiveFrame() == GetParentFrame())){

				m_pStatusBar->SetPaneText(3, "Error", RGB(255, 255, 255), RGB(200, 0, 0));
			}
			CString strText;
			strText.Format(IDS_BROWSE_CMPLT, GetDocument()->m_strUser, m_lcFiles.GetItemCount());
			m_stAni.SetWindowText(strText);
			::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_REMLISTENER, 0, (LPARAM)this);
			m_bWorking = FALSE;
			m_lcFiles.Sort();
			return 1;
		}
	}
	return 0;
}


LRESULT CBrowseView::OnReloadCfg(WPARAM w, LPARAM l)
{

	m_fFont.DeleteObject();
	m_fFont.CreateFont(15, 6, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_STRING_PRECIS, 
		CLIP_STROKE_PRECIS, PROOF_QUALITY, FF_DONTCARE, g_sSettings.GetFont());

	m_lcFiles.SetColors(
		g_sSettings.GetRGBNormalMsg(),
		g_sSettings.GetRGBFiles(),
		g_sSettings.GetRGBLine(),
		g_sSettings.GetRGBIP(),
		g_sSettings.GetRGBPort()
		);
	m_lcFiles.SetBkColor(g_sSettings.GetRGBBg());	
	m_lcFiles.SetFont(&m_fFont);

	m_eSearch.SetBkColor(g_sSettings.GetRGBBg());
	m_eSearch.SetFont(&m_fFont);

	return 1;
}

void CBrowseView::OnSize(UINT nType, int cx, int cy)
{

	CFormView::OnSize(nType, cx, cy);
	Layout();
}

void CBrowseView::Layout(void)
{

	if(!m_stAni.m_hWnd || !m_lcFiles.m_hWnd) return;

	CRect rcParent, rcAni, rcFiles, rcEdit, rcCheck, rcStat;

	GetClientRect(&rcParent);
	m_stAni.GetClientRect(&rcAni);
	m_lcFiles.GetClientRect(&rcFiles);
	m_eSearch.GetClientRect(&rcEdit);
	m_btCheck.GetClientRect(&rcCheck);
	m_wndStat.GetClientRect(&rcStat);

	ScreenToClient(&rcAni);
	ScreenToClient(&rcFiles);
	ScreenToClient(&rcEdit);
	ScreenToClient(&rcCheck);
	ScreenToClient(&rcStat);

	int h = 24;
	rcEdit.top		= rcParent.top + 3;
	rcEdit.bottom	= rcParent.top + 3 + h;
	rcEdit.right	= rcParent.right - 3;
	rcEdit.left		= rcParent.left + rcStat.Width() + 6;

	h = rcAni.Height();
	rcAni.top		= rcParent.top + rcEdit.Height() + 6;
	rcAni.bottom	= rcParent.top + rcEdit.Height() + h + 6;
	rcAni.right		= (rcParent.Width() / 2) - 6;
	rcAni.left		= rcParent.left  + 3;

	int h2 = 12;
	rcCheck.top		= rcParent.top + rcEdit.Height() + 6;
	rcCheck.bottom  = rcParent.top + rcEdit.Height() + 6 + h2;
	rcCheck.right   = rcParent.right - 3;
	rcCheck.left    = (rcParent.Width() / 2) + 6;

	rcFiles.top		= rcAni.bottom + 3;
	rcFiles.bottom  = rcParent.bottom -3;
	rcFiles.right	= rcParent.right - 3;
	rcFiles.left	= rcParent.left  + 3;
	
	m_stAni.MoveWindow(&rcAni);
	m_lcFiles.MoveWindow(&rcFiles);
	m_eSearch.MoveWindow(&rcEdit);
	m_btCheck.MoveWindow(&rcCheck);
	
	m_lcFiles.SetColumnWidth(0, rcParent.right > 540 ? rcParent.right - 200 : 150);
	ShowScrollBar(SB_BOTH , FALSE);
}

void CBrowseView::OnTimer(UINT nIDEvent)
{

	if(nIDEvent == TIMER_START){

		GetApp()->GetMainFrame()->m_wndDocSelector.UpdateTitle(this, GetDocument()->GetTitle());
		KillTimer(TIMER_START);
		if(m_pStatusBar){

			m_pStatusBar->SetPaneText(2, GetDocument()->m_strUser);
		}
	}
	else if(nIDEvent == TIMER_ASCIIANI){

		if(m_pStatusBar && (GetApp()->GetMainFrame()->MDIGetActive() == GetParentFrame())){

			CString strText;
			strText.LoadString(IDS_LOADFILES);
			switch(m_nAsciiAni){

				case 0:
					strText.Insert(0, "|");
					m_nAsciiAni = 1;
					break;
				case 1:
					strText.Insert(0, "/");
					m_nAsciiAni = 2;
					break;
				case 2:
					strText.Insert(0, "-");
					m_nAsciiAni = 3;
					break;
				case 3:
					strText.Insert(0, "\\");
					m_nAsciiAni = 0;
					break;
			}
			

			m_pStatusBar->SetPaneText(3, strText, 0, RGB(255, 213, 64));
			strText.Format(IDS_BROWSE_CMPLT, GetDocument()->m_strUser, m_lcFiles.GetItemCount());
			m_stAni.SetWindowText(strText);
		}
	}
	else{

		CFormView::OnTimer(nIDEvent);
	}
}


void CBrowseView::OnEnChangeSearch()
{

	UpdateData(TRUE);

	if(m_strSearch.IsEmpty()) return;

	int nStart = m_lcFiles.GetSelectionMark();

	int nFound = m_lcFiles.SafeSearch(m_strSearch, nStart+1, FALSE, TRUE);
	if((nFound == -1) && (nStart != 0)){

		nFound = m_lcFiles.SafeSearch(m_strSearch, 0, FALSE, TRUE);
	}
	if(nFound != -1){

		m_lcFiles.SetItemState(nFound, LVNI_SELECTED, LVNI_SELECTED);
		m_lcFiles.EnsureVisible(nFound, FALSE);
	}

}

void CBrowseView::OnBrowseCopyfilepath()
{

	int nSel = m_lcFiles.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		CClipboard::SetText((LPTSTR)(LPCTSTR)m_lcFiles.GetItemText(nSel, 0));
	}	
}

void CBrowseView::OnBrowseCopyfilename()
{

	int nSel = m_lcFiles.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		CString strFile = m_lcFiles.GetItemText(nSel, 0);
		strFile = strFile.Mid(strFile.ReverseFind('\\'));
		strFile.Remove('\\');

		CClipboard::SetText((LPTSTR)(LPCTSTR)strFile);
	}	
}

void CBrowseView::OnBrowseCopyfileinformation()
{

	int nSel = m_lcFiles.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		CString strFileInfo;
		// Filename,Size,Bitrate,Samplerate,Time,
		strFileInfo.Format("%s %s KB [%s kbps, %s kHz %s]", m_lcFiles.GetItemText(nSel, 0),
										  m_lcFiles.GetItemText(nSel, 1),
										  m_lcFiles.GetItemText(nSel, 2),
										  m_lcFiles.GetItemText(nSel, 3),
										  m_lcFiles.GetItemText(nSel, 4));

		CClipboard::SetText((LPTSTR)(LPCTSTR)strFileInfo);
	}	
}

void CBrowseView::OnNMRclickFileList(NMHDR *pNMHDR, LRESULT *pResult)
{

	CMenu mContextMenu;
	mContextMenu.LoadMenu(IDR_BROWSE);

	POINT point;
	GetCursorPos(&point);

	if(m_lcFiles.GetNextItem(-1, LVNI_SELECTED) >= 0){

		mContextMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
											point.x,
											point.y,
											AfxGetMainWnd());
	}

	*pResult = 0;
}


void CBrowseView::OnBnClickedPath()
{

	UpdateData(TRUE);
	m_lcFiles.DeleteAllItems();
	CString strComp;
	for(int i = 0; i < m_aBrowse.GetSize(); i++){

		//int nIcon = 2;
		//strComp = m_aBrowse[i].strFile;
		//strComp.MakeLower();
		//if((strComp.Find(".mp3") >= 0) || (strComp.Find(".wma") >= 0) || (strComp.Find(".ogg") >= 0) || (strComp.Find(".wav") >= 0)) nIcon = 0;
		//else if((strComp.Find(".mpg") >= 0) || (strComp.Find(".mpeg") >= 0) || (strComp.Find(".avi") >= 0) || (strComp.Find(".vob") >= 0)) nIcon = 1;
		//else if((strComp.Find(".jpg") >= 0) || (strComp.Find(".bmp") >= 0) || (strComp.Find(".gif") >= 0) || (strComp.Find(".jpeg") >= 0)) nIcon = 3;
		m_lcFiles.AddItem(
					m_aBrowse[i].nIcon, 
					(m_bFullPath ? (LPCTSTR)m_aBrowse[i].strPath : (LPCTSTR)m_aBrowse[i].strFile), 
					(LPCTSTR)m_aBrowse[i].strType, 
					(LPCTSTR)m_aBrowse[i].strSize, 
					(LPCTSTR)m_aBrowse[i].strBitrate, 
					(LPCTSTR)m_aBrowse[i].strSamplerate, 
					(LPCTSTR)m_aBrowse[i].strLength
				);
	}
	m_lcFiles.Sort();
	OnEnChangeSearch();
}

void CBrowseView::OnActionsReloadfiles()
{

	// Reset view
	m_lcFiles.DeleteAllItems();
	m_aBrowse.RemoveAll();
	m_bWorking = TRUE;
	m_stAni.SetWindowText("");
	m_dwNumAudio = 0;
	m_dwNumVideo = 0;
	m_dwNumImg   = 0;
	m_dwNumOther = 0;

	// re-add us as listener for wpn packets
	::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_ADDLISTENER, 0, (LPARAM)this);

	// send request
	EXT_MSG_STRUCT   msg;
	msg.wParam = (WPARAM)GetDocument()->m_dwIP;
	msg.lParam = (LPARAM)GetDocument()->m_wPort;
	msg.lpszParam = (LPCSTR)GetDocument()->m_strUser;
	::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_REBROWSE, 0, (LPARAM)&msg);
}

void CBrowseView::OnActionsSendprivatemessage()
{

	EXT_MSG_STRUCT   msg;
	msg.wParam = (WPARAM)GetDocument()->m_dwIP;
	msg.lParam = (LPARAM)GetDocument()->m_wPort;
	msg.lpszParam = (LPCSTR)GetDocument()->m_strUser;
	::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_NEWPM, 0, (LPARAM)&msg);
}

void CBrowseView::OnUpdateActionsReloadfiles(CCmdUI *pCmdUI)
{

	pCmdUI->Enable(!m_bWorking);
}

void CBrowseView::NotifyDisplayTooltip(NMHDR * pNMHDR, LRESULT * result)
{

	*result = 0;
    NM_PPTOOLTIP_DISPLAY * pNotify = (NM_PPTOOLTIP_DISPLAY*)pNMHDR;
    
	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	CString strImage;
	int nCnt = ini.GetValue("UserToolTips", "UserImageCnt", 0);
	
	CString strLabel, strName;
	
	for(int i = 0; i < nCnt; i++){

		strLabel.Format("UserName_%d", i);
		strName = ini.GetValue("UserToolTips", strLabel, "");

		if(strName.GetLength() && Util::WildcardMatch(GetDocument()->m_strUser, strName)){

			strLabel.Format("UserImage_%d", i);
			strImage = ini.GetValue("UserToolTips", strLabel, "");
			break;
		}
	}
	if(i >= nCnt){

		strImage = g_sSettings.GetUserlistDefaultImage();
	}

	if(strImage.IsEmpty()){

		strLabel = " ";
	}
	else{
		
		strLabel.Format("<bmp file=\"%s\" mask=\"#FF00FF\">", strImage);
	}

	strImage.Format("<icon idres=%d width=16 height=16>", IDR_BROWSEVIEW_TMPL);

	pNotify->ti->sTooltip = g_sSettings.GetToolFiles();
	pNotify->ti->sTooltip.Replace("%NAME%", GetDocument()->m_strUser);
	pNotify->ti->sTooltip.Replace("%NUMFILES%", Util::Formatint(m_lcFiles.GetItemCount()));
	pNotify->ti->sTooltip.Replace("%NUMAUDIO%", Util::Formatint(m_dwNumAudio));
	pNotify->ti->sTooltip.Replace("%NUMVIDEO%", Util::Formatint(m_dwNumVideo));
	pNotify->ti->sTooltip.Replace("%NUMIMG%", Util::Formatint(m_dwNumImg));
	pNotify->ti->sTooltip.Replace("%NUMOTHER%", Util::Formatint(m_dwNumOther));
	pNotify->ti->sTooltip.Replace("%FILEICON%", strImage);
	pNotify->ti->sTooltip.Replace("%USERIMAGE%", strLabel);

	pNotify->ti->nEffect = g_sSettings.GetToolEffect();
	pNotify->ti->nGranularity = g_sSettings.GetToolGran();
	pNotify->ti->nTransparency = g_sSettings.GetToolTrans();
	pNotify->ti->crBegin = g_sSettings.GetToolBg1();
	pNotify->ti->crMid   = g_sSettings.GetToolBg2();
	pNotify->ti->crEnd   = g_sSettings.GetToolBg3();
}

