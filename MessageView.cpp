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
#include "MessageView.h"

#include "Settings.h"
#include "EmoticonManager.h"
#include "RichElement.h"
#include <mmsystem.h>
#include "wpnpclient.h"
#include "MainFrm.h"
#include "RoboEx.h"
#include "Tokenizer.h"
#include "Messagedoc.h"
#include ".\messageview.h"
#include "InputRequest.h"
#include "SystemInfo.h"
#include "LinkWarning.h"
#include "Clipboard.h"
#include "Ini.h"

// CMessageView
#define TIMER_ID_WAITACK	10123
#define TIMER_START			10126

#define WM_PMSG				WM_APP+5

extern CEmoticonManager g_eEmoticons;
extern CSettings g_sSettings;
extern CSystemInfo g_sSystem;
extern UINT UWM_LOAD_SETTINGS;
extern UINT UWM_WPNNOTIFY;
extern UINT UWM_ADDLISTENER;
extern UINT UWM_REMLISTENER;
extern UINT UWM_BROWSE;

IMPLEMENT_DYNCREATE(CMessageView, CFormView)

CMessageView::CMessageView()
	: CFormView(CMessageView::IDD)
{

	m_strText		= _T("");
	m_bKeep			= TRUE;
	m_nWait			= 0;
	m_bMsgPending	= FALSE;
	m_nAway			= 0;
	m_pStatusBar	= 0;
	m_bUnread       = FALSE;
	m_dwMsgCount    = 0;
	m_dwMsgSentCount = 0;
}

CMessageView::~CMessageView()
{

	if(m_fFont.m_hObject) m_fFont.DeleteObject();
	if(m_fBold.m_hObject) m_fBold.DeleteObject();
	if(m_fItalic.m_hObject) m_fItalic.DeleteObject();
	if(m_fUnderline.m_hObject) m_fUnderline.DeleteObject();
}

void CMessageView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_INPUT, m_eInput);
	DDX_Text(pDX, IDC_INPUT, m_strText);
	DDV_MaxChars(pDX, m_strText, 400);
	DDX_Check(pDX, IDC_KEEP_OPEN, m_bKeep);
	DDX_Control(pDX, IDC_KEEP_OPEN, m_btKeep);
	DDX_Control(pDX, ID_SEND, m_btOK);
	DDX_Control(pDX, ID_CLOSEPM, m_btCancel);
	DDX_Control(pDX, IDC_STATIC_ANI, m_wndAni);
	DDX_Control(pDX, IDC_BOLD, m_btBold);
	DDX_Control(pDX, IDC_UNDERLINE, m_btUnderline);
	DDX_Control(pDX, IDC_ITALIC, m_btItalic);
	DDX_Control(pDX, IDC_COLOR, m_btColor);
	DDX_Control(pDX, IDC_STATIC1, m_wndNotics);
}

