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
#include "MyEdit.h"
#include "Settings.h"
#include ".\myedit.h"
#include "util.h"
#include "EmoticonManager.h"
#include "Clipboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CSettings g_sSettings;
extern CEmoticonManager    g_eEmoticons;

CStringArray g_aHistory;

#define ID_COPY       32000
#define ID_CUT        32001
#define ID_PASTE      32002
#define ID_UNDO       32003
#define ID_CLEAR      32004
#define ID_SELECTALL  32005
#define ID_SOUNDBASE  32100
#define ID_EMOBASE    33100
#define ID_CMD_START  ID_COPY
#define ID_CMD_END    ID_EMOBASE+1000
/////////////////////////////////////////////////////////////////////////////
// CMyEdit

UINT UWM_TEXT_INPUT = ::RegisterWindowMessage("UWM_TEXT_INPUT-7A14F66B-ABAB-4525-AC01-841C82EC48B6");

#define m_aCommandsNUM (DWORD)m_pCommands->GetSize()
#define g_aQuickNUM (DWORD)g_sSettings.m_aQuick.GetSize() 
#define g_aHistoryNUM (DWORD)g_aHistory.GetSize()

CMyEdit::CMyEdit()
{

	m_crBg = ::GetSysColor(COLOR_WINDOW); // Initializing the Background Color to the system face color.
	m_crBgFocus = g_sSettings.GetRGBFocus();
	m_brBkgnd.CreateSolidBrush(m_crBg); // Create the Brush Color for the Background.
	m_crDraw	= m_crBg;
	m_bEx		= FALSE;
	m_pCommands = NULL;
	m_bMenuEx   = FALSE;
}

CMyEdit::~CMyEdit()
{
}


BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
	//{{AFX_MSG_MAP(CMyEdit)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND_RANGE(ID_CMD_START, ID_CMD_END, OnRclickCmds)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyEdit message handlers

/////////////////////////////////
// Search string in autocommand
/////////////////////////////////
int CMyEdit::SearchItem(CString strString)
{

	CString strTmp;

	for(int nIndex = 0; nIndex < g_sSettings.m_aQuick.GetSize(); nIndex++){

		strTmp = g_sSettings.m_aQuick[nIndex];
		if(strTmp == strString) break;
	}

	if((nIndex >= g_sSettings.m_aQuick.GetSize())  || g_sSettings.m_aQuick.GetSize() == 0) return 0;

	return nIndex + 1;
}

int CMyEdit::SearchHistory(CString strString)
{

	CString strTmp;

	for(int nIndex = 0; nIndex < g_aHistory.GetSize(); nIndex++){

		strTmp = g_aHistory[nIndex];
		if(strTmp == strString) break;
	}

	if((nIndex >= g_aHistory.GetSize())  || g_aHistory.GetSize() == 0) return 0;

	return nIndex + 1;
}

int CMyEdit::SearchRCMSItem(CString strString)
{

	CString strTmp;

	for(int nIndex = 0; nIndex < m_pCommands->GetSize(); nIndex++){

		strTmp = m_pCommands->GetAt(nIndex);
		if(strTmp == strString) break;
	}

	if((nIndex >= m_pCommands->GetSize())  || m_pCommands->GetSize() == 0) return 0;

	return nIndex + 1;
}

