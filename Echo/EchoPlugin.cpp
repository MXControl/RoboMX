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

#include "StdAfx.h"
#include "EchoPlugin.h"
#include "../rmx_qapi.h"
#include "../Ini.h"

#define ID_MENUBASE  40800

CEchoPlugin::CEchoPlugin(void)
{

	m_strName			= "EchoPlugin";
	m_strDescription	= "Displays messages from other chatrooms in window of your choice.";
	m_strAuthor			= "Bender979";
	m_dwOut				= 0;
	
	m_crName			= RGB(255, 255, 0);
	m_crMsg				= RGB(0, 0, 0);
	m_crAct				= RGB(255, 0, 255);
	m_crBg				= RGB(0, 0, 0);
	m_crOp				= RGB(255, 0, 0);
}

CEchoPlugin::~CEchoPlugin(void)
{
}

void CEchoPlugin::Init()
{

	int nLen = (int)QueryInformation(RMX_GET_CONFIG, 0, 0);

	if(nLen > 0){

		char *buffer = new char[nLen+1];
		if(QueryInformation(RMX_GET_CONFIG, nLen+1, (LPARAM)buffer) > 0){

			m_strIni = buffer;
			CIni ini;
			ini.SetIniFileName(m_strIni);
			m_bSysOut		= ini.GetValue("EchoPlugin", "SysOut", FALSE);
			m_bEnterLeave	= ini.GetValue("EchoPlugin", "ShowEnterLeave", FALSE);
		}
		delete buffer;
		buffer = 0;
	}

	OnReloadCfg();
}

void CEchoPlugin::Quit()
{

	CIni ini;
	ini.SetIniFileName(m_strIni);
	ini.SetValue("EchoPlugin", "SysOut", m_bSysOut);
	ini.SetValue("EchoPlugin", "ShowEnterLeave", m_bEnterLeave);
}

void CEchoPlugin::OnReloadCfg()
{

	m_crName	= (COLORREF)QueryInformation(RMX_CFG_COLOR_NAME, 0, 0);
	m_crMsg		= (COLORREF)QueryInformation(RMX_CFG_COLOR_MSG, 0, 0);
	m_crAct		= (COLORREF)QueryInformation(RMX_CFG_COLOR_ACTION, 0, 0);
	m_crBg		= (COLORREF)QueryInformation(RMX_CFG_COLOR_BG, 0, 0);
	m_crOp		= (COLORREF)QueryInformation(RMX_CFG_COLOR_OPMSG, 0, 0);
	m_crPart	= (COLORREF)QueryInformation(RMX_CFG_COLOR_PART, 0, 0);
	m_crJoin	= (COLORREF)QueryInformation(RMX_CFG_COLOR_ENTER, 0, 0);

	AdjustColor(m_crJoin);
	AdjustColor(m_crPart);
	AdjustColor(m_crName);
	AdjustColor(m_crMsg);
	AdjustColor(m_crAct);
	AdjustColor(m_crBg);
	AdjustColor(m_crOp);
}

void CEchoPlugin::OnJoinChannel(DWORD dwID, CString strChannel, CUserMap* pUserMap, CString strServerString, CString strServerVersion)
{

	MakeShortRoom(strChannel);
	m_aRoomMap.SetAt(dwID, strChannel);
}

void CEchoPlugin::OnRenameChannel(DWORD dwID, CString strNewname)
{

	MakeShortRoom(strNewname);
	m_aRoomMap.SetAt(dwID, strNewname);
}

void CEchoPlugin::OnLeaveChannel(DWORD dwID)
{

	m_aRoomMap.RemoveKey(dwID);
	if(m_dwOut == dwID){

		m_dwOut		= 0;
		m_bSysOut	= FALSE;
	}
}

void CEchoPlugin::MakeShortRoom(CString& strRoom)
{

	int n = strRoom.ReverseFind('_');
	if(n > 0){

       strRoom = strRoom.Left(n);
	}
}

void CEchoPlugin::AdjustColor(COLORREF& crColor)
{

	UINT nRed   = GetRValue(crColor);
	UINT nGreen = GetGValue(crColor);
	UINT nBlue  = GetBValue(crColor);

	if (nRed > 240 && nGreen > 240 && nBlue > 240)
	{
		nRed   -= 8;
		nGreen -= 8;
		nBlue  -= 8;
	}
	else
	{
		if (nRed   < 232) nRed   += nRed   / 10; else nRed   = 255;
		if (nGreen < 232) nGreen += nGreen / 10; else nGreen = 255;
		if (nBlue  < 232) nBlue  += nBlue  / 10; else nBlue  = 255;
	}

	crColor = RGB(nRed, nGreen, nBlue);
}

