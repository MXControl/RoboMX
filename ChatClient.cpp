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
#include "ChatClient.h"
#include "ChatDoc.h"
#include "ChatView.h"
#include "Settings.h"
#include "util.h"
#include ".\chatclient.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CSettings g_sSettings;

#ifdef _DEBUG
extern Hex_Trace(PVOID pBuffer, int nLen);
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
UINT UWM_MESSAGE	= ::RegisterWindowMessage("UWM_MESSAGE-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_ACTION		= ::RegisterWindowMessage("UWM_ACTION-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_TOPIC		= ::RegisterWindowMessage("UWM_TOPIC-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_MOTD		= ::RegisterWindowMessage("UWM_MOTD-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_JOIN		= ::RegisterWindowMessage("UWM_JOIN-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_PART		= ::RegisterWindowMessage("UWM_PART-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_RENAME		= ::RegisterWindowMessage("UWM_RENAME-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_ADDUSER    = ::RegisterWindowMessage("UWM_ADDUSER-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_REDIRECT   = ::RegisterWindowMessage("UWM_REDIRECT-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_UNKNOWN    = ::RegisterWindowMessage("UWM_UNKNOWN-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_SYSTEM     = ::RegisterWindowMessage("UWM_SYSTEM-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_ROOMRENAME = ::RegisterWindowMessage("UWM_ROOMRENAME-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_OPMESSAGE   = ::RegisterWindowMessage("UWM_OPMESSAGE-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_SERVERTYPE = ::RegisterWindowMessage("UWM_SERVERTYPE-229F871A-7B27-44C5-8879-AF881DE94891");
UINT UWM_IPCLIENT = ::RegisterWindowMessage("UWM_IPCLIENT-229F871A-7B27-44C5-8879-AF881DE94891");


CChatClient::CChatClient() :
	m_dwClientIP(16777343),
	m_wClientUDPPort(6257),
	m_dwDWKey(0),
	m_dwUPKey(0),
	m_dwFiles(0),
	m_wLineType(0),
	m_wRoomTCPPort(6699),
	m_pThread(0),
	m_bListen(FALSE),
	m_wServerType(SERVER_UNKNOWN),
	m_bOldJoin(FALSE),
	m_bAbort(FALSE),
	m_strServerString("WinMX"),
	m_strServerVer("3.31 or compatible"),
	m_bServerSet(FALSE)
	, m_bRedirect(FALSE)
	, m_dwLastReconnect(0)
	, m_nTry(0)
	{

}

CChatClient::~CChatClient()
{

}

BOOL CChatClient::Connect()
{

	m_eClose.ResetEvent();
	m_wServerType = SERVER_UNKNOWN;
	m_bAbort  = FALSE;
	m_bListen = FALSE;
	m_pThread = AfxBeginThread(RecvProc, (PVOID)this, THREAD_PRIORITY_NORMAL);

	return TRUE;
}

BOOL CChatClient::Disconnect()
{

	m_bAbort = TRUE;
	if(m_bListen){

		m_bListen = FALSE;
		m_mSocket.Close();
		DWORD n = WaitForSingleObject(m_eClose.m_hObject, 1000);
		if(n == WAIT_TIMEOUT || n == WAIT_FAILED){

			TerminateThread(m_pThread->m_hThread, 0);
		}
		m_pThread = NULL;
	}

	return TRUE;
}

BOOL CChatClient::SendRename(CString strUser, DWORD dwFiles, WORD wLine)
{

	//0x0065][Line-Type:2][IP-Address:4][UDP-Port:2][Shared-Files:4][New-Name:N][00:1]

	m_dwFiles	= dwFiles;
	m_wLineType = wLine;
	m_strUser   = strUser;

	if(m_bListen){

		char buffer[1024];
		ZeroMemory(buffer, 1024);
		
		WORD wLen = Util::FormatMXMessage(0x0065, (char*)&buffer, "WDWDS", 
									  m_wLineType, m_dwClientIP,
									  m_wClientUDPPort, m_dwFiles,
									  (LPCTSTR)m_strUser);

		m_dwUPKey = EncryptMXTCP((BYTE*)buffer, wLen, m_dwUPKey);

		int nSend = m_mSocket.Send(buffer, wLen,5);

		if(nSend == SOCKET_ERROR){

			m_bListen  = FALSE;
			CString strError;
			strError.Format(IDS_ERROR_NETWORK, m_mSocket.GetLastErrorStr());
			WriteMessage(TRUE, strError, g_sSettings.GetRGBErr());
			return FALSE;
		}
	}
	return TRUE;
}


BOOL CChatClient::SendText(CString strText)
{

	if(!m_bListen) return FALSE;
	if(strText.IsEmpty()) return FALSE;

	char buffer[1024];
	ZeroMemory(buffer, 1024);
	WORD wLen = 0;

	if(m_wServerType < SERVER_WINMX353){

		BOOL bAction = FALSE;
		if(strText.Find("/me ", 0) == 0){

			strText = strText.Mid(4);
			bAction = TRUE;
		}
		else if(strText.Find("/action ", 0) == 0){

			strText = strText.Mid(8);
			bAction = TRUE;
		}
		else if(strText.Find("/emote ", 0) == 0){

			strText = strText.Mid(7);
			bAction = TRUE;
		}
		if(strText.IsEmpty()) return TRUE;

		wLen = Util::FormatMXMessage((bAction ? 0x00CA : 0x00C8), (char*)&buffer, "S", (LPCSTR)strText);
	}
	else{

		wLen = Util::FormatMXMessage(0x1450, (char*)&buffer, "S", (LPCSTR)strText);
	}

	m_dwUPKey = EncryptMXTCP((BYTE*)buffer, wLen, m_dwUPKey);
	int nSend = m_mSocket.Send(buffer, wLen, 5);

	if(nSend == SOCKET_ERROR){

		m_bListen  = FALSE;
		CString strError;
		strError.Format(IDS_ERROR_NETWORK, m_mSocket.GetLastErrorStr());
		WriteMessage(TRUE, strError, g_sSettings.GetRGBErr());
	}

	return TRUE;
}

void CChatClient::Ping()
{

	// 0xFDE8
	if(m_bListen){

		char buffer[4];
		ZeroMemory(buffer, 4);
		WORD wType = 0xFDE8;
		memcpy(buffer, &wType, 2);

		m_dwUPKey = EncryptMXTCP((BYTE*)buffer, 4, m_dwUPKey);
		
		if(g_sSettings.GetPing()){

			WriteMessage(FALSE, "PING", RGB(150,150,150));
		}

		int nSend = m_mSocket.Send(buffer, 4,5);

		if(nSend == SOCKET_ERROR){

			m_bListen  = FALSE;
			CString strError;
			strError.Format(IDS_ERROR_NETWORK, m_mSocket.GetLastErrorStr());
			WriteMessage(TRUE, strError, g_sSettings.GetRGBErr());
		}

		if(GetTickCount() - m_dwLastReconnect > 30000){

			m_nTry = 0;
		}
	}
}

// MCMA Pingclient protocoll extension is supported
void CChatClient::SendIPClient(WORD wType)
{

	if(m_bListen){


		char buffer[16];
		ZeroMemory(buffer, 4);
		
		//WORD wType = ;
		//memcpy(buffer, &wType, 2);
		WORD wLen = Util::FormatMXMessage(wType, (char*)&buffer, "B", 0);

		m_dwUPKey = EncryptMXTCP((BYTE*)buffer, wLen, m_dwUPKey);
		
		int nSend = m_mSocket.Send(buffer, wLen, 5);

		if(nSend == SOCKET_ERROR){

			m_bListen  = FALSE;
			CString strError;
			strError.Format(IDS_ERROR_NETWORK, m_mSocket.GetLastErrorStr());
			WriteMessage(TRUE, strError, g_sSettings.GetRGBErr());
		}
	}
}

void CChatClient::SendClientString(void)
{

	if(m_bListen){

		char buffer[1024];
		ZeroMemory(buffer, 1024);
		//WORD wType = 0x9905;
		CString strString = GetApp()->m_strAppTitle;
		if(strString.Find("RoboMX", 0) < 0) strString.Append(" (RoboMX)");
		WORD wLen = Util::FormatMXMessage(0x9905, (char*)&buffer, "SS", strString, VERSION_STRING);
		m_dwUPKey = EncryptMXTCP((BYTE*)buffer, wLen, m_dwUPKey);
		
		int nSend = m_mSocket.Send(buffer, wLen, 5);

		if(nSend == SOCKET_ERROR){

			m_bListen  = FALSE;
			CString strError;
			strError.Format(IDS_ERROR_NETWORK, m_mSocket.GetLastErrorStr());
			WriteMessage(TRUE, strError, g_sSettings.GetRGBErr());
		}
	}
}

BOOL CChatClient::SetRoom(CString strRoom)
{

	// testroom2_0100007F1A2B
	// 0100007F IP
	// 1A2B = Port
	m_strRoom = strRoom;

	int nIndex = m_strRoom.ReverseFind('_')+1;

	if(nIndex <= 0) return FALSE;

	CString strTmp = m_strRoom.Mid(nIndex, 8);
	
	int nA = 0, nB = 0, nC = 0, nD = 0;
	
	nA = Util::axtoi((LPSTR)(LPCSTR)strTmp.Mid(0,2), 2);
	nB = Util::axtoi((LPSTR)(LPCSTR)strTmp.Mid(2,2), 2);
	nC = Util::axtoi((LPSTR)(LPCSTR)strTmp.Mid(4,2), 2);
	nD = Util::axtoi((LPSTR)(LPCSTR)strTmp.Mid(6,2), 2);

	m_strRoomIP.Format("%d.%d.%d.%d", nD, nC, nB, nA);

	m_wRoomTCPPort = Util::axtoi((LPSTR)(LPCSTR)strRoom.Mid(m_strRoom.GetLength()-4), 4);

	m_dwClientIP = g_sSettings.GetNodeIP();

	nA = ((m_dwClientIP >> 24) & 0xff);
	nB = ((m_dwClientIP >> 16) & 0xff);
	nC = ((m_dwClientIP >> 8) & 0xff);
	nD = (m_dwClientIP & 0xff);

	m_dwClientIP = MAKEIPADDRESS(nA, nB, nC, nD);

	m_wClientUDPPort = (WORD)g_sSettings.GetNodePort();
	
	if(m_wRoomTCPPort < 1024) return FALSE;
	return TRUE;
}


void CChatClient::DecodeCommand(WORD wType, WORD wLen, char *cmd)
{

	if((wType == 0x00C9) || (wType == 0x00CB)){

		LPSTR lpName = 0, lpText = 0;
		if(Util::ScanMessage(cmd, wLen, "SS", &lpName, &lpText) != 2){

			return;
		}
		::SendMessage(m_hView, wType == 0x00C9 ? UWM_MESSAGE : UWM_ACTION, (WPARAM)lpName, (LPARAM)lpText);
	}
	else if((wType == 0x006F) || (wType == 0x0072)){

		DecodeAddUser(wType, wLen, cmd);
	}
	else if((wType == 0x006E) || (wType == 0x0071) || (wType == 0x0075)){

		DecodeJoin(wType, wLen, cmd);
	}
	else if(wType == 0x012C){  // topic

		::SendMessage(m_hView, UWM_TOPIC, 0, (LPARAM)cmd);
	}
	else if(wType == 0x0078){ // motd

		::SendMessage(m_hView, UWM_MOTD, 0, (LPARAM)cmd);

	}
	else if(wType == 0x0073){ // user part

		LPSTR lpUser = 0;
		ClientJoin *pUser = new ClientJoin();
		if(Util::ScanMessage(cmd, wLen, "SDW", &lpUser, &pUser->dwNodeIP, &pUser->wNodePort) != 3){

			CString strMsg;
			strMsg.Format("Error: Server send part-packet (0x0073) with invalid argument count");
			WriteMessage(TRUE, strMsg, g_sSettings.GetRGBErr());
			return;
		}
		pUser->strName = lpUser;
		::SendMessage(m_hView, UWM_PART, 0, (LPARAM)pUser);
		delete pUser;
	}
	else if((wType == 0x0070) || (wType == 0x0074)){ // user rename

		DecodeRename(wType, wLen, cmd);
	}
	else if((wType == 0x00D2) || (wType == 0x00D3)){

		::SendMessage(m_hView, UWM_OPMESSAGE, (WPARAM)wType, (LPARAM)cmd);
	}
	else if(wType == 0x012D){  // roomanme change

		CString strBaseName = cmd, strSuffix;		

		int n = m_strRoom.ReverseFind('_');
		if(n>0){

			strSuffix = m_strRoom.Mid(n);
		}
		m_strRoom = strBaseName + strSuffix;
		::SendMessage(m_hView, UWM_ROOMRENAME, 0, 0);
	}
	else if(wType == 0x0190){  // redirect

		::SendMessage(m_hView, UWM_REDIRECT, 0, (LPARAM)cmd);
	}
	else if(wType == 0x0068){  // indicate WinMX > 3.5x

		WriteMessage(FALSE, IDS_SERVER_WINMX353, g_sSettings.GetRGBOp());

		if(m_bOldJoin){

			WriteMessage(FALSE, IDS_WINMX353_WRONGJOIN_WARNING, g_sSettings.GetRGBPend());
		}

		if(m_wServerType < SERVER_WINMX353){

			m_wServerType	  = SERVER_WINMX353;
			m_strServerString = "WinMX";
			m_strServerVer	  = "3.5x/compatible";
		}
		::SendMessage(m_hView, UWM_SERVERTYPE, SERVER_WINMX353, 0);
	}
	else if(wType == 0x0069){

		m_wServerType = SERVER_MCMA;
		m_strServerString = "WinMX/MCMA";
		m_strServerVer    = "3.53/compatible";
		::SendMessage(m_hView, UWM_SERVERTYPE, SERVER_MCMA, 0);
	}
	else if(wType == 0x00C8){   // rcms garbage

		if(m_wServerType == SERVER_WINMX353){

			m_wServerType = SERVER_RCMS353;
			m_strServerString = "RCMS on WinMX > 3.5x";
			m_strServerVer	  = "N/A";
			WriteMessage(FALSE, IDS_SERVER_RCMS, g_sSettings.GetRGBOp());
			::SendMessage(m_hView, UWM_SERVERTYPE, SERVER_RCMS353, 0);
		}
		else if((m_wServerType != SERVER_RCMS) && (m_wServerType != SERVER_RCMS353)){

			m_wServerType = SERVER_RCMS;
			m_strServerString = "RCMS";
			m_strServerVer	  = "N/A";
			WriteMessage(FALSE, IDS_SERVER_RCMS, g_sSettings.GetRGBOp());
			::SendMessage(m_hView, UWM_SERVERTYPE, SERVER_RCMS, 0);
		}
	}
	else if(wType == 0x9900){ // ip client question

		SendIPClient(0x9901);
	}
	else if(wType == 0x9902){ // ip client package

		DecodeIP(wType, wLen, cmd);
	}
	else if(wType == 0x9904){ // colorstring

		WriteMessage(FALSE, IDS_SERVER_MCMA, g_sSettings.GetRGBOp());
		m_strColorString = cmd;
	}
	else if(wType == 0x9905){

		LPSTR lpServer = 0, lpVersion = 0;
		if(Util::ScanMessage(cmd, wLen, "SS", &lpServer, &lpVersion) == 2){

			m_strServerString	= lpServer;
			m_strServerVer		= lpVersion;

			if(m_strServerString == "MXTools Chat Server"){

				m_wServerType = SERVER_MCS;
			}
			else if(m_strServerString.Find("roboServ", 0) >= 0){

				m_wServerType = SERVER_ROSE;
			}
			else if(m_strServerString.Find("Ouka") >= 0){

				m_wServerType = SERVER_OUKA;
			}
			else if(m_strServerString.Find("WCS") >= 0){

				m_wServerType = SERVER_WCS;
			}
			else if(m_strServerString.Find("FXS") >= 0){

				m_wServerType = SERVER_FXSERV;
			}
			
			::SendMessage(m_hView, UWM_SERVERTYPE, m_wServerType, 0);
		}
		CString strServer;
		strServer.Format(IDS_SERVERNEW, m_strServerString, m_strServerVer);
		WriteMessage(FALSE, strServer, g_sSettings.GetRGBOp());
		m_bServerSet = TRUE;
		SendClientString();
	}
}

int CChatClient::DecodeIP(WORD wType, WORD wLen, char* buffer)
{

	ClientJoin* pIP = new ClientJoin();

	LPSTR lpName = 0, lpIP = 0, lpHost = 0;
	BYTE  btLevel = 0;
	WORD wFiles = 0;
	//[UserName:N][PrimaryIP:4][PrimaryPort:2][Files:2][Connection:2][Rank:1][IP:N][[HOST:N] this is roboserve only]

	if(Util::ScanMessage(buffer, wLen, "SDWWWBSS", &lpName, &pIP->dwNodeIP, &pIP->wNodePort,
													   &wFiles, &pIP->wLine, &btLevel,
													   &lpIP, &lpHost) < 7){

		TRACE("IP Client packet has too few arguments.\n");
		return 0;
	}

	pIP->dwFiles = wFiles;
	pIP->dwSrcIP = CMySocket::GetIP(lpIP);

	if(lpHost == 0){

		pIP->strHost = CMySocket::GetHostName(lpIP);
	}
	else{

		pIP->strHost = lpHost;
	}

	pIP->strName = lpName;

	if((btLevel >= 0) && (btLevel <= 2)){

		pIP->wLevel  = (WORD)btLevel;
	}
	else{

		pIP->wLevel  = 0;
	}

	::SendMessage(m_hView, UWM_IPCLIENT, 0, (LPARAM)pIP);
	delete pIP; 

	return 1;
}

int CChatClient::DecodeAddUser(WORD wType, WORD wLen, char* buffer)
{

	ClientJoin* pUser = new ClientJoin();

	LPSTR lpUser = 0;

	// 0x006F][Username:N][00:1][IP-Address:4][UDP-Port:2][Line-Type:2][Shared-Files:4]
	if(wType == 0x072){ // winmx 3.5x

		if(Util::ScanMessage(buffer, wLen, "SDWWDW", &lpUser, &pUser->dwNodeIP, 
								&pUser->wNodePort, &pUser->wLine, &pUser->dwFiles, 
								  &pUser->wLevel) < 6){

			return 0;
		}
	}
	else{

		if(Util::ScanMessage(buffer, wLen, "SDWWD", &lpUser, &pUser->dwNodeIP, 
								&pUser->wNodePort, &pUser->wLine, 
								  &pUser->dwFiles) != 5){

			return 0;
		}
		pUser->wLevel = 0;
	}

	pUser->strName = lpUser;

	::SendMessage(m_hView, UWM_ADDUSER, 0,(LPARAM)pUser);
	delete pUser;

	return 1;
}
    
int CChatClient::DecodeJoin(WORD wType, WORD wLen, char* buffer)
{

	ClientJoin* pJoin = new ClientJoin();
	
	LPSTR lpUser  = 0;
	BYTE  btDummy = 0;

	if(wType == 0x071){ // winmx 3.52 and later (no-ip display)

		if(Util::ScanMessage(buffer, wLen, "SDWWDB", &lpUser, &pJoin->dwNodeIP, &pJoin->wNodePort, &pJoin->wLine, &pJoin->dwFiles, &btDummy) < 6){

			return 0;
		}
	}
	else if(wType == 0x075){ // winmx 3.52 and later (ip display)

		if(Util::ScanMessage(buffer, wLen, "SDWWDBD", &lpUser, &pJoin->dwNodeIP, &pJoin->wNodePort, &pJoin->wLine, &pJoin->dwFiles, &btDummy, &pJoin->dwSrcIP) < 6){

			return 0;
		}
		pJoin->strHost = CMySocket::GetHostName(Util::FormatIP(pJoin->dwSrcIP));
	}
	else{  // old winmx

		if(Util::ScanMessage(buffer, wLen, "SDWWD", &lpUser, &pJoin->dwNodeIP, &pJoin->wNodePort, &pJoin->wLine, &pJoin->dwFiles) != 5){

			return 0;
		}
	}

	pJoin->strName = lpUser;
	if((btDummy >= 0) && (btDummy <= 2))
		pJoin->wLevel  = (WORD)btDummy;
	else
		pJoin->wLevel  = 0;

	::SendMessage(m_hView, UWM_JOIN, 0, (LPARAM)pJoin);
	delete pJoin; 

	return 1;
}

int CChatClient::DecodeRename(WORD wType, WORD wLen, char* buffer)
{

	LPSTR oldName = 0, newName = 0;
	ClientJoin* pUser = new ClientJoin();

	if(wType == 0x0074){

		// 0x0074	353
		//[Name:N][IP-Address:4][UDP-Port:2]	// old
		//[Name:N][IP-Address:4][UDP-Port:2][Line-Type:2][Shared-Files:4][Rank:1] // new
		if(Util::ScanMessage(buffer, wLen, "SDWSDWWDB", &oldName, &pUser->dwOldNodeIP, &pUser->wOldNodePort,
								&newName, &pUser->dwNodeIP, &pUser->wNodePort, 
								&pUser->wLine, &pUser->dwFiles, (BYTE*)&pUser->wLevel)< 9){

			return 0;
		}
	}
	else{

		//0x0070]	331
		//[Name:N][00:1][IP-Address:4][UDP-Port:2] // old
		//[Name:N][00:1][IP-Address:4][UDP-Port:2][Line-Type:2][Shared-Files:4] // new
		if(Util::ScanMessage(buffer, wLen, "SDWSDWWD", &oldName, &pUser->dwOldNodeIP, &pUser->wOldNodePort,
							 &newName, &pUser->dwNodeIP, &pUser->wNodePort,
							 &pUser->wLine, &pUser->dwFiles) < 8){

			 return 0;
		}
	}

	pUser->strOldName = oldName;
	pUser->strName    = newName;
	::SendMessage(m_hView, UWM_RENAME, 0, (LPARAM)pUser);
	delete pUser;
	return 1;
}

void CChatClient::WriteMessage(BOOL bIsError, LPCTSTR lpszMsg, COLORREF rColor)
{

	if(!m_hView) return;
	if(!::IsWindow(m_hView)) return;

	EXT_MSG_STRUCT msg;
	msg.wParam  = (WPARAM)bIsError;
	msg.lpszMsg = lpszMsg;
	msg.lParam  = 0;
	msg.lpszParam = 0;
	::SendMessage(m_hView, UWM_SYSTEM, (WPARAM)rColor, (LPARAM)&msg);
}

void CChatClient::WriteMessage(BOOL bIsError, UINT nID, COLORREF rColor)
{

	CString strText;
	strText.LoadString(nID);
	WriteMessage(bIsError, strText, rColor);
}

#define BUFFER_SIZE 10000

UINT CChatClient::RecvProc(PVOID pParam)
{

	CChatClient* pClient = (CChatClient*)pParam;
	ASSERT(pClient);

	pClient->m_eClose.ResetEvent();

	int nTries = g_sSettings.GetRetry() ? g_sSettings.GetRetries() : 1;

	for(pClient->m_nTry = 0; (pClient->m_nTry <= nTries) && !pClient->m_bAbort;  pClient->m_nTry++){

		if(pClient->m_nTry > 0){

			CString strOut;
			strOut.Format(IDS_CONNECTION_RETRY, pClient->m_nTry, nTries);
			pClient->WriteMessage(FALSE, strOut, g_sSettings.GetRGBPend());
		}
		if(pClient->Connectinternal()){

			pClient->Listeninternal();
		}
		else{

			pClient->m_mSocket.Close();
		}
		pClient->m_bServerSet = FALSE;
	}
	
	TRACE("Setting Event and exiting thread\n");
	pClient->m_bListen = FALSE;
	pClient->m_eClose.SetEvent();
	TRACE("Bye\n");
	return 0;
}

BOOL CChatClient::Connectinternal(void)
{

	m_dwLastReconnect = GetTickCount();
	int nRecv = 0;
	char buffer[BUFFER_SIZE] = {'\0'};
	WORD	wType = 0;
	WORD	wLen  = 0;

	if(m_mSocket.m_sSocket != INVALID_SOCKET){

		m_mSocket.Close();
	}

	if(!m_mSocket.Connect(m_strRoomIP, m_wRoomTCPPort, IPPROTO_TCP)){

		CString strError;
		strError.Format(IDS_ERROR_CONNECTING, m_strRoomIP, m_mSocket.GetLastErrorStr());
		WriteMessage(TRUE, strError, g_sSettings.GetRGBErr());
		return FALSE;
	}

	if(m_bAbort) return FALSE;

	WriteMessage(FALSE, IDS_HANDSHAKE_START, g_sSettings.GetRGBPend());
	m_mSocket.Recv(buffer, 1, 5);
	
	if(buffer[0] != 0x31){
		
		WriteMessage(TRUE, IDS_ERROR_LOGIN_UNKNOWN,  g_sSettings.GetRGBErr());
		return FALSE;
	}

	if(m_bAbort) return FALSE;
	CreateCryptKeyID(0x57, (BYTE *)buffer);

	// Send UP Key Block
	if(m_mSocket.Send(buffer, 16, 5) != 16){
		
		WriteMessage(TRUE, IDS_ERROR_LOGIN_KEY,  g_sSettings.GetRGBErr());
		return FALSE;
	}
	// Recv DW Key Block
	if(m_bAbort) return FALSE;
	if(m_mSocket.Recv(buffer, 16, 5) != 16){
		
		WriteMessage(TRUE, IDS_ERROR_LOGIN_KEY,  g_sSettings.GetRGBErr());
		return FALSE;
	}

	// Check Cryptkey
	if(m_bAbort) return FALSE;
	
	if(GetCryptKeyID((BYTE*)&buffer) != 0x58){
	
		// this was not crypt key from Chatserver :-(
		WriteMessage(TRUE, IDS_ERROR_NO_CHATSERVER,  g_sSettings.GetRGBErr());
		return FALSE;
	}

	// Get the keys
	GetCryptKey((BYTE *)buffer, &m_dwUPKey, &m_dwDWKey);

	// Prepare login buffer...
	ZeroMemory(buffer, BUFFER_SIZE);

	// Send new pre-login packet
	// ED 13 01 00 31	wLen = 5;
	if(m_bAbort) return FALSE;

	if(!m_bOldJoin){

		wLen = Util::FormatMXMessage(0x13ED, buffer, "B", 0x31);

		m_dwUPKey = EncryptMXTCP((BYTE*)buffer, wLen, m_dwUPKey);
		
		if(m_mSocket.Send(buffer, wLen, 5) != wLen){

			CString strError;
			strError.Format(IDS_ERROR_NETWORK, m_mSocket.GetLastErrorStr());
			WriteMessage(TRUE, strError, g_sSettings.GetRGBErr());
			return FALSE;
		}
	}

	if(m_bAbort) return FALSE;

	//Login-Request: (Client)
	//0x0064][00:1][RoomName:N][LineType:2][Node-IP-Address:4][Node-Port:2][SharedFiles:4][Username:N][00:1]
	wLen = Util::FormatMXMessage(0x0064, (char*)&buffer, "SWDWDS", 
					(LPCTSTR)m_strRoom, 
					m_wLineType,
					m_dwClientIP,
					m_wClientUDPPort,
					m_dwFiles, 
					(LPCTSTR)m_strUser);

	m_dwUPKey = EncryptMXTCP((BYTE*)buffer, wLen, m_dwUPKey);

	if(m_bAbort) return FALSE;
	if(m_mSocket.Send(buffer, wLen, 5) != wLen){

		CString strError;
		strError.Format(IDS_ERROR_NETWORK, m_mSocket.GetLastErrorStr());
		WriteMessage(TRUE, strError, g_sSettings.GetRGBErr());
		return FALSE;
	}

	if(m_bAbort) return FALSE;
	ZeroMemory(buffer, BUFFER_SIZE);
	// recieve login answer
	if(m_mSocket.Recv(buffer, 5, 5) != 5){

		CString strError;
		strError.Format(IDS_ERROR_NETWORK, m_mSocket.GetLastErrorStr());
		WriteMessage(TRUE, strError, g_sSettings.GetRGBErr());
		return FALSE;
	}
	
	if(m_bAbort) return FALSE;
	m_dwDWKey = DecryptMXTCP((BYTE *)buffer, 5, m_dwDWKey);

	if((*(WORD*)buffer) != 0x66){

		WriteMessage(TRUE, IDS_ERROR_LOGIN_REJECTED, RGB(150,0,0));
		return FALSE;
	}

 	m_wNumUsers = *(WORD*)(buffer+2);
	WriteMessage(FALSE, IDS_LOGIN_OK, RGB(0, 120, 0));
	m_bListen = TRUE;
	return TRUE;
}

BOOL CChatClient::Listeninternal(void)
{

	int nRecv = 0;
	char buffer[BUFFER_SIZE] = {'\0'};
	WORD	wType = 0;
	WORD	wLen  = 0;

	m_bListen = TRUE;

	ZeroMemory(buffer, BUFFER_SIZE);
	
	// main recv loop
	while(m_bListen) {


		nRecv = m_mSocket.Recv(buffer, 4, 15);
		if(nRecv == SOCKET_ERROR){

			if((m_mSocket.GetLastError() != SOCK_TIMEOUT) && m_bListen){

				m_bListen  = FALSE;
				CString strError;
				strError.Format(IDS_ERROR_NETWORK, m_mSocket.GetLastErrorStr());
				WriteMessage(TRUE, strError, g_sSettings.GetRGBErr());
				TRACE("1) %s\n", strError);
				break;
			}
			else continue;
		}
		

		if(nRecv != 4){
			
			Sleep(100);
			CString strError;
			strError.Format(IDS_ERROR_FIXME, buffer);
			WriteMessage(TRUE, strError, RGB(150, 150, 150));
			continue;
		}
		
		m_dwDWKey = DecryptMXTCP((BYTE*)(buffer), 4, m_dwDWKey);

		memcpy(&wType, buffer, 2);
		wLen = *(WORD*)(buffer+2);

		if(wType == 0xFDE8){

			if(g_sSettings.GetPing())
				WriteMessage(FALSE, "PONG", RGB(150,150,150));
			
		}

		wLen = *(WORD*)(buffer+2);
		
		if(wLen <= 0) continue;

		if(!m_bListen) break;

		if(wLen > BUFFER_SIZE){
			
			int nTotal = 0;
			while(wLen - nTotal > 0){

				nRecv = m_mSocket.Recv(buffer, wLen, 20);
				if(nRecv){

					nTotal+=nRecv;
				}
				else break;
			}
			continue;
		}

		nRecv = m_mSocket.Recv(buffer, wLen, 20);

		if(nRecv == SOCKET_ERROR){

			if(m_mSocket.GetLastError() != SOCK_TIMEOUT){

				m_bListen  = FALSE;
				CString strError;
				strError.Format(IDS_ERROR_NETWORK, m_mSocket.GetLastErrorStr());
				WriteMessage(TRUE, strError, g_sSettings.GetRGBErr());
				TRACE("2) %s\n", strError);
				break;
			}
		}
		if(nRecv != wLen){
			
			m_dwDWKey = DecryptMXTCP((BYTE*)(buffer), nRecv, m_dwDWKey);
			continue;
		}

		m_dwDWKey = DecryptMXTCP((BYTE*)(buffer), wLen, m_dwDWKey);

		DecodeCommand(wType, wLen, buffer);
		ZeroMemory(buffer,BUFFER_SIZE);

	}

	return m_bListen;
}
