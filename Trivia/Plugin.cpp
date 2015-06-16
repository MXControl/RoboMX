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

#include "StdAfx.h"
#include ".\plugin.h"
#include "tinyxml.h"

#define ID_MENUBASE  40500

CTrivia::CTrivia(void)
{

	m_strName			= "Trivia v1.1";
	m_strDescription	= "Trivia Plugin for RoboMX";
	m_strAuthor			= "Bender979";
	m_strWd				= "";  
	m_dwTotal			= 0;
	m_crBg				= 0;
	m_nPoints			= 0;
	m_nTime				= 0;
	m_nRoundEnd			= -1;
	m_nRoundCurrent		= 0;
	m_bHints			= 0;
	m_nCurrentQuestion	= 0;      
	m_dwFloodTick		= 0;
	m_dwFloodCounter	= 0;

}

CTrivia::~CTrivia(void)
{

}

void CTrivia::Init()
{

	TCHAR szBuffer[_MAX_PATH]; 
	::GetModuleFileName(m_hInstance, szBuffer, _MAX_PATH);
	m_strWd.Format("%s", szBuffer);
	m_strWd = m_strWd.Left(m_strWd.ReverseFind('\\'));
	m_eTrivia.SetEvent();
	
	CString strParent = m_strWd.Left(m_strWd.ReverseFind('\\'));
	m_Ini.SetIniFileName(strParent + "\\RoboMX.ini");
	m_crBg = m_Ini.GetValue("Colors", "Background", RGB(255,255,255));

	//srand(GetTickCount());
}

void CTrivia::Quit()
{

	if(m_bTrivia){

		m_bTrivia = FALSE;
		WaitForSingleObject(m_eTrivia.m_hObject, INFINITE);
	}
}

void CTrivia::OnInputHook(DWORD dwIP, CString *pInput)
{

	if(pInput->CompareNoCase("/trivia start") == 0){

		if(m_bTrivia){

			WriteEchoText(dwIP, "Trivia game is already running!\n", RGB(0, 0, 255), RGB(230, 200, 0));
		}
		else{

			m_dwTriviaID = dwIP;
            WriteEchoText(dwIP, "Initializing Trivia game. Please stand by...!\n", RGB(0, 0, 255), RGB(230, 200, 0));
			PrepareGame();
		}
		pInput->Empty();
	}
	else if(pInput->CompareNoCase("/trivia stop") == 0){

		if(m_bTrivia){

			WriteEchoText(dwIP, "Stopping game. Please stand by...!\n", RGB(0, 0, 255), RGB(230, 200, 0));
			m_bTrivia = FALSE;
			m_dwTriviaID = 0;
			WaitForSingleObject(m_eTrivia.m_hObject, INFINITE);
			WriteEchoText(dwIP, "Game stopped.\n", RGB(0, 0, 255), RGB(230, 200, 0));
		}
		else{

			WriteEchoText(dwIP, "Trivia game is not running!\n", RGB(255,255,255), RGB(230, 200, 0));
		}
		pInput->Empty();
	}
	else if(pInput->Find("/addgamemaster ") == 0){

		CString strName = *pInput;
		strName.Replace("/addgamemaster ", "");

		SetAdmin(strName, TRUE);
		pInput->Empty();
		FloodSafeInput(dwIP, strName + " was added as a game master.");
	}
	else if(pInput->Find("/remgamemaster ") == 0){

		CString strName = *pInput;
		strName.Replace("/remgamemaster ", "");

		SetAdmin(strName, FALSE);
		pInput->Empty();
		FloodSafeInput(dwIP, strName + " is no longer a game master.");
	}
	else if(pInput->CompareNoCase("/config") == 0){

		ShellExecute(0, "open", m_strWd + "\\Trivia.ini", 0, 0, SW_MAXIMIZE);
		WriteEchoText(dwIP, "You have to restart the game for changes to take effect.\n", RGB(0,0,255), m_crBg);
		pInput->Empty();
	}
	else if(pInput->CompareNoCase("/trivia") == 0){

		WriteEchoText(dwIP, "Trivia Plugin for RoboMX (c) 2004 by Thees Schwab.\nAvailable commands:\n", RGB(0,0,255), m_crBg);
		WriteEchoText(dwIP, "/trivia start  -  Start a new trivia game in this channel\n", RGB(0,0,255), m_crBg);
		WriteEchoText(dwIP, "/trivia stop   -  Stop current trivia game\n", RGB(0,0,255), m_crBg);
		WriteEchoText(dwIP, "/addgamemaster <user> - Add <user> as a gamemaster\n", RGB(0,0,255), m_crBg);
		WriteEchoText(dwIP, "/addgamemaster <user> - Remove <user> from gamemasters\n", RGB(0,0,255), m_crBg);
		WriteEchoText(dwIP, "/config   - Open configuration file.\n", RGB(0,0,255), m_crBg);
		WriteEchoText(dwIP, "/trivia   - Display this message\n", RGB(0,0,255), m_crBg);
		WriteEchoText(dwIP, "Note: Game masters can use the SKIP, RESET and PAUSE commands.\n", RGB(0,0,255), m_crBg);
		pInput->Empty();
	}

	if(pInput->GetLength()){

		if(m_dwFloodTick - GetTickCount() < 3000){

			// if the last message has been input in the last 3 seconds 
			// raise the counter
			m_dwFloodCounter++;
		}
		else{

			// we are safe, reset the counter
			m_dwFloodCounter = 0;
		}

		if(m_dwFloodCounter > 3){

			// We need to wait for the next output or we will get kicked
			// After that we can reset the counter and input the message
			Sleep(4000);
			m_dwFloodCounter = 0;
		}

		m_dwFloodTick = GetTickCount();
	}
}

