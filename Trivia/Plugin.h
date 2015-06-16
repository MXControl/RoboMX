/*
** Copyright (C) 2003-2004 Thees Winkler
**  
** Trivia is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** Trivia is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#pragma once
#include "roboex.h"
#include "ini.h"

typedef struct TAG_USERINFO{

	CString strName;
	int   nTriviaScore;
	BOOL  bIsAdmin;
} PLAYER, *PPLAYER;

typedef struct _TAG_TRIVIA{

	CString strFile;
	DWORD	dwNum;
} TRIVIA, *PTRIVIA;

typedef CArray<TRIVIA, TRIVIA> CTriviaArray;
typedef CMap<CString, LPCTSTR, PLAYER, PLAYER> CPlayerMap;

class CTrivia :
	public CRoboEx
{
public:
	CTrivia(void);
	virtual ~CTrivia(void);

	// Called when a context menu (either chat- or usermenu) is
	// about to be displayed. If bIsUser is TRUE then the userlist menu
	// is about to be display, otherwise the chatmenu.
	// hMenu is a handle to the menu to be displayed
	virtual void OnPrepareMenu(DWORD dwID, BOOL bIsUser, HMENU hMenu);
	
	// Called when a user selects an item from a menu. nCmd is the command identifer, 
	// dwID is the ID of the channel. this is -1 if the command is not channel related
	virtual void OnMenuCommand(UINT nCmd, DWORD dwID, CString strName);
	void OnJoinChannel(DWORD dwID, CString strChannel, CUserMap* pUserMap);
	void OnLeaveChannel(DWORD dwID);
	void OnMessage(DWORD dwID, CString* pUser, CString* pMsg, BOOL bIsAction);
	void OnInputHook(DWORD dwIP, CString *pInput);
	void OnSetUserName(CString strNewUsername);
	// Called when a user in the channel changes an attribute
	virtual void OnRename(DWORD dwID, PMX_USERINFO pOld, PMX_USERINFO pNew);
	void Init();
	void Quit();
	BOOL BuiltTriviaIndex(void);

	CPlayerMap m_aPlayers;
	CTriviaArray m_aTrivia;
	DWORD	m_dwTotal;
	BOOL	m_bTrivia;
	static	UINT TriviaThread(PVOID pVoid);
	CEvent	 m_eTrivia;
	DWORD	 m_dwTriviaID;
	COLORREF m_crBg;
	CString  m_strWd;
	CIni     m_Ini;
	CString		 m_strQuestion;
	CStringArray m_aAnswers;
	int 		 m_nPoints;
	int			 m_nTime;
	int m_nRoundEnd;
	int m_nRoundCurrent;


	DWORD m_dwFloodTick;
	DWORD m_dwFloodCounter;

	HMENU m_hMenu; 
	BOOL m_bHints;

	HINSTANCE m_hInstance;
	void Game(void);
	static CString itocstr(int n);
	int ContainsStringExact(CString strLine, CString strSubString, BOOL bCase, BOOL bExactMatch = FALSE);
	void CTrivia::FinishRound(DWORD dwID); //, CString strWinner);
	int GetScore(const CString strNickname);
	int IncreaseScore(CString &strUser, int nPoints); //, PINT nDone);
	BOOL IsAdmin(CString strNickname);
	CString m_strQuestionText;
	CString m_strSkipText;
	CString m_strCorrect;
	CString m_strNoAnswer;
	CString m_strScore;
	CString m_strStart;
	CString m_strStartAdmin;
	BOOL	m_bPause;
	CString m_strLastName;
	int     m_nCurrentRow;
	int     m_nRowWin;
	int		m_nCurrentWin;
	int     m_nRoundsWin;
	int     m_nCurrentQuestion;
	CString  m_strHintText;
	CString m_strLastWinner;
	CString m_strStreakMsg;
	CString m_strWinMsg;
	CString m_strCurrentHint;

	CString m_strBotname;

	void SetAdmin(CString strUser, bool bAdmin);
	CString GetBaseName(CString strUser);
	void PrepareGame(void);
	void FloodSafeInput(DWORD dwID, const CString strText);
};