BEGIN_MESSAGE_MAP(CMessageView, CFormView)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_SEND, OnBnClickedOk)
	ON_BN_CLICKED(ID_CLOSEPM, OnBnClickedCancel)
	ON_REGISTERED_MESSAGE(UWM_LOAD_SETTINGS, OnReloadCfg)
	ON_REGISTERED_MESSAGE(UWM_WPNNOTIFY, OnWPNNotify)
	ON_COMMAND(ID_USER_BROWSE, OnActionsReloadfiles)
	ON_COMMAND(ID_ACTIONS_SETAWAYMESSAGE, OnActionsSetawaymessage)
	ON_COMMAND(ID_ACTIONS_IGNOREUSER, OnActionsIgnoreuser)
	ON_COMMAND(ID_ACTIONS_TRUSTUSER, OnActionsTrustuser)
	ON_COMMAND(ID_SHIFTENTER, OnShiftenter)
	ON_COMMAND(ID_CHATROOM_DISPLAYINCHANNEL_WINAMPSONG, OnChatroomDisplayinchannelWinampsong)
	ON_UPDATE_COMMAND_UI(ID_CHATROOM_DISPLAYINCHANNEL_WINAMPSONG, OnUpdateChatroomDisplayinchannelWinampsong)
	ON_COMMAND(ID_DISPLAYINCHANNEL_ONLINETIME, OnDisplayinchannelOnlinetime)
	ON_COMMAND(ID_CHATROOM_DISPLAYINCHANNEL_SYSTEMUPTIME, OnChatroomDisplayinchannelSystemuptime)
	ON_COMMAND(ID_CHATROOM_DISPLAYINCHANNEL_SYSTEMINFO, OnChatroomDisplayinchannelSysteminfo)
	ON_COMMAND(ID_DISPLAYINCHANNEL_EXTENDEDSYSTEMINFO, OnDisplayinchannelExtendedsysteminfo)
	ON_RICH_NOTIFY(RVN_RCLICK, IDC_MESSAGES, OnRclickMsgs)
	ON_COMMAND(ID_OPEN_OPEN, OnOpenURL)
	ON_COMMAND(ID_OPEN_OPENINNEWBROWSER, OnOpenURLinnewbrowser)
	ON_COMMAND(ID_COPY_ADDRESS, OnCopyAddress)
	ON_BN_CLICKED(IDC_BOLD, OnBnClickedBold)
	ON_BN_CLICKED(IDC_UNDERLINE, OnBnClickedUnderline)
	ON_BN_CLICKED(IDC_ITALIC, OnBnClickedItalic)
	ON_BN_CLICKED(IDC_COLOR, OnBnClickedColor)
	ON_NOTIFY (UDM_TOOLTIP_DISPLAY, NULL, NotifyDisplayTooltip)
END_MESSAGE_MAP()


// CMessageView diagnostics

#ifdef _DEBUG
void CMessageView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMessageView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMessageDoc* CMessageView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMessageDoc)));
	return (CMessageDoc*)m_pDocument;
}
#endif //_DEBUG


// CMessageView message handlers
void CMessageView::OnInitialUpdate()
{

	CFormView::OnInitialUpdate();

    GetParentFrame()->RecalcLayout();

	GetApp()->GetMainFrame()->m_wndDocSelector.AddButton(this, IDR_MESSAGEVIEW_TMPL);
	::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_ADDLISTENER, 0, (LPARAM)this);
	
	m_pStatusBar = (CColorStatusBar *)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	
	if(m_pStatusBar){

		m_pStatusBar->SetPaneText(2, "");
		m_pStatusBar->SetPaneText(3, "");
		m_pStatusBar->SetPaneText(4, "");
	}

	if(GetDocument()->m_strUser.GetLength()){

		CString strTitle;
		strTitle.Format(IDS_SENDPM, GetDocument()->m_strUser); 
		GetDocument()->SetTitle(strTitle);
	}
	m_fFont.CreateFont(15, 6, 0, 0, FW_BOLD, 0, 0, 0, g_sSettings.GetCharSet(), OUT_STRING_PRECIS, 
		CLIP_STROKE_PRECIS, PROOF_QUALITY, FF_DONTCARE, g_sSettings.GetFont());

	m_fBold.CreateFont(15, 6, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
		g_sSettings.GetCharSet(), OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, g_sSettings.GetFont());
	
	m_fItalic.CreateFont(15, 6, 0, 0, FW_NORMAL, TRUE, FALSE, FALSE,
		g_sSettings.GetCharSet(), OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, g_sSettings.GetFont());
	
	
	m_fUnderline.CreateFont(15, 6, 0, 0, FW_NORMAL, FALSE, TRUE, FALSE,
		g_sSettings.GetCharSet(), OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_DONTCARE, g_sSettings.GetFont());

	m_btBold.SetFont(&m_fBold);
	m_btItalic.SetFont(&m_fItalic);
	m_btUnderline.SetFont(&m_fUnderline);

	m_eInput.SetBkColor(g_sSettings.GetRGBBg());
	m_eInput.SetExtended();
	m_eInput.SetExtMenu();
	m_eInput.SetFont(&m_fFont);

	CRect rc(2, 2, 520, 217);

	m_rOut.Create(WS_CHILD|WS_VISIBLE, rc, this, IDC_MESSAGES);
	m_rOut.SetDocument(&m_rDoc);
	m_rOut.SetSelectable(TRUE);
	m_rOut.SetFollowBottom(TRUE);
	m_rDoc.m_szMargin = CSize(1, 10);
	m_rDoc.m_crBackground = g_sSettings.GetRGBBg();
	m_rDoc.m_crLink       = g_sSettings.GetRGBLink();

	m_rDoc.CreateFonts(g_sSettings.GetFont(), g_sSettings.GetFontSize());

	SetTimer(TIMER_START, 100, NULL);
	Layout();
}