// Called when a context menu (either chat- or usermenu) is
// about to be displayed. If bIsUser is TRUE then the userlist menu
// is about to be display, otherwise the chatmenu.
// hMenu is a handle to the menu to be displayed
void CTrivia::OnPrepareMenu(DWORD dwID, BOOL bIsUser, HMENU hMenu)
{

	if(m_hMenu){
		
		//delete m_hMenu;
		m_hMenu = NULL;
	}

	m_hMenu = ::CreateMenu();
	if(m_hMenu){

		if(bIsUser){

			::AppendMenu(m_hMenu, MF_STRING, ID_MENUBASE+10, "Add as Gamemaster");
			::AppendMenu(m_hMenu, MF_STRING, ID_MENUBASE+11, "Remove Gamemaster");
			::AppendMenu(m_hMenu, MF_SEPARATOR, 0, (LPCTSTR)0);
			::AppendMenu(m_hMenu, MF_STRING, ID_MENUBASE+12, "Print Score");
		}
		else{

			::AppendMenu(m_hMenu, MF_STRING, ID_MENUBASE, "Set as Trivia channel");
			::AppendMenu(m_hMenu, MF_STRING, ID_MENUBASE+1, "Unset as Trivia channel");
			::AppendMenu(m_hMenu, MF_SEPARATOR, 0, (LPCTSTR)0);
			::AppendMenu(m_hMenu, MF_STRING, ID_MENUBASE+2, "Start Game (Standard)");
			::AppendMenu(m_hMenu, MF_STRING, ID_MENUBASE+3, "Start Game (Endless)");
			::AppendMenu(m_hMenu, MF_STRING, ID_MENUBASE+4, "Stop Game");
			::AppendMenu(m_hMenu, MF_SEPARATOR, 0, (LPCTSTR)0);
			::AppendMenu(m_hMenu, MF_STRING, ID_MENUBASE+5, "Print Ranking");
			::AppendMenu(m_hMenu, MF_SEPARATOR, 0, (LPCTSTR)0);
			::AppendMenu(m_hMenu, MF_STRING, ID_MENUBASE+6, "Enable/Disable hints");
		}
		::AppendMenu(hMenu, MF_SEPARATOR, 0, (LPCTSTR)0);
		::AppendMenu(hMenu, MF_POPUP,(UINT)m_hMenu, "Trivia");
	}
}

// Called when a user selects an item from a menu. nCmd is the command identifer, 
// dwID is the ID of the channel. this is -1 if the command is not channel related
void CTrivia::OnMenuCommand(UINT nCmd, DWORD dwID, CString strName)
{


	CString strBase;
	if(strName.GetLength()){
		
		strBase = GetBaseName(strName);
	}

	switch(nCmd){

		case ID_MENUBASE: // set
			InputMessage(dwID, "/trivia start");
			break;
		case ID_MENUBASE+1: // unset
			InputMessage(dwID, "/trivia stop");
			break;
		case ID_MENUBASE+2: // start
			FloodSafeInput(dwID, "!trivia 20");
			break;
		case ID_MENUBASE+3: // start endless
			FloodSafeInput(dwID, "!trivia");
			break;
		case ID_MENUBASE+4: // stop
			FloodSafeInput(dwID, "!strivia");
			break;
		case ID_MENUBASE+5: // ranking
			FloodSafeInput(dwID, "!ranking");
			break;
		case ID_MENUBASE+6: // hints
			FloodSafeInput(dwID, "!hints");
			break;
		case ID_MENUBASE+10: // Add as Gamemaster
			InputMessage(dwID, "/addgamemaster " + strBase);
			break;
		case ID_MENUBASE+11: // Remove Gamemaster
			InputMessage(dwID, "/removegamemaster " + strBase);
			break;
		case ID_MENUBASE+12: // score
			FloodSafeInput(dwID, "!score " + strBase);
			break;
	}
}

void CTrivia::OnJoinChannel(DWORD dwID, CString strChannel, CUserMap* pUserMap)
{

	if(!m_bTrivia){

		WriteEchoText(dwID, "Type '/trivia start' to enable the trivia bot in this channel.\n", RGB(0, 0, 255), RGB(230, 220, 0));
		WriteEchoText(dwID, "'/trivia' will list all available commands.\n", RGB(0, 0, 255), RGB(230, 220, 0));
	}
}

void CTrivia::OnLeaveChannel(DWORD dwID)
{

	if(m_bTrivia && (dwID == m_dwTriviaID)){

		m_bTrivia = FALSE;
		WaitForSingleObject(m_eTrivia.m_hObject, INFINITE);
	}
}

