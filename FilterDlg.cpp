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
#include "FilterDlg.h"
#include "ini.h"
#include "settings.h"


extern CSettings g_sSettings;

// FilterDlg dialog

IMPLEMENT_DYNAMIC(FilterDlg, CDialog)
FilterDlg::FilterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(FilterDlg::IDD, pParent)
{
}

FilterDlg::~FilterDlg()
{
}

void FilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IGNORED, m_lbIgnored);
	DDX_Text(pDX, IDC_ADD_BOX, m_strAdd);
}


BEGIN_MESSAGE_MAP(FilterDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
END_MESSAGE_MAP()


// FilterDlg message handlers
BOOL FilterDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());
	int nFilters = ini.GetValue("ChannelFilter", "Num", 0);
	
	CString strFilter;

	for(int i = 0; i < nFilters; i++){

		strFilter.Format("Filter_%d", i);
		strFilter = ini.GetValue("ChannelFilter", strFilter, "");
		if(strFilter.IsEmpty()) continue;
		
		m_lbIgnored.AddString(strFilter);
	}
	return TRUE;
}

void FilterDlg::OnBnClickedOk()
{

	CString strTmp;

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());
	
	ini.SetValue("ChannelFilter", "Num", m_lbIgnored.GetCount());
	
	CString strFilter;
	for(int i = 0; i < m_lbIgnored.GetCount(); i++){

		m_lbIgnored.GetText(i, strTmp);
		strFilter.Format("Filter_%d", i);
		ini.SetValue("ChannelFilter", strFilter, strTmp);
	}

	OnOK();
}

void FilterDlg::OnBnClickedAdd()
{

	UpdateData(TRUE);
	
	if(m_strAdd.IsEmpty()) return;

	CString strTmp;

	for(int i = 0; i < m_lbIgnored.GetCount(); i++){

		m_lbIgnored.GetText(i, strTmp);
		if(strTmp == m_strAdd) break;
	}
	if(i >= m_lbIgnored.GetCount()){

		m_lbIgnored.AddString(m_strAdd);
	}
	
	m_strAdd.Empty();
	UpdateData(FALSE);
}

void FilterDlg::OnBnClickedRemove()
{

	int iCnt = 0, *pIndex = 0;

	iCnt = m_lbIgnored.GetSelCount();
	
	if(iCnt == 0) return;
	
	pIndex = new int[iCnt];
	m_lbIgnored.GetSelItems(iCnt, pIndex);
	
	CStringArray aTmp;
	CString	strTmp;

	for(int i = 0; i < iCnt; i++){

		m_lbIgnored.GetText(pIndex[i], strTmp);
		aTmp.Add(strTmp);
	}

	for(i = 0; i < iCnt; i++){

		m_lbIgnored.DeleteString(m_lbIgnored.FindStringExact(-1, aTmp[i]));
	}
	
	aTmp.RemoveAll();
	delete pIndex;
	pIndex = NULL;
}