void CMessageView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	if(m_pStatusBar){

		m_pStatusBar->SetPaneText(2, GetDocument()->m_strUser);
		m_pStatusBar->SetPaneText(3, "");
		m_pStatusBar->SetPaneText(4, "");
	}
	if(GetDocument()->m_strUser.GetLength()){

		CString strTitle;
		strTitle.Format(IDS_SENDPM, GetDocument()->m_strUser); 
		GetDocument()->SetTitle(strTitle);
	}
	if(m_bUnread){

		m_bUnread = FALSE;
		GetApp()->GetMainFrame()->StopPMAni();
	}
}

LRESULT CMessageView::OnReloadCfg(WPARAM w, LPARAM l)
{

	m_fFont.DeleteObject();
	m_fFont.CreateFont(15, 6, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_STRING_PRECIS, 
		CLIP_STROKE_PRECIS, PROOF_QUALITY, FF_DONTCARE, g_sSettings.GetFont());

	m_eInput.SetFont(&m_fFont);
	m_eInput.SetBkColor(g_sSettings.GetRGBBg());
	m_rDoc.m_crBackground = g_sSettings.GetRGBBg();
	m_rDoc.m_crLink       = g_sSettings.GetRGBLink();

	m_rDoc.CreateFonts(g_sSettings.GetFont(), g_sSettings.GetFontSize());

	return 1;
}

void CMessageView::OnSize(UINT nType, int cx, int cy)
{

	CFormView::OnSize(nType, cx, cy);
	Layout();
}

void CMessageView::OnDestroy()
{

	::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_REMLISTENER, 0, (LPARAM)this);
	GetApp()->GetMainFrame()->m_wndDocSelector.RemoveButton(this);
	if(m_bUnread){

		GetApp()->GetMainFrame()->StopPMAni();
	}
	CFormView::OnDestroy();
}

void CMessageView::OnBnClickedOk()
{

	if(UpdateData(TRUE)){

		if(m_strText.IsEmpty()) return;

		EXT_MSG_STRUCT msg;
		msg.wParam    = GetDocument()->m_dwIP;
		msg.lParam	  = GetDocument()->m_wPort;
		msg.lpszParam = GetDocument()->m_strUser;
		msg.lpszMsg   = m_strText;

		AfxGetMainWnd()->SendMessage(WM_PMSG, 0, (LPARAM)&msg);
		m_nAway		  = 0;
		if(!m_bKeep){

			GetDocument()->OnCloseDocument();
		}
		else{

			WriteOutgoing(m_strText);
			m_nWait = 0;
			m_bMsgPending = TRUE;
			SetTimer(TIMER_ID_WAITACK, 1000, NULL);
			m_strText.Empty();
			UpdateData(FALSE);
		}
	}
}

void CMessageView::OnBnClickedCancel()
{

	GetDocument()->OnCloseDocument();
}

