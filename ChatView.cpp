/*
** Copyright (C) 2004 Bender
**  
** 
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

#include <math.h>
#include "MainFrm.h"
#include "ChatDoc.h"
#include "ChatView.h"
#include "RenameDlg.h"
#include "Clipboard.h"
#include "RichElement.h"
#include "Settings.h"
#include "Tokenizer.h"
#include "SfxCfg.h"
#include "ini.h"
#include "InputRequest.h"
#include "RoboEx.h"
#include <mmsystem.h>
#include "RoboEx.h"
#include "Util.h"
#include "mysocket.h"
#include "ping.h"
#include "RichFragment.h"
#include "MessageView.h"
#include "LinkWarning.h"
#include "EmoticonManager.h"
#include "LogView.h"
#include "SystemInfo.h"
#include "rmx_qapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const CString COLOR_DELIM((char)0x03); // delimiter

#ifdef _ROBO_READER
#include "VoiceDlg.h"
#include ".\chatview.h"
extern CVoiceDlg m_dlgVoice;
#endif

const BYTE color_map[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 48, 
                          49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
						  59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 
						  69, 70, 71, 72, 73, 74, 75, 76, 80, 81, 
						  82, 83, 84, 221, 228, 237, 244, 252, 253, 254};

const COLORREF rgb_map[] = {1, 2, 3, 4, 5, 6, 7, 8, 
							RGB(127,127,127), RGB(255, 255, 255), RGB(0, 0, 0), 
							RGB(0, 0, 255),   RGB(0, 128, 255), RGB(0, 255, 255), 
							RGB(0, 255, 128), RGB(0, 255, 0),   RGB(128, 255, 0), 
							RGB(255, 255, 0), RGB(255, 128, 0), RGB(255, 0, 0),
							RGB(255, 0, 128), RGB(255, 0, 255), RGB(128, 0, 255), 
							RGB(192, 192, 192), RGB(64, 64, 64), RGB(0, 0, 128), 
							RGB(0, 64, 128), RGB(0, 128, 128), RGB(0, 128, 64), 
							RGB(0, 128, 0), RGB(64, 128, 0), RGB(128, 128, 0),
							RGB(128, 64, 0), RGB(128, 0, 0), RGB(128, 0, 64), 
							RGB(128, 0, 128), RGB(64, 0, 128), RGB(127, 127, 127), 
							RGB(127, 127, 127),	RGB(127, 127, 127),	RGB(127, 127, 127),
							RGB(127, 127, 127),	RGB(127, 127, 127),	RGB(208, 182, 82),
							RGB(86, 6, 9), 	RGB(128, 5, 18), RGB(56, 4, 49),
							RGB(84, 160, 70), RGB(23, 55, 0), RGB(55, 23, 0)
							};
							

const BYTE num_color = sizeof(color_map)/sizeof(char);

#define COLOR_1		0x01 // 01 Normal Textcolor
#define COLOR_2		0x02 // 02 Name color 
#define COLOR_3		0x03 // 03 action color 
#define COLOR_4		0x04 // 04 join color 
#define COLOR_5		0x05 // 05 part color 
#define COLOR_6		0x06 // 06 motd color 
#define COLOR_7		0x07 // 07 topic color 
#define COLOR_8		0x08 // 08 operator color 
#define COLOR_9		0x09 // 09 notifications 
#define COLOR_48	0x30 // 10 ---
#define COLOR_49	0x31 // 11
#define COLOR_50	0x32 // 12 
#define COLOR_51	0x33 // 13 
#define COLOR_52	0x34 // 14 
#define COLOR_53	0x35 // 15 
#define COLOR_54	0x36 // 16 
#define COLOR_55	0x37 // 17 
#define COLOR_56	0x38 // 18 
#define COLOR_57	0x39 // 19 
#define COLOR_58	0x3A // 20
#define COLOR_59	0x3B // 21 
#define COLOR_60	0x3C // 22 
#define COLOR_61	0x3D // 23 
#define COLOR_62	0x3E // 24 
#define COLOR_63	0x3F // 25 
#define COLOR_64	0x40 // 26 
#define COLOR_65	0x41 // 27 
#define COLOR_66	0x42 // 28 
#define COLOR_67	0x43 // 29
#define COLOR_68	0x44 // 30 
#define COLOR_69	0x45 // 31
#define COLOR_70	0x46 // 32 
#define COLOR_71	0x47 // 33 
#define COLOR_72	0x48 // 34 
#define COLOR_73	0x49 // 35 
#define COLOR_74	0x4A // 36 
#define COLOR_75	0x4B // 37
#define COLOR_76    0x4C // 38
#define COLOR_80    0x50 // 39
#define COLOR_81    0x51 // 40
#define COLOR_82    0x52 // 41
#define COLOR_83    0x53 // 42
#define COLOR_84    0x54 //	43
#define COLOR_221	0xDD // 44 ---
#define COLOR_228	0xE4 // 45 ---
#define COLOR_237	0xED // 46 ---
#define COLOR_244   0xF4
#define COLOR_247	0xF7 // 47 ---
#define COLOR_252	0xFC // 48 ...
#define COLOR_253	0xFD // 49
#define COLOR_254	0xFE // 50
                    
#define ID_CSTM_MENUBASE  40000
#define ID_CSTM_MENUBASE2 41001
#define ID_AWAY_BASE	  42002//32796
#define ID_AWAY_MAX       42102

#define TIMER_CONNECT 10334
#define TIMER_PING    10035
#define TIMER_REDIRECT 10036

extern UINT UWM_TEXT_INPUT;
extern UINT UWM_MESSAGE;
extern UINT UWM_ACTION;
extern UINT UWM_TOPIC;
extern UINT UWM_MOTD;
extern UINT UWM_JOIN;
extern UINT UWM_PART;
extern UINT UWM_RENAME;
extern UINT UWM_ADDUSER;
extern UINT UWM_UNKNOWN;
extern UINT UWM_SYSTEM;
extern UINT UWM_REDIRECT;
extern UINT UWM_LOAD_SETTINGS;
extern UINT UWM_ROOMRENAME;
extern UINT UWM_SERVERTYPE;
extern UINT UWM_OPMESSAGE;
extern UINT UWM_WPNNOTIFY;
extern UINT UWM_WHOIS;
extern UINT UWM_ADDLISTENER;
extern UINT UWM_REMLISTENER;
extern UINT UWM_BROWSE;
extern UINT UWM_NEWPM;
extern UINT UWM_IPCLIENT;

#define WM_ECHOTEXT WM_APP+1
#define WM_ECHOSYSTEXT WM_APP+2

extern CSettings g_sSettings;
extern CSystemInfo g_sSystem;
extern CArray<PLUGIN_DATA, PLUGIN_DATA> g_aPlugins;
extern CStringArray g_aHistory;
extern CEmoticonManager g_eEmoticons;

#define PLUGIN g_aPlugins[i].pEx
#define PLUGIN_VER g_aPlugins[i].lApiVersion

UINT UWM_SETAWAY = ::RegisterWindowMessage("UWM_SETAWAY-{494D99C1-03BE-49e3-8A47-D0D17C6D4ACE}");
UINT UWM_SETBACK = ::RegisterWindowMessage("UWM_SETBACK-{494D99C1-03BE-49e3-8A47-D0D17C6D4ACE}");
UINT UWM_SETCHANNELPIC = ::RegisterWindowMessage("UWM_SETCHANNELPIC-{494D99C1-03BE-49e3-8A47-D0D17C6D4ACE}");
UINT UWM_RENAMECL = ::RegisterWindowMessage("UWM_RENAMECL-{494D99C1-03BE-49e3-8A47-D0D17C6D4ACE}");
/////////////////////////////////////////////////////////////////////////////
// CChatView

IMPLEMENT_DYNCREATE(CChatView, CFormView)

BEGIN_MESSAGE_MAP(CChatView, CFormView)
	ON_COMMAND(ID_RENAME, OnRename)
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_LEAVE, OnLeave)
	ON_NOTIFY(NM_RCLICK, IDC_USERLIST, OnRclickUserlist)
	ON_COMMAND(ID_USERLIST_SENDMESSAGE, OnUserlistSendmessage)
	ON_COMMAND(ID_USERLIST_REMOVEADMIN, OnUserlistRemoveadmin)
	ON_COMMAND(ID_USERLIST_REDIRECT, OnUserlistRedirect)
	ON_COMMAND(ID_USERLIST_PRINTUSERSTAT, OnUserlistPrintuserstat)
	ON_COMMAND(ID_USERLIST_PRINTIP, OnUserlistPrintip)
	ON_COMMAND(ID_USERLIST_UNBAN, OnUserlistUnban)
	ON_COMMAND(ID_USERLIST_KICKBAN, OnUserlistKickban)
	ON_COMMAND(ID_USERLIST_KICK, OnUserlistKick)
	ON_COMMAND(ID_USERLIST_COPYUSERNAME, OnUserlistCopyusername)
	ON_COMMAND(ID_USERLIST_BAN, OnUserlistBan)
	ON_COMMAND(ID_USERLIST_ADDADMIN, OnUserlistAddadmin)
	ON_COMMAND(ID_USERLIST_READUSERMESSAGE, OnUserlistReadusermessage)
	ON_COMMAND(ID_USERLIST_CUSTOMIZETHISMENU, OnUserlistCustomizethismenu)
	ON_COMMAND(ID_USERLIST_IGNORE, OnUserlistIgnore)
	ON_COMMAND(ID_USERLIST_UNIGNORE, OnUserlistUnignore)
	ON_COMMAND(ID_RECONNECT, OnReconnect)
	ON_COMMAND(ID_CHATROOM_REJOIN, OnReconnect)
	ON_UPDATE_COMMAND_UI(ID_RECONNECT, OnUpdateReconnect)
	ON_UPDATE_COMMAND_UI(ID_CHATROOM_DISPLAYINCHANNEL_WINAMPSONG, OnUpdateWinampsongMenu)
	ON_COMMAND(ID_CHATROOM_CLEARSCREEN, OnChatroomClearscreen)
	ON_COMMAND(ID_CHATROOM_COPYROOMNAME, OnChatroomCopyroomname)
	ON_COMMAND(ID_CHATROOM_DISPLAYINCHANNEL_WINAMPSONG, OnDisplayWinampsong)
	ON_COMMAND(ID_CHATROOM_DISPLAYINCHANNEL_SYSTEMUPTIME, OnDisplaySystemuptime)
	ON_COMMAND(ID_CHATROOM_DISPLAYINCHANNEL_SYSTEMINFO, OnDisplaySysteminfo)
	ON_COMMAND(ID_CHATROOM_DISPLAYINCHANNEL_AVERAGELAG, OnDisplayAveragelag)
	ON_COMMAND(ID_CHATROOM_TEXTTRICKS_BUBBLES, OnChatroomTexttricksBubbles)
	ON_COMMAND(ID_CHATROOM_TEXTTRICKS_BOX, OnChatroomTexttricksBox)
	ON_COMMAND(ID_CHATROOM_TEXTTRICKS_HACKER, OnChatroomTexttricksHacker)
	ON_COMMAND(ID_CHATROOM_ASCIIARTDESIGN, OnChatroomAsciiartdesign)
	ON_COMMAND(ID_CHAT_TEXTTRICKS_3DBUTTONSNORMAL, OnChatTexttricks3dbuttonsnormal)
	ON_COMMAND(ID_CHAT_TEXTTRICKS_3DBUTTONSACTION, OnChatTexttricks3dbuttonsaction)
	//ON_UPDATE_COMMAND_UI_RANGE(ID_USERLIST_SENDMESSAGE,ID_USERLIST_READUSERMESSAGE, OnUpdateUserlistMenu)
	ON_RICH_NOTIFY(RVN_RCLICK, IDC_CHAT_WND, OnRclickChat)
	ON_MESSAGE(WM_ECHOTEXT, OnEchoText)
	ON_MESSAGE(WM_ECHOSYSTEXT, OnEchoSysText)
	ON_REGISTERED_MESSAGE(UWM_TEXT_INPUT, OnInput)
	ON_REGISTERED_MESSAGE(UWM_MESSAGE, OnMessage)
	ON_REGISTERED_MESSAGE(UWM_ACTION, OnAction)
	ON_REGISTERED_MESSAGE(UWM_TOPIC, OnTopic)
	ON_REGISTERED_MESSAGE(UWM_MOTD, OnMotd)
	ON_REGISTERED_MESSAGE(UWM_JOIN, OnJoin)
	ON_REGISTERED_MESSAGE(UWM_PART, OnPart)
	ON_REGISTERED_MESSAGE(UWM_RENAME, OnRenameMsg)
	ON_REGISTERED_MESSAGE(UWM_ADDUSER, OnAddUser)
	ON_REGISTERED_MESSAGE(UWM_UNKNOWN, OnUnknown)
	ON_REGISTERED_MESSAGE(UWM_SYSTEM, OnSystem)
	ON_REGISTERED_MESSAGE(UWM_REDIRECT, OnRedirect)
	ON_REGISTERED_MESSAGE(UWM_LOAD_SETTINGS, OnReloadCfg)
	ON_REGISTERED_MESSAGE(UWM_ROOMRENAME, OnRoomRename)
	ON_REGISTERED_MESSAGE(UWM_SERVERTYPE, OnSetServerType)
	ON_REGISTERED_MESSAGE(UWM_OPMESSAGE, OnOpMessage)
	ON_REGISTERED_MESSAGE(UWM_RENAMECL, OnRenameCl)
	ON_REGISTERED_MESSAGE(UWM_IPCLIENT, OnIPClient)
	ON_REGISTERED_MESSAGE(UWM_WPNNOTIFY, OnWPNNotify)
	ON_UPDATE_COMMAND_UI_RANGE(ID_USER_BROWSE, ID_USERLIST_WHOIS, OnUpdateUserlistMenu)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SYSTEMCONSOLE, OnUpdateViewSystemconsole)
	ON_COMMAND_RANGE(ID_BACK_SETBACK, ID_BACK_SETSI8LENTBACK, OnAwayControlBack)
	ON_COMMAND_RANGE(ID_CSTM_MENUBASE, ID_CSTM_MENUBASE+1000, OnUserCmd)
	ON_COMMAND_RANGE(ID_CSTM_MENUBASE2, ID_CSTM_MENUBASE2+1000, OnChatCmd)
	ON_COMMAND(ID_VIEW_LOGFILE, OnViewLogfile)
	ON_COMMAND(ID_CHATROOM_VIEWTOPIC, OnChatroomViewtopic)
	ON_COMMAND(ID_CHATROOM_ADDTOAUTOJOIN, OnChatroomAddtoautojoin)
	ON_COMMAND(ID_CHATROOM_VIEWCURRENTBANS, OnChatroomViewcurrentbans)
	ON_COMMAND(ID_DISPLAYINCHANNEL_PING, OnDisplayinchannelPing)
	ON_COMMAND(ID_VIEW_SYSTEMCONSOLE, OnViewSystemconsole)
	ON_COMMAND(ID_USERLIST_WHOIS, OnUserlistWhois)
	ON_COMMAND(ID_USER_BROWSE, OnUserlistBrowse)
	ON_COMMAND(ID_OPEN_OPEN, OnOpenURL)
	ON_COMMAND(ID_OPEN_OPENINNEWBROWSER, OnOpenURLinnewbrowser)
	ON_COMMAND(ID_COPY_ADDRESS, OnCopyAddress)
	ON_COMMAND(ID_CUSTOMIZE, OnCustomizeUserCmds)
	ON_COMMAND(ID_CHAT_CHANNELSTATISTICS, OnChatChannelstatistics)
	ON_COMMAND(ID_DISPLAYINCHANNEL_LURKMETER, OnDisplayinchannelLurkmeter)
	ON_COMMAND(ID_TEXTTRICKS_3DBUTTONS, OnTexttricks3dbuttons)
	ON_COMMAND(ID_DISPLAYINCHANNEL_STATISTICS, OnDisplayinchannelStatistics)
	ON_COMMAND(ID_TEXTTRICKS_WILDCOLORS, OnTexttricksWildcolors)
	ON_COMMAND(ID_DISPLAYINCHANNEL_EXTENDEDSYSTEMINFO, OnDisplayinchannelExtendedsysteminfo)
	ON_COMMAND(ID_ADVANCED_TRUSTUSER, OnAdvancedTrustuser)
	ON_UPDATE_COMMAND_UI(ID_ADVANCED_TRUSTUSER, OnUpdateAdvancedTrustuser)
	ON_COMMAND(ID_ADVANCED_REMOVETRUSTED, OnAdvancedRemovetrusted)
	ON_UPDATE_COMMAND_UI(ID_ADVANCED_REMOVETRUSTED, OnUpdateAdvancedRemovetrusted)
	ON_COMMAND(ID_DISPLAYINCHANNEL_ONLINETIME, OnDisplayinchannelOnlineTime)
	//ON_NOTIFY(HDN_ITEMDBLCLICK, 0, OnHdnItemdblclickUserlist)
	ON_NOTIFY(NM_DBLCLK, IDC_USERLIST, OnDblclickUserlist)
	ON_NOTIFY (UDM_TOOLTIP_DISPLAY, NULL, NotifyDisplayTooltip)
	ON_MESSAGE(WM_QAPI, OnPluginQueryInfo)
	ON_COMMAND(ID_ADVANCED_SETPICTURE, OnAdvancedSetpicture)
	ON_COMMAND(ID_USERLIST_DETAILEDVIEW, OnUserlistDetailedview)
	ON_UPDATE_COMMAND_UI(ID_USERLIST_DETAILEDVIEW, OnUpdateUserlistDetailedview)
	ON_COMMAND(ID_USERLIST_SHORTVIEW, OnUserlistShortview)
	ON_UPDATE_COMMAND_UI(ID_USERLIST_SHORTVIEW, OnUpdateUserlistShortview)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_USERLIST, OnLvnItemchangedUserlist)
	ON_REGISTERED_MESSAGE(UWM_SETCHANNELPIC, OnSetChannelPic)
	ON_COMMAND_RANGE(ID_AWAY_BASE, ID_AWAY_MAX, OnAwayControl)
	ON_EN_CHANGE(IDC_INPUT, OnEnChangeInput)
	END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatView construction/destruction

CChatView::CChatView()
	: CFormView(CChatView::IDD)
{

	m_strInput		= _T("");
	m_pStatusBar	= 0;
	m_dwAvLag		= 0;
	m_dwLastTic		= 0;
	m_bHasJoined	= FALSE;
	m_nServerType	= SERVER_RCMS;
	m_bHideSystem	= FALSE;
	m_nRetries		= 0;
	m_dwLast		= 0;
	m_dwMessages	= 0;
	//m_bAni			= FALSE;
	m_dwYourMessages   = 0;
	m_bMsgAfterLastSep = TRUE;
	m_bShowStatus	 = TRUE;
}

CChatView::~CChatView()
{

	m_iImageList.DeleteImageList();
	m_fFont.DeleteObject();
}

void CChatView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatView)
	DDX_Control(pDX, IDC_LEAVE, m_btExit);
	DDX_Control(pDX, IDC_INPUT, m_eInput);
	DDX_Control(pDX, IDC_USERLIST, m_lcUsers);
	DDX_Text(pDX, IDC_INPUT, m_strInput);
	DDV_MaxChars(pDX, m_strInput, 400);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LETTERSLEFT, m_pgLetters);
}

BOOL CChatView::PreCreateWindow(CREATESTRUCT& cs)
{

	return CFormView::PreCreateWindow(cs);
}

void CChatView::OnInitialUpdate()
{
	
	CFormView::OnInitialUpdate();
    GetParentFrame()->RecalcLayout();
	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	int nCnt = ini.GetValue("ChannelToolTips", "ChannelImageCnt", 0);
	
	CString strLabel, strChannel, strImage;
	
	for(int i = 0; i < nCnt; i++){

		strLabel.Format("ChannelName_%d", i);
		strChannel = ini.GetValue("ChannelToolTips", strLabel, "");

		if(strChannel.GetLength() && Util::WildcardMatch(GetDocument()->m_strRoom, strChannel)){

			strLabel.Format("ChannelImage_%d", i);
			strImage = ini.GetValue("ChannelToolTips", strLabel, "");
			break;
		}
	}
	if(i >= nCnt){

		strImage.Empty();
		strImage = g_sSettings.GetChannelDefaultImage();
	}
	if(strImage.IsEmpty()){

		GetApp()->GetMainFrame()->m_wndDocSelector.AddButton( this, IDR_CHANNEL);
	}
	else{

		GetApp()->GetMainFrame()->m_wndDocSelector.AddButton( this, 0, strImage);
	}

	CTime now = CTime::GetCurrentTime();
	ini.SetValue("LastJoins", GetDocument()->m_strRoomShort, now.Format("%A, %B %d, %Y, %H:%M:%S"));
	
	int nJoin = ini.GetValue("LastJoins", GetDocument()->m_strRoomShort + "_Count", 0);
	nJoin++;
	ini.SetValue("LastJoins", GetDocument()->m_strRoomShort + "_Count", nJoin);

	::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_ADDLISTENER, 0, (LPARAM)this);

	m_pStatusBar = (CColorStatusBar *)AfxGetApp()->m_pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
	
	LoadRCMSMenu();

	// Splitter between userlist and chat-/systemview
	CRect rc(441, 3, 444, 352);
	m_sSplitter1.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER_1);
	m_sSplitter1.SetStyle(SPS_VERTICAL);
	m_sSplitter1.SetRange(200, 200, 1);
	
	// Splitter between chat-/systemview
	rc.SetRect(2, 88, 442, 93);
	m_sSplitter2.Create(WS_CHILD | WS_VISIBLE, rc, this, IDC_SPLITTER_2);
	m_sSplitter2.SetStyle(SPS_HORIZONTAL);
	m_sSplitter2.SetRange(150, 150, 1);

	// Systemconsole
	rc.SetRect(2, 3, 445, 88);
	m_rSys.Create(WS_CHILD|WS_VISIBLE, rc, this, IDC_SYS_WND);
	m_rSys.SetDocument(&m_rSysDoc);
	m_rSys.SetSelectable(TRUE);
	m_rSys.SetFollowBottom(TRUE);
	m_rSysDoc.m_szMargin = CSize(1, 10);
	m_rSysDoc.m_crBackground = g_sSettings.GetRGBBg();
	m_rSysDoc.m_crLink		 = g_sSettings.GetRGBLink();

	// chatview
	rc.SetRect(2, 93, 442, 353);
	m_rChat.Create(WS_CHILD|WS_VISIBLE, rc, this, IDC_CHAT_WND);
	m_rChat.SetDocument(&m_rChatDoc);
	m_rChat.SetSelectable(TRUE);
	m_rChat.SetFollowBottom(TRUE);
	m_rChatDoc.m_szMargin = CSize(1, 10);
	m_rChatDoc.m_crBackground = g_sSettings.GetRGBBg();
	m_rChatDoc.m_crLink		  = g_sSettings.GetRGBLink();

	m_rSysDoc.CreateFonts(g_sSettings.GetFont(), g_sSettings.GetFontSize());
	m_rChatDoc.CreateFonts(g_sSettings.GetFont(), g_sSettings.GetFontSize());

	m_fFont.CreateFont(15, 6, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_STRING_PRECIS, 
		CLIP_STROKE_PRECIS, PROOF_QUALITY, FF_DONTCARE, g_sSettings.GetFont());
	
	if(g_sSettings.GetUserUseOldTools()){

		m_lcUsers.SetExtendedStyle(LVS_EX_FULLROWSELECT |LVS_EX_LABELTIP);
	}
	else{

		m_lcUsers.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	}
	m_lcUsers.m_bUserList = TRUE;
	
	m_iImageList.Create(16, 16, ILC_COLORDDB|ILC_MASK, 4, 1);

	HBITMAP hTmp = (HBITMAP)::LoadImage(NULL, g_sSettings.GetUserModeNormal(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_DEFAULTCOLOR);
	if(hTmp){

		m_iImageList.Add(CBitmap::FromHandle(hTmp), RGB(255,0,255));
		DeleteObject(hTmp);
	}
	hTmp = (HBITMAP)::LoadImage(NULL, g_sSettings.GetUserModeAdmin(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_DEFAULTCOLOR);
	if(hTmp){

		m_iImageList.Add(CBitmap::FromHandle(hTmp), RGB(255,0,255));
		DeleteObject(hTmp);
	}

	hTmp = (HBITMAP)::LoadImage(NULL, g_sSettings.GetUserModeVoiced(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_DEFAULTCOLOR);
	if(hTmp){

		m_iImageList.Add(CBitmap::FromHandle(hTmp), RGB(255,0,255));
		DeleteObject(hTmp);
	}

	hTmp = (HBITMAP)::LoadImage(NULL, g_sSettings.GetUserModeHost(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_DEFAULTCOLOR);
	if(hTmp){

		m_iImageList.Add(CBitmap::FromHandle(hTmp), RGB(255,0,255));
		DeleteObject(hTmp);
	}

	m_lcUsers.SetImageList(&m_iImageList, LVSIL_SMALL);
    m_lcUsers.SetImageList(&m_iImageList, LVSIL_NORMAL);
	if(g_sSettings.GetDisplayNode()){

		m_lcUsers.SetHeadings(IDS_USERLIST_NODE);
	}
	else{

		m_lcUsers.SetHeadings(IDS_USERLIST_SHORT);
	}
	m_lcUsers.LoadColumnInfo();
	
	m_lcUsers.SetColors(
		g_sSettings.GetRGBNormalMsg(),
		g_sSettings.GetRGBFiles(),
		g_sSettings.GetRGBLine(),
		g_sSettings.GetRGBIP(),
		g_sSettings.GetRGBPort()
		);
	m_lcUsers.SetBkColor(g_sSettings.GetRGBBg());	  

	if(g_sSettings.GetUseUserlistBg()){

		CImage imgTemp;
		imgTemp.Load(g_sSettings.GetUserlistBg());
		m_lcUsers.SetBkImage((HBITMAP)imgTemp, g_sSettings.GetUserlistTileBg(), 
								g_sSettings.GetUserlistXOffsest(), g_sSettings.GetUSerlistYOffset());
		imgTemp.Destroy();
	}

	switch(g_sSettings.GetUserlistMode()){

		case 1:
			m_lcUsers.ModifyStyle(LVS_REPORT, LVS_LIST, 0);
			break;
		case 2:
			m_lcUsers.ModifyStyle(LVS_REPORT|LVS_NOLABELWRAP ,LVS_SMALLICON, 0);
			break;
		case 3:
			m_lcUsers.ModifyStyle(LVS_REPORT|LVS_NOLABELWRAP, LVS_ICON, 0);
			break;
	}

	m_lcUsers.ModifyStyle(0, LVS_SINGLESEL, 0);

	if(g_sSettings.GetHiliteUsers()){

		m_lcUsers.SetHiLite();
	}

	m_lcUsers.SetFont(&m_fFont);

	m_pgLetters.SetRange(0, 400);

	m_bShowStatus = g_sSettings.GetShowFillStatus();
	if(m_bShowStatus){

		m_pgLetters.ShowWindow(SW_SHOW);
	}
	else{

		m_pgLetters.ShowWindow(SW_HIDE);
	}

	m_eInput.SetFont(&m_fFont);
	m_eInput.SetBkColor(g_sSettings.GetRGBBg());
	m_eInput.SetExtMenu();
	m_eInput.SetCommands(&g_sSettings.m_aRCMSCommands);

	if(!g_sSettings.GetUserUseOldTools()){

		if(m_ToolTip.Create(this, TRUE)){

			m_ToolTip.SetNotify();
			m_ToolTip.EnableEscapeSequences(FALSE);
			m_ToolTip.SetEffectBk(g_sSettings.GetUserToolEffect(), g_sSettings.GetUserToolGran());
			m_ToolTip.SetBehaviour(PPTOOLTIP_MULTIPLE_SHOW);
			m_ToolTip.SetColorBk(g_sSettings.GetUserToolBg1(), g_sSettings.GetUserToolBg2(), g_sSettings.GetUserToolBg3());
			m_ToolTip.SetSize(CPPToolTip::PPTTSZ_MARGIN_ANCHOR, 0);
			m_ToolTip.SetSize(CPPToolTip::PPTTSZ_WIDTH_ANCHOR, 0);
			m_ToolTip.SetSize(CPPToolTip::PPTTSZ_HEIGHT_ANCHOR, 0);
			m_ToolTip.HideBorder();
			m_ToolTip.AddTool(&m_lcUsers);
			m_ToolTip.AddTool(&m_rChat);
		}
		else{

			WriteSystemMsg(TRUE, IDS_ERROR_CREATETOOL, g_sSettings.GetRGBErr());
		}
	}

	// Set up client
	m_sSplitter1.SetRange(150, 50, -1);

	if(g_sSettings.GetMaxi() && !g_sSettings.GetSavePos()){

		ReCalcLayout();
	}
	else{

		ResizeParentToFit(FALSE);
	}

	m_lLog.SetRoom(GetDocument()->m_strRoomShort);

	// Setup client
	m_mxClient.m_hView		= m_hWnd;
	m_mxClient.m_dwFiles	= GetDocument()->m_dwFiles;
	m_mxClient.m_wLineType	= GetDocument()->m_wLine;
	m_mxClient.m_strUser	= GetDocument()->m_strName;
	m_mxClient.m_bOldJoin   = GetDocument()->m_bOldJoin;

	m_mxClient.SetRoom(GetDocument()->m_strRoom);
	WriteSystemMsg(FALSE, IDS_CONNECTING, g_sSettings.GetRGBPend());

	OnUpdate(this, 0, NULL);
	
	SetTimer(TIMER_CONNECT, 500, 0);
	m_dwLast = GetTickCount();
}


LRESULT CChatView::OnReloadCfg(WPARAM w, LPARAM l)
{

	m_fFont.DeleteObject();
	m_fFont.CreateFont(15, 6, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_STRING_PRECIS, 
		CLIP_STROKE_PRECIS, PROOF_QUALITY, FF_DONTCARE, g_sSettings.GetFont());
	m_lcUsers.SetFont(&m_fFont);
	m_eInput.SetFont(&m_fFont);

	m_lcUsers.SetColors(
		g_sSettings.GetRGBNormalMsg(),
		g_sSettings.GetRGBFiles(),
		g_sSettings.GetRGBLine(),
		g_sSettings.GetRGBIP(),
		g_sSettings.GetRGBPort()
		);
	m_lcUsers.SetBkColor(g_sSettings.GetRGBBg());
	if(g_sSettings.GetUseUserlistBg()){

		CImage imgTemp;
		imgTemp.Load(g_sSettings.GetUserlistBg());
		m_lcUsers.SetBkImage((HBITMAP)imgTemp, g_sSettings.GetUserlistTileBg(), 
								g_sSettings.GetUserlistXOffsest(), g_sSettings.GetUSerlistYOffset());
		imgTemp.Destroy();
	}


	m_lcUsers.SetImageList(0, LVSIL_SMALL);
    m_lcUsers.SetImageList(0, LVSIL_NORMAL);

	m_iImageList.DeleteImageList();
	m_iImageList.Create(16, 16, ILC_COLORDDB|ILC_MASK, 4, 1);

	HBITMAP hTmp = (HBITMAP)::LoadImage(NULL, g_sSettings.GetUserModeNormal(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_DEFAULTCOLOR);
	if(hTmp){

		m_iImageList.Add(CBitmap::FromHandle(hTmp), RGB(255,0,255));
		DeleteObject(hTmp);
	}
	hTmp = (HBITMAP)::LoadImage(NULL, g_sSettings.GetUserModeAdmin(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_DEFAULTCOLOR);
	if(hTmp){

		m_iImageList.Add(CBitmap::FromHandle(hTmp), RGB(255,0,255));
		DeleteObject(hTmp);
	}

	hTmp = (HBITMAP)::LoadImage(NULL, g_sSettings.GetUserModeVoiced(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_DEFAULTCOLOR);
	if(hTmp){

		m_iImageList.Add(CBitmap::FromHandle(hTmp), RGB(255,0,255));
		DeleteObject(hTmp);
	}

	hTmp = (HBITMAP)::LoadImage(NULL, g_sSettings.GetUserModeHost(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_DEFAULTCOLOR);
	if(hTmp){

		m_iImageList.Add(CBitmap::FromHandle(hTmp), RGB(255,0,255));
		DeleteObject(hTmp);
	}

	m_lcUsers.SetImageList(&m_iImageList, LVSIL_SMALL);
    m_lcUsers.SetImageList(&m_iImageList, LVSIL_NORMAL);

	if(g_sSettings.GetHiliteUsers()){

		m_lcUsers.SetHiLite();
	}
	else{

		m_lcUsers.SetHiLite(FALSE);
	}		 
	m_lcUsers.Invalidate();


	m_bShowStatus = g_sSettings.GetShowFillStatus();
	if(m_bShowStatus){

		m_pgLetters.ShowWindow(SW_SHOW);
	}
	else{

		m_pgLetters.ShowWindow(SW_HIDE);
	}
	m_pgLetters.Invalidate();

	m_eInput.SetBkColor(g_sSettings.GetRGBBg());
	m_rSys.Init(IDC_SYS);
	m_rSysDoc.m_crBackground = g_sSettings.GetRGBBg();
	m_rSysDoc.m_crLink       = g_sSettings.GetRGBLink();
	m_rSysDoc.CreateFonts(g_sSettings.GetFont(), g_sSettings.GetFontSize());

	m_rChat.Init(IDC_CHAT);
	m_rChatDoc.m_crBackground = g_sSettings.GetRGBBg();
	m_rChatDoc.m_crLink       = g_sSettings.GetRGBLink();
	m_rChatDoc.CreateFonts(g_sSettings.GetFont(), g_sSettings.GetFontSize());
	m_rChat.UpdateEmoticons();

	m_rChat.InvalidateIfModified();
	m_rSys.InvalidateIfModified();
	
	Invalidate();
	ReCalcLayout();

	return 1;
}

BOOL CChatView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{

	if (wParam == IDC_SPLITTER_1){	

		SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;
		DoResize1(pHdr->delta);
	}	
	else if (wParam == IDC_SPLITTER_2){	

		SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;
		DoResize2(pHdr->delta);
	}	
	return CFormView::OnNotify(wParam, lParam, pResult);
}

void CChatView::DoResize1(int delta)
{

	CSplitterControl::ChangeWidth(&m_lcUsers, -delta, CW_RIGHTALIGN);
	CSplitterControl::ChangeWidth(&m_rSys, delta, CW_LEFTALIGN);
	CSplitterControl::ChangeWidth(&m_rChat, delta, CW_LEFTALIGN);
	CSplitterControl::ChangeWidth(&m_sSplitter2, delta, CW_LEFTALIGN);
	Invalidate();
	UpdateWindow();
}

void CChatView::DoResize2(int delta)
{

	CSplitterControl::ChangeHeight(&m_rSys, delta, CW_TOPALIGN);
	CSplitterControl::ChangeHeight(&m_rChat, -delta, CW_BOTTOMALIGN);

	Invalidate();
	UpdateWindow();
}

/////////////////////////////////////////////////////////////////////////////
// CChatView diagnostics

#ifdef _DEBUG
void CChatView::AssertValid() const
{
	CFormView::AssertValid();
}

void CChatView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CChatDoc* CChatView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CChatDoc)));
	return (CChatDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChatView message handlers


void CChatView::OnDestroy() 
{
	
	KillTimer(TIMER_PING);

	::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_REMLISTENER, 0, (LPARAM)this);
	GetApp()->GetMainFrame()->m_wndDocSelector.RemoveButton(this);

	m_lcUsers.SaveColumnInfo();

	for(int i = 0; i < g_aPlugins.GetSize() && m_bHasJoined; i++){

		PLUGIN->OnLeaveChannel((DWORD)m_hWnd);
	}

	m_mxClient.m_hView = NULL;
	m_mxClient.Disconnect();

	if(g_sSettings.GetSoundFX() && m_bHasJoined){

		if(g_sSettings.GetSfxPart().IsEmpty()) return;
		PlaySound(g_sSettings.GetSfxPart(), NULL, SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
	}

	CFormView::OnDestroy();
}

void CChatView::OnTimer(UINT nIDEvent) 
{

	if(nIDEvent == TIMER_CONNECT){

		KillTimer(TIMER_CONNECT);

		// Load log
		if(g_sSettings.GetLog() && g_sSettings.GetLoadLog()){

			CString strDate;
			if(m_lLog.GetPreviousJoinDate(strDate)){

				// We have here before, so load the damn log :)
				CString strLog, strLine;
				
				m_lLog.GetLast50Lines(strLog);
				CTokenizer token(strLog, "\n");
				while(token.Next(strLine)){

					CRichElement* pE = 	m_rChatDoc.Add(retText, strLine, NULL,
											(g_sSettings.GetMessageBold() ? retfBold : 0) |
											(g_sSettings.GetMessageItalic() ? retfItalic : 0) |
											(g_sSettings.GetMessageLine() ? retfUnderline : 0) |
											retfColor);
					pE->m_cColor = RGB(150,150,150);
					pE->m_cBgColor = g_sSettings.GetRGBBg();
					m_rChatDoc.Add(retNewline, _T("4"));
				}
				m_rChatDoc.Add(retNewline, _T("4"));
			}
		} // end if load log

		if(m_mxClient.Connect()){

			SetTimer(TIMER_PING, 5*60*1000, NULL);
		}
	}
	else if(nIDEvent == TIMER_PING){

		if(m_mxClient.m_bListen){

			m_mxClient.Ping();
		}
	}
	else if(nIDEvent == TIMER_REDIRECT){

		CString strOut;

		strOut.Format(IDS_REDIRECTING, m_strTarget);
		WriteSystemMsg(FALSE, strOut, g_sSettings.GetRGBPend());

		// 0100007F1A2B
		if((m_strTarget.GetLength() - m_strTarget.ReverseFind('_') -1) != 12){

			strOut.Format(IDS_ERROR_REDIRECT_ROOMNAMEINVALID, m_strTarget);
			WriteSystemMsg(TRUE, strOut, g_sSettings.GetRGBErr());
			if(m_mxClient.m_bListen){

				m_mxClient.Disconnect();
			}
		}
		else{

			Disconnect();
			m_mxClient.SetRoom(m_strTarget);
			GetDocument()->m_strRoom = m_strTarget;
			GetDocument()->SetTitle(GetDocument()->m_strRoom, FALSE);
			m_mxClient.Connect();
			if(g_sSettings.GetSoundFX()){

				if(!g_sSettings.GetSfxRedirect().IsEmpty())
					PlaySound(g_sSettings.GetSfxRedirect(), NULL, SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
			}
		} // end redirect

		m_strTarget.Empty();
		KillTimer(TIMER_REDIRECT);

	}
	CFormView::OnTimer(nIDEvent);
}


void CChatView::WriteTime(COLORREF crTime)
{

	WriteText("[", g_sSettings.GetRGBTime());
	WriteText(Util::GetMyLocalTime(), crTime);
	WriteText("] ", g_sSettings.GetRGBTime());
}

void CChatView::WriteText(CString strText, COLORREF crText, COLORREF crBg , BOOL bUseBg, BOOL bNewLine, BOOL bName)
{

	m_bMsgAfterLastSep = TRUE;

	m_dwLast = GetTickCount();
	//if(m_bAni) StopAni();

	strText.Replace("\r\n", "\n");
	
	CTokenizer token(strText, COLOR_DELIM);

	CString strTmp;
	COLORREF crUse = crText;
	BOOL bDelim = FALSE;

	while(token.Next(strTmp, bDelim)){

		if(strTmp.IsEmpty()) break; // uups

		if(bDelim && !g_sSettings.GetBlockMCMA()){

			
			switch(strTmp[0]){

				case COLOR_1:  // Normal Textcolor
					crUse  = g_sSettings.GetRGBNormalMsg();
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_2:  // Name color
					crUse  = g_sSettings.GetRGBNormalName();
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_3:  // action color
					crUse  = g_sSettings.GetRGBActionMsg();
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_4:  // join color
					crUse  = g_sSettings.GetRGBJoin();
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_5:  // part color
					crUse  = g_sSettings.GetRGBPart();
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_6:  // motd color
					crUse  = g_sSettings.GetRGBMotd();
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_7:  // topic color
					crUse  = g_sSettings.GetRGBTopic();
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_8:  // operator color
					crUse  = g_sSettings.GetRGBOp();
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_9: 
					crUse  = rgb_map[8];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_48:  
					crUse  = rgb_map[9];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_49:  
					crUse  = rgb_map[10];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_50:  
					crUse  = rgb_map[11];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_51:  
					crUse  = rgb_map[12];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_52:  
					crUse  = rgb_map[13];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_53:  
					crUse  = rgb_map[14];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_54:  
					crUse  = rgb_map[15];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_55:  
					crUse  = rgb_map[16];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_56:  
					crUse  = rgb_map[17];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_57:  
					crUse  = rgb_map[18];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_58:  
					crUse  = rgb_map[19];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_59:  
					crUse  = rgb_map[20];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_60:  
					crUse  = rgb_map[21];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_61:  
					crUse  = rgb_map[22];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_62:  
					crUse  = rgb_map[23];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_63:  
					crUse  = rgb_map[24];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_64:  
					crUse  = rgb_map[25];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_65:  
					crUse  = rgb_map[26];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_66:  
					crUse  = rgb_map[27];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_67:  
					crUse  = rgb_map[28];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_68:  
					crUse  = rgb_map[29];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_69:  
					crUse  = rgb_map[30];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_70:  
					crUse  = rgb_map[31];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_71:  
					crUse  = rgb_map[32];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_72:  
					crUse  = rgb_map[33];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_73:  
					crUse  = rgb_map[34];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_74:  
					crUse  = rgb_map[35];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_75: 
					crUse  = rgb_map[36];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_76:
					crUse  = rgb_map[37];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_80:
					crUse  = rgb_map[38];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_81:
					crUse  = rgb_map[39];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_82:
					crUse  = rgb_map[40];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_83:
					crUse  = rgb_map[41];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_84:
					crUse  = rgb_map[42];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_221: 
					crUse  = rgb_map[43];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_228: 
					crUse  = rgb_map[44];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_237: 
					crUse  = rgb_map[45];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_244: 
					crUse  = rgb_map[46];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_252: 
					crUse  = rgb_map[47];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_253: 
					crUse  = rgb_map[48];
					strTmp = strTmp.Mid(1);
					break;
				case COLOR_254: 
					crUse  = rgb_map[49];
					strTmp = strTmp.Mid(1);
					break;
				default:
					strTmp = strTmp.Mid(1);
					crUse = crText;
					TRACE("%d <-> %s \n", strTmp[0], strTmp);
					break;
			} // end swich()
		} // if bDelim 
		else if(bDelim && g_sSettings.GetBlockMCMA()){

			strTmp = strTmp.Mid(1);
			crUse = crText;
		}
 		else{ 

			crUse = crText;
		}  // end if(bDelim)

		g_eEmoticons.FormatText(&m_rChatDoc, 
									strTmp, 
									TRUE, 
									g_sSettings.GetMessageBold(),
									g_sSettings.GetMessageItalic(),
									g_sSettings.GetMessageLine(),
									crUse,
									bUseBg ? crBg : g_sSettings.GetRGBBg(),
									bName
								);
	}	// while(token.Next(strTmp))
	if(bNewLine)
		m_rChatDoc.Add(retNewline, NEWLINE_FORMAT);

	m_rChat.InvalidateIfModified();
}

void CChatView::WriteSystemMsg(BOOL bIsError, CString strMsg, COLORREF crText, COLORREF crBg , BOOL bUseBg)
{

	strMsg.Replace("\r\n", "\n");

	CRichElement *pEl = 0;

	//WriteTime(crText);

	pEl = m_rSysDoc.Add(retText, "[", NULL, g_sSettings.GetDefaultStyle());
	pEl->m_cBgColor = bUseBg ? crBg : g_sSettings.GetRGBBg();
	pEl->m_cColor   = g_sSettings.GetRGBTime();

	pEl = m_rSysDoc.Add(retText, Util::GetMyLocalTime(), NULL, g_sSettings.GetDefaultStyle());
	pEl->m_cBgColor = bUseBg ? crBg : g_sSettings.GetRGBBg();
	pEl->m_cColor   = crText;

	pEl = m_rSysDoc.Add(retText, "] ", NULL, g_sSettings.GetDefaultStyle());
	pEl->m_cBgColor = bUseBg ? crBg : g_sSettings.GetRGBBg();
	pEl->m_cColor   = g_sSettings.GetRGBTime();

	pEl = m_rSysDoc.Add(retText, strMsg, NULL, g_sSettings.GetDefaultStyle());
	pEl->m_cBgColor = bUseBg ? crBg : g_sSettings.GetRGBBg();
	pEl->m_cColor   = crText;

	m_rSysDoc.Add(retNewline, NEWLINE_FORMAT);
	m_rSys.InvalidateIfModified();

	if(bIsError){

		if(g_sSettings.GetSoundFX()){

			if(!g_sSettings.GetSfxError().IsEmpty())
				PlaySound(g_sSettings.GetSfxError(), NULL, SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
		}
		m_bHideSystem = FALSE;
		if(!m_mxClient.m_bListen){
			
			m_bHasJoined  = FALSE;
			m_aUsers.RemoveAll();
			m_lcUsers.DeleteAllItems();
		}
		ReCalcLayout();
	}
}


void CChatView::WriteSystemMsg(BOOL bIsError, UINT nID, COLORREF rColor)
{

	CString strMsg;
	strMsg.LoadString(nID);
	WriteSystemMsg(bIsError, strMsg, rColor);
}

void CChatView::RemoveUser(const CString strUser, const CString strIP, WORD wPort)
{

	int nPos = m_lcUsers.SafeFind(strUser);

	if(nPos >= 0){

		m_lcUsers.DeleteItem(nPos);
		m_lcUsers.Sort();
	}
}

void CChatView::AddUser(CString strUsername, WORD wLine, DWORD dwFiles, DWORD dwNodeIP, WORD wNodePort, CString strIP, CString strHost, WORD wUserLevel)
{

	CString strFiles, strLine, strPort, strNodeIP;
	strFiles.Format("%d", dwFiles);
	strLine = Util::FormatLine(wLine);
	strPort.Format("%d", wNodePort);
	strNodeIP = Util::FormatIP(dwNodeIP);

	if(g_sSettings.GetDisplayNode()){

		m_lcUsers.AddItem(wUserLevel, strUsername, (LPCTSTR)strFiles, (LPCTSTR)strLine, (LPCTSTR)strIP, (LPCTSTR)strHost, (LPCTSTR)strNodeIP, (LPCTSTR)strPort);
	}
	else{

		m_lcUsers.AddItem(wUserLevel, strUsername, (LPCTSTR)strFiles, (LPCTSTR)strLine, (LPCTSTR)strIP, (LPCTSTR)strHost);
	}
	m_lcUsers.Sort();
}


LRESULT CChatView::OnRoomRename(WPARAM wParam, LPARAM lParam)
{

	CString strRoom = m_mxClient.m_strRoom;
	CString strOldRoom = GetDocument()->m_strRoom;
	CString strOut;

	GetDocument()->SetTitle(strRoom, TRUE);
	GetDocument()->m_strRoom = strRoom;
	GetApp()->GetMainFrame()->m_wndDocSelector.UpdateTitle(this, strRoom);

	strOut.Format(IDS_ROOMRENAME, strOldRoom, GetDocument()->m_strRoom);
	
	WriteSystemMsg(FALSE, strOut, g_sSettings.GetRGBOK());
	Log("System:", strOut);

	for(int i = 0; i < g_aPlugins.GetSize(); i++){

		if(PLUGIN_VER >= 0x1001L){

			PLUGIN->OnRenameChannel((DWORD)m_hWnd, GetDocument()->m_strRoom);
		}
	}
	
	return 1;
}

LRESULT CChatView::OnTopic(WPARAM wParam, LPARAM lParam)
{

	CString strTopic = (LPCSTR)lParam;

	GetDocument()->m_strTopic = strTopic;

	if(g_sSettings.GetPrintTime()){

		WriteTime(g_sSettings.GetRGBTopic());
	}

	WriteText(strTopic, g_sSettings.GetRGBTopic(), g_sSettings.GetRGBBg(), TRUE, TRUE);

	Log("Topic:", strTopic);

	if(g_sSettings.GetSoundFX() && m_bHasJoined){

		PlaySound(g_sSettings.GetSfxTopic(), NULL, SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
	}
	return 1;
}

LRESULT CChatView::OnMotd(WPARAM wParam, LPARAM lParam)
{

	CString strMotd = (LPCSTR)lParam;

	strMotd.Replace("\n", " \n");
	CTokenizer token(strMotd, "\n");
	CString strOut;

	BOOL bDelim = FALSE;
	while(token.Next(strOut)){

		if(g_sSettings.GetPrintTime()){

			WriteTime(g_sSettings.GetRGBMotd());
		}

		WriteText(strOut, g_sSettings.GetRGBMotd(), g_sSettings.GetRGBBg(), TRUE, TRUE);
		Log("", strOut);
	}
	if(g_sSettings.GetSoundFX() && m_bHasJoined){

		if(!g_sSettings.GetSfxMotd().IsEmpty())
			PlaySound(g_sSettings.GetSfxMotd(), NULL, SND_FILENAME|SND_ASYNC|SND_NODEFAULT);
	}
	return 1;
}

LRESULT CChatView::OnAddUser(WPARAM wParam, LPARAM lParam)
{

	ClientJoin* pUser = (ClientJoin*)lParam;
	if(pUser <= 0) return 0;

	MX_USERINFO user;

	user.strUser	= pUser->strName;
	user.dwNodeIP	= pUser->dwNodeIP;
	user.wNodePort	= pUser->wNodePort;
	user.wLineType	= pUser->wLine;
	user.dwNumFiles = pUser->dwFiles;
	user.wUserLever = pUser->wLevel;
	
	if((m_aUsers.GetSize() == 0) && (Util::FormatIP(user.dwNodeIP) == m_mxClient.m_strRoomIP)){

		user.wUserLever = USER_HOST;
	}
	user.strNodeIP = Util::FormatIP(user.dwNodeIP);
	user.dwStayTime = GetTickCount();
	user.dwIdleTime = GetTickCount();
	user.cJoinTime  = CTime::GetCurrentTime();
	user.dwNumMsg   = 0;

	//m_aUsers.Add(user);
	m_aUsers.SetAt(user.strUser, user);
	m_aUserNoID.SetAt(Util::GetBaseName(user.strUser, TRUE, TRUE), user.strUser);
	m_aUserHalfID.SetAt(Util::GetBaseName(user.strUser, TRUE), user.strUser);

	if(m_nServerType >= SERVER_WINMX353){

		CString strColorName = CChatView::MakeColorName(user.strUser, m_mxClient.m_strColorString);//, m_nServerType);
		m_aMCMAMap.SetAt(strColorName, user.strUser);
	}
	
	AddUser(user.strUser, user.wLineType, user.dwNumFiles, user.dwNodeIP, user.wNodePort, user.strRealIP, user.strHostname, user.wUserLever);

	return 1;
}

LRESULT CChatView::OnJoin(WPARAM wParam, LPARAM lParam)
{

	ClientJoin* pJoin = (ClientJoin*)lParam;

	MX_USERINFO user;
	user.strUser		= pJoin->strName;
	user.strNodeIP		= Util::FormatIP(pJoin->dwNodeIP);
	user.dwNodeIP		= pJoin->dwNodeIP;
	user.strRealIP		= Util::FormatIP(pJoin->dwSrcIP);
	user.wLineType		= pJoin->wLine;
	user.wNodePort		= pJoin->wNodePort;
	user.wUserLever		= pJoin->wLevel;
	user.dwNumFiles		= pJoin->dwFiles;
	user.strHostname	= pJoin->strHost;
	user.dwStayTime		= GetTickCount();
	user.dwIdleTime		= GetTickCount();
	user.cJoinTime		= CTime::GetCurrentTime();
	user.dwNumMsg		= 0;

	//m_aUsers.Add(user);
	m_aUsers.SetAt(user.strUser, user);
	m_aUserNoID.SetAt(Util::GetBaseName(user.strUser, TRUE, TRUE), user.strUser);
	m_aUserHalfID.SetAt(Util::GetBaseName(user.strUser, TRUE), user.strUser);
	
	if(m_nServerType >= SERVER_WINMX353){

		CString strColorName = CChatView::MakeColorName(user.strUser, m_mxClient.m_strColorString); //, m_nServerType);
		m_aMCMAMap.SetAt(strColorName, user.strUser);
	}
	AddUser(pJoin->strName, user.wLineType, user.dwNumFiles, user.dwNodeIP, user.wNodePort, user.strRealIP, user.strHostname, user.wUserLever);

	CString strJoin;

	if(g_sSettings.GetPrintTime()){

		WriteTime(g_sSettings.GetRGBJoin());
	}

	strJoin = g_sSettings.GetJoin();
	strJoin.Replace("%LINE%", Util::FormatLine(user.wLineType));
	strJoin.Replace("%FILES%", Util::Formatint(user.dwNumFiles));
	strJoin.Replace("%IP%", user.strRealIP);
	strJoin.Replace("%IPBRACKETS%", user.strRealIP.IsEmpty() ? "" : "(" + user.strRealIP + ")");
	strJoin.Replace("%HOSTNAME%", user.strHostname);
	strJoin.Replace("%HOSTNAMEBRACKETS%", user.strHostname.IsEmpty() ? "" : "(" + user.strHostname + ")");
	strJoin.Replace("%USERS%", Util::Formatint(m_lcUsers.GetItemCount()));
	strJoin.Replace("%ROOMNAME%", GetDocument()->m_strRoomShort);
	Util::ReplaceVars(strJoin);


	if(strJoin.Find("%RAWNAME%", 0) >= 0){

		int n = strJoin.Find("%RAWNAME%", 0);
		CString strPrefix, strSuffix;
		strPrefix = strJoin.Left(n);
		strSuffix = strJoin.Mid(n+9);
		WriteText(strPrefix, g_sSettings.GetRGBJoin(), g_sSettings.GetRGBBg(), TRUE);
		WriteText(pJoin->strName, g_sSettings.GetRGBJoin(), g_sSettings.GetRGBBg(), TRUE, FALSE, TRUE);
		WriteText(strSuffix, g_sSettings.GetRGBJoin(), g_sSettings.GetRGBBg(), TRUE, TRUE);

    }
	else if(strJoin.Find("%NAME%", 0) >= 0){

		int n = strJoin.Find("%NAME%", 0);
		CString strPrefix, strSuffix;
		strPrefix = strJoin.Left(n);
		strSuffix = strJoin.Mid(n+6);
		WriteText(strPrefix, g_sSettings.GetRGBJoin(), g_sSettings.GetRGBBg(), TRUE);
		WriteText(Util::GetBaseName(pJoin->strName, TRUE), g_sSettings.GetRGBJoin(), g_sSettings.GetRGBBg(), TRUE, FALSE, TRUE);
		WriteText(strSuffix, g_sSettings.GetRGBJoin(), g_sSettings.GetRGBBg(), TRUE, TRUE);

    }
	else{

		WriteText(strJoin, g_sSettings.GetRGBJoin(), g_sSettings.GetRGBBg(), TRUE, TRUE);
	}

	strJoin.Replace("%NAME%", Util::GetBaseName(pJoin->strName, TRUE));
	strJoin.Replace("%RAWNAME%", pJoin->strName);

	Log(strJoin, "");

	if(!m_bHasJoined){

		if(Util::GetBaseName(pJoin->strName, TRUE) == Util::GetBaseName(GetDocument()->m_strName)){

			for(int i = 0; i < g_aPlugins.GetSize(); i++){

				PLUGIN->OnJoinChannel((DWORD)m_hWnd, GetDocument()->m_strRoom, &m_aUsers, m_mxClient.m_strServerString, m_mxClient.m_strServerVer);
				if(PLUGIN_VER >= 0x1002L){

					PLUGIN->OnSetUserName(GetDocument()->m_strName);
				}
			}

			m_bHasJoined = TRUE;
			m_bHideSystem = g_sSettings.GetHideSystem();
			InputWelcome();
			ReCalcLayout();
		}
	}

	for(int i = 0; i < g_aPlugins.GetSize(); i++){

		PLUGIN->OnJoin((DWORD)m_hWnd, &user);
	}
	OnUpdate(this, 0, 0);
	return 1;
}

LRESULT CChatView::OnRenameMsg(WPARAM wParam, LPARAM lParam)
{

	ClientJoin* pUser = (ClientJoin*)lParam;
	if(pUser <= 0) return 0;

	m_lcUsers.Sort();

	int nPos = m_lcUsers.SafeFind(pUser->strOldName);

	if(GetDocument()->m_strName.Find(pUser->strOldName, 0) == 0){

		GetDocument()->m_wUserMode = pUser->wLevel;
	}

	MX_USERINFO oldUser, newUser;
	
	if(nPos >= 0){

		// Find user...
		
		if(m_aUsers.Lookup(pUser->strOldName, oldUser) == FALSE){

			CString strMsg;
			strMsg.Format(IDS_FIXME_NAME_ERROR, pUser->strOldName);
			WriteSystemMsg(TRUE, strMsg, g_sSettings.GetRGBErr());
			return 0;
		}

		newUser = oldUser;
		newUser.strUser		= pUser->strName;
		newUser.wLineType	= pUser->wLine;
		newUser.wNodePort	= pUser->wNodePort;
		newUser.dwNumFiles	= pUser->dwFiles;
		newUser.strNodeIP	= Util::FormatIP(pUser->dwNodeIP);
		newUser.dwNodeIP    = pUser->dwNodeIP;
		newUser.wUserLever	= pUser->wLevel;
		newUser.strRealIP	= oldUser.strRealIP;
		newUser.strHostname = oldUser.strHostname;
		if(oldUser.wUserLever == USER_HOST)
		{
			newUser.wUserLever = USER_HOST;
		}
		
		m_aUsers.RemoveKey(oldUser.strUser);
		m_aUserNoID.RemoveKey(Util::GetBaseName(oldUser.strUser, TRUE, TRUE));
		m_aUserHalfID.RemoveKey(Util::GetBaseName(oldUser.strUser, TRUE));

		m_lcUsers.DeleteItem(nPos);
		m_lcUsers.Sort();

		m_aUsers.SetAt(newUser.strUser, newUser);
		m_aUserNoID.SetAt(Util::GetBaseName(newUser.strUser, TRUE, TRUE), newUser.strUser);
		m_aUserHalfID.SetAt(Util::GetBaseName(newUser.strUser, TRUE), newUser.strUser);

		if(m_nServerType >= SERVER_WINMX353){

			RemoveColorName(oldUser.strUser, m_mxClient.m_strColorString);
			CString strColorName = CChatView::MakeColorName(newUser.strUser, m_mxClient.m_strColorString); //, m_nServerType);
			m_aMCMAMap.SetAt(strColorName, newUser.strUser);
		}

		AddUser(newUser.strUser, newUser.wLineType, newUser.dwNumFiles, newUser.dwNodeIP, newUser.wNodePort, newUser.strRealIP, newUser.strHostname, newUser.wUserLever); 
		m_lcUsers.Sort();																																		   
	}
	else{

		CString strErr;
		strErr.Format(IDS_FIXME_NAME_ERROR, pUser->strOldName, nPos);
		WriteSystemMsg(TRUE, strErr, g_sSettings.GetRGBErr());
		return 0;
	}
	for(int i = 0; i < g_aPlugins.GetSize(); i++){

		PLUGIN->OnRename((DWORD)m_hWnd, &oldUser, &newUser);
	}

	OnUpdate(this, 0, 0);
	return 1;
}

LRESULT CChatView::OnIPClient(WPARAM wParam, LPARAM lParam)
{

	ClientJoin* pIP = (ClientJoin*)lParam;
	ASSERT(pIP);

	int nPos = m_lcUsers.SafeFind(pIP->strName);

	MX_USERINFO user, old;

	if(nPos >= 0){

		// Find user...
		if(m_aUsers.Lookup(pIP->strName, old) == FALSE){

			return 0;
		}

		user = old;

		user.strUser	 = pIP->strName;
		user.wLineType	 = pIP->wLine;
		user.wNodePort	 = pIP->wNodePort;
		user.dwNumFiles	 = pIP->dwFiles;
		user.strNodeIP	 = Util::Formatint(pIP->dwNodeIP);
		user.dwNodeIP    = pIP->dwNodeIP;
		user.wUserLever	 = pIP->wLevel;
		user.strRealIP	 = Util::FormatIP(pIP->dwSrcIP);
		user.strHostname = pIP->strHost;

		if(old.wUserLever == USER_HOST){

			user.wUserLever = USER_HOST;
		}

		m_aUsers.RemoveKey(old.strUser);
		m_aUserNoID.RemoveKey(Util::GetBaseName(old.strUser, TRUE, TRUE));
		m_aUserHalfID.RemoveKey(Util::GetBaseName(old.strUser, TRUE));

		m_aUsers.SetAt(user.strUser, user);
		m_aUserNoID.SetAt(Util::GetBaseName(user.strUser, TRUE, TRUE), user.strUser);
		m_aUserHalfID.SetAt(Util::GetBaseName(user.strUser, TRUE), user.strUser);

		m_lcUsers.DeleteItem(nPos);
		m_lcUsers.Sort();
		AddUser(user.strUser, user.wLineType, user.dwNumFiles, user.dwNodeIP, user.wNodePort, user.strRealIP, user.strHostname, user.wUserLever); 
		m_lcUsers.Sort();																																		   
	}   
	else{

		return 0;
	}
	return 1;
}

LRESULT CChatView::OnOpMessage(WPARAM wParam, LPARAM lParam)
{

	CString strMsg = (char*)lParam;

	if(strMsg == "") return 0;  // MCMA sends sometimes this garbage package which creates
	                              // idiotic empty lines, so filter this out

	WORD  wType		= LOWORD(wParam);
	WORD  wLen		= HIWORD(wParam);

	if(wType == 0x00D3){	// command echo

		if(strMsg.IsEmpty()) return 0;
		if(g_sSettings.GetPrintTime()){

			WriteTime(g_sSettings.GetRGBOp());
		}
		WriteText(g_sSettings.GetBrMsgEnd() + " ", g_sSettings.GetRGBOp(), g_sSettings.GetRGBBg(), TRUE);
		WriteText(strMsg, g_sSettings.GetRGBNormalMsg(), g_sSettings.GetRGBBg(), TRUE, TRUE);
		Log(":", strMsg);
		HandleHiLite();
		CString strTmp;
		for(int i = 0; i < g_aPlugins.GetSize(); i++){

			PLUGIN->OnOpMessage((DWORD)m_hWnd, &strTmp, &strMsg);
		}
	}
	else if(wType == 0x00D2){ // opmessages or colored messages

		CString strName, strNoColorName, strRealName, strTmp;
		BOOL bOK = FALSE;
		int nIndex = strMsg.Find(" ");
		if(nIndex > 4){ // usually at least 4 chars long ;)

			// this is the name as found in the message
            strName = strMsg.Left(nIndex);
			bOK = ((strName.Find("<") != -1) && (strName.Find(">") != -1));

			// this is the name with binary color codes removed
			strNoColorName = RemoveColorCodes(strName);
			// now we try to lookup the realname
			// the MCMA Map contains the name with all color codes
			// removed and maps it to the real RAWNAME
			POSITION pos = m_aMCMAMap.GetStartPosition();
			while(pos){


				m_aMCMAMap.GetNextAssoc(pos, strTmp, strRealName);
				if((strNoColorName.Find(strRealName) >= 0) || (strRealName.Find(strNoColorName) >= 0)){

					break;
				}
				else{
					
					strRealName.Empty();
				}
			}
			if(strRealName.IsEmpty()){

				strRealName = strName;
			}

			strMsg  = strMsg.Mid(nIndex);
			strMsg.TrimLeft();

			if(strMsg.Find(m_strAvLagBuff) >= 0){

				UpdateAverageLag(FALSE);
			}
		
			for(int i = 0; i < g_aPlugins.GetSize(); i++){

				PLUGIN->OnOpMessage((DWORD)m_hWnd, &strRealName, &strMsg);
			}

			if(strMsg.IsEmpty()) return 0;

			// is the user ignored?
			for(int i = 0; i < g_sSettings.m_aIgnored.GetSize(); i++){

				if(Util::WildcardMatch(strRealName, g_sSettings.m_aIgnored[i]))
					return 1;
			}

			if(g_sSettings.GetPrintTime()){

				WriteTime(g_sSettings.GetRGBOp());
			}

			if(g_sSettings.GetBlockMCMA() && g_sSettings.GetMCMARawname()){

				WriteText(g_sSettings.GetBrMsgFront(), g_sSettings.GetRGBBrMessage(), g_sSettings.GetRGBBg(), TRUE);
				WriteText(strRealName, g_sSettings.GetRGBNormalName(), g_sSettings.GetRGBBg(), TRUE, FALSE, TRUE);
				WriteText(g_sSettings.GetBrMsgEnd() + " ", g_sSettings.GetRGBBrMessage(), g_sSettings.GetRGBBg(), TRUE);
				WriteText(strMsg, g_sSettings.GetRGBNormalMsg(), g_sSettings.GetRGBBg(), TRUE, TRUE);
			}
			else{

				WriteText(strName, g_sSettings.GetRGBOp(), g_sSettings.GetRGBBg(), TRUE, FALSE, TRUE);
				WriteText(" " + strMsg, bOK? g_sSettings.GetRGBNormalMsg() : g_sSettings.GetRGBOp(), g_sSettings.GetRGBBg(), TRUE, TRUE);
			}
			HandleHiLite();
			Log(strRealName, strMsg);
			UpdateUserStats(strRealName, strMsg);

		}
		else{ // not a real name

			if(strMsg.IsEmpty()) return 0;
			
			strTmp.Empty();

			if(g_sSettings.GetPrintTime()){

				WriteTime(g_sSettings.GetRGBOp());
			}

			WriteText(strMsg, 
				(m_nServerType >= SERVER_WINMX353 ? g_sSettings.GetRGBActionMsg() : g_sSettings.GetRGBOp()), 
				g_sSettings.GetRGBBg(), TRUE, TRUE);
			Log(strMsg, "");
			HandleHiLite();
			for(int i = 0; i < g_aPlugins.GetSize(); i++){

				PLUGIN->OnOpMessage((DWORD)m_hWnd, &strTmp, &strMsg);
			}
		}
	}

	return 1;
}

LRESULT CChatView::OnPart(WPARAM wParam, LPARAM lParam)
{

	ClientJoin* pUser = (ClientJoin*)lParam;

	CString strIP   = Util::FormatIP(pUser->dwNodeIP);

	MX_USERINFO user;

	if(m_aUsers.Lookup(pUser->strName, user)){

		m_aUsers.RemoveKey(user.strUser);
		m_aUserNoID.RemoveKey(Util::GetBaseName(user.strUser, TRUE, TRUE));
		m_aUserHalfID.RemoveKey(Util::GetBaseName(user.strUser, TRUE));
		RemoveUser(pUser->strName, strIP, pUser->wNodePort);
		if(m_nServerType >= SERVER_WINMX353){

			RemoveColorName(user.strUser, m_mxClient.m_strColorString);
		}
	}
	else{

		user.strUser	= pUser->strName;
		user.dwNodeIP	= pUser->dwNodeIP;
		user.wNodePort	= pUser->wNodePort;
		user.dwStayTime = GetTickCount();
		user.wLineType  = 0;
		user.dwNumFiles = 0;
		user.wUserLever = 0;
	}

	CString strPart, strTime;
	if(g_sSettings.GetPrintTime()){

		WriteTime(g_sSettings.GetRGBPart());
	}
	
	strPart = g_sSettings.GetPart();
	strPart.Replace("%NAME%", Util::GetBaseName(user.strUser, TRUE));
	strPart.Replace("%RAWNAME%", user.strUser);
	strPart.Replace("%ROOMNAME%", GetDocument()->m_strRoomShort);
	strPart.Replace("%USERS%", Util::Formatint(m_lcUsers.GetItemCount()));
	strPart.Replace("%STAYTIME%", Util::GetStayTime(user.dwStayTime));
	strPart.Replace("%LINE%", Util::FormatLine(user.wLineType));
	strPart.Replace("%FILES%", Util::Formatint(user.dwNumFiles));
	strPart.Replace("%IP%", user.strRealIP);
	strPart.Replace("%IPBRACKETS%", user.strRealIP.IsEmpty() ? "" : "(" + user.strRealIP + ")");
	strPart.Replace("%HOSTNAME%", user.strHostname);
	strPart.Replace("%HOSTNAMEBRACKETS%", user.strHostname.IsEmpty() ? "" : "(" + user.strHostname + ")");
	Util::ReplaceVars(strPart);

	WriteText(strPart, g_sSettings.GetRGBPart(), g_sSettings.GetRGBBg(), TRUE, TRUE);
	Log(strPart, "");
	for(int i = 0; i < g_aPlugins.GetSize(); i++){

		PLUGIN->OnPart((DWORD)m_hWnd, &user);
	}

	return 1;
}

LRESULT CChatView::OnMessage(WPARAM wParam, LPARAM lParam)
{

	CString strName = (LPSTR)wParam, strMsg = (LPSTR)lParam;

	if(strMsg.IsEmpty() && strName.IsEmpty()) return 0;
	
	if(m_strAvLagBuff == strMsg){

		UpdateAverageLag(FALSE);
	}

	m_dwMessages++;
	for(int i = 0; i < g_sSettings.m_aIgnored.GetSize(); i++){

		if(Util::WildcardMatch(strName, g_sSettings.m_aIgnored[i]))
			return 1;
	}

	for(i = 0; i < g_aPlugins.GetSize(); i++){

		PLUGIN->OnMessage((DWORD)m_hWnd, &strName, &strMsg, FALSE);
	}
	if(strMsg.IsEmpty()) return 1;

	// Update user statistics
	UpdateUserStats(strName, strMsg);

	if(g_sSettings.GetPrintTime()){

		WriteTime(g_sSettings.GetRGBNormalName());
	}
	
	WriteText(g_sSettings.GetBrMsgFront(), g_sSettings.GetRGBBrMessage(), g_sSettings.GetRGBBg(), TRUE);
	WriteText(strName, g_sSettings.GetRGBNormalName(), g_sSettings.GetRGBBg(), TRUE, FALSE, TRUE);
	WriteText(g_sSettings.GetBrMsgEnd() + " ", g_sSettings.GetRGBBrMessage(), g_sSettings.GetRGBBg(), TRUE);
	WriteText(strMsg, g_sSettings.GetRGBNormalMsg(), g_sSettings.GetRGBBg(), TRUE, TRUE);
	HandleHiLite();

#ifdef _ROBO_READER
	Speak(strName, strMsg, FALSE);
#endif

	Log(g_sSettings.GetBrMsgFront() +  strName + g_sSettings.GetBrMsgEnd(), strMsg);

	HandleSounds(strMsg);

	return 1;
}

void CChatView::HandleSounds(CString strMsg)
{
	

	if(g_sSettings.GetSfxChatSfx() && 
		 (GetApp()->GetMainFrame()->GetActiveFrame() == GetParentFrame())){
		
		if(strMsg.Find(COLOR_DELIM) >= 0){

			CString strColor;
			for(int i = 0; i < num_color; i++){
				
                strColor.Format("%c%c", COLOR_DELIM, color_map[i]);
				strMsg.Replace(strColor, "");
			}
		}
		CString strNoCase = strMsg;
		strNoCase.MakeLower();
		for(int i = 0; i < g_sSettings.m_aSounds.GetSize(); i++){

			if(Util::WildcardMatch(strNoCase, g_sSettings.m_aSounds[i].strTrigger, TRUE)){

				PlaySound(g_sSettings.m_aSounds[i].strSound, NULL, SND_FILENAME|SND_ASYNC);
				break;
			}
		}
	}
}

LRESULT CChatView::OnAction(WPARAM wParam, LPARAM lParam)
{

	CString strName = (LPSTR)wParam, strMsg = (LPSTR)lParam;
	
	if(strMsg.IsEmpty() && strName.IsEmpty()) return 0;

	if(m_strAvLagBuff == strMsg){

		UpdateAverageLag(FALSE);
	}
	
	m_dwMessages++;
	for(int i = 0; i < g_sSettings.m_aIgnored.GetSize(); i++){

		if(Util::WildcardMatch(strName, g_sSettings.m_aIgnored[i]))
			return 1;
	}

	for(i = 0; i < g_aPlugins.GetSize(); i++){

		PLUGIN->OnMessage((DWORD)m_hWnd, &strName, &strMsg, TRUE);
	}
	
	// Update user statistics
	UpdateUserStats(strName, strMsg);

	if(g_sSettings.GetPrintTime()){

		WriteTime(g_sSettings.GetRGBActionMsg());
	}

	WriteText(g_sSettings.GetBrActionFront(), g_sSettings.GetRGBBrAction(), g_sSettings.GetRGBBg(), TRUE);
	WriteText(strName, (g_sSettings.GetColorAcName() ? g_sSettings.GetRGBNormalName() : g_sSettings.GetRGBActionMsg()), g_sSettings.GetRGBBg(), TRUE, FALSE, TRUE);
	WriteText(g_sSettings.GetBrActionEnd() + " ", g_sSettings.GetRGBBrAction(), g_sSettings.GetRGBBg(), TRUE);
	WriteText(strMsg, g_sSettings.GetRGBActionMsg(), g_sSettings.GetRGBBg(), TRUE, TRUE);

	Log(g_sSettings.GetBrActionFront() +  strName + g_sSettings.GetBrActionEnd(), strMsg);

	HandleHiLite();

#ifdef _ROBO_READER
	Speak(strName, strMsg, TRUE);
#endif

	HandleSounds(strMsg);
	return 1;
}

LRESULT CChatView::OnUnknown(WPARAM wParam, LPARAM lParam)
{
	
	WORD wType = LOWORD(wParam);
	WORD wLen  = HIWORD(wParam);
	char* pCmd = (char*)lParam;

	CString strCmd, strOut;
	unsigned char c;
	for(int i = 0; i < wLen; i++)
	{

		c = (unsigned char)pCmd[i];
		if(c == '\0') strCmd+="0";
		else strCmd.Insert(i, c);
	}
	strOut.Format(IDS_FIXME_UNKNOWN_COMMAND, wType, wLen, strCmd);
	WriteSystemMsg(FALSE, strOut);

	return 1;
}

void CChatView::Input(CString strText)
{

	//HandleCustomCmds(m_strInput);	
	//Util::ReplaceVars(m_strInput);
	HandleCustomCmds(strText);	
	Util::ReplaceVars(strText);
	strText.Replace("%MYNAME%", Util::GetBaseName(GetDocument()->m_strName, TRUE, FALSE));
	strText.Replace("%MYRAWNAME%", GetDocument()->m_strName);
	strText.Replace("%ROOMNAME%", GetDocument()->m_strRoomShort);
	strText.Replace("%ROOMRAWNAME%", GetDocument()->m_strRoom);
	strText.Replace("%SERVER%", m_mxClient.m_strServerString);
	strText.Replace("%SERVERVER%", m_mxClient.m_strServerVer);
	strText.Replace("%AVERAGELAG%", Util::Formatint(m_dwAvLag));
	strText.Replace("%USERS%", Util::Formatint(m_aUsers.GetSize()));
	
	for(int i = 0; i < g_aPlugins.GetSize(); i++){

		PLUGIN->OnInputHook((DWORD)m_hWnd, &strText);
	}

	m_mxClient.SendText(strText);
}


BOOL CChatView::HandleCustomCmds(CString& rString)
{

	if(rString.Find("/nick ") == 0){

		CString strNick = rString.Mid(6);
		Util::MakeValidUserName(strNick, FALSE);
		GetDocument()->m_strName = strNick;
		m_mxClient.SendRename(GetDocument()->m_strName, GetDocument()->m_dwFiles, GetDocument()->m_wLine);
		if(m_nServerType == SERVER_WINMX353){

			m_mxClient.SendRename(GetDocument()->m_strName, GetDocument()->m_dwFiles, GetDocument()->m_wLine);
		}
		rString.Empty();
		return TRUE; // dont handle this any further ... bla
	}
	else if(rString.Find("/walk") == 0){

		rString = rString.Mid(5);
		rString.Insert(0, "/me walks");
	}
	else if(rString.Find("/sleep") == 0){

		rString = rString.Mid(6);
		rString.Insert(0, "/me sleeps");
	}
	else if(rString.Find("/cry") == 0){

		rString = rString.Mid(4);
		rString.Insert(0, "/me cries");
	}
	else if(rString.Find("/shout") == 0){

		rString = rString.Mid(6);
		rString.Insert(0, "/me shouts");
	}
	else if(rString.Find("/steal") == 0){

		rString = rString.Mid(6);
		rString.Insert(0, "/me steals");
	}

	return FALSE;
}


LRESULT CChatView::OnInput(WPARAM wParam, LPARAM lParam)
{

	if(wParam == 1){

		m_strInput = (LPCTSTR)lParam;
	}
	else{

		UpdateData(TRUE);
	}

	m_pgLetters.SetPos(0);

	if(m_strInput.GetLength() > 400){

		//m_strInput = m_strInput.Left(400);
		m_strInput.Truncate(400);
	}
	
	BOOL bAction = FALSE;

	if(HandleCustomCmds(m_strInput)){

		// Command is not to be send to server!
		// abort ;)
		UpdateData(FALSE);
		return 1;
	}

	if((CMyEdit::SearchHistory(m_strInput) == 0) && g_sSettings.GetSaveHistory()){

		if((g_aHistory.GetSize() > g_sSettings.GetHistoryDepth()) && g_aHistory.GetSize()){

            g_aHistory.RemoveAt(0);
		}
		g_aHistory.Add(m_strInput);	
	}

	if(m_strInput.Find("/all ", 0) == 0){

		m_strInput = m_strInput.Mid(5);
		GetApp()->GetMainFrame()->m_wndDocSelector.InputAll(m_strInput);
		m_strInput.Empty();
		UpdateData(FALSE);
		return 1;
	}
	else if(m_strInput == "/clear"){

		m_strInput.Empty();
		UpdateData(FALSE);
		OnChatroomClearscreen();
		return 1;
	}
	
	Util::ReplaceVars(m_strInput);

	for(int i = 0; i < g_aPlugins.GetSize(); i++){

		PLUGIN->OnInputHook((DWORD)m_hWnd, &m_strInput);
	}

	m_strAvLagBuff = m_strInput;
	if(m_strAvLagBuff.Find("/me ", 0) == 0){

		m_strAvLagBuff = m_strAvLagBuff.Mid(4);
	}
	else if(m_strAvLagBuff.Find("/action ", 0) == 0){

		m_strAvLagBuff = m_strAvLagBuff.Mid(8);
	}
	else if(m_strAvLagBuff.Find("/emote ", 0) == 0){

		m_strAvLagBuff = m_strAvLagBuff.Mid(7);
	}

	m_mxClient.SendText(m_strInput);

	UpdateAverageLag();
	m_strInput.Empty();
	UpdateData(FALSE);
	m_dwYourMessages++;

	UpdateData(FALSE);
	OnUpdate(0,0,0);
	return 1;
}


void CChatView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	ReCalcLayout();	
}

void CChatView::OnRename() 
{

	CRenameDlg dlg;
	dlg.m_dwFiles = m_mxClient.m_dwFiles;
	dlg.m_nLine   = m_mxClient.m_wLineType;
	dlg.m_strName = Util::GetBaseName(m_mxClient.m_strUser);

	if(dlg.DoModal() == IDOK){

		GetDocument()->m_dwFiles = dlg.m_dwFiles;
		GetDocument()->m_strName = dlg.m_strName;
		GetDocument()->m_wLine   = dlg.m_nLine;
		GetDocument()->m_bWPN	 = dlg.m_bAllRooms;
		GetDocument()->SetTitle(GetDocument()->m_strRoom, FALSE);

		if(dlg.m_bAllRooms){

			GetApp()->GetMainFrame()->m_mxClient.SendRename(dlg.m_strName, dlg.m_nLine);
			GetApp()->GetMainFrame()->m_wndDocSelector.BroadcastMessage(UWM_RENAMECL, 0, 0);

		}
		else{
			
			m_mxClient.SendRename(dlg.m_strName, dlg.m_dwFiles, dlg.m_nLine);
			if(m_nServerType == SERVER_WINMX353){

				// yes, we are bug per bug compatible
				// winmx 3.53 always sends this twice :D
				m_mxClient.SendRename(dlg.m_strName, dlg.m_dwFiles, dlg.m_nLine);
			}
		}
		for(int i = 0; i < g_aPlugins.GetSize(); i++){

			if(PLUGIN_VER >= 0x1002L){

				PLUGIN->OnSetUserName(GetDocument()->m_strName);
			}
		}
	}
}

LRESULT CChatView::OnSystem(WPARAM wParam, LPARAM lParam)
{

	EXT_MSG_STRUCT *msg = (EXT_MSG_STRUCT*)lParam;

	WriteSystemMsg((BOOL)msg->wParam, msg->lpszMsg, (COLORREF)wParam);

	return 1;
}

LRESULT CChatView::OnRedirect(WPARAM wParam, LPARAM lParam)
{

	m_strTarget = (LPCTSTR)lParam;

	SetTimer(TIMER_REDIRECT, 2000, NULL);

	return 1;
}

void CChatView::OnLeave() 
{

	GetDocument()->OnCloseDocument();
}

void CChatView::ReCalcLayout()
{

	if(!m_rSys.m_hWnd || !m_rChat.m_hWnd || !m_sSplitter1.m_hWnd || !m_sSplitter2.m_hWnd){

		return;
	}

	CRect	rcItem;
	CRect	rcItem2;
	CRect	rcParent;

	GetClientRect(rcParent);
	ShowScrollBar(SB_BOTH , FALSE);
	// Get Button Position
	m_btExit.GetWindowRect(rcItem2);
	ScreenToClient(rcItem2);
	// Get Edit Position
	m_eInput.GetWindowRect(rcItem);
	ScreenToClient(rcItem);

	int nHeight = rcItem.Height();  // Save height edit control
	rcItem.bottom = rcParent.bottom - 1;
	rcItem.top = rcItem.bottom - nHeight;
	rcItem.right = rcParent.right - 6 - rcItem2.Width();

	nHeight = rcItem2.Height(); // save height of exit-button
	int nWidth = rcItem2.Width();
	rcItem2.bottom = rcParent.bottom - 1;
	rcItem2.right  = rcParent.right - 1;
	rcItem2.top    = rcItem2.bottom - nHeight;
	rcItem2.left   = rcItem2.right  - nWidth;

	m_eInput.MoveWindow(rcItem);
	m_btExit.MoveWindow(rcItem2);

	if(m_bShowStatus){

		m_pgLetters.GetWindowRect(rcItem);
		ScreenToClient(rcItem);
	
		rcItem.left   = rcParent.left  + 1;
		rcItem.right  = rcParent.right - 1;
		rcItem.bottom = rcParent.bottom - nHeight;
		rcItem.top    = rcParent.bottom - nHeight - 1 - 10;
		
		m_pgLetters.MoveWindow(rcItem);
	}
	// move userlist
	m_lcUsers.GetWindowRect(rcItem);
	ScreenToClient(rcItem);

	nWidth = rcItem.Width();
	rcItem.right = rcParent.right;
	rcItem.left  = rcParent.right - nWidth;
	rcItem.bottom = rcItem2.top - (m_bShowStatus ? 10 : 2);
	m_lcUsers.MoveWindow(rcItem);

	// Splitter 1 is vertical between userlist and the rest
	// move/size splitter1	
	m_sSplitter1.GetWindowRect(rcItem);
	ScreenToClient(rcItem);
	rcItem.right = rcParent.right - nWidth - 1;
	rcItem.left  = rcParent.right - nWidth - 6;
	rcItem.bottom = rcParent.bottom - nHeight - (m_bShowStatus ? 10 : 2);
	m_sSplitter1.MoveWindow(rcItem);


	int nChatTop = 0;

	if(m_bHideSystem){

		m_rSys.ShowWindow(SW_HIDE);
		m_rSys.GetWindowRect(rcItem);
		ScreenToClient(rcItem);
		nChatTop = rcItem.top;

		m_sSplitter2.ShowWindow(SW_HIDE);

	}
	else{

		m_rSys.ShowWindow(SW_SHOW);
		m_sSplitter2.ShowWindow(SW_SHOW);

		m_rSys.GetWindowRect(rcItem);
		ScreenToClient(rcItem);

		// resize system out
		nChatTop = rcItem.bottom + 5;
		rcItem.right = rcParent.right - 6 - nWidth;
		m_rSys.MoveWindow(rcItem);

		// resize splitter2 is horizontal between sys out and chat
		m_sSplitter2.GetWindowRect(rcItem);
		ScreenToClient(rcItem);

		rcItem.right = rcParent.right - 6 - nWidth;
		m_sSplitter2.MoveWindow(rcItem);
	}

	// resize chat
	m_rChat.GetWindowRect(rcItem);
	ScreenToClient(rcItem);
	rcItem.top    = nChatTop;
	rcItem.right  = rcParent.right - 6 - nWidth;
	rcItem.bottom = rcItem2.top - (m_bShowStatus ? 10 : 2);
	m_rChat.MoveWindow(rcItem);

}

void CChatView::OnCustomizeUserCmds()
{

	ShellExecute(0, "open", g_sSettings.GetWorkingDir() + "\\menu.ini", 0, 0, SW_SHOW);
}

void CChatView::OnChatCmd(UINT nID)
{

	int nCmd = nID - ID_CSTM_MENUBASE2;

	if(nID < 0) return;

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetWorkingDir() + "\\menu.ini");

	CString strItem, strTmp, strCmd;
	strItem.Format("Chat_%d", nCmd);

	for(int i = 0; TRUE; i++){

		strTmp.Format("Cmd_%d", i);
		strCmd = ini.GetValue(strItem, strTmp, "");

		if(strCmd.IsEmpty()) break;
		
		//Util::ReplaceVars(strCmd);

		Input(strCmd);
	}
}

void CChatView::OnUserCmd(UINT nID)
{

	int nCmd = nID - ID_CSTM_MENUBASE;

	if(nID < 0) return;

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);

	CString strName, strRawName;
	if(n >= 0){

		strRawName = m_lcUsers.GetItemText(n, 0);
		strName    = Util::GetBaseName(strRawName);

		CIni ini;
		ini.SetIniFileName(g_sSettings.GetWorkingDir() + "\\menu.ini");

		CString strItem, strTmp, strCmd;
		strItem.Format("User_%d", nCmd);

		for(int i = 0; TRUE; i++){

			strTmp.Format("Cmd_%d", i);
			strCmd = ini.GetValue(strItem, strTmp, "");

			if(strCmd.IsEmpty()) break;

			//Util::ReplaceVars(strCmd);
			strCmd.Replace("%RAWNAME%", strRawName);
			strCmd.Replace("%NAME%", strName);
			Input(strCmd);
		}
	}

	for(int i = 0; i < g_aPlugins.GetSize() && m_bHasJoined; i++){

		PLUGIN->OnMenuCommand(nID, (DWORD)m_hWnd, strRawName);
	}

}

void CChatView::LoadAwayMenu(CMenu& rMenu)
{
	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	int nCnt = ini.GetValue("AwayControl", "AwayCnt", 0);
	CString strLabel, strTmp;

	for(int i = 0; i < nCnt; i++){

		strTmp.Format("Label_%d", i);
		strLabel = ini.GetValue("AwayControl", strTmp, "");
		
		if(strLabel.IsEmpty()) continue;

		rMenu.AppendMenu(MF_STRING, ID_AWAY_BASE+1+i, strLabel);
	}
	if(nCnt){

		rMenu.AppendMenu(MF_SEPARATOR, 0, (LPCTSTR)0);
	}
	rMenu.AppendMenu(MF_STRING, ID_AWAY_BASE, Util::LoadString(IDS_ENTERREASON));
}

void CChatView::LoadUserMenu(CMenu& rMenu, CMenu& rSub, const UINT dwBase)
{

	// append advanced menu
	CIni ini;
	ini.SetIniFileName(g_sSettings.GetWorkingDir() + "\\menu.ini");

	//int nCnt = ini.GetValue(dwBase == ID_CSTM_MENUBASE ? "UserMenu" : "ChatMenu", "Count", 0);
	CString strLabel, strTmp;
	for(int i = 1; i; i++){

		strTmp.Format(dwBase == ID_CSTM_MENUBASE ? "User_%d" : "Chat_%d", i);

		// check if this item is to be displayed with this server
		strLabel = ini.GetValue(strTmp, "Server", "all");
		strLabel.MakeLower();
		
		if((m_nServerType == SERVER_RCMS) && (strLabel.Find("rcms") < 0) && (strLabel != "all")) continue;
		if((m_nServerType == SERVER_UNKNOWN) && (strLabel.Find("winmx331") < 0) && (strLabel != "all")) continue;
		if((m_nServerType == SERVER_WINMX353) && (strLabel.Find("winmx353") < 0) && (strLabel != "all")) continue;
		if((m_nServerType == SERVER_MCS) && (strLabel.Find("mxtools") < 0) && (strLabel != "all")) continue;
		if((m_nServerType == SERVER_WCS) && (strLabel.Find("wcs") < 0) && (strLabel != "all")) continue;
		if((m_nServerType == SERVER_OUKA) && (strLabel.Find("ouka") < 0) && (strLabel != "all")) continue;
		if((m_nServerType == SERVER_ROSE) && (strLabel.Find("rose") < 0) && (strLabel != "all")) continue;
		if((m_nServerType == SERVER_FXSERV) && (strLabel.Find("fxserv") < 0) && (strLabel != "all")) continue;

		strLabel = ini.GetValue(strTmp, "Label", "");

		if(strLabel.IsEmpty()) break;
		
		if(strLabel.CompareNoCase("separator") == 0){

			rSub.InsertMenu(i, MF_SEPARATOR|MF_BYPOSITION, 0, (LPCTSTR)0);
		}
		else{

			rSub.InsertMenu(i, MF_STRING|MF_BYPOSITION, dwBase + i, (LPCTSTR)strLabel);
		}
	}

	rSub.InsertMenu(i++, MF_SEPARATOR|MF_BYPOSITION, 0, (LPCTSTR)0);
	rSub.InsertMenu(i, MF_STRING|MF_BYPOSITION, ID_CUSTOMIZE, Util::LoadString(IDS_CUSTOMIZE));

	if((m_nServerType == SERVER_WINMX353) && (dwBase == ID_CSTM_MENUBASE)){

		// adjust command set for winmx 3.53 rooms
		rMenu.ModifyMenu(ID_USERLIST_REDIRECT, MF_STRING|MF_BYCOMMAND, ID_USERLIST_REDIRECT, Util::LoadString(IDS_KICKBAN_5));
		rMenu.ModifyMenu(ID_USERLIST_PRINTUSERSTAT, MF_STRING|MF_BYCOMMAND, ID_USERLIST_PRINTUSERSTAT, Util::LoadString(IDS_REMOVE_VOICE));
		rMenu.ModifyMenu(ID_USERLIST_PRINTIP, MF_STRING|MF_BYCOMMAND, ID_USERLIST_PRINTIP, Util::LoadString(IDS_GIVE_VOICE));
		rMenu.ModifyMenu(ID_USERLIST_READUSERMESSAGE, MF_STRING|MF_BYCOMMAND, ID_USERLIST_READUSERMESSAGE, Util::LoadString(IDS_DISPLAY_USERLEVEL));
	}
	else if(((m_nServerType == SERVER_ROSE)|| (m_nServerType == SERVER_FXSERV)) && (dwBase == ID_CSTM_MENUBASE)){

		// adjust command set for robomx rooms
		rMenu.ModifyMenu(ID_USERLIST_PRINTUSERSTAT, MF_STRING|MF_BYCOMMAND, ID_USERLIST_PRINTUSERSTAT, Util::LoadString(IDS_GIVE_VOICE));
		rMenu.ModifyMenu(ID_USERLIST_PRINTIP, MF_STRING|MF_BYCOMMAND, ID_USERLIST_PRINTIP, Util::LoadString(IDS_REMOVE_VOICE));
		rMenu.ModifyMenu(ID_USERLIST_READUSERMESSAGE, MF_STRING|MF_BYCOMMAND, ID_USERLIST_READUSERMESSAGE, Util::LoadString(IDS_DISPLAY_USERLEVEL));
	}   
	else if((m_nServerType == SERVER_MCS) && (dwBase == ID_CSTM_MENUBASE)){

		rMenu.DeleteMenu(ID_USERLIST_REDIRECT, MF_STRING|MF_BYCOMMAND);
		rMenu.DeleteMenu(ID_USERLIST_PRINTIP, MF_STRING|MF_BYCOMMAND);
		rMenu.DeleteMenu(ID_USERLIST_READUSERMESSAGE, MF_STRING|MF_BYCOMMAND);
	}

	rMenu.ModifyMenu(ID_USERLIST_CUSTOMCOMMANDS, MF_POPUP|MF_BYCOMMAND, (UINT)rSub.m_hMenu, Util::LoadString(IDS_USERMNU));

}

void CChatView::OnRclickUserlist(NMHDR* pNMHDR, LRESULT* pResult) 
{

	int n = -1;
	if((n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED)) >= 0){

		POINT point;
		GetCursorPos(&point);

		CMenu mContextMenu;
		CMenu mCustom;

		mContextMenu.LoadMenu(IDR_USER);
        mCustom.CreateMenu();

		LoadUserMenu(mContextMenu, mCustom, ID_CSTM_MENUBASE);

		// Add username and seperator on top of menu
		CString strUser = m_lcUsers.GetItemText(n, 0);
		strUser.Replace("&", "&&");
		mContextMenu.GetSubMenu(0)->InsertMenu(0, MF_STRING|MF_BYPOSITION|MF_DISABLED, 0, strUser);
		mContextMenu.GetSubMenu(0)->InsertMenu(1, MF_SEPARATOR|MF_BYPOSITION, 0, (LPCSTR)0);

		for(int i = 0; i < g_aPlugins.GetSize() && m_bHasJoined; i++){

			PLUGIN->OnPrepareMenu((DWORD)m_hWnd, TRUE, mContextMenu.GetSubMenu(0)->m_hMenu);
		}
		
		mContextMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
											point.x,
											point.y,
											AfxGetMainWnd());
	}

	*pResult = 0;
}

void CChatView::OnRclickChat(RVN_ELEMENTEVENT* pNotify, LRESULT *pResult)
{

	CRichElement* pElement = pNotify->pElement;

	CMenu mContextMenu;
	CMenu mAway;
	CMenu mCustom;
	BOOL  bIsUser = FALSE;

	if(pElement != NULL){

		if(pElement->m_nType == retName){

			//////////////////////
			// Load the usermenu
			int n = -1;
			
			CString strName, strRawname;
			
			if(m_nServerType < SERVER_WINMX353){
			
				strName = Util::GetBaseName(pElement->m_sText, TRUE);
			}
			else{
				
				
				POSITION pos = m_aMCMAMap.GetStartPosition();
				while(pos){

					m_aMCMAMap.GetNextAssoc(pos, strName, strRawname);
					if(strName.Find(pElement->m_sText) >= 0){

						// match
						strName = Util::GetBaseName(strRawname, TRUE);
						break;
					}
					else{

						strRawname.Empty();
						strName.Empty();
					}
				}
				if(strRawname.IsEmpty()){

					// fallback in case we didnt find anything in the mcmamap
					strName = Util::GetBaseName(pElement->m_sText, TRUE);
				}
			}
			
			// try locate this user in the userlist
			n = m_lcUsers.SafeSearch(strName, -1, TRUE);
			if((n == -1) && (strName.GetLength() > 3)){

				// last chance of fallback...
				strName = pElement->m_sText;
				strName = strName.Mid(1, strName.GetLength()-2);
				strName = Util::GetBaseName(strName, TRUE);
			}
			n = m_lcUsers.SafeSearch(strName, -1, TRUE);

			if(n >= 0){

				m_lcUsers.SetItemState(n, LVNI_SELECTED, LVNI_SELECTED);
				m_lcUsers.EnsureVisible(n, FALSE);
				mContextMenu.LoadMenu(IDR_USER);
				mCustom.CreateMenu();
				LoadUserMenu(mContextMenu, mCustom, ID_CSTM_MENUBASE);

				// Add username and seperator on top of menu
				CString strUser = m_lcUsers.GetItemText(n, 0);
				strUser.Replace("&", "&&");
				mContextMenu.GetSubMenu(0)->InsertMenu(0, MF_STRING|MF_BYPOSITION|MF_DISABLED, 0, strUser);
				mContextMenu.GetSubMenu(0)->InsertMenu(1, MF_SEPARATOR|MF_BYPOSITION, 0, (LPCSTR)0);
				bIsUser = TRUE;
			}
			else{

				return;
			}
		}
		else if(pElement->m_nType == retLink){

			//////////////////////
			// Load the linkmenu
			mContextMenu.LoadMenu(IDR_URL);
			m_strSelURL = pElement->m_sLink;
		}
		else{

			//////////////////////
			// Load the default
			mContextMenu.LoadMenu(IDR_CHAT);
			
			mCustom.CreateMenu();
			LoadUserMenu(mContextMenu, mCustom, ID_CSTM_MENUBASE2);

			BOOL bAway = GetApp()->GetMainFrame()->m_bAway;
			if(bAway){ //user is away, load back menu

				mAway.LoadMenu(IDR_BACK);
			}
			else{ // user is not away

				//mAway.LoadMenu(IDRAWAY);
				mAway.CreateMenu();
				LoadAwayMenu(mAway);
			}

			CString strRoom = GetDocument()->m_strRoomShort;
		    strRoom.Replace("&", "&&");
			
			if(strRoom.GetLength() > 40) strRoom.Truncate(40);
			mContextMenu.GetSubMenu(0)->InsertMenu(0, MF_STRING|MF_BYPOSITION, 0, strRoom);
			mContextMenu.GetSubMenu(0)->InsertMenu(1, MF_SEPARATOR|MF_BYPOSITION, 0, (LPCTSTR)0);
			mContextMenu.ModifyMenu(ID_CHATROOM_REJOIN, MF_STRING|MF_BYCOMMAND, ID_CHATROOM_REJOIN, Util::LoadString(IDS_REJOIN_ROOM) + GetDocument()->m_strRoomShort);
			mContextMenu.ModifyMenu(ID_CHATROOM_ADDTOAUTOJOIN, MF_STRING|MF_BYCOMMAND, ID_CHATROOM_ADDTOAUTOJOIN, Util::LoadString(IDS_ADD) + GetDocument()->m_strRoomShort + " to Auto-&Join");
			mContextMenu.ModifyMenu(IDM_AWAY, MF_POPUP|MF_BYCOMMAND, bAway ? (UINT)mAway.GetSubMenu(0)->m_hMenu : (UINT)mAway.m_hMenu, Util::LoadString(IDS_AWAY_CONTROL));
		}
	}
	else{

		mContextMenu.LoadMenu(IDR_CHAT);

		mCustom.CreateMenu();
		LoadUserMenu(mContextMenu, mCustom, ID_CSTM_MENUBASE2);

		BOOL bAway = GetApp()->GetMainFrame()->m_bAway;

		if(bAway){ //user is away, load back menu

			mAway.LoadMenu(IDR_BACK);
		}
		else{ // user is not away

			mAway.CreateMenu();
			LoadAwayMenu(mAway);
		}

		CString strRoom = GetDocument()->m_strRoomShort;
		strRoom.Replace("&", "&&");
		
		if(strRoom.GetLength() > 40) strRoom.Truncate(40);
		mContextMenu.GetSubMenu(0)->InsertMenu(0, MF_STRING|MF_BYPOSITION, 0, strRoom);
		mContextMenu.GetSubMenu(0)->InsertMenu(1, MF_SEPARATOR|MF_BYPOSITION, 0, (LPCTSTR)0);
		mContextMenu.ModifyMenu(ID_CHATROOM_REJOIN, MF_STRING|MF_BYCOMMAND, ID_CHATROOM_REJOIN, Util::LoadString(IDS_REJOIN_ROOM) + GetDocument()->m_strRoomShort);
		mContextMenu.ModifyMenu(ID_CHATROOM_ADDTOAUTOJOIN, MF_STRING|MF_BYCOMMAND, ID_CHATROOM_ADDTOAUTOJOIN, Util::LoadString(IDS_ADD) + GetDocument()->m_strRoomShort + " to Auto-&Join");
		mContextMenu.ModifyMenu(IDM_AWAY, MF_POPUP|MF_BYCOMMAND, bAway ? (UINT)mAway.GetSubMenu(0)->m_hMenu : (UINT)mAway.m_hMenu, Util::LoadString(IDS_AWAY_CONTROL));
	}

	for(int i = 0; i < g_aPlugins.GetSize() && m_bHasJoined; i++){

		PLUGIN->OnPrepareMenu((DWORD)m_hWnd, bIsUser, mContextMenu.GetSubMenu(0)->m_hMenu);
	}

	POINT point;
	GetCursorPos(&point);
	mContextMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON,
											point.x,
											point.y,
											AfxGetMainWnd());

}

LRESULT CChatView::OnWPNNotify(WPARAM wParam, LPARAM lParam)
{

	BOOL bReturn = FALSE;

	if((wParam == WPN_CLIENTID) && GetDocument()->m_bWPN){

		WORD wNew = (WORD)lParam;

		Util::MakeValidUserName(GetDocument()->m_strName, TRUE, wNew);

		m_mxClient.m_dwClientIP		= g_sSettings.GetNodeIP();
		m_mxClient.m_wClientUDPPort = (WORD)g_sSettings.GetNodePort();

		m_mxClient.SendRename(GetDocument()->m_strName, GetDocument()->m_dwFiles, GetDocument()->m_wLine);
		GetDocument()->SetTitle(GetDocument()->m_strRoom, FALSE);
		TRACE("Sent name %s to room\n", GetDocument()->m_strName);
		bReturn = TRUE;
	}
	else if(wParam == WPN_WHOIS){

		WHOIS* pWhois = (PWHOIS)lParam;
		for(int i = 0; i < m_aWhoisBuffer.GetSize(); i++){

			if(strcmp(m_aWhoisBuffer[i].lpszParam, pWhois->lpszName) == 0){

				CString strWhois;
				strWhois.Format(IDS_WHOIS_1, pWhois->lpszName);

				WriteText(strWhois, g_sSettings.GetRGBTime(), g_sSettings.GetRGBOp(), TRUE, TRUE, FALSE);

				DWORD dwDays = 0, dwHours = 0, dwMinutes = 0, dwSeconds = 0;
				dwSeconds = pWhois->dwOnline%60;
				dwMinutes = pWhois->dwOnline/60;
				dwDays	  = dwMinutes / (60*24);
				dwHours   = (dwMinutes % (60*24)) / 60;
				dwMinutes = (dwMinutes % (60*24)) % 60;
				strWhois.Format(IDS_WHOIS_2,
								dwDays,
								dwHours,
								dwMinutes, 
								dwSeconds, 
								Util::FormatLine(pWhois->wLine),
								pWhois->dwFiles, 
								pWhois->lpszQueue);

				WriteText(strWhois, g_sSettings.GetRGBOp(), g_sSettings.GetRGBBg(), TRUE, TRUE, FALSE);
				m_aWhoisBuffer.RemoveAt(i);
				bReturn = TRUE;
				break;
			}
		}
	}
	else if(wParam == WPN_WHOIS_ERROR){

		WHOIS* pWhois = (PWHOIS)lParam;
		for(int i = 0; i < m_aWhoisBuffer.GetSize(); i++){

			if(strcmp(m_aWhoisBuffer[i].lpszParam, pWhois->lpszName) == 0){

				CString strWhois;
				strWhois.Format(IDS_WHOIS_ERROR, 	pWhois->lpszName);
				WriteText(strWhois, g_sSettings.GetRGBTime(), g_sSettings.GetRGBOp(), TRUE, TRUE, FALSE);
				m_aWhoisBuffer.RemoveAt(i);
				bReturn = TRUE;
				break;
			}
		}
	}
	return (LRESULT)bReturn;
}


void CChatView::OnOpenURL()
{

	LinkWarning warning;
	warning.m_strURL = m_strSelURL;
	BOOL bOpen = g_sSettings.GetShowURLWarning() ? (warning.DoModal() == IDOK) : TRUE;
	
	if(bOpen){

		ShellExecute(0, "open", m_strSelURL, NULL, NULL, SW_SHOW);
	}
}

void CChatView::OnOpenURLinnewbrowser()
{

	LinkWarning warning;
	warning.m_strURL = m_strSelURL;

	BOOL bOpen = g_sSettings.GetShowURLWarning() ? (warning.DoModal() == IDOK) : TRUE;
	
	if(bOpen){
	
		ShellExecute(0, "open", "iexplore.exe", "-new " + m_strSelURL, NULL, SW_SHOW);
	}
}

void CChatView::OnCopyAddress()
{
	
	CClipboard::SetText((LPSTR)(LPCTSTR)m_strSelURL);
}

void CChatView::OnUserlistSendmessage() 
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strName;
	DWORD dwIP   = 0;
	WORD  wPort  = 0;

	if(n >= 0){

		strName = m_lcUsers.GetItemText(n, 0);
		
		MX_USERINFO user;
		if(m_aUsers.Lookup(strName, user)){

			EXT_MSG_STRUCT   msg;
			msg.wParam = (WPARAM)user.dwNodeIP;
			msg.lParam = (LPARAM)user.wNodePort;
			msg.lpszParam = (LPCSTR)user.strUser;
			::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_NEWPM, 0, (LPARAM)&msg);
		}
	}
}

void CChatView::OnUserlistBrowse()
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strName;
	DWORD dwIP   = 0;
	WORD  wPort  = 0;

	if(n >= 0){

		strName = m_lcUsers.GetItemText(n, 0);
		
		MX_USERINFO user;
		if(m_aUsers.Lookup(strName, user)){

			EXT_MSG_STRUCT   msg;
			msg.wParam = (WPARAM)user.dwNodeIP;
			msg.lParam = (LPARAM)user.wNodePort;
			msg.lpszParam = (LPCSTR)user.strUser;
			::SendMessage(AfxGetMainWnd()->m_hWnd, UWM_BROWSE, 0, (LPARAM)&msg);
		}
	}
}

void CChatView::OnUserlistWhois()
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strName;
	DWORD dwIP   = 0;
	WORD  wPort  = 0;

	if(n >= 0){

		strName = m_lcUsers.GetItemText(n, 0);
		
		MX_USERINFO user;
		if(m_aUsers.Lookup(strName, user)){

			EXT_MSG_STRUCT whois;
			whois.wParam	= user.dwNodeIP;
			whois.lParam	= user.wNodePort;
			whois.lpszParam	= user.strUser;
			m_aWhoisBuffer.Add(whois);

			AfxGetMainWnd()->SendMessage(UWM_WHOIS, 0, (LPARAM)&whois);

			CString strWhois;
			strWhois.Format(IDS_SEND_WHOIS, whois.lpszParam);
			WriteText(strWhois, g_sSettings.GetRGBTime(), g_sSettings.GetRGBOp(), TRUE, TRUE, FALSE);
		}
	}
}

void CChatView::OnUserlistRedirect() 
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strCmd;
	if(n >= 0){

		strCmd = m_aRCMSMenu[0];
		strCmd.Replace("%NAME%", m_lcUsers.GetItemText(n, 0));
		Input(strCmd);
	}
}

void CChatView::OnUserlistKick() 
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strCmd;
	if(n >= 0){

		strCmd = m_aRCMSMenu[1];
		strCmd.Replace("%NAME%", m_lcUsers.GetItemText(n, 0));
		Input(strCmd);
	}
}

void CChatView::OnUserlistKickban() 
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strCmd;
	if(n >= 0){

		strCmd = m_aRCMSMenu[2];
		strCmd.Replace("%NAME%", m_lcUsers.GetItemText(n, 0));
		Input(strCmd);
	}
}

void CChatView::OnUserlistBan() 
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strCmd;
	if(n >= 0){

		strCmd = m_aRCMSMenu[3];
		strCmd.Replace("%NAME%", m_lcUsers.GetItemText(n, 0));
		Input(strCmd);
	}
}

void CChatView::OnUserlistUnban() 
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strCmd;
	if(n >= 0){

		strCmd = m_aRCMSMenu[4];
		strCmd.Replace("%NAME%", m_lcUsers.GetItemText(n, 0));
		Input(strCmd);
	}
}

void CChatView::OnUserlistPrintuserstat() 
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strCmd;
	if(n >= 0){

		strCmd = m_aRCMSMenu[5];
		strCmd.Replace("%NAME%", m_lcUsers.GetItemText(n, 0));
		Input(strCmd);
	}
}

void CChatView::OnUserlistPrintip() 
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strCmd;
	if(n >= 0){

		strCmd = m_aRCMSMenu[6];
		strCmd.Replace("%NAME%", m_lcUsers.GetItemText(n, 0));
		Input(strCmd);
	}
}

void CChatView::OnUserlistAddadmin() 
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strCmd;
	if(n >= 0){

		strCmd = m_aRCMSMenu[7];
		strCmd.Replace("%NAME%", m_lcUsers.GetItemText(n, 0));
		Input(strCmd);
	}
}

void CChatView::OnUserlistRemoveadmin() 
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strCmd;
	if(n >= 0){

		strCmd = m_aRCMSMenu[8];
		strCmd.Replace("%NAME%", m_lcUsers.GetItemText(n, 0));
		Input(strCmd);
	}
}

void CChatView::OnUserlistReadusermessage() 
{

	
	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strCmd;
	if(n >= 0){

		strCmd = m_aRCMSMenu[9];
		strCmd.Replace("%NAME%", m_lcUsers.GetItemText(n, 0));
		Input(strCmd);
	}
}

void CChatView::OnUserlistCustomizethismenu() 
{
	CString strIniFile, strRoom;

	/*strRoom = GetDocument()->m_strRoomShort;
	strRoom.Remove('?');
	strRoom.Remove(':');
	strRoom.Remove(',');
	strRoom.Remove('\\');
	strRoom.Remove('/');
	strRoom.Remove('<');
	strRoom.Remove('>');
	strRoom.Remove('\"');
	strRoom.Remove('*');
	strRoom.Remove('|');
	strRoom.Replace(1, '-');*/
	
	strRoom = Util::MakeValidFilename(GetDocument()->m_strRoomShort);
	strIniFile.Format("%s\\%s.mnu", g_sSettings.GetWorkingDir(), strRoom);


	CStdioFile ini;
	CString strBuffer;
	//m_aRCMSMenu.RemoveAll();

	TRY{

		ini.Open(strIniFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeText|CFile::shareExclusive);

		if(ini.GetLength() == 0){

			if(m_nServerType == SERVER_WINMX353){

				ini.WriteString("/kickban %NAME% 5\n");
				ini.WriteString("/kick %NAME%\n");
				ini.WriteString("/kickban %NAME%\n");
				ini.WriteString("/ban %NAME%\n");
				ini.WriteString("/unban %NAME%\n");
				ini.WriteString("/setuserlevel %NAME% 5\n");
				ini.WriteString("/setuserlevel %NAME% 65\n");
				ini.WriteString("/setuserlevel %NAME% 200\n");
				ini.WriteString("/setuserlevel %NAME% 65\n");
				ini.WriteString("/level %NAME%\n");
			}
			else if(m_nServerType == SERVER_ROSE){

				ini.WriteString("/exile %NAME%\n");
				ini.WriteString("/kick %NAME%\n");
				ini.WriteString("/kickban %NAME%");
				ini.WriteString("/ban %NAME%\n");
				ini.WriteString("/unban %NAME%\n");
				ini.WriteString("/muzzle %NAME%\n");
				ini.WriteString("/voice %NAME%\n");
				ini.WriteString("/admin %NAME%\n");
				ini.WriteString("/remadmin %NAME%\n");
				ini.WriteString("/usermodes %NAME%\n");
			}
			else if((m_nServerType == SERVER_MCS) || (m_nServerType == SERVER_WCS)){

				ini.WriteString("#UserCmd Redirect %NAME%\n");
				ini.WriteString("/kick %NAME%\n");
				ini.WriteString("/kickban %NAME%\n");
				ini.WriteString("/ban %NAME%\n");
				ini.WriteString("/unban %NAME%\n");
				ini.WriteString("/stats %NAME%\n");
				ini.WriteString("#AdminCmd PrintIP %NAME%\n");
				ini.WriteString("/addadmin %NAME%\n");
				ini.WriteString("/removeadmin %NAME%\n");
				ini.WriteString("#UserCmd Readmsg %NAME%\n");
			}
			else if(m_nServerType == SERVER_FXSERV){

				ini.WriteString("/exile %NAME%\n");
				ini.WriteString("/kick %NAME%\n");
				ini.WriteString("/kickban %NAME%\n");
				ini.WriteString("/ban %NAME%\n");
				ini.WriteString("/unban %NAME%\n");
				ini.WriteString("/setuserlevel +v %NAME%\n");
				ini.WriteString("/setuserlevel -v %NAME%\n");
				ini.WriteString("/setuserlevel +kbtwm@ %NAME%\n");
				ini.WriteString("/removeadmin -kbtwm@ %NAME%\n");
				ini.WriteString("/level %NAME%\n");
			}
			else{

				ini.WriteString("#UserCmd Redirect %NAME%\n");
				ini.WriteString("#UserCmd /kick %NAME%\n");
				ini.WriteString("#AdminCmd /kickban %NAME%\n");
				ini.WriteString("#AdminCmd /ban %NAME%\n");
				ini.WriteString("#UserCmd /unban %NAME%\n");
				ini.WriteString("#userCmd PrintUserStat %NAME%\n");
				ini.WriteString("#AdminCmd PrintIP %NAME%\n");
				ini.WriteString("#AdminCmd AddAdmin %NAME%\n");
				ini.WriteString("#AdminCmd RemoveAdmin %NAME%\n");
				ini.WriteString("#UserCmd Readmsg %NAME%\n");
			}
		}

		ini.Close();

		
	}
	CATCH(CFileException, e){

	}END_CATCH;


	AfxMessageBox(IDS_EDIT_MENU, MB_ICONINFORMATION);

	ShellExecute(0, "open", "notepad.exe", strIniFile, 0, SW_SHOW);
}

