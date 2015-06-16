/*
** Copyright (C) 2004 Bender
**  
** RoboMX is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** RoboMX is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#include "stdafx.h"
#include "RoboMX.h"
#include "BrowseDoc.h"


// CBrowseDoc

IMPLEMENT_DYNCREATE(CBrowseDoc, CMyDocument)

CBrowseDoc::CBrowseDoc()
{

	m_dwIP = 0;
	m_wPort = 0;
	m_strMyDocString = "CBroswDoc";
}

BOOL CBrowseDoc::OnNewDocument()
{
	if (!CMyDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CBrowseDoc::~CBrowseDoc()
{
}


BEGIN_MESSAGE_MAP(CBrowseDoc, CMyDocument)
END_MESSAGE_MAP()


// CBrowseDoc diagnostics

#ifdef _DEBUG
void CBrowseDoc::AssertValid() const
{
	CMyDocument::AssertValid();
}

void CBrowseDoc::Dump(CDumpContext& dc) const
{
	CMyDocument::Dump(dc);
}
#endif //_DEBUG


// CBrowseDoc serialization

void CBrowseDoc::Serialize(CArchive& ar)
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


// CBrowseDoc commands