void CMessageView::WriteOutgoing(CString strText)
{
	
	strText.Replace("\r\n", "\n");

	CString strNickname;
	
	if(g_sSettings.GetPrintTime()){

		strNickname.Format(IDS_PM_TIME_YOUWRITE, Util::GetMyLocalTime());
	}
	else{

		strNickname.LoadString(IDS_PM_NOTIME_YOUWRITE);
	}
	g_eEmoticons.FormatText(&m_rDoc,  strNickname, TRUE, 
							FALSE, FALSE, 	FALSE, g_sSettings.GetRGBNormalName(), 
							g_sSettings.GetRGBBg());
	g_eEmoticons.FormatText(&m_rDoc,  " " + strText, TRUE, 
		                    FALSE, FALSE, FALSE, g_sSettings.GetRGBNormalMsg(),
							g_sSettings.GetRGBBg(), FALSE, TRUE);

	m_rDoc.Add(retNewline, NEWLINE_FORMAT);
	m_rOut.InvalidateIfModified();
	
	if(g_sSettings.GetSfxChatSfx() &&
		(GetApp()->GetMainFrame()->GetActiveFrame() == GetParentFrame())){
		
		CString strNoCase = strText;
		strNoCase.MakeLower();
		for(int i = 0; i < g_sSettings.m_aSounds.GetSize(); i++){

			if(Util::WildcardMatch(strNoCase, g_sSettings.m_aSounds[i].strTrigger, TRUE)){

				PlaySound(g_sSettings.m_aSounds[i].strSound, NULL, SND_FILENAME|SND_ASYNC|SND_NODEFAULT );
				break;
			}
		}
	}

}

void CMessageView::WriteIncoming(CString strText)
{

	strText.Replace("\r\n", "\n");

	CString strNickname;
	
	if(g_sSettings.GetPrintTime()){

		strNickname.Format(IDS_PM_TIME_WRITES, Util::GetMyLocalTime(), GetDocument()->m_strUser);
	}
	else{

		strNickname.Format(IDS_PM_NOTIME_WRITES, GetDocument()->m_strUser);
	}
	g_eEmoticons.FormatText(&m_rDoc,  strNickname, TRUE, 
							FALSE, FALSE, FALSE, g_sSettings.GetRGBNormalName(), 
							g_sSettings.GetRGBBg());
	g_eEmoticons.FormatText(&m_rDoc, " " + strText, TRUE, 
		                    FALSE, FALSE, FALSE, g_sSettings.GetRGBNormalMsg(), 
							g_sSettings.GetRGBBg(), FALSE, TRUE);

	m_rDoc.Add(retNewline, NEWLINE_FORMAT);

	m_rOut.InvalidateIfModified();

	WINDOWPLACEMENT wp;
	GetApp()->GetMainFrame()->GetWindowPlacement(&wp);
	if((wp.showCmd == SW_SHOWMINIMIZED) || 
		GetApp()->GetMainFrame()->MDIGetActive() != GetParentFrame()){

		GetApp()->GetMainFrame()->StartPMAni();
		GetApp()->GetMainFrame()->m_wndDocSelector.SetHiLite(this);
		m_bUnread = TRUE;
	}

	if(g_sSettings.GetSoundFX()){

		if(!g_sSettings.GetSfxPM().IsEmpty())
			PlaySound(g_sSettings.GetSfxPM(), NULL, SND_FILENAME|SND_ASYNC|SND_NODEFAULT );
	}
}

