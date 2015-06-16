#include "StdAfx.h"
#include ".\tagplugin.h"
#include "..\rmx_qapi.h"
#include "..\ini.h"
#include "Options.h"

#define ID_MENUBASE  40810

CTagPlugin::CTagPlugin(void)
{

	m_strName			= "AntiTagger";
	m_strDescription	= "Remove Room Tags from nicknames.";
	m_strAuthor			= "Bender979";
	m_bRun				= 0;
}

CTagPlugin::~CTagPlugin(void)
{
}

void CTagPlugin::Init(void)
{

	int nLen = (int)QueryInformation(RMX_GET_CONFIG, 0, 0);

	if(nLen > 0){

		char *buffer = new char[nLen+1];
		if(QueryInformation(RMX_GET_CONFIG, nLen+1, (LPARAM)buffer) > 0){

			m_strIni = buffer;
			CIni ini;
			ini.SetIniFileName(m_strIni);
			m_bRun		= ini.GetValue("AntiTagger", "Run", TRUE);
		}
		delete buffer;
		buffer = 0;
	}
}

void CTagPlugin::OnMessage(DWORD dwID, CString* pUser, CString* pMsg, BOOL bIsAction)
{

	AntiTag(*pUser);
}

void CTagPlugin::OnOpMessage(DWORD dwID, CString* pUser, CString* pMsg)
{

	AntiTag(*pUser);
}

void CTagPlugin::AntiTag(CString& strName)
{

	
	if(!m_bRun) return;

	CIni ini;
	ini.SetIniFileName(m_strIni);

	int nCount = ini.GetValue("AntiTagger", "Count", 0);
	
	CString strLabel, strTmp;
	for(int i = 0; i < nCount; i++){

		strLabel.Format("TAG_%d", i);
		strTmp = ini.GetValue("AntiTagger", strLabel, "");
		
		if(strTmp.IsEmpty()) continue;

		strName.Replace(strTmp, "");
	}
}


void CTagPlugin::OnPrepareMenu(DWORD dwID, BOOL bIsUser, HMENU hMenu)
{

	if(!bIsUser && dwID){

		::AppendMenu(hMenu, MF_SEPARATOR, 0, (LPCTSTR)0);
		::AppendMenu(hMenu, MF_POPUP, ID_MENUBASE, m_bRun ? "Don't filter Name Tags" : "Filter Name Tags");
		::AppendMenu(hMenu, MF_POPUP, ID_MENUBASE+1, "Configure AntiTagger");
	}
}

void CTagPlugin::OnMenuCommand(UINT nCmd, DWORD dwID, CString strName)
{

	if(nCmd == ID_MENUBASE){

		m_bRun = !m_bRun;
	}
	else if(nCmd == ID_MENUBASE+1){

		Configure();
	}
}

void CTagPlugin::Configure(void)
{

	COptions dlg;
	dlg.m_strIni = m_strIni;
	dlg.DoModal();
}