// Called when your own username changes, either because of
// Userinteraction or if the client-id changes
// new since API Version 0x1002L
void CTrivia::OnSetUserName(CString strNewUsername)
{

	PLAYER player;
	player.bIsAdmin		= TRUE;
	player.nTriviaScore = 0;

	if(m_strBotname.GetLength()){

		if(m_aPlayers.Lookup(m_strBotname, player)){

			m_aPlayers.RemoveKey(m_strBotname);
		}
	}

	m_strBotname    = GetBaseName(strNewUsername);
	player.strName  = m_strBotname;

	m_aPlayers.SetAt(player.strName, player);

}

// Called when a user in the channel changes an attribute
void CTrivia::OnRename(DWORD dwID, PMX_USERINFO pOld, PMX_USERINFO pNew)
{

	PLAYER player;
	player.bIsAdmin		= FALSE;
	player.nTriviaScore = 0;

	if(m_aPlayers.Lookup(GetBaseName(pOld->strUser), player)){

		m_aPlayers.RemoveKey(player.strName);
		
		player.strName = GetBaseName(pNew->strUser);
		
		m_aPlayers.SetAt(player.strName, player);

		TRACE("Player name change applied succesfully :-)\n");
	}

}


void CTrivia::OnMessage(DWORD dwID, CString* pUser, CString* pMsg, BOOL bIsAction)
{
 	

	if(m_dwTriviaID != dwID){

		return;
	}

	CString strNickname = *pUser;
	CString strMessage	= *pMsg;

	// Nickname is emtpy, ignore input :-P
	if(strNickname.IsEmpty()){
	
		return;
	}
	// Message is emtpy, ignore input :-P
	if(strMessage.IsEmpty()){
		
		return;
	}
	
	if(m_strQuestion.GetLength() && (strMessage.Find(m_strQuestion) >= 0)){
		
		return;
	}

	CString strTmp;

	if((strMessage == "!hints") && IsAdmin(strNickname)){

		m_bHints = !m_bHints;
		if(m_bHints) FloodSafeInput(dwID, "Enabled Hints.");
		else FloodSafeInput(dwID, "Disabled Hints.");
	}

	if(!m_bTrivia){

        if((strMessage.Find("!trivia") == 0) && IsAdmin(strNickname)){

			strTmp = strMessage.Mid(7);
			strTmp.TrimLeft(); strTmp.TrimRight();
			
			if(strTmp.IsEmpty()){

				m_nRoundEnd = -1;
			}
			else{

				m_nRoundEnd		= atoi(strTmp);
			}
			m_bTrivia    = TRUE;
			m_eTrivia.ResetEvent();
			AfxBeginThread(TriviaThread, (LPVOID)this, THREAD_PRIORITY_NORMAL);
		}
		else{

			return;
		}
	}
	if(m_bTrivia){

		if((strMessage == "!strivia") && IsAdmin(strNickname)){

			m_bTrivia = FALSE;
			WaitForSingleObject(m_eTrivia.m_hObject, INFINITE);
			CString strMsg;
			strMsg.Format("Game has been stopped by '%s'.", strNickname);
			FloodSafeInput(m_dwTriviaID, strMsg);
		}
	}
	if(strMessage == "!score"){

 		strTmp = m_strScore;
		strTmp.Replace("%NAME%", strNickname);
		strTmp.Replace("%POINTS%", itocstr(GetScore(strNickname)));
		FloodSafeInput(m_dwTriviaID, strTmp);
		return;		
	}
	
	if(strMessage == "!ranking"){

		if(!m_aPlayers.GetSize()) return;

		PLAYER uTemp, u1, u2, u3;

		//CString strTmp;

		u1.nTriviaScore = -1;
		u1.strName = "";
		POSITION pos = m_aPlayers.GetStartPosition();
		//for(int i = 0; i < m_aPlayers.GetSize(); i++){
		
		while(pos){

			m_aPlayers.GetNextAssoc(pos, strTmp, uTemp);
			if(uTemp.nTriviaScore > u1.nTriviaScore){

				TRACE("1> %s %d\n", uTemp.strName, uTemp.nTriviaScore);
				u1 = uTemp;
			}
		}
		
		pos = NULL;

		if(m_aPlayers.GetSize() > 1){

			u2.nTriviaScore = -1;
			u2.strName = "";
						
			pos = m_aPlayers.GetStartPosition();

			//for(i = 0; i < m_aPlayers.GetSize(); i++){
			while(pos){

				m_aPlayers.GetNextAssoc(pos, strTmp, uTemp);

				if((uTemp.nTriviaScore > u2.nTriviaScore) &&
					(uTemp.nTriviaScore <= u1.nTriviaScore) &&
					(uTemp.strName != u1.strName)){

					TRACE("2> %s %d\n", uTemp.strName, uTemp.nTriviaScore);
					u2 = uTemp;
				}
			}
		}

		pos = NULL;

		if(m_aPlayers.GetSize() > 2){


			u3.nTriviaScore = -1;
			u3.strName = "";

			pos = m_aPlayers.GetStartPosition();

			//for(i = 0; i < m_aPlayers.GetSize(); i++){
			while(pos){

				m_aPlayers.GetNextAssoc(pos, strTmp, uTemp);

				if((uTemp.nTriviaScore > u3.nTriviaScore) &&
					(uTemp.nTriviaScore <= u2.nTriviaScore) &&
					(uTemp.strName != u2.strName) &&
					(uTemp.strName != u1.strName)){

					TRACE("3> %s %d\n", uTemp.strName, uTemp.nTriviaScore);
					u3 = uTemp;
				}
			}
		}


		FloodSafeInput(m_dwTriviaID, "Trivia UserRanking:");
		Sleep(100);

		strTmp.Format("  Place 1: %s (%02d points)", u1.strName, u1.nTriviaScore);
		FloodSafeInput(m_dwTriviaID, strTmp);
		Sleep(100);

		if(m_aPlayers.GetSize() > 1){

			strTmp.Format("  Place 2: %s (%02d points)", u2.strName, u2.nTriviaScore);
			FloodSafeInput(m_dwTriviaID, strTmp);
			Sleep(100);
		}

		if(m_aPlayers.GetSize() > 2){

			strTmp.Format("  Place 3: %s (%02d points)", u3.strName, u3.nTriviaScore);
			FloodSafeInput(m_dwTriviaID, strTmp);
			Sleep(100);
		}
		return;		
	}

	if((strMessage == "!pause") && IsAdmin(strNickname)){

		m_bPause = !m_bPause;
		if(m_bPause){

			FloodSafeInput(m_dwTriviaID, "Game has been paused.");
		}
		else{

			FloodSafeInput(m_dwTriviaID, "Game has been unpaused.");
		}
	}

	if((strMessage == "!skip") && IsAdmin(strNickname)){


		strTmp = m_strSkipText;
		strTmp.Replace("%QUES%", m_strQuestion);
		strTmp.Replace("%ANS%", m_aAnswers[0]);
		strTmp.Replace("%TIME%", itocstr(m_nTime));
		strTmp.Replace("%POINTS%", itocstr(m_nPoints));
	    strTmp.Replace("%NAME%", strNickname);
		FloodSafeInput(m_dwTriviaID, strTmp);
		m_strQuestion.Empty();
		m_aAnswers.RemoveAll();
		return;		
	}

	if((strMessage.Find("!reset") == 0) && IsAdmin(strNickname)){

		
		POSITION pos = m_aPlayers.GetStartPosition();

		while(pos){

			PLAYER uTemp;
			m_aPlayers.GetNextAssoc(pos, strTmp, uTemp);
			
			uTemp.nTriviaScore = 0;
			m_aPlayers.SetAt(uTemp.strName, uTemp);

		}

		strTmp = strMessage.Mid(6);
		strTmp.TrimLeft(); strTmp.TrimRight();
		
		if(strTmp.IsEmpty()){

			m_nRoundEnd = -1;
		}
		else{

			m_nRoundEnd		= atoi(strTmp);
		}

		if(m_nRoundEnd != -1){

			strTmp.Format("Trivia score reset. Starting new round: %d questions", m_nRoundEnd);
		}
		else{

			strTmp	= "Trivia score reset.";
		}
		
		FloodSafeInput(m_dwTriviaID, strTmp);
		m_aAnswers.RemoveAll();
		m_strQuestion.Empty();
		m_nCurrentRow = 0;
		m_strLastName.Empty();
		m_strLastWinner.Empty();
		m_nCurrentWin	= 0;
		return;		
	}

	if(m_strQuestion.IsEmpty()){


		return;
	}

	for(int i = 0; i < m_aAnswers.GetSize(); i++){

		if(m_strQuestion.IsEmpty()) break;
		if(ContainsStringExact(strMessage, m_aAnswers[i], FALSE) >= 0){
		
			//int nDone = 0;
			
			strTmp = m_strCorrect;
			strTmp.Replace("%SCORE%", itocstr(IncreaseScore(strNickname, m_nPoints))); //, &nDone)));
			strTmp.Replace("%ANS%", m_aAnswers[i]);
			strTmp.Replace("%QUES%", m_strQuestion);
			strTmp.Replace("%TIME%",	itocstr(m_nTime));
			strTmp.Replace("%POINTS%", itocstr(m_nPoints));
			strTmp.Replace("%NAME%", strNickname);

			FloodSafeInput(m_dwTriviaID, strTmp);

			if(m_strLastName == GetBaseName(strNickname)){

				m_nCurrentRow++;
				if((m_nCurrentRow % m_nRowWin) == 0){

					strTmp = m_strStreakMsg;
					strTmp.Replace("%NAME%", strNickname);
					strTmp.Replace("%STREAK%", itocstr(m_nCurrentRow));
					FloodSafeInput(m_dwTriviaID, strTmp);
				}
			}
			else{

				m_strLastName = GetBaseName(strNickname);
				m_nCurrentRow = 0;
			}

			if((m_nRoundEnd != -1) && (m_nCurrentQuestion >= m_nRoundEnd+1)){

				FinishRound(dwID); //, strNickname);
			}
			m_strQuestion.Empty();
			m_aAnswers.RemoveAll();
			break;
		}
	}
}

