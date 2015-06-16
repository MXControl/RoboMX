#include "StdAfx.h"
#include ".\plugin.h"

#include "..\Ini.h"
#include "..\Tokenizer.h"

#include "ConfigDlg.h"


CPlugin::CPlugin(void)
{

	m_strName			= "iceAnnounce.rEx";
	m_strDescription	= "Icecast2 Stream-Announcer v1.0";
	m_strAuthor			= "Bender979";
	crColor				= RGB(144, 144, 144);
	crBg				= RGB(255, 255, 255);
	m_bRun				= FALSE;
	m_nInterval			= 5000;
	m_strAnnouncement	= "/me listens to %SONG%\r\n/me streams on %URL%\r\n/me Listeners %LISTENERS% - Stream type: %STREAMTYPE%";
	m_strStreamURL		= "http://192.168.0.105:8080/stream.ogg.m3u";
	m_strStart          = "Icecast2 Stream-Announcer started.";
	m_strStop           = "Icecast2 Stream-Announcer stopped.";
	m_strStatURL		= "http://192.168.0.105:8080";
}

CPlugin::~CPlugin(void)
{
}

void CPlugin::OnMessage(DWORD dwID, CString* pUser, CString* pMsg, BOOL bIsAction)
{

}

void CPlugin::Init()
{

	TCHAR szBuffer[_MAX_PATH]; 
	::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH);
	m_strWd.Format("%s", szBuffer);
	m_strWd = m_strWd.Left(m_strWd.ReverseFind('\\'));
	LoadCfg();
}

void CPlugin::Quit()
{

}

void CPlugin::OnInputHook(DWORD dwIP, CString *pInput)
{

    if(pInput->IsEmpty()) return;

	if(pInput->CompareNoCase("/ice start") == 0){

		if(!m_bRun){

			m_strSong.Empty();
			m_eDone.ResetEvent();
			m_bRun = TRUE;
			GetRoom(dwIP)->bAnnounce = TRUE;
			AfxBeginThread(MonitorThread, (LPVOID)this, THREAD_PRIORITY_NORMAL);
			WriteEchoText(dwIP, "Started Icecast2 Stream-Announcer.\n", crColor, crBg);
		}
		else{

			WriteEchoText(dwIP, "Icecast2 Stream-Announcer already running.\n", crColor, crBg);
		}
		pInput->Empty();
	}
	else if(pInput->CompareNoCase("/ice stop") == 0){

		if(m_bRun){

			m_bRun = FALSE;
			WaitForSingleObject(&m_eDone, INFINITE);
			InputInActive(m_strStop);
			WriteEchoText(dwIP, "Stopped Icecast2 Stream-Announcement.\n", crColor, crBg);
			m_eDone.ResetEvent();
			m_strSong.Empty();
			for(int i = 0; i < m_aRooms.GetSize(); i++){

				m_aRooms[i].bAnnounce = FALSE;
			}
		}
		else{

			WriteEchoText(dwIP, "Icecast2 Stream-Announcer is not running.\n", crColor, crBg);
		}
		pInput->Empty();
	}
	else if(pInput->CompareNoCase("/ice add") == 0){

		if(m_bRun){

			GetRoom(dwIP)->bAnnounce = TRUE;
			WriteEchoText(dwIP, "iceAnnounce: Added room to list.\n", crColor, crBg);
		}
		else{

			WriteEchoText(dwIP, "Start iceAnnounce with /ice start first.\n", crColor, crBg);
		}
		pInput->Empty();
	}
	else if(pInput->CompareNoCase("/ice rem") == 0){

		if(m_bRun){

			GetRoom(dwIP)->bAnnounce = FALSE;
			WriteEchoText(dwIP, "iceAnnounce: Removed room to list.\n", crColor, crBg);
		}
		else{

			WriteEchoText(dwIP, "Start iceAnnounce with /ice start first.\n", crColor, crBg);
		}
		pInput->Empty();
	}
	else if(pInput->CompareNoCase("/help") == 0){

		WriteEchoText(dwIP, "Icecast2 Stream Announcement Plugin Version 1.0\n", crColor, crBg);
		WriteEchoText(dwIP, "Type /help ice  for more command listing\n", crColor, crBg);
	}
	else if(pInput->CompareNoCase("/help ice") == 0){

		WriteEchoText(dwIP, "Icecast2 Stream Announcement Plugin Version 1.0\n", crColor, crBg);
		WriteEchoText(dwIP, "Available Commands:\n", crColor, crBg);
		WriteEchoText(dwIP, "/ice start - Start Announcement\n", crColor, crBg);
		WriteEchoText(dwIP, "/ice stop  - Stop Announcement\n", crColor, crBg);
		WriteEchoText(dwIP, "/ice add  - Add a room to announcement list\n", crColor, crBg);
		WriteEchoText(dwIP, "/ice rem  - Remove a room to to annoucement list\n", crColor, crBg);
		WriteEchoText(dwIP, "For further configuration go to View->Settings->Extensions select the Icecast plugin and click on configure.\n\n", crColor, crBg);
		pInput->Empty();
	}
}

