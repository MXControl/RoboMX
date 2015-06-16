/*
** Copyright (C) 2004 Bender
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
#include "MessageCfg.h"
#include "Settings.h"
#include ".\messagecfg.h"
#include "ExtWinampReport/wa_ipc.h"
#include "util.h"
#include "Tokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCfgMessage dialog

extern CSettings    g_sSettings;

CCfgMessage::CCfgMessage(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CCfgMessage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCfgMessage)
	m_strJoin = _T("");
	m_strWatch = _T("");
	m_strListen = _T("");
	m_strPart = _T("");
	//}}AFX_DATA_INIT
}


void CCfgMessage::DoDataExchange(CDataExchange* pDX)
{
	CCfgDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCfgMessage)
	DDX_Text(pDX, IDC_JOIN, m_strJoin);
	DDX_Text(pDX, IDC_NOW_WATCHING, m_strWatch);
	DDX_Text(pDX, IDC_NOWLISTENING, m_strListen);
	DDX_Text(pDX, IDC_PART, m_strPart);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC_WA, m_wndWAPlugin);
	DDX_Control(pDX, IDC_INSTALL, m_wndInstall);
}


BEGIN_MESSAGE_MAP(CCfgMessage, CCfgDlg)
	//{{AFX_MSG_MAP(CCfgMessage)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_JOIN, OnEnChangeJoin)
	ON_EN_CHANGE(IDC_PART, OnEnChangePart)
	ON_EN_CHANGE(IDC_NOWLISTENING, OnEnChangeNowlistening)
	ON_EN_CHANGE(IDC_NOW_WATCHING, OnEnChangeNowWatching)
	ON_BN_CLICKED(IDC_INSTALL, OnBnClickedInstall)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCfgMessage message handlers

BOOL CCfgMessage::OnInitDialog() 
{

	CCfgDlg::OnInitDialog();
	
	CString strTest;
	if(!Util::GetExtWinampVars(strTest)){

		m_wndWAPlugin.SetWindowText("Advanced Winamp Reportplugin not detected. Advanced Variables are NOT available :-(");
		m_wndInstall.EnableWindow(TRUE);
	}
	else{

		m_wndInstall.EnableWindow(TRUE);
		m_wndInstall.SetWindowText("&Test Plugin");
	}

	m_mToolTip.Create(this, FALSE);
	m_mToolTip.AddTool(GetDlgItem(IDC_JOIN), IDS_TOOL_JOINVARIABLES, 0, 0, 0);
	m_mToolTip.AddTool(GetDlgItem(IDC_PART), IDS_TOOL_PARTVARIABLES, 0, 0, 0);
	m_mToolTip.AddTool(GetDlgItem(IDC_NOWLISTENING), IDS_TOOL_WAVARIABLES, 0, 0, 0);
	m_mToolTip.AddTool(GetDlgItem(IDC_NOW_WATCHING), IDS_TOOL_WAVARIABLES, 0, 0, 0);

	m_strJoin = g_sSettings.GetJoin();
	m_strListen = g_sSettings.GetWinampMsg();
	m_strPart = g_sSettings.GetPart();
	m_strWatch = g_sSettings.GetVideoMsg();
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCfgMessage::Apply()
{
	UpdateData(TRUE);
	
	g_sSettings.SetJoin(m_strJoin);
	g_sSettings.SetWinampMsg(m_strListen);
	g_sSettings.SetPart(m_strPart);
	g_sSettings.SetVideoMsg(m_strWatch);
}



BOOL CCfgMessage::PreTranslateMessage( MSG *pMsg )
{

	/*if(pMsg->message == WM_MOUSEMOVE){

		POINT pt = pMsg->pt;    
		ScreenToClient(&pt);

		// this is the only function to call
		m_mToolTip.ShowToolTip((CPoint)pt);
	}*/
	m_mToolTip.RelayEvent(pMsg);

	return CCfgDlg::PreTranslateMessage(pMsg);
}

