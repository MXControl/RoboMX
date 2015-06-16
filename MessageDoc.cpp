// MessageDoc.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "MessageDoc.h"
#include "MainFrm.h"

// CMessageDoc

IMPLEMENT_DYNCREATE(CMessageDoc, CMyDocument)

CMessageDoc::CMessageDoc()
{

	m_dwIP = 0;
	m_wPort = 0;
	m_strMyDocString = "CMessagDoc";
}

BOOL CMessageDoc::OnNewDocument()
{
	if (!CMyDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}

CMessageDoc::~CMessageDoc()
{
}


BEGIN_MESSAGE_MAP(CMessageDoc, CMyDocument)
END_MESSAGE_MAP()


// CMessageDoc diagnostics

#ifdef _DEBUG
void CMessageDoc::AssertValid() const
{
	CMyDocument::AssertValid();
}

void CMessageDoc::Dump(CDumpContext& dc) const
{
	CMyDocument::Dump(dc);
}
#endif //_DEBUG


// CMessageDoc serialization

void CMessageDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CMessageDoc commands

