/*
 * RoboMX - Chatclient for WinMX.
 * Copyright (C) 2003-2004 by Bender
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Contact: Bender979@yahoo.com
 *
*/

#include "stdafx.h"
#include "RoboMX.h"
#include "Settings.h"
#include "Ini.h"
#include "Tokenizer.h"
#include "RichElement.h"
#include "SystemInfo.h"
#include ".\settings.h"
#include "RenameDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

extern CSystemInfo g_sSystem;

CSettings::CSettings()
{

	m_bAutoWPN		= TRUE;
	m_bSavePos		= TRUE;
	m_bMsBold		= TRUE;
	m_bMsItalic		= FALSE;
	m_bMsLine		= FALSE;
	m_bColorAcName	= FALSE;
	m_bTime			= TRUE;
	m_nDepth		= 50;
	m_bHistory		= TRUE;
	m_nFontSize		= 12;
	m_bUpdate		= TRUE;
	m_bAutoList		= TRUE;
	m_bMaxi			= FALSE;	
	m_bDisplayNode	= FALSE;
	m_strFont		= "Arial";
	m_bBarTop		= TRUE;

	m_dwFiles		= 1;
	m_wLine			= 0;

	m_dwTextStyle = retfColor|retfBold;

	m_crBg = RGB(255,255,255);
	m_bForcePrimary	 = FALSE;

	m_FA = "";
	m_FM = "";
	m_EA = "";
	m_EM = ":";

	m_bRetry	= TRUE;
	m_nRetries	= 3;
	m_bCacheRooms	= TRUE;
	m_bCacheFavs    = TRUE;

	LARGE_INTEGER tick;
	if(QueryPerformanceCounter(&tick)){

		srand(tick.LowPart);
	}
	else{

		srand(GetTickCount());
	}
	m_nToolEffect = 0;
	m_crToolBg1 = 0;
	m_crToolBg2 = 0;
	m_crToolBg3 = 0;
	m_nToolGran = 5;
	m_nToolTrans = 0;
	m_dwIdleDuration = 15*1000*60;
}

CSettings::~CSettings()
{

}