void CTrivia::FinishRound(DWORD dwID) //, CString strWinner)
{

	if(!m_aPlayers.GetSize()) return;

	PLAYER u1, u2, u3, uTemp;
	CString strTmp;

	u1.nTriviaScore = -1;
	u1.strName = "";

	POSITION pos = m_aPlayers.GetStartPosition();

	//for(int i = 0; i < m_aPlayers.GetSize(); i++){
	while(pos){

		m_aPlayers.GetNextAssoc(pos, strTmp, uTemp);

		if(uTemp.nTriviaScore > u1.nTriviaScore){

			TRACE("1> %s %d\n", uTemp.strName, uTemp.nTriviaScore);
			u1 = uTemp;
		}
	}

	pos = NULL;

	if(m_aPlayers.GetSize() > 1){

		u2.nTriviaScore = -1;
		u2.strName = "";

		pos = m_aPlayers.GetStartPosition();

		//for(i = 0; i < m_aPlayers.GetSize(); i++){
		while(pos){

			m_aPlayers.GetNextAssoc(pos, strTmp, uTemp);

			if((uTemp.nTriviaScore > u2.nTriviaScore) &&
				(uTemp.nTriviaScore <= u1.nTriviaScore) &&
				(uTemp.strName != u1.strName) &&
				(uTemp.nTriviaScore > 0)){

				TRACE("2> %s %d\n", uTemp.strName, uTemp.nTriviaScore);
				u2 = uTemp;
			}
		}
	}

	pos = NULL;

	if(m_aPlayers.GetSize() > 2){


		u3.nTriviaScore = -1;
		u3.strName = "";

		pos = m_aPlayers.GetStartPosition();

		//for(i = 0; i < m_aPlayers.GetSize(); i++){
		while(pos){

			m_aPlayers.GetNextAssoc(pos, strTmp, uTemp);

			if((uTemp.nTriviaScore > u3.nTriviaScore) &&
				(uTemp.nTriviaScore <= u2.nTriviaScore) &&
				(uTemp.strName != u2.strName) &&
				(uTemp.strName != u1.strName) &&
				(uTemp.nTriviaScore > 0)){

				TRACE("3> %s %d\n", uTemp.strName, uTemp.nTriviaScore);
				u3 = uTemp;
			}
		}
	}

	CString strWinner, strOut;

	if(u1.strName.IsEmpty()){

		strWinner = "No One";
		FloodSafeInput(m_dwTriviaID, "No player has won a point in this round. The house wins :-P");
	}
	else{

		strWinner = u1.strName;

		strOut.Format("%s won this round:", strWinner);
		FloodSafeInput(m_dwTriviaID, strOut);

		strOut.Format("  Place 1: %s (%02d points)", u1.strName, u1.nTriviaScore);
		FloodSafeInput(m_dwTriviaID, strOut);

		if(m_aPlayers.GetSize() > 1){

			strOut.Format("  Place 2: %s (%02d points)", u2.strName, u2.nTriviaScore);
			FloodSafeInput(m_dwTriviaID, strOut);
		}

		if(m_aPlayers.GetSize() > 2){

			strOut.Format("  Place 3: %s (%02d points)", u3.strName, u3.nTriviaScore);
			FloodSafeInput(m_dwTriviaID, strOut);
		}
	} // end ranking

	// reset the game and save stats
	//for(int i = 0; i <  m_aPlayers.GetSize(); i++){
	pos = m_aPlayers.GetStartPosition();
	while(pos){

        m_aPlayers.GetNextAssoc(pos, strTmp, uTemp);
		uTemp.nTriviaScore = 0;
		m_aPlayers.SetAt(strTmp, uTemp);
	}

	if(m_strLastWinner == GetBaseName(strWinner)){

		m_nCurrentWin++;
		if((m_nCurrentWin % m_nRoundsWin) == 0){

			strOut = m_strWinMsg;
			strOut.Replace("%NAME%", strWinner);
			strOut.Replace("%STREAK%", itocstr(m_nCurrentWin));
			FloodSafeInput(m_dwTriviaID, strOut);
		}
	}
	else{

		m_strLastWinner = strWinner;
		m_nCurrentWin	= 0;
	}

	m_strLastName.Empty();
	m_nCurrentRow = 0;
	strOut	= "Round finished. Gamemaster type !trivia [num questions] to start a new game.";
	FloodSafeInput(m_dwTriviaID, strOut);
}

