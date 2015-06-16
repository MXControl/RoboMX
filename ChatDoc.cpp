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

#include "ChatDoc.h"
#include "ConnectionDlg.h"
#include "MainFrm.h"
#include "Settings.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CSettings g_sSettings;

/////////////////////////////////////////////////////////////////////////////
// CChatDoc

IMPLEMENT_DYNCREATE(CChatDoc, CMyDocument)

BEGIN_MESSAGE_MAP(CChatDoc, CMyDocument)
	//{{AFX_MSG_MAP(CChatDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatDoc construction/destruction

CChatDoc::CChatDoc()
{

	m_dwFiles    = 0;
	m_wLine      = 0x00;
	m_wUserMode  = 0;
	m_bOldJoin   = FALSE;
	m_bWPN		 = TRUE;
	m_strMyDocString = "CRoboMXDoc";
}

CChatDoc::~CChatDoc()
{
}

BOOL CChatDoc::OnNewDocument()
{

	if (!CMyDocument::OnNewDocument()){

		CString strTmp;
		strTmp.Format("Fatal internal error: The framework failed to create an empty Document (0x%X)", GetLastError());
		AfxMessageBox(strTmp, MB_ICONSTOP);
		return FALSE;
	}
	if(GetApp()->GetMainFrame()->m_bQuickJoin){

		m_dwFiles = g_sSettings.GetFiles();
		m_wLine   = g_sSettings.GetLine();
		m_strRoom = GetApp()->GetMainFrame()->m_strRoom;
		m_strLogin = GetApp()->GetMainFrame()->m_strQuickLogin;
		m_strName = g_sSettings.GetNickname();
		m_bWPN	  = TRUE;

		Util::MakeValidUserName(m_strName, TRUE, ((CMainFrame*)GetApp()->m_pMainWnd)->m_wClientID);
	}
	else{

		CConnectionDlg dlg;

		if(dlg.DoModal() == IDCANCEL){

			return FALSE;
		}
		m_dwFiles  = dlg.m_dwFiles;
		m_wLine    = (WORD)dlg.m_nLine;
		m_strRoom  = dlg.m_strRoom;
		if(m_strRoom.IsEmpty()){

			AfxMessageBox("Internal Error: CChatDoc::m_strRoom is empty. Aborting :-(", MB_ICONEXCLAMATION);
			return FALSE;
		}
		m_strName  = dlg.m_strName;
		m_strLogin = dlg.m_strLogin;
		m_bOldJoin = dlg.m_bOldJoin;
		m_bWPN	   = dlg.m_bWPN;
	}

	GetApp()->GetMainFrame()->m_bQuickJoin = FALSE;
	GetApp()->GetMainFrame()->m_strRoom.Empty();
	GetApp()->GetMainFrame()->m_strQuickLogin.Empty();
	int n = m_strRoom.ReverseFind('_');
	if(n > 0){

		m_strRoomShort = m_strRoom.Left(n);
	}
	
	CMyDocument::SetTitle(m_strRoom + " (" + m_strName + ")");

	if(m_bWPN){

		GetApp()->GetMainFrame()->m_mxClient.SendRename(m_strName, m_wLine);
	}

	return TRUE;
}


void CChatDoc::SetTitle(LPCTSTR lpszTitle, BOOL bShort)
{

	if(bShort){ // the roomname doesnt have the numbers at the end

		CString strAppendix;
		
		int n = m_strRoom.ReverseFind('_');

		m_strRoomShort = lpszTitle;
		
		if(n > 0){

			strAppendix = m_strRoom.Mid(n);
		}

		m_strRoom = m_strRoomShort + strAppendix;
	}
	else{ // the roomname has numbers at the end


		m_strRoom = lpszTitle;
		int n = m_strRoom.ReverseFind('_');
		if(n > 0){

			m_strRoomShort = m_strRoom.Left(n);
		}
	}
	CMyDocument::SetTitle(m_strRoom + " (" + m_strName + ")");
}
/////////////////////////////////////////////////////////////////////////////
// CChatDoc serialization

void CChatDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CChatDoc diagnostics

#ifdef _DEBUG
void CChatDoc::AssertValid() const
{
	CMyDocument::AssertValid();
}

void CChatDoc::Dump(CDumpContext& dc) const
{
	CMyDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChatDoc commands