void CSettings::Load()
{

	TCHAR szBuffer[_MAX_PATH]; 
	::GetModuleFileName(AfxGetInstanceHandle(), szBuffer, _MAX_PATH);
	m_strWd.Format("%s", szBuffer);
	m_strWd = m_strWd.Left(m_strWd.ReverseFind('\\'));

	SetIniFile(m_strWd + "\\RoboMX.ini");

	CIni ini;
	ini.SetIniFileName(m_strIniFile);

	m_strNickname = ini.GetValue("UserInfo", "Nickname", "");
	m_dwFiles	  = ini.GetValue("UserInfo", "Files", 1);
	m_wLine		  = ini.GetValue("UserInfo", "Line", 0);

	if(m_strNickname.IsEmpty()){

		char user[256];
		DWORD dwSize = sizeof(user);
		if(GetUserName((char*)&user, &dwSize) && g_sSystem.IsWindowsNT()){

			m_strNickname = (LPCTSTR)user;
			
			if(m_strNickname.GetLength() > 30)
				m_strNickname.Truncate(30);
				//m_strNickname = m_strNickname.Left(30);
			
			m_strNickname.Replace(" ", ".");

			m_strNickname += "..100";
		}
		if(m_strNickname.IsEmpty()){ // still empty?

			m_strNickname = "RoboMX..100";
		}

		CRenameDlg dlg;
		dlg.m_strName   = m_strNickname;
		dlg.m_dwFiles   = m_dwFiles;
		dlg.m_nLine     = m_wLine;
		dlg.m_bAllRooms = TRUE;
		dlg.m_bWPNOnly  = 2;
		dlg.DoModal();
        m_strNickname = dlg.m_strName;
		m_dwFiles	  = dlg.m_dwFiles;
		m_wLine		  = (WORD)dlg.m_nLine;
	}

	if(m_wLine > 8) m_wLine = 0;
	if(m_dwFiles > 65535) m_dwFiles = 1;
	
	m_bUpdate				= ini.GetValue("General", "CheckUpdate", TRUE);
	m_strLogDir				= ini.GetValue("General", "LogDir", m_strWd + "\\Logs");
	m_bLog					= ini.GetValue("General", "Log", FALSE);
	m_bLoadLog			    = ini.GetValue("General", "LoadLogOnEnter", FALSE);
	m_bRetry				= ini.GetValue("General", "ConnectionRetry", TRUE);
	m_nRetries				= ini.GetValue("General", "ConnectionRetries", 3);
	m_bAutoWPN				= ini.GetValue("General", "AutoWPN", TRUE);
	m_bForcePrimary			= ini.GetValue("General", "ForcePrimary", FALSE);
	m_strForcedNode			= ini.GetValue("General", "ForcedNode", "localhost:6699");
	m_strNodeExternal		= ini.GetValue("General", "ForcedExt", "");
	m_bAllowTrustedPMOnly	= ini.GetValue("General", "AllowTrustedPMOnly", FALSE);
    m_bCacheRooms			= ini.GetValue("General", "CacheRoomList", TRUE);
	m_bCacheFavs			= ini.GetValue("General", "CacheFavsOnly", TRUE);
	m_bHiliteTop			= ini.SetValue("General", "KeepHilitedRoomsTop", TRUE);
	m_bShowExpert			= ini.GetValue("General", "ShowExpertOptions", FALSE);

	m_bSepOnSwitch  = ini.GetValue("Look & Feel", "SepOnSwitch", FALSE);
	m_bSavePos		= ini.GetValue("Look & Feel", "SaveWinPos", TRUE);
	m_bAutoList		= ini.GetValue("Look & Feel", "AutoList", TRUE);
	m_bMaxi			= ini.GetValue("Look & Feel", "AlwaysMaximizeMDI", FALSE);	
	m_bHideSystem	= ini.GetValue("Look & Feel", "AutoHideSystem", TRUE);
	m_bShowFillStatus = ini.GetValue("Look & Feel", "ShowFillStatus", FALSE);

	m_bMiniTray		= ini.GetValue("Look & Feel", "Mini2Tray", FALSE);
	m_bTime			= ini.GetValue("Look & Feel", "Timestamp", TRUE);
	m_bBarTop		= ini.GetValue("Look & Feel", "BarTop", TRUE);

	m_FA    = ini.GetValue("Look & Feel", "EnActFront", "");
	m_FM    = ini.GetValue("Look & Feel", "EnMsgFront", "");
	m_EA    = ini.GetValue("Look & Feel", "EnActEnd", "<");
	m_EM    = ini.GetValue("Look & Feel", "EnMsgEnd", ">");

	m_FA.Replace(" ", " ");  // replace Alt+0160 with regular space
	m_FM.Replace(" ", " ");  // replace Alt+0160 with regular space
	m_EA.Replace(" ", " ");  // replace Alt+0160 with regular space
	m_EM.Replace(" ", " ");  // replace Alt+0160 with regular space

	m_strModeNormal	= ini.GetValue("Userlist", "UserNormal", m_strWd + "\\gfx\\user_normal.bmp");
	m_strModeVoiced = ini.GetValue("Userlist", "UserVoiced", m_strWd + "\\gfx\\user_voiced.bmp");
	m_strModeAdmin	= ini.GetValue("Userlist", "UserAdmin", m_strWd + "\\gfx\\user_admin.bmp");
	m_strModeHost	= ini.GetValue("Userlist", "UserHost", m_strWd + "\\gfx\\user_host.bmp");
	m_wUserlistMode	= ini.GetValue("Userlist", "DisplayMode", 0);
	m_strUserlistBg	= ini.GetValue("Userlist", "BgImage", "");
	m_bUserUserlistBg = ini.GetValue("Userlist", "UseImage", FALSE);

    m_bSoundFX		= ini.GetValue("Look & Feel", "SoundFX", FALSE);
	m_strImage		= ini.GetValue("Look & Feel", "BG-Image", "");
	m_bImage		= ini.GetValue("Look & Feel", "UseImage", m_strImage.IsEmpty() ? FALSE : TRUE);
	m_bPing			= ini.GetValue("Look & Feel", "ShowPing", FALSE);
	m_bDisplayNode	= ini.GetValue("Look & Feel", "DisplayNode", FALSE);
	m_bPMToolTip	= ini.GetValue("Look & Feel", "PMToolTip", TRUE);

	m_nTimeFormat	= ini.GetValue("Look & Feel", "TimeFormat", 0);
	m_bHiliteUsers  = ini.GetValue("Look & Feel", "HiliteUsers", FALSE);
	m_bEmoticons    = ini.GetValue("Look & Feel", "Emoticons", TRUE);
	m_bBlockMCMA    = ini.GetValue("Look & Feel", "BlockMCMA", FALSE);
	m_bMCMARawname	= ini.GetValue("Look & Feel", "MCMAShowRawname", FALSE);
	m_bOpsTop	    = ini.GetValue("Look & Feel", "OperatorsTop", TRUE);

	m_nDepth		= ini.GetValue("History", "Depth", 50);
	m_bHistory		= ini.GetValue("History", "Enable", TRUE);

	m_bScroller		= ini.GetValue("Messages", "EnableScroller", TRUE);
	m_strVideoMsg	= ini.GetValue("Messages", "WinampVideo", "/me watches '%WA-ARTIST% - %WA-SONG%' [%WA-ELATIME% - %WA-TOTALTIME%]");
	m_strWinampMsg	= ini.GetValue("Messages", "Winamp", "/me listens to '%WA-ARTIST% - %WA-SONG%' [%WA-ELATIME% - %WA-TOTALTIME%]");
	m_bOnEnter		= ini.GetValue("Messages", "OnEnterEnable", FALSE);
	m_strJoin		= ini.GetValue("Messages", "JoinMessage", "%NAME% (%LINE%, %FILES% files) has entered %IPBRACKETS%");
	m_strPart		= ini.GetValue("Messages", "PartMessage", "%NAME% has left");
	m_strPrivacyMsg		= ini.GetValue("Messages", "PM-Privacy", "Message blocked due to privacy settings.");
	m_strIgnoredMsg		= ini.GetValue("Messages", "PM-Ignored", "User is ignoring you.");
	m_strPMAway			= ini.GetValue("Messages", "PM-Away", "");
	m_bShowURLWarning	= ini.GetValue("Warnings", "URLWarning", TRUE);
	m_bShowExitConfirm	= ini.GetValue("Warnings", "Exit", TRUE);
	
	// Format stuff
	m_bMsBold   = ini.GetValue("Font", "MessageBold", TRUE);
	m_bMsItalic = ini.GetValue("Font", "MessageItalic", FALSE);
	m_bMsLine   = ini.GetValue("Font", "MessageLine", FALSE);
	m_strFont   = ini.GetValue("Font", "ChatFont",   "Arial");
	m_nFontSize = ini.GetValue("Font", "ChatFontSize", 13);
	
	switch(ini.GetValue("Font", "Charset", 0)){

	case 1:
		m_nCharset = BALTIC_CHARSET;
		break;
	case 2:
		m_nCharset = CHINESEBIG5_CHARSET;
		break;
	case 3:
		m_nCharset = DEFAULT_CHARSET;
		break;
	case 4:
		m_nCharset = EASTEUROPE_CHARSET;
		break;
	case 5:
		m_nCharset = GB2312_CHARSET;
		break;
	case 6:
		m_nCharset = GREEK_CHARSET;
		break;
	case 7:
		m_nCharset = HANGEUL_CHARSET;
		break;
	case 8:
		m_nCharset = MAC_CHARSET;
		break;
	case 9:
		m_nCharset = OEM_CHARSET;
		break;
	case 10:
		m_nCharset = RUSSIAN_CHARSET;
		break;
	case 11:
		m_nCharset = SHIFTJIS_CHARSET;
		break;
	case 12:
		m_nCharset = SYMBOL_CHARSET;
		break;
	case 13:
		m_nCharset = TURKISH_CHARSET;
		break;
	case 0:
	default:
		m_nCharset = ANSI_CHARSET;
		break;
	}

	m_bColorAcName = ini.GetValue("Colors", "ColorActionName", FALSE);

	m_dwTextStyle = retfColor;
	if(m_bMsBold)	m_dwTextStyle |= retfBold;
	if(m_bMsItalic) m_dwTextStyle |= retfItalic;
	if(m_bMsLine)	m_dwTextStyle |= retfUnderline;

	LoadColorsFromFile(m_strIniFile);

	m_strSfxPM			= ini.GetValue("SoundFX", "PM", m_strWd + "");
	m_strSfxJoin		= ini.GetValue("SoundFX", "Join", m_strWd + "");
	m_strSfxPart		= ini.GetValue("SoundFX", "Part", m_strWd + "");
	m_strSfxRedirect	= ini.GetValue("SoundFX", "Redirect", m_strWd + "");
	m_strSfxTopic		= ini.GetValue("SoundFX", "Topic", m_strWd + "");
	m_strSfxMotd		= ini.GetValue("SoundFX", "Motd", m_strWd + "");
	m_strSfxStart		= ini.GetValue("SoundFX", "Start", m_strWd + "");
	m_strSfxError		= ini.GetValue("SoundFX", "Error", m_strWd + "");
	m_bChatSfx			= ini.GetValue("SoundFX", "ChatSfx", FALSE);
	m_bSoundFX			= ini.GetValue("SoundFX", "Sfx", FALSE);

	

	m_strToolList		= ini.GetValue("ChannelToolTips", "List", "<table cellpadding=\"0\" cellspacing=\"0\"><tr>\r\n<td bgcolor=\"#0000aa\">%LISTICON%</td>\r\n<td bgcolor=\"#0000AA\"><right><b><font color=\"white\">Channellist</font></b></right></td>\r\n</tr>\r\n<tr>\r\n<td> </td>\r\n<td>\r\n<t>%NUMCHANNELS%<t>channels<br>\r\n<t>%NUMFILTERED%<t>filtered\r\n</td>\r\n</tr>\r\n</table>", 2048);
	m_strToolListHover  = ini.GetValue("ChannelToolTips", "ListHover", "<table cellpadding=\"2\" cellspacing=\"2\" align=\"left\">\r\n<tr>\r\n<td>\r\n%ROOMIMAGE% \r\n</td>\r\n<td><table cellpadding=\"0\" cellspacing=\"0\" align=\"left\"><tr>\r\n<td bgcolor=\"#0000cc\"><font color=\"white\"><b>%ROOMSHORT%</b></font></td></tr></table>\r\n<b>Topic: </b><t><t>%TOPIC%<br>\r\n<hr><br>\r\n<b>Last Joined:</b><t>%LASTJOIN%<br>\r\n<b>Joined:</b><t><t>%JOINCOUNT% times<br>\r\n<b>Users:</b><t><t>%NUMUSERS%<br>\r\n<b>Maximum:</b><t>%MAXUSERS%<br>\r\n<b>Login:</b><t><t>%LOGIN%<br>\r\n<b>Ping:</b><t><t>%PING%<br>\r\n</td>\r\n</tr>\r\n</table>\r\n", 2048);
	m_strToolRoom		= ini.GetValue("ChannelToolTips", "Room", "<table cellpadding=\"2\" cellspacing=\"2\" align=\"left\">\r\n<tr>\r\n<td>\r\n%ROOMIMAGE%\r\n</td>\r\n<td><table cellpadding=\"0\" cellspacing=\"0\" align=\"left\"><tr><td bgcolor=\"#0000cc\"><font color=\"white\"><b>%ROOMSHORT%</b></font></td></tr></table>\r\n<b>Joined at:</b><t><t>%JOINTIME%<br>\r\n<b>Server: </b><t><t>%SERVERBRAND%<br>\r\n<hr><br>\r\n<b>Average Lag:</b><t>%AVLAG% ms<br>\r\n<b>Messages:</b><t>%TOTALMSGS%<br>\r\n<b>Your Msgs:</b><t>%YOURMSGS%<br>\r\n<b>Msgs/Secs:</b><t>%MSGPERSEC%<br>\r\n</td>\r\n</tr>\r\n</table>", 2048);
	m_strToolSettings	= ini.GetValue("ChannelToolTips", "Settings", "<table cellpadding=\"0\" cellspacing=\"0\">\r\n<tr>\r\n<td bgcolor=\"#0000CC\">%SETTINGSICON%</td>\r\n<td bgcolor=\"#0000CC\" align=\"right\">\r\n<font color=\"white\"><b>Settings</b></font>\r\n</td>\r\n</tr>\r\n</table><br>Adjust settings of RoboMX\r\n", 2048);
	m_strToolPM			= ini.GetValue("ChannelToolTips", "PM", "<table cellpadding=\"0\" cellspacing=\"0\" align=\"left\">\\r\\n<tr><td>%USERIMAGE% </td><td>\\r\\n<table cellpadding=\"0\" cellspacing=\"0\" align=\"left\"  bgcolor=\"#0000cc\">\\r\\n<tr>\\r\\n<td valign=\"top\">%PMICON%</td>\\r\\n<td valign=\"top\"><font color=\"white\"><b>%NAME%</b></font></td>\\r\\n</tr>\\r\\n</table>\\r\\n<br><font color=\"white\">\\r\\n<b>Private Message</b><br><br>\\r\\n<t>%MSGCOUNT%<t>Msgs. recieved<br>\\r\\n<t>%MSGSENTCOUNT%<t>Msgs. sent<br>\\r\\n</font></td></tr>\\r\\n</table>\\r\\n", 2048);
	m_strToolFiles		= ini.GetValue("ChannelToolTips", "Files", "<table cellpadding=\"0\" cellspacing=\"0\">\r\n<tr>\r\n<td bgcolor=\"#0000aa\">%FILEICON%</td>\r\n<td bgcolor=\"#0000AA\"><right><b><font color=\"white\">Files %NAME%</font></b></right></td>\r\n</tr><tr>\r\n<td> </td>\r\n<td>\r\n<t>%NUMFILES%<t>files<br>\r\n<t>%NUMAUDIO%<t>audio files<br>\r\n<t>%NUMVIDEO%<t>video files<br>\r\n<t>%NUMIMG%<t>image files<br>\r\n<t>%NUMOTHER%<t>other files\r\n</td></tr>\r\n</table>", 2048);

	m_nToolEffect		= ini.GetValue("ChannelToolTips", "Effect", 0);
	m_crToolBg1			= ini.GetValue("ChannelToolTips", "Bg1", RGB(204, 216, 255));
	m_crToolBg2			= ini.GetValue("ChannelToolTips", "Bg2", RGB(204, 216, 255));
	m_crToolBg3			= ini.GetValue("ChannelToolTips", "Bg3", RGB(204, 216, 255));
	m_nToolGran			= ini.GetValue("ChannelToolTips", "Gran", 5);
	m_nToolTrans		= ini.GetValue("ChannelToolTips", "Trans", 0);
	m_bOldTools			= ini.GetValue("ChannelToolTips", "UseOldStyleTools", 0);
	m_strDefaultChannelImage = ini.GetValue("ChannelToolTips", "DefChannelImg", "");
	
	m_strDefaultUserImage  = ini.GetValue("UserToolTips", "DefUserImg", "");

	m_strUserTool		= ini.GetValue("UserToolTips", "UserToolHTML", "<table cellpadding=\"0\" cellspacing=\"0\" align=\"left\">\r\n<tr>\r\n<td>\r\n%USERIMAGE%\r\n</td>\r\n<td><table cellpadding=\"0\" cellspacing=\"0\" align=\"left\"><tr><td bgcolor=\"#0000cc\"><font color=\"white\"><b>%NAME%</b></font></td></tr></table>\r\n<br>\r\n<b>Files:</b><t><t>%FILES%<br>\r\n<b>Line:</b><t><t><t>%LINE%<br>\r\n<b>Joined:</b><t><t>%JOINTIME%<br>\r\n<b>Idle Time:</b><t>%IDLETIME%<br>\r\n<hr><br>\r\n<b>IP:</b><t><t><t>%IP%<br>\r\n<b>Hostname:</b><t>%HOSTNAME%<br>\r\n</td>\r\n</tr>\r\n</table>\r\n", 2048);
	m_nUserToolEffect	= ini.GetValue("UserToolTips", "Effect", 0);
	m_crUserToolBg1		= ini.GetValue("UserToolTips", "Bg1", RGB(255, 235, 204));
	m_crUserToolBg2		= ini.GetValue("UserToolTips", "Bg2", RGB(255, 235, 204));
	m_crUserToolBg3		= ini.GetValue("UserToolTips", "Bg3", RGB(255, 235, 204));
	m_nUserToolGran		= ini.GetValue("UserToolTips", "Gran", 5);
	m_nUserToolTrans	= ini.GetValue("UserToolTips", "Trans", 0);
	m_bUserOldTools		= ini.GetValue("UserToolTips", "UseOldStyleTools", 0);
	m_bTile				= ini.GetValue("UserToolTips", "TileBg", TRUE);
	m_nXOffset			= ini.GetValue("UserToolTips", "X-Offset", 0);
	m_nYOffset			= ini.GetValue("UserToolTips", "Y-Offset", 0);
		
	m_strUserTool.Replace("\\r", "\r");
	m_strUserTool.Replace("\\n", "\n");
	m_strToolList.Replace("\\r", "\r");
	m_strToolList.Replace("\\n", "\n");
	m_strToolRoom.Replace("\\r", "\r");
	m_strToolRoom.Replace("\\n", "\n");
	m_strToolSettings.Replace("\\r", "\r");
	m_strToolSettings.Replace("\\n", "\n");
	m_strToolListHover.Replace("\\r", "\r");
	m_strToolListHover.Replace("\\n", "\n");
	m_strToolPM.Replace("\\r", "\r");
	m_strToolPM.Replace("\\n", "\n");
	m_strToolFiles.Replace("\\r", "\r");
	m_strToolFiles.Replace("\\n", "\n");

	m_dwNodeIP			= ini.GetValue("Node", "IP", 2130706433);
	m_dwNodePort		= ini.GetValue("Node", "Port", 14223);

#ifdef _ROBO_READER
	m_strMale		= ini.GetValue("Voice", "Male", m_strWd + "\\gfx\\char_Male.bmp");
	m_strFemale	    = ini.GetValue("Voice", "Female", m_strWd + "\\gfx\\char_Female.bmp");
#endif


	m_dwIdleDuration = ini.GetValue("AwayControl", "IdleDuration", 15*60*1000);
	m_bAwayIfIdle    = ini.GetValue("AwayControl", "AutoIfIdle", 0);
	m_strAutoReason  = ini.GetValue("AwayControl", "AutoReason", "Fell asleep on the keyboard");
	m_strAutoPrefix	 = ini.GetValue("AwayControl", "AutoPrefix", "");
	m_strAutoSuffix	 = ini.GetValue("AwayControl", "AutoSuffix", "");
	m_bAutoBack      = ini.GetValue("AwayControl", "AutoBack", 0);

	LoadHiLite();
	LoadRCMS();
	LoadSounds();
	LoadUsers();
}

