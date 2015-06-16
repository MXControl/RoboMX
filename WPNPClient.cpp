/*
** The original implementation of this class was written by
** Nushi@2SEN.
**
** Adaption, Modification, Extension & Redesign for RoboMX
** by (c) Bender
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
#include ".\wpnpclient.h"
#include "settings.h"
#include "Tokenizer.h"
#include "Ini.h"

extern CSettings g_sSettings;

#define WM_PM WM_APP+20
UINT UWM_WPNNOTIFY = ::RegisterWindowMessage("UWM_WPNNOTIFY-{AFA648D5-3366-4a47-905F-F53ABE3D37D6}");

CWPNPClient::CWPNPClient(void)   :
	m_wTransferPort(0),
	m_wLineType(0x00),  
	m_wParentTCPPort(0),
	m_wParentUDPPort(0),
	m_dwParentIP(0),
	m_dwParentIP_External(0),	
	m_bAutoParent(TRUE),
	m_strWPNUserName(""),
	m_strBaseUserName(""),
	m_bKeepLogin(TRUE),
	m_bIsLoggedIn(FALSE),
	m_bIsWPNClientProc(FALSE),
	m_bIsWPNSendProc(FALSE),
	m_hSendEvent(NULL),
	m_dwSendType(0),
	m_dwSendSize(0),
	m_hWnd(NULL),
	m_dwUPKey(0),
	m_dwDWKey(0)
{
 

	ZeroMemory(m_SendArray, sizeof(m_SendArray));

	::InitializeCriticalSection(&m_ClientLock);
}

CWPNPClient::~CWPNPClient(void)
{

	Disconnect();
	
	// Delete Critical Section
	::DeleteCriticalSection(&m_ClientLock);
}

void CWPNPClient::AddListener(HWND hWnd)
{

	if(m_hWnd == NULL){

		m_hWnd = hWnd;
	}
}

void CWPNPClient::RemoveListener(HWND hWnd)
{

	if(m_hWnd == hWnd){

		m_hWnd = NULL;
	}
}

int CWPNPClient::ConnectWPN(PARENTNODEINFO *pNodeInfo, LPCTSTR lpszUserName, WORD wLineType, WORD wTransferPort)
{

	// Check Parameters
	if(!lpszUserName || m_bIsWPNClientProc || m_bIsWPNSendProc) return 0;
	if(!lstrlen(lpszUserName)) return 0;

	::EnterCriticalSection(&m_ClientLock);
	{

		// Save Parameter
		m_bKeepLogin		= TRUE;
		m_bAutoParent		= (pNodeInfo)? FALSE : TRUE;
		m_strBaseUserName	= lpszUserName;
		m_strWPNUserName	= "";
		m_wLineType			= wLineType;
		m_wTransferPort		= wTransferPort;
		
		if(pNodeInfo){

			m_dwParentIP = pNodeInfo->dwNodeIP;
			m_wParentTCPPort = pNodeInfo->wTCPPort;
			m_wParentUDPPort = pNodeInfo->wUDPPort;

			CString strNode, strPort;
			CString strHost = g_sSettings.GetNodeExternal();
			if(strHost.IsEmpty()){
				
				m_dwParentIP_External = m_dwParentIP;
			}
			else{

				m_dwParentIP_External = CMySocket::GetIP(strNode);
			}
		}

		m_bIsWPNClientProc = FALSE;
		CWinThread *pWPNClientThread = AfxBeginThread(CWPNPClient::WPNClientProc, this);
		if(!pWPNClientThread){

			::LeaveCriticalSection(&m_ClientLock);
			return 0;
		}

		while(!m_bIsWPNClientProc){
			
			Sleep(1);
		}

	}
	::LeaveCriticalSection(&m_ClientLock);

	return 1;
}

BOOL CWPNPClient::GetParentNode(PARENTNODEINFO *pNodeInfo)
{

	if(!pNodeInfo) return FALSE;

	char buffer[512];
	ZeroMemory(buffer, 512);

	BYTE *pNodeBuffer = (BYTE *)buffer + sizeof(buffer) / 2;

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());
	int nCount = ini.GetValue(_T("PeerCache"), _T("Count"), 0);
	CString strTmp, str;

	CStringArray strHosts;
	for(int n = 0; n < nCount; n++){

		strTmp.Format(_T("Server_%03d"), n);
		str = ini.GetValue(_T("PeerCache"), strTmp, _T(""));

		if(str.IsEmpty()) continue;

		strHosts.Add(str);
	}
	if(strHosts.IsEmpty()){

		return FALSE;
	}

	WORD wPorts[6];
	wPorts[0] = 7950;
	wPorts[1] = 7951;
	wPorts[2] = 7952;
	wPorts[3] = 3871;
	wPorts[4] = 4127;
	wPorts[5] = 3615;

	PARENTNODEINFO pnFreeNode;
	int i = 0, j = 0, nFree = 0;

	for(WORD wIndex = 0 ; wIndex < nCount && m_bKeepLogin; wIndex++){
	
		Close();
		for(WORD wPort = 0; wPort < 6; wPort++){

			if(!Connect(strHosts[wIndex], wPorts[wPort], 5)){

				// We failed to connect. use alternate port
				continue;
			}
		}

		if(Recv(buffer, 1, 5) != 1){
			
			// error, try different peercache server
			continue;
		}

		if(buffer[0] != 0x0038){
			
			// error, try different peercache server
			continue;
		}

		if(Recv(buffer, 16, 5) != 16){
			
			// error, try different peercache server
			continue;
		}
		
		if(GetCryptKeyID((BYTE*)&buffer) != 0x54){
			
			// error, try different peercache server
			continue;
		}

		// Get free nodes
		if(Recv(buffer, 132, 5) != 132){
			
			// error, try different peercache server
			continue;
		}

		Close();

		DecryptFrontCode((BYTE*)buffer, pNodeBuffer);
		memcpy(pNodeInfo, pNodeBuffer, 120);

		// Sort by Free Secondary Num
		for(j = 0 ; j < 9 ; j++){

			nFree = j;

			for(i = j + 1 ; i < 10 ; i++){

				if(pNodeInfo[i].bFreeSec > pNodeInfo[nFree].bFreeSec || 
					(pNodeInfo[i].bFreeSec == pNodeInfo[nFree].bFreeSec && 
						pNodeInfo[i].bFreePri < pNodeInfo[nFree].bFreePri)){
					
					// More free secondary slots and less free primary slots are better
					nFree = i;
				}
			}

			pnFreeNode = pNodeInfo[j];
			pNodeInfo[j] = pNodeInfo[nFree];
			pNodeInfo[nFree] = pnFreeNode;
		}

		// Retry if No Free Parent Node
		if(!pNodeInfo[0].bFreeSec){

			// error, try different peercache server
			continue;
		}

		return TRUE;
	}

	return FALSE;
}

BOOL CWPNPClient::LoginParent(PARENTNODEINFO *pNodeInfo, LPCTSTR lpszBaseName, CString &strWPNUserName, WORD wLineType, WORD wTransferPort)
{


	m_bIsLoggedIn = FALSE;

	if(!lpszBaseName || !pNodeInfo) return FALSE;
	if(!lstrlen(lpszBaseName)) return FALSE;

	int nNameLen = lstrlen(lpszBaseName);
	char buffer[1024] = {'\0'};

	if(!Connect(pNodeInfo->dwNodeIP, pNodeInfo->wTCPPort, 5)) return FALSE;

	if(Recv(buffer, 1, 5) != 1) return FALSE;

	if(buffer[0] != 0x31) return FALSE;

	// Send UP Key Block
	CreateCryptKeyID(0x52, (BYTE *)buffer);

	if(Send(buffer, 16, 5) != 16){
		
		return FALSE;
	}

	// Recv DW Key Block
	if(Recv(buffer, 16, 5) != 16){
		
		return FALSE;
	}

	if(GetCryptKeyID((BYTE*)buffer) != 0x53){

		TRACE("Invalid cipher id :-(\n");
		return FALSE;
	}

	GetCryptKey((BYTE *)buffer, &m_dwUPKey, &m_dwDWKey);

	// Send Login Request

	nNameLen = Util::FormatMXMessage(0x03E9, buffer, "SWW", lpszBaseName, wLineType, wTransferPort);

	m_dwUPKey = EncryptMXTCP((BYTE*)buffer, nNameLen, m_dwUPKey);

	if(Send(buffer, nNameLen, 5) != nNameLen){

		TRACE("Parent rejected login :-(\n");
		return FALSE;
	}

	// Recv User ID
	if(Recv(buffer, 6, 5) != 6){
		
		return FALSE;
	}

	m_dwDWKey = DecryptMXTCP((BYTE*)buffer, 6, m_dwDWKey);

	if((*(WORD*)buffer != 0x0460) || (*(WORD*)(buffer + 2) != 2)){
		
		return FALSE;
	}

	// Make User Name
	m_wID = *(WORD*)(buffer + 4);
	m_strWPNUserName.Format("%s_%05u", lpszBaseName, m_wID);

	// Save Parent Node Information
	m_wTransferPort  = wTransferPort;
	m_wLineType      = wLineType;
	m_wParentTCPPort = pNodeInfo->wTCPPort;
	m_wParentUDPPort = pNodeInfo->wUDPPort;
	m_dwParentIP	 = pNodeInfo->dwNodeIP;

	g_sSettings.SetNodeIP((g_sSettings.GetForcePrimary() ? m_dwParentIP_External : m_dwParentIP));
	g_sSettings.SetNodePort(m_wParentUDPPort);

	SendNotify(WPN_CLIENTID,  m_wID);

	return TRUE;
}

UINT CWPNPClient::WPNClientProc(LPVOID lParam)
{

	CWPNPClient *pClient = (CWPNPClient*)lParam;

	int i = 0, nRef = 0;
	BOOL bIsFirst = TRUE;
	char buffer[MAXRECVMESSAGESIZE] = {'\0'};
	WORD wMsgLen = 0;
	WORD wMsgType = 0;

	PARENTNODEINFO NodeInfo[10];
	ZeroMemory(NodeInfo, sizeof(NodeInfo));


	//srand(GetTickCount());

	// Begin Send Thread
	CWinThread *pWPNSendThread = AfxBeginThread(CWPNPClient::WPNSendProc, pClient);
	if(!pWPNSendThread){

		// End Recv Thread
		pClient->m_bKeepLogin = FALSE;
		pClient->m_bIsWPNClientProc = TRUE;
		return 0;
	}

	// Wait for Send Thread Start
	while(!pClient->m_bIsWPNSendProc)Sleep(1);
	pClient->m_bIsWPNClientProc = TRUE;

	pClient->SendNotify(WPN_WPNCONNECTSTART, 0);

	while(pClient->m_bKeepLogin){
		if(pClient->m_bAutoParent){

			if(!bIsFirst)pClient->SendNotify(WPN_CHANGEPARENTSTART, 0);

			while(pClient->m_bKeepLogin){

				if(!pClient->GetParentNode(NodeInfo)){

					if(pClient->m_bKeepLogin)pClient->SendNotify(WPN_ERROR, WPN_ERROR_GETNODE);
					Sleep(5000);
					continue;
				}

				// Try Login Parent
				for(i = 0 ; i < 10 && pClient->m_bKeepLogin ; i++){

					if(pClient->LoginParent(&NodeInfo[i], pClient->m_strBaseUserName, pClient->m_strWPNUserName, pClient->m_wLineType, pClient->m_wTransferPort))break;
					pClient->Close();
				}
	
				// Judge Login Success
				if(i < 10 && pClient->m_bKeepLogin){

					pClient->m_bIsLoggedIn = TRUE;
					break;
				}
				else{

					if(pClient->m_bKeepLogin){

						pClient->SendNotify(WPN_ERROR, WPN_ERROR_LOGIN);
						continue;
					}
					else break;
				}
			}
			if(!pClient->m_bKeepLogin) break;
		}
		else{

			// Manual Parent Mode
			NodeInfo[0].dwNodeIP = pClient->m_dwParentIP;
			NodeInfo[0].wTCPPort = pClient->m_wParentTCPPort;
			NodeInfo[0].wUDPPort = pClient->m_wParentUDPPort;

			// Notify (Login Start)
			if(!bIsFirst)pClient->SendNotify(WPN_WPNCONNECTSTART, 0);

			while(pClient->m_bKeepLogin){

				if(!pClient->LoginParent(&NodeInfo[0], pClient->m_strBaseUserName, pClient->m_strWPNUserName, pClient->m_wLineType, pClient->m_wTransferPort)){

					pClient->Close();

					if(pClient->m_bKeepLogin){

						pClient->SendNotify(WPN_ERROR, WPN_ERROR_LOGIN);
						Sleep(5000);
						continue;
					}
					else{
						
						break;
					}
				}
				else{

					pClient->m_bIsLoggedIn = TRUE;
					break;
				}
			}
			
			if(!pClient->m_bIsLoggedIn){
				
				continue;
			}
		}

		// Clear Ringbuffer
		pClient->ClearSendArray();
		
		// Notify (Login Complete)
		if(pClient->m_bAutoParent){

			pClient->SendNotify((bIsFirst)? WPN_WPNCONNECTED : WPN_CHANGEPARENTCOMP, 0);
		}
		else{
			
			pClient->SendNotify(WPN_WPNCONNECTED, 0);
		}
		bIsFirst = FALSE;

		// WPNP Message Recv Loop
		while(pClient->m_bKeepLogin){

			// Recv Header
			nRef = pClient->Recv(buffer, 4);
			if(nRef != 4){

				if(pClient->GetLastError() == SOCK_TIMEOUT){

					continue;
				}
				else break;
			}

			// Decrypt Header
			pClient->m_dwDWKey = DecryptMXTCP((BYTE*)buffer, 4, pClient->m_dwDWKey);
			wMsgType = *(WORD *)buffer;
			wMsgLen = *(WORD *)(buffer + 2);
			if(!wMsgLen){
				
				continue;
			}
			else if(wMsgLen >= sizeof(buffer)){
				
				// catch invalid packets
				// thanks KM and nushi for not checking their stuff
				// *rolleyes*
				int nRecv = 0, nTotal = 0;
				while(wMsgLen - nTotal > 0){

					nRecv += pClient->Recv(buffer, MAXRECVMESSAGESIZE-1, 5);
					if(nRecv){

						nTotal+=nRecv;
						pClient->m_dwDWKey = DecryptMXTCP((BYTE*)buffer, nRecv, pClient->m_dwDWKey);
					}
					else break;
				}
				continue;
			}
			
			// Recv Data
			if(pClient->Recv(buffer, wMsgLen, 5) == wMsgLen){

				// Decrypt Data
				pClient->m_dwDWKey = DecryptMXTCP((BYTE*)buffer, wMsgLen, pClient->m_dwDWKey);
			
				// Analysis Message
				pClient->ProcWPNPMessage(wMsgType, wMsgLen, buffer);
				
				// Clear Buffer
				ZeroMemory(buffer, sizeof(buffer));
			}
			else break;
		}

		// Disconnect
		pClient->Close();
		pClient->m_bIsLoggedIn = FALSE;
		::SetEvent(pClient->m_hSendEvent);
		pClient->SendNotify(WPN_PARENTDISCONNECTED, 0);
	}
	
	// End Send Thread
	::SetEvent(pClient->m_hSendEvent);
	while(pClient->m_bIsWPNSendProc)Sleep(1);

	pClient->m_strWPNUserName = "";

	// Thread End
	pClient->SendNotify(WPN_WPNDISCONNECTED, 0);
	pClient->m_bIsWPNClientProc = FALSE;

	return 0;
}

UINT CWPNPClient::WPNSendProc(LPVOID lParam)
{

	CWPNPClient *pClient = (CWPNPClient*)lParam;
	WORD wType = 0, wLen = 0;	
	char buffer[MAXSENDMESSAGESIZE] = {'\0'};
	//srand(GetTickCount());

	// Initialize Event
	pClient->m_hSendEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	// Send Thread is Active
	pClient->m_bIsWPNSendProc = TRUE;

	// Initialize Event Failed
	if(!pClient->m_hSendEvent){

		// End Recv Thread
		pClient->m_bKeepLogin = FALSE;
		pClient->Close();
	}

	// Send Loop
	while(pClient->m_bKeepLogin){

		if(::WaitForSingleObject(pClient->m_hSendEvent, INFINITE) != WAIT_OBJECT_0){

			// End Recv Thread
			pClient->m_bKeepLogin = FALSE;
			pClient->Close();
			}

		// Signal Status
		while(pClient->m_bKeepLogin && pClient->m_bIsLoggedIn){

			::EnterCriticalSection(&pClient->m_ClientLock);
			if(pClient->m_dwSendSize){

				// Get Send Data
				wType = pClient->m_SendArray[pClient->m_dwSendType].wType;
				wLen = pClient->m_SendArray[pClient->m_dwSendType].wLen;
				memcpy(buffer + sizeof(WORD) * 2, pClient->m_SendArray[pClient->m_dwSendType].Data, wLen);

				// Upadate Ring Buffer
				pClient->m_dwSendType++;
				pClient->m_dwSendSize--;
				if(pClient->m_dwSendType >= MAXSENDCACHENUM)pClient->m_dwSendType = 0;
				::LeaveCriticalSection(&pClient->m_ClientLock);
			}
			else{

				::LeaveCriticalSection(&pClient->m_ClientLock);
				break;
			}
			
			// Encrypt
			*(WORD*)(buffer) = wType;
			*(WORD*)(buffer + sizeof(WORD)) = wLen;
			wLen += (sizeof(WORD) * 2);
			pClient->m_dwUPKey = EncryptMXTCP((BYTE*)buffer, wLen, pClient->m_dwUPKey);
			
			// Send
			if(pClient->Send(buffer, wLen, 5) != wLen){

				// Send Failed
				pClient->m_bKeepLogin = FALSE;
				pClient->Close();
				break;
			}
			
		}		
	}
	
	// Thread End
	pClient->ClearSendArray();
	if(pClient->m_hSendEvent)::CloseHandle(pClient->m_hSendEvent);
	pClient->m_bIsWPNSendProc = FALSE;

	return 0;
}

BOOL CWPNPClient::Disconnect()
{

	if(!m_bIsWPNClientProc) return FALSE;

	::EnterCriticalSection(&m_ClientLock);
	m_bKeepLogin = FALSE;
	Close();
	//Close();
	while(m_bIsWPNClientProc) PumpMessage();
	::LeaveCriticalSection(&m_ClientLock);

	return TRUE;
}

BOOL CWPNPClient::ProcWPNPMessage(WORD wType, WORD wLen, char *buffer)
{

	if(wType == 0x238E){
		
		// Chatroom Info
		CHATROOMINFO RoomInfo = {0, 0, NULL, NULL};
		if(Util::ScanMessage(buffer, wLen, "DDSS", &RoomInfo.dwUsers, &RoomInfo.dwLimit, &RoomInfo.lpszRoomName, &RoomInfo.lpszTopic) == 4){

			SendNotify(WPN_CHATROOMINFO, (LPARAM)&RoomInfo);
		}
	}	
	else if(wType == 0x13F6){

		// Incoming PM
		DWORD dwDummy1 = 0, dwDummy2 = 0, dwDummy3 = 0;
		PMINFO PMInfo;

		if(Util::ScanMessage(buffer, wLen, "DDDSSS", &dwDummy1, &dwDummy2, &dwDummy3, &PMInfo.lpszHandle, &PMInfo.lpszName, &PMInfo.lpszMessage) == 6){

			sscanf(PMInfo.lpszHandle, "WinMX %08X%04X", &PMInfo.dwIP, &PMInfo.wPort);
			
			CString strSent;

			switch(SendNotify(WPN_INCOMING_PM, (LPARAM)&PMInfo)){

				case PM_IGNORED:
					strSent = g_sSettings.GetIgnoredMsg();
					break;
				case PM_PRIVACY:
					strSent = g_sSettings.GetPrivacyMsg();
					break;
				default:
					strSent = "SENT";
			}

			SendFormatMessage(0x144F, "SSDDDS", PMInfo.lpszHandle, PMInfo.lpszName, dwDummy1, dwDummy2, dwDummy3, (LPCTSTR)strSent);
		}
	}
	else if(wType == 0x13ED){ // PM Ack

		// PM acknowledgement
		PMINFO PMAck;
 		if(Util::ScanMessage(buffer, wLen, "SSS", &PMAck.lpszHandle, &PMAck.lpszName, &PMAck.lpszMessage) == 3){

			sscanf(PMAck.lpszHandle, "WinMX %08X%04X", &PMAck.dwIP, &PMAck.wPort);
			SendNotify(WPN_PM_ACK, (LPARAM)&PMAck);
		}

	}
	else if(wType == 0x0460){

		// Rename confirmation
		WORD wID = 0;
		if(Util::ScanMessage(buffer, wLen, "W", &wID) == 1){

			SendQueStatus(1, 1, 0);
			m_strWPNUserName.Format("%s_%05d", m_strBaseUserName, wID);
			SendFormatMessage(0x038D, "S", m_strWPNUserName);
			SendNotify(WPN_CLIENTID, (LPARAM)wID);
			m_wID = wID;
		}
	}
	else if(wType == 0x138A){

		// Whois Answer
		// 0x138A][WinMX [HANDLE:S:12]][NAME:S:n][00:1][LINE:W:2][ONLINE:D:4][FILES:D:4][QUEUE:S:N][00:1]
		WHOIS whois;
		WORD wTmp;
		DWORD dwDummy;
		if(Util::ScanMessage(buffer, wLen, "SSWDWDS", &whois.lpszHandle, &whois.lpszName, &whois.wLine, &whois.dwOnline, &wTmp, &dwDummy, &whois.lpszQueue) == 7){

			whois.dwFiles = wTmp;
			sscanf(whois.lpszHandle, "WinMX %08X%04X", &whois.dwIP, &whois.wPort);
			SendNotify(WPN_WHOIS, (LPARAM)&whois);
		}
	}
	else if(wType == 0x138B){

		// Whois answer user offline
		WHOIS whois;
		if(Util::ScanMessage(buffer, wLen, "SS", &whois.lpszHandle, &whois.lpszName) == 2){

			whois.dwFiles = 0;
			whois.wLine   = 0;
			whois.dwOnline = 0;
			sscanf(whois.lpszHandle, "WinMX %08X%04X", &whois.dwIP, &whois.wPort);
			SendNotify(WPN_WHOIS_ERROR, (LPARAM)&whois);
		}
	}
	else if(wType == 0x1BBC){

		// Browse answer
		// [HANDLE|NAME:N|0:00|FILENAME:N|0:00|HASH:20|FILESIZE:4[|??:2|MP3BITRATE:2|SMPLFREQ:2|PLAYBCKTIME:4]]
		BROWSE_INFO browse;
		ZeroMemory(&browse.btHash, 20);
		browse.dwFileSize	= 0; 
		browse.dwIP			= 0;
		browse.wSampleFQ	= 0;
		browse.dwTime		= 0;
		browse.wBitrate		= 0;
		browse.wPort		= 0;
		WORD wDummy			= 0;
		if(Util::ScanMessage(buffer, wLen, "SSSHDWWWWW", &browse.lpszHandle, &browse.lpszName, 
												    &browse.lpszFile, (BYTE*)&browse.btHash,
													&browse.dwFileSize,	&wDummy,
													&browse.wBitrate, &browse.wSampleFQ, &wDummy, &browse.dwTime
													) >= 6){

			sscanf(browse.lpszHandle, "WinMX %08X%04X", &browse.dwIP, &browse.wPort);
			SendNotify(WPN_BROWSE_ENTRY, (LPARAM)&browse); 
		}
	}
	else if(wType == 0x1BC6){

		// Browse end
		// HANDLE|NAME
		BROWSE_INFO browse;
		if(Util::ScanMessage(buffer, wLen, "SS", &browse.lpszHandle, &browse.lpszName) == 2){

			browse.lpszFile		= 0;
			ZeroMemory(&browse.btHash, 20);
			sscanf(browse.lpszHandle, "WinMX %08X%04X", &browse.dwIP, &browse.wPort);
			SendNotify(WPN_BROWSE_DONE, (LPARAM)&browse);
		}
	}
	else if(wType == 0x1BD0){

		// Browse error
		BROWSE_INFO browse;
		if(Util::ScanMessage(buffer, wLen, "SS", &browse.lpszHandle, &browse.lpszName) == 2){

			browse.lpszFile		= 0;
			ZeroMemory(&browse.btHash, 20);
			sscanf(browse.lpszHandle, "WinMX %08X%04X", &browse.dwIP, &browse.wPort);
			SendNotify(WPN_BROWSE_ERROR, (LPARAM)&browse);
		}
	}
	else{

		TRACE("WPNP Recv: wType=0x%04X len=%02d data=%s\n", wType, wLen, buffer);
	}
	return TRUE;
}

void CWPNPClient::ClearSendArray()
{

	// Clear Send Ring Buffer
	m_dwSendType = 0;
	m_dwSendSize = 0;
	ZeroMemory(m_SendArray, sizeof(m_SendArray));
}

BOOL CWPNPClient::SendMXMessage(WORD wType, WORD wLen, char *pData)
{

	if(!m_bIsLoggedIn) return FALSE;
	if(m_dwSendSize >= MAXSENDCACHENUM) return FALSE;

	::EnterCriticalSection(&m_ClientLock);
	{

		// Add to Send Ring Buffer
		DWORD dwPos = m_dwSendType + m_dwSendSize;
		if(dwPos >= MAXSENDCACHENUM)dwPos -= MAXSENDCACHENUM;
		m_dwSendSize++;
		
		// Set Data
		m_SendArray[dwPos].wType = wType;
		m_SendArray[dwPos].wLen  = wLen;
		if(wLen)memcpy(m_SendArray[dwPos].Data, pData, wLen);

		::LeaveCriticalSection(&m_ClientLock);

		// Send Start
		::SetEvent(m_hSendEvent);
	}
	return TRUE;
}

BOOL CWPNPClient::SendFormatMessage(WORD wType, char *lpszFormat, ...)
{

	if(!m_bIsLoggedIn) return FALSE;
	if(m_dwSendSize >= MAXSENDCACHENUM) return FALSE;

	::EnterCriticalSection(&m_ClientLock);
	{

		// Add to Send Ring Buffer
		DWORD dwPos = m_dwSendType + m_dwSendSize;
		if(dwPos >= MAXSENDCACHENUM){
			
			dwPos -= MAXSENDCACHENUM;
		}
		m_dwSendSize++;
		
		// Data Format
		va_list Args;
		va_start(Args, lpszFormat);
		WORD wLen = 0;
		char *lpText = NULL;
		char *pBuff = m_SendArray[dwPos].Data;

		ZeroMemory(m_SendArray[dwPos].Data, sizeof(m_SendArray[dwPos].Data));

		while(*lpszFormat){

			switch(*lpszFormat){

				case 'S' :	// NULL Terminated String
					lpText = va_arg(Args, char *);
					lstrcpy(pBuff + wLen, lpText);
					wLen += (lstrlen(lpText) + 1);
					break;
				case 's' :	// Not NULL Terminated String
					lpText = va_arg(Args, char *);
					lstrcpy(pBuff + wLen, lpText);
					wLen += (lstrlen(lpText));
					break;
				case 'B' :	// BYTE
					pBuff[wLen] = va_arg(Args, BYTE);
					wLen += 1;
					break;
				case 'W' :	// WORD
					*((WORD *)(pBuff + wLen)) = va_arg(Args, WORD);
					wLen += 2;
					break;
				case 'D' :	// DWORD
					*((DWORD *)(pBuff + wLen)) = va_arg(Args, DWORD);
					wLen += 4;
					break;
				default :
					ASSERT(0);
					break;
			}
		
			lpszFormat++;
		}

		pBuff[wLen] = 0;
		va_end(Args);

		// Set Data
		m_SendArray[dwPos].wType = wType;
		m_SendArray[dwPos].wLen  = wLen;

	}
	::LeaveCriticalSection(&m_ClientLock);

	// Send Start
	::SetEvent(m_hSendEvent);

	return TRUE;
}


BOOL CWPNPClient::SendQueStatus(WORD wULMaxNum, WORD wULFreeNum, WORD wULQueNum)
{

	// Send Que Status
	if(!m_bIsLoggedIn) return FALSE;

	char szQueStatus[MAXTEXTBUFFSIZE] = "";
	
	if(wULFreeNum){
		
		sprintf(szQueStatus, "%u %u of 1 available", QUESTATUSBASE + wULFreeNum - wULQueNum, wULFreeNum, wULMaxNum);
	}
	else{
		
		sprintf(szQueStatus, "%u %u in queue (%u of %u available)", QUESTATUSBASE + wULFreeNum - wULQueNum, wULQueNum, wULFreeNum, wULMaxNum);
	}

	return SendFormatMessage(0x0384, "S", szQueStatus);
}

BOOL CWPNPClient::SendRename(LPCTSTR lpszNewName, WORD wLine, WORD wTransferPort)
{

	if(!m_bIsLoggedIn) return FALSE;

	m_wLineType		  = wLine;
	m_strBaseUserName = lpszNewName;
	
	int n  = m_strBaseUserName.ReverseFind('_');
	if((m_strBaseUserName.GetLength() - n)  == 6){

		m_strBaseUserName = m_strBaseUserName.Left(m_strBaseUserName.GetLength() - 6);
	}

	SendFormatMessage(0x03E9, "SWW", (LPCTSTR)m_strBaseUserName, wLine, wTransferPort);
	SendFormatMessage(0x038D, "S", (LPCTSTR)m_strWPNUserName);
	return 1;
}

BOOL CWPNPClient::SendWhois(DWORD dwTargetNode, WORD wTargetPort, LPCTSTR lpszUser)
{

	if(!m_bIsLoggedIn) return FALSE;

	char winmx[19];
	ZeroMemory(winmx, 19);

	sprintf((char*)winmx, "Winmx %08X%04X", dwTargetNode, wTargetPort);

	return SendFormatMessage(0x1389, "SS", (LPCTSTR)&winmx, lpszUser);
}

BOOL CWPNPClient::SendBrowseRequest(DWORD dwTargetNode, WORD wTargetPort, LPCTSTR lpszUser)
{

	if(!m_bIsLoggedIn) return FALSE;

	char winmx[19];
	ZeroMemory(winmx, 19);

	sprintf((char*)winmx, "Winmx %08X%04X", dwTargetNode, wTargetPort);
	SendFormatMessage(0x1B5A, "SS", (LPCTSTR)&winmx, lpszUser);
	return SendFormatMessage(0x1B59, "SS", (LPCTSTR)&winmx, lpszUser);
}

BOOL CWPNPClient::SendPM(DWORD dwTargetNode, WORD wTargetPort, LPCTSTR lpszUser, LPCTSTR lpszMessage)
{

	if(!m_bIsLoggedIn) return FALSE;

	char winmx[19];
	ZeroMemory(winmx, 19);

	sprintf((char*)winmx, "Winmx %08X%04X", dwTargetNode, wTargetPort);

	return SendFormatMessage(0x13EC, "SSS", (LPCTSTR)&winmx, lpszUser, lpszMessage);
}


BOOL CWPNPClient::SendPMAck(PMINFO* pmInfo)
{

	if(!m_bIsLoggedIn) return FALSE;

	char winmx[12];
	ZeroMemory(winmx, 12);

	sprintf((char*)&winmx, "%08X%04X", pmInfo->dwIP, pmInfo->wPort);
	
	LPCTSTR sent = "SENT";
	return SendFormatMessage(0x144F, "SSSS", pmInfo->lpszHandle, pmInfo->lpszName, (LPCTSTR)&winmx, sent);
}


BOOL CWPNPClient::SendDummyShares(void)
{

	if(!m_bIsLoggedIn) return FALSE;

	// Fake data ;) yeah its stupid to set it up this way but i was too lazy to think ;)
	// ahoi :D
	BYTE fake_hash[22];
	ZeroMemory(&fake_hash, 20);
	*(WORD*)(fake_hash) = 0xCB;
	*(WORD*)(fake_hash+1) = 0xEC;
	*(WORD*)(fake_hash+2) = 0x6A; 
	*(WORD*)(fake_hash+3) = 0xCF; 
	*(WORD*)(fake_hash+4) = 0xF7; 
	*(WORD*)(fake_hash+5) = 0x3B; 
	*(WORD*)(fake_hash+6) = 0x8F; 
	*(WORD*)(fake_hash+7) = 0x19; 
	*(WORD*)(fake_hash+8) = 0xD5; 
	*(WORD*)(fake_hash+9) = 0x54; 
	*(WORD*)(fake_hash+10) = 0x11; 
	*(WORD*)(fake_hash+11) = 0xC1; 
	*(WORD*)(fake_hash+12) = 0xF8; 
	*(WORD*)(fake_hash+13) = 0x89; 
	*(WORD*)(fake_hash+14) = 0x2A; 
	*(WORD*)(fake_hash+15) = 0xEF; 
	*(WORD*)(fake_hash+16) = 0xE7; 
	*(WORD*)(fake_hash+17) = 0x03; 

	DWORD	dwSize	 = 0x03E7;   // 999 bytes :-)
	DWORD   dwDummy2 = 0x3E80;
	DWORD   dwDummy3 = 0x0258;
	WORD    wDummy1	 = 0x18;

	// send buffer
	char send_buffer[MAXSENDMESSAGESIZE];
	ZeroMemory(&send_buffer, MAXSENDMESSAGESIZE);

	SendFormatMessage(0x038D, "S", (LPCTSTR)m_strWPNUserName);
	SendQueStatus(1, 1, 0);

	CStdioFile file;
	CString strBuffer, strPath, strFile;
	DWORD dwLen = 0;
	BOOL  bSkip = FALSE;
	int n = 0;
	TRY{

		if(file.Open(g_sSettings.GetWorkingDir() + "\\browse_message.txt", CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::typeText) == FALSE){

			return	FALSE;
		}

		while(file.ReadString(strBuffer)){

			if(strBuffer.IsEmpty()) continue;
			if(strBuffer[0] ==  ';') continue;

			if(strBuffer.Find("Path=", 0) == 0){

				bSkip = FALSE;
				strPath = strBuffer.Mid(5);
				strcpy((char*)(send_buffer), (LPCTSTR)strPath);
				dwLen = strPath.GetLength()+1;
				continue;
			}
			if(strBuffer.Find("EndPath", 0) == 0){

				if(dwLen == 0) continue;
				SendMXMessage(0x0FA3, (WORD)dwLen, send_buffer);
				dwLen = 0;
				ZeroMemory(send_buffer, MAXSENDMESSAGESIZE);
				bSkip = FALSE;
				continue;
			}

			if(bSkip) continue; // maximum length of directory exceeded. skip the rest :-P

			strFile = strBuffer;
			// check if we will exceed maximum buffer length
			if((strFile.GetLength() + 36 + dwLen) >= MAXSENDMESSAGESIZE){
				
				TRACE("Warning: insufficent buffer. Sending Incomplete shares.\n");
				bSkip = TRUE;
				continue;
			}

			strcpy((char*)(send_buffer+dwLen), (LPCTSTR)strFile);
			dwLen += strFile.GetLength()+1;
			memcpy((char*)(send_buffer+dwLen), (char*)&fake_hash, 20);
			dwLen += 20;
			memcpy((char*)(send_buffer+dwLen), (char*)&dwSize, 4);
			dwLen += 4;
			memcpy((char*)(send_buffer+dwLen), (char*)&wDummy1, 2);
			dwLen += 2;
			memcpy((char*)(send_buffer+dwLen), (char*)&dwDummy2, 4);
			dwLen += 4;
			memcpy((char*)(send_buffer+dwLen), (char*)&dwDummy3, 4);
			dwLen += 4;			
		}

		file.Close();
	}
	CATCH(CFileException, e){

		TRACE("CFileException while reading browse_message.txt\n");
	}END_CATCH;

	if(dwLen == 0) return FALSE;  // nothing to send

	return SendMXMessage(0x0FA3, (WORD)dwLen, send_buffer);
}

BOOL CWPNPClient::SendNotify(WPARAM wParam, LPARAM lParam)
{

	if(m_hWnd == NULL) return FALSE;

	return  (BOOL)::SendMessage(m_hWnd, UWM_WPNNOTIFY, wParam, lParam);
}

BOOL CWPNPClient::RefreshWPN()
{

	if(!m_bIsWPNClientProc) return FALSE;

	if(m_bIsLoggedIn){
		
		Close();
	}
	else{
		
		Close();
	}

	return TRUE;
}

void PumpMessage(void)
{

	MSG Msg;

	while(::PeekMessage(&Msg, NULL, 0, 0, PM_NOREMOVE)){

		if(!AfxGetThread()->PumpMessage()){

			AfxPostQuitMessage((int)Msg.wParam);
			return;
		}
	}
}

void WaitThreadMessage(UINT nMessage, WPARAM *pwParam, LPARAM *plParam)
{

	MSG Msg;

	do{

		while(!PeekMessage(&Msg, (HWND)-1, 0, 0, PM_NOREMOVE)) Sleep(1);
	}
	while(Msg.message != nMessage);

	if(pwParam && plParam){

		*pwParam = Msg.wParam;
		*plParam = Msg.lParam;
	}
}		  
