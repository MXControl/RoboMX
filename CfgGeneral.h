#pragma once

#include "CfgDlg.h"

// CCfgGeneral dialog

class CCfgGeneral : public CCfgDlg
{

public:
	CCfgGeneral(CWnd* pParent = NULL);   // standard constructor
	void Apply(void);

// Dialog Data
	enum { IDD = IDD_CFG_GENERAL };
	BOOL	m_bUpdate;
	BOOL	m_bScroller;
	BOOL	m_bHistory;
	BOOL    m_bAutoList;
	int		m_nHistory;
	BOOL	m_bPing;
	CString m_strPath;
	BOOL    m_bLog;
	BOOL	m_bRetry;
	BOOL	m_bAutoWPN;
	int		m_nRetries;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedRetryJoins();
	afx_msg void OnBnClickedEnableHistory();
	afx_msg void OnBnClickedOpenfolder();
	afx_msg void OnBnClickedConnectWpn();
	afx_msg void OnBnClickedSelectLogDir();
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bLoadLog;
};
