// CfgUserlist.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "CfgUserlist.h"
#include "Settings.h"
#include ".\cfguserlist.h"


// CCfgUserlist dialog
extern CSettings g_sSettings;

CCfgUserlist::CCfgUserlist(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CCfgUserlist::IDD, pParent)
	, m_bOpTop(TRUE)
	, m_bHiliteUsers(FALSE)
	, m_nUserListMode(0)
	, m_strImage(_T(""))
	, m_hNormal(NULL)
	, m_hAdmin(NULL)
	, m_hVoiced(NULL)
	, m_hHost(NULL)
	, m_bUseBg(FALSE)
	, m_bTile(FALSE)
	, m_nXOffset(0)
	, m_nYOffset(0)
	, m_strAvatar(_T(""))
	, m_strDefChannel(_T(""))
{
}

CCfgUserlist::~CCfgUserlist()
{
}

void CCfgUserlist::DoDataExchange(CDataExchange* pDX)
{
	CCfgDlg::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_OPTTOP, m_bOpTop);
	DDX_Check(pDX, IDC_USERHILITE, m_bHiliteUsers);
	DDX_Control(pDX, IDC_STATIC_NORMAL, m_stNormal);
	DDX_Control(pDX, IDC_STATIC_VOICED, m_stVoiced);
	DDX_Control(pDX, IDC_STATIC_ADMIN, m_stAdmin);
	DDX_Control(pDX, IDC_STATIC_HOST, m_stHost);
	DDX_Text(pDX, IDC_STATIC_PATH, m_strImage);
	DDX_Control(pDX, IDC_COMPACT, m_btCompact);
	DDX_Control(pDX, IDC_DETAILED, m_btDetailed);
	DDX_Control(pDX, IDC_BIGICON, m_btBigIcon);
	DDX_Control(pDX, IDC_SMALLICON, m_btSmallIcon);
	DDX_Check(pDX, IDC_USE_BG_IMG, m_bUseBg);
	DDX_Check(pDX, IDC_TILE, m_bTile);
	DDX_Text(pDX, IDC_X_OFFSET, m_nXOffset);
	DDV_MinMaxInt(pDX, m_nXOffset, 0, 100);
	DDX_Text(pDX, IDC_Y_OFFSET, m_nYOffset);
	DDV_MinMaxInt(pDX, m_nYOffset, 0, 100);
	DDX_Text(pDX, IDC_STATIC_PATHAVATAR, m_strAvatar);
	DDX_Text(pDX, IDC_STATIC_PATH3, m_strDefChannel);
}


BEGIN_MESSAGE_MAP(CCfgUserlist, CCfgDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_NORMAL, OnBnClickedNormal)
	ON_BN_CLICKED(IDC_VOICED, OnBnClickedVoiced)
	ON_BN_CLICKED(IDC_ADMIN, OnBnClickedAdmin)
	ON_BN_CLICKED(IDC_HOST, OnBnClickedHost)
	ON_BN_CLICKED(IDC_SELECT_IMAGE, OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_DETAILED, OnBnClickedDetailed)
	ON_BN_CLICKED(IDC_COMPACT, OnBnClickedCompact)
	ON_BN_CLICKED(IDC_BIGICON, OnBnClickedBigicon)
	ON_BN_CLICKED(IDC_SMALLICON, OnBnClickedSmallicon)
	ON_BN_CLICKED(IDC_SELECT_AVATAR, OnBnClickedSelectAvatar)
	ON_BN_CLICKED(IDC_SELECT_IMAGE3, OnBnClickedSelectImage3)
END_MESSAGE_MAP()


// CCfgUserlist message handlers
void CCfgUserlist::OnDestroy()
{

	CCfgDlg::OnDestroy();

	if(m_hAdmin != NULL){

		DeleteObject(m_hAdmin);
	}
	if(m_hVoiced != NULL){

		DeleteObject(m_hVoiced);
	}
	if(m_hHost != NULL){

		DeleteObject(m_hHost);
	}
	if(m_hNormal != NULL){

		DeleteObject(m_hNormal);
	}
}

