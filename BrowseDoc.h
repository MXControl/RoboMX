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

#pragma once
#include "MyDocument.h"


// CBrowseDoc document

class CBrowseDoc : public CMyDocument
{
	DECLARE_DYNCREATE(CBrowseDoc)

public:
	CBrowseDoc();
	virtual ~CBrowseDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public: // attributes
	CString m_strUser;
	DWORD	m_dwIP;
	WORD	m_wPort;

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