void CSettings::Save()
{

	ASSERT(!m_strIniFile.IsEmpty());
	
	CIni ini;
	ini.SetIniFileName(m_strIniFile);

	ini.SetValue("UserInfo", "Nickname", m_strNickname);
	ini.SetValue("UserInfo", "Files", m_dwFiles);
	ini.SetValue("UserInfo", "Line", m_wLine);

	ini.SetValue("General", "CheckUpdate", m_bUpdate);
	ini.SetValue("General", "LogDir", m_strLogDir);
	ini.SetValue("General", "Log", m_bLog);
	ini.SetValue("General", "LoadLogOnEnter", m_bLoadLog);
	ini.SetValue("General", "ConnectionRetry", m_bRetry);
	
	if((m_nRetries < 0) || (m_nRetries > 5)) m_nRetries = 3;
	ini.SetValue("General", "ConnectionRetries", m_nRetries);

	ini.SetValue("General", "ForcePrimary", m_bForcePrimary);
	ini.SetValue("General", "ForcedNode", m_strForcedNode);
	ini.SetValue("General", "ForcedExt", m_strNodeExternal);
	ini.SetValue("General", "AutoWPN", m_bAutoWPN);
	ini.SetValue("General", "AllowTrustedPMOnly", m_bAllowTrustedPMOnly);
    ini.SetValue("General", "CacheRoomList", m_bCacheRooms);
	ini.SetValue("General", "KeepHilitedRoomsTop", m_bHiliteTop);
	ini.SetValue("General", "CacheFavsOnly", m_bCacheFavs);
	ini.SetValue("General", "ShowExpertOptions", m_bShowExpert);

	ini.SetValue("Look & Feel", "SaveWinPos", m_bSavePos);

	ini.SetValue("Look & Feel", "SepOnSwitch", m_bSepOnSwitch);
	ini.SetValue("Look & Feel", "AutoList", m_bAutoList);
	ini.SetValue("Look & Feel", "AlwaysMaximizeMDI", m_bMaxi);	
	ini.SetValue("Look & Feel", "PMToolTip", m_bPMToolTip);

	ini.SetValue("Look & Feel", "Timestamp", m_bTime);

	ini.SetValue("Userlist", "UserNormal", m_strModeNormal);
	ini.SetValue("Userlist", "UserVoiced", m_strModeVoiced);
	ini.SetValue("Userlist", "UserAdmin", m_strModeAdmin);
	ini.SetValue("Userlist", "UserHost", m_strModeHost);
	ini.SetValue("Userlist", "DisplayMode", m_wUserlistMode);
	ini.SetValue("Userlist", "BgImage", m_strUserlistBg);
	ini.SetValue("Userlist", "UseImage", m_bUserUserlistBg);

	m_FA.Replace(" ", " ");  // replace space with Alt+0160 because the windows ini function will trimm spaces :rolleyes:
	m_FM.Replace(" ", " ");  
	m_EA.Replace(" ", " ");  
	m_EM.Replace(" ", " ");  
	ini.SetValue("Look & Feel", "EnActFront", m_FA);
	ini.SetValue("Look & Feel", "EnMsgFront", m_FM);
	ini.SetValue("Look & Feel", "EnActEnd", m_EA);
	ini.SetValue("Look & Feel", "EnMsgEnd", m_EM);
	ini.SetValue("Look & Feel", "Mini2Tray", m_bMiniTray);
	ini.SetValue("Look & Feel", "UseImage", m_bImage);
	ini.SetValue("Look & Feel", "BG-Image", m_strImage);
	ini.SetValue("Look & Feel", "ShowPing", m_bPing);
	ini.SetValue("Look & Feel", "TimeFormat", m_nTimeFormat);
	ini.SetValue("Look & Feel", "HiliteUsers", m_bHiliteUsers);
	ini.SetValue("Look & Feel", "Emoticons", m_bEmoticons);
	ini.SetValue("Look & Feel", "DisplayNode", m_bDisplayNode);
	ini.SetValue("Look & Feel", "AutoHideSystem", m_bHideSystem);
	ini.SetValue("Look & Feel", "ShowFillStatus", m_bShowFillStatus);
	ini.SetValue("Look & Feel", "BarTop", m_bBarTop);
	ini.SetValue("Look & Feel", "OperatorsTop", m_bOpsTop);
	ini.SetValue("History", "Depth", m_nDepth);
	ini.SetValue("History", "Enable", m_bHistory);
	ini.SetValue("Look & Feel", "BlockMCMA", m_bBlockMCMA);
	ini.SetValue("Look & Feel", "MCMAShowRawname", m_bMCMARawname);

	ini.SetValue("Warnings", "URLWarning", m_bShowURLWarning);
	ini.SetValue("Warnings", "Exit", m_bShowExitConfirm);

	ini.SetValue("Messages", "EnableScroller", m_bScroller);
	ini.SetValue("Messages", "WinampVideo", m_strVideoMsg);
	ini.SetValue("Messages", "Winamp", m_strWinampMsg);
	ini.SetValue("Messages", "OnEnterEnable", m_bOnEnter);
	ini.SetValue("Messages", "OnEnterMsg", m_strEnterMsg);
	ini.SetValue("Messages", "JoinMessage", m_strJoin);
	ini.SetValue("Messages", "PartMessage", m_strPart);
	ini.SetValue("Messages", "PM-Privacy", m_strPrivacyMsg);
	ini.SetValue("Messages", "PM-Ignored", m_strIgnoredMsg);
	ini.SetValue("Messages", "PM-Away", m_strPMAway);

	ini.SetValue("Font", "MessageBold", m_bMsBold);
	ini.SetValue("Font", "MessageItalic", m_bMsItalic);
	ini.SetValue("Font", "MessageLine", m_bMsLine);
	ini.SetValue("Font", "ChatFont",   m_strFont);
	ini.SetValue("Font", "ChatFontSize", m_nFontSize);

	SaveColorsToFile(m_strIniFile);

	switch(m_nCharset){

	case BALTIC_CHARSET:
		ini.SetValue("Font", "Charset", 1);
		break;
	case CHINESEBIG5_CHARSET:
		ini.SetValue("Font", "Charset", 2);
		break;
	case DEFAULT_CHARSET:
		ini.SetValue("Font", "Charset", 3);
		break;
	case EASTEUROPE_CHARSET:
		ini.SetValue("Font", "Charset", 4);
		break;
	case GB2312_CHARSET:
		ini.SetValue("Font", "Charset", 5);
		break;
	case GREEK_CHARSET:
		ini.SetValue("Font", "Charset", 6);
		break;
	case HANGEUL_CHARSET:
		ini.SetValue("Font", "Charset", 7);
		break;
	case MAC_CHARSET:
		ini.SetValue("Font", "Charset", 8);
		break;
	case OEM_CHARSET:
		ini.SetValue("Font", "Charset", 9);
		break;
	case RUSSIAN_CHARSET:
		ini.SetValue("Font", "Charset", 10);
		break;
	case SHIFTJIS_CHARSET:
		ini.SetValue("Font", "Charset", 11);
		break;
	case SYMBOL_CHARSET:
		ini.SetValue("Font", "Charset", 12);
		break;
	case TURKISH_CHARSET:
		ini.SetValue("Font", "Charset", 13);
		break;
	case ANSI_CHARSET:
	default:
		ini.SetValue("Font", "Charset", 0);
		break;
	}

	ini.SetValue("SoundFX", "PM", m_strSfxPM);
	ini.SetValue("SoundFX", "Join", m_strSfxJoin);
	ini.SetValue("SoundFX", "Part", m_strSfxPart);
	ini.SetValue("SoundFX", "Redirect", m_strSfxRedirect);
	ini.SetValue("SoundFX", "Topic", m_strSfxTopic);
	ini.SetValue("SoundFX", "Motd", m_strSfxMotd);
	ini.SetValue("SoundFX", "Start", m_strSfxStart);
	ini.SetValue("SoundFX", "Error", m_strSfxError);
	ini.SetValue("SoundFX", "ChatSfx", m_bChatSfx);
	ini.SetValue("SoundFX", "Sfx", m_bSoundFX);
	
	m_strUserTool.Replace("\r", "\\r");
	m_strUserTool.Replace("\n", "\\n");
	m_strToolList.Replace("\r", "\\r");
	m_strToolList.Replace("\n", "\\n");
	m_strToolRoom.Replace("\r", "\\r");
	m_strToolRoom.Replace("\n", "\\n");
	m_strToolSettings.Replace("\r", "\\r");
	m_strToolSettings.Replace("\n", "\\n");
	m_strToolListHover.Replace("\r", "\\r");
	m_strToolListHover.Replace("\n", "\\n");
	m_strToolPM.Replace("\r", "\\r");
	m_strToolPM.Replace("\n", "\\n");
	m_strToolFiles.Replace("\r", "\\r");
	m_strToolFiles.Replace("\n", "\\n");

	ini.SetValue("ChannelToolTips", "List", m_strToolList);
	ini.SetValue("ChannelToolTips", "ListHover", m_strToolListHover);
	ini.SetValue("ChannelToolTips", "Room", m_strToolRoom);
	ini.SetValue("ChannelToolTips", "Settings", m_strToolSettings);
	ini.SetValue("ChannelToolTips", "PM", m_strToolPM);
	ini.SetValue("ChannelToolTips", "Files", m_strToolFiles);
	ini.SetValue("ChannelToolTips", "Effect", m_nToolEffect);
	ini.SetValue("ChannelToolTips", "Bg1", m_crToolBg1);
	ini.SetValue("ChannelToolTips", "Bg2", m_crToolBg2);
	ini.SetValue("ChannelToolTips", "Bg3", m_crToolBg3);
	ini.SetValue("ChannelToolTips", "Gran", m_nToolGran);
	ini.SetValue("ChannelToolTips", "Trans", m_nToolTrans);
	ini.SetValue("ChannelToolTips", "UseOldStyleTools", m_bOldTools);
	ini.SetValue("ChannelToolTips", "DefChannelImg", m_strDefaultChannelImage);
	
	ini.SetValue("UserToolTips", "DefUserImg", m_strDefaultUserImage);
	ini.SetValue("UserToolTips", "UserToolHTML", m_strUserTool);
	ini.SetValue("UserToolTips", "Effect", m_nUserToolEffect);
	ini.SetValue("UserToolTips", "Bg1", m_crUserToolBg1);
	ini.SetValue("UserToolTips", "Bg2", m_crUserToolBg2);
	ini.SetValue("UserToolTips", "Bg3", m_crUserToolBg3);
	ini.SetValue("UserToolTips", "Gran", m_nUserToolGran);
	ini.SetValue("UserToolTips", "Trans", m_nUserToolTrans);
	ini.SetValue("UserToolTips", "UseOldStyleTools", m_bUserOldTools);
	ini.SetValue("UserToolTips", "TileBg", m_bTile);
	ini.SetValue("UserToolTips", "X-Offset", m_nXOffset);
	ini.SetValue("UserToolTips", "Y-Offset", m_nYOffset);

	ini.SetValue("AwayControl", "IdleDuration", m_dwIdleDuration);
	ini.SetValue("AwayControl", "AutoIfIdle", m_bAwayIfIdle);
	ini.SetValue("AwayControl", "AutoReason", m_strAutoReason);
	ini.SetValue("AwayControl", "AutoPrefix", m_strAutoPrefix);
	ini.SetValue("AwayControl", "AutoSuffix", m_strAutoSuffix);
	ini.SetValue("AwayControl", "AutoBack", m_bAutoBack);

	ini.SetValue("Node", "IP", m_dwNodeIP);
	ini.SetValue("Node", "Port", m_dwNodePort);
}

