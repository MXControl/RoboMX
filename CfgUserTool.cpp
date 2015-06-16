// CfgUserTool.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "CfgUserTool.h"
#include "Settings.h"
#include "Ini.h"

extern CSettings g_sSettings;

// CCfgUserTool dialog

CCfgUserTool::CCfgUserTool(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CCfgUserTool::IDD, pParent)
	, m_bUseOld(FALSE)
{
}

CCfgUserTool::~CCfgUserTool()
{
}

void CCfgUserTool::DoDataExchange(CDataExchange* pDX)
{
	CCfgDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TOOLTIPHTML, m_strToolHtml);
	DDX_Control(pDX, IDC_BG_EFFECT, m_cbEffect);
	DDX_CBIndex(pDX, IDC_BG_EFFECT, m_nEffect);
	DDX_Control(pDX, IDC_STATIC_BG1, m_stBg1);
	DDX_Control(pDX, IDC_STATIC_BG2, m_stBg2);
	DDX_Control(pDX, IDC_STATIC_BG3, m_stBg3);
	DDX_Text(pDX, IDC_GRAN, m_nGran);
	DDV_MinMaxInt(pDX, m_nGran, 0, 255);
	DDX_Text(pDX, IDC_TRANS, m_nTrans);
	DDV_MinMaxInt(pDX, m_nTrans, 0, 100);
	DDX_Text(pDX, IDC_USERNAME, m_strChannel);
	DDX_Text(pDX, IDC_IMAGE, m_strImage);
	DDX_Control(pDX, IDC_USERIMAGES, m_lcUserImages);
	DDX_Check(pDX, IDC_USEOLD, m_bUseOld);
}


