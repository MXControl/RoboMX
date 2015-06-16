; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChannelView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Metis3.h"
LastPage=0

ClassCount=23
Class1=CMetis3App
Class2=CMetis3Doc
Class3=CMetis3View
Class4=CMainFrame

ResourceCount=21
Resource1=IDR_CHANNEL
Resource2=IDR_CHAT
Resource3=IDR_LIST
Class5=CAboutDlg
Class6=CConnectionDlg
Class7=CMyEdit
Resource4=IDD_METIS3_FORM
Class8=CRenameDlg
Class9=CMyListCtrl
Resource5=IDR_FULLSCREEN
Resource6=IDD_CONNECT
Class10=CChannelView
Class11=CChannelDoc
Resource7=IDR_LISTRMENU
Resource8=IDR_AWAY
Resource9=IDD_ABOUTBOX
Resource10=IDD_GENERAL
Class12=CServerDoc
Class13=CServerView
Resource11=IDD_CHANNELLIST
Resource12=IDD_RENAME
Class14=CColorStatusBar
Class15=CSettingsDlg
Class16=CCfgDlg
Resource13=IDR_USER
Resource14=IDD_MESSAGES
Class17=CGeneralCfg
Class18=CColorCfg
Resource15=IDR_MAINFRAME
Resource16=IDD_SFX_EDIT
Class19=CMessageCfg
Resource17=IDD_SOUND
Class20=CSfxCfg
Resource18=IDD_COLORS
Class21=CSfxEdit
Resource19=IDD_SETTINGS
Class22=CInputRequest
Resource20=IDD_INPUT_REQUEST
Class23=CFunCfg
Resource21=IDD_FUN

[CLS:CMetis3App]
Type=0
HeaderFile=Metis3.h
ImplementationFile=Metis3.cpp
Filter=N

[CLS:CMetis3Doc]
Type=0
HeaderFile=Metis3Doc.h
ImplementationFile=Metis3Doc.cpp
Filter=N

[CLS:CMetis3View]
Type=0
HeaderFile=Metis3View.h
ImplementationFile=Metis3View.cpp
Filter=W
BaseClass=CFormView
VirtualFilter=VWC
LastObject=AFX_IDS_SCNEXTWINDOW


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=W
BaseClass=CMDIFrameWnd
VirtualFilter=fWC
LastObject=ID_CHAT_TEXTTRICKS_3DBUTTONSNORMAL




[CLS:CAboutDlg]
Type=0
HeaderFile=Metis3.cpp
ImplementationFile=Metis3.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=12
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308481
Control3=IDC_STATIC,static,1342308353
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_MYFILE_NEW
Command2=ID_CHANNEL_CHANNELLIST
Command3=ID_RENAME
Command4=IDR_START_NODESERVER
Command5=ID_APP_EXIT
Command6=ID_VIEW_CHANNELBAR
Command7=ID_VIEW_TOOLBAR
Command8=ID_VIEW_STATUS_BAR
Command9=ID_VIEW_OPTIONS
Command10=ID_APP_ABOUT
CommandCount=10

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_CHANNEL_CHANNELLIST
Command2=ID_MYFILE_NEW
Command3=ID_CHATROOM_DISPLAYINCHANNEL_SYSTEMUPTIME
Command4=ID_CHATROOM_DISPLAYINCHANNEL_SYSTEMINFO
Command5=ID_CHATROOM_DISPLAYINCHANNEL_AVERAGELAG
Command6=ID_NEXT_PANE
Command7=ID_PREV_PANE
Command8=ID_CHATROOM_DISPLAYINCHANNEL_WINAMPSONG
CommandCount=8

[DLG:IDD_METIS3_FORM]
Type=1
Class=CMetis3View
ControlCount=7
Control1=IDC_INPUT,edit,1350631552
Control2=IDC_USERLIST,SysListView32,1350631445
Control3=IDC_LEAVE,button,1342242816
Control4=IDC_STATIC_SYSOUT,static,1073741831
Control5=IDC_STATIC_CHAT,static,1073741831
Control6=IDC_SPLITTER_1,static,1073741831
Control7=IDC_SPLITTER_2,static,1073741831