void CMessageView::Layout(void)
{

	if(!m_eInput.m_hWnd || !m_rOut.m_hWnd) return;

	CRect rcParent, rcEdit, rcOut, rcCheck, rcSend, rcCancel, rcAni, rcBold, rcItalic, rcUnderline, rcColor, rcNotice;

	GetClientRect(&rcParent);
	m_eInput.GetClientRect(&rcEdit);
	m_rOut.GetClientRect(&rcOut);
	m_btOK.GetClientRect(&rcSend);
	m_btCancel.GetClientRect(&rcCancel);
	m_btKeep.GetClientRect(&rcCheck);
	m_wndAni.GetClientRect(&rcAni);
	m_btBold.GetClientRect(&rcBold);
	m_btItalic.GetClientRect(&rcItalic);
	m_btUnderline.GetClientRect(&rcUnderline);
	m_btColor.GetClientRect(&rcColor);
	m_wndNotics.GetClientRect(&rcNotice);

	ScreenToClient(&rcEdit);
	ScreenToClient(&rcAni);
	ScreenToClient(&rcOut);
	ScreenToClient(&rcSend);
	ScreenToClient(&rcCancel);
	ScreenToClient(&rcCheck);
	ScreenToClient(&rcBold);
	ScreenToClient(&rcItalic);
	ScreenToClient(&rcUnderline);
	ScreenToClient(&rcColor);
	ScreenToClient(&rcNotice);

	int h = 0, w = 0;
		
	h = rcCancel.Height();
	w = rcCancel.Width();
	rcCancel.right	= rcParent.right - 3;
	rcCancel.left	= rcCancel.right - w;
	rcCancel.bottom = rcParent.bottom - 3;
	rcCancel.top    = rcCancel.bottom - h;

	rcSend.right	= rcCancel.left - 5;
	rcSend.left		= rcSend.right - w;
	rcSend.bottom	= rcParent.bottom - 3;
	rcSend.top		= rcSend.bottom - h;

	h = rcCheck.Height();
	w = rcCheck.Width();

	rcCheck.bottom = rcParent.bottom - 3;
	rcCheck.top    = rcCheck.bottom - h;
	rcCheck.right  = rcParent.left + w + 3;
	rcCheck.left   = rcParent.left + 3;

	h = rcAni.Height();
	rcAni.right     = rcSend.left - 3;
	rcAni.left      = rcCheck.right + 3;
	rcAni.bottom    = rcParent.bottom - 3;
	rcAni.top       = rcAni.bottom - h;

	h = 80;
	w = rcEdit.Width();
	rcEdit.bottom	= rcSend.top - 3;
	rcEdit.top		= rcSend.top - h - 3;
	rcEdit.right	= rcParent.right - 3;
	rcEdit.left		= rcParent.left + 3;
	
	rcBold.left		= rcParent.left + 3;
	rcBold.right    = rcParent.left + 23;
	rcBold.bottom   = rcEdit.top - 3;
	rcBold.top		= rcEdit.top - 23;

	rcItalic.left	= rcBold.right + 3;
	rcItalic.right  = rcBold.right + 23;
	rcItalic.bottom = rcEdit.top - 3;
	rcItalic.top	= rcEdit.top - 23;

	rcUnderline.left	= rcItalic.right + 3;
	rcUnderline.right   = rcItalic.right + 23;
	rcUnderline.bottom  = rcEdit.top - 3;
	rcUnderline.top		= rcEdit.top - 23;

	rcColor.left	= rcUnderline.right + 3;
	rcColor.right   = rcUnderline.right + 43;
	rcColor.bottom  = rcEdit.top - 3;
	rcColor.top		= rcEdit.top - 23;
	
	rcNotice.left   = rcColor.right + 3;
	rcNotice.right  = rcParent.right - 3;
	rcNotice.bottom = rcEdit.top - 3;
	rcNotice.top    = rcEdit.top - 23;

	rcOut.right		= rcParent.right - 3;
	rcOut.bottom	= rcBold.top - 3;
	rcOut.left		= rcParent.left + 3;
	rcOut.top		= rcParent.top + 3;

	m_eInput.MoveWindow(rcEdit);
	m_rOut.MoveWindow(rcOut);
	m_btOK.MoveWindow(rcSend);
	m_btCancel.MoveWindow(rcCancel);
	m_btKeep.MoveWindow(rcCheck);
	m_wndAni.MoveWindow(rcAni);
	m_btBold.MoveWindow(rcBold);
	m_btItalic.MoveWindow(rcItalic);
	m_btUnderline.MoveWindow(rcUnderline);
	m_btColor.MoveWindow(rcColor);
	m_wndNotics.MoveWindow(rcNotice);

	ShowScrollBar(SB_BOTH , FALSE);
}

