// CfgSounds.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "CfgSounds.h"
#include "Ini.h"
#include "Settings.h"
#include "Tokenizer.h"
#include <mmsystem.h>
#include ".\cfgsounds.h"


// CCfgSounds dialog
extern CSettings g_sSettings;

IMPLEMENT_DYNAMIC(CCfgSounds, CCfgDlg)
CCfgSounds::CCfgSounds(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CCfgSounds::IDD, pParent)
	, m_bChatSfx(FALSE)
	, m_strTrigger(_T(""))
	, m_strSound(_T(""))
{
}

CCfgSounds::~CCfgSounds()
{
}

void CCfgSounds::DoDataExchange(CDataExchange* pDX)
{
	CCfgDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SOUNDLIST, m_lcSounds);
	DDX_Check(pDX, IDC_ENABLE_CSFX, m_bChatSfx);
	DDX_Text(pDX, IDC_TRIGGER, m_strTrigger);
	DDX_Text(pDX, IDC_REACTION, m_strSound);
}


BEGIN_MESSAGE_MAP(CCfgSounds, CCfgDlg)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_BN_CLICKED(IDC_PLAY, OnPlay)
	ON_BN_CLICKED(IDC_BROWSE_SOUND, OnBnClickedBrowseSound)
END_MESSAGE_MAP()


// CCfgSounds message handlers

BOOL CCfgSounds::OnInitDialog() 
{

	CCfgDlg::OnInitDialog();
	
	CString strTitle;
	strTitle.LoadString(IDS_TRIGGER);
	m_lcSounds.InsertColumn(0, strTitle, LVCFMT_LEFT, 150);
	strTitle.LoadString(IDS_SOUND);
	m_lcSounds.InsertColumn(1, strTitle, LVCFMT_LEFT, 250);

	m_lcSounds.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	LoadSounds();
	m_bChatSfx = g_sSettings.GetSfxChatSfx();

	UpdateData(FALSE);

	return TRUE;  
}

void CCfgSounds::Apply()
{

	UpdateData(TRUE);

	g_sSettings.SetSfxChatSfx(m_bChatSfx);
	WriteSounds();
	LoadSounds();
}

void CCfgSounds::LoadSounds()
{

	m_lcSounds.DeleteAllItems();
	g_sSettings.LoadSounds();
	int p = 0;
	for(int i = 0; i < g_sSettings.m_aSounds.GetSize(); i++){


		p = m_lcSounds.InsertItem(m_lcSounds.GetItemCount(), g_sSettings.m_aSounds[i].strTrigger, 0);
		m_lcSounds.SetItemText(p, 1, g_sSettings.m_aSounds[i].strSound);
	}
}

void CCfgSounds::WriteSounds()
{

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	CString strTmp;
	ini.SetValue("ChatSounds", "Count", m_lcSounds.GetItemCount());

	for(int i = 0; i < m_lcSounds.GetItemCount(); i++){

		SOUND s;
		strTmp.Format("Trigger_%d", i);
		
		ini.SetValue("ChatSounds", strTmp, m_lcSounds.GetItemText(i, 0));
		
		strTmp.Format("Sound_%d", i);
		ini.SetValue("ChatSounds", strTmp, m_lcSounds.GetItemText(i, 1));
	}
}

void CCfgSounds::OnAdd() 
{

	if(!UpdateData(TRUE)) return;
	if(m_strTrigger.IsEmpty() || m_strSound.IsEmpty()){

		AfxMessageBox(IDS_MISSING_INPUT, MB_OK+MB_ICONINFORMATION);
		return;
	}

	LVITEM iItem = {0};
	iItem.mask = LVIF_IMAGE | LVIF_TEXT;
	iItem.iItem = m_lcSounds.GetItemCount();
	iItem.mask = LVIF_TEXT | LVIF_IMAGE;
	iItem.pszText = (LPTSTR)(LPCSTR)m_strTrigger;
	iItem.iImage = 0;
	int nPos = m_lcSounds.InsertItem(&iItem);
	m_lcSounds.SetItemText(nPos, 1, m_strSound);

	m_strTrigger.Empty();
	m_strSound.Empty();
	UpdateData(FALSE);

}

void CCfgSounds::OnDelete() 
{

	if(m_lcSounds.GetSelectedCount() == 0){

		AfxMessageBox(IDS_ERROR_SELECTEVENT, MB_OK+MB_ICONINFORMATION);
		return;
	}
	LVITEM iItem = {0};
	iItem.mask = LVIF_IMAGE | LVIF_TEXT;
	iItem.iItem = m_lcSounds.GetSelectionMark();
	m_lcSounds.GetItem(&iItem);

	m_lcSounds.DeleteItem(m_lcSounds.GetSelectionMark());
}

void CCfgSounds::OnEdit() 
{

	if(m_lcSounds.GetSelectedCount() == 0){

		AfxMessageBox(IDS_ERROR_SELECTEVENT, MB_OK+MB_ICONINFORMATION);
		return;
	}

    int n = m_lcSounds.GetSelectionMark();
	m_strTrigger = m_lcSounds.GetItemText(n, 0);
	m_strSound   = m_lcSounds.GetItemText(n, 1);
	m_lcSounds.DeleteItem(n);
	UpdateData(FALSE);
	/*CSfxEdit dlg;
	dlg.Init(
			   iItem.iImage, 
			   m_lcSounds.GetItemText(iItem.iItem, 0), 
			   m_lcSounds.GetItemText(iItem.iItem, 1)
		   );

	if(dlg.DoModal() == IDOK){
		
		int nEvent;
		CString strTrigger, strReaction;
		dlg.GetValues(nEvent, strTrigger, strReaction);
		iItem.mask = LVIF_TEXT | LVIF_IMAGE;
		iItem.pszText = (LPTSTR)(LPCSTR)strTrigger;
		iItem.iImage = nEvent;
		m_lcSounds.SetItem(&iItem);
		m_lcSounds.SetItemText(iItem.iItem, 1, strReaction);
	}*/
}

void CCfgSounds::OnPlay() 
{

	if(m_lcSounds.GetSelectedCount() == 0){

		AfxMessageBox(IDS_ERROR_SELECTEVENT, MB_OK+MB_ICONINFORMATION);
		return;
	}

	LVITEM iItem = {0};
	iItem.mask = LVIF_IMAGE | LVIF_TEXT;
	iItem.iItem = m_lcSounds.GetSelectionMark();
	m_lcSounds.GetItem(&iItem);
	if(!PlaySound(m_lcSounds.GetItemText(iItem.iItem, 1), NULL, SND_FILENAME|SND_ASYNC)){

		CString strError;
		strError.Format(IDS_ERROR_PLAYWAVE,	m_lcSounds.GetItemText(iItem.iItem, 1));
		AfxMessageBox(strError, MB_ICONSTOP);
	}
}

void CCfgSounds::OnBnClickedBrowseSound()
{
	UpdateData(TRUE);

	CString strFilter;
	strFilter.LoadString(IDS_WAVE_FILTER);

	CFileDialog dlg(TRUE, ".wav", m_strSound, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter, this);
	
	if(dlg.DoModal() == IDOK){

		m_strSound = dlg.GetPathName();
		UpdateData(FALSE);
	}
}