void CChatView::OnUserlistCopyusername() 
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strCmd;
	if(n >= 0){

		strCmd = m_lcUsers.GetItemText(n, 0);
		CClipboard::SetText((LPSTR)(LPCSTR)strCmd);
	}	
}


void CChatView::OnUserlistIgnore() 
{
	
	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strUser;
	if(n >= 0){

		strUser = Util::GetBaseName(m_lcUsers.GetItemText(n, 0), TRUE, TRUE);
		/*strUser = Util::GetBaseName(m_lcUsers.GetItemText(n, 0));
		int nLen = strUser.GetLength();
		if(nLen - 3 > 0){

			if(_istdigit(strUser[nLen-1]) && _istdigit(strUser[nLen-2]) && _istdigit(strUser[nLen-3])){
			
				strUser = strUser.Left(nLen-3);
			}
		}*/
		strUser += "*";

		for(int i = 0; i < g_sSettings.m_aIgnored.GetSize(); i++){

			if(g_sSettings.m_aIgnored[i] == strUser)
				return;
		}
		g_sSettings.m_aIgnored.Add(strUser);
		if(GetKeyState(VK_SHIFT) < 0){

			CString strIgnored;
			strIgnored.Format(IDS_IGNORED_MX353, strUser);
			m_mxClient.SendText(strIgnored);
		}
	}	
}

