#if !defined(AFX_COLORSTATUSBAR_H__7C404582_D184_4405_ADAD_8A1F1AACA3A5__INCLUDED_)
#define AFX_COLORSTATUSBAR_H__7C404582_D184_4405_ADAD_8A1F1AACA3A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorStatusBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorStatusBar window
#define WPN_STATUS_DISCONNECTED 0
#define WPN_STATUS_CONNECTED	1
#define WPN_STATUS_CONNECTING	2

class CColorStatusBar : public CStatusBar
{
// Construction
public:
	CColorStatusBar();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorStatusBar)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetPaneText(DWORD dwIndex, CString strText, COLORREF cr = 0, COLORREF bg = COLORREF(-1));
	//void SetLagColor(COLORREF cr, CString strLag);
	void SetWPNStatus(int nStatus){ 
		
		m_nWPN = nStatus; 
		Invalidate();
	}

	virtual ~CColorStatusBar();

protected:
	COLORREF m_crLag;
	COLORREF m_crBg;
	CString  m_strLag;
	int		 m_nWPN;
	DWORD    m_dwConnecting;
	WORD     m_wAni;

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorStatusBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSTATUSBAR_H__7C404582_D184_4405_ADAD_8A1F1AACA3A5__INCLUDED_)
