// FunCfg.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "FunCfg.h"
#include "Ini.h"
#include "Settings.h"
#include ".\funcfg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCfgFun dialog

extern CSettings g_sSettings;

CCfgFun::CCfgFun(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CCfgFun::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCfgFun)
	m_strVendor = _T("");
	m_strSuff = _T("");
	m_strName = _T("");
	//}}AFX_DATA_INIT
	m_nAct = 0;
}


void CCfgFun::DoDataExchange(CDataExchange* pDX)
{
	CCfgDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCfgFun)
	DDX_Text(pDX, IDC_VENDOR_ADD, m_strVendor);
	DDX_Text(pDX, IDC_SUFFIX_ADD, m_strSuff);
	DDX_Text(pDX, IDC_NAME_ADD, m_strName);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_VENDORS, m_lcVendors);
	DDX_Control(pDX, IDC_NAMES, m_lcNames);
	DDX_Control(pDX, IDC_SUFFIXES, m_lcSuff);
}


BEGIN_MESSAGE_MAP(CCfgFun, CCfgDlg)
	ON_BN_CLICKED(IDC_BTN_VENDOR_ADD, OnBtnVendorAdd)
	ON_BN_CLICKED(IDC_BTN_NAME_ADD, OnBtnNameAdd)
	ON_BN_CLICKED(IDC_BTN_SUFFIX_ADD, OnBtnSuffixAdd)
	ON_NOTIFY(NM_DBLCLK, IDC_VENDORS, OnNMDblclkVendors)
	ON_NOTIFY(NM_DBLCLK, IDC_NAMES, OnNMDblclkNames)
	ON_NOTIFY(NM_DBLCLK, IDC_SUFFIXES, OnNMDblclkSuffixes)
	ON_BN_CLICKED(IDC_BTN_VENDOR_REM, OnBnClickedBtnVendorRem)
	ON_BN_CLICKED(IDC_BTN_NAME_REM, OnBnClickedBtnNameRem)
	ON_BN_CLICKED(IDC_BTN_SUFFIX_REM, OnBnClickedBtnSuffixRem)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCfgFun message handlers


BOOL CCfgFun::OnInitDialog() 
{
	CCfgDlg::OnInitDialog();
	m_lcSuff.InsertColumn(0, "Items", LVCFMT_LEFT, 250);
	m_lcNames.InsertColumn(0, "Items", LVCFMT_LEFT, 250);
	m_lcVendors.InsertColumn(0, "Items", LVCFMT_LEFT, 250);
	m_lcSuff.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lcNames.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lcVendors.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	Load();

	m_lcSuff.SetColumnWidth(0, LVSCW_AUTOSIZE);
	m_lcNames.SetColumnWidth(0, LVSCW_AUTOSIZE);
	m_lcVendors.SetColumnWidth(0, LVSCW_AUTOSIZE);

	UpdateData(FALSE);

	return TRUE;
}


void CCfgFun::Apply()
{

	if(!m_lcVendors.GetItemCount() || !m_lcNames.GetItemCount() || !m_lcSuff.GetItemCount()){

		AfxMessageBox(IDS_ERROR_NAMES_EMPTY, MB_ICONWARNING);
	}
	Save();
	UpdateData(TRUE);
}

void CCfgFun::OnBtnVendorAdd() 
{

	UpdateData(TRUE);
	
	if(!m_strVendor.IsEmpty()){

		m_lcVendors.InsertItem(0, m_strVendor, 0);
		m_strVendor.Empty();
		UpdateData(FALSE);
	}
	m_lcVendors.SetColumnWidth(0, LVSCW_AUTOSIZE);
}

void CCfgFun::OnBtnNameAdd() 
{
	
	UpdateData(TRUE);
	
	if(!m_strName.IsEmpty()){

		m_lcNames.InsertItem(0, m_strName, 0);
		m_strName.Empty();
		UpdateData(FALSE);
	}
	m_lcNames.SetColumnWidth(0, LVSCW_AUTOSIZE);
}

void CCfgFun::OnBtnSuffixAdd() 
{
	
	UpdateData(TRUE);
	
	if(!m_strSuff.IsEmpty()){

		m_lcSuff.InsertItem(0, m_strSuff, 0);
		m_strSuff.Empty();
		UpdateData(FALSE);
	}
	m_lcSuff.SetColumnWidth(0, LVSCW_AUTOSIZE);
}

void CCfgFun::Load()
{

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	int i = 0, nNum = 0;
	
	CString strNum;

	nNum = ini.GetValue("Title", "NumVendors", 1);
	
	for(i = 0; i < nNum; i++){

		strNum.Format("Vendor_%02d", i);
		m_lcVendors.InsertItem(0, ini.GetValue("Title", strNum, "BendSoft"));
	}

	nNum = ini.GetValue("Title", "NumNames", 1);
	
	for(i = 0; i < nNum; i++){

		strNum.Format("Name_%02d", i);
		m_lcNames.InsertItem(0, ini.GetValue("Title", strNum, "Robo"));
	}

	nNum = ini.GetValue("Title", "NumSuffix", 1);
	
	for(i = 0; i < nNum; i++){

		strNum.Format("Suffix_%02d", i);
		m_lcSuff.InsertItem(0, ini.GetValue("Title", strNum, "MX"));
	}
}

void CCfgFun::Save()
{

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	int i = 0, nNum = 0;
	
	CString strNum;

	nNum = m_lcVendors.GetItemCount();
	ini.SetValue("Title", "NumVendors", nNum);

	for(i = 0; i < nNum; i++){

		strNum.Format("Vendor_%02d", i);
		ini.SetValue("Title", strNum, m_lcVendors.GetItemText(i, 0));
	}

	nNum = m_lcNames.GetItemCount();
	ini.SetValue("Title", "NumNames", nNum);

	for(i = 0; i < nNum; i++){

		strNum.Format("Name_%02d", i);
		ini.SetValue("Title", strNum, m_lcNames.GetItemText(i, 0));
	}

	nNum = m_lcSuff.GetItemCount();
	ini.SetValue("Title", "NumSuffix", nNum);

	for(i = 0; i < nNum; i++){

		strNum.Format("Suffix_%02d", i);
		ini.SetValue("Title", strNum, m_lcSuff.GetItemText(i, 0));
	}
}


void CCfgFun::OnNMDblclkVendors(NMHDR *pNMHDR, LRESULT *pResult)
{

	int nSel = m_lcVendors.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		m_lcVendors.DeleteItem(nSel);
	}
	*pResult = 0;
}

void CCfgFun::OnNMDblclkNames(NMHDR *pNMHDR, LRESULT *pResult)
{

	int nSel = m_lcNames.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		m_lcNames.DeleteItem(nSel);
	}
	*pResult = 0;
}

void CCfgFun::OnNMDblclkSuffixes(NMHDR *pNMHDR, LRESULT *pResult)
{

	int nSel = m_lcSuff.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		m_lcSuff.DeleteItem(nSel);
	}
	*pResult = 0;
}


void CCfgFun::OnBnClickedBtnVendorRem()
{

	int nSel = m_lcVendors.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		m_lcVendors.DeleteItem(nSel);
	}
}

void CCfgFun::OnBnClickedBtnNameRem()
{
	int nSel = m_lcNames.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		m_lcNames.DeleteItem(nSel);
	}
}

void CCfgFun::OnBnClickedBtnSuffixRem()
{
	int nSel = m_lcSuff.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		m_lcSuff.DeleteItem(nSel);
	}
}