void CTrivia::PrepareGame(void)
{

	if(!BuiltTriviaIndex()){

		WriteEchoText(m_dwTriviaID, "No questions found. Put your questions xml file into your RoboMX Plugins folder!\n", RGB(0, 0, 255), RGB(230, 20, 02));
		return;
	}

	m_nPoints = 0;
	m_nTime	  = 0;

	WriteEchoText(m_dwTriviaID, "Loading game settings...\n", RGB(0, 0, 255), RGB(230, 200, 0));
	m_Ini.SetIniFileName(m_strWd + "\\Trivia.ini");

	m_strQuestionText   = m_Ini.GetValue("Trivia", "Question", "%NUM%: %QUES% (%POINTS% points, %TIME% seconds)");
	m_strSkipText       = m_Ini.GetValue("Trivia", "Skip", "Question '%QUES%' has been skipped. The correct answer was '%ANS%'");
	m_strCorrect		= m_Ini.GetValue("Trivia", "Correct", "%NAME% gave the correct answer (%ANS%)");
	m_strNoAnswer		= m_Ini.GetValue("Trivia", "NoAnswer", "Question '%QUES%' has not been answered. The correct answer is '%ANS%'");
	m_strScore			= m_Ini.GetValue("Trivia", "Score", "%NAME%'s score is %POINTS% points.");
	m_strStart			= m_Ini.GetValue("Trivia", "StartMsg", "Trivia Commands: !score, !rankng");
	m_strStartAdmin     = m_Ini.GetValue("Trivia", "StartMsgAdmin", "Game Master commands: !trivia [num questions], !strivia, !pause, !hints, !skip, !reset [num questions].");
	m_strHintText		= m_Ini.GetValue("Trivia", "HintText", "Hint: %HINT%");
	
	m_bHints			= m_Ini.GetValue("Trivia", "GiveHints", FALSE);
	m_nRowWin			= m_Ini.GetValue("Trivia", "WinningStreak", 10);
	m_strStreakMsg		= m_Ini.GetValue("Trivia", "WinningStreakMsg", "%NAME% gets a six-pack of icy beer for getting the last %STREAK% questions.");
	
	m_nCurrentWin		= 0;
	m_nRoundsWin		= m_Ini.GetValue("Trivia", "RoundStreak", 4);
	if(m_nRoundsWin <= 0) m_nRoundsWin = 4;
	m_strWinMsg			= m_Ini.GetValue("Trivia", "RoundStreakMsg", "%NAME% gets a large Pizza for winning the last %STREAK% rounds");

	m_strLastName.Empty();
	m_strLastWinner.Empty();

	m_nRoundCurrent     = 0;
	m_nRoundEnd			= m_Ini.GetValue("Trivia", "QuestionsPerRound", 10);

	m_bPause = FALSE;
	

	m_strQuestion.Empty();

	WriteEchoText(m_dwTriviaID, "Initializing random number generator...\n", RGB(0, 0, 255), RGB(230, 200, 0));
    srand(GetTickCount());
	CString strOut;
	strOut.Format("Game ready (indexed %d questions)... Game Master: Type !trivia to start the round.", m_dwTotal);
	FloodSafeInput(m_dwTriviaID, strOut);
}