BOOL CPlugin::QueryServer(void)
{

	CInternetSession	is("iceAnnounce for RoboMX", 1, INTERNET_OPEN_TYPE_DIRECT, NULL,
		                    NULL, INTERNET_FLAG_DONT_CACHE);
	CString				strTmp, strContent;
	
	try{

		
		CHttpFile* pFile = (CHttpFile*) is.OpenURL(m_strStatURL, 1, INTERNET_FLAG_TRANSFER_ASCII|INTERNET_FLAG_RELOAD|INTERNET_FLAG_DONT_CACHE, 0, 0);
		
		if(pFile != NULL){

			while(pFile->ReadString(strTmp)){

				strContent += strTmp;
			}
			pFile->Close();
			delete pFile;
		}

		is.Close();
	}
	catch(CInternetException* pEx){
		
		TCHAR   szCause[255];
		CString strFormatted;

		pEx->GetErrorMessage(szCause, 255);
		strFormatted.Format("Error during query: %s\n", szCause);
		TRACE(strFormatted);
		return 0;
	}


	// We have the string
	// no parse out the info
	strContent.Replace("<td>", "");
	strContent.Replace("<b>", "");
	strContent.Replace("</td>", "");
	strContent.Replace("</font>", "");
	strContent.Replace("</tr>", "");
	strContent.Replace("<font class=\"default2\">", "");
	strContent.Replace("<td width=\"100\">=\"default2\">", "");

    int nStart = strContent.Find("Stream Type: ", 0);
	int nEnd = 0;

	if(nStart == -1){

		m_strStreamType = "Unknown";
	}
	else{
		
		nStart+=13;
		nEnd = strContent.Find("</b>", nStart+1);
		m_strStreamType = strContent.Mid(nStart, nEnd-nStart);
	}

    nStart = strContent.Find("Stream Listeners: ", nEnd);
	if(nStart == -1){

		m_strListeners = "0";
	}
	else{

		nStart+=18;
		nEnd = strContent.Find("</b>", nStart+1);
		m_strListeners = strContent.Mid(nStart, nEnd-nStart);
	}

	CString strSong;
	
    nStart = strContent.Find("Current Song: ", nEnd);
	if(nStart == -1){

		strSong = "Stream not active";
	}
	else{
		
		nStart+=14;
		nEnd = strContent.Find("</b>", nStart+1);
		strSong = strContent.Mid(nStart, nEnd-nStart);
    }
	
	if(strSong == m_strSong) return 0;

	m_strSong = strSong;

	CString strAnnounce = m_strAnnouncement;

	strAnnounce.Replace("%LISTENERS%", m_strListeners);
	strAnnounce.Replace("%SONG%", m_strSong);
	strAnnounce.Replace("%STREAM-TYPE%", m_strStreamType);
	strAnnounce.Replace("%STREAM-URL%", m_strStreamURL);
	strAnnounce.Replace("%HOME-URL%", m_strStatURL);

	CTokenizer token(strAnnounce, "\r\n");

	while(token.Next(strTmp)){

		InputInActive(strTmp);
		Sleep(100);
	}

	return 1;
}

