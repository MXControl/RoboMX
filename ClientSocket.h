/*
** Copyright (C) 2004 Thees Schwab
**  
4** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
#ifdef _ENABLE_SERVER

#pragma once

#include "mysocket.h"
#include "MXSock.h"

#define MAX_BUFFER_SIZE		10000
#define MAX_SEND_SIZE		1024

// define chatprotocol levels
#define CLIENT_WINMX331		0x001
#define CLIENT_WINMX353		0x002
#define CLIENT_IPCLIENT		0x003

// define usermodes
#define UM_NORMAL	        0x00000000L  // normal state, logged in no voice
#define UM_PENDING          0x10000000L  // logging in, no voice, not in channel
#define UM_OPERATOR         0x01000000L  // operator 
#define UM_SPEAKPERMIT	    0x00100000L  // voice
#define UM_BANNED			0x00010000L  // banned
#define UM_IDIOT			0x00001000L  // idiot
#define UM_IMPOSE			0x00000100L  // may use /impose
#define UM_HIDDEN			0x00000010L  // hidden
#define UM_HOST				0x00000001L  // host

#define UM_VOICED		    UM_SPEAKPERMIT

#ifdef _SUPPORT_COLORS
# define UM_NOCOLORS		0x0000001L  // disable colors
#endif

typedef BOOL (RECVMESSAGEPROC)(DWORD dwParam, WPARAM wParam, LPARAM lParam);

#define CC_ERROR  0x000
#define CC_RENAME 0x001
#define CC_MSG    0x002
#define CC_ACTION 0x003
#define CC_PING   0x004
#define CC_OPMSG  0x005

////////////////////////
// Notification packets
class ClientNotify
{
public:
	ClientNotify(){ wType = 0; dwIP = 0; wPort = 0; uMode = UM_NORMAL;};
	~ClientNotify(){};

	WORD  wType;
	DWORD dwIP;
	WORD  wPort;
	UINT  uMode;
	GUID  guid;
};

class ClientMessage : public ClientNotify
{
public:
	ClientMessage(){ wType = CC_MSG; dwIP = 0; wPort = 0; };
	CString strName;
	CString strText;
};

class ClientAction : public ClientNotify
{
public:
	ClientAction(){ wType = CC_ACTION; dwIP = 0; wPort = 0; };
	CString strName;
	CString strText;
};

class ClientOpMsg : public ClientNotify
{
public:
	ClientOpMsg(){ wType = CC_OPMSG; dwIP = 0; wPort = 0; uParam = 0; uRecipientMode = UM_OPERATOR; };
	CString strName;
	CString strText;
	UINT    uParam;
	UINT	uRecipientMode;	 // minimum required userlevel to see this message
};

class ClientError : public ClientNotify
{
public:
	ClientError(){ wType = CC_ERROR; dwIP = 0; wPort = 0; }
	GUID    guID;
	CString strCause;
};

class ClientRename : public ClientNotify
{

public:
	ClientRename(){ wType = CC_RENAME; dwIP = 0; wPort = 0;  nVisible = 2; uModeInternal = UM_PENDING;};
	CString  strOldName;
	DWORD    dwOldIP;
	WORD     wOldPort;
	WORD     wOldLine;
	DWORD    dwOldFiles;
	CString  strNewName;
	DWORD    dwNewIP;
	WORD	 wNewPort;
	WORD     wNewLine;
	DWORD    dwNewFiles;
	WORD	 wUserLevel;
	int		 nVisible; // 0 = remove, 1 = add, 2 = not changed
	UINT	 uModeInternal;
};

// Notification packets end
////////////////////////////


////////////////////////////
// Clientsocket
// 
class CClientSocket :
	public CMySocket
{
public:
	CClientSocket(SOCKET socket);
	~CClientSocket(void);

public: // attributes

	CString m_strName;		// username
	CString m_strSrcHost;	// hostname of user
	DWORD   m_dwSrcIP;		// ip of user
	WORD    m_wSrcPort;		// port of user
	DWORD   m_dwIP;			// node ip of user
	WORD    m_wPort;		// node port of user
	WORD    m_wLineType;	// line of user
	DWORD   m_dwFiles;		// number of files
	UINT    m_uMode;		// usermode of user
	GUID	m_guID;			// genuine ID
	WORD    m_wClientType;	// Client protocol level
	CString m_strClientString;	// Client Ident string
	CString m_strClientVer;		// Client Version string

	DWORD   m_dwLast;
	WORD	m_wFloodCounter;
	
	// statistical data
	CString m_strEnterDate;
	CString m_strEnterTime;
	CString m_strEnterName;
	unsigned __int64 m_nMessages;

	CString m_strParam;		// general parameter buffer

	HWND    m_hMsgTarget;	// server this user belongs to
	BOOL	m_bListen;		// still connected?
	CEvent  m_eDone;		// client done, ready to kill

	DWORD   m_dwUpKey;
	DWORD   m_dwDWKey;

	char    m_cBuffer[MAX_BUFFER_SIZE];
	CWinThread *m_pThread;

public: // interface

	BOOL HandShake(CString strRoom);
	BOOL HighlevelHandshake(CString strKey);
	void StartUp(void);
	void LogOut(void);
	BOOL SendNotify(WPARAM wParam, LPARAM lParam);
	
	void HandlePacket(WORD wType, WORD wLen, char* buffer);

	int SendCrypted(char *pBuff, int nLen, int nWait);

	BOOL SendTopic(CString strTopic);
	BOOL SendMotd(CString strMotd);
	BOOL SendMsg(CString strUser, CString strMsg);
	BOOL SendOperator(CString strUser, CString strMsg, BOOL bEcho, UINT uColor = 0);
	BOOL SendAction(CString strUser, CString strMsg);
	BOOL SendJoin(CString strUser, DWORD dwIP, WORD wPort, WORD wLine, DWORD dwFiles, WORD wUserLevel = 0, DWORD dwRealIP = 0);
	BOOL SendRename(CString strOldName, DWORD dwOldIP, WORD wOldPort, CString strNewName, DWORD dwNewIP, WORD wNewPort, WORD wLine, DWORD dwFiles, WORD wUserLevel = 0);
	BOOL SendPart(CString strUser, DWORD dwIP, WORD wPort);
	BOOL SendUserlist(CString strUser, DWORD dwIP, WORD wPort, WORD wLine, DWORD dwFiles, WORD wUserLevel = 0);
	BOOL SendPong();
	BOOL SendRedirect(CString strTarget);
	static UINT RecvProc(LPVOID pParam);
	
	// Note all users start as UM_PENDING and have no rights
	UINT GetUserMode(){ return m_uMode; }
	void SetUserMode(UINT uMode){ m_uMode = uMode; }
	void AddMode(UINT uMode, CString strParam = "");
	void RemoveMode(UINT uMode, CString strParam = "");

	BOOL SendChannelRename(CString strRoom);
	BOOL ReplaceColors(CString& rString);
	CString GetUserStats(void);
protected:
	BOOL IsFlooding(void);
};

#endif