void CSettings::LoadColorsFromFile(const CString strIniFile)
{

	CIni ini;
	ini.SetIniFileName(strIniFile);

	m_crFocus		= ini.GetValue("Colors", "FocusColor", RGB(74,74,74));
	m_bFocus		= ini.GetValue("Colors", "UseFocusColor", 1);
	m_crActionBr	= ini.GetValue("Colors", "ActionNameEnc", RGB(255,0,255));
	m_crAction		= ini.GetValue("Colors", "ActionText", RGB(255,0,255));
	m_crNickBr		= ini.GetValue("Colors", "MessageNameEnc", RGB(255,255,0));
	m_crMessage		= ini.GetValue("Colors", "MessageText", RGB(255,255,255));
	m_crNick		= ini.GetValue("Colors", "NormalNick", RGB(255,255,0));
	m_crJoin		= ini.GetValue("Colors", "Join", RGB(128,255,0));
	m_crPart		= ini.GetValue("Colors", "Part", RGB(255,128,128));
	m_crTopic		= ini.GetValue("Colors", "Topic", RGB(0,0,255));
	m_crMotd		= ini.GetValue("Colors", "Motd", RGB(128,255,250));
	m_crFiles		= ini.GetValue("Colors", "Files", RGB(250,0,0));
	m_crLine		= ini.GetValue("Colors", "Line", RGB(255,255,0));
	m_crIP			= ini.GetValue("Colors", "IP", RGB(255,245,210));
	m_crPort		= ini.GetValue("Colors", "Port", RGB(255,245,210));
	m_crPend		= ini.GetValue("Colors", "Pend", RGB(254,128,64));
	m_crOK			= ini.GetValue("Colors", "OK", RGB(0,225,0));
	m_crErr			= ini.GetValue("Colors", "Err", RGB(245,0,0));
	m_crTime		= ini.GetValue("Colors", "Time", RGB(0,0,0));
	m_crHiLite		= ini.GetValue("Colors", "HiLite", RGB(255,128, 128));
	m_crHiLiteTxt	= ini.GetValue("Colors", "HiLiteTxt", RGB(255, 255, 255));
	m_crGrid		= ini.GetValue("Colors", "Grid", RGB(0,0,0));
	m_crGridTxt		= ini.GetValue("Colors", "GridTxt", RGB(255,255,255));
	m_crDocHiLite	= ini.GetValue("Colors", "DocHiLite", RGB(0,0,255));
	m_crBg			= ini.GetValue("Colors", "Background", RGB(0,0,0));
	m_crOp			= ini.GetValue("Colors", "OpMsgs", RGB(255, 0, 0));
	m_crLink		= ini.GetValue("Colors", "Links", RGB(0, 0, 255));
}

