#pragma once
#include "..\roboex.h"

class CTagPlugin :
	public CRoboEx
{
public:
	CTagPlugin(void);
	~CTagPlugin(void);
	
	void Init();
	void OnMessage(DWORD dwID, CString* pUser, CString* pMsg, BOOL bIsAction);
	void OnOpMessage(DWORD dwID, CString* pUser, CString* pMsg);
	void AntiTag(CString& strName);
	void OnPrepareMenu(DWORD dwID, BOOL bIsUser, HMENU hMenu);
	void OnMenuCommand(UINT nCmd, DWORD dwID, CString strName);
	void Configure();

	BOOL    m_bRun;
	CString m_strIni;
};
