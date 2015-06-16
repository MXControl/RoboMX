#include "StdAfx.h"
#include ".\rainbowplugin.h"

CRainBowPlugin::CRainBowPlugin(void)
{

	m_strName			= "RainBow";
	m_strDescription	= "Creates messages with random colors in channels that support colored text.";
	m_strAuthor			= "Bender979";
}

CRainBowPlugin::~CRainBowPlugin(void)
{
}

void CRainBowPlugin::OnInputHook(DWORD dwID, CString *pInput)
{

	if(pInput->Find("#>>#", 0) == 0){

		pInput->Replace("#>>#", "");

		int nMaxColors = 8;

		CString strServer;
		m_aServers.Lookup(dwID, strServer);
		if(strServer.Find("WCS") >= 0){

			nMaxColors = 255;
		}

		else if(strServer.Find("robo") >= 0){

			nMaxColors = 37;
		}
		else if(strServer.Find("FXServ") >= 0){

			nMaxColors = 37;
		}
		else if(strServer.Find("Ouka") >= 0){

			nMaxColors = 36;
		}
		CString strColor, strTmp;
		for(int i = 0; i < pInput->GetLength(); i++){

			strColor.Format("#c%d#", (rand()%nMaxColors)+1);
            strTmp += strColor + pInput->Mid(i, 1);							
		}
		*pInput = strTmp;
	}
}

void CRainBowPlugin::OnLeaveChannel(DWORD dwID)
{

	m_aServers.RemoveKey(dwID);
}

void CRainBowPlugin::OnJoinChannel(DWORD dwID, CString strChannel, CUserMap* pUserMap, CString strServerString, CString strServerVersion)
{

	m_aServers.SetAt(dwID, strServerString);
}
