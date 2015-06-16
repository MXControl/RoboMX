// SwitcherButton.cpp
//
// Programmed by: JIMMY BRUSH (Kathy007@email.msn.com)
// 
// Legal:
//
// THIS CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND.
// You may use and distribute this code provided that you do not
// remove this title header and that you do not charge money for
// it. If you want to update the code, feel free to do so, as long
// as you *mark your changes* in code AND in the revision log below
// (and send it to me ;)
//
//
//
// Version: 1.0 revision 1
//
// Revision Log:
//
// SUN MAR 14 1999 - JIMMY BRUSH -  Finished Writing version 1.0
// MON MAR 15 1999 - JIMMY BRUSH -  Fixed RemoveButton to correctly decrement selected button
//									Added CMemDC by Keith Rule
//									Fixed up Animation a bit
//
//
//
// In the next version \ Wish List:
//
// 1. Tool Tips in CSwitcherButton
// 2. Support for more buttons than can display (with the up/down button at the right)
////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Modifications by Yogesh Jagota.
// All my modifications in the existing code...
//	 a) either start with 'Yogesh Jagota' and end with 'End Yogesh Jagota',
//	 b) or, are commented in the end of line with 'Yogesh Jagota',
//
//						/************************
// Everything after the  * All the following... * box is added by me.
//						 ************************/
//
//  Sn. Description											Date
// ==== =================================================== ===============
//   1. Removed bold fonts from pressed buttons (Reason :
//		because I did'nt liked them. One pressed button was 
//		destroying the looks of all the other buttons).		07th Mar, 2000
// ---- ---------------------------------------------------	---------------
//   2. Turned buttons to "Hot" flat buttons [active on 
//		mouse over]. (Reason : Everything is going the hot
//		way, isn'nt it....)									07th Mar, 2000
// ---- ---------------------------------------------------	---------------
//	 3. Added borders before buttons to look more cool.		07th Mar, 2000
// ---- ---------------------------------------------------	---------------
//   4. Added Tooltip support.								07th Mar, 2000
// ---- ---------------------------------------------------	---------------
//   5. Now, the button cannot be unselected.				08th Mar, 2000
// ---- ---------------------------------------------------	---------------
//   6. The default font is now 'Tahoma'. If it's not found
//		'MS Sans Serif' is used. (You know something, I 
//		love that font.)									09th Mar, 2000
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SwitcherButton.h"
#include "docselect.h"
#include "resource.h"
#include <winuser.h>
#include "settings.h"
#include ".\switcherbutton.h"
#include "SystemInfo.h"

extern CSystemInfo  g_sSystem;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSwitcherButton

extern CSettings g_sSettings;

CSwitcherButton::CSwitcherButton()
{
	m_nState = SWITCHBUTTON_UP;
	HasCapture = false;
	m_iIcon = NULL;
	m_hBmp  = NULL;
	m_iID = -1;

	// Yogesh Jagota
	m_bPainted		= FALSE; 
	m_bLBtnDown		= FALSE;
	m_bHiLited		= FALSE;
	m_AttachedView	= NULL;
	m_bIsChatClient = FALSE;
	m_bOldTool      = FALSE;
	// End Yogesh Jagota

	// Win95/98 balks at creating & deleting the font in OnPaint
	// so put it here

	// Yogesh Jagota
	if ( !m_fNormal.CreateFont( 13, 0, 0, 0, FW_NORMAL, 0, 
		0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_QUALITY | FF_DONTCARE, _T("Tahoma") ) )
	{
		m_fNormal.CreateFont(10,0,0,0,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_QUALITY|FF_DONTCARE,_T("MS Sans Serif"));
	}
	// End Yogesh Jagota

	m_fBold.CreateFont(13, 0, 0, 0, FW_BOLD, 0,
		0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_QUALITY|FF_DONTCARE, "Tahoma");
//	End Yogesh Jagota
}

CSwitcherButton::~CSwitcherButton()
{
	m_fNormal.DeleteObject();
	m_fBold.DeleteObject();
	if(m_hBmp) DeleteObject(m_hBmp);
	if(m_iIcon) DeleteObject(m_iIcon);
}


