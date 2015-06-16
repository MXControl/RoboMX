#pragma once
#include "afxwin.h"


// CLogView dialog

class CLogView : public CDialog
{
	DECLARE_DYNAMIC(CLogView)

public:
	CLogView(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLogView();

// Dialog Data
	enum { IDD = IDD_LOGVIEW };
	CButton m_btOK;
	CListBox m_lcDates;
	CString m_strRoom;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeDates();
 	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnDblclkDates();
	static CString GetDateString(CString strDate);
};
