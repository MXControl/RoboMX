// ColorStatusBar.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "ColorStatusBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorStatusBar

CColorStatusBar::CColorStatusBar()
{
	m_nWPN = 0;
	m_strLag = "";
	m_dwConnecting = 0;
	m_wAni = 0;
}

CColorStatusBar::~CColorStatusBar()
{
}


BEGIN_MESSAGE_MAP(CColorStatusBar, CStatusBar)
	//{{AFX_MSG_MAP(CColorStatusBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorStatusBar message handlers

void CColorStatusBar::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	if(lpDrawItemStruct->itemID == 0){

		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);
		dc.SetBkMode(OPAQUE);

		CRect rect(&lpDrawItemStruct->rcItem);
		CString strText;
		COLORREF crText = 0, crBg = 0;

		switch(m_nWPN){

			case WPN_STATUS_DISCONNECTED:
				strText.LoadString(IDS_WPN_DISCONNECTED);
				crText   = RGB(255,255,255);
				crBg	 = RGB(200, 0, 0);
				break;
			case WPN_STATUS_CONNECTED:
				strText.LoadString(IDS_WPN_CONNECTED);
				crText   = RGB(255,255,255);
				crBg	 = RGB(0,200,0);
				break;
			case WPN_STATUS_CONNECTING:
				strText.LoadString(IDS_WPN_CONNECTING);
				if(m_wAni == 0){

					strText += " ..";
				}
				else if(m_wAni == 1){

					strText += ". .";
				}
				else{

					strText += ".. ";
				}
				if(GetTickCount() - m_dwConnecting > 1000){

					m_dwConnecting = GetTickCount();
					m_wAni++;
					if(m_wAni > 2) m_wAni = 0;
				}
				crText   = 0;
				crBg	 = RGB(255,200,0);
				break;
			default:
				ASSERT(FALSE);
		}

		dc.SetBkColor(crBg);
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, crBg);
		dc.SetTextColor(crText);
		dc.TextOut(rect.left+2, rect.top, strText);

		dc.Detach();
	}
	else if(lpDrawItemStruct->itemID == 3){

		CDC dc;
		dc.Attach(lpDrawItemStruct->hDC);

		dc.SetBkMode(TRANSPARENT);

		// Get the pane rectangle and calculate text coordinates
		CRect rect(&lpDrawItemStruct->rcItem);

		if(m_crBg != COLORREF(-1)){
		
			dc.SetBkColor(m_crBg);
			dc.FillSolidRect(&lpDrawItemStruct->rcItem, m_crBg);
		}
		dc.SetTextColor(m_crLag);
		dc.TextOut(rect.left+2, rect.top, m_strLag);

		dc.Detach();
	}
}

/*void CColorStatusBar::SetLagColor(COLORREF cr, CString strLag)
{

	m_strLag = strLag;
	m_crLag  = cr;
}*/

void CColorStatusBar::SetPaneText(DWORD dwIndex, CString strText, COLORREF cr, COLORREF bg)
{

	if(!::IsWindow(m_hWnd)) return;
	CStatusBar::SetPaneText(dwIndex, strText, TRUE);

	if(dwIndex == 0){

		GetStatusBarCtrl().SetText(strText, 0, SBT_OWNERDRAW); 
	}
	else if(dwIndex == 3){

		GetStatusBarCtrl().SetText(strText, 3, SBT_OWNERDRAW); 
		m_crLag = cr;
		m_crBg   = bg;
		m_strLag = strText;
	}
	Invalidate();
}
