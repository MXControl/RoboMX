// CfgWelcome.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "CfgWelcome.h"
#include "Settings.h"
#include "ini.h"
#include ".\cfgwelcome.h"

extern CSettings g_sSettings;


// CCfgWelcome dialog

CCfgWelcome::CCfgWelcome(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CCfgWelcome::IDD, pParent)
	, m_bAllChannels(FALSE)
	, m_strChannel(_T(""))
	, m_strGreeting(_T(""))
{
}

void CCfgWelcome::DoDataExchange(CDataExchange* pDX)
{
	CCfgDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WELCOMES, m_lbGreetings);
	DDX_Text(pDX, IDC_CHANNELNAME, m_strChannel);
	DDX_Text(pDX, IDC_GREETING, m_strGreeting);
	DDX_Check(pDX, IDC_ALLCHANNELS, m_bAllChannels);
	DDX_Control(pDX, IDC_CHANNELLIST, m_lbChannels);
}


BEGIN_MESSAGE_MAP(CCfgWelcome, CCfgDlg)
	ON_BN_CLICKED(IDC_ALLCHANNELS, OnAllchannels)
	ON_BN_CLICKED(IDC_ADDCHANNEL, OnAddchannel)
	ON_BN_CLICKED(IDC_REMOVECHANNEL, OnRemovechannel)
	ON_BN_CLICKED(IDC_ADDCHANNEL2, OnBnClickedAddchannel2)
	ON_BN_CLICKED(IDC_REMOVECHANNEL2, OnBnClickedRemovechannel2)
	ON_EN_CHANGE(IDC_GREETING, OnEnChangeGreeting)
	ON_BN_CLICKED(IDC_EDITROOM, OnBnClickedEditroom)
	ON_BN_CLICKED(IDC_EDITWELCOME, OnBnClickedEditwelcome)
END_MESSAGE_MAP()


// CCfgWelcome message handlers
BOOL CCfgWelcome::OnInitDialog() 
{

	CCfgDlg::OnInitDialog();
	
	m_bAllChannels = g_sSettings.GetDoEnterMsg();
	UpdateData(FALSE);

	m_mToolTip.Create(this, FALSE);
	m_mToolTip.AddTool(GetDlgItem(IDC_GREETING), IDS_TOOL_MSGVARIABLES, 0, 0, 0);
	m_mToolTip.AddTool(GetDlgItem(IDC_CHANNELNAME), IDS_TOOL_WILDCARDS, 0, 0, 0);
	LoadRooms();
	OnAllchannels();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCfgWelcome::OnAllchannels() 
{

	UpdateData(TRUE);
	GetDlgItem(IDC_CHANNELNAME)->EnableWindow(!m_bAllChannels);
	GetDlgItem(IDC_CHANNELLIST)->EnableWindow(!m_bAllChannels);
	GetDlgItem(IDC_ADDCHANNEL)->EnableWindow(!m_bAllChannels);
	GetDlgItem(IDC_REMOVECHANNEL)->EnableWindow(!m_bAllChannels);
}

void CCfgWelcome::LoadRooms()
{

	CIni ini;
	int n = 0;
	CString strNum, strTmp;
	
	m_lbChannels.ResetContent();
	m_lbGreetings.ResetContent();
	
	ini.SetIniFileName(g_sSettings.GetIniFile());
	
	n = ini.GetValue("WelcomeRooms", "Num", 0);

	for(int i = 0; i < n; i++){

		strNum.Format("Room_%d", i);
		strTmp = ini.GetValue("WelcomeRooms", strNum, "");
		if(!strTmp.IsEmpty()){
 
			m_lbChannels.InsertString(m_lbChannels.GetCount(), strTmp);
		}	
	}

	n = ini.GetValue("WelcomeMsgs", "Num", 0);
	for(int i = 0; i < n; i++){

		strNum.Format("Msg_%d", i);
		strTmp = ini.GetValue("WelcomeMsgs", strNum, "");
		if(!strTmp.IsEmpty()){
 
			m_lbGreetings.InsertString(m_lbGreetings.GetCount(), strTmp);
		}	
	}
}

void CCfgWelcome::SaveRooms()
{

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	CString strTmp, strBuffer;
	ini.SetValue("WelcomeRooms", "Num", m_lbChannels.GetCount());

	for(int i = 0; i < m_lbChannels.GetCount(); i++){
		
		m_lbChannels.GetText(i, strBuffer);
		strTmp.Format("Room_%d", i);
		ini.SetValue("WelcomeRooms", strTmp, strBuffer);
	}

	ini.SetValue("WelcomeMsgs", "Num", m_lbGreetings.GetCount());
	for(int i = 0; i < m_lbGreetings.GetCount(); i++){
			
		m_lbGreetings.GetText(i, strBuffer);
		strTmp.Format("Msg_%d", i);
		ini.SetValue("WelcomeMsgs", strTmp, strBuffer);
	}
}

void CCfgWelcome::Apply()
{

	UpdateData(TRUE);

	g_sSettings.SetDoEnterMsg(m_bAllChannels);	

	SaveRooms();
	LoadRooms();
}

void CCfgWelcome::OnAddchannel() 
{
	
	UpdateData(TRUE);
	if(m_strChannel.IsEmpty()) return;

	m_lbChannels.AddString(m_strChannel);
	m_strChannel.Empty();
	UpdateData(FALSE);
}

void CCfgWelcome::OnRemovechannel() 
{

	if(m_lbChannels.GetCurSel() != -1){

		m_lbChannels.DeleteString(m_lbChannels.GetCurSel());
	}
}


void CCfgWelcome::OnBnClickedAddchannel2()
{

	UpdateData(TRUE);
	if(m_strGreeting.IsEmpty()) return;

	m_lbGreetings.AddString(m_strGreeting);
	m_strGreeting.Empty();
	UpdateData(FALSE);
}

void CCfgWelcome::OnBnClickedRemovechannel2()
{

	if(m_lbGreetings.GetCurSel() != -1){

		m_lbGreetings.DeleteString(m_lbGreetings.GetCurSel());
	}
}

void CCfgWelcome::OnBnClickedEditroom()
{

	if(m_lbChannels.GetCurSel() != -1){

		m_lbChannels.GetText(m_lbGreetings.GetCurSel(), m_strChannel);
		m_lbChannels.DeleteString(m_lbChannels.GetCurSel());
	}
}

void CCfgWelcome::OnBnClickedEditwelcome()
{
	if(m_lbGreetings.GetCurSel() != -1){

		m_lbGreetings.GetText(m_lbGreetings.GetCurSel(), m_strGreeting);
		m_lbGreetings.DeleteString(m_lbGreetings.GetCurSel());
		UpdateData(FALSE);
	}
}

BOOL CCfgWelcome::PreTranslateMessage( MSG *pMsg )
{

	/*if(pMsg->message == WM_MOUSEMOVE){

		POINT pt = pMsg->pt;    
		ScreenToClient(&pt);

		// this is the only function to call
		m_mToolTip.ShowToolTip((CPoint)pt);
	}*/
	m_mToolTip.RelayEvent(pMsg);

	return CCfgDlg::PreTranslateMessage(pMsg);
}

void CCfgWelcome::OnEnChangeGreeting()
{

	//m_mToolTip.ShowToolTip(IDC_GREETING);
}


