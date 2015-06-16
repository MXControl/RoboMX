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
#include "ExitConfirmation.h"
#include "settings.h"

// ExitConfirmation dialog
extern CSettings g_sSettings;

IMPLEMENT_DYNAMIC(ExitConfirmation, CDialog)
ExitConfirmation::ExitConfirmation(CWnd* pParent /*=NULL*/)
	: CDialog(ExitConfirmation::IDD, pParent)
{
}

ExitConfirmation::~ExitConfirmation()
{
}

void ExitConfirmation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(ExitConfirmation, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// ExitConfirmation message handlers

BOOL ExitConfirmation::OnInitDialog()
{
	
	CDialog::OnInitDialog();

	GetDlgItem(IDCANCEL)->SetFocus();
	((CButton*)GetDlgItem(IDC_ALWAYS_WARN))->SetCheck(TRUE);
	return TRUE;
}

void ExitConfirmation::OnBnClickedOk()
{

	g_sSettings.SetShowExitConfirm(((CButton*)GetDlgItem(IDC_ALWAYS_WARN))->GetCheck());
	OnOK();
}