BEGIN_MESSAGE_MAP(CCfgUserTool, CCfgDlg)
	ON_BN_CLICKED(IDC_BG1, OnBnClickedBg1)
	ON_BN_CLICKED(IDC_BG2, OnBnClickedBg2)
	ON_BN_CLICKED(IDC_BG3, OnBnClickedBg3)
	ON_BN_CLICKED(IDC_BROWSE, OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_BN_CLICKED(IDC_HELP, OnBnClickedHelp)
	ON_BN_CLICKED(IDC_VARS, OnBnClickedVars)
	ON_BN_CLICKED(IDC_USEOLD, OnBnClickedUseold)
	ON_BN_CLICKED(IDC_EDIT, OnBnClickedEdit)
END_MESSAGE_MAP()


// CCfgUserTool message handlers
void CCfgUserTool::Apply()
{

	if(UpdateData(TRUE)){

		g_sSettings.SetUserToolBg1(m_stBg1.GetBkColor());
		g_sSettings.SetUserToolBg2(m_stBg2.GetBkColor());
		g_sSettings.SetUserToolBg3(m_stBg3.GetBkColor());
		g_sSettings.SetUserToolEffect(m_nEffect);
		g_sSettings.SetUserToolGran(m_nGran);
		g_sSettings.SetUserToolTrans(m_nTrans);
		g_sSettings.SetUserUseOldTools(m_bUseOld);
		g_sSettings.SetUserTool(m_strToolHtml);

		int nCnt = m_lcUserImages.GetItemCount();
		
		CIni ini;
		ini.SetIniFileName(g_sSettings.GetIniFile());

		ini.SetValue("UserToolTips", "UserImageCnt", nCnt);

		CString strLabel, strChannel, strImage;

		for(int i = 0; i < nCnt; i++){

			strChannel	= m_lcUserImages.GetItemText(i, 0);
			strImage	= m_lcUserImages.GetItemText(i, 1);

			strLabel.Format("UserName_%d", i);
			ini.SetValue("UserToolTips", strLabel, strChannel);
			strLabel.Format("UserImage_%d", i);
		    ini.SetValue("UserToolTips", strLabel, strImage);
		}
	}
}

BOOL CCfgUserTool::OnInitDialog()
{

	CCfgDlg::OnInitDialog();


	m_nGran		= g_sSettings.GetUserToolGran();
	m_nTrans	= g_sSettings.GetUserToolTrans();
	m_nType		= 0;
	m_nEffect	= g_sSettings.GetUserToolEffect();
	m_stBg1.SetBkColor(g_sSettings.GetUserToolBg1());
	m_stBg2.SetBkColor(g_sSettings.GetUserToolBg2());
	m_stBg3.SetBkColor(g_sSettings.GetUserToolBg3());
	m_strToolHtml = g_sSettings.GetUserTool();
	m_bUseOld = g_sSettings.GetUserUseOldTools();
	UpdateData(FALSE);


	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	m_lcUserImages.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lcUserImages.InsertColumn(0, "Channel", LVCFMT_LEFT, 200);
	m_lcUserImages.InsertColumn(1, "Image", LVCFMT_LEFT, 200);

	int nCnt = ini.GetValue("UserToolTips", "UserImageCnt", 0);
	CString strLabel, strChannel, strImage;
	int nAdd;

	for(int i = 0; i < nCnt; i++){

		strLabel.Format("UserName_%d", i);
		strChannel = ini.GetValue("UserToolTips", strLabel, "");
		strLabel.Format("UserImage_%d", i);
		strImage = ini.GetValue("UserToolTips", strLabel, "");

		if(strChannel.IsEmpty() || strImage.IsEmpty()) continue;

		nAdd = m_lcUserImages.InsertItem(0, strChannel, 0);
		m_lcUserImages.SetItemText(nAdd, 1, strImage);
	}

	OnBnClickedUseold();

	return TRUE;
}

void CCfgUserTool::OnBnClickedBg1()
{

	CColorDialog dlg(m_stBg1.GetBkColor());

	if(dlg.DoModal() == IDOK){

		m_stBg1.SetBkColor(dlg.GetColor());
	}
}

void CCfgUserTool::OnBnClickedBg2()
{

	CColorDialog dlg(m_stBg2.GetBkColor());

	if(dlg.DoModal() == IDOK){

		m_stBg2.SetBkColor(dlg.GetColor());
	}
}

void CCfgUserTool::OnBnClickedBg3()
{

	CColorDialog dlg(m_stBg3.GetBkColor());

	if(dlg.DoModal() == IDOK){

		m_stBg3.SetBkColor(dlg.GetColor());
	}
}

void CCfgUserTool::OnBnClickedBrowse()
{

	if(!UpdateData(TRUE)) return;

	CString strFilter;
	strFilter.LoadString(IDS_IMAGE_FILTER);

	CFileDialog dlg(TRUE, ".jpg", m_strImage, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		if(dlg.GetPathName() != m_strImage){

			m_strImage = dlg.GetPathName();
			UpdateData(FALSE);
		}
	}
}

void CCfgUserTool::OnBnClickedAdd()
{

	if(!UpdateData(TRUE)) return;
	if(m_strChannel.IsEmpty() || m_strImage.IsEmpty()){

		AfxMessageBox(IDS_SPECIFYFIRST, MB_ICONINFORMATION);
		return;
	}

	int nAdd = m_lcUserImages.InsertItem(0, m_strChannel, 0);
	m_lcUserImages.SetItemText(nAdd, 1, m_strImage);

	m_strImage.Empty();
	m_strChannel.Empty();

	UpdateData(FALSE);    
}


void CCfgUserTool::OnBnClickedEdit()
{

	int nSel = m_lcUserImages.GetSelectionMark();
	if(nSel >= 0){

		m_strChannel = m_lcUserImages.GetItemText(nSel, 0);
		m_strImage   = m_lcUserImages.GetItemText(nSel, 1);
		m_lcUserImages.DeleteItem(nSel);
		UpdateData(FALSE);
	}
}

void CCfgUserTool::OnBnClickedRemove()
{

	int nSel = m_lcUserImages.GetSelectionMark();
	if(nSel >= 0){

		m_lcUserImages.DeleteItem(nSel);
	}
}

void CCfgUserTool::OnBnClickedHelp()
{

	ShellExecute(0, "open", "http://mxcontrol.sf.net/appfiles/UserToolTips.html", 0, 0, SW_SHOW);
}

void CCfgUserTool::OnBnClickedVars()
{

	ShellExecute(0, "open", g_sSettings.GetWorkingDir() + "\\tooltip-variables.txt", 0, 0, SW_SHOW);
}

void CCfgUserTool::OnBnClickedUseold()
{

	UpdateData(TRUE);
	GetDlgItem(IDC_VARS)->EnableWindow(!m_bUseOld);
	GetDlgItem(IDC_HELP_INET)->EnableWindow(!m_bUseOld);
	GetDlgItem(IDC_TOOLTIPHTML)->EnableWindow(!m_bUseOld);
	GetDlgItem(IDC_BG_EFFECT)->EnableWindow(!m_bUseOld);
	GetDlgItem(IDC_BG1)->EnableWindow(!m_bUseOld);
	GetDlgItem(IDC_STATIC_BG1)->EnableWindow(!m_bUseOld);
	GetDlgItem(IDC_BG2)->EnableWindow(!m_bUseOld);
	GetDlgItem(IDC_STATIC_BG2)->EnableWindow(!m_bUseOld);
	GetDlgItem(IDC_BG3)->EnableWindow(!m_bUseOld);
	GetDlgItem(IDC_STATIC_BG3)->EnableWindow(!m_bUseOld);
	GetDlgItem(IDC_GRAN)->EnableWindow(!m_bUseOld);
	GetDlgItem(IDC_TRANS)->EnableWindow(!m_bUseOld);
}