void CSettings::SaveColorsToFile(const CString strIniFile)
{

	CIni ini;
	ini.SetIniFileName(strIniFile);

	ini.SetValue("Colors", "ColorActionName", m_bColorAcName);
	ini.SetValue("Colors", "NormalNick", m_crNick);
	ini.SetValue("Colors", "Time", m_crTime);
	
	ini.SetValue("Colors", "ActionNameEnc", m_crActionBr);
	ini.SetValue("Colors", "ActionText", m_crAction);

	ini.SetValue("Colors", "MessageNameEnc", m_crNickBr);
	ini.SetValue("Colors", "MessageText", m_crMessage);

	ini.SetValue("Colors", "Join", m_crJoin);
	ini.SetValue("Colors", "Part", m_crPart);
	ini.SetValue("Colors", "Topic", m_crTopic);
	ini.SetValue("Colors", "Motd", m_crMotd);

	ini.SetValue("Colors", "Files", m_crFiles);
	ini.SetValue("Colors", "Line", m_crLine);
	ini.SetValue("Colors", "IP", m_crIP);
	ini.SetValue("Colors", "Port", m_crPort);
	ini.SetValue("Colors", "Pend", m_crPend);
	ini.SetValue("Colors", "OK", m_crOK);
	ini.SetValue("Colors", "Err", m_crErr);
	ini.SetValue("Colors", "FocusColor", m_crFocus);
	ini.SetValue("Colors", "UseFocusColor", m_bFocus);
	ini.SetValue("Colors", "HiLite", m_crHiLite);
	ini.SetValue("Colors", "HiLiteTxt", m_crHiLiteTxt);
	ini.SetValue("Colors", "Grid", m_crGrid);
	ini.SetValue("Colors", "GridTxt", m_crGridTxt);
	ini.SetValue("Colors", "DocHiLite", m_crDocHiLite);
	ini.SetValue("Colors", "Background", m_crBg);
	ini.SetValue("Colors", "OpMsgs", m_crOp);
	ini.SetValue("Colors", "Links", m_crLink);
}
void CSettings::SetCharSet(int nValue)
{

	switch(nValue){

	case 1:
		m_nCharset = BALTIC_CHARSET;
		break;
	case 2:
		m_nCharset = CHINESEBIG5_CHARSET;
		break;
	case 3:
		m_nCharset = DEFAULT_CHARSET;
		break;
	case 4:
		m_nCharset = EASTEUROPE_CHARSET;
		break;
	case 5:
		m_nCharset = GB2312_CHARSET;
		break;
	case 6:
		m_nCharset = GREEK_CHARSET;
		break;
	case 7:
		m_nCharset = HANGEUL_CHARSET;
		break;
	case 8:
		m_nCharset = MAC_CHARSET;
		break;
	case 9:
		m_nCharset = OEM_CHARSET;
		break;
	case 10:
		m_nCharset = RUSSIAN_CHARSET;
		break;
	case 11:
		m_nCharset = SHIFTJIS_CHARSET;
		break;
	case 12:
		m_nCharset = SYMBOL_CHARSET;
		break;
	case 13:
		m_nCharset = TURKISH_CHARSET;
		break;
	case 0:
	default:
		m_nCharset = ANSI_CHARSET;
		break;
	}
}

