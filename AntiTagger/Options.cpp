// Options.cpp : implementation file
//

#include "stdafx.h"
//#include "AntiTagger.h"
#include "Options.h"
#include ".\options.h"
#include "..\Ini.h"

// COptions dialog

IMPLEMENT_DYNAMIC(COptions, CDialog)
COptions::COptions(CWnd* pParent /*=NULL*/)
	: CDialog(COptions::IDD, pParent)
	, m_strTag(_T(""))
{
}

COptions::~COptions()
{
}

void COptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADDTAG, m_strTag);
	DDX_Control(pDX, IDC_TAGS, m_lbTags);
}


BEGIN_MESSAGE_MAP(COptions, CDialog)
	ON_BN_CLICKED(IDC_ADD, OnBnClickedAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnBnClickedRemove)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// COptions message handlers
BOOL COptions::OnInitDialog()
{

	CDialog::OnInitDialog();

	CIni ini;
	ini.SetIniFileName(m_strIni);

	int nCount = ini.GetValue("AntiTagger", "Count", 0);
	
	CString strLabel, strTmp;
	for(int i = 0; i < nCount; i++){

		strLabel.Format("TAG_%d", i);
		strTmp = ini.GetValue("AntiTagger", strLabel, "");
		
		if(strTmp.IsEmpty()) continue;

        m_lbTags.AddString(strTmp);
	}

	return TRUE;
}

void COptions::OnBnClickedAdd()
{

	if(UpdateData(TRUE)){

		int n = m_lbTags.FindStringExact(-1, m_strTag);
		if(n < 0){

			m_lbTags.AddString(m_strTag);
		}
		m_strTag.Empty();
		UpdateData(FALSE);
	}
}

void COptions::OnBnClickedRemove()
{

	int nSel = m_lbTags.GetCurSel();
	if(nSel >= 0){

		m_lbTags.DeleteString(nSel);
	}
}

void COptions::OnBnClickedOk()
{

	if(UpdateData(TRUE)){

		if(m_strTag.GetLength()){

			CString strMsg;
			strMsg.Find("Tag \"%s\" was not added to list yet. Add it now (Otherwise it will be lost)?");
			if(AfxMessageBox(strMsg, MB_YESNO) == IDYES){

				OnBnClickedAdd();
			}
		}
	}

	CIni ini;
	ini.SetIniFileName(m_strIni);

	int nCount = m_lbTags.GetCount();
	
	ini.SetValue("AntiTagger", "Count", nCount);
	
	CString strLabel, strTmp;
	for(int i = 0; i < nCount; i++){

		m_lbTags.GetText(i, strTmp);
		strLabel.Format("TAG_%d", i);
		ini.SetValue("AntiTagger", strLabel, strTmp);
	}

	OnOK();
}
