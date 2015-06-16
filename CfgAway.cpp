// CfgAway.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "CfgAway.h"
#include ".\cfgaway.h"
#include "Settings.h"
#include "Ini.h"
#include "Util.h"

extern CSettings g_sSettings;

// CCfgAway dialog

CCfgAway::CCfgAway(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CCfgAway::IDD, pParent)
	, m_bAwayIfIdle(FALSE)
	, m_dwIdleDuration(15)
	, m_strAutoReason(_T(""))
	, m_strAutoPrefix(_T(""))
	, m_strAutoSuffix(_T(""))
	, m_strLabel(_T(""))
	, m_strReason(_T(""))
	, m_strPrefix(_T(""))
	, m_strSuffix(_T(""))
	, m_bAutoBack(FALSE)
{
}

CCfgAway::~CCfgAway()
{
}

void CCfgAway::DoDataExchange(CDataExchange* pDX)
{
	CCfgDlg::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_AWAY_IF_IDLE, m_bAwayIfIdle);
	DDX_Text(pDX, IDC_MINUTES, m_dwIdleDuration);
#ifdef _DEBUG
    DDV_MinMaxDWord(pDX, m_dwIdleDuration, 1, 1000);
#else
	DDV_MinMaxDWord(pDX, m_dwIdleDuration, 15, 1000);
#endif
	DDX_Text(pDX, IDC_AUTOREASON, m_strAutoReason);
	DDV_MaxChars(pDX, m_strAutoReason, 250);
	DDX_Text(pDX, IDC_AUTOPREFIX, m_strAutoPrefix);
	DDV_MaxChars(pDX, m_strAutoPrefix, 10);
	DDX_Text(pDX, IDC_AUTOSUFFIX, m_strAutoSuffix);
	DDV_MaxChars(pDX, m_strAutoSuffix, 10);
	DDX_Text(pDX, IDC_LABEL, m_strLabel);
	DDV_MaxChars(pDX, m_strLabel, 20);
	DDX_Text(pDX, IDC_REASON, m_strReason);
	DDV_MaxChars(pDX, m_strReason, 250);
	DDX_Text(pDX, IDC_PREFIX, m_strPrefix);
	DDV_MaxChars(pDX, m_strPrefix, 10);
	DDX_Text(pDX, IDC_SUFFIX, m_strSuffix);
	DDV_MaxChars(pDX, m_strSuffix, 10);
	DDX_Control(pDX, IDC_AWAY, m_lcAway);
	DDX_Check(pDX, IDC_AUTOBACK, m_bAutoBack);
}


BEGIN_MESSAGE_MAP(CCfgAway, CCfgDlg)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_EDIT, OnBnClickedEdit)
	ON_BN_CLICKED(IDC_DELETE, OnBnClickedDelete)
END_MESSAGE_MAP()


// CCfgAway message handlers
BOOL CCfgAway::OnInitDialog()
{

	CCfgDlg::OnInitDialog();

	m_lcAway.InsertColumn(0, "Label", LVCFMT_LEFT, 100);
	m_lcAway.InsertColumn(1, "Reason", LVCFMT_LEFT, 150);
	m_lcAway.InsertColumn(2, "Prefix", LVCFMT_LEFT, 50);
	m_lcAway.InsertColumn(3, "Suffix", LVCFMT_LEFT, 50);
	ListView_SetExtendedListViewStyle(m_lcAway.m_hWnd, LVS_EX_FULLROWSELECT);

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	CString strLabel, strValue;

	int nCnt = ini.GetValue("AwayControl", "AwayCnt", 0);
	int n = 0;
	for(int i = 0; i < nCnt; i++){

		strLabel.Format("Label_%d", i);
		strValue = ini.GetValue("AwayControl", strLabel, "");
		if(strValue.IsEmpty()) continue;
		n = m_lcAway.InsertItem(0, strValue, 0);

		strLabel.Format("Reason_%d", i);
		strValue = ini.GetValue("AwayControl", strLabel, "");
		m_lcAway.SetItemText(n, 1, strValue);

		strLabel.Format("Prefix_%d", i);
		strValue = ini.GetValue("AwayControl", strLabel, "");
		m_lcAway.SetItemText(n, 2, strValue);

		strLabel.Format("Suffix_%d", i);
		strValue = ini.GetValue("AwayControl", strLabel, "");
		m_lcAway.SetItemText(n, 3, strValue);
	}

	m_dwIdleDuration = g_sSettings.GetIdleDuration() / 60000;
	m_strAutoReason = g_sSettings.GetAutoReason();
	m_strAutoPrefix = g_sSettings.GetAutoPrefix();
	m_strAutoSuffix = g_sSettings.GetAutoSuffix();
	m_bAwayIfIdle   = g_sSettings.GetAwayIfIdle();
	m_bAutoBack		= g_sSettings.GetAutoBack();

	UpdateData(FALSE);
	return TRUE;
}

