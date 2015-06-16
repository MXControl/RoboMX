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
#include "WPNSetup.h"
#include ".\wpnsetup.h"
#include "settings.h"
#include "CfgAddPeerCacheDlg.h"

extern CSettings g_sSettings;

// CCfgWPN dialog

IMPLEMENT_DYNAMIC(CCfgWPN, CCfgDlg)
CCfgWPN::CCfgWPN(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CCfgWPN::IDD, pParent)
	, m_strPrivacy(_T(""))
	, m_strIgnored(_T(""))
{

}

CCfgWPN::~CCfgWPN()
{
}

void CCfgWPN::DoDataExchange(CDataExchange* pDX)
{
	CCfgDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GET_NODE, m_btAutoNode);
	DDX_Control(pDX, IDC_OWN_NODE, m_btForcedNode);
	DDX_Control(pDX, IDC_NODENAME, m_eNodeName);
	DDX_Text(pDX, IDC_NODENAME, m_strNodeName);
	DDX_Control(pDX, IDC_NODENAME2, m_eExternal);
	DDX_Text(pDX, IDC_NODENAME2, m_strExternal);
	DDX_Text(pDX, IDC_PRIVACY, m_strPrivacy);
	DDX_Text(pDX, IDC_IGNORED, m_strIgnored);
}


BEGIN_MESSAGE_MAP(CCfgWPN, CCfgDlg)
	ON_BN_CLICKED(IDC_GET_NODE, OnBnClickedGetNode)
	ON_BN_CLICKED(IDC_OWN_NODE, OnBnClickedOwnNode)
	ON_BN_CLICKED(IDC_EDIT_LIST, OnBnClickedEditList)
	ON_BN_CLICKED(IDC_EDITPCS, OnBnClickedEditpcs)
END_MESSAGE_MAP()


// CCfgWPN message handlers
BOOL CCfgWPN::OnInitDialog()
{

	CCfgDlg::OnInitDialog();
	
	Load();
	return TRUE;
}

void CCfgWPN::OnBnClickedGetNode()
{

	m_btForcedNode.SetCheck(!m_btAutoNode.GetCheck());
	m_eNodeName.SetReadOnly(m_btAutoNode.GetCheck());
	m_eExternal.SetReadOnly(m_btAutoNode.GetCheck());
}

void CCfgWPN::OnBnClickedOwnNode()
{

	m_btAutoNode.SetCheck(!m_btForcedNode.GetCheck());
	m_eNodeName.SetReadOnly(m_btAutoNode.GetCheck());
	m_eExternal.SetReadOnly(m_btAutoNode.GetCheck());
}

void CCfgWPN::OnBnClickedEditList()
{

	ShellExecute(0, "open", g_sSettings.GetWorkingDir() + "\\browse_message.txt", NULL, NULL, SW_SHOW); 
}

void CCfgWPN::Save()
{

	UpdateData(TRUE);

	if(m_strNodeName.IsEmpty() && m_btForcedNode.GetCheck()){

		AfxMessageBox(IDS_ERROR_NODEREQUIRED, MB_ICONERROR);
		return;
	}

	g_sSettings.SetPrivacyMsg(m_strPrivacy);
	g_sSettings.SetIgnoredMsg(m_strIgnored);
	g_sSettings.SetForcedNode(m_strNodeName);
	g_sSettings.SetNodeExternal(m_strExternal);
	g_sSettings.SetForcePrimary(m_btForcedNode.GetCheck());
}

void CCfgWPN::Load()
{

	m_strNodeName = g_sSettings.GetForcedNode();
	m_strExternal = g_sSettings.GetNodeExternal();
	m_strPrivacy  = g_sSettings.GetPrivacyMsg();
	m_strIgnored  = g_sSettings.GetIgnoredMsg();

	if(g_sSettings.GetForcePrimary() == 1){

		m_btAutoNode.SetCheck(FALSE);
		m_btForcedNode.SetCheck(TRUE);
		m_eNodeName.SetReadOnly(FALSE);
		m_eExternal.SetReadOnly(FALSE);
	}
	else{

		m_btAutoNode.SetCheck(TRUE);
		m_btForcedNode.SetCheck(FALSE);
		m_eNodeName.SetReadOnly(TRUE);
		m_eExternal.SetReadOnly(TRUE);
	}
	
	UpdateData(FALSE);
}

void CCfgWPN::Apply()
{
	
	Save();
	Load();
}

void CCfgWPN::OnBnClickedEditpcs()
{

	CCfgAddPeerCacheDlg dlg;
	dlg.DoModal();
}