LRESULT CChatView::OnSetChannelPic(WPARAM wParam, LPARAM lParam)
{

	CString strName = GetDocument()->m_strRoomShort + "*";

	CString strFilter, strValue, strLabel, strImage;
	strFilter.LoadString(IDS_IMAGE_FILTER);

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());
	
	int n = ini.GetValue("ChannelToolTips", "ChannelImageCnt", 0);
	for(int i = 0; i < n; i++){

		strLabel.Format("ChannelName_%d", i);
		strValue = ini.GetValue("ChannelToolTips", strLabel, "");

		if(strName ==  strValue){

			strLabel.Format("ChannelImage_%d", i);
			strImage = ini.GetValue("ChannelToolTips", strLabel, "");
			break;
		}
	}

	CFileDialog dlg(TRUE, ".jpg", strImage, OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		if(dlg.GetPathName() != strImage){

			strLabel.Format("ChannelName_%d", i);
			ini.SetValue("ChannelToolTips", strLabel, strName);
			strLabel.Format("ChannelImage_%d", i);
			ini.SetValue("ChannelToolTips", strLabel, dlg.GetPathName());
			if(i >= n){

				ini.SetValue("ChannelToolTips", "ChannelImageCnt", n+1);
			}
		}
	}
	return 2;
}

void CChatView::OnAdvancedSetpicture()
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strName;
	if(n >= 0){

		strName = Util::GetBaseName(m_lcUsers.GetItemText(n, 0), TRUE, TRUE) + "*";

		CString strFilter, strValue, strLabel, strImage;
		strFilter.LoadString(IDS_IMAGE_FILTER);

		CIni ini;
		ini.SetIniFileName(g_sSettings.GetIniFile());
		
		n = ini.GetValue("UserToolTips", "UserImageCnt", 0);
		for(int i = 0; i < n; i++){

			strLabel.Format("UserName_%d", i);
			strValue = ini.GetValue("UserToolTips", strLabel, "");

			if(strName ==  strValue){

				strLabel.Format("UserImage_%d", i);
				strImage = ini.GetValue("UserToolTips", strLabel, "");
				break;
			}
		}

		CFileDialog dlg(TRUE, ".jpg", strImage, OFN_FILEMUSTEXIST, strFilter, this);

		if(dlg.DoModal() == IDOK){

			if(dlg.GetPathName() != strImage){

				strLabel.Format("UserName_%d", i);
				ini.SetValue("UserToolTips", strLabel, strName);
				strLabel.Format("UserImage_%d", i);
				ini.SetValue("UserToolTips", strLabel, dlg.GetPathName());
				if(i >= n){

					ini.SetValue("UserToolTips", "UserImageCnt", n+1);
				}
			}
		}
	}
}

