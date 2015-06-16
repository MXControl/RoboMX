#pragma once
#include "CfgDlg.h"
#include "afxcmn.h"

// CCfgAway dialog

class CCfgAway : public CCfgDlg
{

public:
	CCfgAway(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCfgAway();
	void Apply();

// Dialog Data
	enum { IDD = IDD_CFG_AWAY };
	BOOL m_bAwayIfIdle;
	DWORD m_dwIdleDuration;
	CString m_strAutoReason;
	CString m_strAutoPrefix;
	CString m_strAutoSuffix;
	CString m_strLabel;
	CString m_strReason;
	CString m_strPrefix;
	CString m_strSuffix;
	CListCtrl m_lcAway;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedEdit();
	afx_msg void OnBnClickedDelete();
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bAutoBack;
};