void CMessageView::OnTimer(UINT nIDEvent)
{

	if(nIDEvent == TIMER_START){

		KillTimer(TIMER_START);
		GetApp()->GetMainFrame()->m_wndDocSelector.UpdateTitle(this, GetDocument()->GetTitle());
		if(!GetDocument()->m_strMsg.IsEmpty()){

			WriteIncoming(GetDocument()->m_strMsg);
		}
		if(m_pStatusBar){

			m_pStatusBar->SetPaneText(2, GetDocument()->m_strUser);
			m_pStatusBar->SetPaneText(3, "");
			m_pStatusBar->SetPaneText(4, "");
		}
	}
	else if(nIDEvent == TIMER_ID_WAITACK){

		if(m_nWait >= 45){	 // timeout

			m_bMsgPending = FALSE;
			KillTimer(TIMER_ID_WAITACK);
			CString strErr;
			strErr.LoadString(ID_SEND_FAILED);
			WriteIncoming(strErr);
		}
		else m_nWait++;
	}
	else{

		CFormView::OnTimer(nIDEvent);
	}
}


void CMessageView::PMAck(BOOL bOK)
{

	// Set focus to edit line if this view has the focus...
	if(GetApp()->GetMainFrame()->MDIGetActive() == GetParentFrame()){

		m_eInput.SetFocus();
	}
	
	m_bMsgPending = FALSE;
	KillTimer(TIMER_ID_WAITACK);
}

void CMessageView::CheckAway(void)
{

	CString strAway = g_sSettings.GetPMAwayMsg();
	
	if(strAway.IsEmpty()) strAway = GetApp()->GetMainFrame()->m_strPMAway;

	if(!strAway.IsEmpty() && (m_nAway < 3)){

		EXT_MSG_STRUCT msg;
		msg.wParam    = GetDocument()->m_dwIP;
		msg.lParam	  = GetDocument()->m_wPort;
		msg.lpszParam = GetDocument()->m_strUser;
		msg.lpszMsg   = strAway;

		AfxGetMainWnd()->SendMessage(WM_PMSG, 0, (LPARAM)&msg);
		m_nAway++;
	}
}

LRESULT CMessageView::OnWPNNotify(WPARAM w, LPARAM l)
{

	LRESULT lReturn = 0;

	switch(w){
		case WPN_INCOMING_PM:
			{
				PMINFO *pm = (PMINFO*)l;
				if(GetDocument()->m_strUser.Compare(pm->lpszName) == 0){

					GetDocument()->m_dwIP	= pm->dwIP;
					GetDocument()->m_wPort = pm->wPort;
					WriteIncoming(pm->lpszMessage);
					CheckAway();
					m_dwMsgCount++;
					KillTimer(TIMER_ID_WAITACK);
					lReturn = 1;
				}
			}
			break;
		case WPN_PM_ACK:
			{
				
				if(!m_bMsgPending) return 0;

				PMINFO *pm = (PMINFO*)l;
				if(Util::GetBaseName(GetDocument()->m_strUser).Compare(Util::GetBaseName(pm->lpszName)) == 0){
					
					GetDocument()->m_dwIP	= pm->dwIP;
					GetDocument()->m_wPort = pm->wPort;
					if(strstr(pm->lpszMessage, "SENT") != NULL){
    
						m_dwMsgSentCount++;
						PMAck();
					}
					else{

						WriteIncoming(pm->lpszMessage);
						PMAck(FALSE);
					}
					lReturn = 1;
				}
			}
			break;
	}

	return lReturn;
}

void CMessageView::OnActionsReloadfiles()
{

	EXT_MSG_STRUCT   msg;
	msg.wParam = (WPARAM)GetDocument()->m_dwIP;
	msg.lParam = (LPARAM)GetDocument()->m_wPort;
	msg.lpszParam = (LPCSTR)GetDocument()->m_strUser;
	::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_BROWSE, 0, (LPARAM)&msg);
}

