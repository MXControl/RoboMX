#pragma once
#include "afxwin.h"

class CMyProgressCtrl :
	public CStatic
{
public:
	CMyProgressCtrl(void);
	~CMyProgressCtrl(void);
public:
	void SetRange(UINT nStart, UINT nEnd);
	void SetPos(UINT nPos);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	UINT m_nStart;
	UINT m_nEnd;
	UINT m_nPos;

	DECLARE_MESSAGE_MAP()
};
