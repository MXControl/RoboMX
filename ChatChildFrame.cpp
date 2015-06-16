// ChatChildFrame.cpp : implementation file
//

#include "stdafx.h"
#include "Metis3.h"
#include "ChatChildFrame.h"


// CChatChildFrame

IMPLEMENT_DYNCREATE(CChatChildFrame, CMDIChildWnd)

CChatChildFrame::CChatChildFrame()
	: m_strProfileName( _T("Default RoboMX Chatroom Profile") )
{
	m_hChildFrameIcon = NULL;
HINSTANCE hInstResource =
		AfxFindResourceHandle(
			MAKEINTRESOURCE(IDR_CHANNEL),
			RT_GROUP_ICON
			);
	ASSERT( hInstResource != NULL );
	if( hInstResource != NULL )
	{
		m_hChildFrameIcon =
			(HICON)::LoadImage(
				hInstResource,
				MAKEINTRESOURCE(IDR_CHANNEL),
				IMAGE_ICON,
				16,
				16,
				0
				);
		ASSERT( m_hChildFrameIcon != NULL );
	}
}

CChatChildFrame::~CChatChildFrame()
{
	if( m_hChildFrameIcon != NULL )
	{
		::DestroyIcon( m_hChildFrameIcon );
	}
}


BEGIN_MESSAGE_MAP(CChatChildFrame, CMDIChildWnd)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_WM_SETFOCUS()
	ON_WM_CREATE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_NCPAINT()

	ON_COMMAND_EX(ID_VIEW_SYSTEM_BAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_SYSTEM_BAR, OnUpdateControlBarMenu)

	ON_COMMAND_EX(ID_VIEW_USERLIST_BAR, OnBarCheck )
	ON_UPDATE_COMMAND_UI(ID_VIEW_USERLIST_BAR, OnUpdateControlBarMenu)
END_MESSAGE_MAP()


// CChatChildFrame message handlers
void CChatChildFrame::OnUpdateControlBarMenu(CCmdUI* pCmdUI)
{
	CExtControlBar::DoFrameBarCheckUpdate( this, pCmdUI, true );
}

BOOL CChatChildFrame::OnBarCheck(UINT nID)
{
	return CExtControlBar::DoFrameBarCheckCmd( this, nID, true );
}

BOOL CChatChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChatChildFrame diagnostics

#ifdef _DEBUG
void CChatChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChatChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChatChildFrame message handlers
void CChatChildFrame::OnFileClose() 
{
	// To close the frame, just send a WM_CLOSE, which is the equivalent
	// choosing close from the system menu.

	SendMessage(WM_CLOSE);
}

int CChatChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	ASSERT( m_hChildFrameIcon != NULL );
	SetIcon( m_hChildFrameIcon, FALSE );
	SetIcon( m_hChildFrameIcon, TRUE );

	// create a view to occupy the client area of the frame
	/*if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW, 
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

	if(	!m_wndToolbar0.Create(
			_T("Child Toolbar 0"),
			this,
			IDR_TOOLBAR_CHILD0
			)
		|| !m_wndToolbar0.LoadToolBar(IDR_TOOLBAR_CHILD0)
		)
	{
		TRACE0("Failed to create m_wndToolbar0\n");
		return -1;		// fail to create
	}

	if(	!m_wndToolbar1.Create(
			_T("Child Toolbar 1"),
			this,
			IDR_TOOLBAR_CHILD1
			)
		|| !m_wndToolbar1.LoadToolBar(IDR_TOOLBAR_CHILD1)
		)
	{
		TRACE0("Failed to create m_wndToolbar1\n");
		return -1;		// fail to create
	}

	if(	!m_wndResizableBar0.Create(
			_T("Resizable Bar 0"),
			this,
			ID_VIEW_CHILDFRAME_BAR_0
			)
		|| !m_wndListBox0.Create(
			WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT,
			CRect( 0, 0, 0, 0 ),
			&m_wndResizableBar0,
			UINT( IDC_STATIC )
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar0\n");
		return -1;		// fail to create
	}
	m_wndListBox0.SetFont( CFont::FromHandle( (HFONT)::GetStockObject(DEFAULT_GUI_FONT) ) );
	m_wndListBox0.AddString( _T("CChatChildFrame::m_wndListBox0") );

	if(	!m_wndResizableBar1.Create(
			_T("Resizable Bar 1"),
			this,
			ID_VIEW_CHILDFRAME_BAR_1
			)
		|| !m_wndListBox1.Create(
			WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT,
			CRect( 0, 0, 0, 0 ),
			&m_wndResizableBar1,
			UINT( IDC_STATIC )
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar1\n");
		return -1;		// fail to create
	}
	m_wndListBox1.SetFont( CFont::FromHandle( (HFONT)::GetStockObject(DEFAULT_GUI_FONT) ) );
	m_wndListBox1.AddString( _T("CChatChildFrame::m_wndListBox1") );

	if(	!m_wndResizableBar2.Create(
			_T("Resizable Bar 2"),
			this,
			ID_VIEW_CHILDFRAME_BAR_2
			)
		|| !m_wndListBox2.Create(
			WS_CHILD|WS_VISIBLE|LBS_NOINTEGRALHEIGHT,
			CRect( 0, 0, 0, 0 ),
			&m_wndResizableBar2,
			UINT( IDC_STATIC )
			)
		)
	{
		TRACE0("Failed to create m_wndResizableBar2\n");
		return -1;		// fail to create
	}
	m_wndListBox2.SetFont( CFont::FromHandle( (HFONT)::GetStockObject(DEFAULT_GUI_FONT) ) );
	m_wndListBox2.AddString( _T("CChatChildFrame::m_wndListBox2") );

	m_wndToolbar0.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolbar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar0.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar1.EnableDocking(CBRS_ALIGN_ANY);
	m_wndResizableBar2.EnableDocking(CBRS_ALIGN_ANY);

	if( !CExtControlBar::FrameEnableDocking(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
#if (!defined __EXT_MFC_NO_TAB_CONTROLBARS)
	if( !CExtControlBar::FrameInjectAutoHideAreas(this) )
	{
		ASSERT( FALSE );
		return -1;
	}
#endif // (!defined __EXT_MFC_NO_TAB_CONTROLBARS)

	DockControlBar(&m_wndToolbar0,AFX_IDW_DOCKBAR_TOP);
	DockControlBar(&m_wndToolbar1,AFX_IDW_DOCKBAR_LEFT);


	m_wndResizableBar0.DockControlBar( AFX_IDW_DOCKBAR_TOP, 1 );
	m_wndResizableBar0.DockControlBar( &m_wndResizableBar1, true );
	m_wndResizableBar2.DockControlBar( AFX_IDW_DOCKBAR_BOTTOM, 1 );

CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );

	CExtControlBar::ProfileBarStateLoad(
		this,
		pApp->m_pszRegistryKey,
		pApp->m_pszProfileName,
		LPCTSTR( m_strProfileName )
		);
*/
	return 0;
}