[DLG:IDD_CONNECT]
Type=1
Class=CConnectionDlg
ControlCount=14
Control1=IDC_STATIC_ROOM,static,1342308352
Control2=IDC_ROOM,combobox,1344340226
Control3=IDC_STATIC_USERNAME,static,1342308352
Control4=IDC_USERNAME,edit,1350631552
Control5=IDC_STATIC_FILES,static,1342308352
Control6=IDC_FILES,edit,1350639744
Control7=IDC_STATIC_LINE,static,1342308352
Control8=IDC_LINE,combobox,1344339971
Control9=IDOK,button,1342242817
Control10=IDCANCEL,button,1342242816
Control11=IDC_STATIC,static,1342177283
Control12=IDC_STATIC,button,1476395008
Control13=IDC_STATIC_USERSETUP,button,1342177287
Control14=IDC_CLEAR,button,1342242816

[CLS:CConnectionDlg]
Type=0
HeaderFile=ConnectionDlg.h
ImplementationFile=ConnectionDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_USENODE
VirtualFilter=dWC

[CLS:CMyEdit]
Type=0
HeaderFile=MyEdit.h
ImplementationFile=MyEdit.cpp
BaseClass=CEdit
Filter=W
VirtualFilter=WC

[DLG:IDD_RENAME]
Type=1
Class=CRenameDlg
ControlCount=9
Control1=IDC_STATIC_NAME,static,1342308352
Control2=IDC_NAME,edit,1350631552
Control3=IDC_STATIC_FILES,static,1342308352
Control4=IDC_FILES,edit,1350639744
Control5=IDC_STATIC_LINE,static,1342308352
Control6=IDC_LINE,combobox,1344339971
Control7=IDC_STATIC,static,1342177283
Control8=IDOK,button,1342242817
Control9=IDCANCEL,button,1342242816

[CLS:CRenameDlg]
Type=0
HeaderFile=RenameDlg.h
ImplementationFile=RenameDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_NAME
VirtualFilter=dWC

[CLS:CMyListCtrl]
Type=0
HeaderFile=MyListCtrl.h
ImplementationFile=MyListCtrl.cpp
BaseClass=CListCtrl
Filter=W
VirtualFilter=FWC

[MNU:IDR_CHANNEL]
Type=1
Class=CMainFrame
Command1=ID_MYFILE_NEW
Command2=ID_FILE_CLOSE
Command3=ID_CHANNEL_CHANNELLIST
Command4=IDR_START_NODESERVER
Command5=ID_RENAME
Command6=ID_APP_EXIT
Command7=ID_CHATROOM_SELECTALL
Command8=ID_CHATROOM_COPY
Command9=ID_CHATROOM_DISPLAYINCHANNEL_WINAMPSONG
Command10=ID_CHATROOM_DISPLAYINCHANNEL_SYSTEMUPTIME
Command11=ID_CHATROOM_DISPLAYINCHANNEL_SYSTEMINFO
Command12=ID_CHATROOM_DISPLAYINCHANNEL_AVERAGELAG
Command13=ID_CHATROOM_TEXTTRICKS_HACKER
Command14=ID_CHATROOM_TEXTTRICKS_BUBBLES
Command15=ID_CHATROOM_TEXTTRICKS_BOX
Command16=ID_CHAT_TEXTTRICKS_3DBUTTONSNORMAL
Command17=ID_CHAT_TEXTTRICKS_3DBUTTONSACTION
Command18=ID_CHATROOM_ASCIIARTDESIGN
Command19=ID_CHATROOM_AWAYCONTROL_SETBACK
Command20=ID_CHATROOM_AWAYCONTROL_SETSILENTBACK
Command21=ID_CHATROOM_SEARCHONGOOGLE
Command22=ID_CHATROOM_QUOTE
Command23=ID_CHATROOM_COPYROOMNAME
Command24=ID_CHATROOM_CLEARSCREEN
Command25=ID_USERLIST_SENDMESSAGE
Command26=ID_USERLIST_IGNORE
Command27=ID_USERLIST_UNIGNORE
Command28=ID_USERLIST_COPYUSERNAME
Command29=ID_USERLIST_KICK
Command30=ID_USERLIST_BAN
Command31=ID_USERLIST_KICKBAN
Command32=ID_USERLIST_UNBAN
Command33=ID_USERLIST_REDIRECT
Command34=ID_USERLIST_ADDADMIN
Command35=ID_USERLIST_REMOVEADMIN
Command36=ID_USERLIST_PRINTUSERSTAT
Command37=ID_USERLIST_PRINTIP
Command38=ID_USERLIST_READUSERMESSAGE
Command39=ID_USERLIST_CUSTOMIZETHISMENU
Command40=ID_VIEW_FULL_SCREEN
Command41=ID_VIEW_CHANNELBAR
Command42=ID_VIEW_TOOLBAR
Command43=ID_VIEW_STATUS_BAR
Command44=ID_VIEW_OPTIONS
Command45=AFX_IDS_SCNEXTWINDOW
Command46=ID_WINDOW_CASCADE
Command47=ID_WINDOW_TILE_HORZ
Command48=ID_WINDOW_ARRANGE
Command49=ID_APP_ABOUT
CommandCount=49

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_MYFILE_NEW
Command2=ID_RECONNECT
Command3=ID_FILE_CLOSE
Command4=ID_CHANNEL_CHANNELLIST
Command5=ID_RENAME
Command6=AFX_IDS_SCNEXTWINDOW
Command7=ID_APP_ABOUT
CommandCount=7

