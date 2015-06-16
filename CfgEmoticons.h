#pragma once

#include "CfgDlg.h"


// CCfgEmoticons dialog

class CCfgEmoticons : public CCfgDlg
{
	DECLARE_DYNAMIC(CCfgEmoticons)

public:
	CCfgEmoticons(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCfgEmoticons();
	void Save();
	void Load();
	void Apply();
	void WriteEmoticons(void);

// Dialog Data
	enum { IDD = IDD_CFG_EMOTICONS };
	HBITMAP m_hPreview;
	CString m_strActivation;
	CString m_strPath;
	CListCtrl m_lcEmo;
	CStatic m_stPreview;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnDestroy();
	afx_msg void OnBnClickedSelectbmp();
	afx_msg void OnBnClickedAddEmoticon();
	afx_msg void OnBnClickedDeleteEmoticon();
	afx_msg void OnLvnItemchangedEmoticonlist(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedEdit();
};