void CChatView::OnAdvancedTrustuser()
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strUser;
	if(n >= 0){

		strUser = Util::GetBaseName(m_lcUsers.GetItemText(n, 0));
		int nLen = strUser.GetLength();
		if(nLen - 3 > 0){

			if(_istdigit(strUser[nLen-1]) && _istdigit(strUser[nLen-2]) && _istdigit(strUser[nLen-3])){
			
				strUser = strUser.Left(nLen-3);
			}
		}
		strUser += "*";

		for(int i = 0; i < g_sSettings.m_aTrusted.GetSize(); i++){

			if(g_sSettings.m_aTrusted[i] == strUser)
				return;
		}
		g_sSettings.m_aTrusted.Add(strUser);
	}	
}

void CChatView::OnUpdateAdvancedTrustuser(CCmdUI *pCmdUI)
{

	pCmdUI->Enable(m_lcUsers.GetSelectedCount());
}

void CChatView::OnAdvancedRemovetrusted()
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strUser;
	if(n >= 0){

		strUser = Util::GetBaseName(m_lcUsers.GetItemText(n, 0));
		int nLen = strUser.GetLength();
		if(nLen - 3 > 0){

			if(_istdigit(strUser[nLen-1]) && _istdigit(strUser[nLen-2]) && _istdigit(strUser[nLen-3])){
			
				strUser = strUser.Left(nLen-3);
			}
		}
		strUser += "*";

		for(int i = 0; i < g_sSettings.m_aTrusted.GetSize(); i++){

			if(g_sSettings.m_aTrusted[i] == strUser){

				g_sSettings.m_aTrusted.RemoveAt(i);
			}
		}
	}	
}

