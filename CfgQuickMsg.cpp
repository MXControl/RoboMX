// CfgQuickMsg.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "CfgQuickMsg.h"
#include "Settings.h"
#include ".\cfgquickmsg.h"
//#include "util.h"


// CCfgQuickMsg dialog
extern CSettings    g_sSettings;

CCfgQuickMsg::CCfgQuickMsg(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CCfgQuickMsg::IDD, pParent)
	, m_strAdd(_T(""))

{
}

CCfgQuickMsg::~CCfgQuickMsg()
{
}

void CCfgQuickMsg::DoDataExchange(CDataExchange* pDX)
{
	CCfgDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_QUICKCMDS, m_lbQuick);
	DDX_Text(pDX, IDC_MSG_ADD, m_strAdd);
}


BEGIN_MESSAGE_MAP(CCfgQuickMsg, CCfgDlg)
	ON_BN_CLICKED(IDC_MSG_ADD_BTN, OnMsgAddBtn)
	ON_BN_CLICKED(IDC_QUICKREM, OnQuickrem)
	ON_BN_CLICKED(IDC_QUICKUP, OnQuickup)
	ON_BN_CLICKED(IDC_QUICKDOWN, OnQuickdown)
	ON_EN_CHANGE(IDC_MSG_ADD, OnEnChangeMsgAdd)
	ON_BN_CLICKED(IDC_EDIT, OnBnClickedEdit)
END_MESSAGE_MAP()

BOOL CCfgQuickMsg::OnInitDialog() 
{

	CCfgDlg::OnInitDialog();
	
	//m_mToolTip.Create(this);
	//m_mToolTip.AddControlInfo(IDC_MSG_ADD, IDS_TOOL_MSGVARIABLES);
	m_mToolTip.Create(this, FALSE);
	m_mToolTip.AddTool(GetDlgItem(IDC_MSG_ADD), IDS_TOOL_MSGVARIABLES, 0, 0, 0);

	LoadQuickCmds();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCfgQuickMsg::Apply()
{
	UpdateData(TRUE);
	
	SaveQuickCmds();
	LoadQuickCmds();
}


void CCfgQuickMsg::LoadQuickCmds()
{

	m_lbQuick.ResetContent();

	for(int i = 0; i < g_sSettings.m_aQuick.GetSize(); i++){

		m_lbQuick.InsertString(m_lbQuick.GetCount(), g_sSettings.m_aQuick[i]);
	}
}

void CCfgQuickMsg::SaveQuickCmds()
{

	CString strIniFile = g_sSettings.GetWorkingDir() + "\\quick.ini";
	CStdioFile ini;
	CString strBuffer;

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText|CFile::shareExclusive);

		for(int i = 0; i < m_lbQuick.GetCount(); i++){
			
			m_lbQuick.GetText(i, strBuffer);
			ini.WriteString(strBuffer + "\n");
		}
		ini.Flush();
		ini.Close();
		
	}
	CATCH(CFileException, e){

		AfxMessageBox("Error during file operation", MB_OK+MB_ICONSTOP);

	}END_CATCH;
}

void CCfgQuickMsg::OnMsgAddBtn() 
{

	UpdateData(TRUE);
	if(m_strAdd.IsEmpty()) return;

	m_lbQuick.AddString(m_strAdd);
	g_sSettings.m_aQuick.Add(m_strAdd);
	m_strAdd.Empty();

	UpdateData(FALSE);
}

void CCfgQuickMsg::OnBnClickedEdit()
{

	int nSel = m_lbQuick.GetCurSel();
	if(nSel != -1){

		m_lbQuick.GetText(nSel, m_strAdd);
		m_lbQuick.DeleteString(nSel);
		g_sSettings.m_aQuick.RemoveAt(nSel);
		UpdateData(FALSE);
	}
}

void CCfgQuickMsg::OnQuickrem() 
{

	int nSel = m_lbQuick.GetCurSel();
	if(nSel != -1){

		m_lbQuick.DeleteString(nSel);
		g_sSettings.m_aQuick.RemoveAt(nSel);
	}
}

void CCfgQuickMsg::OnQuickup() 
{
	
	int nSel = m_lbQuick.GetCurSel();
	if(nSel != LB_ERR){

		CString strTmp;
		m_lbQuick.GetText(nSel, strTmp);
		m_lbQuick.DeleteString(nSel);
		g_sSettings.m_aQuick.RemoveAt(nSel--);
		m_lbQuick.InsertString(nSel < 0 ? m_lbQuick.GetCount() : nSel, strTmp);
		g_sSettings.m_aQuick.InsertAt(nSel < 0 ? g_sSettings.m_aQuick.GetSize() : nSel, strTmp);
		m_lbQuick.SetCurSel(nSel < 0 ? m_lbQuick.GetCount()-1 : nSel);
	}	
}

void CCfgQuickMsg::OnQuickdown() 
{
	
	int nSel = m_lbQuick.GetCurSel();
	if(nSel != LB_ERR){

		CString strTmp;
		m_lbQuick.GetText(nSel, strTmp);
		m_lbQuick.DeleteString(nSel);
		g_sSettings.m_aQuick.RemoveAt(nSel++);
		m_lbQuick.InsertString(nSel > m_lbQuick.GetCount() ? 0 : nSel,strTmp);
		g_sSettings.m_aQuick.InsertAt(nSel > g_sSettings.m_aQuick.GetSize() ? 0 : nSel, strTmp);
		m_lbQuick.SetCurSel(nSel > m_lbQuick.GetCount()-1 ? 0 : nSel);
	}	
}

BOOL CCfgQuickMsg::PreTranslateMessage( MSG *pMsg )
{

	/*if(pMsg->message == WM_MOUSEMOVE){

		//POINT pt = pMsg->pt;    
		//ScreenToClient(&pt);

		// this is the only function to call
		//m_mToolTip.ShowToolTip((CPoint)pt);
	}*/

	m_mToolTip.RelayEvent(pMsg);

	return CCfgDlg::PreTranslateMessage(pMsg);
}

void CCfgQuickMsg::OnEnChangeMsgAdd()
{

	//m_mToolTip.ShowToolTip(IDC_MSG_ADD);
}