void CMessageView::OnActionsSetawaymessage()
{
	CInputRequest dlg;
	dlg.m_strInput = g_sSettings.GetPMAwayMsg();
	if(dlg.m_strInput.IsEmpty()) dlg.m_strInput = GetApp()->GetMainFrame()->m_strPMAway;
	dlg.SetMode(6, &m_fFont);
	if(dlg.DoModal() == ID_SEND){

		g_sSettings.SetPMAwayMsg(dlg.m_strInput);
	}
}

void CMessageView::OnActionsIgnoreuser()
{

	g_sSettings.m_aIgnored.Add(Util::GetBaseName(GetDocument()->m_strUser) + "*");
}

void CMessageView::OnActionsTrustuser()
{

	g_sSettings.m_aTrusted.Add(Util::GetBaseName(GetDocument()->m_strUser) + "*");
}

void CMessageView::OnShiftenter()
{

	OnBnClickedOk();
}

void CMessageView::OnChatroomDisplayinchannelWinampsong()
{

	if(Util::IsVideoPlaying()){

		CString strText = g_sSettings.GetVideoMsg();
		Util::ReplaceVars(strText);
		m_eInput.InsertText(strText);
	}
	else{

		CString strText = g_sSettings.GetWinampMsg();
		Util::ReplaceVars(strText);
		m_eInput.InsertText(strText);
	}
}

void CMessageView::OnUpdateChatroomDisplayinchannelWinampsong(CCmdUI *pCmdUI)
{

	HWND hwndWinamp = ::FindWindow("Winamp v1.x",NULL);
	pCmdUI->Enable(hwndWinamp != NULL);	
}

void CMessageView::OnDisplayinchannelOnlinetime()
{

	CString strOnline;
	strOnline.Format("Online: %s", g_sSystem.GetOnlineString());
	m_eInput.InsertText(strOnline);
}

void CMessageView::OnChatroomDisplayinchannelSystemuptime()
{
	
	m_eInput.InsertText(Util::GetUpTime());
}

void CMessageView::OnChatroomDisplayinchannelSysteminfo()
{
	
	m_eInput.InsertText(Util::GetMySystemInfo());
}

void CMessageView::OnDisplayinchannelExtendedsysteminfo()
{
	
	m_eInput.InsertText(Util::GetMySystemInfoEx());
}

void CMessageView::OnRclickMsgs(RVN_ELEMENTEVENT* pNotify, LRESULT *pResult)
{

	CRichElement* pElement = pNotify->pElement;

	

	if(pElement != NULL){

		if(pElement->m_nType == retName){

			//////////////////////
			// Load the usermenu
			CMenu mContextMenu;
			if((pElement->m_sText.Find(Util::LoadString(IDS_PM_NOTIME_YOUWRITE)) >= 0) ||
				(pElement->m_sText.Find(Util::LoadString(IDS_PM_TIME_YOUWRITE)) >= 0)){

				return;
			}

			m_strSelText = GetDocument()->m_strUser;

			mContextMenu.LoadMenu(IDR_PMMENU);

			// Add username and seperator on top of menu
			m_strSelText.Replace("&", "&&");
			mContextMenu.GetSubMenu(0)->InsertMenu(0, MF_STRING|MF_BYPOSITION|MF_DISABLED, 0, m_strSelText);
			mContextMenu.GetSubMenu(0)->InsertMenu(1, MF_SEPARATOR|MF_BYPOSITION, 0, (LPCSTR)0);
		}
		else if(pElement->m_nType == retLink){

			//////////////////////
			// Load the linkmenu
			CMenu mContextMenu;
			mContextMenu.LoadMenu(IDR_URL);
			m_strSelText = pElement->m_sLink;
		}
	}
}

void CMessageView::OnOpenURL()
{

	LinkWarning warning;
	warning.m_strURL = m_strSelText;
	BOOL bOpen = g_sSettings.GetShowURLWarning() ? (warning.DoModal() == ID_SEND) : TRUE;
	
	if(bOpen){

		ShellExecute(0, "open", m_strSelText, NULL, NULL, SW_SHOW);
	}
}

