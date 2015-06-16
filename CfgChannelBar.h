#pragma once

#include "CfgDlg.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "colorstatic.h"
// CCfgChannelBar dialog

class CCfgChannelBar : public CCfgDlg
{

public:
	CCfgChannelBar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCfgChannelBar();

// Dialog Data
	enum { IDD = IDD_CFG_CHANNELBAR };
	CString m_strToolHtml;
	BOOL m_bUseOld;
	int m_nEffect;
	int m_nType;
	int m_nGran;
	int m_nTrans;
	CString m_strChannel;
	CString m_strImage;
	CComboBox m_cbType;
	CComboBox m_cbEffect;
	CColorStatic m_stBg1;
	CColorStatic m_stBg2;
	CColorStatic m_stBg3;
	CListCtrl m_lcChannelImages;
	void Apply();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedHelp();
	afx_msg void OnBnClickedVars();
	afx_msg void OnBnClickedUseold();
	afx_msg void OnBnClickedEdit();
	afx_msg void OnBnClickedBg1();
	afx_msg void OnBnClickedBg2();
	afx_msg void OnBnClickedBg3();
	afx_msg void OnBnClickedBrowse();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedRemove();
	afx_msg void OnCbnSelchangeTooltiptype();
	DECLARE_MESSAGE_MAP()
public:
};
