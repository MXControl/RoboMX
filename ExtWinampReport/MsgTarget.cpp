// MsgTarget.cpp : implementation file
//

#include "stdafx.h"
#include "ExtWinampReport.h"
#include "MsgTarget.h"


// CMsgTarget dialog

IMPLEMENT_DYNCREATE(CMsgTarget, CDHtmlDialog)

CMsgTarget::CMsgTarget(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CMsgTarget::IDD, CMsgTarget::IDH, pParent)
{
}

CMsgTarget::~CMsgTarget()
{
}

void CMsgTarget::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CMsgTarget::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CMsgTarget, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CMsgTarget)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CMsgTarget message handlers

HRESULT CMsgTarget::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;  // return TRUE  unless you set the focus to a control
}

HRESULT CMsgTarget::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;  // return TRUE  unless you set the focus to a control
}
