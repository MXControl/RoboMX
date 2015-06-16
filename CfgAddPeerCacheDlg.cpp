// CfgAddPeerCacheDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "CfgAddPeerCacheDlg.h"
#include ".\cfgaddpeercachedlg.h"
#include "Ini.h"
#include "Settings.h"

// CCfgAddPeerCacheDlg dialog

extern CSettings g_sSettings;

IMPLEMENT_DYNAMIC(CCfgAddPeerCacheDlg, CDialog)
CCfgAddPeerCacheDlg::CCfgAddPeerCacheDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCfgAddPeerCacheDlg::IDD, pParent)
	, m_strServer(_T(""))
{
}

CCfgAddPeerCacheDlg::~CCfgAddPeerCacheDlg()
{
}

void CCfgAddPeerCacheDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVERS, m_wndServers);
	DDX_Text(pDX, IDC_ADD_SERVER, m_strServer);
}


BEGIN_MESSAGE_MAP(CCfgAddPeerCacheDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CCfgAddPeerCacheDlg message handlers

void CCfgAddPeerCacheDlg::OnBnClickedButton1()
{

	if(UpdateData(TRUE)){

		m_wndServers.AddString(m_strServer);
		m_strServer.Empty();
		UpdateData(FALSE);
	}
}

void CCfgAddPeerCacheDlg::OnBnClickedDelete()
{

	if(m_wndServers.GetCurSel() >= 0){

		m_wndServers.DeleteString(m_wndServers.GetCurSel());
	}
}

void CCfgAddPeerCacheDlg::OnBnClickedOk()
{

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());
	int nNum = m_wndServers.GetCount();

	CString strTmp, str;
	for(int i = 0; i < nNum; i++){

		strTmp.Format(_T("Server_%03d"), i);
		m_wndServers.GetText(i, str);
		ini.SetValue(_T("PeerCache"), strTmp, str);
	}

	ini.SetValue(_T("PeerCache"), _T("Count"), nNum);

	OnOK();
}


BOOL CCfgAddPeerCacheDlg::OnInitDialog(void)
{

	CDialog::OnInitDialog();

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());
	CString strTmp, str;
	int nNum = ini.GetValue(_T("PeerCache"), _T("Count"), 0);

	for(int i = 0; i < nNum; i++){

		strTmp.Format(_T("Server_%03d"), i);
		str = ini.GetValue(_T("PeerCache"), strTmp, _T(""));
		
		if(str.IsEmpty()) continue;
		
		m_wndServers.AddString(str);
	}
	return TRUE;
}
