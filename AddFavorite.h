#pragma once
#include "afxwin.h"


// CAddFavorite dialog

class CAddFavorite : public CDialog
{
	DECLARE_DYNAMIC(CAddFavorite)

public:
	CAddFavorite(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddFavorite();

// Dialog Data
	enum { IDD = IDD_ADDFAV };
	CString m_strChannel;
	CString m_strLogin;
	CComboBox m_cbNetwork;
	BOOL m_bAutoJoin;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog(void);

	afx_msg void OnBnClickedOk();
	DECLARE_MESSAGE_MAP()
public:
};
