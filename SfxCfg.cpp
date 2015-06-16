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
#include "SfxCfg.h"
#include "Settings.h"
#include "Tokenizer.h"
//#include "SfxEdit.h"
#include <mmsystem.h>
#include ".\sfxcfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSfxCfg dialog

extern CSettings g_sSettings;

CSfxCfg::CSfxCfg(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CSfxCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSfxCfg)
	m_strConnect = _T("");
	m_strDisconnect = _T("");
	m_strMotd = _T("");
	m_strRedirect = _T("");
	m_strStart = _T("");
	m_strError = _T("");
	m_strTopic = _T("");
	m_bSfx = FALSE;
	//}}AFX_DATA_INIT
}


void CSfxCfg::DoDataExchange(CDataExchange* pDX)
{
	CCfgDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSfxCfg)
	DDX_Text(pDX, IDC_STATIC_CONNECT, m_strConnect);
	DDX_Text(pDX, IDC_STATIC_DISCONNECTED, m_strDisconnect);
	DDX_Text(pDX, IDC_STATIC_MOTD, m_strMotd);
	DDX_Text(pDX, IDC_STATIC_REDIRECTED, m_strRedirect);
	DDX_Text(pDX, IDC_STATIC_STARTUP, m_strStart);
	DDX_Text(pDX, IDC_STATIC_SYSERROR, m_strError);
	DDX_Text(pDX, IDC_STATIC_TOPIC, m_strTopic);
	DDX_Text(pDX, IDC_STATIC_PM, m_strPM);
	DDX_Check(pDX, IDC_ENABLE_SFX, m_bSfx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSfxCfg, CCfgDlg)
	//{{AFX_MSG_MAP(CSfxCfg)
	ON_BN_CLICKED(IDC_CONNECTED, OnConnected)
	ON_BN_CLICKED(IDC_DISCONNECTED, OnDisconnected)
	ON_BN_CLICKED(IDC_MOTD, OnMotd)
	ON_BN_CLICKED(IDC_REDIRECTED, OnRedirected)
	ON_BN_CLICKED(IDC_STARTUP, OnStartup)
	ON_BN_CLICKED(IDC_SYSERROR, OnSyserror)
	ON_BN_CLICKED(IDC_TOPIC, OnTopic)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PLAY_CONNECTED, OnBnClickedPlayConnected)
	ON_BN_CLICKED(IDC_PLAY_DISCONNECTED, OnBnClickedPlayDisconnected)
	ON_BN_CLICKED(IDC_PLAY_TOPIC, OnBnClickedPlayTopic)
	ON_BN_CLICKED(IDC_PLAY_MOTD, OnBnClickedPlayMotd)
	ON_BN_CLICKED(IDC_PLAY_REDIRECTED, OnBnClickedPlayRedirected)
	ON_BN_CLICKED(IDC_PLAY_START, OnBnClickedPlayStart)
	ON_BN_CLICKED(IDC_PLAY_ERROR, OnBnClickedPlayError)
	ON_BN_CLICKED(IDC_PM, OnBnClickedPm)
	ON_BN_CLICKED(IDC_PLAY_PM, OnBnClickedPlayPm)
	ON_BN_CLICKED(IDC_CONNECTED_NO, OnBnClickedConnectedNo)
	ON_BN_CLICKED(IDC_DISCONNECTED_NO, OnBnClickedDisconnectedNo)
	ON_BN_CLICKED(IDC_TOPIC_NO, OnBnClickedTopicNo)
	ON_BN_CLICKED(IDC_MOTD_NO, OnBnClickedMotdNo)
	ON_BN_CLICKED(IDC_REDIRECT_NO, OnBnClickedRedirectNo)
	ON_BN_CLICKED(IDC_START_NO, OnBnClickedStartNo)
	ON_BN_CLICKED(IDC_ERROR_NO, OnBnClickedErrorNo)
	ON_BN_CLICKED(IDC_PM_NO, OnBnClickedPmNo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSfxCfg message handlers

BOOL CSfxCfg::OnInitDialog() 
{

	CCfgDlg::OnInitDialog();
	
	m_strConnect	= g_sSettings.GetSfxJoin();
	m_strDisconnect = g_sSettings.GetSfxPart();
	m_strMotd		= g_sSettings.GetSfxMotd();
	m_strRedirect	= g_sSettings.GetSfxRedirect();
	m_strStart		= g_sSettings.GetSfxStart();
	m_strError		= g_sSettings.GetSfxError();
	m_strTopic		= g_sSettings.GetSfxTopic();
	m_strPM			= g_sSettings.GetSfxPM();
	m_bSfx			= g_sSettings.GetSoundFX();
	
	UpdateData(FALSE);

	return TRUE;  
}

void CSfxCfg::Apply()
{

	UpdateData(TRUE);

	g_sSettings.SetSfxJoin(m_strConnect);
	g_sSettings.SetSfxPart(m_strDisconnect);
	g_sSettings.SetSfxMotd(m_strMotd);
	g_sSettings.SetSfxRedirect(m_strRedirect);
	g_sSettings.SetSfxStart(m_strStart);
	g_sSettings.SetSfxError(m_strError);
	g_sSettings.SetSfxTopic(m_strTopic);
	g_sSettings.SetSfxPM(m_strPM);
	g_sSettings.SetSoundFX(m_bSfx);
}


void CSfxCfg::OnConnected() 
{

	CString strFilter;
	strFilter.LoadString(IDS_WAVE_FILTER);

	CFileDialog dlg(TRUE, ".wav", m_strConnect, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		m_strConnect = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CSfxCfg::OnDisconnected() 
{

	CString strFilter;
	strFilter.LoadString(IDS_WAVE_FILTER);

	CFileDialog dlg(TRUE, ".wav", m_strDisconnect, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		m_strDisconnect = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CSfxCfg::OnMotd() 
{

	CString strFilter;
	strFilter.LoadString(IDS_WAVE_FILTER);

	CFileDialog dlg(TRUE, ".wav", m_strMotd, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		m_strMotd = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CSfxCfg::OnRedirected() 
{

	CString strFilter;
	strFilter.LoadString(IDS_WAVE_FILTER);

	CFileDialog dlg(TRUE, ".wav", m_strRedirect, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		m_strRedirect = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CSfxCfg::OnStartup() 
{

	CString strFilter;
	strFilter.LoadString(IDS_WAVE_FILTER);

	CFileDialog dlg(TRUE, ".wav", m_strStart, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		m_strStart = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CSfxCfg::OnSyserror() 
{

	CString strFilter;
	strFilter.LoadString(IDS_WAVE_FILTER);

	CFileDialog dlg(TRUE, ".wav", m_strError, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		m_strError = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CSfxCfg::OnTopic() 
{

	CString strFilter;
	strFilter.LoadString(IDS_WAVE_FILTER);

	CFileDialog dlg(TRUE, ".wav", m_strTopic, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		m_strTopic = dlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CSfxCfg::OnBnClickedPm()
{

	CString strFilter;
	strFilter.LoadString(IDS_WAVE_FILTER);

	CFileDialog dlg(TRUE, ".wav", m_strPM, OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		m_strPM = dlg.GetPathName();
		UpdateData(FALSE);
	}
}


void CSfxCfg::OnBnClickedPlayConnected()
{

	PlaySound(m_strConnect, NULL, SND_FILENAME|SND_ASYNC);
}

void CSfxCfg::OnBnClickedPlayDisconnected()
{

	PlaySound(m_strDisconnect, NULL, SND_FILENAME|SND_ASYNC);
}

void CSfxCfg::OnBnClickedPlayTopic()
{

	PlaySound(m_strTopic, NULL, SND_FILENAME|SND_ASYNC);
}

void CSfxCfg::OnBnClickedPlayMotd()
{

	PlaySound(m_strMotd, NULL, SND_FILENAME|SND_ASYNC);
}

void CSfxCfg::OnBnClickedPlayRedirected()
{

	PlaySound(m_strRedirect, NULL, SND_FILENAME|SND_ASYNC);}

void CSfxCfg::OnBnClickedPlayStart()
{

	PlaySound(m_strStart, NULL, SND_FILENAME|SND_ASYNC);}

void CSfxCfg::OnBnClickedPlayError()
{

	PlaySound(m_strError, NULL, SND_FILENAME|SND_ASYNC);
}

void CSfxCfg::OnBnClickedPlayPm()
{

	PlaySound(m_strPM, NULL, SND_FILENAME|SND_ASYNC);
}

void CSfxCfg::OnBnClickedConnectedNo()
{

	m_strConnect.Empty();
	UpdateData(FALSE);
}

void CSfxCfg::OnBnClickedDisconnectedNo()
{

	m_strDisconnect.Empty();
	UpdateData(FALSE);
}

void CSfxCfg::OnBnClickedTopicNo()
{

	m_strTopic.Empty();
	UpdateData(FALSE);
}

void CSfxCfg::OnBnClickedMotdNo()
{

	m_strMotd.Empty();
	UpdateData(FALSE);
}

void CSfxCfg::OnBnClickedRedirectNo()
{

	m_strRedirect.Empty();
	UpdateData(FALSE);
}

void CSfxCfg::OnBnClickedStartNo()
{

	m_strStart.Empty();
	UpdateData(FALSE);
}

void CSfxCfg::OnBnClickedErrorNo()
{
	
	m_strError.Empty();
	UpdateData(FALSE);
}

void CSfxCfg::OnBnClickedPmNo()
{
	
	m_strPM.Empty();
	UpdateData(FALSE);
}
