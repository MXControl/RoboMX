// LogView.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "LogView.h"
#include ".\logview.h"
#include "util.h"
#include "settings.h"

extern CSettings g_sSettings;

// CLogView dialog

IMPLEMENT_DYNAMIC(CLogView, CDialog)
CLogView::CLogView(CWnd* pParent /*=NULL*/)
	: CDialog(CLogView::IDD, pParent)
{
}

CLogView::~CLogView()
{
}

void CLogView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDC_DATES, m_lcDates);
}


BEGIN_MESSAGE_MAP(CLogView, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_DATES, OnLbnSelchangeDates)
	ON_LBN_DBLCLK(IDC_DATES, OnLbnDblclkDates)
END_MESSAGE_MAP()


// CLogView message handlers
BOOL CLogView::OnInitDialog()
{

	CDialog::OnInitDialog();

	m_btOK.EnableWindow(FALSE);

	CFileFind finder;
	BOOL bFind = finder.FindFile(g_sSettings.GetLogDir() + "\\" + m_strRoom + "_*.txt");
	CString strDate;
	while(bFind){

		bFind = finder.FindNextFile();
		strDate = finder.GetFileTitle();
		strDate.Replace(m_strRoom, "");
		strDate.Remove('_');
		m_lcDates.AddString(GetDateString(strDate));
	}
	if(m_lcDates.GetCount()){

		m_lcDates.SetCurSel(m_lcDates.GetCount()-1);
		m_btOK.EnableWindow(TRUE);
	}

	GetWindowText(strDate);
	strDate += " " + m_strRoom;
	SetWindowText(strDate);
	return TRUE;
}

void CLogView::OnBnClickedOk()
{

	int n = m_lcDates.GetCurSel();
	if(n >= 0){

		CString strFile; 
		m_lcDates.GetText(n, strFile);
		strFile.Remove('/');
		strFile = strFile.Mid(1,8);
		strFile.Insert(0, g_sSettings.GetLogDir() + "\\" + m_strRoom + "_");
		strFile+= ".txt";
		ShellExecute(0, "open", strFile, 0, 0, SW_SHOW);
		OnOK();
	}
}

void CLogView::OnLbnSelchangeDates()
{

	if(m_lcDates.GetCurSel() >= 0){

		m_btOK.EnableWindow(TRUE);
	}
	else{

		m_btOK.EnableWindow(FALSE);
	}
}

void CLogView::OnLbnDblclkDates()
{
	
	OnBnClickedOk();	
}

CString CLogView::GetDateString(CString strDate)
{

	int nYear  = atoi((LPCTSTR)strDate.Left(4));
	int nMonth = atoi((LPCTSTR)strDate.Mid(4,2));
	int nDay   = atoi((LPCTSTR)strDate.Mid(6,2));
	CTime time(nYear, nMonth, nDay, 1, 1, 1);
	CString strReturn = time.Format("[%Y/%m/%d] %A, %B %d, %Y");
	return strReturn;
}

