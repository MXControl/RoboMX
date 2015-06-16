// ConfigDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "RoboMXPlugin.h"
#include "ConfigDlg.h"


// CConfigDlg dialog

IMPLEMENT_DYNAMIC(CConfigDlg, CDialog)
CConfigDlg::CConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigDlg::IDD, pParent)
{
}

CConfigDlg::~CConfigDlg()
{
}

void CConfigDlg::DoDataExchange(CDataExchange* pDX)
{

	DDX_Text(pDX, IDC_STOP, m_strStop);
	DDX_Text(pDX, IDC_START, m_strStart);
	DDX_Text(pDX, IDC_ANNOUNCE, m_strMsg);
	DDX_Text(pDX, IDC_URL, m_strURL);
	DDX_Text(pDX, IDC_STATS, m_strStats);

	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CConfigDlg, CDialog)
END_MESSAGE_MAP()


// CConfigDlg message handlers
BOOL CConfigDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	UpdateData(FALSE);

	return TRUE;
}