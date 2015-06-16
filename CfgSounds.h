#pragma once


// CCfgSounds dialog
#include "CfgDlg.h"

class CCfgSounds : public CCfgDlg
{
	DECLARE_DYNAMIC(CCfgSounds)

public:
	CCfgSounds(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCfgSounds();
	void WriteSounds();
	void LoadSounds();
	void Apply();

// Dialog Data
	enum { IDD = IDD_CFG_CHATSOUNDS };
	CListCtrl	m_lcSounds;
	BOOL	m_bChatSfx;
	CString m_strTrigger;
	CString m_strSound;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnAdd();
	afx_msg void OnDelete();
	afx_msg void OnEdit();
	afx_msg void OnPlay();
	afx_msg void OnBnClickedBrowseSound();
	DECLARE_MESSAGE_MAP()
};
