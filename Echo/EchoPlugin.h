/*
** Copyright (C) 2002-2004 Thees Schwab
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
#include "..\roboex.h"

class CEchoPlugin :
	public CRoboEx
{
public:
	CEchoPlugin(void);
	~CEchoPlugin(void);
	void OnJoin(DWORD dwID, PMX_USERINFO pUser);
	void OnPart(DWORD dwID, PMX_USERINFO pUser);
	void OnMessage(DWORD dwID, CString* pUser, CString* pMsg, BOOL bIsAction);
	void OnOpMessage(DWORD dwID, CString* pUser, CString* pMsg);
	void OnPrepareMenu(DWORD dwID, BOOL bIsUser, HMENU hMenu);
	void OnMenuCommand(UINT nCmd, DWORD dwID, CString strName);
	void OnReloadCfg();
	void Init();
	void Quit(void);
	void OnJoinChannel(DWORD dwID, CString strChannel, CUserMap* pUserMap, CString strServerString, CString strServerVersion);
	void OnRenameChannel(DWORD dwID, CString strNewname);
	void OnLeaveChannel(DWORD dwID);

	void MakeShortRoom(CString& strRoom);
	void AdjustColor(COLORREF& crColor);

	CMap<DWORD, DWORD, CString, CString&> m_aRoomMap;
	DWORD m_dwOut;
	BOOL  m_bSysOut;
	HMENU m_hMenu;
	CString m_strIni;
	BOOL  m_bEnterLeave;

	COLORREF m_crName;
	COLORREF m_crMsg;
	COLORREF m_crAct;
	COLORREF m_crBg;
	COLORREF m_crOp;
	COLORREF m_crJoin;
	COLORREF m_crPart;
};