[DLG:IDD_CHANNELLIST]
Type=1
Class=CChannelView
ControlCount=8
Control1=IDC_CHANNELS,SysListView32,1350631453
Control2=IDC_REFRESH,button,1476460544
Control3=IDC_CLEAR_REFRESH,button,1476460544
Control4=IDC_STATIC_SEARCH,static,1342308352
Control5=IDC_SEARCH,edit,1350631552
Control6=IDC_STATIC_MESSAGES,static,1342308352
Control7=IDC_STATIC_ROOMS,static,1342308354
Control8=IDC_SELECTION_NOSCROLL,button,1342242819

[CLS:CChannelView]
Type=0
HeaderFile=ChannelView.h
ImplementationFile=ChannelView.cpp
BaseClass=CFormView
Filter=M
VirtualFilter=VWC
LastObject=ID_MYFILE_NEW

[CLS:CChannelDoc]
Type=0
HeaderFile=ChannelDoc.h
ImplementationFile=ChannelDoc.cpp
BaseClass=CDocument
Filter=N
LastObject=IDC_REFRESH

[MNU:IDR_LIST]
Type=1
Class=CMainFrame
Command1=ID_MYFILE_NEW
Command2=ID_CHANNEL_CHANNELLIST
Command3=ID_RENAME
Command4=IDR_START_NODESERVER
Command5=ID_APP_EXIT
Command6=ID_VIEW_FULL_SCREEN
Command7=ID_VIEW_CHANNELBAR
Command8=ID_VIEW_TOOLBAR
Command9=ID_VIEW_STATUS_BAR
Command10=ID_VIEW_OPTIONS
Command11=AFX_IDS_SCNEXTWINDOW
Command12=ID_WINDOW_CASCADE
Command13=ID_WINDOW_TILE_HORZ
Command14=ID_WINDOW_ARRANGE
Command15=ID_APP_ABOUT
CommandCount=15

[MNU:IDR_LISTRMENU]
Type=1
Class=CChannelView
Command1=ID_MYFILE_NEW
Command2=ID_POPUP_LOOPBACKJOIN
Command3=ID_POPUP_COPYROOMNAME
Command4=ID_POPUP_COPYTOPIC
CommandCount=4