BOOL CMyEdit::PreTranslateMessage(MSG* pMsg) 
{

	if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)
			&& (GetKeyState(VK_SHIFT) >= 0)){

		::SendMessage(GetParent()->m_hWnd, UWM_TEXT_INPUT, 0, 0);
	}
	else if((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN)
				&& (GetKeyState(VK_SHIFT) < 0) && (GetStyle() & ES_MULTILINE)){

		CString strText;
		GetWindowText(strText);
		strText+="\r\n";
		SetWindowText(strText);
		SetSel(0, -1, FALSE);
		SetSel(-1, 0, FALSE);
		return TRUE;
	}
	else if((pMsg->message == WM_MOUSEWHEEL || pMsg->message == WM_KEYDOWN) && !m_bEx && g_sSettings.GetEnableScroller()){

		ASSERT(m_pCommands);
		///////////////////////////////////////////////////////
		// Scroll through RCMS commands UP (PAGE UP)
		///////////////////////////////////////////////////////
		if((pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_PRIOR) || 
			(pMsg->message == WM_MOUSEWHEEL && ((short)HIWORD(pMsg->wParam)) > 0)){
		
			if(!m_pCommands) return TRUE;
			if(m_pCommands->GetSize() == 0) return TRUE;
			SetSel(0,-1);
			TCHAR szTempStr[1024];
			if(SendMessage(WM_GETTEXT, 1024, (LPARAM)szTempStr)){
		
				//DWORD dwIndex = atol(szTempStr);
				DWORD dwIndex = SearchRCMSItem(szTempStr);
				if(dwIndex == 0){

					lstrcpy(szTempStr, m_pCommands->GetAt(m_aCommandsNUM - 1));
					ReplaceSel(szTempStr, TRUE);
					SendMessage(WM_KEYDOWN, VK_END, 0);
					return TRUE;
				}
				else if((dwIndex == 1) || (dwIndex > m_aCommandsNUM)){

					ReplaceSel("", TRUE);
				}
				else{

					lstrcpy(szTempStr, m_pCommands->GetAt(dwIndex - 2));
					ReplaceSel(szTempStr, TRUE);
					SendMessage(WM_KEYDOWN, VK_END, 0);
				}
				return TRUE;
			}
			else{

				lstrcpy(szTempStr, m_pCommands->GetAt(m_aCommandsNUM - 1));
				ReplaceSel(szTempStr, TRUE);
				SendMessage(WM_KEYDOWN, VK_END, 0);
				return TRUE;
			}
		}
		///////////////////////////////////////////////////////
		// RCMS Down (PAGE DOWN)
		///////////////////////////////////////////////////////
		else if(((pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_NEXT) || 
			(pMsg->message == WM_MOUSEWHEEL && ((short)HIWORD(pMsg->wParam)) <= 0)) && !m_bEx){
		
			if(!m_pCommands) return TRUE;
			if(m_pCommands->GetSize() == 0) return TRUE;
			SetSel(0,-1);
			TCHAR szTempStr[1024];
			if(SendMessage(WM_GETTEXT, 1024, (LPARAM)szTempStr)){

				//DWORD dwIndex = atol(szTempStr);
				DWORD dwIndex = SearchRCMSItem(szTempStr);
				if(dwIndex >= m_aCommandsNUM){

					ReplaceSel("", TRUE);
					//Beep(1000, 50);
				}
				else{

					lstrcpy(szTempStr, m_pCommands->GetAt(dwIndex));
					ReplaceSel(szTempStr, TRUE);
					SendMessage(WM_KEYDOWN, VK_END, 0);
				}
				return TRUE;
			}
			else{

				lstrcpy(szTempStr, m_pCommands->GetAt(0));
				ReplaceSel(szTempStr, TRUE);
				SendMessage(WM_KEYDOWN, VK_END, 0);
				return TRUE;
			}
		}
		///////////////////////////////////////////////////////
		// Commands CTRL+UP UPARROW or CTRL+MOUSEHWEL UP
		///////////////////////////////////////////////////////
		else if(((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_UP)) && (GetKeyState(VK_CONTROL) < 0) && (g_aHistoryNUM != 0) && !m_bEx){
		
			TRACE("History UP %d\n",GetKeyState(VK_LCONTROL));
			SetSel(0,-1);
			TCHAR szTempStr[1024];
			if(SendMessage(WM_GETTEXT, 1024, (LPARAM)szTempStr)){
		
				DWORD dwIndex = SearchHistory(szTempStr);
				if(dwIndex == 0){ // Item was not found

					CString strOut = g_aHistory[g_aHistory.GetSize() - 1];
					//Util::ReplaceVars(strOut);
					lstrcpy(szTempStr, strOut);
					ReplaceSel(szTempStr, TRUE);
					SendMessage(WM_KEYDOWN, VK_END, 0);
					return TRUE;
				}
				else if(dwIndex == 1){ // Item found but we are at no1

					SetWindowText("");
					//Beep(1000, 50);
				}
				else{ // item found 

					CString strOut = g_aHistory[dwIndex - 2];
					lstrcpy(szTempStr, strOut);
					ReplaceSel(szTempStr, TRUE);
					SendMessage(WM_KEYDOWN, VK_END, 0);
				}
				return TRUE;
			}
			else{

				CString strOut = g_aHistory[g_aHistory.GetSize() - 1];
				lstrcpy(szTempStr, strOut);
				ReplaceSel(szTempStr, TRUE);
				SendMessage(WM_KEYDOWN, VK_END, 0);
				return TRUE;
			}
		}
		///////////////////////////////////////////////////////
		// HISTORY down CTRL+DOWNARROW or CTRL+MOUSEWHEEL DOWN
		///////////////////////////////////////////////////////
		else if((pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DOWN) && (GetKeyState(VK_CONTROL) < 0) && (g_aHistoryNUM != 0) && !m_bEx){
		
			TRACE("HistoryUP %d\n",GetKeyState(VK_LCONTROL));
			SetSel(0,-1);
			TCHAR szTempStr[1024];
			if(SendMessage(WM_GETTEXT, 1024, (LPARAM)szTempStr)){
				DWORD dwIndex = SearchHistory(szTempStr);
				if(dwIndex >= g_aHistoryNUM){

					SetWindowText("");
					//Beep(1000, 50);
				}
				else{

					CString strOut = g_aHistory[dwIndex];

					lstrcpy(szTempStr, strOut);

					SetWindowText(szTempStr);
					SendMessage(WM_KEYDOWN, VK_END, 0);
				}
				return TRUE;
			}
			else{

				CString strOut = g_aHistory[0];

				lstrcpy(szTempStr, strOut);
				SetWindowText(szTempStr);
				SendMessage(WM_KEYDOWN, VK_END, 0);
				return TRUE;
			}
		}

		///////////////////////////////////////////////////////
		// Commands UP UPARROW or MOUSEHWEL UP
		///////////////////////////////////////////////////////
		else if((pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_UP) && (g_aQuickNUM != 0) && !m_bEx){
		
			TRACE("Cmd UP %d\n",GetKeyState(VK_LCONTROL));
			SetSel(0,-1);
			TCHAR szTempStr[1024];
			if(SendMessage(WM_GETTEXT, 1024, (LPARAM)szTempStr)){
		
				DWORD dwIndex = SearchItem(szTempStr);
				if(dwIndex == 0){ // Item was not found

					CString strOut = g_sSettings.m_aQuick[g_sSettings.m_aQuick.GetSize() - 1];
					//Util::ReplaceVars(strOut);
					lstrcpy(szTempStr, strOut);
					ReplaceSel(szTempStr, TRUE);
					SendMessage(WM_KEYDOWN, VK_END, 0);
					return TRUE;
				}
				else if(dwIndex == 1){ // Item found but we are at no1

					SetWindowText("");
					//Beep(1000, 50);
				}
				else{ // item found 

					CString strOut = g_sSettings.m_aQuick[dwIndex - 2];
					//Util::ReplaceVars(strOut);
					lstrcpy(szTempStr, strOut);
					ReplaceSel(szTempStr, TRUE);
					SendMessage(WM_KEYDOWN, VK_END, 0);
				}
				return TRUE;
			}
			else{

				CString strOut = g_sSettings.m_aQuick[g_sSettings.m_aQuick.GetSize() - 1];
				//Util::ReplaceVars(strOut);
				lstrcpy(szTempStr, strOut);
				ReplaceSel(szTempStr, TRUE);
				SendMessage(WM_KEYDOWN, VK_END, 0);
				return TRUE;
			}
		}
		///////////////////////////////////////////////////////
		// Commands down DOWNARROW or MOUSEWHEEL DOWN
		///////////////////////////////////////////////////////
		else if((pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DOWN) && (g_aQuickNUM != 0) && !m_bEx){
		
			TRACE("Cmd DOWN %d\n",GetKeyState(VK_LCONTROL));
			SetSel(0,-1);
			TCHAR szTempStr[1024];
			if(SendMessage(WM_GETTEXT, 1024, (LPARAM)szTempStr)){
				DWORD dwIndex = SearchItem(szTempStr);
				if(dwIndex >= g_aQuickNUM){

					SetWindowText("");
					//Beep(1000, 50);
				}
				else{

					CString strOut = g_sSettings.m_aQuick[dwIndex];
					//Util::ReplaceVars(strOut);

					lstrcpy(szTempStr, strOut);

					SetWindowText(szTempStr);
					SendMessage(WM_KEYDOWN, VK_END, 0);
				}
				return TRUE;
			}
			else{

				CString strOut = g_sSettings.m_aQuick[0];
				//Util::ReplaceVars(strOut);

				lstrcpy(szTempStr, strOut);
				SetWindowText(szTempStr);
				SendMessage(WM_KEYDOWN, VK_END, 0);
				return TRUE;
			}
		}

	}

	return CEdit::PreTranslateMessage(pMsg);
}

HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{

	
	HBRUSH hbr;
	hbr = (HBRUSH)m_brBkgnd;
	pDC->SetBkColor(m_crDraw);
	pDC->SetTextColor(g_sSettings.GetRGBNormalMsg());
	return hbr;
	
}

void CMyEdit::OnKillFocus(CWnd* pNewWnd) 
{

	CEdit::OnKillFocus(pNewWnd);

	m_crDraw = m_crBg;
	m_brBkgnd.DeleteObject(); 
	m_brBkgnd.CreateSolidBrush(m_crDraw);
	RedrawWindow();
}

void CMyEdit::OnSetFocus(CWnd* pOldWnd) 
{

	CEdit::OnSetFocus(pOldWnd);

	m_crDraw = (g_sSettings.GetFocus() && !m_bEx ?  m_crBgFocus : m_crBg);
	m_brBkgnd.DeleteObject();
	m_brBkgnd.CreateSolidBrush(m_crDraw);
	RedrawWindow();
}

void CMyEdit::SetBkColor(COLORREF cr)
{

	m_crBg = cr;
	m_crBgFocus = g_sSettings.GetRGBFocus();
	m_crDraw = cr;
	RedrawWindow();
}

void CMyEdit::SetExtended()
{

	m_bEx = TRUE;
}

