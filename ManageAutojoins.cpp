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
#include "ManageAutojoins.h"
#include ".\manageautojoins.h"
#include "Ini.h"
#include "settings.h"

extern CSettings g_sSettings;

// ManageAutojoins dialog

IMPLEMENT_DYNAMIC(ManageAutojoins, CDialog)
ManageAutojoins::ManageAutojoins(CWnd* pParent /*=NULL*/)
	: CDialog(ManageAutojoins::IDD, pParent)
{
}

ManageAutojoins::~ManageAutojoins()
{
}

void ManageAutojoins::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AUTOJOINS, m_lcRooms);
	DDX_Text(pDX, IDC_ADD_BOX, m_strAdd);
	DDX_Text(pDX, IDC_LOGIN, m_strAddLogin);
}


BEGIN_MESSAGE_MAP(ManageAutojoins, CDialog)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// ManageAutojoins message handlers

BOOL ManageAutojoins::OnInitDialog()
{

	CDialog::OnInitDialog();

	m_lcRooms.InsertColumn(0, "Channel", LVCFMT_LEFT, 200, 0);
	m_lcRooms.InsertColumn(1, "Login", LVCFMT_LEFT, 200, 0);
	m_lcRooms.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_UNDERLINEHOT);
	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());
	
	int n = ini.GetValue("AutoJoins", "Num", 0);
	CString strTmp;

	for(int i = 1; i < n+1; i++){

		strTmp.Format("AutoJoin_%03d", i);
		int n = m_lcRooms.InsertItem(0, ini.GetValue("AutoJoins", strTmp, ""), 0);
		strTmp.Format("AutoJoinLogin_%03d", i);
		m_lcRooms.SetItemText(n, 1, ini.GetValue("AutoJoins", strTmp, ""));
	}
	return TRUE;
}

void ManageAutojoins::OnBnClickedAdd()
{

	UpdateData(TRUE);
	
	if(m_strAdd.IsEmpty()) return;

	CString strTmp;

	for(int i = 0; i < m_lcRooms.GetItemCount(); i++){

		 strTmp = m_lcRooms.GetItemText(i, 0);
		if(strTmp == m_strAdd) break;
	}
	if(i >= m_lcRooms.GetItemCount()){

		int n = m_lcRooms.InsertItem(0, m_strAdd, 0);
		m_lcRooms.SetItemText(n, 1, m_strAddLogin);
	}
	
	m_strAdd.Empty();
	m_strAddLogin.Empty();
	UpdateData(FALSE);
}

void ManageAutojoins::OnBnClickedRemove()
{

	int iCnt = 0, *pIndex = 0;

	iCnt = m_lcRooms.GetSelectedCount();
	
	if(iCnt == 0) return;
	
	pIndex = new int[iCnt];
	
	int iCounter = 0;
	POSITION pos = m_lcRooms.GetFirstSelectedItemPosition();
	while(pos){

		pIndex[iCounter++] = m_lcRooms.GetNextSelectedItem(pos);
	}
	
	CStringArray aTmp;
	CString	strTmp;

	for(int i = 0; i < iCnt; i++){

		//m_lbRooms.GetText(pIndex[i], strTmp);
		strTmp = m_lcRooms.GetItemText(pIndex[i], 0);
		aTmp.Add(strTmp);
	}

	for(i = 0; i < iCnt; i++){

		//m_lbRooms.DeleteString(m_lbRooms.FindStringExact(-1, aTmp[i]));
		LVFINDINFO find;
		find.flags = LVFI_STRING;
		find.psz   = aTmp[i];
		m_lcRooms.DeleteItem(m_lcRooms.FindItem(&find, -1));
	}
	
	aTmp.RemoveAll();
	delete pIndex;
	pIndex = NULL;
}

void ManageAutojoins::OnBnClickedOk()
{

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());
	
	int nCnt = m_lcRooms.GetItemCount();
	
	ini.SetValue("AutoJoins", "Num", nCnt);

	CString strTmp, strRoom;

	for(int i = 0; i < nCnt; i++){

		strTmp.Format("AutoJoin_%03d", i+1);
		strRoom = m_lcRooms.GetItemText(i, 0);
		ini.SetValue("AutoJoins", strTmp, strRoom);
		
		strTmp.Format("AutoJoinLogin_%03d", i);
		strRoom = m_lcRooms.GetItemText(i, 1);
		ini.SetValue("AutoJoins", strTmp, strRoom);
	}

	OnOK();
}