void CTrivia::Game(void)
{

	Sleep(50);
	
	DWORD		 dwQuestion;
	int			 nFile = 0;
	UINT		 nStartTic = 0;
	int			 nQuestion = 0;
	int nDefPoints = m_Ini.GetValue("Trivia", "Points", 1);
	if(nDefPoints <= 0) nDefPoints = 1;

	int nDefTime   = m_Ini.GetValue("Trivia", "Time", 45);
	if(nDefTime <= 0) nDefTime = 45;

	CString strTmp;

	FloodSafeInput(m_dwTriviaID, m_strStart);
	FloodSafeInput(m_dwTriviaID, m_strStartAdmin);

	while(m_bTrivia){


		if(m_strQuestion.IsEmpty() && !m_bPause){

			// Get the next question
			nFile		= rand() % (int)m_aTrivia.GetSize();
			dwQuestion	= rand() % m_aTrivia[nFile].dwNum;

			TiXmlDocument doc(m_aTrivia[nFile].strFile);

 			if(doc.LoadFile() == false){

				CString strError;
				strError.Format("Error in file %s.", m_aTrivia[nFile].strFile);
				WriteEchoText(m_dwTriviaID, strError, RGB(0, 0, 255), RGB(255,50, 50));
				continue;
			}

			TiXmlNode* mainNode = 0;
			mainNode = doc.FirstChild("config");
			TiXmlNode* triviaNode = 0, *childNode = 0;

			DWORD n = 0;

			while((triviaNode = mainNode->IterateChildren("Trivia", triviaNode)) != NULL){

				if(n++ >= dwQuestion){

					triviaNode->ToElement()->Attribute("score", (int*)&m_nPoints);
					if(m_nPoints <= 0) m_nPoints = nDefPoints;

					triviaNode->ToElement()->Attribute("time", (int*)&m_nTime);
					if(m_nTime <= 0) m_nTime = nDefTime;

					childNode = triviaNode->FirstChild("q");

					if(childNode != NULL){

						if(childNode->FirstChild() != NULL){

							m_strQuestion = childNode->FirstChild()->ToText()->Value();
						}
					}
					else continue;

					childNode = 0;
					while((childNode = triviaNode->IterateChildren("a", childNode)) != NULL){

						if(childNode->FirstChild() != NULL){
				
							m_aAnswers.Add(childNode->FirstChild()->ToText()->Value());
						}
					}
					if(m_aAnswers.IsEmpty()){
						
						m_strQuestion.Empty();
						continue;
					}
					else break;
				}  // end if(n++ >= dwQuestion)				
			}// end while(trivia)

			strTmp = m_strQuestionText;
			m_nCurrentQuestion = nQuestion;
			nQuestion++;
			strTmp.Replace("%QUES%", m_strQuestion);
			strTmp.Replace("%NUM%", itocstr(nQuestion));
			strTmp.Replace("%POINTS%", itocstr(m_nPoints));
			strTmp.Replace("%TIME%", itocstr(m_nTime));
			if(m_bHints){

				m_strCurrentHint = m_aAnswers.GetAt(0);
				int nLen = m_strCurrentHint.GetLength();
				int nBlankStart = nLen / 3;
				if(nBlankStart > 3) nBlankStart = 3;
				for(int i = nBlankStart; i < nLen; i++){

					if(m_strCurrentHint[i] != ' ') m_strCurrentHint.SetAt(i, '_');
				}
				TRACE("Hint: %s\n", m_strCurrentHint);
			}
			else{

				m_strCurrentHint.Empty();
			}
			Sleep(3000);
			FloodSafeInput(m_dwTriviaID, strTmp);
#ifdef _DEBUG
			WriteEchoText(m_dwTriviaID, m_aAnswers[0] + "\n", RGB(255,255,255), m_crBg);
#endif
			nStartTic = GetTickCount();
		
		} // end if(strQuestion.IsEmpty())
		else if(m_bHints  && m_strCurrentHint.GetLength() && ((GetTickCount() - nStartTic) >= (UINT)m_nTime*500)){

			strTmp = m_strHintText;
			strTmp.Replace("%HINT%", m_strCurrentHint);
			FloodSafeInput(m_dwTriviaID, strTmp);
			m_strCurrentHint.Empty();
		}
		else if((GetTickCount() - nStartTic) >= (UINT)m_nTime*1000){

			// time is up;
			strTmp = m_strNoAnswer;
			strTmp.Replace("%QUES%", m_strQuestion);
			strTmp.Replace("%ANS%", m_aAnswers[0]);
			strTmp.Replace("%POINTS%", itocstr(m_nPoints));
			strTmp.Replace("%TIME%", itocstr(m_nTime));

			m_strQuestion.Empty();
			m_aAnswers.RemoveAll();

			FloodSafeInput(m_dwTriviaID, strTmp);
			if((m_nRoundEnd != -1) && (m_nCurrentQuestion >= m_nRoundEnd+1)){

				FinishRound(m_dwTriviaID); //, strNickname);
				break;
			}
		}

		Sleep(10);
	}


	m_bTrivia = FALSE;
	//m_dwTriviaID = 0;
}

