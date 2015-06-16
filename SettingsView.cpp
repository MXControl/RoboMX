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
#include "SettingsView.h"
#include "MainFrm.h"
#include "Settings.h"
#include ".\settingsview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSettings g_sSettings;

UINT UWM_LOAD_SETTINGS = ::RegisterWindowMessage("UWM_LOAD_SETTINGS_A1C54C04-EF90-43D7-9050-6CC3BCA18D22");

/////////////////////////////////////////////////////////////////////////////
// CSettingsView

IMPLEMENT_DYNCREATE(CSettingsView, CFormView)


CSettingsView::CSettingsView()
	: CFormView(CSettingsView::IDD)
	, m_bExpert(FALSE)
{
	
	m_pStatusBar = NULL;
}

CSettingsView::~CSettingsView()
{
}

void CSettingsView::DoDataExchange(CDataExchange* pDX) 
{

	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAT, m_lcCat);

	DDX_Control(pDX, IDC_OK, m_btOK);
	DDX_Control(pDX, IDC_APPLY, m_btApply);
	DDX_Control(pDX, IDC_CANCEL, m_btCancel);
	DDX_Check(pDX, IDC_EXPERT, m_bExpert);
	DDX_Control(pDX, IDC_EXPERT, m_btExpert);
}

BEGIN_MESSAGE_MAP(CSettingsView, CFormView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_OK, OnOK)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CAT, OnItemchangedCat)
	ON_BN_CLICKED(IDC_CANCEL, OnBnClickedCancel)
	ON_NOTIFY (UDM_TOOLTIP_DISPLAY, NULL, NotifyDisplayTooltip)
	ON_BN_CLICKED(IDC_EXPERT, OnBnClickedExpert)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsView drawing

void CSettingsView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CSettingsView diagnostics

#ifdef _DEBUG
void CSettingsView::AssertValid() const
{
	CView::AssertValid();
}

void CSettingsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSettingsView message handlers
#define TIMER_LOAD 10000