BEGIN_MESSAGE_MAP(CSwitcherButton, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSwitcherButton message handlers

void CSwitcherButton::OnPaint() 
{
	CPaintDC paintdc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect);

	CMemDC dc(&paintdc, &rect);
	
	// center the icon
	int icontop = (rect.Height() / 2) - (16 / 2);

	CString text;
	GetWindowText(text);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(m_bHiLited ? g_sSettings.GetRGBDocHiLite() : ::GetSysColor(COLOR_BTNTEXT));

	CBrush brush;

	// Yogesh Jagota
	// Don't want borders before buttons, remove this fraction
	// of code, and a single line (commented) in the OnTimer function.
	brush.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
	dc.FillRect(&rect, &brush);
	brush.DeleteObject();

	CRect rct;
	rct.left = rect.left;
	rct.top = rect.top;
	rct.right  = rect.left + 2;
	rct.bottom = rect.bottom;

	dc.Draw3dRect(&rct, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));

	rect.left += 3;
	// End Yogesh Jagota

	if (m_nState == SWITCHBUTTON_UP)
	{
		brush.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
		dc.FillRect(&rect, &brush);

//		Yogesh Jagota
//		dc.Draw3dRect(&rect, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DDKSHADOW));
//		rect.DeflateRect(1, 1);
//		dc.Draw3dRect(&rect, ::GetSysColor(COLOR_3DLIGHT), ::GetSysColor(COLOR_3DSHADOW));
//		End Yogesh Jagota
	}
	else if (m_nState == SWITCHBUTTON_DOWN)
	{
		brush.CreateSolidBrush(::GetSysColor(COLOR_3DFACE));
		dc.FillRect(&rect, &brush);
		dc.Draw3dRect(&rect, ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_3DHIGHLIGHT));
//		Yogesh Jagota
//		rect.DeflateRect(1,1);
//		dc.Draw3dRect(&rect, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DLIGHT));
//		End Yogesh Jagota

		// make it look pressed
		rect.top += 1;
		icontop += 1;
	}
	else if (m_nState == SWITCHBUTTON_SELECTED)
	{
		brush.CreateSolidBrush(::GetSysColor(COLOR_3DLIGHT));
		dc.FillRect(&rect, &brush);
		dc.Draw3dRect(&rect, ::GetSysColor(COLOR_3DDKSHADOW), ::GetSysColor(COLOR_3DHIGHLIGHT));
//		Yogesh Jagota
//		rect.DeflateRect(1,1);
//		dc.Draw3dRect(&rect, ::GetSysColor(COLOR_3DSHADOW), ::GetSysColor(COLOR_3DLIGHT));
//		End Yogesh Jagota

		// make it look pressed
		rect.top+= 1;
		icontop += 1;
	}

	// dont even bother if no text
	if (text.IsEmpty() == FALSE)
	{
		// dont want text near the border
		rect.DeflateRect(2,2);

		// MS BUG: DT_VCENTER dont work! (or im doing something wrong)
		// so must Vertical Center it ourself
		CSize size;
		GetTextExtentPoint32(dc.GetSafeHdc(), text, text.GetLength(), &size);

		rect.top += (rect.Height() / 2) - (size.cy / 2) + 1;
		rect.left += 20;

//		Yogesh Jagota
//		if (m_nState == SWITCHBUTTON_SELECTED)
//			dc.SelectObject(&m_fBold);
//		else
//		End Yogesh Jagota
		dc.SelectObject((m_bHiLited ? &m_fBold : &m_fNormal));

		dc.DrawText(text, &rect, DT_END_ELLIPSIS|DT_VCENTER);
	}

	if (m_iIcon != NULL)
		::DrawIconEx(dc.GetSafeHdc(), 4, icontop, m_iIcon, 16, 16, 0, (HBRUSH)brush, DI_NORMAL);
	else if (m_hBmp != NULL){


		HDC hMemDC = ::CreateCompatibleDC(dc.GetSafeHdc());
		::SelectObject(hMemDC, m_hBmp);

		BITMAP bm;
		GetObject(m_hBmp, sizeof (BITMAP), (LPSTR)&bm);

		int nOldMode = dc.GetStretchBltMode();
		dc.SetStretchBltMode(HALFTONE);
		::StretchBlt(dc.GetSafeHdc(), 4, icontop, 16, 16, hMemDC, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
		dc.SetStretchBltMode(nOldMode);
		//::BitBlt(dc.GetSafeHdc(), 4, icontop, 16, 16, hMemDC, 0, 0, SRCCOPY);
		DeleteObject(hMemDC);
	}
	brush.DeleteObject();
}

void CSwitcherButton::OnRButtonDown(UINT nFlags, CPoint point) 
{

	::PostMessage(m_wndParent->GetSafeHwnd(), SWM_RCLICK, (WPARAM)this, 0);
}

void CSwitcherButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);

	if (m_nState != SWITCHBUTTON_SELECTED)
	{
		SetCapture();
		m_nState = SWITCHBUTTON_DOWN;
		HasCapture = true;
		m_bHiLited = FALSE;
		Invalidate();
	}

	m_bLBtnDown = TRUE; // Yogesh Jagota
}

void CSwitcherButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonUp(nFlags, point);

	bool change = true;

	if (m_nState == SWITCHBUTTON_DOWN)
	{
		m_nState = SWITCHBUTTON_SELECTED;
		m_FrameWnd->ShowWindow(SW_SHOW);
		::SendMessage(m_wndParent->GetSafeHwnd(), SWM_SELCHANGE, (WPARAM)this, 0);
	}
	else if(m_nState == SWITCHBUTTON_SELECTED){

		WINDOWPLACEMENT wp;
		m_FrameWnd->GetWindowPlacement(&wp);
		if(wp.showCmd == SW_SHOWMINIMIZED){

			m_FrameWnd->ShowWindow(SW_SHOW);
			m_FrameWnd->ShowWindow(SW_RESTORE);
		}
	}
//	Yogesh Jagota.
//	Can't deselect button now...
//	else
//	{
//		m_bLBtnDown = FALSE; // Yogesh Jagota
//
//		if (m_nState == SWITCHBUTTON_UP)
//			change = false;
//
//		if (m_nState == SWITCHBUTTON_SELECTED)
//			::SendMessage(m_wndParent->GetSafeHwnd(), SWM_UNSELECT, (WPARAM)this, 0);
//
//		m_nState = SWITCHBUTTON_UP;
//	}
//	End Yogesh Jagota

	ReleaseCapture();
	HasCapture = false;

	if (change)
		Invalidate();
}

void CSwitcherButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CWnd::OnMouseMove(nFlags, point);

	SetTimer(1, 10, NULL);	// Yogesh Jagota
	OnTimer(1);				// Yogesh Jagota

	if (HasCapture)
	{
		RECT rect;
		GetClientRect(&rect);

		if (point.x > rect.right || point.x < rect.left || point.y < rect.top || point.y > rect.bottom)
		{
			if (m_nState != SWITCHBUTTON_UP)
			{
				m_nState = SWITCHBUTTON_UP;
				Invalidate();
			}
		}
		else
		{
			if (m_nState != SWITCHBUTTON_DOWN)
			{
				m_nState = SWITCHBUTTON_DOWN;
				Invalidate();
			}
		}
	}
}



// Yogesh Jagota : Added support for tooltips...
BOOL CSwitcherButton::DoCreate(CWnd *parent, int x, int y, int cx, int cy, CString sFileName, CString sPath )
{

	m_bOldTool = g_sSettings.GetUseOldTools();

	m_wndParent = parent;

	sFileName.Replace("&", "&&");
	sPath.Replace("&", "&&");

	WNDCLASS myclass;
	myclass.style = CS_HREDRAW|CS_VREDRAW|CS_PARENTDC;
	myclass.lpfnWndProc = AfxWndProc;
	myclass.cbClsExtra = 0;
	myclass.cbWndExtra = 0;
	myclass.hInstance = AfxGetInstanceHandle();
	myclass.hIcon = NULL;
	myclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	myclass.hbrBackground = NULL;
	myclass.lpszMenuName = NULL;
	myclass.lpszClassName = _T("SwitcherButtonClass");
	AfxRegisterClass(&myclass);

	CRect rct(x,y,cx+x,cy+y);
	BOOL ret = Create( _T("SwitcherButtonClass"), sFileName, WS_CHILD|WS_VISIBLE, rct, parent, 0);

	// Yogesh Jagota
	GetClientRect( rct );
	BOOL bSuccess = FALSE;
	if(m_bOldTool){

		bSuccess = m_OldToolTip.Create( this );
	}
	else{

		bSuccess = m_ToolTip.Create( this );
	}
	
	if(bSuccess){

		CreateToolTip();
	}
		//m_ToolTip.AddTool( this, sPath, rct, 1 );
	// End Yogesh Jagota

	return ret;
}

void CSwitcherButton::SetText(CString text)
{
	text.Replace("&", "&&");
	//CRect rct;
	//GetClientRect(rct);
	//m_ToolTip.RemoveTool(this);
	//m_ToolTip.DelTool(this);
	//m_ToolTip.AddTool(this, text, rct, 1);
	SetWindowText(text);
	CreateToolTip();
	Invalidate();
	return;
}

void CSwitcherButton::ReplaceIcon(HICON icon)
{
	m_iIcon = icon;
	Invalidate();
}

void CSwitcherButton::Refresh()
{
	Invalidate();
}