void CMessageView::OnOpenURLinnewbrowser()
{

	LinkWarning warning;
	warning.m_strURL = m_strSelText;

	BOOL bOpen = g_sSettings.GetShowURLWarning() ? (warning.DoModal() == ID_SEND) : TRUE;
	
	if(bOpen){
	
		ShellExecute(0, "open", "iexplore.exe", "-new " + m_strSelText, NULL, SW_SHOW);
	}
}

void CMessageView::OnCopyAddress()
{
	
	CClipboard::SetText((LPSTR)(LPCTSTR)m_strSelText);
}
void CMessageView::OnBnClickedBold()
{

	m_eInput.InsertText("[b][/b]");
	m_eInput.SetFocus();
	int nStart = 0, nEnd = 0;
	m_eInput.GetSel(nStart, nEnd);
	nStart -=4;
	nEnd = nStart;
	if(nStart > 0){

		m_eInput.SetSel(nStart, nEnd, FALSE);
	}
}

void CMessageView::OnBnClickedUnderline()
{

	m_eInput.InsertText("[u][/u]");
	m_eInput.SetFocus();
	int nStart = 0, nEnd = 0;
	m_eInput.GetSel(nStart, nEnd);
	nStart -=4;
	nEnd = nStart;
	if(nStart > 0){

		m_eInput.SetSel(nStart, nEnd, FALSE);
	}
}

void CMessageView::OnBnClickedItalic()
{
	m_eInput.InsertText("[i][/i]");
	m_eInput.SetFocus();
	int nStart = 0, nEnd = 0;
	m_eInput.GetSel(nStart, nEnd);
	nStart -=4;
	nEnd = nStart;
	if(nStart > 0){

		m_eInput.SetSel(nStart, nEnd, FALSE);
	}
}

void CMessageView::OnBnClickedColor()
{

	CColorDialog dlg(g_sSettings.GetRGBNormalMsg(), 0, this);
	
	if(dlg.DoModal() == ID_SEND){

		CString strColor;
		COLORREF cr= dlg.GetColor();
		strColor.Format("[c:#%02X%02X%02X][/c]", GetRValue(cr), GetGValue(cr), GetBValue(cr));
		m_eInput.InsertText(strColor);
		m_eInput.SetFocus();
		int nStart = 0, nEnd = 0;
		m_eInput.GetSel(nStart, nEnd);
		nStart -=4;
		nEnd = nStart;
		if(nStart > 0){

			m_eInput.SetSel(nStart, nEnd, FALSE);
		}
	}
}

void CMessageView::NotifyDisplayTooltip(NMHDR * pNMHDR, LRESULT * result)
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

	strImage.Format("<icon idres=%d width=16 height=16>", IDR_MESSAGEVIEW_TMPL);

	pNotify->ti->sTooltip = g_sSettings.GetToolPM();

	pNotify->ti->sTooltip.Replace("%PMICON%", strImage);
	pNotify->ti->sTooltip.Replace("%USERIMAGE%", strLabel);
	pNotify->ti->sTooltip.Replace("%NAME%", Util::GetBaseName(GetDocument()->m_strUser));
	pNotify->ti->sTooltip.Replace("%RAWNAME%", GetDocument()->m_strUser);
	pNotify->ti->sTooltip.Replace("%MSGCOUNT%", Util::Formatint(m_dwMsgCount));
	pNotify->ti->sTooltip.Replace("%MSGSENTCOUNT%", Util::Formatint(m_dwMsgSentCount));

	pNotify->ti->nEffect = g_sSettings.GetToolEffect();
	pNotify->ti->nGranularity = g_sSettings.GetToolGran();
	pNotify->ti->nTransparency = g_sSettings.GetToolTrans();
	pNotify->ti->crBegin = g_sSettings.GetToolBg1();
	pNotify->ti->crMid   = g_sSettings.GetToolBg2();
	pNotify->ti->crEnd   = g_sSettings.GetToolBg3();
}