void CChatChildFrame::OnSetFocus(CWnd* pOldWnd) 
{
	CMDIChildWnd::OnSetFocus(pOldWnd);

//	m_wndView.SetFocus();
}

BOOL CChatChildFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// let the view have first crack at the command
/*	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	*/
	// otherwise, do default handling
	return CMDIChildWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BOOL CChatChildFrame::PreTranslateMessage(MSG* pMsg) 
{
/*CMainFrame * pMainFrame =
		DYNAMIC_DOWNCAST(
			CMainFrame,
			::AfxGetMainWnd()
			);
	ASSERT_VALID( pMainFrame );
	if( pMainFrame->m_wndMenuBar.TranslateMainFrameMessage(pMsg) )
		return TRUE;
	*/
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

BOOL CChatChildFrame::DestroyWindow() 
{
CWinApp * pApp = ::AfxGetApp();
	ASSERT( pApp != NULL );
	ASSERT( pApp->m_pszRegistryKey != NULL );
	ASSERT( pApp->m_pszRegistryKey[0] != _T('\0') );

	VERIFY(
		CExtControlBar::ProfileBarStateSave(
			this,
			pApp->m_pszRegistryKey,
			pApp->m_pszProfileName,
			LPCTSTR( m_strProfileName )
			)
		);
	g_CmdManager->ProfileWndRemove( GetSafeHwnd() );
	
	return CMDIChildWnd::DestroyWindow();
}

void CChatChildFrame::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	// OnWindowPosChanged() and OnNcPaint() methods
	// implemented to fix WinXP MDI interface flicker bug
CMDIFrameWnd * pMdiFrame =
		STATIC_DOWNCAST( CMDIFrameWnd, GetParentFrame() );
	ASSERT_VALID( pMdiFrame );
BOOL bMax = FALSE;
CMDIChildWnd * pActive = pMdiFrame->MDIGetActive( &bMax );
	pActive;
	if( ! bMax )
	{
		CMDIChildWnd::OnWindowPosChanged(lpwndpos);
		return;
	} // if( ! bMax )
	SetRedraw( FALSE );
	CMDIChildWnd::OnWindowPosChanged(lpwndpos);
	SetRedraw( TRUE );
	RedrawWindow(
		NULL, NULL,
		RDW_INVALIDATE|RDW_ERASE|RDW_ALLCHILDREN|RDW_FRAME
		);
}

void CChatChildFrame::OnNcPaint() 
{
	// OnWindowPosChanged() and OnNcPaint() methods
	// implemented to fix WinXP MDI interface flicker bug
CMDIFrameWnd * pMdiFrame =
		STATIC_DOWNCAST( CMDIFrameWnd, GetParentFrame() );
	ASSERT_VALID( pMdiFrame );
BOOL bMax = FALSE;
CMDIChildWnd * pActive = pMdiFrame->MDIGetActive( &bMax );
	pActive;
	if( bMax )
		return;
	CMDIChildWnd::OnNcPaint();
}
