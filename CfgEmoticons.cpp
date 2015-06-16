// CfgEmoticons.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "CfgEmoticons.h"
#include "Ini.h"
#include "Settings.h"
#include "EmoticonManager.h"
#include ".\cfgemoticons.h"

extern CSettings g_sSettings;
extern CEmoticonManager g_eEmoticons;


// CCfgEmoticons dialog

IMPLEMENT_DYNAMIC(CCfgEmoticons, CCfgDlg)
CCfgEmoticons::CCfgEmoticons(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CCfgEmoticons::IDD, pParent)
	, m_strActivation(_T(""))
	, m_strPath(_T(""))
	, m_hPreview(NULL)
{

}

CCfgEmoticons::~CCfgEmoticons()
{
}

void CCfgEmoticons::DoDataExchange(CDataExchange* pDX)
{
	CCfgDlg::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ACTTEXT, m_strActivation);
	DDX_Text(pDX, IDC_BMP, m_strPath);
	DDX_Control(pDX, IDC_EMOTICONLIST, m_lcEmo);
	DDX_Control(pDX, IDC_PREVIEW, m_stPreview);
}


BEGIN_MESSAGE_MAP(CCfgEmoticons, CCfgDlg)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SELECTBMP, OnBnClickedSelectbmp)
	ON_BN_CLICKED(IDC_ADD_EMOTICON, OnBnClickedAddEmoticon)
	ON_BN_CLICKED(IDC_DELETE_EMOTICON, OnBnClickedDeleteEmoticon)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_EMOTICONLIST, OnLvnItemchangedEmoticonlist)
	ON_BN_CLICKED(IDC_EDIT, OnBnClickedEdit)
END_MESSAGE_MAP()

void CCfgEmoticons::OnDestroy()
{

	CCfgDlg::OnDestroy();

	if(m_hPreview != NULL){

		DeleteObject(m_hPreview);
	}
}


BOOL CCfgEmoticons::OnInitDialog() 
{
	CCfgDlg::OnInitDialog();


	CString strTitle;
	strTitle.LoadString(IDS_EMO_ACT);
	m_lcEmo.InsertColumn(0, strTitle, LVCFMT_LEFT, 60);
	strTitle.LoadString(IDS_EMO_FILE);
	m_lcEmo.InsertColumn(1, strTitle, LVCFMT_LEFT, 350);
	ListView_SetExtendedListViewStyle(m_lcEmo.m_hWnd, LVS_EX_FULLROWSELECT);

	Load();

	int nIn = 0;
	for(int i = 0; i < g_eEmoticons.m_aEmoticons.GetSize(); i++){

		nIn = m_lcEmo.InsertItem(m_lcEmo.GetItemCount(), g_eEmoticons.m_aEmoticons[i]->szActivationText);
		m_lcEmo.SetItemText(nIn, 1, g_eEmoticons.m_aEmoticons[i]->szFileName);
	}

	UpdateData(FALSE);

	return TRUE;
}


void CCfgEmoticons::Apply()
{

	Save();
	WriteEmoticons();
	g_eEmoticons.Free();
	g_eEmoticons.Load();
	UpdateData(TRUE);
}

void CCfgEmoticons::Load()
{

}

void CCfgEmoticons::Save()
{

}

void CCfgEmoticons::OnBnClickedSelectbmp()
{

	UpdateData(TRUE);

	CString strFilter;
	strFilter.LoadString(IDS_BITMAP_FILTER);

	CFileDialog dlg(TRUE, ".bmp", m_strPath, OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		if(dlg.GetPathName() != m_strPath){

			m_strPath = dlg.GetPathName();
			UpdateData(FALSE);
		}
	}
}

void CCfgEmoticons::OnBnClickedAddEmoticon()
{

	UpdateData(TRUE);
	if(m_strPath.IsEmpty() || m_strActivation.IsEmpty()){

		AfxMessageBox(IDS_ERROR_MISSING_INPUT, MB_ICONINFORMATION);
		return;
	}
	int nPos = m_lcEmo.InsertItem(m_lcEmo.GetItemCount(), m_strActivation);
	m_lcEmo.SetItemText(nPos, 1, m_strPath);
	m_strActivation.Empty();
	m_strPath.Empty();
	UpdateData(FALSE);
}


void CCfgEmoticons::OnBnClickedEdit()
{

	int nSel = m_lcEmo.GetSelectionMark();
	if(nSel >= 0){

		m_strActivation = m_lcEmo.GetItemText(nSel, 0);
		m_strPath		= m_lcEmo.GetItemText(nSel, 1);
		UpdateData(FALSE);
		m_lcEmo.DeleteItem(nSel);
	}
	else{

		AfxMessageBox(IDS_ERROR_NO_SELECTION, MB_ICONSTOP);
	}
}

void CCfgEmoticons::OnBnClickedDeleteEmoticon()
{

	int nSel = m_lcEmo.GetSelectionMark();
	if(nSel >= 0){

		m_lcEmo.DeleteItem(nSel);
	}
	else{

		AfxMessageBox(IDS_ERROR_NO_SELECTION, MB_ICONSTOP);
	}
}

void CCfgEmoticons::WriteEmoticons(void)
{

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	CString strTmp;
	ini.SetValue("Emoticons", "Count", m_lcEmo.GetItemCount());

	for(int i = 0; i < m_lcEmo.GetItemCount(); i++){

		SOUND s;
		strTmp.Format("Text_%d", i);
		
		ini.SetValue("Emoticons", strTmp, m_lcEmo.GetItemText(i, 0));
		
		strTmp.Format("Pic_%d", i);
		ini.SetValue("Emoticons", strTmp, m_lcEmo.GetItemText(i, 1));
	}
}

void CCfgEmoticons::OnLvnItemchangedEmoticonlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	int n = m_lcEmo.GetNextItem(-1, LVNI_SELECTED);
	
	if(n >= 0){

        CString strIcon = m_lcEmo.GetItemText(n, 1);

		if(m_hPreview != NULL){

			DeleteObject(m_hPreview);
		}

		HBITMAP hTmp = (HBITMAP)::LoadImage(NULL, strIcon, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADTRANSPARENT|LR_LOADFROMFILE);
		if(hTmp){

			m_hPreview = CEmoticonManager::ReplaceColor(hTmp, RGB(255, 0, 255), GetSysColor(COLOR_3DFACE), 0);
			DeleteObject(hTmp);
		}
		m_stPreview.SetBitmap(m_hPreview);
	}
	*pResult = 0;
}

