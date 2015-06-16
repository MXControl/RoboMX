// CfgFormat.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "CfgFormat.h"
#include "Settings.h"


// CCfgFormat dialog
extern CSettings g_sSettings;

CCfgFormat::CCfgFormat(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CCfgFormat::IDD, pParent)
	, m_bEmoticons(FALSE)
	, m_bMaxi(FALSE)
	, m_nBarTop(0)
	, m_bTimeStamp(FALSE)
	, m_nTime(-1)
	, m_bMiniTray(FALSE)
	, m_bHideSystem(TRUE)
	, m_bSavePos(TRUE)
	, m_bPMToolTip(TRUE)
	, m_bImage(FALSE)
	, m_strImage(_T(""))
	, m_nEncoding(-1)
	, m_nSize(12)
	, m_strFont(_T(""))
	, m_bBold(FALSE)
	, m_bItalic(FALSE)
	, m_bLine(FALSE)
	, m_bSep(FALSE)
	, m_bShowFillStatus(FALSE)
{
}

CCfgFormat::~CCfgFormat()
{
}

void CCfgFormat::DoDataExchange(CDataExchange* pDX)
{
	CCfgDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FONT, m_cbFont);
	DDX_Control(pDX, IDC_ENCODING, m_cbEncoding);
	DDX_CBIndex(pDX, IDC_ENCODING, m_nEncoding);
	DDX_Text(pDX, IDC_FONTSIZE, m_nSize);
	DDX_Text(pDX, IDC_FONT, m_strFont);
	DDX_Check(pDX, IDC_BOLD, m_bBold);
	DDX_Check(pDX, IDC_ITALIC, m_bItalic);
	DDX_Check(pDX, IDC_UNDERLINE, m_bLine);
	DDX_Check(pDX, IDC_USE_BGIMAGE, m_bImage);
	DDX_Text(pDX, IDC_STATIC_IMAGEPATH, m_strImage);
	DDX_Check(pDX, IDC_TIMESTAMP, m_bTimeStamp);
	DDX_CBIndex(pDX, IDC_TIME, m_nTime);
	DDX_Check(pDX, IDC_MINITRAY, m_bMiniTray);
	DDX_Check(pDX, IDC_EMOTICONS, m_bEmoticons);
	DDX_Check(pDX, IDC_MDIMAXI, m_bMaxi);
	DDX_Check(pDX, IDC_HIDESYSTEM, m_bHideSystem);
	DDX_Check(pDX, IDC_REMEMBER_POS, m_bSavePos);
	DDX_CBIndex(pDX, IDC_BARTOP, m_nBarTop);
	DDX_Control(pDX, IDC_MDIMAXI, m_btMaxi);
	DDX_Control(pDX, IDC_REMEMBER_POS, m_btWinPos);
	DDX_Check(pDX, IDC_DISPLAYPMTOOL, m_bPMToolTip);
	DDX_Check(pDX, IDC_SEPERATOR, m_bSep);
	DDX_Check(pDX, IDC_SHOWFILLSTATUS, m_bShowFillStatus);
}


BEGIN_MESSAGE_MAP(CCfgFormat, CCfgDlg)
	ON_BN_CLICKED(IDC_SELECT_IMAGE, OnSelectImage)
	ON_BN_CLICKED(IDC_MDIMAXI, OnBnClickedMdimaxi)
	ON_BN_CLICKED(IDC_REMEMBER_POS, OnBnClickedRememberPos)
END_MESSAGE_MAP()