[MNU:IDR_USER]
Type=1
Class=CMetis3View
Command1=ID_USERLIST_SENDMESSAGE
Command2=ID_USERLIST_IGNORE
Command3=ID_USERLIST_UNIGNORE
Command4=ID_USERLIST_COPYUSERNAME
Command5=ID_USERLIST_KICK
Command6=ID_USERLIST_BAN
Command7=ID_USERLIST_KICKBAN
Command8=ID_USERLIST_UNBAN
Command9=ID_USERLIST_REDIRECT
Command10=ID_USERLIST_ADDADMIN
Command11=ID_USERLIST_REMOVEADMIN
Command12=ID_USERLIST_PRINTUSERSTAT
Command13=ID_USERLIST_PRINTIP
Command14=ID_USERLIST_READUSERMESSAGE
Command15=ID_USERLIST_CUSTOMIZETHISMENU
CommandCount=15

[DLG:IDD_SETTINGS]
Type=1
Class=CSettingsDlg
ControlCount=5
Control1=IDC_CAT,SysListView32,1350688908
Control2=IDOK,button,1342242817
Control3=IDC_APPLY,button,1342242816
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC_CLIENT,static,1073741824

[CLS:CServerDoc]
Type=0
HeaderFile=ServerDoc.h
ImplementationFile=ServerDoc.cpp
BaseClass=CDocument
Filter=N

[CLS:CServerView]
Type=0
HeaderFile=ServerView.h
ImplementationFile=ServerView.cpp
BaseClass=CEditView
Filter=C
LastObject=ID_USERLIST_SENDMESSAGE

[TB:IDR_FULLSCREEN]
Type=1
Class=?
Command1=ID_VIEW_FULL_SCREEN
CommandCount=1

[MNU:IDR_CHAT]
Type=1
Class=?
Command1=ID_CHATROOM_SELECTALL
Command2=ID_CHATROOM_COPY
Command3=ID_CHATROOM_DISPLAYINCHANNEL_WINAMPSONG
Command4=ID_CHATROOM_DISPLAYINCHANNEL_SYSTEMUPTIME
Command5=ID_CHATROOM_DISPLAYINCHANNEL_SYSTEMINFO
Command6=ID_CHATROOM_DISPLAYINCHANNEL_AVERAGELAG
Command7=ID_CHATROOM_TEXTTRICKS_HACKER
Command8=ID_CHATROOM_TEXTTRICKS_BUBBLES
Command9=ID_CHATROOM_TEXTTRICKS_BOX
Command10=ID_CHAT_TEXTTRICKS_3DBUTTONSNORMAL
Command11=ID_CHAT_TEXTTRICKS_3DBUTTONSACTION
Command12=ID_CHATROOM_ASCIIARTDESIGN
Command13=ID_CHATROOM_AWAYCONTROL_SETBACK
Command14=ID_CHATROOM_AWAYCONTROL_SETSILENTBACK
Command15=ID_CHATROOM_SEARCHONGOOGLE
Command16=ID_CHATROOM_QUOTE
Command17=ID_CHATROOM_COPYROOMNAME
Command18=ID_CHATROOM_CLEARSCREEN
CommandCount=18

[CLS:CColorStatusBar]
Type=0
HeaderFile=ColorStatusBar.h
ImplementationFile=ColorStatusBar.cpp
BaseClass=CStatusBarCtrl
Filter=W
LastObject=IDC_STATIC_CLIENT

[CLS:CSettingsDlg]
Type=0
HeaderFile=SettingsDlg.h
ImplementationFile=SettingsDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=AFX_IDS_SCNEXTWINDOW

[CLS:CCfgDlg]
Type=0
HeaderFile=CfgDlg.h
ImplementationFile=CfgDlg.cpp
BaseClass=CDialog
Filter=D

