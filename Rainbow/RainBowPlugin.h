#pragma once
#include "..\roboex.h"

class CRainBowPlugin :
	public CRoboEx
{
public:
	CRainBowPlugin(void);
	~CRainBowPlugin(void);
	void OnLeaveChannel(DWORD dwID);
	void OnJoinChannel(DWORD dwID, CString strChannel, CUserMap* pUserMap, CString strServerString, CString strServerVersion);
	void OnInputHook(DWORD dwID, CString *pInput);
	CMap<DWORD, DWORD, CString, CString&> m_aServers;
};
