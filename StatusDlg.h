#pragma once
#include "afxcmn.h"


// CStatusDlg dialog
#define WM_SETSTATUS WM_USER+1

class CStatusDlg : public CDialog
{
	DECLARE_DYNAMIC(CStatusDlg)

public:
	CStatusDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStatusDlg();

// Dialog Data
	enum { IDD = IDD_STATUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg LRESULT OnSetStatus(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_pgStatus;
	CString m_strOperation;
	void Center(CWnd* pParent);
};
