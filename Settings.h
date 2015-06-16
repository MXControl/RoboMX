/*
 * BendMX - Chat extension library for WinMX.
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

#if !defined(AFX_SETTINGS_H__25ADA47A_C2DF_42B2_B3AE_6C12BA658D11__INCLUDED_)
#define AFX_SETTINGS_H__25ADA47A_C2DF_42B2_B3AE_6C12BA658D11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct TAG_MXSOUND{

	CString strTrigger;
	CString strSound;
} SOUND, *PSOUND;


class CSettings  
{
public:
	int GetCharSet(BOOL bIndexOnly = FALSE);
	void SetCharSet(int nValue);
	void Save();
	void Load();
	void LoadColorsFromFile(const CString strIniFile);
	void SaveColorsToFile(const CString strIniFile);
	CSettings();
	virtual ~CSettings();
	void    SetIniFile(CString strFile){ m_strIniFile = strFile; }

	// Set Functions
	void    SetShowExpert(BOOL bValue){ m_bShowExpert = bValue; }
	void	SetPrivacyMsg(CString strValue){ m_strPrivacyMsg = strValue; }
	void	SetIgnoredMsg(CString strValue){ m_strIgnoredMsg = strValue; }
	void    SetPMAwayMsg(CString strValue){ m_strPMAway = strValue; }
	void	SetNodeExternal(CString strValue){ m_strNodeExternal = strValue; }
	void	SetForcePrimary(BOOL bValue){ m_bForcePrimary = bValue; }
	void	SetForcedNode(CString strValue){ m_strForcedNode = strValue; }
	void	SetAutoWPN(BOOL bValue){ m_bAutoWPN = bValue; }
	void    SetSavePos(BOOL bValue){ m_bSavePos = bValue; }
	void    SetLogDir(CString strValue){ m_strLogDir = strValue; }
	void	SetLog(BOOL bValue){m_bLog = bValue; }
	void	SetLoadLog(BOOL bValue){m_bLoadLog = bValue; }
	void	SetSepOnSwith(BOOL bValue){m_bSepOnSwitch = bValue; }
	void    SetEnableScroller(BOOL bValue){ m_bScroller = bValue; }
	void	SetMessageItalic(BOOL bValue){ m_bMsItalic = bValue; }
	void	SetMessageLine(BOOL bValue){ m_bMsLine = bValue; }
	void	SetMessageBold(BOOL bValue){ m_bMsBold = bValue; }
	void	SetPrintTime(BOOL bValue){ m_bTime = bValue; }
	void    SetHistoryDepth(int nValue){ m_nDepth = nValue; }
	void    SetSaveHistory(int nValue){ m_bHistory = nValue; }
	void    SetJoin(CString strValue){ m_strJoin = strValue; }
	void    SetPart(CString strValue){ m_strPart = strValue; }
	void	SetEmoticons(BOOL bValue){ m_bEmoticons = bValue; }
	void	SetHiliteUsers(BOOL bValue){ m_bHiliteUsers = bValue; }
	void    SetHideSystem(BOOL bValue){ m_bHideSystem = bValue; }
	void    SetShowURLWarning(BOOL bValue){ m_bShowURLWarning = bValue; }
	void	SetPMToolTip(BOOL bValue){ m_bPMToolTip = bValue; }
	void	SetPMOnlyTrusted(BOOL bValue){ m_bAllowTrustedPMOnly = bValue; }
	void	SetShowExitConfirm(BOOL bValue){ m_bShowExitConfirm = bValue; }
	void	SetOpsTop(BOOL bValue) { m_bOpsTop = bValue; }
	void	SetCacheRooms(BOOL bValue) { m_bCacheRooms = bValue; }
	void	SetCacheFavs(BOOL bValue) { m_bCacheFavs = bValue; }
	void	SetHiliteTop(BOOL bValue) { m_bHiliteTop = bValue; }
	void	SetDefaulStyle(DWORD dwValue){ m_dwTextStyle = dwValue; }
	void	SetRGBLink(COLORREF crValue){ m_crLink = crValue; }
	void	SetRGBNormalName(COLORREF crValue){ m_crNick = crValue; }
	void	SetRGBNormalMsg(COLORREF crValue){ m_crMessage = crValue; }
	void	SetRGBActionMsg(COLORREF crValue){ m_crAction = crValue; }
	void	SetRGBJoin(COLORREF crValue){ m_crJoin = crValue; }
	void	SetRGBPart(COLORREF crValue){ m_crPart = crValue; }
	void	SetRGBTopic(COLORREF crValue){ m_crTopic = crValue; }
	void	SetRGBMotd(COLORREF crValue){ m_crMotd = crValue; }
	void	SetRGBBrMessage(COLORREF crValue){ m_crNickBr = crValue; }
	void	SetRGBBrAction(COLORREF crValue){ m_crActionBr = crValue; }
	void	SetRGBBg(COLORREF crValue){ m_crBg = crValue; }
	void	SetRGBFiles(COLORREF crValue){  m_crFiles = crValue; }
	void	SetRGBLine(COLORREF crValue){  m_crLine = crValue; }
	void	SetRGBIP(COLORREF crValue){  m_crIP = crValue; }
	void	SetRGBPort(COLORREF crValue){  m_crPort = crValue; }
	void	SetRGBPend(COLORREF crValue){  m_crPend = crValue; }
	void	SetRGBOK(COLORREF crValue){  m_crOK = crValue; }
	void	SetRGBErr(COLORREF crValue){  m_crErr = crValue; }
	void	SetRGBFocus(COLORREF crValue){ m_crFocus = crValue; }
	void    SetRGBTime(COLORREF crValue){ m_crTime = crValue; }
	void    SetRGBHiLite(COLORREF crValue){ m_crHiLite = crValue; }
	void    SetRGBHiLiteTxt(COLORREF crValue){ m_crHiLiteTxt = crValue; }
	void    SetRGBGrid(COLORREF crValue){ m_crGrid = crValue; }
	void    SetRGBGridTxt(COLORREF crValue){ m_crGridTxt = crValue; }
	void	SetRGBDocHiLite(COLORREF crValue){ m_crDocHiLite = crValue; }
	void	SetRGBOp(COLORREF crValue){m_crOp = crValue; }
	void	SetShowFillStatus(BOOL bValue){ m_bShowFillStatus = bValue; }
	
	void    SetUseImage(BOOL bValue){ m_bImage = bValue; }
	void	SetImage(CString strValue){ m_strImage = strValue; }
	void    SetUserlistDefaultImage(CString strValue){ m_strDefaultUserImage = strValue; }
	void	SetChannelDefaultImage(CString strValue){ m_strDefaultChannelImage = strValue; }

	void	SetWorkingDir(CString strValue){ m_strWd = strValue; }
	void	SetBrActionFront(CString strValue){ m_FA = strValue; }
	void	SetBrMsgFront(CString strValue){ m_FM = strValue; }
	void	SetBrActionEnd(CString strValue){ m_EA = strValue; }
	void	SetBrMsgEnd(CString strValue){ m_EM = strValue; }
	void	SetFont(CString strValue){ m_strFont = strValue; }
	void    SetFontSize(int nValue){ m_nFontSize = nValue; }

	void	SetFiles(DWORD dwValue){m_dwFiles = dwValue; }
	void	SetLine(WORD wValue){ m_wLine = wValue; }
	void    SetNickname(CString strValue){m_strNickname = strValue; }
	void	SetWinampMsg(CString strValue){ m_strWinampMsg = strValue; }
	void	SetVideoMsg(CString strValue){ m_strVideoMsg = strValue; }
	void	SetEnterMsg(CString strValue){ m_strEnterMsg = strValue; }
	void	SetDoEnterMsg(BOOL bValue){ m_bOnEnter = bValue; }
	void	SetColorAcName(BOOL bValue){ m_bColorAcName = bValue; }
	void    SetMiniTray(BOOL bValue){ m_bMiniTray = bValue; }
	void	SetPing(BOOL bValue){ m_bPing = bValue; }
	void	SetTimeFmt(int nValue){ m_nTimeFormat = nValue; }
	void	SetFocus(BOOL bValue){ m_bFocus = bValue; }
	void	SetUpdate(BOOL bValue){ m_bUpdate = bValue; }
	void	SetAutoList(BOOL bValue){ m_bAutoList = bValue; }
	void	SetMaxi(BOOL bValue){ m_bMaxi = bValue; }
	void	SetBarTop(BOOL bValue){ m_bBarTop = bValue; }
	void	SetRetry(BOOL bValue){ m_bRetry = bValue; }
	void	SetRetries(int nValue){ m_nRetries = nValue; }
	void	SetBlockMCMA(BOOL bValue){ m_bBlockMCMA = bValue; }
	void	SetMCMARawname(BOOL bValue){ m_bMCMARawname = bValue; }

	// Serve shit
	void	SetNodeIP(DWORD dwValue){  m_dwNodeIP = dwValue; }
	void	SetNodePort(DWORD dwValue){  m_dwNodePort = dwValue; }

	// Sound stuff
	void	SetSfxPM(CString strValue){ m_strSfxPM = strValue; }
	void    SetSfxJoin(CString strValue){ m_strSfxJoin = strValue; }
	void	SetSfxPart(CString strValue){ m_strSfxPart = strValue; }
	void	SetSfxRedirect(CString strValue){ m_strSfxRedirect = strValue; }
	void	SetSfxTopic(CString strValue){ m_strSfxTopic = strValue; }
	void	SetSfxMotd(CString strValue){ m_strSfxMotd = strValue; }
	void	SetSfxStart(CString strValue){ m_strSfxStart = strValue; }
	void	SetSfxError(CString strValue){ m_strSfxError = strValue; }
	void	SetSfxChatSfx(BOOL bValue){ m_bChatSfx = bValue; }
	void	SetSoundFX(BOOL bValue){ m_bSoundFX = bValue; }
	
	void    SetToolList(CString strValue){ m_strToolList = strValue; }
	void	SetToolListHover(CString strValue){ m_strToolListHover = strValue; }
	void    SetToolRoom(CString strValue){ m_strToolRoom = strValue; }
	void    SetToolSettings(CString strValue){ m_strToolSettings = strValue; }
	void    SetToolPM(CString strValue){ m_strToolPM = strValue; }
	void    SetToolFiles(CString strValue){ m_strToolFiles = strValue; }
	void    SetToolEffect(int nValue){ m_nToolEffect = nValue; }
	void    SetToolBg1(COLORREF crValue){ m_crToolBg1 = crValue; }
	void	SetToolBg2(COLORREF crValue){ m_crToolBg2 = crValue; }
	void    SetToolBg3(COLORREF crValue){ m_crToolBg3 = crValue; }
	void    SetToolGran(int nValue){ m_nToolGran = nValue; }
	void    SetToolTrans(int nValue){  m_nToolTrans = nValue; }
	void    SetUseOldTools(BOOL bValue){ m_bOldTools = bValue; }
	void    SetUserTool(CString strValue){ 	m_strUserTool = strValue; }
	void    SetUserToolEffect(int nValue){ m_nUserToolEffect = nValue; }
	void    SetUserToolBg1(COLORREF crValue){ m_crUserToolBg1 = crValue; }
	void	SetUserToolBg2(COLORREF crValue){ m_crUserToolBg2 = crValue; }
	void    SetUserToolBg3(COLORREF crValue){ m_crUserToolBg3 = crValue; }
	void    SetUserToolGran(int nValue){ m_nUserToolGran = nValue; }
	void    SetUserToolTrans(int nValue){  m_nUserToolTrans = nValue; }
	void    SetUserUseOldTools(BOOL bValue){ m_bUserOldTools = bValue; }

	void    SetUserModeNormal(CString strValue){ m_strModeNormal = strValue; }
	void    SetUserModeVoiced(CString strValue){ m_strModeVoiced = strValue; }
	void    SetUserModeAdmin(CString strValue){ m_strModeAdmin = strValue; }
	void    SetUserModeHost(CString strValue){ m_strModeHost = strValue; }
	void    SetUserlistMode(WORD wValue){ m_wUserlistMode = wValue; }
	void    SetUserlistBg(CString strValue){ m_strUserlistBg = strValue; }
	void    SetUseUserlistBg(BOOL bValue){ m_bUserUserlistBg = bValue; }
	void    SetUserlistTileBg(BOOL bValue){ m_bTile = bValue; }
	void    SetUserlistXOffsest(int nValue){ m_nXOffset = nValue; }
	void    SetUSerlistYOffset(int nValue){ m_nYOffset = nValue; }
	void    SetIdleDuration(DWORD dwValue){ m_dwIdleDuration = dwValue; }
	void    SetAwayIfIdle(BOOL bValue){ m_bAwayIfIdle = bValue; }
	void    SetAutoReason(CString strValue){ m_strAutoReason = strValue; }
	void    SetAutoPrefix(CString strValue){ m_strAutoPrefix = strValue; }
	void    SetAutoSuffix(CString strValue){ m_strAutoSuffix = strValue; }
	void    SetAutoBack(BOOL bValue){ m_bAutoBack = bValue; }

	// Access functions
	BOOL	GetShowExpert(){ return m_bShowExpert; }
	CString GetPrivacyMsg(){ return m_strPrivacyMsg; }
	CString GetIgnoredMsg(){ return m_strIgnoredMsg; }
	CString GetPMAwayMsg(){ return m_strPMAway; }
	CString GetNodeExternal(){ return m_strNodeExternal; }
	BOOL    GetForcePrimary(){ return m_bForcePrimary; }
	CString GetForcedNode(){ return m_strForcedNode; }
    BOOL	GetAutoWPN(){ return m_bAutoWPN; }
	BOOL	GetSavePos(){ return m_bSavePos; }
	CString GetWorkingDir(BOOL bCached = TRUE);
	CString GetIniFile(){ return m_strIniFile; }
	CString GetLogDir(){return m_strLogDir;}
	BOOL    GetLog(){return m_bLog;}
	BOOL    GetSepOnSwitch(){ return m_bSepOnSwitch; }
	BOOL    GetLoadLog(){ return m_bLoadLog; }
	BOOL    GetUseImage(){ return m_bImage; }
	CString GetImage(){ return m_strImage; }
	BOOL	GetEmoticons(){ return m_bEmoticons; }
	BOOL	GetHiliteUsers(){ return m_bHiliteUsers; }
	DWORD	GetFiles(){ return m_dwFiles; }
	WORD	GetLine(){ return m_wLine; }
	BOOL	GetDisplayNode(){ return m_bDisplayNode; }
	BOOL	GetHideSystem(){ return m_bHideSystem; }
	BOOL	GetBarTop(){ return m_bBarTop; }
 	BOOL	GetRetry(){ return m_bRetry; }
	int 	GetRetries(){ return m_nRetries; }
	BOOL    GetShowURLWarning(){ return m_bShowURLWarning; }
	BOOL	GetBlockMCMA(){ return m_bBlockMCMA; }
	BOOL	GetMCMARawname(){ return m_bMCMARawname; }
	BOOL	GetPMToolTip(){ return m_bPMToolTip; }
	BOOL	GetPMOnlyTrusted(){ return m_bAllowTrustedPMOnly; }
 	BOOL	GetShowExitConfirm(){ return m_bShowExitConfirm; }
	BOOL	GetOpsTop(){ return m_bOpsTop; }
	BOOL	GetCacheRooms(){ return m_bCacheRooms; }
	BOOL    GetCacheFavs(){ return m_bCacheFavs; }
	BOOL	GetHiliteTop(){ return m_bHiliteTop; }
	BOOL	GetShowFillStatus(){ return m_bShowFillStatus; }

	BOOL	GetMessageItalic(){ return m_bMsItalic; }
	BOOL	GetMessageLine(){ return m_bMsLine; }
	BOOL	GetMessageBold(){ return m_bMsBold; }
	BOOL	GetPrintTime(){ return m_bTime; }
	int     GetHistoryDepth(){ return m_nDepth; }
	int     GetSaveHistory(){ return m_bHistory; }
	CString GetNickname(){ return m_strNickname; }
	BOOL	GetFocus(){ return m_bFocus; }

	CString GetJoin(BOOL bReturn = FALSE){ 
	
		CString strTmp = m_strJoin;
		if(bReturn) strTmp.Replace("\\n", "\r\n");
		return strTmp; 
	}
	CString GetPart(BOOL bReturn = FALSE){ 
		
		CString strTmp = m_strPart;
		if(bReturn) strTmp.Replace("\\n", "\r\n");
		return strTmp; 
	}

	DWORD		GetDefaultStyle(){return m_dwTextStyle; }
	COLORREF	GetRGBLink(){ return m_crLink; }
	COLORREF	GetRGBTime(){ return m_crTime; }
	COLORREF	GetRGBNormalName(){ return m_crNick; }
	COLORREF	GetRGBNormalMsg(){ return m_crMessage; }
	COLORREF	GetRGBActionMsg(){ return m_crAction; }
	COLORREF	GetRGBJoin(){ return m_crJoin; }
	COLORREF	GetRGBPart(){ return m_crPart; }
	COLORREF	GetRGBTopic(){ return m_crTopic; }
	COLORREF	GetRGBMotd(){ return m_crMotd; }
	COLORREF	GetRGBBrMessage(){ return m_crNickBr; }
	COLORREF	GetRGBBrAction(){ return m_crActionBr; }
	COLORREF	GetRGBBg(){ return m_crBg; }
	COLORREF	GetRGBFiles(){ return m_crFiles; }
	COLORREF	GetRGBLine(){ return m_crLine; }
	COLORREF	GetRGBIP(){ return m_crIP; }
	COLORREF	GetRGBPort(){ return m_crPort; }
	COLORREF	GetRGBPend(){ return m_crPend; }
	COLORREF	GetRGBOK(){ return m_crOK; }
	COLORREF	GetRGBErr(){ return m_crErr; }
	COLORREF	GetRGBFocus(){ return m_crFocus; }
	COLORREF	GetRGBHiLite(){ return m_crHiLite; }
	COLORREF    GetRGBHiLiteTxt(){ return m_crHiLiteTxt; }
	COLORREF    GetRGBGrid(){ return m_crGrid; }
	COLORREF    GetRGBGridTxt(){ return m_crGridTxt; }
	COLORREF	GetRGBDocHiLite() { return m_crDocHiLite; }
	COLORREF	GetRGBOp(){return m_crOp; }

	CString		GetBrActionFront(){ return m_FA; }
	CString		GetBrMsgFront(){ return m_FM; }
	CString		GetBrActionEnd(){ return m_EA; }
	CString		GetBrMsgEnd(){ return m_EM; }
	CString		GetFont(){ return m_strFont; }
	int         GetFontSize(){ return m_nFontSize; }
	
	BOOL        GetEnableScroller(){ return m_bScroller; }
	CString		GetWinampMsg(){ return m_strWinampMsg; }
	CString		GetVideoMsg(){ return m_strVideoMsg; }
	CString		GetEnterMsg(){ return m_strEnterMsg; }
	BOOL		GetDoEnterMsg(){ return m_bOnEnter; }
	BOOL		GetColorAcName(){ return m_bColorAcName; }
	BOOL		GetMiniTray(){ return m_bMiniTray; }
	BOOL		GetPing(){ return m_bPing; }
	int			GetTimeFmt(){ return m_nTimeFormat; }
	BOOL		GetUpdate(){ return m_bUpdate; }
	BOOL		GetAutoList(){ return m_bAutoList; }
	BOOL		GetMaxi(){ return m_bMaxi; }
	// Sound stuff
	CString		GetSfxPM(){ return m_strSfxPM; }
	CString     GetSfxJoin(){ return m_strSfxJoin; }
	CString	    GetSfxPart(){ return m_strSfxPart; }
	CString		GetSfxRedirect(){ return m_strSfxRedirect; }
	CString		GetSfxTopic(){ return m_strSfxTopic; }
	CString		GetSfxMotd(){ return m_strSfxMotd; }
	CString		GetSfxStart(){ return m_strSfxStart; }
	CString		GetSfxError(){ return m_strSfxError; }
	volatile BOOL	GetSfxChatSfx(){ return m_bChatSfx; }
	BOOL		GetSoundFX(){ return m_bSoundFX; }

	DWORD      GetNodeIP(){ return m_dwNodeIP; }
	DWORD	   GetNodePort(){ return m_dwNodePort; }

	CString    GetToolList(){ return m_strToolList; }
	CString	   GetToolListHover(){ return m_strToolListHover; }
	CString	   GetToolRoom(){ return m_strToolRoom; }
	CString    GetToolSettings(){ return m_strToolSettings; }
	CString    GetToolPM(){ return m_strToolPM; }
	CString    GetToolFiles(){ return m_strToolFiles; }
	int        GetToolEffect(){  return m_nToolEffect; }
	COLORREF   GetToolBg1(){ return m_crToolBg1; }
	COLORREF   GetToolBg2(){ return m_crToolBg2; }
	COLORREF   GetToolBg3(){ return m_crToolBg3; }
	int        GetToolGran(){ return m_nToolGran; }
	int        GetToolTrans(){ return m_nToolTrans; }
	BOOL	   GetUseOldTools(){ return m_bOldTools; }

	CString    GetUserTool(){ return m_strUserTool; }
	int        GetUserToolEffect(){  return m_nUserToolEffect; }
	COLORREF   GetUserToolBg1(){ return m_crUserToolBg1; }
	COLORREF   GetUserToolBg2(){ return m_crUserToolBg2; }
	COLORREF   GetUserToolBg3(){ return m_crUserToolBg3; }
	int        GetUserToolGran(){ return m_nUserToolGran; }
	int        GetUserToolTrans(){ return m_nUserToolTrans; }
	BOOL	   GetUserUseOldTools(){ return m_bUserOldTools; }
	
	CString	   GetUserModeNormal(){ return m_strModeNormal; }
	CString	   GetUserModeVoiced(){ return m_strModeVoiced; }
	CString	   GetUserModeAdmin(){ return m_strModeAdmin; }
	CString	   GetUserModeHost(){ return m_strModeHost; }
	WORD	   GetUserlistMode(){ return m_wUserlistMode; }
	CString	   GetUserlistBg(){ return m_strUserlistBg; }
	BOOL	   GetUseUserlistBg(){ return m_bUserUserlistBg; }
	BOOL	   GetUserlistTileBg(){ return m_bTile; }
	int		   GetUserlistXOffsest(){ return m_nXOffset; }
	int		   GetUSerlistYOffset(){ return m_nYOffset; }
	CString	   GetUserlistDefaultImage(){ return m_strDefaultUserImage; }
	CString	   GetChannelDefaultImage(){ return m_strDefaultChannelImage; }
	
	DWORD      GetIdleDuration(){ return m_dwIdleDuration; }
	BOOL	   GetAwayIfIdle(){ return m_bAwayIfIdle; }
	CString    GetAutoReason(){ return m_strAutoReason; }
	CString    GetAutoPrefix(){ return m_strAutoPrefix; }
	CString    GetAutoSuffix(){ return m_strAutoSuffix; }
	BOOL	   GetAutoBack(){ return m_bAutoBack; }

// Loading stuff
	void LoadSounds(void);
	void LoadRCMS(void);

public: // public attributes
	CStringArray m_aHilite;
	CStringArray m_aIgnored;
	CStringArray m_aTrusted;
	CStringArray m_aQuick;
	CStringArray m_aRCMSCommands;
	CStringArray m_aWinMXCommands;
	CStringArray m_aMXToolsCommands;
	CStringArray m_aRoseCommands;
	CStringArray m_aFXServCommands;
	CArray<SOUND, SOUND> m_aSounds;


protected:
	CString m_strWd;
	CString m_strLogDir;
	CString m_strIniFile;
	
	BOOL   m_bShowExpert;
   
	BOOL    m_bSepOnSwitch;
	BOOL    m_bLoadLog;
	BOOL    m_bLog;
	BOOL	m_bMsItalic;
	BOOL	m_bMsLine;
	BOOL	m_bMsBold;
	BOOL	m_bTime;
	BOOL	m_bOnEnter;
	BOOL    m_bColorAcName;
	BOOL    m_bPing;
	int     m_nFontSize;
	int		m_nCharset;
	int     m_nDepth;//   = NULL;
	int     m_bHistory;// = TRUE;
	int     m_nTimeFormat;
	BOOL	m_bAllowTrustedPMOnly;
	BOOL	m_bShowFillStatus;

	BOOL    m_bForcePrimary;
	CString m_strForcedNode;
	CString m_strNodeExternal;
	CString m_strPrivacyMsg;
	CString m_strIgnoredMsg;
	CString m_strPMAway;

	BOOL	m_bAutoWPN;
	BOOL	m_bSavePos;
	BOOL	m_bImage;
	BOOL    m_bMiniTray;
	BOOL	m_bFocus;
	BOOL	m_bHiliteUsers;
	BOOL	m_bEmoticons;
	BOOL	m_bUpdate;
	BOOL	m_bAutoList;
	BOOL	m_bMaxi;
	BOOL    m_bScroller;
	BOOL	m_bDisplayNode;
	BOOL    m_bHideSystem;
	BOOL    m_bBarTop;
	BOOL	m_bRetry;
	BOOL	m_bPMToolTip;
	int		m_nRetries;
	BOOL    m_bShowURLWarning;
	BOOL	m_bShowExitConfirm;
	BOOL	m_bBlockMCMA;
	BOOL    m_bMCMARawname;
	BOOL	m_bOpsTop;
    BOOL	m_bCacheRooms;
	BOOL    m_bCacheFavs;
	BOOL	m_bHiliteTop;

	COLORREF	m_crLink;
	COLORREF	m_crFocus;
	COLORREF	m_crNick;
	COLORREF	m_crMessage;
	COLORREF	m_crAction;
	COLORREF	m_crJoin;
	COLORREF	m_crPart;
	COLORREF	m_crTopic;
	COLORREF	m_crMotd;
	COLORREF	m_crNickBr;
	COLORREF	m_crActionBr;
	COLORREF	m_crFiles;
	COLORREF	m_crLine;
	COLORREF	m_crIP;
	COLORREF	m_crPort;
	COLORREF	m_crPend;
	COLORREF	m_crOK;
	COLORREF	m_crErr;
	COLORREF	m_crBg;
	COLORREF    m_crTime;
	COLORREF    m_crHiLite;
	COLORREF    m_crHiLiteTxt;
	COLORREF    m_crGrid;
	COLORREF    m_crGridTxt;
	COLORREF	m_crDocHiLite;
	COLORREF    m_crOp;
	CString		m_FA;
	CString		m_FM;
	CString		m_EA;
	CString		m_EM;
	CString		m_strFont;
	CString		m_strEnterMsg;
	CString		m_strWinampMsg;
	CString		m_strVideoMsg;
	CString	    m_strJoin;
	CString		m_strNickname;
	CString     m_strPart;
	CString     m_strImage;
	DWORD		m_dwFiles;
	WORD		m_wLine;

	// Sound stuff
	CString		m_strSfxPM;
	CString     m_strSfxJoin;
	CString	    m_strSfxPart;
	CString		m_strSfxRedirect;
	CString		m_strSfxTopic;
	CString		m_strSfxMotd;
	CString		m_strSfxStart;
	CString		m_strSfxError;
	volatile BOOL		m_bChatSfx;
	BOOL		m_bSoundFX;
		
	DWORD      m_dwNodeIP;
	DWORD	   m_dwNodePort;

	DWORD	   m_dwTextStyle;

	CString    m_strToolList;
	CString	   m_strToolRoom;
	CString    m_strToolSettings;
	CString    m_strToolPM;
	CString    m_strToolFiles;
	CString	   m_strToolListHover;
	int        m_nToolEffect;
	COLORREF   m_crToolBg1;
	COLORREF   m_crToolBg2;
	COLORREF   m_crToolBg3;
	int        m_nToolGran;
	int        m_nToolTrans;
	BOOL	   m_bOldTools;

	CString    m_strUserTool;
	int        m_nUserToolEffect;
	COLORREF   m_crUserToolBg1;
	COLORREF   m_crUserToolBg2;
	COLORREF   m_crUserToolBg3;
	int        m_nUserToolGran;
	int        m_nUserToolTrans;
	BOOL	   m_bUserOldTools;

	CString    m_strDefaultUserImage;
	CString    m_strDefaultChannelImage;
	CString	   m_strModeNormal;
	CString	   m_strModeVoiced;
	CString	   m_strModeAdmin;
	CString	   m_strModeHost;
	WORD	   m_wUserlistMode;
	CString	   m_strUserlistBg;
	BOOL	   m_bUserUserlistBg;

	BOOL m_bTile;
	int m_nXOffset;
	int m_nYOffset;
	
	BOOL	   m_bAutoBack;
	DWORD      m_dwIdleDuration;
	BOOL       m_bAwayIfIdle;
	CString    m_strAutoReason;
	CString    m_strAutoPrefix;
	CString    m_strAutoSuffix;


public:
	void LoadHiLite(void);
	void LoadUsers(void);
	void SaveUsers(void);

#ifdef _ROBO_READER
public:
	CString GetMale(){ return m_strMale; }
	CString GetFemale(){ return m_strFemale; }

	void SetMale(CString strValue){ m_strMale = strValue; }
	void SetFemale(CString strValue){ m_strFemale = strValue; }

protected:
	CString m_strMale;
	CString m_strFemale;
#endif
public:
	CString GetUserModeBmp(int nMode);
};

#endif // !defined(AFX_SETTINGS_H__25ADA47A_C2DF_42B2_B3AE_6C12BA658D11__INCLUDED_)
