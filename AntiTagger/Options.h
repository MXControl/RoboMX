#pragma once
#include "afxwin.h"

#include "resource.h"
// COptions dialog

class COptions : public CDialog
{
	DECLARE_DYNAMIC(COptions)

public:
	COptions(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptions();

// Dialog Data
	enum { IDD = IDD_ANTI_TAG };
	CString m_strTag;
	CListBox m_lbTags;
	CString m_strIni;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();


	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedRemove();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
