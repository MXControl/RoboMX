/*
** The original implementation of this class was written by
** Nushi@2SEN.
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

#pragma once

#include "MySocket.h"
#include "Util.h"
#include "MXSock.h"

#define MAXTEXTBUFFSIZE		1024
#define QUESTATUSBASE		9000

#define MAXRECVMESSAGESIZE	3072	// Max Recv Message Size
#define MAXSENDCACHENUM		32		// Max Send Message Cache Num
#define MAXSENDMESSAGESIZE	3072	// Max Send Message Size


// Thread Synchronize Message
#define WM_RECVPROCSTART		(WM_USER + 0x100)	// Recv Thread Start
#define WM_RECVPROCEND			(WM_USER + 0x101)	// Recv Thread End


// Event Notifaction-Messages
#define WPN_ERROR				0x0000	// Error

#define WPN_WPNCONNECTSTART		0x0001	// Login Start
#define WPN_WPNCONNECTED		0x0002	// Login Complete
#define WPN_WPNDISCONNECTED		0x0003	// Disconnect from Parent WPN
#define WPN_CHANGEPARENTSTART	0x0004	// Change Parent Start
#define WPN_CHANGEPARENTCOMP	0x0005	// Change Parent Complete
#define WPN_PARENTDISCONNECTED	0x0006	// Disconnected from Parent
#define WPN_CHATROOMINFO		0x0007	// Chat List Notifation
#define WPN_CLIENTID			0x0008  // Client ID
#define WPN_INCOMING_PM			0x0009  // Recieving PM
#define WPN_PM_ACK				0x0010  // Remote site has acknoledged the PM
#define WPN_WHOIS				0x0011  // Answer for whois
#define WPN_WHOIS_ERROR			0x0012  // Answer whois failed
#define WPN_BROWSE_ENTRY		0x0013  // One file of browse request
#define WPN_BROWSE_DONE			0x0014  // Final packet with queue status
#define WPN_BROWSE_ERROR		0x0015  // timeout of browse
// Error codes
#define WPN_ERROR_LOGIN			0x0001	// Login Failed
#define WPN_ERROR_GETNODE		0x0002	// Get Parent Node Failed

#define PM_NOTPROCESSED			0x0000	// PM has not been processed yet
#define PM_IGNORED				0x0001	// PM User is Ignored 
#define PM_PRIVACY				0x0002  // PM Ignored due to privacy settings
#define PM_RECIEVED				0x0003  // PM recieved

// Frontcode
#define FRONTCODEHOSTNAME	"216.127.74.62"
#define FRONTCODESTARTHOST	0
#define FRONTCODEENDHOST	0
#define FRONTCODESTARTPORT	7952
#define FRONTCODEENDPORT	7950


typedef struct TAG_PARENTNODEINFO {

	DWORD dwNodeIP;		
	WORD wUDPPort;		
	WORD wTCPPort;		
	BYTE bFreePri;		
	BYTE bFreeSec;		
	WORD wReserved;
} PARENTNODEINFO, *PPARENTNODEINFO;

typedef struct TAG_WPNPMESSAGE {

	WORD wType;			
	WORD wLen;			
	char Data[MAXSENDMESSAGESIZE];
} WPNPMESSAGE, *PWPNPMESSAGE;

typedef struct TAG_CHATROOMINFO {

	DWORD dwUsers;
	DWORD dwLimit;
	LPSTR lpszRoomName;
	LPSTR lpszTopic;
} CHATROOMINFO, *PCHATROOMINFO;

typedef struct TAG_PMINFO {

	LPSTR lpszHandle;
	LPSTR lpszName;
	LPSTR lpszMessage;
	DWORD dwIP;
	WORD  wPort;
} PMINFO, *PPMINFO;

typedef struct TAG_BROWSEINFO {

	LPSTR lpszHandle;
	LPSTR lpszName;
	LPSTR lpszFile;
	BYTE  btHash[20];
	DWORD dwFileSize;
	WORD wSampleFQ;
	WORD  wBitrate;
	DWORD dwTime;
	DWORD dwIP;
	WORD  wPort;
} BROWSE_INFO, *PBROWSE_INFO;

typedef struct TAG_WHOIS{

	LPSTR lpszHandle;
	LPSTR lpszName;
	LPSTR lpszQueue;
	DWORD dwIP;
	WORD  wPort;
	DWORD dwFiles;
	WORD  wLine;
	DWORD dwOnline;
} WHOIS, *PWHOIS;


class CWPNPClient :
	public CMySocket
{
public:

	CWPNPClient(void);
	~CWPNPClient(void);

	// Public interface
	//void SetCallBackProck(RECVMESSAGEPROC *pProc, DWORD dwParam);
	void AddListener(HWND hWnd);
	void RemoveListener(HWND hWnd);
	
	// Connect to parent node
	int	 ConnectWPN(PARENTNODEINFO *pNodeInfo, LPCTSTR lpszUserName, WORD wLineType, WORD wTransferPort);

	// Disconnect from parent node
	BOOL Disconnect(void);

	// send a message
	BOOL SendMXMessage(WORD wType, WORD wLen, char *pData);

	// sends queue status to parent
	BOOL SendQueStatus(WORD wULMaxNum, WORD wULFreeNum, WORD wULQueNum);

	// sends new name to parent
	BOOL SendRename(LPCTSTR lpszNewName, WORD wLine, WORD wTransferPort = 0);

	// get a new parent node
	BOOL RefreshWPN(void);

	BOOL SendFormatMessage(WORD wType, char *lpszFormat, ...);

	BOOL SendPM(DWORD dwTargetNode, WORD wTargetPort, LPCTSTR lpszUser, LPCTSTR lpszMessage);

	BOOL SendWhois(DWORD dwTargetNode, WORD wTargetPort, LPCTSTR lpszUser);

	BOOL SendBrowseRequest(DWORD dwTargetNode, WORD wTargetPort, LPCTSTR lpszUser);

	BOOL SendPMAck(PMINFO* pmInfo);

	BOOL SendDummyShares(void);

	// Userinfo
	CString m_strBaseUserName;	// Base User Name XXXXXNNN
	CString m_strWPNUserName;	// Full User Name XXXXXNNN_MMMMM
	WORD	m_wTransferPort;	// TCP Port for File Transfer
	WORD	m_wLineType;		// Connection Line Type
	WORD	m_wID;				// User ID

	// Parent Node info
	DWORD m_dwParentIP;			// Parent Node IP IP Addresses
	WORD  m_wParentTCPPort;		// Parent Node TCP Port
	WORD  m_wParentUDPPort;		// Parent Node UDP Port
	BOOL  m_bAutoParent;		// Get Parent Node Automatically
	BOOL  m_bIsLoggedIn;		// WPN Login Status

	DWORD m_dwParentIP_External;	 // If Forced Primary
	
	DWORD m_dwUPKey;				// Send Crypt Key
	DWORD m_dwDWKey;				// Recv Crypt Key

	HWND m_hWnd;

protected:
	/*typedef struct _TAG_PEERCACHESERVER {

		LPCTSTR lpszHost;
		WORD    wPort;
	} PEERCHACHESERVER, *PPEERCHACHESERVER;*/

private:

	static UINT WPNClientProc(LPVOID lParam);
	static UINT WPNSendProc(LPVOID lParam);

	BOOL GetParentNode(PARENTNODEINFO *pNodeInfo);
	BOOL LoginParent(PARENTNODEINFO *pNodeInfo, LPCTSTR lpszBaseName, CString &strWPNUserName,  WORD wLineType, WORD wTransferPort);
	BOOL ProcWPNPMessage(WORD wType, WORD wLen, char *Buff);
	void ClearSendArray(void);
	BOOL SendNotify(WPARAM wParam, LPARAM lParam);

	// Thread synchronisation
	HANDLE				m_hSendEvent;
	CRITICAL_SECTION	m_ClientLock;

	// bufers
	DWORD				m_dwSendType;
	DWORD				m_dwSendSize;			
	WPNPMESSAGE			m_SendArray[MAXSENDCACHENUM];	


	BOOL m_bKeepLogin;					// Keep Login Flag
	BOOL m_bIsWPNSendProc;				// Send Thread is Active
	BOOL m_bIsWPNClientProc;			// Recv Thread is Active
};

void PumpMessage(void);