// StatusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "StatusDlg.h"
#include ".\statusdlg.h"

// CStatusDlg dialog

IMPLEMENT_DYNAMIC(CStatusDlg, CDialog)
CStatusDlg::CStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStatusDlg::IDD, pParent)
	, m_strOperation(_T(""))
{
}

CStatusDlg::~CStatusDlg()
{
}

void CStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS, m_pgStatus);
	DDX_Text(pDX, IDC_TEXT_ACTION, m_strOperation);
}


BEGIN_MESSAGE_MAP(CStatusDlg, CDialog)
	ON_MESSAGE(WM_SETSTATUS, OnSetStatus)
END_MESSAGE_MAP()


// CStatusDlg message handlers
BOOL CStatusDlg::OnInitDialog()
{

	CDialog::OnInitDialog();

	m_pgStatus.SetRange32(0, 100);
	m_pgStatus.SetPos(0);

	return TRUE;

}

LRESULT CStatusDlg::OnSetStatus(WPARAM wParam, LPARAM lParam)
{

	
	BOOL bUpdateTitle = LOWORD(wParam);
	int  nStatus      = HIWORD(wParam);

	if(bUpdateTitle){

		m_strOperation = (LPCTSTR)lParam;
		UpdateData(FALSE);
	}

	m_pgStatus.SetPos(nStatus);

	return 1;
}
void CStatusDlg::Center(CWnd* pParent)
{

	CRect rc, rcDlg, rcMove;
	
	pParent->GetWindowRect(&rc);
	GetWindowRect(&rcDlg);
	POINT p;
	p.x  = (rc.Width() - rcDlg.Width()) / 2;
	p.y   = (rc.Height() - rcDlg.Height()) / 2;
	//ClientToScreen(&rc);
	
	//ClientToScreen(&p);
	//ScreenToClient(&p);

	SetWindowPos(NULL, rc.left+p.x, rc.top+p.y, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}