void CMyEdit::SetExtMenu()
{

	m_bMenuEx = TRUE;
}

void CMyEdit::SetCommands(CStringArray* pCmd)
{

	ASSERT(pCmd);
	
	m_pCommands = pCmd;
}

void CMyEdit::InsertText(const CString strText)
{

	ReplaceSel(strText, TRUE);
}

void CMyEdit::OnRButtonDown(UINT nFlags, CPoint point)
{

	if(m_bMenuEx){

		CMenu popup, sound, emoticons;
		if(popup.CreatePopupMenu() && sound.CreatePopupMenu() && emoticons.CreatePopupMenu()){

			POINT p = point;
			//p.x = LOWORD(pMsg->lParam); 
			//p.y= HIWORD(pMsg->lParam);
			ClientToScreen(&p);

			for(int i = 0; i < g_sSettings.m_aSounds.GetSize(); i++){

				sound.AppendMenu(MF_STRING|MF_BYPOSITION, ID_SOUNDBASE+i, g_sSettings.m_aSounds[i].strTrigger);
			}

			CMap<CString, LPCTSTR, WORD, WORD&> aPathCheck;
			WORD  wCheck = 0;
			UINT  uAdded = 0;
			for(i = 0; i < g_eEmoticons.m_aEmoticons.GetSize() && i < 100; i++){

				// avoid adding same emoticon file twice
				if(aPathCheck.Lookup(g_eEmoticons.m_aEmoticons[i]->szFileName, wCheck)) continue;
				else aPathCheck.SetAt(g_eEmoticons.m_aEmoticons[i]->szFileName, wCheck);

				::AppendMenu(emoticons.m_hMenu, 
					(uAdded % 10 == 0) && uAdded > 0 ? MF_MENUBARBREAK|MF_BITMAP : MF_BITMAP, 
					ID_EMOBASE+i, (LPCTSTR)g_eEmoticons.m_aEmoticons[i]->hMenuBitmap);
				uAdded++;
			}
			aPathCheck.RemoveAll();

			BOOL bSel = FALSE;
			
			UINT uFlags = GetModify() ? MF_STRING : MF_STRING|MF_DISABLED|MF_GRAYED;
			
			popup.AppendMenu(uFlags, ID_UNDO, "Undo");
			popup.InsertMenu(1, MF_SEPARATOR|MF_BYPOSITION, 0, (LPCTSTR)0);

			DWORD dwSel = GetSel();
			bSel = HIWORD(dwSel) != LOWORD(dwSel);
			uFlags = bSel ? MF_STRING|MF_BYPOSITION : MF_STRING|MF_BYPOSITION|MF_DISABLED|MF_GRAYED;
			
			dwSel = 0;
			CClipboard::GetTextLength(&dwSel, 0);
			

			popup.AppendMenu(uFlags, ID_CUT, "Cut");
			popup.AppendMenu(uFlags, ID_COPY, "Copy");
			popup.AppendMenu(dwSel ? MF_STRING : MF_STRING|MF_DISABLED|MF_GRAYED, ID_PASTE, "Paste");
			popup.AppendMenu(uFlags, ID_CLEAR, "Delete");
			popup.AppendMenu(MF_SEPARATOR|MF_BYPOSITION, 0, (LPCTSTR)0);
			popup.AppendMenu(MF_STRING|MF_BYPOSITION, ID_SELECTALL, "Select All");
			
			if(g_sSettings.m_aSounds.GetSize() || g_eEmoticons.m_aEmoticons.GetSize()){

				popup.AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
			}
			if(g_sSettings.m_aSounds.GetSize()){

				popup.AppendMenu(MF_POPUP, (UINT_PTR)sound.m_hMenu, "Sounds");
			}
			if(g_eEmoticons.m_aEmoticons.GetSize()){

				popup.AppendMenu(MF_POPUP, (UINT_PTR)emoticons.m_hMenu, "Emoticons");
			}

			popup.TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, p.x, p.y, this, 0);
		}

		
	}
	else{

		CEdit::OnRButtonDown(nFlags, point);
	}
}