void CChatView::OnUpdateAdvancedRemovetrusted(CCmdUI *pCmdUI)
{

	pCmdUI->Enable(m_lcUsers.GetSelectedCount());
}

void CChatView::OnUserlistUnignore() 
{

	int n = m_lcUsers.GetNextItem(-1, LVNI_SELECTED);
	CString strUser;
	if(n >= 0){

		strUser = Util::GetBaseName(m_lcUsers.GetItemText(n, 0), TRUE, TRUE);
		/*strUser = Util::GetBaseName(m_lcUsers.GetItemText(n, 0));
		int nLen = strUser.GetLength();
		if(nLen - 3 > 0){

			if(_istdigit(strUser[nLen-1]) && _istdigit(strUser[nLen-2]) && _istdigit(strUser[nLen-3])){
			
				strUser = strUser.Left(nLen-3);
			}
		}*/
		strUser += "*";

		for(int i = 0; i < g_sSettings.m_aIgnored.GetSize(); i++){

			if(g_sSettings.m_aIgnored[i] == strUser){

				g_sSettings.m_aIgnored.RemoveAt(i);
				if(GetKeyState(VK_SHIFT) < 0){

					CString strIgnored;
					strIgnored.Format(IDS_UNIGNORE_MX353, strUser);
					m_mxClient.SendText(strIgnored);
				}
			}
		}
	}	
}

