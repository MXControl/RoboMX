#pragma once


// CCfgQuickMsg dialog
#include "CfgDlg.h"

class CCfgQuickMsg : public CCfgDlg
{

public:
	CCfgQuickMsg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCfgQuickMsg();
	void SaveQuickCmds();
	void LoadQuickCmds();
	void Apply();

// Dialog Data
	enum { IDD = IDD_CFG_QUICKMSG };
	CListBox	m_lbQuick;
	CString	m_strAdd;
	//CMFECToolTip	m_mToolTip;
	CPPToolTip m_mToolTip;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage( MSG* pMsg );
	virtual BOOL OnInitDialog();
	afx_msg void OnMsgAddBtn();
	afx_msg void OnQuickrem();
	afx_msg void OnQuickup();
	afx_msg void OnQuickdown();
	afx_msg void OnEnChangeMsgAdd();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedEdit();
};
