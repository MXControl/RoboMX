// CfgChannelBar.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "CfgChannelBar.h"
#include ".\cfgchannelbar.h"
#include "Settings.h"
#include "Ini.h"

extern CSettings g_sSettings;

// CCfgChannelBar dialog

CCfgChannelBar::CCfgChannelBar(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CCfgChannelBar::IDD, pParent)
	, m_bUseOld(FALSE)
{
}

CCfgChannelBar::~CCfgChannelBar()
{
}

void CCfgChannelBar::DoDataExchange(CDataExchange* pDX)
{
	CCfgDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TOOLTIPTYPE, m_cbType);
	DDX_CBIndex(pDX, IDC_TOOLTIPTYPE, m_nType);
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
	DDX_Text(pDX, IDC_CHANNELNAME, m_strChannel);
	DDX_Text(pDX, IDC_IMAGE, m_strImage);
	DDX_Control(pDX, IDC_CHANNELIMAGES, m_lcChannelImages);
	DDX_Check(pDX, IDC_USEOLD, m_bUseOld);
}


BEGIN_MESSAGE_MAP(CCfgChannelBar, CCfgDlg)
	ON_BN_CLICKED(IDC_BG1, OnBnClickedBg1)
	ON_BN_CLICKED(IDC_BG2, OnBnClickedBg2)
	ON_BN_CLICKED(IDC_BG3, OnBnClickedBg3)
	ON_BN_CLICKED(IDC_BROWSE, OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_CBN_SELCHANGE(IDC_TOOLTIPTYPE, OnCbnSelchangeTooltiptype)
	ON_BN_CLICKED(IDC_HELP, OnBnClickedHelp)
	ON_BN_CLICKED(IDC_VARS, OnBnClickedVars)
	ON_BN_CLICKED(IDC_USEOLD, OnBnClickedUseold)
	ON_BN_CLICKED(IDC_EDIT, OnBnClickedEdit)
END_MESSAGE_MAP()

void CCfgChannelBar::Apply()
{

	if(UpdateData(TRUE)){

		OnCbnSelchangeTooltiptype();
		g_sSettings.SetToolBg1(m_stBg1.GetBkColor());
		g_sSettings.SetToolBg2(m_stBg2.GetBkColor());
		g_sSettings.SetToolBg3(m_stBg3.GetBkColor());
		g_sSettings.SetToolEffect(m_nEffect);
		g_sSettings.SetToolGran(m_nGran);
		g_sSettings.SetToolTrans(m_nTrans);
		g_sSettings.SetUseOldTools(m_bUseOld);

		int nCnt = m_lcChannelImages.GetItemCount();
		
		CIni ini;
		ini.SetIniFileName(g_sSettings.GetIniFile());

		ini.SetValue("ChannelToolTips", "ChannelImageCnt", nCnt);

		CString strLabel, strChannel, strImage;

		for(int i = 0; i < nCnt; i++){

			strChannel	= m_lcChannelImages.GetItemText(i, 0);
			strImage	= m_lcChannelImages.GetItemText(i, 1);

			strLabel.Format("ChannelName_%d", i);
			ini.SetValue("ChannelToolTips", strLabel, strChannel);
			strLabel.Format("ChannelImage_%d", i);
		    ini.SetValue("ChannelToolTips", strLabel, strImage);
		}
	}
}

BOOL CCfgChannelBar::OnInitDialog()
{

	CCfgDlg::OnInitDialog();

	m_nGran		= g_sSettings.GetToolGran();
	m_nTrans	= g_sSettings.GetToolTrans();
	m_nType		= 0;
	m_nEffect	= g_sSettings.GetToolEffect();
	m_stBg1.SetBkColor(g_sSettings.GetToolBg1());
	m_stBg2.SetBkColor(g_sSettings.GetToolBg2());
	m_stBg3.SetBkColor(g_sSettings.GetToolBg3());
	m_strToolHtml = g_sSettings.GetToolList();
	m_bUseOld = g_sSettings.GetUseOldTools();
	UpdateData(FALSE);

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	m_lcChannelImages.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lcChannelImages.InsertColumn(0, "Channel", LVCFMT_LEFT, 200);
	m_lcChannelImages.InsertColumn(1, "Image", LVCFMT_LEFT, 200);

	int nCnt = ini.GetValue("ChannelToolTips", "ChannelImageCnt", 0);
	CString strLabel, strChannel, strImage;
	int nAdd;

	for(int i = 0; i < nCnt; i++){

		strLabel.Format("ChannelName_%d", i);
		strChannel = ini.GetValue("ChannelToolTips", strLabel, "");
		strLabel.Format("ChannelImage_%d", i);
		strImage = ini.GetValue("ChannelToolTips", strLabel, "");

		if(strChannel.IsEmpty() || strImage.IsEmpty()) continue;

		nAdd = m_lcChannelImages.InsertItem(0, strChannel, 0);
		m_lcChannelImages.SetItemText(nAdd, 1, strImage);
	}

	OnBnClickedUseold();

	return TRUE;
}
// CCfgChannelBar message handlers

void CCfgChannelBar::OnCbnSelchangeTooltiptype()
{

	switch(m_nType){

		case 0:
			g_sSettings.SetToolList(m_strToolHtml);
			break;
		case 1:
			g_sSettings.SetToolListHover(m_strToolHtml);
			break;
		case 2:
			g_sSettings.SetToolRoom(m_strToolHtml);
			break;
		case 3:
			g_sSettings.SetToolPM(m_strToolHtml);
			break;
		case 4:
			g_sSettings.SetToolFiles(m_strToolHtml);
			break;
		case 5:
			g_sSettings.SetToolSettings(m_strToolHtml);
			break;
	}

	if(!UpdateData(TRUE)) return;
	
	switch(m_nType){

		case 0:
			m_strToolHtml = g_sSettings.GetToolList();
			break;
		case 1:
			m_strToolHtml = g_sSettings.GetToolListHover();
			break;
		case 2:
			m_strToolHtml = g_sSettings.GetToolRoom();
			break;
		case 3:
			m_strToolHtml = g_sSettings.GetToolPM();
			break;
		case 4:
			m_strToolHtml = g_sSettings.GetToolFiles();
			break;
		case 5:
			m_strToolHtml = g_sSettings.GetToolSettings();
			break;
	}

	UpdateData(FALSE);
}

void CCfgChannelBar::OnBnClickedBg1()
{

	CColorDialog dlg(m_stBg1.GetBkColor());

	if(dlg.DoModal() == IDOK){

		m_stBg1.SetBkColor(dlg.GetColor());
	}
}

void CCfgChannelBar::OnBnClickedBg2()
{

	CColorDialog dlg(m_stBg2.GetBkColor());

	if(dlg.DoModal() == IDOK){

		m_stBg2.SetBkColor(dlg.GetColor());
	}
}

void CCfgChannelBar::OnBnClickedBg3()
{

	CColorDialog dlg(m_stBg3.GetBkColor());

	if(dlg.DoModal() == IDOK){

		m_stBg3.SetBkColor(dlg.GetColor());
	}
}

void CCfgChannelBar::OnBnClickedBrowse()
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

void CCfgChannelBar::OnBnClickedAdd()
{

	if(!UpdateData(TRUE)) return;
	if(m_strChannel.IsEmpty() || m_strImage.IsEmpty()){

		AfxMessageBox(IDS_SPECIFYFIRST, MB_ICONINFORMATION);
		return;
	}

	int nAdd = m_lcChannelImages.InsertItem(0, m_strChannel, 0);
	m_lcChannelImages.SetItemText(nAdd, 1, m_strImage);

	m_strImage.Empty();
	m_strChannel.Empty();

	UpdateData(FALSE);    
}


void CCfgChannelBar::OnBnClickedEdit()
{

	int nSel = m_lcChannelImages.GetSelectionMark();
	if(nSel >= 0){

		m_strChannel = m_lcChannelImages.GetItemText(nSel, 0);
		m_strImage   = m_lcChannelImages.GetItemText(nSel, 1);
		m_lcChannelImages.DeleteItem(nSel);
		UpdateData(FALSE);
	}
}

void CCfgChannelBar::OnBnClickedRemove()
{

	int nSel = m_lcChannelImages.GetSelectionMark();
	if(nSel >= 0){

		m_lcChannelImages.DeleteItem(nSel);
	}
}

void CCfgChannelBar::OnBnClickedHelp()
{

	ShellExecute(0, "open", "http://mxcontrol.sf.net/appfiles/tooltips.html", 0, 0, SW_SHOW);
}

void CCfgChannelBar::OnBnClickedVars()
{

	ShellExecute(0, "open", g_sSettings.GetWorkingDir() + "\\tooltip-variables.txt", 0, 0, SW_SHOW);
}

void CCfgChannelBar::OnBnClickedUseold()
{

	UpdateData(TRUE);
	GetDlgItem(IDC_TOOLTIPTYPE)->EnableWindow(!m_bUseOld);
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