void CSettingsView::OnInitialUpdate() 
{
	
	CFormView::OnInitialUpdate();
	
    GetParentFrame()->RecalcLayout();
	if(!g_sSettings.GetMaxi()){

		ResizeParentToFit(FALSE);
	}
	m_pStatusBar = (CColorStatusBar *)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	GetApp()->GetMainFrame()->m_wndDocSelector.AddButton( this, IDR_SETTINGS );

	if(m_pStatusBar){

		m_pStatusBar->SetPaneText(2, "");
		m_pStatusBar->SetPaneText(3, "");
		m_pStatusBar->SetPaneText(4, "");
	}

	m_bExpert = g_sSettings.GetShowExpert();
	UpdateData(FALSE);

	HIMAGELIST hList = ImageList_Create(24,24, ILC_COLOR16 |ILC_MASK , 12, 1);
	m_cImageList.Attach(hList);

	CBitmap cBmp;
	cBmp.LoadBitmap(IDB_PREFERENCES);
	m_cImageList.Add(&cBmp, RGB(255,0, 255));
	cBmp.DeleteObject();
	
	m_lcCat.SetImageList(&m_cImageList,LVSIL_SMALL);
	m_lcCat.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lcCat.InsertColumn(0, "", LVCFMT_CENTER, 120, 0);
	//m_lcCat.SetIconSpacing(64,32);
	
	CRect rcClient;
	GetDlgItem(IDC_STATIC_CLIENT)->GetWindowRect(&rcClient);
	ScreenToClient(&rcClient);

	cfgEmo.Create(IDD_CFG_EMOTICONS, this);
	cfgEmo.MoveWindow(rcClient);
	cfgEmo.ShowWindow(SW_SHOW);
	cfgEmo.m_nIcon     = 6;
	cfgEmo.m_uDescID   = IDS_CFG_EMO;
	cfgEmo.m_bIsExpert = FALSE;

	cfgWPN.Create(IDD_CFG_WPN, this);
	cfgWPN.MoveWindow(rcClient);
	cfgWPN.m_nIcon     = 10;
	cfgWPN.m_uDescID   = IDS_CFG_WPN;
	cfgWPN.m_bIsExpert = TRUE;

	cfgGeneral.Create(IDD_CFG_GENERAL, this);
	cfgGeneral.MoveWindow(rcClient);
	cfgGeneral.m_nIcon     = 0;
	cfgGeneral.m_uDescID   = IDS_CFG_GENERAL;
	cfgGeneral.m_bIsExpert = FALSE;

	cfgWelcome.Create(IDD_CFG_WELCOME, this);
	cfgWelcome.MoveWindow(rcClient);
	cfgWelcome.m_nIcon     = 3;
	cfgWelcome.m_uDescID   = IDS_CFG_WELCOME;
	cfgWelcome.m_bIsExpert = FALSE;

	cfgQuick.Create(IDD_CFG_QUICKMSG, this);
	cfgQuick.MoveWindow(rcClient);
	cfgQuick.m_nIcon     = 4;
	cfgQuick.m_uDescID   = IDS_CFG_QUICKMSG;
	cfgQuick.m_bIsExpert = FALSE;

	cfgFormat.Create(IDD_CFG_FORMAT, this);
	cfgFormat.MoveWindow(rcClient);
	cfgFormat.m_nIcon     = 1;
	cfgFormat.m_uDescID   = IDS_CFG_FORMAT;
	cfgFormat.m_bIsExpert = FALSE;

	cfgColor.Create(IDD_COLORS, this);
	cfgColor.MoveWindow(rcClient);
	cfgColor.m_nIcon     = 2;
	cfgColor.m_uDescID   = IDS_CFG_COLORS;
	cfgColor.m_bIsExpert = FALSE;

	cfgMsg.Create(IDD_MESSAGES, this);
	cfgMsg.MoveWindow(rcClient);
	cfgMsg.m_nIcon     = 5;
	cfgMsg.m_uDescID   = IDS_CFG_MSG;
	cfgMsg.m_bIsExpert = FALSE;

	cfgSfx.Create(IDD_CFG_SYSSOUNDS, this);
	cfgSfx.MoveWindow(rcClient);
	cfgSfx.m_nIcon     = 7;
	cfgSfx.m_uDescID   = IDS_CFG_SND;
	cfgSfx.m_bIsExpert = FALSE;

	cfgChatSfx.Create(IDD_CFG_CHATSOUNDS, this);
	cfgChatSfx.MoveWindow(rcClient);
	cfgChatSfx.m_nIcon     = 8;
	cfgChatSfx.m_uDescID   = IDS_CFG_CHATSOUND;
	cfgChatSfx.m_bIsExpert = FALSE;

	cfgBar.Create(IDD_CFG_CHANNELBAR, this);
	cfgBar.MoveWindow(rcClient);
	cfgBar.m_nIcon     = 12;
	cfgBar.m_uDescID   = IDS_CFG_CHANNELBAR;
	cfgBar.m_bIsExpert = TRUE;

	cfgUserTool.Create(IDD_CFG_USERTOOL, this);
	cfgUserTool.MoveWindow(rcClient);
	cfgUserTool.m_nIcon     = 13;
	cfgUserTool.m_uDescID   = IDS_CFG_USERTOOL;
	cfgUserTool.m_bIsExpert = TRUE;

	cfgUserList.Create(IDD_CFG_USERLIST, this);
	cfgUserList.MoveWindow(rcClient);
	cfgUserList.m_nIcon     = 14;
	cfgUserList.m_uDescID   = IDS_CFG_USERLIST;
	cfgUserList.m_bIsExpert = TRUE;

	cfgAway.Create(IDD_CFG_AWAY, this);
	cfgAway.MoveWindow(rcClient);
	cfgAway.m_nIcon     = 15;
	cfgAway.m_uDescID   = IDS_CFG_AWAY;
	cfgAway.m_bIsExpert = FALSE;

	cfgFun.Create(IDD_CFG_FUN, this);
	cfgFun.MoveWindow(rcClient);
	cfgFun.m_nIcon     = 9;
	cfgFun.m_uDescID   = IDS_CFG_FUN;
	cfgFun.m_bIsExpert = TRUE;

    cfgExt.Create(IDD_EXTENSIONS, this);
	cfgExt.MoveWindow(rcClient);
	cfgExt.m_nIcon     = 11;
	cfgExt.m_uDescID   = IDS_CFG_EXT;
	cfgExt.m_bIsExpert = FALSE;

	AddCategory(&cfgGeneral); //, 0, IDS_CFG_GENERAL);
	AddCategory(&cfgFormat); //, 1, IDS_CFG_FORMAT);
	AddCategory(&cfgColor); //, 2, IDS_CFG_COLORS);
	AddCategory(&cfgWelcome); //, 3, IDS_CFG_WELCOME);
	AddCategory(&cfgQuick); //, 4, IDS_CFG_QUICKMSG);
	AddCategory(&cfgMsg); //, 5, IDS_CFG_MSG);
	AddCategory(&cfgAway); //, 15, IDS_CFG_AWAY);
	AddCategory(&cfgEmo); //, 6, IDS_CFG_EMO);
	AddCategory(&cfgSfx); //, 7, IDS_CFG_SND);
	AddCategory(&cfgChatSfx); //, 8, IDS_CFG_CHATSOUND);
	AddCategory(&cfgBar); //, 12, IDS_CFG_CHANNELBAR);
	AddCategory(&cfgUserList); //, 14, IDS_CFG_USERLIST);
	AddCategory(&cfgUserTool); //, 13, IDS_CFG_USERTOOL);
	AddCategory(&cfgFun); //, 9, IDS_CFG_FUN);
	AddCategory(&cfgWPN); //, 10, IDS_CFG_WPN);
	AddCategory(&cfgExt); //, 11, IDS_CFG_EXT);
	m_lcCat.SetItemState(0, LVNI_SELECTED, LVNI_SELECTED);
	cfgGeneral.ShowWindow(SW_SHOW);

	Layout();
}