[DLG:IDD_GENERAL]
Type=1
Class=CGeneralCfg
ControlCount=26
Control1=IDC_STATIC,button,1342177287
Control2=IDC_CHANNELNAME,edit,1350631552
Control3=IDC_ADDCHANNEL,button,1342242816
Control4=IDC_CHANNELLIST,listbox,1352728835
Control5=IDC_REMOVECHANNEL,button,1342242816
Control6=IDC_ALLCHANNELS,button,1342251011
Control7=IDC_STATIC_GREETING,static,1342308352
Control8=IDC_STATIC_LANG,button,1476395015
Control9=IDC_LANG,combobox,1478557955
Control10=IDC_STATIC_OPTIONS,button,1342177287
Control11=IDC_ENABLE_HISTORY,button,1476460547
Control12=IDC_LINES,button,1342242819
Control13=IDC_LINESNUM,edit,1350631552
Control14=IDC_HISTORDEPTH,edit,1484849280
Control15=IDC_TIMESTAMP,button,1342242819
Control16=IDC_GREETING,edit,1350631552
Control17=IDC_STATIC_PLUGINS,button,1476395015
Control18=IDC_PLUGINLIST,SysListView32,1484881921
Control19=IDC_REMOVE,button,1476460544
Control20=IDC_RELOAD_ALL,button,1476460544
Control21=IDC_UNLOAD_ALL,button,1476460544
Control22=IDC_CONFIGURE,button,1476460544
Control23=IDC_MINITRAY,button,1342242819
Control24=IDC_STATIC_TIMEFORMAT,button,1342177287
Control25=IDC_TIME,combobox,1344339971
Control26=IDC_PING,button,1342242819

[DLG:IDD_COLORS]
Type=1
Class=CColorCfg
ControlCount=66
Control1=IDC_STATIC_MESSAGE,button,1342177287
Control2=IDC_STATIC_NICKENC_MSG,static,1342308352
Control3=IDC_STATIC_FRONT_MSG,static,1342308352
Control4=IDC_MSG_FRONT,edit,1350631552
Control5=IDC_STATIC_END_MSG,static,1342308352
Control6=IDC_MSG_END,edit,1350631552
Control7=IDC_STATIC_NICKNAME_MSG,static,1342308352
Control8=IDC_ENC_MSG_COLOR,button,1342242816
Control9=IDC_STATIC_COLOR_ENC_MSG_PREV,static,1342308352
Control10=IDC_COLOR_MESSAGE,button,1342242816
Control11=IDC_STATIC_COLOR_MSG_PREV,static,1342308352
Control12=IDC_STATIC_ACTION,button,1342177287
Control13=IDC_STATIC_NICKENC_MSG2,static,1342308352
Control14=IDC_STATIC_FRONT_ACTION,static,1342308352
Control15=IDC_ACTION_FRONT,edit,1350631552
Control16=IDC_STATIC_END_ACTION,static,1342308352
Control17=IDC_ACTION_END,edit,1350631552
Control18=IDC_STATIC_NICKNAME_ACTION,static,1342308352
Control19=IDC_ENC_ACTION_COLOR,button,1342242816
Control20=IDC_STATIC_COLOR_ENG_ACTION_PREV,static,1342308352
Control21=IDC_COLOR_ACTION,button,1342242816
Control22=IDC_STATIC_COLOR_ACTION_PREV,static,1342308352
Control23=IDC_STATIC_MISCCOLOR,button,1342177287
Control24=IDC_TOPIC,button,1342242816
Control25=IDC_STATIC_TOPIC,static,1342308352
Control26=IDC_MOTD,button,1342242816
Control27=IDC_STATIC_MOTD,static,1342308352
Control28=IDC_JOIN,button,1342242816
Control29=IDC_STATIC_JOIN,static,1342308352
Control30=IDC_PART,button,1342242816
Control31=IDC_STATIC_PART,static,1342308352
Control32=IDC_BACKG,button,1342242816
Control33=IDC_STATIC_BG,static,1342308352
Control34=IDC_STATIC_FONT,button,1342177287
Control35=IDC_FONT,combobox,1344340227
Control36=IDC_STATIC_SIZE,static,1342308352
Control37=IDC_FONTSIZE,edit,1350631552
Control38=IDC_ENCODING,combobox,1344339971
Control39=IDC_BOLD,button,1342242819
Control40=IDC_ITALIC,button,1342242819
Control41=IDC_UNDERLINE,button,1342242819
Control42=IDC_STATIC_USERLIST,button,1342177287
Control43=IDC_LINE,button,1342242816
Control44=IDC_STATIC_LINE,static,1342308352
Control45=IDC_FILES,button,1342242816
Control46=IDC_STATIC_FILES,static,1342308352
Control47=IDC_NODEIP,button,1342242816
Control48=IDC_STATIC_NODEIP,static,1342308352
Control49=IDC_PORT,button,1342242816
Control50=IDC_STATIC_NODEPORT,static,1342308352
Control51=IDC_COLORACTIONNAME,button,1342242819
Control52=IDC_STATIC_SYSTEM,button,1342177287
Control53=IDC_PENDING,button,1342242816
Control54=IDC_STATIC_PENDING,static,1342308352
Control55=IDC_SUCCESS,button,1342242816
Control56=IDC_STATIC_SUCCESS,static,1342308352
Control57=IDC_ERROR,button,1342242816
Control58=IDC_STATIC_ERROR,static,1342308352
Control59=IDC_STATIC_BGIMAGE,button,1342177287
Control60=IDC_USE_BGIMAGE,button,1342242819
Control61=IDC_SELECT_IMAGE,button,1342242816
Control62=IDC_STATIC_IMAGEPATH,static,1342308864
Control63=IDC_STATIC_HILIGHT,button,1342177287
Control64=IDC_COLORFOCUS,button,1342242816
Control65=IDC_STATIC_FOCUS,static,1342308352
Control66=IDC_FOCUSCLR,button,1342242819