int CSettings::GetCharSet(BOOL bIndexOnly)
{

	int nReturn = 0;
	if(!bIndexOnly){

		nReturn = m_nCharset;
	}
	else{

		switch(m_nCharset){

		case BALTIC_CHARSET:
			nReturn = 1;
			break;
		case CHINESEBIG5_CHARSET:
			nReturn = 2;
			break;
		case DEFAULT_CHARSET:
			nReturn = 3;
			break;
		case EASTEUROPE_CHARSET:
			nReturn = 4;
			break;
		case GB2312_CHARSET:
			nReturn = 5;
			break;
		case GREEK_CHARSET:
			nReturn = 6;
			break;
		case HANGEUL_CHARSET:
			nReturn = 7;
			break;
		case MAC_CHARSET:
			nReturn = 8;
			break;
		case OEM_CHARSET:
			nReturn = 9;
			break;
		case RUSSIAN_CHARSET:
			nReturn = 10;
			break;
		case SHIFTJIS_CHARSET:
			nReturn = 11;
			break;
		case SYMBOL_CHARSET:
			nReturn = 12;
			break;
		case TURKISH_CHARSET:
			nReturn = 13;
			break;
		case ANSI_CHARSET:
		default:
			nReturn = 0;
			break;
		}
	}
	return nReturn;
}


