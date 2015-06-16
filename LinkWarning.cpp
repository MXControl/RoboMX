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
#include "LinkWarning.h"
#include "settings.h"
#include ".\linkwarning.h"

extern CSettings g_sSettings;

// LinkWarning dialog

IMPLEMENT_DYNAMIC(LinkWarning, CDialog)
LinkWarning::LinkWarning(CWnd* pParent /*=NULL*/)
	: CDialog(LinkWarning::IDD, pParent)
{
}

LinkWarning::~LinkWarning()
{
}

void LinkWarning::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_URL, m_strURL);
	DDX_Check(pDX, IDC_ALWAYS_SHOW_WARNING, m_bWarning);
}


BEGIN_MESSAGE_MAP(LinkWarning, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_EN_CHANGE(IDC_URL, OnEnChangeUrl)
END_MESSAGE_MAP()

BOOL LinkWarning::OnInitDialog()
{

	CDialog::OnInitDialog();

	m_bWarning = g_sSettings.GetShowURLWarning();

	UpdateData(FALSE);
	
	GetDlgItem(IDCANCEL)->SetFocus();
	return TRUE;
}
// LinkWarning message handlers

void LinkWarning::OnBnClickedOk()
{

	UpdateData(TRUE);
	g_sSettings.SetShowURLWarning(m_bWarning);
	OnOK();
}

void LinkWarning::OnBnClickedCancel()
{

	UpdateData(TRUE);
	g_sSettings.SetShowURLWarning(m_bWarning);
	OnCancel();
}

void LinkWarning::OnEnChangeUrl()
{

	UpdateData(FALSE);
}