void CMyEdit::OnRclickCmds(UINT nID)
{

	if(m_bMenuEx){
        
		if(nID == ID_SELECTALL){

			SetSel(0, -1);
		}
		else if(nID == ID_COPY){

			Copy();
		}  
		else if(nID == ID_CUT){

			Cut();
		}
		else if(nID == ID_PASTE){

			Paste();
		}
		else if(nID == ID_UNDO){

			Undo();
		}
		else if(nID == ID_CLEAR){

			Clear();
		}
		else if((nID >= ID_SOUNDBASE) && (nID <= ID_SOUNDBASE+(UINT)g_sSettings.m_aSounds.GetSize())){

			int nIndex = nID - ID_SOUNDBASE;
			if((nIndex >= 0) && nIndex < g_sSettings.m_aSounds.GetSize()){

				InsertText(g_sSettings.m_aSounds[nIndex].strTrigger);
			}
		}
		else if((nID >= ID_EMOBASE) && (nID <= ID_EMOBASE+(UINT)g_eEmoticons.m_aEmoticons.GetSize())){

			int nIndex = nID - ID_EMOBASE;
			if((nIndex >= 0) && nIndex < g_eEmoticons.m_aEmoticons.GetSize()){

				InsertText(g_eEmoticons.m_aEmoticons[nIndex]->szActivationText);
			}
		}
	}
}