void CSettingsView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if(m_pStatusBar){

		m_pStatusBar->SetPaneText(2, "");
		m_pStatusBar->SetPaneText(3, "");
		m_pStatusBar->SetPaneText(4, "");
	}
}

void CSettingsView::OnDestroy() 
{

	GetApp()->GetMainFrame()->m_wndDocSelector.RemoveButton(this);
	GetApp()->GetMainFrame()->m_bSettings = FALSE;
	CFormView::OnDestroy();
}

void CSettingsView::OnApply() 
{

	/*for(int i = 0; i < m_aDlgs.GetSize(); i++){

		((CCfgDlg*)m_aDlgs[i])->Apply();
	}*/

	POSITION pos = m_aDlgs.GetStartPosition();
	CCfgDlg* dlg = NULL;
	CString strTemp;
	while(pos){

		m_aDlgs.GetNextAssoc(pos, strTemp, dlg);
		dlg->Apply();
	}
	g_sSettings.Save();
	g_sSettings.Load();

	GetApp()->GetMainFrame()->SendMessage(UWM_LOAD_SETTINGS, 0, 0);
}


void CSettingsView::OnOK() 
{

	OnApply();
	GetDocument()->OnCloseDocument();
}

void CSettingsView::OnItemchangedCat(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int n = m_lcCat.GetNextItem(-1, LVNI_SELECTED);

	if((n >= 0) && (n < m_aDlgs.GetSize())){

		/*for(int i = 0; i < m_aDlgs.GetSize(); i++){

			((CCfgDlg*)m_aDlgs[i])->ShowWindow(SW_HIDE);
		}*/
		POSITION pos = m_aDlgs.GetStartPosition();
		CCfgDlg* dlg; CString strTemp;
		while(pos){

			m_aDlgs.GetNextAssoc(pos, strTemp, dlg);
			dlg->ShowWindow(SW_HIDE);
		}

		//((CCfgDlg*)m_aDlgs[n])->ShowWindow(SW_SHOW);
		strTemp = m_lcCat.GetItemText(n, 0);
		if(m_aDlgs.Lookup(strTemp, dlg)){

			dlg->ShowWindow(SW_SHOW);
		}
	}
	*pResult = 0;
}

void CSettingsView::AddCategory(CCfgDlg *pCat) //, int nIcon, UINT nID)
{
	
	CString strName;
	strName.LoadString(pCat->m_uDescID);
	//m_aDlgs.Add(pCat);

	m_aDlgs.SetAt(strName, pCat);

	if(!m_bExpert && pCat->m_bIsExpert) return;

	m_lcCat.InsertItem(m_lcCat.GetItemCount(), strName, pCat->m_nIcon);
}
 
void CSettingsView::OnBnClickedCancel()
{

	GetDocument()->OnCloseDocument();
}

void CSettingsView::OnSize(UINT nType, int cx, int cy)
{

	CFormView::OnSize(nType, cx, cy);
	Layout();
}

