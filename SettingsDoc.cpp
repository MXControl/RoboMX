// SettingsDoc.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "SettingsDoc.h"


// CSettingsDoc

IMPLEMENT_DYNCREATE(CSettingsDoc, CMyDocument)

CSettingsDoc::CSettingsDoc()
{

	m_strMyDocString = "CSettingsDoc";
}

BOOL CSettingsDoc::OnNewDocument()
{
	if (!CMyDocument::OnNewDocument())
		return FALSE;

	SetTitle("Settings");
	return TRUE;
}

CSettingsDoc::~CSettingsDoc()
{
}


BEGIN_MESSAGE_MAP(CSettingsDoc, CMyDocument)
END_MESSAGE_MAP()


// CSettingsDoc diagnostics

#ifdef _DEBUG
void CSettingsDoc::AssertValid() const
{
	CMyDocument::AssertValid();
}

void CSettingsDoc::Dump(CDumpContext& dc) const
{
	CMyDocument::Dump(dc);
}
#endif //_DEBUG


// CSettingsDoc serialization

void CSettingsDoc::Serialize(CArchive& ar)
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


// CSettingsDoc commands
