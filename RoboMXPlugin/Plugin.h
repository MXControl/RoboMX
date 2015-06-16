#pragma once
#include "roboex.h"
#include <Afxmt.h>
#include <afxtempl.h>

typedef struct _ROOMDATA{

	DWORD dwID;
	BOOL bAnnounce;
} ROOMDATA;

class CPlugin :
	public CRoboEx
{
public:
	CPlugin(void);
	virtual ~CPlugin(void);

	void OnJoinChannel(DWORD dwID, CString strChannel, CUserMap *pUserMap);
	void OnLeaveChannel(DWORD dwID);
	void OnMessage(DWORD dwID, CString* pUser, CString* pMsg, BOOL bIsAction);
	void OnInputHook(DWORD dwIP, CString *pInput);
	void Init();
	void Quit();
	void Configure();
	static UINT MonitorThread(LPVOID pParam);

	CString m_strWd;
	CEvent  m_eDone;
	volatile BOOL m_bRun;
	int m_nInterval;

	// Stream Data
	CString m_strSong;
	CString m_strStreamType;
	CString m_strListeners;
	CString m_strStreamURL;
	CString m_strStatURL;

	CArray<ROOMDATA, ROOMDATA> m_aRooms;

	// Messages
    //DWORD m_dwRoom;
	CString m_strStart;
	CString m_strStop;
	CString m_strAnnouncement;

	COLORREF crColor;
	COLORREF crBg;
	BOOL QueryServer(void);
	void SaveCfg(void);
	void LoadCfg(void);
	ROOMDATA* GetRoom(DWORD dwID);
	void InputInActive(CString strText);
};