void CChatView::OnUpdateUserlistMenu(CCmdUI* pCmdUI) 
{

	pCmdUI->Enable(m_lcUsers.GetNextItem(-1, LVNI_SELECTED) != -1);
}

void CChatView::LoadRCMSMenu()
{

	CString strIniFile, strRoom;

	/*strRoom = GetDocument()->m_strRoomShort;
	strRoom.Remove('?');
	strRoom.Remove(':');
	strRoom.Remove(',');
	strRoom.Remove('\\');
	strRoom.Remove('/');
	strRoom.Remove('<');
	strRoom.Remove('>');
	strRoom.Remove('\"');
	strRoom.Remove('*');
	strRoom.Remove('|');
	strRoom.Replace(1, '-');*/

	strRoom = Util::MakeValidFilename(GetDocument()->m_strRoomShort);

	strIniFile.Format("%s\\%s.mnu", g_sSettings.GetWorkingDir(), strRoom);

	CStdioFile ini;
	CString strBuffer;
	m_aRCMSMenu.RemoveAll();

	TRY{

		if(ini.Open(strIniFile, CFile::modeRead|CFile::typeText|CFile::shareExclusive)){


			while(ini.ReadString(strBuffer)){

				if(!strBuffer.IsEmpty()){

					strBuffer.TrimLeft();
					strBuffer.TrimRight();
					m_aRCMSMenu.Add(strBuffer);
				}
			}
			ini.Close();
		}
		
	}
	CATCH(CFileException, e){

	}END_CATCH;

	if(m_aRCMSMenu.GetSize() == 0){

		if(m_nServerType == SERVER_WINMX353){

			m_aRCMSMenu.Add("/kickban %NAME% 5");
			m_aRCMSMenu.Add("/kick %NAME%");
			m_aRCMSMenu.Add("/kickban %NAME% 255");
			m_aRCMSMenu.Add("/ban %NAME% 255");
			m_aRCMSMenu.Add("/unban %NAME%");
			m_aRCMSMenu.Add("/setuserlevel %NAME% 5");	  // remove voice
			m_aRCMSMenu.Add("/setuserlevel %NAME% 65");	  // give voice
			m_aRCMSMenu.Add("/setuserlevel %NAME% 200");  // add admin
			m_aRCMSMenu.Add("/setuserlevel %NAME% 65");	  // remove admin
			m_aRCMSMenu.Add("/level %NAME%");             // display userlevel
		}
		else if((m_nServerType == SERVER_MCS) || (m_nServerType == SERVER_WCS)){

			m_aRCMSMenu.Add("#UserCmd Redirect %NAME%");
			m_aRCMSMenu.Add("/kick %NAME%");
			m_aRCMSMenu.Add("/kickban %NAME%");
			m_aRCMSMenu.Add("/ban %NAME%");
			m_aRCMSMenu.Add("/unban %NAME%");
			m_aRCMSMenu.Add("/stats %NAME%");
			m_aRCMSMenu.Add("#AdminCmd PrintIP %NAME%");
			m_aRCMSMenu.Add("/addadmin %NAME%");
			m_aRCMSMenu.Add("/removeadmin %NAME%");
			m_aRCMSMenu.Add("#UserCmd Readmsg %NAME%");
		}
		else if(m_nServerType == SERVER_ROSE){

			m_aRCMSMenu.Add("/exile %NAME%");
			m_aRCMSMenu.Add("/kick %NAME%");
			m_aRCMSMenu.Add("/kickban %NAME%");
			m_aRCMSMenu.Add("/ban %NAME%");
			m_aRCMSMenu.Add("/unban %NAME%");
			m_aRCMSMenu.Add("/voice %NAME%");
			m_aRCMSMenu.Add("/muzzle %NAME%");
			m_aRCMSMenu.Add("/admin %NAME%");
			m_aRCMSMenu.Add("/remadmin %NAME%");
			m_aRCMSMenu.Add("/usermode %NAME%");
		}
		else if(m_nServerType == SERVER_FXSERV){

			m_aRCMSMenu.Add("/exile %NAME%");
			m_aRCMSMenu.Add("/kick %NAME%");
			m_aRCMSMenu.Add("/kickban %NAME%");
			m_aRCMSMenu.Add("/ban %NAME%");
			m_aRCMSMenu.Add("/unban %NAME%");
			m_aRCMSMenu.Add("/setuserlevel +v %NAME%");
			m_aRCMSMenu.Add("/setuserlevel -v %NAME%");
			m_aRCMSMenu.Add("/setuserlevel +kbtwm@ %NAME%");
			m_aRCMSMenu.Add("/removeadmin -kbtwm@ %NAME%");
			m_aRCMSMenu.Add("/level %NAME%");
		}
		else{

			// Fill in defaults
			m_aRCMSMenu.Add("#UserCmd Redirect %NAME%");
			m_aRCMSMenu.Add("#UserCmd /kick %NAME%");
			m_aRCMSMenu.Add("#AdminCmd /kickban %NAME%");
			m_aRCMSMenu.Add("#AdminCmd /ban %NAME%");
			m_aRCMSMenu.Add("#UserCmd /unban %NAME%");
			m_aRCMSMenu.Add("#UserCmd PrintUserStat %NAME%");
			m_aRCMSMenu.Add("#AdminCmd PrintIP %NAME%");
			m_aRCMSMenu.Add("#AdminCmd AddAdmin %NAME%");
			m_aRCMSMenu.Add("#AdminCmd RemoveAdmin %NAME%");
			m_aRCMSMenu.Add("#UserCmd Readmsg %NAME%");
		}
	}
}

