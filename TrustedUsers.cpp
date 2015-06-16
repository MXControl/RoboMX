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
#include "TrustedUsers.h"
#include ".\trustedusers.h"

#include "Settings.h"

// TrustedUsers dialog

extern CSettings g_sSettings;


IMPLEMENT_DYNAMIC(TrustedUsers, CDialog)
TrustedUsers::TrustedUsers(CWnd* pParent /*=NULL*/)
	: CDialog(TrustedUsers::IDD, pParent)
{

	m_bPMTrusted = FALSE;
}

TrustedUsers::~TrustedUsers()
{
}

void TrustedUsers::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRUSTED, m_lbTrusted);
	DDX_Text(pDX, IDC_ADD_BOX, m_strAdd);
	DDX_Check(pDX, IDC_ACCEPT_TRUSTED_ONLY, m_bPMTrusted);
}


BEGIN_MESSAGE_MAP(TrustedUsers, CDialog)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// TrustedUsers message handlers
BOOL TrustedUsers::OnInitDialog()
{

	CDialog::OnInitDialog();

	for(int i = 0; i < g_sSettings.m_aTrusted.GetSize(); i++){

		m_lbTrusted.AddString(g_sSettings.m_aTrusted[i]);
	}
	
	m_bPMTrusted = g_sSettings.GetPMOnlyTrusted();
	UpdateData(FALSE);

	return TRUE;
}

void TrustedUsers::OnBnClickedAdd()
{

	UpdateData(TRUE);
	
	if(m_strAdd.IsEmpty()) return;

	CString strTmp;

	for(int i = 0; i < m_lbTrusted.GetCount(); i++){

		m_lbTrusted.GetText(i, strTmp);
		if(strTmp == m_strAdd) break;
	}
	if(i >= m_lbTrusted.GetCount()){

		m_lbTrusted.AddString(m_strAdd);
	}
	
	m_strAdd.Empty();
	UpdateData(FALSE);
}

void TrustedUsers::OnBnClickedRemove()
{

	int iCnt = 0, *pIndex = 0;

	iCnt = m_lbTrusted.GetSelCount();
	
	if(iCnt == 0) return;
	
	pIndex = new int[iCnt];
	m_lbTrusted.GetSelItems(iCnt, pIndex);
	
	CStringArray aTmp;
	CString	strTmp;

	for(int i = 0; i < iCnt; i++){

		m_lbTrusted.GetText(pIndex[i], strTmp);
		aTmp.Add(strTmp);
	}

	for(i = 0; i < iCnt; i++){

		m_lbTrusted.DeleteString(m_lbTrusted.FindStringExact(-1, aTmp[i]));
	}
	
	aTmp.RemoveAll();
	delete pIndex;
	pIndex = NULL;
}

void TrustedUsers::OnBnClickedOk()
{

	UpdateData(TRUE);
	CString strTmp;

	g_sSettings.m_aTrusted.RemoveAll();
	
	for(int i = 0; i < m_lbTrusted.GetCount(); i++){

		m_lbTrusted.GetText(i, strTmp);
		g_sSettings.m_aTrusted.Add(strTmp);
	}

	g_sSettings.SetPMOnlyTrusted(m_bPMTrusted);
	OnOK();
}