[CLS:CGeneralCfg]
Type=0
HeaderFile=GeneralCfg.h
ImplementationFile=GeneralCfg.cpp
BaseClass=CCfgDlg
Filter=D
LastObject=IDC_SOUNDEFFECT
VirtualFilter=dWC

[CLS:CColorCfg]
Type=0
HeaderFile=ColorCfg.h
ImplementationFile=ColorCfg.cpp
BaseClass=CCfgDlg
Filter=D
VirtualFilter=dWC
LastObject=IDC_FOCUSCLR

[MNU:IDR_AWAY]
Type=1
Class=CMainFrame
Command1=ID_AWAYCONTROL_ZZZZZZZZZZZZZZZZ
Command2=ID_AWAYCONTROL_WORK
Command3=ID_AWAYCONTROL_SLEEP
Command4=ID_AWAYCONTROL_FRESHAIR
Command5=ID_AWAYCONTROL_BATHROOM
Command6=ID_AWAYCONTROL_SMOKEBREAK
Command7=ID_AWAYCONTROL_STORE
Command8=ID_AWAYCONTROL_FOOD
Command9=ID_AWAYCONTROL_BEER
Command10=ID_AWAYCONTROL_TV
Command11=ID_AWAYCONTROL_NONEOFYOURBUISNESS
Command12=ID_AWAYCONTROL_ENTERREASON
CommandCount=12

[DLG:IDD_MESSAGES]
Type=1
Class=CMessageCfg
ControlCount=20
Control1=IDC_STATIC_QUICK,button,1342177287
Control2=IDC_MSG_ADD,edit,1350631552
Control3=IDC_MSG_ADD_BTN,button,1342242816
Control4=IDC_QUICKCMDS,listbox,1352728833
Control5=IDC_QUICKREM,button,1342242816
Control6=IDC_QUICKUP,button,1342242816
Control7=IDC_QUICKDOWN,button,1342242816
Control8=IDC_STATIC_JOINPART,button,1342177287
Control9=IDC_JOIN,edit,1350631552
Control10=IDC_PART,edit,1350631552
Control11=IDC_STATIC_JOIN,static,1342308352
Control12=IDC_STATIC_PART,static,1342308352
Control13=IDC_STATIC_PART_VAR,static,1342308352
Control14=IDC_STATIC_JOIN_VAR,static,1342308352
Control15=IDC_STATIC_WINAMP,button,1342177287
Control16=IDC_STATIC_MUSIC,static,1342308352
Control17=IDC_NOWLISTENING,edit,1350631552
Control18=IDC_STATIC_VIDEO,static,1342308352
Control19=IDC_NOW_WATCHING,edit,1350631552
Control20=IDC_STATIC_VARS,static,1342308352

[CLS:CMessageCfg]
Type=0
HeaderFile=MessageCfg.h
ImplementationFile=MessageCfg.cpp
BaseClass=CDialog
Filter=D