BOOL CTrivia::BuiltTriviaIndex(void)
{
	
	WriteEchoText(m_dwTriviaID, "Indexing questions. This could take a moment...\n", RGB(0, 0, 255), RGB(230, 200, 0));

	CFileFind finder;
	
	BOOL bResult = finder.FindFile(m_strWd + "\\*.xml");
	
	while(bResult){

		bResult = finder.FindNextFile();

		TiXmlDocument doc(finder.GetFilePath());

 		if(doc.LoadFile() == false){

			CString strError;
			if(doc.ErrorRow() == 0){

				strError.Format("Error in %s: %s\nFile can not be loaded. Please correct the error.\n", finder.GetFilePath(), doc.ErrorDesc());
			}
			else{

				strError.Format("Error in %s line %d: %s\nFile can not be loaded. Please correct the error.\n", finder.GetFilePath(), doc.ErrorRow(), doc.ErrorDesc());
			}
			WriteEchoText(m_dwTriviaID, strError, RGB(0, 0, 255), RGB(255, 100, 100));
			
			continue;
		}

		TiXmlNode* mainNode = 0;
		mainNode = doc.FirstChild("config");

		if(mainNode == NULL){
			
			CString strError;
			strError.Format("Error in file %s: The <config> supernode is missing. Is this a Trivia file?", finder.GetFilePath());
			WriteEchoText(m_dwTriviaID, strError, RGB(0, 0, 255), RGB(255, 100, 100));

			continue;
		}

		TiXmlNode* triviaNode = 0;
		std::string strBuffer;

		TRIVIA t;
		t.strFile = finder.GetFilePath();
		t.dwNum   = 0;

		while((triviaNode = mainNode->IterateChildren("Trivia", triviaNode)) != NULL){

			t.dwNum++;
		}

		m_aTrivia.Add(t);

		m_dwTotal+= t.dwNum;
	}

	CString strText;
	strText.Format("Found %u questions in %u files\n", m_dwTotal, m_aTrivia.GetSize());
	WriteEchoText(m_dwTriviaID, strText, RGB(0, 0, 255), RGB(255, 230, 0));

	
	return !m_aTrivia.IsEmpty();
}

UINT CTrivia::TriviaThread(PVOID pParam)
{

	CTrivia *pPlugin = (CTrivia*)pParam;

	ASSERT(pPlugin);

	pPlugin->Game();

	pPlugin->m_eTrivia.SetEvent();
	return 0;
}

