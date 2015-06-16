// RichEditTestDoc.h : interface of the CRichEditTestDoc class
//

#include "MyDocument.h"

#pragma once

class CServerDoc : public CMyDocument
{
protected: // create from serialization only
	CServerDoc();
	DECLARE_DYNCREATE(CServerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual BOOL SaveModified();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CServerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