void CEchoPlugin::OnJoin(DWORD dwID, PMX_USERINFO pUser)
{

	if((m_dwOut != 0) && (dwID != m_dwOut) && m_bEnterLeave){

		CString strText, strRoom;
		m_aRoomMap.Lookup(dwID, strRoom);

		strText.Format("[%s] %s has entered", strRoom, *pUser->strUser);

		if(m_bSysOut){

			WriteSystemEchoText(m_dwOut, strText, m_crJoin, m_crBg);
		}
		else{

			WriteEchoText(m_dwOut, strText + "\n", m_crJoin, m_crBg);
		}
	}
}

void CEchoPlugin::OnPart(DWORD dwID, PMX_USERINFO pUser)
{

	if((m_dwOut != 0) && (dwID != m_dwOut) && m_bEnterLeave){

		CString strText, strRoom;
		m_aRoomMap.Lookup(dwID, strRoom);

		strText.Format("[%s] %s has left", strRoom, *pUser->strUser);

		if(m_bSysOut){

			WriteSystemEchoText(m_dwOut, strText, m_crPart, m_crBg);
		}
		else{

			WriteEchoText(m_dwOut, strText + "\n", m_crPart, m_crBg);
		}
	}
}

void CEchoPlugin::OnMessage(DWORD dwID, CString* pUser, CString* pMsg, BOOL bIsAction)
{

	if((m_dwOut != 0) && (dwID != m_dwOut)){

		CString strPrefix, strRoom;
		m_aRoomMap.Lookup(dwID, strRoom);

		strPrefix.Format(bIsAction ? "[%s] %s " : "[%s] <%s> ", strRoom, *pUser);

		if(m_bSysOut){

			WriteSystemEchoText(m_dwOut, strPrefix + *pMsg, m_crMsg, m_crBg);
		}
		else{

			WriteEchoText(m_dwOut, strPrefix, (bIsAction ? m_crAct : m_crName), m_crBg);
			WriteEchoText(m_dwOut, *pMsg + "\n", (bIsAction ? m_crAct : m_crMsg), m_crBg);
		}
	}
}

void CEchoPlugin::OnOpMessage(DWORD dwID, CString* pUser, CString* pMsg)
{

	if((m_dwOut != 0) && (dwID != m_dwOut)){

		CString strPrefix, strRoom;
		m_aRoomMap.Lookup(dwID, strRoom);

		strPrefix.Format("[%s] %s ", strRoom, *pUser);

		if(m_bSysOut){

			WriteSystemEchoText(m_dwOut, strPrefix + *pMsg, m_crOp, m_crBg);
		}
		else{

			WriteEchoText(m_dwOut, strPrefix, m_crOp, m_crBg);
			WriteEchoText(m_dwOut, *pMsg + "\n", m_crOp, m_crBg);
		}
	}
}

void CEchoPlugin::OnPrepareMenu(DWORD dwID, BOOL bIsUser, HMENU hMenu)
{

	if(!bIsUser && dwID){


		m_hMenu = ::CreatePopupMenu();
		if(m_hMenu){

			::AppendMenu(m_hMenu, MF_STRING, ID_MENUBASE, "Echo in this window");
			::AppendMenu(m_hMenu, MF_STRING, ID_MENUBASE+1, "Echo in Systemconsole");
			::AppendMenu(m_hMenu, MF_STRING, ID_MENUBASE+3, "Echo Enter and Leave messages");
			::AppendMenu(m_hMenu, MF_SEPARATOR, 0, (LPCTSTR)0);
			::AppendMenu(m_hMenu, MF_STRING, ID_MENUBASE+2, "Stop Echo");
			::AppendMenu(hMenu, MF_SEPARATOR, 0, (LPCTSTR)0);
			::AppendMenu(hMenu, MF_POPUP, (UINT_PTR)m_hMenu, "Echo");
		}
	}
}

void CEchoPlugin::OnMenuCommand(UINT nCmd, DWORD dwID, CString strName)
{

	if(dwID){

		switch(nCmd){

			case ID_MENUBASE:
				OnReloadCfg();
				m_dwOut		= dwID;
				m_bSysOut	= FALSE;
				break;
			case ID_MENUBASE+1:
				OnReloadCfg();
				m_dwOut		= dwID;
				m_bSysOut	= TRUE;
				break;
			case ID_MENUBASE+2:
				m_dwOut		= 0;
				m_bSysOut	= FALSE;
				break;
			case ID_MENUBASE+3:
				m_bEnterLeave = !m_bEnterLeave;
		}
	}
}