UINT CPlugin::MonitorThread(LPVOID pParam)
{

	CPlugin* pPlugin = (CPlugin*)pParam;

	ASSERT(pPlugin);

	pPlugin->InputInActive(pPlugin->m_strStart);

	while(pPlugin->m_bRun){

		pPlugin->QueryServer();
		Sleep(pPlugin->m_nInterval);
	}

	TRACE("Setting done event\n");
	pPlugin->m_eDone.SetEvent();

	return 0;
}

void CPlugin::Configure()
{

	CConfigDlg dlg;
	dlg.m_strMsg	= m_strAnnouncement;
	dlg.m_strStart	= m_strStart;
	dlg.m_strStop	= m_strStop;
	dlg.m_strStats	= m_strStatURL;
	dlg.m_strURL	= m_strStreamURL;

	if(dlg.DoModal() == IDOK){

		m_strAnnouncement	= dlg.m_strMsg ;
		m_strStart			= dlg.m_strStart;
		m_strStop			= dlg.m_strStop;
		m_strStatURL		= dlg.m_strStats;
		m_strStreamURL		= dlg.m_strURL;
		SaveCfg();
	}
}
void CPlugin::SaveCfg(void)
{

	CIni ini;
	ini.SetIniFileName(m_strWd + "\\Add-Ons\\iceAnnounce.ini");

	CString strTmp = m_strAnnouncement;
	strTmp.Replace("\r\n", "\\r\\n");
	ini.SetValue("Conf", "Announce", strTmp);

	ini.SetValue("Conf", "Start", m_strStart);
	ini.SetValue("Conf", "Stop", m_strStop);
	ini.SetValue("Conf", "URL", m_strStreamURL);
	ini.SetValue("Conf", "Stats", m_strStatURL);
}

void CPlugin::LoadCfg(void)
{

	CIni ini;
	ini.SetIniFileName(m_strWd + "\\Add-Ons\\iceAnnounce.ini");

	m_strAnnouncement = ini.GetValue("Conf", "Announce", m_strAnnouncement);
	m_strAnnouncement.Replace("\\r\\n", "\r\n");

	m_strStart = ini.GetValue("Conf", "Start", m_strStart);
	m_strStop = ini.GetValue("Conf", "Stop", m_strStop);
	m_strStreamURL = ini.GetValue("Conf", "URL", m_strStreamURL);
	m_strStatURL = ini.GetValue("Conf", "Stats", m_strStatURL);
}

void CPlugin::OnJoinChannel(DWORD dwID, CString strChannel, CUserMap *pUserMap)
{

	ROOMDATA room;
	room.dwID = dwID;
	room.bAnnounce = FALSE;
	m_aRooms.Add(room);
}

void CPlugin::OnLeaveChannel(DWORD dwID)
{

	for(int i = 0; i < m_aRooms.GetSize(); i++){

		if(m_aRooms[i].dwID == dwID){

			m_aRooms.RemoveAt(i);
			return;
		}
	}
}

ROOMDATA* CPlugin::GetRoom(DWORD dwID)
{

	for(int i = 0; i < m_aRooms.GetSize(); i++){

		if(m_aRooms[i].dwID == dwID)
			return &m_aRooms[i];
	}
	return NULL;
}

void CPlugin::InputInActive(CString strText)
{

	for(int i = 0; i < m_aRooms.GetSize(); i++){

		if(m_aRooms[i].bAnnounce){

			InputMessage(m_aRooms[i].dwID, strText);
		}
	}
}
