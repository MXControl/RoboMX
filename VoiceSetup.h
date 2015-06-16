#pragma once
#include "afxwin.h"
#ifdef _ROBO_READER

// CVoiceSetup dialog

class CVoiceSetup : public CDialog
{
	DECLARE_DYNAMIC(CVoiceSetup)

public:
	CVoiceSetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVoiceSetup();

// Dialog Data
	enum { IDD = IDD_VOICE_SETUP };
 	CComboBox m_cbMaleVoice;
	int m_nMaleVoice;
	CComboBox m_cbMaleChar;
	CString m_strAddMale;
	CListBox m_lbMales;
	BOOL m_bDefMale;
	BOOL m_bSpeakOnly;
 	int m_nFemVoice;
	CComboBox m_cbFemChar;
	CComboBox m_cbFemVoice;
	CString m_strFemAdd;
	CListBox m_lbFemales;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedAddMale();
	afx_msg void OnBnClickedRemMale();
	afx_msg void OnBnClickedAddFem();
	afx_msg void OnBnClickedRemFem();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog(void);
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeMaleVoice();
	afx_msg void OnCbnSelchangeFemaleVoice();
};

#endif