void CSwitcherButton::Select()
{
	if (HasCapture)
	{
		ReleaseCapture();
		HasCapture = false;
	}

	m_nState = SWITCHBUTTON_SELECTED;
	m_bLBtnDown = TRUE; // Yogesh Jagota
	m_bHiLited = FALSE;
	Invalidate();
}

void CSwitcherButton::Unselect()
{
	if (HasCapture)
	{
		ReleaseCapture();
		HasCapture = false;
	}

	m_bLBtnDown = FALSE; // Yogesh Jagota
	m_nState = SWITCHBUTTON_UP;
	m_bHiLited = FALSE;
	Invalidate();
}

/************************************************************************
 ************************************************************************
 *																		*
 * All following code is added by Yogesh Jagota							*
 *																		*
 ************************************************************************
 ************************************************************************/
void CSwitcherButton::OnTimer(UINT nIDEvent) 
{
	CRect rcItem;
	GetWindowRect(rcItem);

	CPoint ptCursor;
	GetCursorPos(&ptCursor);

	if(( m_bLBtnDown == TRUE ) || ( !rcItem.PtInRect( ptCursor )))
	{
		KillTimer(1);

		if (m_bPainted == TRUE) {
			InvalidateRect (NULL);
		}

		m_bPainted = FALSE;
		return;
	}

	// On mouse over, show raised button.
	else if ( !m_bPainted )
	{
		CDC* pDC = GetDC();
		GetClientRect(rcItem);
		rcItem.left += 3; // Don't want borders. Delete this line.
		pDC->Draw3dRect( rcItem, ::GetSysColor(COLOR_3DHIGHLIGHT), ::GetSysColor(COLOR_3DSHADOW));
		m_bPainted = TRUE;
		ReleaseDC(pDC);
	}
	
	CWnd::OnTimer(nIDEvent);
}

BOOL CSwitcherButton::PreTranslateMessage( MSG *pMsg )
{

	if(m_bOldTool){

		m_OldToolTip.RelayEvent( pMsg );
	}
	else{

		m_ToolTip.RelayEvent( pMsg );
	}

	return CWnd::PreTranslateMessage( pMsg );
}

void CSwitcherButton::SetHiLite(void)
{

	m_bHiLited = TRUE;
	Invalidate();
}

void CSwitcherButton::CreateToolTip(void)
{
	
	CRect rct;
	GetClientRect(rct);
	CString sPath;
	GetWindowText(sPath);

	if(m_bOldTool){

		m_OldToolTip.DelTool(this);
		m_OldToolTip.AddTool(this, sPath, rct, 1);
	}
	else{

		m_ToolTip.RemoveAllTools();
		//m_ToolTip.SetDefaultSizes(FALSE);
		m_ToolTip.SetEffectBk(g_sSettings.GetToolEffect(), g_sSettings.GetToolGran());
		m_ToolTip.SetBehaviour(PPTOOLTIP_MULTIPLE_SHOW);
		m_ToolTip.SetColorBk(g_sSettings.GetToolBg1(), g_sSettings.GetToolBg2(), g_sSettings.GetToolBg3());
		m_ToolTip.HideBorder();
		m_ToolTip.SetDelayTime(PPTOOLTIP_TIME_INITIAL, 500);
		m_ToolTip.SetDelayTime(PPTOOLTIP_TIME_AUTOPOP, 15000);
		m_ToolTip.SetDelayTime(PPTOOLTIP_TIME_FADEIN, 5);
		m_ToolTip.SetDelayTime(PPTOOLTIP_TIME_FADEOUT, 5);
		m_ToolTip.EnableEscapeSequences(FALSE);
		m_ToolTip.SetDirection(PPTOOLTIP_TOPEDGE_LEFT);
		m_ToolTip.SetTransparency(g_sSettings.GetToolTrans());
		m_ToolTip.SetSize(CPPToolTip::PPTTSZ_MARGIN_ANCHOR, 0);
		m_ToolTip.SetSize(CPPToolTip::PPTTSZ_WIDTH_ANCHOR, 0);
		m_ToolTip.SetSize(CPPToolTip::PPTTSZ_HEIGHT_ANCHOR, 0);

		if(m_AttachedView != NULL && IsWindow(m_AttachedView->m_hWnd)){

			m_ToolTip.SetNotify(m_AttachedView->m_hWnd);
		}
		m_ToolTip.AddTool(this, sPath, rct, 1);
	}
}

void CSwitcherButton::DisplayTool(CString strText)
{

	if(g_sSettings.GetUseOldTools() == FALSE){

		CRect rc;
		GetWindowRect(rc);
		CPoint pt = rc.CenterPoint();
		m_ToolTip.ShowHelpTooltip(&pt, strText);
	}
}
