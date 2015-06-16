#pragma once

#include "MyDocument.h"


// CMessageDoc document

class CMessageDoc : public CMyDocument
{
	DECLARE_DYNCREATE(CMessageDoc)

public:
	CMessageDoc();
	virtual ~CMessageDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public: // Attributes
	CString m_strUser;
	CString m_strMsg;
	DWORD	m_dwIP;
	WORD	m_wPort;

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
};
