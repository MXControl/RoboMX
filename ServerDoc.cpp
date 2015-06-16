// RichEditTestDoc.cpp : implementation of the CRichEditTestDoc class
//

#include "stdafx.h"

#ifdef _ENABLE_SERVER
#include "Metis3.h"

#include "ServerDoc.h"
#include "ServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRichEditTestDoc

IMPLEMENT_DYNCREATE(CServerDoc, CMyDocument)

BEGIN_MESSAGE_MAP(CServerDoc, CMyDocument)
END_MESSAGE_MAP()


// CRichEditTestDoc construction/destruction

CServerDoc::CServerDoc()
{

	m_strMyDocString = "CServerDoc";
}

CServerDoc::~CServerDoc()
{
}

BOOL CServerDoc::OnNewDocument()
{
	if (!CMyDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	SetTitle("Unnamed Roomserver");
	return TRUE;
}

BOOL CServerDoc::SaveModified()
{

	// we dont want that dammned save document message :P
	// but we need to stop the server before the view is destroyed :P

	POSITION pos = GetFirstViewPosition();
	CServerView* pView = (CServerView*)GetNextView(pos);
	if(pView != NULL){

		pView->OnCloseDocument();
	}
	return TRUE;
}


// CRichEditTestDoc serialization

void CServerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}

	CMyDocument::Serialize(ar);
}


// CRichEditTestDoc diagnostics

#ifdef _DEBUG
void CServerDoc::AssertValid() const
{
	CMyDocument::AssertValid();
}

void CServerDoc::Dump(CDumpContext& dc) const
{
	CMyDocument::Dump(dc);
}
#endif //_DEBUG

#endif
// CRichEditTestDoc commands
