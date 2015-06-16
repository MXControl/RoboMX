#pragma once
#include "CfgDlg.h"


// CCfgFormat dialog

class CCfgFormat : public CCfgDlg
{

public:
	CCfgFormat(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCfgFormat();
	void Apply();

// Dialog Data
	enum { IDD = IDD_CFG_FORMAT };
	CComboBox		m_cbFont;
	CComboBox		m_cbEncoding;
	int		m_nEncoding;
	int		m_nSize;
	CString m_strFont;
	BOOL	m_bBold;
	BOOL	m_bItalic;
	BOOL	m_bLine;
	BOOL	m_bImage;
	CString	m_strImage;
	BOOL	m_bEmoticons;
	BOOL	m_bMaxi;
	int		m_nBarTop;
	int		m_nTime;
	BOOL	m_bMiniTray;
	BOOL    m_bHideSystem;
	BOOL	m_bSavePos;
	BOOL	m_bPMToolTip;
	CButton m_btMaxi;
	CButton m_btWinPos;
	BOOL	m_bTimeStamp;

protected:
	friend BOOL CALLBACK EnumFontsProc(LPLOGFONT lpFont, LPTEXTMETRIC lpMetric, DWORD dwType, LPARAM lpData);
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnBnClickedSelectLogDir();
	afx_msg void OnBnClickedMdimaxi();
	afx_msg void OnBnClickedRememberPos();

	afx_msg void OnSelectImage();
	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bSep;
	BOOL m_bShowFillStatus;
};