CString CSettings::GetWorkingDir(BOOL bCached)
{


	if(bCached){

		return m_strWd;
	}

	TCHAR  pszAppPath[ MAX_PATH + 1 ];
	TCHAR* pszSearch = NULL;

	memset( pszAppPath, 0, sizeof( pszAppPath ) );

	GetModuleFileName( NULL, pszAppPath, sizeof( pszAppPath ) );

	// find last \ character
	pszSearch = _tcsrchr( pszAppPath, _T( '\\' ) );

	if ( pszSearch )
	{
		// strip everything after the last \ char, \ char including 
		pszSearch[ 0 ] = '\0';
	}

	return pszAppPath;
}

void CSettings::LoadHiLite(void)
{

	CString strIniFile = GetWorkingDir() + "\\hilite.ini";
	BOOL bReturn = TRUE;
	CStdioFile ini;
	CString strBuffer;
	m_aHilite.RemoveAll();

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::typeText|CFile::shareExclusive);

		while(ini.ReadString(strBuffer)){

			if(!strBuffer.IsEmpty()){

				if(strBuffer[0] != ';'){

					m_aHilite.Add(strBuffer);
				}
			}
		}
		ini.Close();
		
	}
	CATCH(CFileException, e){

		AfxMessageBox(IDS_ERROR_FILE_GENERIC, MB_OK+MB_ICONSTOP);
	}END_CATCH;
}

