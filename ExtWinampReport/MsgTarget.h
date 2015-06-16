#pragma once


// CMsgTarget dialog

class CMsgTarget : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CMsgTarget)

public:
	CMsgTarget(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMsgTarget();
// Overrides
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Dialog Data
	enum { IDD = IDD_MSGDUMMY, IDH = IDR_HTML_MSGTARGET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
};