void CCfgAway::Apply()
{

	if(UpdateData(TRUE)){

		g_sSettings.SetIdleDuration(m_dwIdleDuration*1000*60);
		g_sSettings.SetAutoReason(m_strAutoReason);
		g_sSettings.SetAutoPrefix(m_strAutoPrefix);
		g_sSettings.SetAutoSuffix(m_strAutoSuffix);
		g_sSettings.SetAwayIfIdle(m_bAwayIfIdle);
		g_sSettings.SetAutoBack(m_bAutoBack);
	}
	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	CString strLabel, strValue;

	int nCnt = m_lcAway.GetItemCount();
	ini.SetValue("AwayControl", "AwayCnt", nCnt);

	for(int i = 0; i < nCnt; i++){

		strLabel.Format("Label_%d", i);
		ini.SetValue("AwayControl", strLabel, m_lcAway.GetItemText(i, 0));

		strLabel.Format("Reason_%d", i);
		ini.SetValue("AwayControl", strLabel, m_lcAway.GetItemText(i, 1));

		strLabel.Format("Prefix_%d", i);
		ini.SetValue("AwayControl", strLabel, m_lcAway.GetItemText(i, 2));
		
		strLabel.Format("Suffix_%d", i);
		ini.SetValue("AwayControl", strLabel, m_lcAway.GetItemText(i, 3));
	}
}

void CCfgAway::OnBnClickedAdd()
{

	if(UpdateData(TRUE)){

		if(m_strLabel.IsEmpty() || m_strReason.IsEmpty()){

			AfxMessageBox(IDS_AWAYREQUIRED, MB_ICONINFORMATION);
			return;
		}

		CString strAwayName = Util::GetBaseName(g_sSettings.GetNickname());
		//strAwayName);
		strAwayName.Insert(0, m_strPrefix);
		if(strAwayName.GetLength()-3 > 0){

			strAwayName.Insert(strAwayName.GetLength()-3, m_strSuffix);
		}
		Util::MakeValidUserName(strAwayName, FALSE, 0);
		if(strAwayName.GetLength() > 44){

			CString strWarning;
			AfxMessageBox(IDS_STATUSPREFIXLENGTHWARNING,	MB_OK+MB_ICONWARNING);
		}

		int nAdd = m_lcAway.InsertItem(0, m_strLabel, 0);
		m_lcAway.SetItemText(nAdd, 1, m_strReason);
		m_lcAway.SetItemText(nAdd, 2, m_strPrefix);
		m_lcAway.SetItemText(nAdd, 3, m_strSuffix);

		m_strLabel.Empty();
		m_strReason.Empty();
		m_strPrefix.Empty();
		m_strSuffix.Empty();
		UpdateData(FALSE);
	}
}

void CCfgAway::OnBnClickedEdit()
{

	if(m_lcAway.GetSelectedCount()){

		int nSel	= m_lcAway.GetSelectionMark();
		m_strLabel	= m_lcAway.GetItemText(nSel, 0);
		m_strReason = m_lcAway.GetItemText(nSel, 1);
		m_strPrefix = m_lcAway.GetItemText(nSel, 2);
		m_strSuffix = m_lcAway.GetItemText(nSel, 3);
		m_lcAway.DeleteItem(nSel);
		UpdateData(FALSE);
	}
}

void CCfgAway::OnBnClickedDelete()
{

	if(m_lcAway.GetSelectedCount()){

		int nSel = m_lcAway.GetSelectionMark();
		m_lcAway.DeleteItem(nSel);
	}
}
