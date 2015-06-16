#pragma once


// CAddCache dialog

class CAddCache : public CDialog
{
	DECLARE_DYNAMIC(CAddCache)

public:
	CAddCache(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddCache();

// Dialog Data
	enum { IDD = IDD_ADD_CACHE };
	CString m_strRoom;
	CString m_strTopic;
	CString m_strUsers;
	CString m_strLimit;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strLogin;
};
