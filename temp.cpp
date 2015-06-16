	CString strTime;

	if(wType == 0x00D2){ // opmsg

		CTokenizer token(strMsg, " ");
		BOOL bOK = FALSE;

		token.Next(strName);

		if(strName.GetLength() > 2){

			strMsg = token.Tail();
			//%c%c<%s%c%c>	 MCMA
			//<> %c%c winmx
			bOK = ((strName.Find("<") != -1) && (strName.Find(">") != -1));

			CString strRealName;
			
			if(!m_aMCMAMap.Lookup(strName, strRealName)){

				strRealName = strName;
			}


			if(strMsg.Find(m_strAvLagBuff) >= 0){

				UpdateAverageLag(FALSE);
			}
		
			if(strMsg.IsEmpty()) return 0;

			for(int i = 0; i < g_sSettings.m_aIgnored.GetSize(); i++){

				if(Util::WildcardMatch(strName, g_sSettings.m_aIgnored[i]))
					return 1;
			}


			if(g_sSettings.GetPrintTime()){

				WriteTime(g_sSettings.GetRGBOp());
			}

			WriteText(strName, g_sSettings.GetRGBOp(), g_sSettings.GetRGBBg(), TRUE, FALSE, TRUE);
			WriteText(" " + strMsg, bOK ? g_sSettings.GetRGBNormalMsg() : g_sSettings.GetRGBOp(), g_sSettings.GetRGBBg(), TRUE, TRUE);

			Log(strRealName, strMsg);
		}
		else{

			if(strMsg.IsEmpty()) return 0;
			
			if(g_sSettings.GetPrintTime()){

				WriteTime(g_sSettings.GetRGBOp());
			}

			WriteText(strMsg, 
				(m_nServerType >= SERVER_WINMX353 ? g_sSettings.GetRGBActionMsg() : g_sSettings.GetRGBOp()), 
				g_sSettings.GetRGBBg(), TRUE, TRUE);
			Log(strMsg, "");
		}

#ifdef _ROBO_READER
		Speak(strName, strMsg, FALSE);
#endif

	}
	else if(wType == 0x00D3){ // command echo

		if(strMsg.IsEmpty()) return 0;
		if(g_sSettings.GetPrintTime()){

			WriteTime(g_sSettings.GetRGBOp());
		}
		WriteText(g_sSettings.GetBrMsgEnd() + " ", g_sSettings.GetRGBOp(), g_sSettings.GetRGBBg(), TRUE);
		WriteText(strMsg, g_sSettings.GetRGBNormalMsg(), g_sSettings.GetRGBBg(), TRUE, TRUE);
		Log(":", strMsg);
	}

	m_dwMessages++;
	HandleHiLite();
	HandleSounds(strMsg);

	if(m_nServerType >= SERVER_WINMX353){

		if(strName.GetLength() > 0){

			strName.TrimLeft();
			strName.TrimRight();
			int nEnd = strName.GetLength() - 1;

			if((strName[0] == '<') && (strName[nEnd] == '>')){

				strName = strName.Mid(1, nEnd-1);
			}
			CString strTmp;
			if(m_aMCMAMap.Lookup(strName, strTmp)){

				TRACE("%s maps to %s\n", strName, strTmp);
				strName = strTmp;
			}
		}
	}

	UpdateUserStats(strName, strMsg);

	for(int i = 0; i < g_aPlugins.GetSize(); i++){

		PLUGIN->OnOpMessage((DWORD)m_hWnd, &strName, &strMsg);
	}
