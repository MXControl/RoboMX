#pragma once
#include "afxwin.h"


// CCfgAddPeerCacheDlg dialog

class CCfgAddPeerCacheDlg : public CDialog
{
	DECLARE_DYNAMIC(CCfgAddPeerCacheDlg)

public:
	CCfgAddPeerCacheDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCfgAddPeerCacheDlg();

// Dialog Data
	enum { IDD = IDD_CFG_PEERCACHE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedOk();

	DECLARE_MESSAGE_MAP()
public:
	CString m_strServer;
	CListBox m_wndServers;
	BOOL OnInitDialog(void);
};