void CChatView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{

	if(GetApp()->GetMainFrame()->GetActiveFrame() != GetParentFrame()){

		return;
	}

	if(m_pStatusBar){

		CString strText;
		
		m_pStatusBar->SetPaneText(2, GetDocument()->m_strRoomShort);

		strText.Format(IDS_AVERAGE_LAG, m_dwAvLag);
		COLORREF cr = 0;
		if(m_dwAvLag <= 500) cr = RGB(0, 100, 0);
		else if((m_dwAvLag > 500) && (m_dwAvLag <= 1000)) cr = 0;
		else if((m_dwAvLag > 1000) && (m_dwAvLag < 1500)) cr = RGB(255, 192, 64);
		else cr = RGB(200, 0, 0);
		m_pStatusBar->SetPaneText(3, strText, cr);

		strText.Format(IDS_USERS, m_lcUsers.GetItemCount());
		m_pStatusBar->SetPaneText(4, strText);
	}
}

void CChatView::UpdateAverageLag(BOOL bStart)
{

	if(bStart){

		m_dwLastTic = GetTickCount();
	}
	else if(m_dwLastTic != 0){


		DWORD dwTime = GetTickCount() - m_dwLastTic;
		CString strText;

		m_dwAvLag = (m_dwAvLag + dwTime) / 2;
		m_dwLastTic = 0;
		
		strText.Format(IDS_AVERAGE_LAG, m_dwAvLag);
		
		COLORREF cr = 0;
		if(m_dwAvLag <= 500) cr = RGB(0, 170, 85);
		else if((m_dwAvLag > 500) && (m_dwAvLag <= 1000)) cr = RGB(255, 180, 0);
		else if((m_dwAvLag > 1000) && (m_dwAvLag < 1500)) cr = RGB(255, 120, 0);
		else cr = RGB(200, 0, 0);
		
		m_pStatusBar->SetPaneText(3, strText, cr);
	}
}


void CChatView::OnReconnect() 
{

	Disconnect();
	ReCalcLayout();
	WriteSystemMsg(FALSE, IDS_RECONNECTING, g_sSettings.GetRGBPend());
	m_mxClient.Connect();
}

void CChatView::OnUpdateReconnect(CCmdUI* pCmdUI) 
{
}

void CChatView::OnDisplayWinampsong() 
{

	if(!m_mxClient.m_bListen) return;
	
	CString strMsg;
	if(Util::IsVideoPlaying()){

		strMsg = g_sSettings.GetVideoMsg();
	}
	else{

		strMsg = g_sSettings.GetWinampMsg();
	}

	//Util::ReplaceVars(strMsg);

	Input(strMsg);
}

void CChatView::OnDisplaySystemuptime() 
{
	if(!m_mxClient.m_bListen) return;
	
	Input(Util::GetUpTime());
}

void CChatView::OnDisplaySysteminfo() 
{
	if(!m_mxClient.m_bListen) return;
	
	Input(Util::GetMySystemInfo());
}

void CChatView::OnDisplayinchannelExtendedsysteminfo()
{
	if(!m_mxClient.m_bListen) return;

	CString strTmp, strInfo = Util::GetMySystemInfoEx();
	CTokenizer token(strInfo, "\n");
	
	while(token.Next(strTmp)){

		Input(strTmp);
		Sleep(100);
	}
}

void CChatView::OnDisplayAveragelag() 
{
	if(!m_mxClient.m_bListen) return;
	
	CString strMsg;
	strMsg.Format(IDS_AVERAGE_LAG_CHAT, m_dwAvLag / 1000, m_dwAvLag % 1000);
	Input(strMsg);
}

void CChatView::OnUpdateWinampsongMenu(CCmdUI* pCmdUI) 
{

	HWND hwndWinamp = ::FindWindow("Winamp v1.x",NULL);
	pCmdUI->Enable(hwndWinamp != NULL);	
}


void CChatView::OnChatroomClearscreen() 
{

	m_rChatDoc.Clear();
	m_rChat.InvalidateIfModified();
}

void CChatView::OnChatroomCopyroomname() 
{

	CClipboard::SetText((LPSTR)(LPCSTR)GetDocument()->m_strRoom);
}

void CChatView::InputWelcome()
{

	if(g_sSettings.GetSoundFX()){

		if(!g_sSettings.GetSfxJoin().IsEmpty())
			PlaySound(g_sSettings.GetSfxJoin(), 0, SND_ASYNC|SND_FILENAME|SND_NODEFAULT);
	}
	if(GetDocument()->m_strLogin.GetLength()){

		CString strLogin;
		strLogin.Format("/login %s", GetDocument()->m_strLogin);
		Input(strLogin);
	}
	if(g_sSettings.GetDoEnterMsg()){
		
		CIni ini;
		ini.SetIniFileName(g_sSettings.GetIniFile());

		int n = ini.GetValue("WelcomeMsgs", "Num", 0);
		if(n == 0) return;

		CString strMsg;
		strMsg.Format("Msg_%d", (rand()+GetTickCount()) %n);

		strMsg = ini.GetValue("WelcomeMsgs", strMsg, "");

		if(strMsg.IsEmpty()) return;

		//Util::ReplaceVars(strMsg);
		//strMsg.Replace("%USERS%", Util::Formatint(m_lcUsers.GetItemCount()));
		Input(strMsg);
	}
	else{

		CIni ini;
		ini.SetIniFileName(g_sSettings.GetIniFile());
		int nRooms = ini.GetValue("WelcomeRooms", "Num", 0);
		int nHello = ini.GetValue("WelcomeMsgs", "Num", 0);
		
		CString strRoom, strMsg;

		for(int i = 0; (i < nRooms) && nHello; i++){

			strRoom.Format("Room_%d", i);
			strRoom = ini.GetValue("WelcomeRooms", strRoom, "");
			if(strRoom.IsEmpty()) continue;

			if(Util::WildcardMatch(GetDocument()->m_strRoom, strRoom, FALSE, FALSE)){

				strMsg.Format("Msg_%d", (rand()+GetTickCount())%nHello);
				strMsg = ini.GetValue("WelcomeMsgs", strMsg, "");
				if(strMsg.IsEmpty()) return;

				//Util::ReplaceVars(strMsg);
				//strMsg.Replace("%USERS%", Util::Formatint(m_lcUsers.GetItemCount()));
				Input(strMsg);
				break;
			}
		}
	}
}


void CChatView::OnSetFocus(CWnd* pOldWnd) 
{

	if(pOldWnd != this){

		CFormView::OnSetFocus(pOldWnd);
		
 		OnUpdate(this, 0, 0);
	}
}

void CChatView::OnChatroomTexttricksHacker() 
{

	
	CInputRequest dlg;
	dlg.SetMode(0, &m_fFont);
	if(dlg.DoModal() == IDOK){

		Input(dlg.m_strInput);
	}
}

void CChatView::OnChatroomTexttricksBubbles() 
{

	CInputRequest dlg;
	dlg.SetMode(1, &m_fFont);
	if(dlg.DoModal() == IDOK){

		Input(dlg.m_strInput);
	}
}

void CChatView::OnChatroomTexttricksBox() 
{

	
	CInputRequest dlg;
	dlg.SetMode(2, &m_fFont);
	if(dlg.DoModal() == IDOK){

		Input(dlg.m_strInput);
	}
}

void CChatView::OnChatTexttricks3dbuttonsnormal() 
{
	
	CInputRequest dlg;
	dlg.SetMode(3, &m_fFont);
	if(dlg.DoModal() == IDOK){

		Input(dlg.m_strInput);
	}
}


void CChatView::OnTexttricksWildcolors()
{

	CInputRequest dlg;
	dlg.SetMode(8, &m_fFont);
	if(dlg.DoModal() == IDOK){

		Input(dlg.m_strInput);
	}
}

void CChatView::OnChatTexttricks3dbuttonsaction() 
{

	CInputRequest dlg;
	dlg.SetMode(4, &m_fFont);
	if(dlg.DoModal() == IDOK){

		Input(dlg.m_strInput);
	}
}

void CChatView::OnTexttricks3dbuttons()
{

	CInputRequest dlg;
	dlg.SetMode(7, &m_fFont);
	if(dlg.DoModal() == IDOK){

		Input(dlg.m_strInput);
	}
}


void CChatView::OnChatroomAsciiartdesign() 
{

	
	CInputRequest dlg;
	dlg.SetMode(5, &m_fFont);
	if(dlg.DoModal() == IDOK){

		Input(dlg.m_strInput);
	}
}

LRESULT CChatView::OnEchoText(WPARAM wParam, LPARAM lParam)
{

	WriteText(((EXT_MSG_STRUCT*)lParam)->lpszMsg, (COLORREF)((EXT_MSG_STRUCT*)lParam)->wParam, (COLORREF)((EXT_MSG_STRUCT*)lParam)->lParam, TRUE, FALSE);
	return 1;
}

LRESULT CChatView::OnEchoSysText(WPARAM wParam, LPARAM lParam)
{

	if(((EXT_MSG_STRUCT*)lParam)->wParam == g_sSettings.GetRGBErr()){


		m_bHideSystem = FALSE;
		ReCalcLayout();
	}

	WriteSystemMsg(FALSE, ((EXT_MSG_STRUCT*)lParam)->lpszMsg, (COLORREF)((EXT_MSG_STRUCT*)lParam)->wParam, (COLORREF)((EXT_MSG_STRUCT*)lParam)->lParam, TRUE);
	return 1;
}

void CChatView::HandleHiLite(void)
{

	if(GetApp()->GetMainFrame()->MDIGetActive() != GetParentFrame()){

		GetApp()->GetMainFrame()->m_wndDocSelector.SetHiLite(this);
	}
}

LRESULT CChatView::OnSetServerType(WPARAM wParam, LPARAM lParam)
{

	m_nServerType = (int)wParam;
	switch((int)wParam){

		case SERVER_RCMS:
		case SERVER_OUKA:
		case SERVER_RCMS353:
		case SERVER_UNKNOWN:
			m_eInput.SetCommands(&g_sSettings.m_aRCMSCommands);
			break;
		case SERVER_WINMX353:
		case SERVER_MCMA:
			m_eInput.SetCommands(&g_sSettings.m_aWinMXCommands);
			break;
		case SERVER_MCS:
		case SERVER_WCS:
			m_eInput.SetCommands(&g_sSettings.m_aMXToolsCommands);
			break;
		case SERVER_ROSE:
			m_eInput.SetCommands(&g_sSettings.m_aRoseCommands);
			break;
		case SERVER_FXSERV:
			m_eInput.SetCommands(&g_sSettings.m_aFXServCommands);
			break;
		default:
			ASSERT(FALSE);
	}
	LoadRCMSMenu();
	return 1;
}

LRESULT CChatView::OnRenameCl(WPARAM wParam, LPARAM lParam)
{

	GetDocument()->m_strName = g_sSettings.GetNickname();
	GetDocument()->m_wLine   = g_sSettings.GetLine();
	GetDocument()->m_dwFiles = g_sSettings.GetFiles();

	m_mxClient.SendRename(GetDocument()->m_strName, GetDocument()->m_dwFiles, GetDocument()->m_wLine);

	return 1;
}


void CChatView::OnAwayControl(UINT nID)
{

	int nAwayItem = nID - ID_AWAY_BASE - 1;

	::SendMessage(GetApp()->GetMainFrame()->m_hWnd, UWM_SETAWAY, (WPARAM)nAwayItem, 0);
}

void CChatView::OnAwayControlBack(UINT nID)
{
/*
#E033# "/me is now back from »%s«.  %s was away for %s"
#E034# "You are now back from %s. You have been away for %s"
*/	

	::SendMessage(GetApp()->GetMainFrame()->m_hWnd, UWM_SETBACK, 
					nID == ID_BACK_SETBACK ? 0 : 1, 0);
}

CString CChatView::GetUserInput(CString strReason)
{
	
	CString strResult;
	CInputRequest dlg;

	dlg.m_strInput = strReason;
	dlg.SetMode(6, &m_fFont);
	if(dlg.DoModal() == IDOK){

		strResult = dlg.m_strInput;
	}
	
	return strResult;
}

#ifdef _ROBO_READER
void CChatView::Speak(CString strName, CString strMsg, BOOL bAction)
{

	if(GetApp()->GetMainFrame()->GetActiveFrame() == GetParentFrame()){

		m_dlgVoice.SpeakChat(strName, strMsg, bAction);
	}
}
#endif

BOOL CChatView::Log(CString strName, CString strText)
{

	if(g_sSettings.GetLog()){


		if(strText.Find(COLOR_DELIM) >= 0 || strName.Find(COLOR_DELIM) >= 0){
		
			// remove color codes before passing to logger
			CString strColor;
			for(int i = 0; i < num_color; i++){

				strColor.Format("%c%c", 3, color_map[i]);
				strName.Replace(strColor, "");
				strText.Replace(strColor, "");
			}		
		}
		m_lLog.Log(strName, strText);
	}
	return TRUE;
}

void CChatView::OnViewLogfile()
{

	//m_lLog.Open();
	CLogView dlg;
	dlg.m_strRoom = Util::MakeValidFilename(GetDocument()->m_strRoomShort);
	dlg.DoModal();
}

void CChatView::Disconnect(void)
{

	if(m_mxClient.m_bListen){

		m_mxClient.Disconnect();
	}

	m_nRetries		= 0;
	m_bHasJoined	= FALSE;
	m_bHideSystem	= FALSE;

	m_aUsers.RemoveAll();
	m_lcUsers.DeleteAllItems();
}

void CChatView::OnChatroomViewtopic()
{

	if(g_sSettings.GetPrintTime()){

		WriteTime(g_sSettings.GetRGBTopic());
	}

	CString strOut;
	strOut.Format(IDS_TOPIC, GetDocument()->m_strTopic);
	WriteText(strOut, g_sSettings.GetRGBTopic(), g_sSettings.GetRGBBg(), TRUE, TRUE);
}

void CChatView::OnChatroomAddtoautojoin()
{

	CIni ini;
	int n = 0;
	CString strTmp;
	
	ini.SetIniFileName(g_sSettings.GetIniFile());
	n = ini.GetValue("AutoJoins", "Num", 0);

	for(int i = 1; i < n+1; i++){

		strTmp.Format("AutoJoin_%03d", i);
		if(ini.GetValue("AutoJoins", strTmp, "") == GetDocument()->m_strRoom){

			ini.SetValue("AutoJoins", strTmp, "");	

			CString strOut;
			strOut.Format(IDS_AUTOJOIN_REMOVE, GetDocument()->m_strRoom);
			WriteSystemMsg(FALSE, strOut, g_sSettings.GetRGBPend());
			return;
		}
	}

	for(i = 1; i < n+1; i++){

		strTmp.Format("AutoJoin_%03d", i);
		if(ini.GetValue("AutoJoins", strTmp, "").IsEmpty()) break;
	}

	strTmp.Format("AutoJoin_%03d", i);
	ini.SetValue("AutoJoins", strTmp, GetDocument()->m_strRoom);
	strTmp.Format("AutoJoinLogin_%03d", i);
	ini.SetValue("AutoJoins", strTmp, GetDocument()->m_strLogin);
	ini.SetValue("AutoJoins", "Num", (i <= n ? n : n+1));

	CString strOut;
	strOut.Format(IDS_AUTOJOIN_ADD, GetDocument()->m_strRoom);
	WriteSystemMsg(FALSE, strOut, g_sSettings.GetRGBOK());
}

void CChatView::OnChatroomViewcurrentbans()
{

	if(m_mxClient.m_bListen && (m_nServerType >= SERVER_WINMX353)){

		m_mxClient.SendText("/listbans");
	}
	else{

		WriteSystemMsg(TRUE, IDS_ERROR_MX353ONLY, g_sSettings.GetRGBPend());
	}
}


void CChatView::OnDisplayinchannelPing()
{

	CPing ping;
	CPingReply pr;
	CString strPing;

	if(ping.Ping(Util::GetIPFromRoom(GetDocument()->m_strRoom), pr, 64, 2000, 32)){

		strPing.Format("%u", pr.RTT);
	}
	else{
						  
		strPing = "> 2000";
	}

	CString strOut;
	strOut.Format(IDS_PING, 32, strPing, 64);
	Input(strOut);
}

void CChatView::OnViewSystemconsole()
{

	m_bHideSystem = !m_bHideSystem;
	ReCalcLayout();
}

void CChatView::OnUpdateViewSystemconsole(CCmdUI *pCmdUI)
{

	pCmdUI->SetCheck(!m_bHideSystem);
}