[DLG:IDD_SOUND]
Type=1
Class=CSfxCfg
ControlCount=23
Control1=IDC_STATIC_SFX,button,1342177287
Control2=IDC_ENABLE_SFX,button,1342242819
Control3=IDC_CONNECTED,button,1342242816
Control4=IDC_STATIC_CONNECT,static,1342312960
Control5=IDC_DISCONNECTED,button,1342242816
Control6=IDC_STATIC_DISCONNECTED,static,1342312960
Control7=IDC_TOPIC,button,1342242816
Control8=IDC_STATIC_TOPIC,static,1342312960
Control9=IDC_MOTD,button,1342242816
Control10=IDC_STATIC_MOTD,static,1342312960
Control11=IDC_REDIRECTED,button,1342242816
Control12=IDC_STATIC_REDIRECTED,static,1342312960
Control13=IDC_STARTUP,button,1342242816
Control14=IDC_STATIC_STARTUP,static,1342312960
Control15=IDC_SYSERROR,button,1342242816
Control16=IDC_STATIC_SYSERROR,static,1342312960
Control17=IDC_STATIC_CHATSOUNDS,button,1342177287
Control18=IDC_ENABLE_CSFX,button,1342242819
Control19=IDC_ADD,button,1342242816
Control20=IDC_EDIT,button,1342242816
Control21=IDC_DELETE,button,1342242816
Control22=IDC_PLAY,button,1342242816
Control23=IDC_SOUNDLIST,SysListView32,1350664341

[CLS:CSfxCfg]
Type=0
HeaderFile=SfxCfg.h
ImplementationFile=SfxCfg.cpp
BaseClass=CCfgDlg
Filter=D
VirtualFilter=dWC
LastObject=AFX_IDS_SCNEXTWINDOW

[DLG:IDD_SFX_EDIT]
Type=1
Class=CSfxEdit
ControlCount=9
Control1=IDC_STATIC,static,1342308352
Control2=IDC_TRIGGER,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_REACTION,edit,1350631552
Control5=IDC_BROWSE_SOUND,button,1342242816
Control6=IDOK,button,1342242817
Control7=IDCANCEL,button,1342242816
Control8=IDC_STATIC,button,1342177287
Control9=IDC_PLAY,button,1342242816

[CLS:CSfxEdit]
Type=0
HeaderFile=SfxEdit.h
ImplementationFile=SfxEdit.cpp
BaseClass=CDialog
Filter=D

[DLG:IDD_INPUT_REQUEST]
Type=1
Class=CInputRequest
ControlCount=6
Control1=IDC_STATIC_IR,button,1342177287
Control2=IDC_INPUT,edit,1352728580
Control3=IDC_CONVERT,button,1476460547
Control4=IDOK,button,1342242817
Control5=IDCANCEL,button,1342242816
Control6=IDC_STATIC,static,1342308352

[CLS:CInputRequest]
Type=0
HeaderFile=InputRequest.h
ImplementationFile=InputRequest.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_INPUT

[DLG:IDD_FUN]
Type=1
Class=CFunCfg
ControlCount=13
Control1=IDC_STATIC_RANDOMIZER,static,1342308352
Control2=IDC_STATIC_VENDORS,button,1342177287
Control3=IDC_VENDOR_ADD,edit,1350631552
Control4=IDC_BTN_VENDOR_ADD,button,1342242816
Control5=IDC_VENDORS,listbox,1352728835
Control6=IDC_STATIC_NAMES,button,1342177287
Control7=IDC_NAME_ADD,edit,1350631552
Control8=IDC_BTN_NAME_ADD,button,1342242816
Control9=IDC_NAMES,listbox,1352728835
Control10=IDC_STATIC_SUFFIXES,button,1342177287
Control11=IDC_SUFFIX_ADD,edit,1350631552
Control12=IDC_BTN_SUFFIX_ADD,button,1342242816
Control13=IDC_SUFFIXES,listbox,1352728835

[CLS:CFunCfg]
Type=0
HeaderFile=FunCfg.h
ImplementationFile=FunCfg.cpp
BaseClass=CCfgDlg
Filter=D
VirtualFilter=dWC
LastObject=ID_POPUP_LOOPBACKJOIN

