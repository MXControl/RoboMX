#ifdef _ENABLE_SERVER
#pragma once

#include "MySocket.h"
// CServerView view
#include "ClientSocket.h"
#include "RichDocument.h"
#include "RichEditExCtrl.h"
#include "MyEdit.h"
#include "ColorStatusBar.h"

#include <queue>
using std::queue;

// Channel modes:
#define CM_NORMAL	        0x00000000L
#define CM_OPTOPIC          0x40000000L
#define CM_KEYWORD          0x04000000L
#define CM_MODERATED        0x00400000L
#define CM_EXILEENABLE      0x00040000L
#define CM_TROLLPREFIX      0x00004000L
#define CM_NOID			    0x00000400L

#define UM_SUPER			UM_VOICED|UM_OPERATOR|UM_IMPOSE|UM_HIDDEN|UM_HOST

typedef struct TAG_BAN
{

	CString strName;
	CString strIP;
	CString strHost;
} BAN, *PBAN;

typedef  CArray<CClientSocket*, CClientSocket*> CClients;
typedef  CArray<BAN, BAN> CBans;

class CServerView : public CFormView
{
	DECLARE_DYNCREATE(CServerView)
public:
	enum { IDD = IDD_SERVER_VIEW };

protected:
	CServerView();           // protected constructor used by dynamic creation
	virtual ~CServerView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected: // overridables
	virtual void OnInitialUpdate();
	virtual void DoDataExchange(CDataExchange* pDX); 
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

protected:
	afx_msg LRESULT ClientCallback(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT LoadSettings(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnInput(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

public: // attributes
	CRichEditExCtrl		m_rSys;
	CRichDocument		m_rSysDoc;
	CMyEdit				m_eInput;
	CString				m_strInput;
	CColorStatusBar*	m_pStatusBar;
	
	CFont	  m_fFont;
	CMySocket m_mServer;
	BOOL	  m_bHosted;
	CEvent	  m_eDone;
	CEvent    m_eHosted;
	CEvent    m_eNotifyDone;
	BOOL	  m_bShutdown;
	CString   m_strRoomBase;
	CString   m_strRoomFull;
	DWORD	  m_dwIP;
	WORD	  m_wPort;
	DWORD	  m_dwLimit;
	CString	  m_strTopic;
	CString   m_strMotd;
	CClients  m_aClients;
	CBans	  m_aBans;
	UINT      m_uMode;
	CString   m_strKeyword;
	CString	  m_strTrollPrefix;
	CString   m_strExile;
	SOCKET    m_sIn;
	CWinThread *m_pServerThread;
	CWinThread *m_pNotifyThread;
	
	queue<ClientNotify*> m_qNotifies;

	unsigned __int64 m_nTotalMessages;
	unsigned __int64 m_nTotalJoins;
	unsigned __int64 m_nFailedJoins;

	CString			 m_strStartDate;
	CString			 m_strStartTime;

public:
	CRITICAL_SECTION m_csLock;
	void WriteText(LPCTSTR lpszText);
	void WriteText(UINT nID, ...);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void HandleCommand(CString strCmd);
	void PrintCmdNotUnderstood(const CString strCmd = "");
	void PrintHelp(void);
	void OnCloseDocument();
	
	void SetTopic(const CString strTopic);
	void SetMotd(const CString strMotd);
	void CheckClients(void);

	void ReCalcLayout();

	// Broadcast Send
	void SendTopic();
	void SendMotd();
	void SendMsg(CString strUser, CString strMsg);
	void SendOpMsg(CString strUser, CString strMsg, UINT uColor = 0, UINT uRecpMode = UM_OPERATOR);
	void SendAction(CString strUser, CString strMsg);
	void SendJoin(const CString strUser, DWORD dwIP, WORD wPort, WORD wLine, DWORD dwFiles, WORD wUserLevel, DWORD dwRealIP);
	void SendPart(const CString strUser, DWORD dwIP, WORD wPort);
	void SendRename(DWORD dwSrcIP, WORD wSrcPort, const CString strOldName, DWORD dwOldIP, WORD wOldPort, const CString strNewName, DWORD dwNewIP, WORD wNewPort, WORD wLine, DWORD dwFiles, WORD wUserLevel, UINT uModeInternal);
	void SendMode(const CString strSender, const CString strMode);
	void ServerAction(const CString strSender, const CString strMsg, UINT uColor = 0, UINT uMode = UM_OPERATOR);

	void Stop(BOOL bRedirect = FALSE, CString strTarget = "");

	static UINT ServerThread(LPVOID pParam);
	static UINT Loginthread(LPVOID pParam);
	static UINT NotifyThread(LPVOID pParam);

	DWORD TranslateIP(CString strIP);

	BOOL CheckUserName(const CString strName, CString strHost, DWORD dwIP);
	BOOL CheckUserName(const CString strName, DWORD dwIP = 0, WORD wPort = 0);
	BOOL CheckCommands(const CString strUser, const CString strMsg);

	BOOL ExecuteChannelCommand(const CString strUser, const CString strMsg, const UINT uMode);
	void FixString(CString& strFix);
	int GetByID(GUID guid);
	void SendChannelRename(void);
	BOOL HasSpeakPermission(UINT uMode);
	void SendNoVoice(GUID guid);
	void EchoChat(CString strName, CString strMsg, COLORREF cl);
	afx_msg void OnRobomxOpenserverconfiguration();
	afx_msg void OnRobomxSaveserverconfiguration();
	void SendCmdNotUnderstood(const CString strUser, const CString strCmd);
	void SendPermissionDenied(const CString strUser, const CString strCmd);
	BOOL CheckPermission(UINT uMode, UINT uRequired);
	void SendChannelStats(void);
	void HandleVisible(const CString strUser, DWORD dwIP, WORD wPort, WORD wLine, DWORD dwFiles, WORD wUserLevel, DWORD dwRealIP, BOOL bSetVisible);
};

#endif