CString CChatView::MakeColorName(CString strName, CString strStr) //, int nServerType)
{

	// remove all color codes from name
	CString strTag;

	for(int i = 0; i < 255; i++){

		strTag.Format(strStr, i+1);
		strName.Replace(strTag, "");
	}

	strName.Replace("#cx#", "");
	strName.Replace("#c?#", "");

	return strName;
	/*if(nServerType == SERVER_WCS){

		for(int i = 0; i < 255; i++){

			strTag.Format(strStr, i+1);
			strColor.Format("%c%c", 3, i);
			strName.Replace(strTag, strColor);
		}
	}
	else{

		for(int i = 0; i < num_color; i++){

			strTag.Format(strStr, i+1);
			strColor.Format("%c%c", 3, color_map[i]);
			strName.Replace(strTag, strColor);
		}
	}
	CString strColorName;
	strColorName.Format("%c%c<%s%c%c>", 3,2, strName, 3, 2);

	return strColorName;*/
}
CString CChatView::RemoveColorCodes(CString strName)
{

	CString strTag;

	for(int i = 255; i >= 0; i--){

		strTag.Format("%c%c", 3, i);
		strName.Replace(strTag, "");
	}

	return strName;
}

void CChatView::RemoveColorName(CString strName, CString strStr)
{

	CString strColorName = MakeColorName(strName, strStr); //, m_nServerType);
	if(strColorName.GetLength()){

		if(!m_aMCMAMap.RemoveKey(strColorName)){
			
			TRACE("Error removing %s/%s from colormap\n", strColorName, strName);
		}
	}
}

void CChatView::OnChatChannelstatistics()
{

	MX_USERINFO me;
	m_aUsers.Lookup(GetDocument()->m_strName, me);

	time_t t;
	time(&t);
	double elapsed = difftime(t,(time_t)me.cJoinTime.GetTime());
	if(elapsed == 0) elapsed = 0.00001f;

	double mps = (float)m_dwMessages/elapsed;

	CString strStat;
    strStat.Format(IDS_CHANNELSTATS1, GetDocument()->m_strRoomShort);
	WriteText(strStat, g_sSettings.GetRGBJoin(), 0, 0, TRUE);

	MX_USERINFO user;


	POSITION pos = m_aUsers.GetStartPosition();
	while(pos){

		m_aUsers.GetNextAssoc(pos, strStat, user);
		if(user.wUserLever == USER_HOST){

			break;
		}
	}
	if(user.wUserLever != USER_HOST){

		user.strUser		= "unknown";
		user.strHostname	= "unknown";
	}
	/*
	+ Joined at %s\n
	+ %u users\n
	+ %u Messages Total\n
	+ %u Messages by you (%d%%)\n
	+ Lurkmeter: %s\n
	+ Messages/Second: %.02f\n
	+ Host is %s\n
	+ on %s\n
	*/
	strStat.Format(IDS_CHANNELSTATS2,
				   me.cJoinTime.Format("%A, %B %d, %Y, %H:%M:%S"),
				   m_aUsers.GetSize(),
				   m_dwMessages,
				   m_dwYourMessages,
				   m_dwMessages > 0 ? (int)((float)m_dwYourMessages/(float)m_dwMessages*100.f) : 0,
				   GetLurkMeter(),
				   mps,
				   user.strUser,
				   user.strHostname);
	WriteText(strStat, g_sSettings.GetRGBNormalMsg(), 0, 0, TRUE);
	strStat.LoadString(IDS_CHANNELSTATS3);
	WriteText(strStat, g_sSettings.GetRGBPart(), 0, 0, TRUE);

}

CString CChatView::GetLurkMeter(void)
{

	CString strLurk;
	MX_USERINFO me;
	m_aUsers.Lookup(GetDocument()->m_strName, me);

	time_t t;
	time(&t);
	double elapsed = difftime(t,(time_t)me.cJoinTime.GetTime());
	if(elapsed == 0) elapsed = 0.00001f;

#ifdef _DEBUG
	double mps = (float)(m_dwMessages)/elapsed;
#else
	double mps = (float)(m_dwMessages-m_dwYourMessages)/elapsed;
#endif
	if(mps > 0)	strLurk.Format(IDS_LURKMETER1, log(1/mps)-1);   //mps is of course > 0 ;)
	else strLurk.LoadString(IDS_LURKMETER2);
		
	return strLurk;
}

void CChatView::OnDisplayinchannelLurkmeter()
{

	CString str;
	str.Format(IDS_LURKMETER, GetLurkMeter());
	Input(str);
}


void CChatView::OnDisplayinchannelStatistics()
{

	MX_USERINFO me;
	m_aUsers.Lookup(GetDocument()->m_strName, me);

	time_t t;
	time(&t);
	double elapsed = difftime(t,(time_t)me.cJoinTime.GetTime());
	if(elapsed == 0) elapsed = 0.00001f;

	double mps = (float)m_dwMessages/elapsed;

	CString strStat;
	/*
	/me Joined room at %s  | 
	%u users in this room  |  
	%u Messages total, 
	%u messages 
	(%d%%) by myself  |  
	Lurkmeter: %s  |  
	Messages/Second: %.02f
	*/

	//Util::GetBaseName(GetDocument()->m_strName, TRUE),

	strStat.Format(IDS_DISPLAY_CHANNELSTAT,
				   me.cJoinTime.Format("%A, %B %d, %Y, %H:%M:%S"),
				   m_aUsers.GetSize(),
				   m_dwMessages,
				   m_dwYourMessages,
				   m_dwMessages > 0 ? (int)((float)m_dwYourMessages/(float)m_dwMessages*100.f) : 0,
				   GetLurkMeter(),
				   mps);
	m_mxClient.SendText(strStat);
}

void CChatView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{

	if(bActivate && pActivateView == this){

		if(m_pStatusBar){

			CString strText;
			
			m_pStatusBar->SetPaneText(2, GetDocument()->m_strRoomShort);

			strText.Format(IDS_AVERAGE_LAG, m_dwAvLag);
			COLORREF cr = 0;
			if(m_dwAvLag <= 500) cr = RGB(0, 100, 0);
			else if((m_dwAvLag > 500) && (m_dwAvLag <= 1000)) cr = 0;
			else if((m_dwAvLag > 1000) && (m_dwAvLag < 1500)) cr = RGB(255, 192, 64);
			else cr = RGB(200, 0, 0);
			m_pStatusBar->SetPaneText(3, strText, cr);

			strText.Format(IDS_USERS, m_lcUsers.GetItemCount());
			m_pStatusBar->SetPaneText(4, strText);
		}
	}
	else if(m_bHasJoined && pDeactiveView == this){
	    
		if(g_sSettings.GetSepOnSwitch() && m_bMsgAfterLastSep){

			WriteText("-----------", g_sSettings.GetRGBPend(), g_sSettings.GetRGBBg(), TRUE, TRUE, FALSE);
			m_bMsgAfterLastSep = FALSE;
		}
	}
	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}


void CChatView::OnDisplayinchannelOnlineTime()
{

	if(!m_mxClient.m_bListen) return;

	CString strOnline;
	strOnline.Format("Online: %s", g_sSystem.GetOnlineString());
	m_mxClient.SendText(strOnline);
}

void CChatView::OnDblclickUserlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	if(m_lcUsers.GetSelectedCount()){

		CString strUser = m_lcUsers.GetItemText(m_lcUsers.GetNextItem(-1, LVNI_SELECTED), 0);
		m_eInput.InsertText(strUser);
	}
	
	*pResult = 0;
}

BOOL CChatView::PreTranslateMessage(MSG* pMsg) 
{

	m_ToolTip.RelayEvent(pMsg);
	
	return CFormView::PreTranslateMessage(pMsg);
}

void CChatView::MakeUserTool(MX_USERINFO& user, CString& strToolTip)
{

	CIni ini;
	ini.SetIniFileName(g_sSettings.GetIniFile());

	CString strImage;
	int nCnt = ini.GetValue("UserToolTips", "UserImageCnt", 0);
	
	CString strLabel, strName;
	
	for(int i = 0; i < nCnt; i++){

		strLabel.Format("UserName_%d", i);
		strName = ini.GetValue("UserToolTips", strLabel, "");

		if(strName.GetLength() && Util::WildcardMatch(user.strUser, strName)){

			strLabel.Format("UserImage_%d", i);
			strImage = ini.GetValue("UserToolTips", strLabel, "");
			break;
		}
	}
	if(i >= nCnt){

		strImage = g_sSettings.GetUserlistDefaultImage();
	}

	if(strImage.IsEmpty()){

		strLabel = " ";
	}
	else{
		
		strLabel.Format("<bmp file=\"%s\" mask=\"#FF00FF\">", strImage);
	}

    strToolTip.Replace("%USERIMAGE%", strLabel);

	strImage.Format("<bmp file=\"%s\" mask=\"#FF00FF\">", g_sSettings.GetUserModeBmp(user.wUserLever));
	strToolTip.Replace("%USERMODEICON%", strImage);

	strToolTip.Replace("%NAME%", Util::GetBaseName(user.strUser, TRUE));
	strToolTip.Replace("%RAWNAME%", user.strUser);
	strToolTip.Replace("%FILES%", Util::Formatint(user.dwNumFiles));
	strToolTip.Replace("%LINE%", Util::FormatLine(user.wLineType));
	strToolTip.Replace("%IP%", user.strRealIP);
	strToolTip.Replace("%HOSTNAME%", user.strHostname);
	strToolTip.Replace("%LASTMSG%", user.strLastMsg.GetLength() > 30 ? user.strLastMsg.Left(30) + "..." : user.strLastMsg);
	strToolTip.Replace("%NUMMSGS%", Util::Formatint(user.dwNumMsg));
	strToolTip.Replace("%IDLETIME%", Util::GetIdleTime(user.dwIdleTime));
	strToolTip.Replace("%STAYTIME%", Util::GetStayTime(user.dwStayTime));
	strToolTip.Replace("%JOINTIME%", user.cJoinTime.Format(g_sSettings.GetTimeFmt() ? "%I:%M %p, %m/%d/%y": "%H:%M:%S, %m/%d/%y"));
	strToolTip.Replace("%ROOMNAME%", GetDocument()->m_strRoom);
	strToolTip.Replace("%ROOMSHORT%", GetDocument()->m_strRoomShort);
	
	for(int i = 0; i < g_aPlugins.GetSize() && m_bHasJoined; i++){

		PLUGIN->OnToolTipPrepare((DWORD)m_hWnd, FALSE, &strToolTip);
	}
}

void CChatView::NotifyDisplayTooltip(NMHDR * pNMHDR, LRESULT * result)
{

	*result = 0;
    NM_PPTOOLTIP_DISPLAY * pNotify = (NM_PPTOOLTIP_DISPLAY*)pNMHDR;

	if(pNotify->hwndTool == m_rChat.m_hWnd){

		CPoint pt = *pNotify->pt;
		ScreenToClient(&pt);
		CRichFragment* pFrag = m_rChat.PointToFrag(pt);
		if(pFrag){

			if(pFrag->m_pElement->m_nType == retName){

				CString strNickname;
				if(!m_aUserNoID.Lookup(pFrag->m_pElement->m_sText, strNickname)){

					if(!m_aUserHalfID.Lookup(pFrag->m_pElement->m_sText, strNickname)){

						strNickname = pFrag->m_pElement->m_sText;
					}
				}
				MX_USERINFO user;
				if(m_aUsers.Lookup(strNickname, user)){

					pNotify->ti->sTooltip = g_sSettings.GetUserTool();
					MakeUserTool(user, pNotify->ti->sTooltip);
				}
			} // if(pFrag->m_pElement->m_nType == retName)
		}
	}
	else if(pNotify->hwndTool == m_lcUsers.m_hWnd){

		CRect rcCtrl, rect;
		CPoint pt = *pNotify->pt;

		m_lcUsers.GetWindowRect(&rcCtrl);
		pt -= rcCtrl.TopLeft();
		LVHITTESTINFO li;
		li.pt = pt;
		int nItem = m_lcUsers.SubItemHitTest(&li);
		int nSubItem = li.iSubItem;
		UINT nFlags =   li.flags;
		
		if(nFlags & LVHT_ONITEM){
			
			pNotify->ti->nEffect = g_sSettings.GetUserToolEffect();
			pNotify->ti->nGranularity = g_sSettings.GetUserToolGran();
			pNotify->ti->nTransparency = g_sSettings.GetUserToolTrans();
			pNotify->ti->crBegin = g_sSettings.GetUserToolBg1();
			pNotify->ti->crMid   = g_sSettings.GetUserToolBg2();
			pNotify->ti->crEnd   = g_sSettings.GetUserToolBg3();

			CString strUser = m_lcUsers.GetItemText(li.iItem, 0);
			MX_USERINFO user;
			if(m_aUsers.Lookup(strUser, user) == FALSE){

				return;
			}			
			pNotify->ti->sTooltip = g_sSettings.GetUserTool();

			MakeUserTool(user, pNotify->ti->sTooltip);
		} 
	}
	else{
   
		MX_USERINFO me;
		m_aUsers.Lookup(GetDocument()->m_strName, me);

		time_t t;
		time(&t);
		double elapsed = difftime(t,(time_t)me.cJoinTime.GetTime());
		if(elapsed == 0) elapsed = 0.00001f;

		double mps = (float)m_dwMessages/elapsed;

		pNotify->ti->sTooltip = g_sSettings.GetToolRoom();

		CString strImage;
		strImage.Format("<icon idres=%d width=16 height=16>", IDI_ROOM);

		CIni ini;
		ini.SetIniFileName(g_sSettings.GetIniFile());
		pNotify->ti->sTooltip.Replace("%ROOMICON%", strImage);

		int nCnt = ini.GetValue("ChannelToolTips", "ChannelImageCnt", 0);
		
		CString strLabel, strChannel;
		
		for(int i = 0; i < nCnt; i++){

			strLabel.Format("ChannelName_%d", i);
			strChannel = ini.GetValue("ChannelToolTips", strLabel, "");

			if(strChannel.GetLength() && Util::WildcardMatch(GetDocument()->m_strRoom, strChannel)){

				strLabel.Format("ChannelImage_%d", i);
				strImage = ini.GetValue("ChannelToolTips", strLabel, "");
				break;
			}
		}
		if(i >= nCnt){

			strImage = g_sSettings.GetChannelDefaultImage();
		}

		if(strImage.IsEmpty()){

			strLabel = " ";
		}
		else{

			strLabel.Format("<bmp file=\"%s\" mask=#FF00FF>", strImage);
		}



		pNotify->ti->sTooltip.Replace("%ROOMIMAGE%", strLabel);

		pNotify->ti->sTooltip.Replace("%NAME%", Util::GetBaseName(GetDocument()->m_strName));
		pNotify->ti->sTooltip.Replace("%RAWNAME%", GetDocument()->m_strName);
		pNotify->ti->sTooltip.Replace("%TOTALMSGS%", Util::Formatint(m_dwMessages));
		pNotify->ti->sTooltip.Replace("%YOURMSGS%", Util::Formatint(m_dwYourMessages));
		pNotify->ti->sTooltip.Replace("%AVLAG%", Util::Formatint(m_dwAvLag));
		pNotify->ti->sTooltip.Replace("%NUMUSERS%", Util::Formatint(m_lcUsers.GetItemCount()));
		pNotify->ti->sTooltip.Replace("%MSGPERSEC%", Util::FormatFloat((float)mps));
		pNotify->ti->sTooltip.Replace("%JOINTIME%", me.cJoinTime.Format(g_sSettings.GetTimeFmt() ? "%I:%M %p, %m/%d/%y": "%H:%M:%S, %m/%d/%y"));
		pNotify->ti->sTooltip.Replace("%ROOMNAME%", GetDocument()->m_strRoom);
		pNotify->ti->sTooltip.Replace("%ROOMSHORT%", GetDocument()->m_strRoomShort);
		pNotify->ti->sTooltip.Replace("%SERVERBRAND%", m_mxClient.m_strServerString);
		pNotify->ti->sTooltip.Replace("%SERVERVER%", m_mxClient.m_strServerVer);
		pNotify->ti->sTooltip.Replace("%JOINCOUNT%", Util::Formatint(ini.GetValue("LastJoins", GetDocument()->m_strRoomShort + "_Count", 0)));

		pNotify->ti->nEffect = g_sSettings.GetToolEffect();
		pNotify->ti->nGranularity = g_sSettings.GetToolGran();
		pNotify->ti->nTransparency = g_sSettings.GetToolTrans();
		pNotify->ti->crBegin = g_sSettings.GetToolBg1();
		pNotify->ti->crMid   = g_sSettings.GetToolBg2();
		pNotify->ti->crEnd   = g_sSettings.GetToolBg3();

		for(int i = 0; i < g_aPlugins.GetSize() && m_bHasJoined; i++){

			PLUGIN->OnToolTipPrepare((DWORD)m_hWnd, TRUE, &pNotify->ti->sTooltip);
		}

#ifdef _DEBUG_TOOLTIP
		CString strDebug;
		strDebug.Format("<br><b>Debug:</b><br>m_nRetries: %d<br>m_bHasJoined: %d<br>m_bHideSystem: %d<br>m_nServerType: %d",
			m_nRetries, m_bHasJoined, m_bHideSystem, m_nServerType);
		pNotify->ti->sTooltip.Append(strDebug);
#endif

	}
}

LRESULT CChatView::OnPluginQueryInfo(WPARAM wParam, LPARAM lParam)
{

	if(lParam == 0) return 1;

	EXT_MSG_STRUCT* msg = (EXT_MSG_STRUCT*)lParam;
	
	LRESULT lResult = 1;

	switch(wParam){

		// Get Average lag in milliseconds
		// DWORD dwLag = (DWORD)QueryInformation(RMX_GETAVERAGELAG, dwRoomID, 0)
		case RMX_GETAVERAGELAG:
			lResult = (LRESULT)m_dwAvLag;
			break;
		// Get Number of total messages recieved in room
		// DWORD dwNum = (DWORD)QueryInformation(RMX_GETRECVMSGS, dwRoomID, 0)
		case RMX_GETRECVMSGS:
			lResult = (LRESULT)m_dwMessages;
			break;

		// Get Number of total messages sent to room
		// DWORD dwNum = (DWORD)QueryInformation(RMX_GETSENTMSGS, dwRoomID, 0)
		case RMX_GETSENTMSGS:
			lResult = (LRESULT)m_dwYourMessages;
			break;
	}
	return lResult;
}

void CChatView::OnUserlistDetailedview()
{
	// LVS_ICON LVS_LIST LVS_REPORT LVS_SMALLICON
	m_lcUsers.ModifyStyle(LVS_ICON|LVS_REPORT|LVS_SMALLICON, LVS_REPORT|LVS_NOLABELWRAP , 0);
}

void CChatView::OnUpdateUserlistDetailedview(CCmdUI *pCmdUI)
{
	
	pCmdUI->SetRadio((m_lcUsers.GetStyle() & LVS_REPORT) == LVS_REPORT );
}

void CChatView::OnUserlistShortview()
{

	m_lcUsers.ModifyStyle(LVS_ICON|LVS_REPORT|LVS_SMALLICON, LVS_LIST|LVS_NOLABELWRAP , 0);
}

void CChatView::OnUpdateUserlistShortview(CCmdUI *pCmdUI)
{
	pCmdUI->SetRadio((m_lcUsers.GetStyle() & LVS_LIST) == LVS_LIST);
}


void CChatView::OnLvnItemchangedUserlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	RECT rc;
	m_lcUsers.GetItemRect(pNMLV->iItem, &rc, LVIR_BOUNDS );
	m_lcUsers.InvalidateRect(&rc, TRUE);
	*pResult = 0;
}

void CChatView::UpdateUserStats(const CString strName, const CString strMsg)
{

    MX_USERINFO user;
	BOOL bSuccess = m_aUsers.Lookup(strName, user);
	if(!bSuccess)
		if(m_aUserHalfID.Lookup(Util::GetBaseName(strName, TRUE), user.strUser))
			bSuccess = m_aUsers.Lookup(user.strUser, user);
	if(!bSuccess)
		if(m_aUserNoID.Lookup(Util::GetBaseName(strName, TRUE, TRUE), user.strUser))
			bSuccess = m_aUsers.Lookup(user.strUser, user);

	if(bSuccess){

		user.strLastMsg = strMsg;
		user.dwIdleTime = GetTickCount();
		user.dwNumMsg++;
		m_aUsers.SetAt(user.strUser, user);
	}
}


void CChatView::OnEnChangeInput()
{

	CString strText;
	m_eInput.GetWindowText(strText);
	m_pgLetters.SetPos(strText.GetLength());
}
