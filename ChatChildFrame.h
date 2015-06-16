#pragma once

#include <Winsock2.h>
#pragma comment(lib, "WS2_32")

#include "ChatClient.h"
#include "RichDocument.h"
#include "RichEditExCtrl.h"
#include "MyListCtrl.h"
#include "Metis3View.h"

// CChatChildFrame frame

class CChatChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChatChildFrame)

public:
	CExtControlBar    m_wndResizableBar0;
	CExtControlBar    m_wndResizableBar1;
	//CExtControlBar    m_wndResizableBar2;
	
	CRichDocument	m_docSystem;
	CRichEditExCtrl m_wndSys;
	CMyListCtrl     m_wndUsers;

	CString m_strProfileName;
	HICON m_hChildFrameIcon;

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();

public:
	// view for the client area of the frame.
	//CMetis3View m_wndView;
	CChatChildFrame();           // protected constructor used by dynamic creation
	virtual ~CChatChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	afx_msg void OnFileClose();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnNcPaint();
	afx_msg void OnUpdateControlBarMenu(CCmdUI* pCmdUI);
	afx_msg BOOL OnBarCheck(UINT nID);
	DECLARE_MESSAGE_MAP()
};


