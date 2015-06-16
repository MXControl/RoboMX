// VoiceSetup.cpp : implementation file
//

#include "stdafx.h"

#ifdef _ROBO_READER
#include "RoboMX.h"
#include "VoiceSetup.h"
#include "settings.h"
#include <sapi.h>
#include <sphelper.h>
#include <spuihelp.h>
#include "ini.h"
extern CSettings g_sSettings;

// CVoiceSetup dialog

IMPLEMENT_DYNAMIC(CVoiceSetup, CDialog)
CVoiceSetup::CVoiceSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CVoiceSetup::IDD, pParent)
	, m_nMaleVoice(0)
	, m_strAddMale(_T(""))
	, m_nFemVoice(0)
	, m_strFemAdd(_T(""))
	, m_bDefMale(FALSE)
	, m_bSpeakOnly(FALSE)
{
}

CVoiceSetup::~CVoiceSetup()
{
}

void CVoiceSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MALE_VOICE, m_cbMaleVoice);
	DDX_Control(pDX, IDC_MALE_CHAR, m_cbMaleChar);
	DDX_Control(pDX, IDC_MALES, m_lbMales);
	DDX_Control(pDX, IDC_FEMALE_VOICE, m_cbFemVoice);
	DDX_Control(pDX, IDC_FEMALE_CHAR, m_cbFemChar);
	DDX_Control(pDX, IDC_FEMALES, m_lbFemales);
}


BEGIN_MESSAGE_MAP(CVoiceSetup, CDialog)
	ON_BN_CLICKED(IDC_ADD_MALE, OnBnClickedAddMale)
	ON_BN_CLICKED(IDC_REM_MALE, OnBnClickedRemMale)
	ON_BN_CLICKED(IDC_ADD_FEM, OnBnClickedAddFem)
	ON_BN_CLICKED(IDC_REM_FEM, OnBnClickedRemFem)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_MALE_VOICE, OnCbnSelchangeMaleVoice)
	ON_CBN_SELCHANGE(IDC_FEMALE_VOICE, OnCbnSelchangeFemaleVoice)
END_MESSAGE_MAP()

BOOL CVoiceSetup::OnInitDialog(void)
{
	
	CDialog::OnInitDialog();
	
	HRESULT                             hr = S_OK;

	/*hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);
	
	if(SUCCEEDED(hr)){

		hr = cpEnum->GetCount(&ulCount);

	}

	while(SUCCEEDED(hr) && ulCount -- ){
    
		cpVoiceToken.Release();
		if(SUCCEEDED(hr))
			hr = cpEnum->Next( 1, &cpVoiceToken, NULL );
		if(SUCCEEDED(hr))
	        hr = cpVoice->SetVoice(cpVoiceToken);
	    if(SUCCEEDED(hr))
			hr = cpVoice->Speak( L"How are you?", SPF_DEFAULT, NULL);

		m_cbMaleVoice.AddString(
	}  */


	hr = SpInitTokenComboBox(GetDlgItem(IDC_MALE_VOICE)->m_hWnd, SPCAT_VOICES);
	if(FAILED(hr)){

		AfxMessageBox("Error enumerating voices", MB_ICONSTOP);
	}

	hr = SpInitTokenComboBox(GetDlgItem(IDC_FEMALE_VOICE)->m_hWnd, SPCAT_VOICES);
	if(FAILED(hr)){

		AfxMessageBox("Error enumerating voices", MB_ICONSTOP);
	}


	CFileFind finder;

	BOOL bFound = finder.FindFile(g_sSettings.GetWorkingDir() + "\\gfx\\char_*.bmp");
	CString strChar, strTmp;
	while(finder.FindNextFile()){

		strChar = finder.GetFileTitle();
		strChar = strChar.Mid(5);
		m_cbMaleChar.AddString(strChar);
		m_cbFemChar.AddString(strChar);
	}


	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	int nCnt = ini.GetValue("Voice", "MaleCount", 0);
	for(int i = 0; i < nCnt; i++){

		strTmp.Format("Male_%d", i);
		m_lbMales.AddString(ini.GetValue("Voice", strTmp, ""));
	}

	nCnt = ini.GetValue("Voice", "FemaleCount", 0);
	for(int i = 0; i < nCnt; i++){

		strTmp.Format("Female_%d", i);
		m_lbFemales.AddString(ini.GetValue("Voice", strTmp, ""));
	}



	return TRUE;
}

void CVoiceSetup::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

// CVoiceSetup message handlers
void CVoiceSetup::OnBnClickedAddMale()
{
	// TODO: Add your control notification handler code here
}

void CVoiceSetup::OnBnClickedRemMale()
{
	// TODO: Add your control notification handler code here
}

void CVoiceSetup::OnBnClickedAddFem()
{
	// TODO: Add your control notification handler code here
}

void CVoiceSetup::OnBnClickedRemFem()
{
	// TODO: Add your control notification handler code here
}

void CVoiceSetup::OnCbnSelchangeMaleVoice()
{
	// TODO: Add your control notification handler code here
}

void CVoiceSetup::OnCbnSelchangeFemaleVoice()
{
	// TODO: Add your control notification handler code here
}

#endif
