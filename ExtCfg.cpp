// ExtCfg.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "ExtCfg.h"
#include "RoboEx.h"
#include "Settings.h"
#include "MainFrm.h"
#include ".\extcfg.h"

extern CSettings g_sSettings;
extern CArray<PLUGIN_DATA, PLUGIN_DATA> g_aPlugins;

// CExtCfg dialog

IMPLEMENT_DYNAMIC(CExtCfg, CCfgDlg)
CExtCfg::CExtCfg(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CExtCfg::IDD, pParent)
{
}

CExtCfg::~CExtCfg()
{
}

void CExtCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLUGINLIST, m_lcPlugins);
}


BEGIN_MESSAGE_MAP(CExtCfg, CCfgDlg)
	ON_BN_CLICKED(IDC_CONFIGURE, OnBnClickedConfigure)
	ON_BN_CLICKED(IDC_RESCAN, OnBnClickedRescan)
	ON_BN_CLICKED(IDC_UNINSTALL, OnBnClickedUninstall)
	ON_NOTIFY(NM_DBLCLK, IDC_PLUGINLIST, OnBnClickedConfigure)
END_MESSAGE_MAP()

BOOL CExtCfg::OnInitDialog()
{

	CCfgDlg::OnInitDialog();

	CString strTitle;
	strTitle.LoadString(IDS_PLUGIN);
	m_lcPlugins.InsertColumn(0, strTitle, LVCFMT_LEFT, 100);
	strTitle.LoadString(IDS_DESC);
	m_lcPlugins.InsertColumn(1, strTitle, LVCFMT_LEFT, 200);
	strTitle.LoadString(IDS_AUTHOR);
	m_lcPlugins.InsertColumn(2, strTitle, LVCFMT_LEFT, 210);
	ListView_SetExtendedListViewStyle(m_lcPlugins.m_hWnd, LVS_EX_FULLROWSELECT);
	
	int i = 0, j = 0;
	for(i = 0; i < g_aPlugins.GetSize(); i++){

		j = m_lcPlugins.InsertItem(0, g_aPlugins[i].pEx->m_strName, 0);
		m_lcPlugins.SetItemText(j, 1, g_aPlugins[i].pEx->m_strDescription);
		m_lcPlugins.SetItemText(j, 2, g_aPlugins[i].pEx->m_strAuthor);
	}

	return TRUE;
}
// CExtCfg message handlers
void CExtCfg::OnBnClickedConfigure()
{

	int nSel = m_lcPlugins.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		CString strName = m_lcPlugins.GetItemText(nSel, 0);
		for(int i = 0; i < g_aPlugins.GetSize(); i++){

			if(g_aPlugins[i].pEx->m_strName == strName){

				g_aPlugins[i].pEx->Configure();
			}
		}
	}
	else{

		AfxMessageBox(IDS_ERROR_SELECT_PLUGIN, MB_ICONINFORMATION);
	}
}

void CExtCfg::OnBnClickedRescan()
{

	m_lcPlugins.DeleteAllItems();
	GetApp()->GetMainFrame()->ReloadPlugins();

	int i = 0, j = 0;
	for(i = 0; i < g_aPlugins.GetSize(); i++){

		j = m_lcPlugins.InsertItem(0, g_aPlugins[i].pEx->m_strName, 0);
		m_lcPlugins.SetItemText(j, 1, g_aPlugins[i].pEx->m_strDescription);
		m_lcPlugins.SetItemText(j, 2, g_aPlugins[i].pEx->m_strAuthor);
	}
}

void CExtCfg::OnBnClickedUninstall()
{

	int nSel = m_lcPlugins.GetNextItem(-1, LVNI_SELECTED);
	if(nSel >= 0){

		CString strName = m_lcPlugins.GetItemText(nSel, 0);
		CString strOut;
		strOut.Format(IDS_REMOVE_PLUGIN, strName);
		if(AfxMessageBox(strOut, MB_ICONQUESTION+MB_YESNO) == IDYES){

			if(!GetApp()->GetMainFrame()->DeletePlugin(strName)){

				strOut.Format(IDS_ERROR_REMOVE_PLUGIN, strName);
				AfxMessageBox(strOut, MB_ICONSTOP);
			}
			else{

				m_lcPlugins.DeleteItem(nSel);
			}
		}
	}
}

void CExtCfg::OnBnClickedConfigure(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnBnClickedConfigure();
	*pResult = 0;
}
