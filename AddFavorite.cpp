// AddFavorite.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "AddFavorite.h"


// CAddFavorite dialog

IMPLEMENT_DYNAMIC(CAddFavorite, CDialog)
CAddFavorite::CAddFavorite(CWnd* pParent /*=NULL*/)
	: CDialog(CAddFavorite::IDD, pParent)
	, m_strChannel(_T(""))
	, m_strLogin(_T(""))
	, m_bAutoJoin(false)
{
}

CAddFavorite::~CAddFavorite()
{
}

void CAddFavorite::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NETWORK, m_cbNetwork);
	DDX_Text(pDX, IDC_CHANNEL, m_strChannel);
	DDX_Text(pDX, IDC_LOGIN2, m_strLogin);
	DDX_Check(pDX, IDC_AUTOJOIN, m_bAutoJoin);
}


BEGIN_MESSAGE_MAP(CAddFavorite, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CAddFavorite message handlers

BOOL CAddFavorite::OnInitDialog(void)
{

	CDialog::OnInitDialog();

	m_cbNetwork.SetCurSel(0);
	UpdateData(FALSE);
	return TRUE;
}

void CAddFavorite::OnBnClickedOk()
{

	if(UpdateData(TRUE)){

		if(m_strChannel.GetLength()){

			OnOK();
		}
		else{

			AfxMessageBox(IDS_ERROR_ROOMNAME_EMPTY, MB_OK+MB_ICONEXCLAMATION);
		}
	}
}