// CCfgFormat message handlers
BOOL CCfgFormat::OnInitDialog() 
{

	CCfgDlg::OnInitDialog();
	

	CClientDC dc(this);		
	EnumFonts(dc, 0, (FONTENUMPROC)EnumFontsProc, (LPARAM)&m_cbFont);

	m_strFont		= g_sSettings.GetFont();
	
	int nFont = m_cbFont.FindStringExact(-1, m_strFont);
	if(nFont != CB_ERR){

		m_cbFont.SetCurSel(nFont);
	}
	
	m_nSize = g_sSettings.GetFontSize();
	m_nEncoding = g_sSettings.GetCharSet(TRUE);

	m_bBold		   = g_sSettings.GetMessageBold();
	m_bItalic	   = g_sSettings.GetMessageItalic();
	m_bLine        = g_sSettings.GetMessageLine();
	m_bImage	   = g_sSettings.GetUseImage();
	m_strImage	   = g_sSettings.GetImage();
	m_nBarTop	   = g_sSettings.GetBarTop();
	m_bTimeStamp   = g_sSettings.GetPrintTime();
	m_nTime		   = g_sSettings.GetTimeFmt();
	m_bMiniTray    = g_sSettings.GetMiniTray();
	m_bEmoticons   = g_sSettings.GetEmoticons();
	m_bHideSystem  = g_sSettings.GetHideSystem();
	m_bSavePos     = g_sSettings.GetSavePos();
	m_bPMToolTip   = g_sSettings.GetPMToolTip();
	m_bMaxi		   = g_sSettings.GetMaxi();
	m_bSep		   = g_sSettings.GetSepOnSwitch();
	m_bShowFillStatus = g_sSettings.GetShowFillStatus();

	UpdateData(FALSE);

	OnBnClickedRememberPos();
	OnBnClickedMdimaxi();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCfgFormat::Apply()
{

	UpdateData(TRUE);
	
	g_sSettings.SetCharSet(m_nEncoding);
	g_sSettings.SetFontSize(m_nSize);
	g_sSettings.SetFont(m_strFont);
	g_sSettings.SetMessageBold(m_bBold);
	g_sSettings.SetMessageItalic(m_bItalic);
	g_sSettings.SetMessageLine(m_bLine);
	g_sSettings.SetImage(m_strImage);
	g_sSettings.SetUseImage(m_bImage);
	g_sSettings.SetPrintTime(m_bTimeStamp);
	g_sSettings.SetTimeFmt(m_nTime);
	g_sSettings.SetMiniTray(m_bMiniTray);
	g_sSettings.SetEmoticons(m_bEmoticons);
	g_sSettings.SetMaxi(m_bMaxi);
	g_sSettings.SetHideSystem(m_bHideSystem);
	g_sSettings.SetBarTop(m_nBarTop);
	g_sSettings.SetSavePos(m_bSavePos);
	g_sSettings.SetPMToolTip(m_bPMToolTip);
	g_sSettings.SetSepOnSwith(m_bSep);
	g_sSettings.SetShowFillStatus(m_bShowFillStatus);

	GetApp()->ApplyPic();
}

void CCfgFormat::OnSelectImage() 
{

	CString strFilter;
	strFilter.LoadString(IDS_IMAGE_FILTER);

	CFileDialog dlg(TRUE, ".jpg", m_strImage, OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		if(dlg.GetPathName() != m_strImage){

			m_strImage = dlg.GetPathName();
			UpdateData(FALSE);
		}
	}
}

BOOL CALLBACK EnumFontsProc(LPLOGFONT lpFont, LPTEXTMETRIC lpMetric, DWORD dwType, LPARAM lpData)
{


	CComboBox *pThis = (CComboBox*)(lpData);		
	int index = pThis->AddString(lpFont->lfFaceName);
	
	ASSERT(index!=-1);
	
	return TRUE;
}

void CCfgFormat::OnBnClickedMdimaxi()
{

	m_btWinPos.EnableWindow(!m_btMaxi.GetCheck());
	if(m_btMaxi.GetCheck()){

		m_btWinPos.SetCheck(FALSE);
	}
}

void CCfgFormat::OnBnClickedRememberPos()
{

	m_btMaxi.EnableWindow(!m_btWinPos.GetCheck());
	if(m_btWinPos.GetCheck()){

		m_btMaxi.SetCheck(FALSE);
	}
}

