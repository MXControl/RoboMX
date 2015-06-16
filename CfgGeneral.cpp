// CfgGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "CfgGeneral.h"
#include "Settings.h"
#include "MainFrm.h"
#include "FolderDialog.h"
#include "WPNSetup.h"
#include ".\cfggeneral.h"
extern CSettings g_sSettings;


// CCfgGeneral dialog

CCfgGeneral::CCfgGeneral(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CCfgGeneral::IDD, pParent)
	, m_bUpdate(FALSE)
	, m_bAutoList(FALSE)
	, m_bScroller(FALSE)
	, m_nHistory(0)
	, m_bHistory(FALSE)
	, m_bPing(FALSE)
	, m_bRetry(TRUE)
	, m_nRetries(3)
	, m_bAutoWPN(TRUE)
	, m_bLoadLog(FALSE)
{
}

void CCfgGeneral::DoDataExchange(CDataExchange* pDX)
{
	CCfgDlg::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_ENABLE_HISTORY, m_bHistory);
	DDX_Text(pDX, IDC_HISTORDEPTH, m_nHistory);
	DDX_Check(pDX, IDC_PING, m_bPing);
	DDX_Check(pDX, IDC_UPDATE, m_bUpdate);
	DDX_Check(pDX, IDC_LISTCHANNEL, m_bAutoList);
	DDX_Check(pDX, IDC_SCROLLER, m_bScroller);
	DDX_Check(pDX, IDC_LOG, m_bLog);
	DDX_Check(pDX, IDC_CONNECT_WPN, m_bAutoWPN);
	DDX_Text(pDX, IDC_LOGDIR, m_strPath);
	DDX_Text(pDX, IDC_JOINRETRIES, m_nRetries);
	DDX_Check(pDX, IDC_RETRY_JOINS, m_bRetry);
	DDV_MinMaxInt(pDX, m_nRetries, 0, 5);
	DDX_Check(pDX, IDC_LOAD_LAST_LOG, m_bLoadLog);
}


BEGIN_MESSAGE_MAP(CCfgGeneral, CCfgDlg)
	ON_BN_CLICKED(IDC_SELECT_LOG_DIR, OnBnClickedSelectLogDir)
	ON_BN_CLICKED(IDC_RETRY_JOINS, OnBnClickedRetryJoins)
	ON_BN_CLICKED(IDC_ENABLE_HISTORY, OnBnClickedEnableHistory)
	ON_BN_CLICKED(IDC_CONNECT_WPN, OnBnClickedConnectWpn)
	ON_BN_CLICKED(IDC_OPENFOLDER, OnBnClickedOpenfolder)
END_MESSAGE_MAP()


// CCfgGeneral message handlers
BOOL CCfgGeneral::OnInitDialog(void)
{

	CCfgDlg::OnInitDialog();

	m_bHistory     = g_sSettings.GetSaveHistory();
	m_nHistory     = g_sSettings.GetHistoryDepth();
	m_bPing		   = g_sSettings.GetPing();
	m_bUpdate	   = g_sSettings.GetUpdate();
	m_bAutoList	   = g_sSettings.GetAutoList();
	m_bScroller    = g_sSettings.GetEnableScroller();
	m_bLog		   = g_sSettings.GetLog();
	m_strPath	   = g_sSettings.GetLogDir();
	m_bRetry	   = g_sSettings.GetRetry();
	m_nRetries	   = g_sSettings.GetRetries();
	m_bAutoWPN	   = g_sSettings.GetAutoWPN();
	m_bLoadLog	   = g_sSettings.GetLoadLog();

	UpdateData(FALSE);

	return TRUE;
}

void CCfgGeneral::Apply(void)
{

	UpdateData(TRUE);
	g_sSettings.SetSaveHistory(m_bHistory);
	g_sSettings.SetHistoryDepth(m_nHistory);
	g_sSettings.SetPing(m_bPing);
	g_sSettings.SetUpdate(m_bUpdate);
	g_sSettings.SetAutoList(m_bAutoList);
	g_sSettings.SetEnableScroller(m_bScroller);
	g_sSettings.SetLog(m_bLog);
	g_sSettings.SetLogDir(m_strPath);
	g_sSettings.SetRetries(m_nRetries);
	g_sSettings.SetRetry(m_bRetry);
	g_sSettings.SetAutoWPN(m_bAutoWPN);
	g_sSettings.SetLoadLog(m_bLoadLog);
}


void CCfgGeneral::OnBnClickedRetryJoins()
{

	BOOL bCheck = ((CButton*)GetDlgItem(IDC_RETRY_JOINS))->GetCheck();

	GetDlgItem(IDC_JOINRETRIES)->EnableWindow(bCheck);
}

void CCfgGeneral::OnBnClickedEnableHistory()
{ 

	BOOL bCheck = ((CButton*)GetDlgItem(IDC_ENABLE_HISTORY))->GetCheck();

	GetDlgItem(IDC_HISTORDEPTH)->EnableWindow(bCheck);
}

void CCfgGeneral::OnBnClickedConnectWpn()
{

	if(!((CButton*)GetDlgItem(IDC_CONNECT_WPN))->GetCheck()){

		AfxMessageBox(IDS_WPN_WARNING, MB_ICONINFORMATION);
	}
}

void CCfgGeneral::OnBnClickedOpenfolder()
{

	ShellExecute(0, "open", m_strPath, 0, 0, SW_SHOW);
}

void CCfgGeneral::OnBnClickedSelectLogDir()
{

	UpdateData(TRUE);
	CFolderDialog dlg;
	if(dlg.DoModal() == IDOK){

		m_strPath = dlg.GetPathName();
		UpdateData(FALSE);
	}
}