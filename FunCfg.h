#if !defined(AFX_FUNCFG_H__AEA2D9C4_C7EC_4801_A5C8_3C77D313BC84__INCLUDED_)
#define AFX_FUNCFG_H__AEA2D9C4_C7EC_4801_A5C8_3C77D313BC84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FunCfg.h : header file
//

#include "CfgDlg.h"
#include "afxcmn.h"
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// CCfgFun dialog

class CCfgFun : public CCfgDlg
{
// Construction
public:
	void Save();
	void Load();
	void Apply();
	CCfgFun(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCfgFun)
	enum { IDD = IDD_CFG_FUN };
	CString	m_strVendor;
	CString	m_strSuff;
	CString	m_strName;
	//}}AFX_DATA
	
	int m_nAct;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCfgFun)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCfgFun)
	afx_msg void OnBtnVendorAdd();
	afx_msg void OnBtnNameAdd();
	afx_msg void OnBtnSuffixAdd();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lcVendors;
	CListCtrl m_lcNames;
	CListCtrl m_lcSuff;
	afx_msg void OnNMDblclkVendors(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkNames(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkSuffixes(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnVendorRem();
	afx_msg void OnBnClickedBtnNameRem();
	afx_msg void OnBnClickedBtnSuffixRem();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FUNCFG_H__AEA2D9C4_C7EC_4801_A5C8_3C77D313BC84__INCLUDED_)
