#pragma once
#include "CfgDlg.h"


// CCfgWelcome dialog

class CCfgWelcome : public CCfgDlg
{

public:
	CCfgWelcome(CWnd* pParent = NULL);   // standard constructor
	void Apply();
 	void SaveRooms();
	void LoadRooms();

// Dialog Data
	enum { IDD = IDD_CFG_WELCOME };
 	CListBox	m_lbChannels;
	CListBox	m_lbGreetings;
	BOOL	m_bAllChannels;
	CString	m_strChannel;
	CString	m_strGreeting;

protected:
	CPPToolTip	m_mToolTip;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage( MSG* pMsg );
 	virtual BOOL OnInitDialog();
	afx_msg void OnAllchannels();
	afx_msg void OnAddchannel();
	afx_msg void OnRemovechannel();
	afx_msg void OnBnClickedAddchannel2();
	afx_msg void OnBnClickedRemovechannel2();
	afx_msg void OnEnChangeGreeting();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedEditroom();
	afx_msg void OnBnClickedEditwelcome();
};
