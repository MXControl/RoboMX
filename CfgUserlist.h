#pragma once

#include "CfgDlg.h"
#include "afxwin.h"
// CCfgUserlist dialog

class CCfgUserlist : public CCfgDlg
{

public:
	CCfgUserlist(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCfgUserlist();
	void Apply();

	BOOL	m_bHiliteUsers;
	BOOL	m_bOpTop;
	BOOL	m_bUseBg;
	CStatic m_stNormal;
	CStatic m_stVoiced;
	CStatic m_stAdmin;
	CStatic m_stHost;
	CButton m_btCompact;
	CButton m_btDetailed;
	CButton m_btBigIcon;
	CButton m_btSmallIcon;
	int m_nUserListMode;
	CString m_strImage;
	HBITMAP m_hNormal;
	HBITMAP m_hAdmin;
	HBITMAP m_hVoiced;
	HBITMAP m_hHost;
	CString m_strNormal;
	CString m_strAdmin;
	CString m_strVoiced;
	CString m_strHost;
	BOOL m_bTile;
	int m_nXOffset;
	int m_nYOffset;

// Dialog Data
	enum { IDD = IDD_CFG_USERLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedDetailed();
	afx_msg void OnBnClickedCompact();
	afx_msg void OnBnClickedPath();
	afx_msg void OnBnClickedNormal();
	afx_msg void OnBnClickedVoiced();
	afx_msg void OnBnClickedAdmin();
	afx_msg void OnBnClickedHost();
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBigicon();
	afx_msg void OnBnClickedSmallicon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSelectAvatar();
	CString m_strAvatar;
	afx_msg void OnBnClickedSelectImage3();
	CString m_strDefChannel;
};