void CSettings::LoadRCMS()
{

	CString strIniFile = GetWorkingDir() + "\\commands.ini";
	CStdioFile ini;
	CString strBuffer;
	m_aRCMSCommands.RemoveAll();
	m_aWinMXCommands.RemoveAll();
	m_aRoseCommands.RemoveAll();
	m_aMXToolsCommands.RemoveAll();
	m_aFXServCommands.RemoveAll();

	WORD wMode = -1; // 0 = RCMS, 1 = WinMX, 2 = RoboMX, 3 = MXTools

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::typeText|CFile::shareExclusive);

		while(ini.ReadString(strBuffer)){

			if(strBuffer.IsEmpty()) continue;

			if(strBuffer.Find("[RCMS]", 0) == 0){

				wMode = 0;
				continue;
			}
			if(strBuffer.Find("[WINMX]", 0) == 0){

				wMode = 1;
				continue;
			}
			if(strBuffer.Find("[MXTOOLS]", 0) == 0){

				wMode = 2;
				continue;
			}
			if(strBuffer.Find("[ROSE]", 0) == 0){

				wMode = 3;
				continue;
			}
			if(strBuffer.Find("[FXSERV]", 0) == 0){

				wMode = 4;
				continue;
			}
			if(strBuffer.Find("[ROBOMX]", 0) == 0){

				wMode = 5;
				continue;
			}

			switch(wMode){

				case 0:
					m_aRCMSCommands.Add(strBuffer);
					break;
				case 1:
					m_aWinMXCommands.Add(strBuffer);
					break;
				case 2:
					m_aMXToolsCommands.Add(strBuffer);
					break;
				case 3:
					m_aRoseCommands.Add(strBuffer);
					break;
				case 4:
					m_aFXServCommands.Add(strBuffer);
					break;
				default:
					// ignore;
					break;
			}
		}
		ini.Close();
		
	}
	CATCH(CFileException, e){

		AfxMessageBox(IDS_ERROR_FILE_GENERIC, MB_OK+MB_ICONSTOP);
	}END_CATCH;


	///////////////////////
	// Load quick commands
	strIniFile = GetWorkingDir() + "\\quick.ini";
	m_aQuick.RemoveAll();

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead|CFile::typeText|CFile::shareExclusive);

		while(ini.ReadString(strBuffer)){

			if(!strBuffer.IsEmpty()){

				m_aQuick.Add(strBuffer);
			}
		}
		ini.Close();
		
	}
	CATCH(CFileException, e){

		AfxMessageBox(IDS_ERROR_FILE_GENERIC, MB_OK+MB_ICONSTOP);
	}END_CATCH;
}


void CSettings::LoadUsers(void)
{

	CIni ini;
	ini.SetIniFileName(m_strIniFile);

	m_aIgnored.RemoveAll();
	m_aTrusted.RemoveAll();

	CString strTmp;
	int nCnt = ini.GetValue("IgnoredUsers", "Count", 0);

	for(int i = 0; i < nCnt; i++){

		strTmp.Format("Ignored_%03d", i);
		m_aIgnored.Add(ini.GetValue("IgnoredUsers", strTmp, ""));
	}

	nCnt = ini.GetValue("TrustedUsers", "Count", 0);

	for(int i = 0; i < nCnt; i++){

		strTmp.Format("Trusted_%03d", i);
		m_aTrusted.Add(ini.GetValue("TrustedUsers", strTmp, ""));
	}
}

void CSettings::SaveUsers(void)
{

	CIni ini;
	ini.SetIniFileName(GetWorkingDir() + "\\RoboMX.ini");

	CString strTmp;
	
	ini.SetValue("IgnoredUsers", "Count", (int)m_aIgnored.GetSize());
	for(int i = 0; i < m_aIgnored.GetSize(); i++){

		strTmp.Format("Ignored_%03d", i);
		ini.SetValue("IgnoredUsers", strTmp, m_aIgnored[i]);
	}

	ini.SetValue("TrustedUsers", "Count", (int)m_aTrusted.GetSize());
	for(int i = 0; i < m_aTrusted.GetSize(); i++){

		strTmp.Format("Trusted_%03d", i);
		ini.SetValue("TrustedUsers", strTmp, m_aTrusted[i]);
	}
}

void CSettings::LoadSounds(void)
{

	CIni ini;
	ini.SetIniFileName(m_strIniFile);

	m_aSounds.RemoveAll();

	CString strTmp;
	int nCnt = ini.GetValue("ChatSounds", "Count", 0);

	for(int i = 0; i < nCnt; i++){

		SOUND s;
		strTmp.Format("Trigger_%d", i);
		s.strTrigger = ini.GetValue("ChatSounds", strTmp, "");
		if(s.strTrigger.IsEmpty()) continue;
		
		strTmp.Format("Sound_%d", i);
		s.strSound = ini.GetValue("ChatSounds", strTmp, "");
		if(s.strSound.IsEmpty()) continue;

		if(s.strSound.GetLength() > 3){

			if(s.strSound[1] != ':') s.strSound.Insert(0, GetWorkingDir() + "\\");
		}
		else{

			continue;
		}

		m_aSounds.Add(s);
	}
}

CString CSettings::GetUserModeBmp(int nMode)
{

	CString strImg;
	switch(nMode){

		case 0: //normal
			strImg = m_strModeNormal;
			break;
		case 1: //admin
			strImg = m_strModeAdmin;
			break;
		case 2: //voice
			strImg = m_strModeVoiced;
			break;
		case 3: //host
			strImg = m_strModeHost;
			break;
	}
	return strImg;
}