int CTrivia::ContainsStringExact(CString strLine, CString strSubString, BOOL bCase, BOOL bExactMatch)
{
	
    if(strLine.IsEmpty()) return FALSE;
	if(strSubString.IsEmpty()) return TRUE;
	int nReturn = -1;
	BOOL bFound = FALSE;
	if(!bCase){

		strLine.MakeLower();
		strSubString.MakeLower();
	}
	if(bExactMatch){

		if(strLine == strSubString){

			return 0;
		}
		else{

			return -1;
		}
	}

	CString strSearch = strSubString;
	strSearch.Remove('*');
	
	int nIndex = -1;
	BOOL bFront, bEnd;

	if((nIndex = strLine.Find(strSearch, 0)) >= 0){

		//Check if there is a space or ( at front and end of searched item
		bEnd = (nIndex + strSearch.GetLength() >= strLine.GetLength() ? TRUE : strLine.GetAt(strSearch.GetLength() + nIndex) == ' ');
		bFront = (nIndex > 0 ? strLine.GetAt(nIndex-1) == ' ' : TRUE);

		if((strSubString.GetAt(0) == '*') && (strSubString.GetAt(strSubString.GetLength()-1) == '*')){

			bFound = TRUE;
		}
		else if((strSubString.GetAt(0) == '*') && (strSubString.GetAt(strSubString.GetLength()-1) != '*')){

			bFound = bEnd;
		}
		else if((strSubString.GetAt(0) != '*') && (strSubString.GetAt(strSubString.GetLength()-1) == '*')){

			bFound = bFront;
		}
		else{

			bFound = bFront && bEnd;
		}
		if(bFound){
			
			nReturn = nIndex;
		}
	}
	return nReturn;
}

CString CTrivia::itocstr(int n)
{

	CString str;
	str.Format("%d", n);
	return str;
}

int CTrivia::GetScore(const CString strNickname)
{

	/*
	for(int i = 0; i < m_aPlayers.GetSize(); i++){

		if(m_aPlayers[i].strName.Find(strNickname, 0) == 0){

			return m_aPlayers[i].nTriviaScore;
		}
	}*/
	int nScore = 0;
	
	PLAYER player;
	if(m_aPlayers.Lookup(GetBaseName(strNickname), player)){

		nScore = player.nTriviaScore;
	}

	return nScore;
}

BOOL CTrivia::IsAdmin(CString strNickname)
{

	strNickname = GetBaseName(strNickname);
	/*for(int i = 0; i < m_aPlayers.GetSize(); i++){

		if(m_aPlayers[i].strName.Find(strNickname, 0) == 0){

			return m_aPlayers[i].bIsAdmin;
		}
	}*/
	PLAYER player;
	if(m_aPlayers.Lookup(strNickname, player)){

		return player.bIsAdmin;
	}

	return FALSE;
}

void CTrivia::SetAdmin(CString strUser, bool bAdmin)
{

	strUser = GetBaseName(strUser);

	/*for(int i = 0; i < m_aPlayers.GetSize(); i++){

		if(m_aPlayers[i].strName.Find(strUser, 0) == 0){

			m_aPlayers[i].bIsAdmin = bAdmin;
			return;
		}
	}*/

	PLAYER player;

	if(m_aPlayers.Lookup(strUser, player)){

		player.bIsAdmin = bAdmin;
	}
	else{

		player.strName = strUser;
		player.bIsAdmin = bAdmin;
		player.nTriviaScore = 0;
	}

	m_aPlayers.SetAt(strUser, player);
}

int CTrivia::IncreaseScore(CString &strUser, int nPoints) //, PINT nDone)
{

	int nResult = -1;

	PLAYER player;

	if(m_aPlayers.Lookup(GetBaseName(strUser), player)){

		player.nTriviaScore += nPoints;
		
		nResult = player.nTriviaScore;
	}
	else{

		player.strName		= GetBaseName(strUser);
		player.nTriviaScore = nPoints;
		player.bIsAdmin		= FALSE;
	}

	m_aPlayers.SetAt(player.strName, player);

	/*for(int i = 0; i < m_aPlayers.GetSize(); i++){

		//if(m_aPlayers[i].strName.Find(GetBaseName(strUser), 0) == 0){
		i

			m_aPlayers[i].nTriviaScore += nPoints;
			nResult =  m_aPlayers[i].nTriviaScore;
			break;
		}
	}

	if(nResult == -1){

		PLAYER u;
		u.nTriviaScore = nPoints;
		u.strName = GetBaseName(strUser);
		m_aPlayers.Add(u);
		nResult = nPoints;
	}*/

/*	if(m_nRoundEnd != -1){

		*nDone = nResult >= m_nRoundEnd;
	}
	else{

		*nDone = 0;
	}*/
	
	return nResult;
}


CString CTrivia::GetBaseName(CString strUser)
{

	CString strBase;
	
	int n = strUser.ReverseFind('_');

	if((n > 0) && (strUser.GetLength() - n == 6)){

       strBase  = strUser.Left(strUser.GetLength() - 6);
	}
	else{

		strBase = strUser;
	}

	return strBase;
}


void CTrivia::FloodSafeInput(DWORD dwID, const CString strText)
{

	if(m_dwFloodTick - GetTickCount() < 3000){

		// if the last message has been input in the last 3 seconds 
		// raise the counter
		m_dwFloodCounter++;
	}
	else{

		// we are safe, reset the counter
		m_dwFloodCounter = 0;
	}

	if(m_dwFloodCounter > 3){

		// We need to wait for the next output or we will get kicked
		// After that we can reset the counter and input the message
		Sleep(3000);
		m_dwFloodCounter = 0;
	}

	m_dwFloodTick = GetTickCount();
	InputMessage(dwID, strText);
}