BOOL CCfgUserlist::OnInitDialog() 
{

	CCfgDlg::OnInitDialog();

	m_bHiliteUsers = g_sSettings.GetHiliteUsers();
	m_bOpTop	   = g_sSettings.GetOpsTop();

	m_strNormal	= g_sSettings.GetUserModeNormal();
	m_strVoiced	= g_sSettings.GetUserModeVoiced();
	m_strAdmin	= g_sSettings.GetUserModeAdmin();
	m_strHost	= g_sSettings.GetUserModeHost();

	m_hNormal = (HBITMAP)::LoadImage(0, m_strNormal, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT|LR_LOADFROMFILE);
	m_hVoiced = (HBITMAP)::LoadImage(0, m_strVoiced, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT|LR_LOADFROMFILE);
	m_hAdmin = (HBITMAP)::LoadImage(0, m_strAdmin, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT|LR_LOADFROMFILE);
	m_hHost = (HBITMAP)::LoadImage(0, m_strHost, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT|LR_LOADFROMFILE);

	m_stNormal.SetBitmap(m_hNormal);
	m_stVoiced.SetBitmap(m_hVoiced);
	m_stAdmin.SetBitmap(m_hAdmin);
	m_stHost.SetBitmap(m_hHost);

	m_nUserListMode = g_sSettings.GetUserlistMode();
	m_btDetailed.SetCheck(m_nUserListMode == 0);
	m_btCompact.SetCheck(m_nUserListMode == 1);
	m_btSmallIcon.SetCheck(m_nUserListMode == 2);
	m_btBigIcon.SetCheck(m_nUserListMode == 3);

	m_bUseBg = g_sSettings.GetUseUserlistBg();
	m_strImage = g_sSettings.GetUserlistBg();

	m_bTile = g_sSettings.GetUserlistTileBg();
	m_nXOffset = g_sSettings.GetUserlistXOffsest();
	m_nYOffset = g_sSettings.GetUSerlistYOffset();

	m_strAvatar = g_sSettings.GetUserlistDefaultImage();
	m_strDefChannel = g_sSettings.GetChannelDefaultImage();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CCfgUserlist::Apply()
{

	if(UpdateData(TRUE)){

		g_sSettings.SetOpsTop(m_bOpTop);
		g_sSettings.SetHiliteUsers(m_bHiliteUsers);
		g_sSettings.SetUserlistMode(m_nUserListMode);
		g_sSettings.SetUserlistBg(m_strImage);
		g_sSettings.SetUseUserlistBg(m_bUseBg);
		g_sSettings.SetUserModeNormal(m_strNormal);
		g_sSettings.SetUserModeVoiced(m_strVoiced);
		g_sSettings.SetUserModeAdmin(m_strAdmin);
		g_sSettings.SetUserModeHost(m_strHost);
		g_sSettings.SetUserlistTileBg(m_bTile);
		g_sSettings.SetUserlistXOffsest(m_nXOffset);
		g_sSettings.SetUSerlistYOffset(m_nYOffset);
		g_sSettings.SetUserlistDefaultImage(m_strAvatar);
		g_sSettings.SetChannelDefaultImage(m_strDefChannel);
	}
}

void CCfgUserlist::OnBnClickedNormal()
{

	CString strFilter;
	strFilter.LoadString(IDS_BITMAP_FILTER);

	CFileDialog dlg(TRUE, ".bmp", m_strNormal, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		if(dlg.GetPathName() != m_strNormal){

			m_strNormal = dlg.GetPathName();
			if(m_hNormal) DeleteObject(m_hNormal);
			m_hNormal = (HBITMAP)::LoadImage(0, m_strNormal, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT|LR_LOADFROMFILE);
			m_stNormal.SetBitmap(m_hNormal);
		}
	}
}

void CCfgUserlist::OnBnClickedVoiced()
{

	CString strFilter;
	strFilter.LoadString(IDS_BITMAP_FILTER);

	CFileDialog dlg(TRUE, ".bmp", m_strVoiced, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		if(dlg.GetPathName() != m_strVoiced){

			m_strVoiced = dlg.GetPathName();
			if(m_hVoiced) DeleteObject(m_hVoiced);
			m_hVoiced = (HBITMAP)::LoadImage(0, m_strVoiced, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT|LR_LOADFROMFILE);
			m_stVoiced.SetBitmap(m_hVoiced);
		}
	}
}

void CCfgUserlist::OnBnClickedAdmin()
{

	CString strFilter;
	strFilter.LoadString(IDS_BITMAP_FILTER);

	CFileDialog dlg(TRUE, ".bmp", m_strAdmin, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		if(dlg.GetPathName() != m_strAdmin){

			m_strAdmin = dlg.GetPathName();
			if(m_hAdmin) DeleteObject(m_hAdmin);
			m_hAdmin = (HBITMAP)::LoadImage(0, m_strAdmin, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT|LR_LOADFROMFILE);
			m_stAdmin.SetBitmap(m_hAdmin);
		}
	}
}

void CCfgUserlist::OnBnClickedHost()
{

	CString strFilter;
	strFilter.LoadString(IDS_BITMAP_FILTER);

	CFileDialog dlg(TRUE, ".bmp", m_strHost, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		if(dlg.GetPathName() != m_strHost){

			m_strHost = dlg.GetPathName();
			if(m_hHost) DeleteObject(m_hHost);
			m_hHost = (HBITMAP)::LoadImage(0, m_strHost, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT|LR_LOADFROMFILE);
			m_stHost.SetBitmap(m_hHost);
		}
	}
}

void CCfgUserlist::OnBnClickedBrowse()
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


void CCfgUserlist::OnBnClickedSelectAvatar()
{

	CString strFilter;
	strFilter.LoadString(IDS_IMAGE_FILTER);

	CFileDialog dlg(TRUE, ".jpg", m_strAvatar, OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		if(dlg.GetPathName() != m_strAvatar){

			m_strAvatar = dlg.GetPathName();
			UpdateData(FALSE);
		}
	}
}


void CCfgUserlist::OnBnClickedSelectImage3()
{

	CString strFilter;
	strFilter.LoadString(IDS_IMAGE_FILTER);

	CFileDialog dlg(TRUE, ".jpg", m_strDefChannel, OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		if(dlg.GetPathName() != m_strDefChannel){

			m_strDefChannel = dlg.GetPathName();
			UpdateData(FALSE);
		}
	}
}

void CCfgUserlist::OnBnClickedDetailed()
{

	m_nUserListMode = 0;
	m_btDetailed.SetCheck(m_nUserListMode == 0);
	m_btCompact.SetCheck(m_nUserListMode == 1);
	m_btSmallIcon.SetCheck(m_nUserListMode == 2);
	m_btBigIcon.SetCheck(m_nUserListMode == 3);
}

void CCfgUserlist::OnBnClickedCompact()
{

	m_nUserListMode = 1;
	m_btDetailed.SetCheck(m_nUserListMode == 0);
	m_btCompact.SetCheck(m_nUserListMode == 1);
	m_btSmallIcon.SetCheck(m_nUserListMode == 2);
	m_btBigIcon.SetCheck(m_nUserListMode == 3);
}

void CCfgUserlist::OnBnClickedBigicon()
{

	m_nUserListMode = 3;
	m_btDetailed.SetCheck(m_nUserListMode == 0);
	m_btCompact.SetCheck(m_nUserListMode == 1);
	m_btSmallIcon.SetCheck(m_nUserListMode == 2);
	m_btBigIcon.SetCheck(m_nUserListMode == 3);
}


void CCfgUserlist::OnBnClickedSmallicon()
{

	m_nUserListMode = 2;
	m_btDetailed.SetCheck(m_nUserListMode == 0);
	m_btCompact.SetCheck(m_nUserListMode == 1);
	m_btSmallIcon.SetCheck(m_nUserListMode == 2);
	m_btBigIcon.SetCheck(m_nUserListMode == 3);
}

