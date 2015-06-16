#include "stdafx.h"
#include ".\myprogressctrl.h"

CMyProgressCtrl::CMyProgressCtrl(void) :
	m_nStart(0),
	m_nEnd(100),
	m_nPos(0)

{
}

CMyProgressCtrl::~CMyProgressCtrl(void)
{
}

BEGIN_MESSAGE_MAP(CMyProgressCtrl, CStatic)
	//{{AFX_MSG_MAP(CMyProgressCtrl)
	ON_WM_PAINT()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMyProgressCtrl::SetRange(UINT nStart, UINT nEnd)
{

	ASSERT(nStart < nEnd);

	m_nStart = nStart;
	m_nEnd   = nEnd;

	Invalidate();
}

void CMyProgressCtrl::SetPos(UINT nPos)
{

	m_nPos = nPos;
	Invalidate();
}

void CMyProgressCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	

	CRect rc;
	GetWindowRect(rc);

	float fPos = ((float)(m_nPos-m_nStart) / (float)(m_nEnd-m_nStart))*100.f;

	int nWidth  = (int)((float)rc.Width() / 100.f * fPos);
	if(nWidth < 6) nWidth=6;
	int nHeight = rc.Height();

	CRect backgroundRect;
	backgroundRect.top		= 0;
	backgroundRect.bottom	= rc.Height();
	backgroundRect.left		= 0;
	backgroundRect.right	= rc.Width();

	// Fill window rect with background color
	HBRUSH hBackground = ::CreateSolidBrush(RGB(0, 0, 0));
	HBRUSH hForeground = ::CreateSolidBrush(RGB(255, 200, 40));
	
	::FillRect(dc.m_hDC, &backgroundRect, hBackground);
	
	backgroundRect.DeflateRect(1, 1, 1, 1);
    
	CRect statusRect;
	statusRect.top    = 3;
	statusRect.bottom = nHeight-3;
	statusRect.left   = 3;
	statusRect.right  = nWidth-3;

	::FillRect(dc.m_hDC, &statusRect, hForeground);

	dc.Draw3dRect(backgroundRect, RGB(255, 200, 40), RGB(255, 200, 40));

	::DeleteObject(hBackground);
	::DeleteObject(hForeground);
}

void CMyProgressCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CStatic::OnSize(nType, cx, cy);
	
	Invalidate();	
}