void CCfgMessage::OnEnChangeJoin()
{
	
	//m_mToolTip.ShowToolTip(IDC_JOIN);
}

void CCfgMessage::OnEnChangePart()
{

	//m_mToolTip.ShowToolTip(IDC_PART);
}

void CCfgMessage::OnEnChangeNowlistening()
{

	//m_mToolTip.ShowToolTip(IDC_NOWLISTENING);
}

void CCfgMessage::OnEnChangeNowWatching()
{

	//m_mToolTip.ShowToolTip(IDC_NOW_WATCHING);
}



void CCfgMessage::OnBnClickedInstall()
{

	CString strText;
	m_wndInstall.GetWindowText(strText);
	if(strText == "&Test Plugin"){

		TestPlugin();
	}
	else{

		if(AfxMessageBox("This will install the Advanced Winamp Playbackreport Plugin.\n"
						"This plugin enables advanced and more reliable Winamp playback report\n"
						"functionality to RoboMX.\n"
						"The installation process requires a restart of Winamp.\n"
						"Do you wish to continue?", MB_ICONQUESTION+MB_YESNO) == IDYES){


			Util::ShellExecuteWait(g_sSettings.GetWorkingDir() + "\\wa-extreport.exe", "");
			m_wndInstall.SetWindowText("&Test Plugin");

			HWND hwndWinamp = ::FindWindow("Winamp v1.x", NULL);

			if((hwndWinamp != NULL) && 
				AfxMessageBox("The Plugin has been installed. To enable the plugin Winamp needs to be restarted. Do you want RoboMX to restart Winamp for you now?", MB_ICONQUESTION+MB_YESNO) == IDYES){

				CWaitCursor wc;
				::SendMessage(hwndWinamp, WM_WA_IPC, 0, IPC_RESTARTWINAMP);
				hwndWinamp = NULL;
				while(hwndWinamp == NULL){
					
					Sleep(500);
					hwndWinamp = ::FindWindow("Winamp v1.x", NULL);
				}
				Sleep(1500);
				wc.Restore();

				if(AfxMessageBox("Winamp has been restarted. Do you want to test the plugin now?", MB_ICONQUESTION+MB_YESNO) == IDYES){

					TestPlugin();
				}
			}
		}
	}
}

void CCfgMessage::TestPlugin(void)
{

	CString strData, strTmp, strOut;
	if(Util::GetExtWinampVars(strData)){

		m_wndWAPlugin.SetWindowText("Advanced Winamp Reportplugin detected. Advanced Variables are available :-)");

		strOut = "Plugin returned the following data:\n\n"
				 "Filename:\t\t%WA-FILE%\n"
				 "Mode:\t\t%WA-MODE%\n"
				 "Artist:\t\t%WA-ARTIST%\n"
				 "Song:\t\t%WA-SONG%\n"
				 "Album:\t\t%WA-ALBUM%\n"
				 "Track:\t\t%WA-TRACK%\n"
				 "Year:\t\t%WA-YEAR%\n"
				 "Genre:\t\t%WA-GENRE%\n"
				 "Comment:\t\t%WA-COMMENT%\n"
				 "Playtime:\t\t%WA-ELATIME%\n"
				 "Totaltime:\t%WA-TOTALTIME%\n"
				 "Remaining:\t%WA-REMTIME%\n"
				 "Status:\t\t%WA-STATUS%\n"
				 "Channels:\t%WA-CHANNELS%\n"
				 "Samplerate:\t%WA-SAMPLERATE%\n"
				 "Bitrate:\t\t%WA-BITRATE%\n"
				 "Video Res.:\t%WA-VIDRES%\n"
				 "Winamp Version:\t%WA-VERSION%";

		Util::ReplaceVars(strOut);
		

		AfxMessageBox(strOut, MB_ICONINFORMATION+MB_OK);
	}
	else{

		AfxMessageBox("The test failed. Was Winamp running and playing a file?", MB_ICONSTOP+MB_OK);
	}
}