void CSettingsView::Layout(void)
{

	if(!m_btOK.m_hWnd || !m_btCancel.m_hWnd || !m_btApply.m_hWnd || !m_lcCat.m_hWnd || !m_btExpert.m_hWnd) return;

	CRect rcParent, rcOK, rcApply, rcCancel, rcCat, rcExpert;

	GetClientRect(&rcParent);	

	// min 473h/ 648w
	if(rcParent.Height() < 463) return;
    if(rcParent.Width()  < 602) return;

	ShowScrollBar(SB_BOTH , FALSE);

	m_lcCat.GetClientRect(&rcCat);
	m_btOK.GetClientRect(&rcOK);
	m_btApply.GetClientRect(&rcApply);
	m_btCancel.GetClientRect(&rcCancel);
	m_btExpert.GetClientRect(&rcExpert);
	
	ScreenToClient(&rcCat);
	ScreenToClient(&rcOK);
	ScreenToClient(&rcApply);
	ScreenToClient(&rcCancel);
	ScreenToClient(&rcExpert);

	int w = rcExpert.Width();
	rcExpert.bottom = rcParent.bottom - 4;
	rcExpert.top    = rcParent.bottom - 27;
	rcExpert.left   = rcParent.left + 4;
	rcExpert.right  = rcParent.left + w + 4;

	rcCat.top		 = rcParent.top + 4;
	rcCat.bottom	 = rcParent.bottom  - 4 - rcExpert.Height();
	rcCat.left		 = rcParent.left + 4;
	rcCat.right		 = rcParent.left + 120;

	// buttons width = 75
	// buttons height = 23
	rcCancel.bottom = rcParent.bottom - 4;
	rcCancel.top    = rcParent.bottom - 27;
	rcCancel.right  = rcParent.right - 4;
	rcCancel.left   = rcParent.right - 79;

	rcApply.bottom  = rcParent.bottom - 4;
	rcApply.top     = rcParent.bottom - 27;
	rcApply.right   = rcParent.right - 79;
	rcApply.left    = rcParent.right - 2*79;

	rcOK.bottom		= rcParent.bottom - 4;
	rcOK.top		= rcParent.bottom - 27;
	rcOK.right		= rcParent.right - 2*79;
	rcOK.left		= rcParent.right - 3*79;


	m_btExpert.MoveWindow(rcExpert);
	m_lcCat.MoveWindow(rcCat);
	m_btOK.MoveWindow(rcOK);
	m_btCancel.MoveWindow(rcCancel);
	m_btApply.MoveWindow(rcApply);

}

void CSettingsView::NotifyDisplayTooltip(NMHDR * pNMHDR, LRESULT * result)
{

	*result = 0;
    NM_PPTOOLTIP_DISPLAY * pNotify = (NM_PPTOOLTIP_DISPLAY*)pNMHDR;
    
	CString strImage;
	strImage.Format("<icon idres=%d width=16 height=16>", IDR_SETTINGS);
	pNotify->ti->sTooltip = g_sSettings.GetToolSettings();
	pNotify->ti->sTooltip.Replace("%SETTINGSICON%", strImage);

	pNotify->ti->nEffect = g_sSettings.GetToolEffect();
	pNotify->ti->nGranularity = g_sSettings.GetToolGran();
	pNotify->ti->nTransparency = g_sSettings.GetToolTrans();
	pNotify->ti->crBegin = g_sSettings.GetToolBg1();
	pNotify->ti->crMid   = g_sSettings.GetToolBg2();
	pNotify->ti->crEnd   = g_sSettings.GetToolBg3();
}



void CSettingsView::OnBnClickedExpert()
{

	UpdateData(TRUE);
	m_lcCat.DeleteAllItems();
	AddCategory(&cfgGeneral); //, 0, IDS_CFG_GENERAL);
	AddCategory(&cfgFormat); //, 1, IDS_CFG_FORMAT);
	AddCategory(&cfgColor); //, 2, IDS_CFG_COLORS);
	AddCategory(&cfgWelcome); //, 3, IDS_CFG_WELCOME);
	AddCategory(&cfgQuick); //, 4, IDS_CFG_QUICKMSG);
	AddCategory(&cfgMsg); //, 5, IDS_CFG_MSG);
	AddCategory(&cfgAway); //, 15, IDS_CFG_AWAY);
	AddCategory(&cfgEmo); //, 6, IDS_CFG_EMO);
	AddCategory(&cfgSfx); //, 7, IDS_CFG_SND);
	AddCategory(&cfgChatSfx); //, 8, IDS_CFG_CHATSOUND);
	AddCategory(&cfgBar); //, 12, IDS_CFG_CHANNELBAR);
	AddCategory(&cfgUserList); //, 14, IDS_CFG_USERLIST);
	AddCategory(&cfgUserTool); //, 13, IDS_CFG_USERTOOL);
	AddCategory(&cfgFun); //, 9, IDS_CFG_FUN);
	AddCategory(&cfgWPN); //, 10, IDS_CFG_WPN);
	AddCategory(&cfgExt); //, 11, IDS_CFG_EXT);
	m_lcCat.SetItemState(0, LVNI_SELECTED, LVNI_SELECTED);
	cfgGeneral.ShowWindow(SW_SHOW);

	g_sSettings.SetShowExpert(m_bExpert);
}
