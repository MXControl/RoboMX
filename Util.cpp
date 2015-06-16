#include "stdafx.h"
#include ".\util.h"
#include ".\ini.h"
#include ".\settings.h"
#include "SystemInfo.h"
#include "Tokenizer.h"
//#include "winamp.h"
#include <stdio.h>
#include <direct.h>
#include "resource.h"
#include "ExtWinampReport/memmap.h"
#include "ExtWinampReport/wa_ipc.h"

extern CSettings g_sSettings;
CSystemInfo  g_sSystem;

const char* line_types[] = {"Unknown", "14.4K", "28.8K", "33.3K", "56K", 
                            "64K ISDN", "128K ISDN", "Cable", "DSL", "T1", "T3"};

#pragma comment(lib, "version.lib")

#define NUM_LINES 11
#define MAX_LENGTH 2048
#define WM_UPDATE_INFO		WM_USER+1

WORD Util::FormatMXMessage(WORD wType, char *pBuff, char *lpszFormat, ...)
{

	// Data Format
	va_list Args;
	va_start(Args, lpszFormat);
	WORD wLen = 4;
	char *lpText = NULL;
	
	ZeroMemory(pBuff, sizeof(pBuff));
	
	while(*lpszFormat){

		switch(*lpszFormat){

		case 'S' :	// NULL Terminated String
			lpText = va_arg(Args, char *);
			lstrcpy(pBuff + wLen, lpText);
			wLen += (lstrlen(lpText) + 1);
			break;
		case 's' :	// Not NULL Terminated String
			lpText = va_arg(Args, char *);
			lstrcpy(pBuff + wLen, lpText);
			wLen += (lstrlen(lpText));
			break;
		case 'B' :	// BYTE
			pBuff[wLen] = va_arg(Args, BYTE);
			wLen += 1;
			break;
		case 'W' :	// WORD
			*((WORD *)(pBuff + wLen)) = va_arg(Args, WORD);
			wLen += 2;
			break;
		case 'D' :	// DWORD
			*((DWORD *)(pBuff + wLen)) = va_arg(Args, DWORD);
			wLen += 4;
			break;
		default :
			ASSERT(0);
			break;
	}

	lpszFormat++;
	}

	pBuff[wLen] = 0;

	va_end(Args);

	*((WORD*)pBuff) = wType;
	*((WORD*)(pBuff+2)) = wLen-4;

	return wLen;
}

int Util::ScanMessage(char *pBuff, WORD wMaxSize, char *lpszFormat, ...)
{

	ASSERT(pBuff && wMaxSize && lpszFormat);

	va_list Args;
	va_start(Args, lpszFormat);

	WORD wPos = 0;
	int iCount = 0;
	char *lpText = NULL;

	while(*lpszFormat && wPos < wMaxSize){

		switch(*lpszFormat){

		case 'S' : // string
			lpText = pBuff + wPos;
			*va_arg(Args, char **) = lpText;
			wPos += (lstrlen(lpText) + 1);
			iCount++;
			break;
		case 'H': // 20 characters long file hash
			memcpy((void*)va_arg(Args, BYTE *), pBuff + wPos, 20);
			wPos += 20;
			iCount++;
			break;			
		case 'B' : // byte
			*va_arg(Args, BYTE *) = *((BYTE *)(pBuff + wPos));
			wPos += 1;
			iCount++;
			break;
		case 'W' : // word
			*va_arg(Args, WORD *) = *((WORD *)(pBuff + wPos));
			wPos += 2;
			iCount++;
			break;
		case 'D' : // dword
			*va_arg(Args, DWORD *) = *((DWORD *)(pBuff + wPos));
			wPos += 4;
			iCount++;
			break;
		default :
			ASSERT(0);
			break;
		}
	
		lpszFormat++;
	}

	va_end(Args);

	return iCount;
}

CString Util::FormatIP(DWORD dwIP)
{
	
	if(dwIP == 0) return "";

	CString strReturn;
	int a = ((dwIP >> 24) & 0xff);
	int b = ((dwIP >> 16) & 0xff);
	int c = ((dwIP >> 8) & 0xff);
	int d = (dwIP & 0xff);
	strReturn.Format("%d.%d.%d.%d", d, c, b, a);
	return strReturn;
}

CString Util::FormatLine(WORD wLine)
{
	
	CString strLine;
	if((wLine >= 0) && (wLine < NUM_LINES)){
		
		strLine = line_types[wLine];
	}
	else{

		strLine = "N/A";
	}

	return strLine;
}

CString Util::Formatint(int nNumber)
{

	CString strNum;
	strNum.Format("%d", nNumber);
	return strNum;
}

int Util::axtoi(char *hexStg, int nLen)
{

  int n = 0;         // position in string
  int m = 0;         // position in digit[] to shift
  int count;         // loop index
  int intValue = 0;  // integer value of hex string
  int *digit = new int[nLen];      // hold values to convert
  while (n < nLen) {
	if (hexStg[n]=='\0'){
     
		break;
	}
    if (hexStg[n] > 0x29 && hexStg[n] < 0x40 ){
		//if 0 to 9
        digit[n] = hexStg[n] & 0x0f;            //convert to int
	}
    else if (hexStg[n] >='a' && hexStg[n] <= 'f'){ //if a to f

        digit[n] = (hexStg[n] & 0x0f) + 9; 
	}//convert to int
	else if (hexStg[n] >='A' && hexStg[n] <= 'F'){ //if A to F

        digit[n] = (hexStg[n] & 0x0f) + 9;  
	}//convert to int
    else break;
    n++;
  }
  count = n;
  m = n - 1;
  n = 0;
  while(n < count) {
     // digit[n] is value of hex digit at position n
     // (m << 2) is the number of positions to shift
     // OR the bits into return value
     intValue = intValue | (digit[n] << (m << 2));
     m--;   // adjust the position to set
     n++;   // next digit to process
  }

  delete digit;
  digit = 0;
  return (intValue);
}

int Util::ShellExecuteWait(const CString strExec, const CString strParam)
{

	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = strExec;		
	ShExecInfo.lpParameters = strParam;	
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.hInstApp = NULL;	
	ShellExecuteEx(&ShExecInfo);
	WaitForSingleObject(ShExecInfo.hProcess,INFINITE);
	return 0;
}


CString Util::GetBaseRoomName(CString strRoom)
{
	
	CString strBase;
	int n = strRoom.ReverseFind('_');
	if(n > 0){

       strBase  = strRoom.Left(n);
	}
	return strBase;
}

CString Util::GetBaseName(const CString strName, BOOL bNoAdd, BOOL bNoID)
{

	CString strBase;
	
	int n = strName.ReverseFind('_');

	if((n > 0) && (strName.GetLength() - n == 6)){

       strBase  = strName.Left(strName.GetLength() - 6);
	}
	else{

		strBase = strName;
	}
	if(bNoID){

		int nDigits = 0, i = strBase.GetLength() - 1;
		for(nDigits = 0; i > 0 && nDigits < 3; i--){

			if(_istdigit(strBase[i])) nDigits ++;
			else break;
		}
		if(nDigits <= 3){

			strBase = strBase.Left(strBase.GetLength()-nDigits);
			return strBase;
		}
	}
	if(bNoAdd) return strBase;

	// test if the last 3 characters of the basename are digits
	int nDigits = 0, i = strBase.GetLength() - 1;
	for(nDigits = 0; i > 0 && nDigits < 3; i--){

		if(_istdigit(strBase[i])) nDigits ++;
		else break;
	}
	while(nDigits < 3){

		strBase += Util::Formatint(rand() % 9);
		nDigits++;
	}
	
	return strBase;
}


void Util::BaseName(CString& strName)
{

	int n = strName.ReverseFind('_');
	if((n > 0) && (strName.GetLength() - n == 6)){

       strName  = strName.Left(strName.GetLength() - 6);
	}
}

void Util::MakeValidUserName(CString& strName, BOOL bWPN, WORD wID)
{

	strName.Replace(" ", " "); // replace normal space with nonbreaking space
	
	CString strBase = strName;
	Util::BaseName(strBase);

	if(strBase.GetLength() > 36){

		//strBase = strBase.Left(39);
		strBase.Truncate(39);
		int nLen = strBase.GetLength()-1;
		while(nLen > 35){

			if(!_istdigit(strBase[nLen])){

				strBase.SetAt(nLen, (char)(rand()%9 + 48));
			}
			nLen--;
		}
	}

	// Verify the last 3 digits are numbers
	int nDigits = 0, i = strBase.GetLength() - 1;
	for(nDigits = 0; i > 0 && nDigits < 3; i--){

		if(_istdigit(strBase[i])) nDigits ++;
		else break;
	}
	while(nDigits < 3){

		strBase += Util::Formatint(rand() % 9);
		nDigits++;
	}


	if(bWPN){

		strName.Format("%s_%05d", strBase, wID);
	}

	else{

		strName.Format("%s_%05d", strBase, rand() % 99999);
	}
}

CString Util::GetIdleTime(DWORD dwIdleSince)
{

	DWORD dwIdleTime = (GetTickCount() - dwIdleSince)/1000;


	CString strIdleTime;
	strIdleTime.Format("%02d:%02d", dwIdleTime/60, dwIdleTime%60);
	
	return strIdleTime;
}

CString Util::GetStayTime(DWORD dwJointime)
{

	DWORD dwMinutes = GetTickCount() - dwJointime;
	
	CString strTime;
	strTime.Format("%d", dwMinutes / 1000 / 60);
	
	return strTime;
}

CString Util::GetMyLocalTime()
{

	CString strTime;

	CTime myTime;
	myTime = CTime::GetCurrentTime();
    	
	if(g_sSettings.GetTimeFmt() == 0){

		strTime = myTime.Format("%H:%M:%S");
	}
	else{

		strTime = myTime.Format("%I:%M %p");
	}

    return strTime;
}

CString Util::GetMyDate(void)
{

	CString strDate;
	SYSTEMTIME time;

	int n = GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SSHORTDATE, 0, 0);

	if(n != 0){

		char *szFormat = new char[n];
		ZeroMemory(szFormat, n);

		n = GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SSHORTDATE, szFormat, n);

		GetLocalTime(&time);
		
		n = GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &time, szFormat, 0, 0); 
		
		if(n != 0){

			char *szDate = new char[n];
			ZeroMemory(szDate, n);

			n = GetDateFormat(LOCALE_SYSTEM_DEFAULT, 0, &time, szFormat, szDate, n);
			
			strDate = szDate;
			
			delete szDate;
			szDate   = 0;
		}

		delete szFormat;
		szFormat = 0;
	}
	return strDate;
}

CString Util::GetDateString(void)
{

	CString strData;
	SYSTEMTIME time;
	
	GetLocalTime(&time);

	strData.Format("_%04d%02d%02d", time.wYear, time.wMonth, time.wDay);
	return strData;
}


void Util::ReplaceVars(CString &strMsg)
{

	if(strMsg.Find("%", 0) < 0 ) return;

	CString strArtist, strSong, strFile, strMode, strAlbum, strYear, strTrack, strComment, strGenre, strVersion, strPlayTime, strTotalTime, strRemTime, strSampleRate, strBitrate, strNumChannels, strStatus = "not running", strVidRes;
	
	strMsg.Replace(_T("%TIME%"), Util::GetMyLocalTime());
	strMsg.Replace(_T("%DATE%"), Util::GetMyDate());
	strMsg.Replace(_T("%ROBOMX%"), STR_VERSION_DLG);

	if(strMsg.Find("%WA-", 0) >= 0){


		CString strTmp;

		// Get Song and Artist playing in
		// Winamp. This is for videoplayback
		// and in case the advanced lookup fails
		strTmp = Util::GetWinampSong();
		
		CTokenizer token(strTmp, "-");
		
		if(strArtist.IsEmpty()) token.Next(strArtist);
		if(strSong.IsEmpty())   strSong = token.Tail();
		
		strArtist.TrimRight();
		strSong.TrimLeft();


		if(!Util::IsVideoPlaying()){

			// If Winamp does not play a video, get 
			// all the ID3-/ Ogg-Tag entries from 
			// our Winamp Plugin :-)
			BOOL bSuccess = Util::GetExtWinampVars(strTmp);
			
			if(bSuccess){ // Was successfull

				CTokenizer token(strTmp, "\n");
				if(token.Next(strTmp)){
			
					strTmp.TrimRight();
					strTmp.TrimLeft();
					if(strTmp.GetLength())   // If this entry is OK copy it, 
                        strArtist =  strTmp; // Otherwise we use the old value...
				}
				if(token.Next(strTmp)){
			
					strTmp.TrimRight();
					strTmp.TrimLeft();
					if(strTmp.GetLength())  // If this entry is OK copy it, 
						strSong = strTmp;   // Otherwise we use the old value...
				}
				if(token.Next(strTmp)){
			
					strTmp.TrimRight();
					strAlbum = strTmp;
				}
				if(token.Next(strTmp)){
			
					strTmp.TrimRight();
					strYear = strTmp;
				}
				if(token.Next(strTmp)){
			
					strTmp.TrimRight();
					strGenre = strTmp;
				}
				if(token.Next(strTmp)){
			
					strTmp.TrimRight();
					strTrack = strTmp;
				}
				if(token.Next(strTmp)){
			
					strTmp.TrimRight();
					strFile = strTmp;
				}
				if(token.Next(strTmp)){
			
					strTmp.TrimRight();
					strComment = strTmp;
				}
			}
		}

		// Get all the other Winamp stuff
		HWND hwndWinamp = ::FindWindow("Winamp v1.x",NULL);

		if(hwndWinamp != NULL){


			strVersion.Format("%x", ::SendMessage(hwndWinamp, WM_WA_IPC, 0, IPC_GETVERSION));
			strVersion.SetAt(1, '.');

			int nTotal = 0, nRem = 0, nEla = 0;
			nTotal = (int)::SendMessage(hwndWinamp, WM_WA_IPC, 1, IPC_GETOUTPUTTIME);
			strTotalTime.Format("%02d:%02d", nTotal/60, nTotal%60);

			nEla= (int)::SendMessage(hwndWinamp, WM_WA_IPC, 0, IPC_GETOUTPUTTIME) / 1000;
			strPlayTime.Format("%02d:%02d", nEla/60, nEla%60);
			
			nRem = nTotal - nEla;
			strRemTime.Format("%02d:%02d", nRem/60, nRem%60);

			strSampleRate.Format("%d", ::SendMessage(hwndWinamp, WM_WA_IPC, 0, IPC_GETINFO));
			strBitrate.Format("%d", ::SendMessage(hwndWinamp, WM_WA_IPC, 1, IPC_GETINFO));
			strNumChannels = (::SendMessage(hwndWinamp, WM_WA_IPC, 2, IPC_GETINFO) == 1 ? "Mono" : "Stereo");
			
			switch(::SendMessage(hwndWinamp, WM_WA_IPC, 0, IPC_ISPLAYING)){

			case 1: strStatus = "playing";
				break;
			case 3: strStatus = "paused";
				break;
			default: strStatus = "stopped";
			}

			if(::SendMessage(hwndWinamp, WM_WA_IPC, 0, IPC_IS_PLAYING_VIDEO) > 1){

				strMode = "Video";
				int nRes = (int)::SendMessage(hwndWinamp, WM_WA_IPC, 3, IPC_GETINFO);
				strVidRes.Format("%dx%d", LOWORD(nRes), HIWORD(nRes));
			}
			else{

				strMode = "Audio";
			}

		}
		
		strMsg.Replace(_T("%WA-ARTIST%"), strArtist);
		strMsg.Replace(_T("%WA-SONG%"), strSong);
		strMsg.Replace(_T("%WA-ALBUM%"), strAlbum);
		strMsg.Replace(_T("%WA-TRACK%"), strTrack);
		strMsg.Replace(_T("%WA-YEAR%"), strYear);
		strMsg.Replace(_T("%WA-GENRE%"), strGenre);
		strMsg.Replace(_T("%WA-COMMENT%"), strComment);
		strMsg.Replace(_T("%WA-VERSION%"), strVersion);
		strMsg.Replace(_T("%WA-ELATIME%"), strPlayTime);
		strMsg.Replace(_T("%WA-REMTIME%"), strRemTime);
		strMsg.Replace(_T("%WA-TOTALTIME%"), strTotalTime);
		strMsg.Replace(_T("%WA-SAMPLERATE%"), strSampleRate);
		strMsg.Replace(_T("%WA-BITRATE%"), strBitrate);
		strMsg.Replace(_T("%WA-CHANNELS%"), strNumChannels);
		strMsg.Replace(_T("%WA-STATUS%"), strStatus);
		strMsg.Replace(_T("%WA-VIDRES%"), strVidRes);
		strMsg.Replace(_T("%WA-FILE%"), strFile);
		strMsg.Replace(_T("%WA-MODE%"), strMode);
	}	
}

CString Util::GetUpTime()
{

	CString strUp;
	DWORD nMS = GetTickCount();
	int nSec = nMS / 1000;
	int nMin = nMS / 60000;
	int nHour = nMS / 3600000;
	strUp.Format(IDS_SYSTEMUPTIME,
		nHour/24, nHour%24, nMin - nHour*60, nSec - nMin*60, nMS-nSec*1000);

	return strUp;
}

CString Util::GetWinampSong()
{

	CString strWinamp, strOut;
	HWND hwndWinamp = ::FindWindow("Winamp v1.x",NULL);
	if(hwndWinamp != NULL){

		::SendMessage(hwndWinamp, WM_WA_IPC,0,IPC_UPDTITLE);
		TCHAR *buff = new TCHAR[250];
		::GetWindowText(hwndWinamp, buff, 250);
		strWinamp = buff;
		strWinamp = strWinamp.Mid(strWinamp.Find(" ", 0)+1, strWinamp.Find(" - Winamp") - strWinamp.Find(" ", 0)-1);
		delete buff;
		buff = NULL;
	}
	else{

		strWinamp = "Winamp - Not active";
	}

	return strWinamp;
}


BOOL Util::IsVideoPlaying()
{

	HWND hwndWinamp = ::FindWindow("Winamp v1.x",NULL);
	BOOL bReturn = FALSE;
	switch(::SendMessage(hwndWinamp, WM_WA_IPC, 0, IPC_IS_PLAYING_VIDEO)){

	case 0:
	case 1:
		bReturn = FALSE;
		break;
	default:
		bReturn = TRUE;
	}

	return bReturn;

}

BOOL Util::GetExtWinampVars(CString& rData)
{

	BOOL bReturn = FALSE;
	
	HWND hwndExt = ::FindWindow(NULL, "ExtWinampReportv1.1");
	
	if(hwndExt != NULL){ // found the window of our plugin
	
		if(::SendMessage(hwndExt, WM_UPDATE_INFO, 0, 0) == 3){ 
			
			// successfully asked to update the tag info

			CMemMapFile mmf;

			// open memory file with the tag
			if(mmf.MapExistingMemory(_T("ROBOMXREPORT"), (MAX_LENGTH+1)*sizeof(TCHAR))){

				TCHAR tcText[MAX_LENGTH+1];
				_tcscpy(tcText, _T(""));
				
				LPVOID lpData = mmf.Open();
				if(lpData){

					// we successfully got a pointer to the 
					// content of the memory file
					_tcscpy(tcText, (TCHAR*)lpData);
					rData = tcText;
					bReturn = TRUE;
				}
				
				// close the memory file again
				mmf.Close();
			}
		}
	}
	return bReturn;
}

CString Util::GetMySystemInfo()
{

	CString strInfo;
	strInfo.Format(
				"%s build=%d «» CPUs %d Speed %d MHz «» %s %s %s",
				g_sSystem.GetOSType(), g_sSystem.GetBuildNumber(),
				g_sSystem.GetNumProcessors(), g_sSystem.GetCPUSpeed(),
				g_sSystem.GetCPUIdentifier(), g_sSystem.GetCPUVendorIdentifier(), g_sSystem.GetCPUNameString()
			);	
	return strInfo;
}

CString Util::GetMySystemInfoEx(void)
{

	CString strInfo;
	strInfo.Format("System Information for «%s»: %s (Build %d)\n"
				   "CPU: %s %s %s %d MHz, RAM: %s\n"
				   "Harddisks: %s\n",
				   g_sSystem.GetPCHostname(), g_sSystem.GetOSType(), g_sSystem.GetBuildNumber(),
				g_sSystem.GetCPUIdentifier(), g_sSystem.GetCPUVendorIdentifier(), 
				g_sSystem.GetCPUNameString(), g_sSystem.GetCPUSpeed(),
				g_sSystem.GetTotalRAM(), g_sSystem.GetHardiskInfo());
				
	return strInfo;
}

BOOL Util::FileExists(LPCTSTR lpszFile)
{

	TRY{
		
		CFileStatus rStatus;
		if(CFile::GetStatus(lpszFile, rStatus)){

			return TRUE;
		}
		else{

			return FALSE;
		}
	}
	CATCH(CFileException, e){

		TRACE0("File access exception/violation in Util::FileExist\n");
		return TRUE;
	}
	END_CATCH;

	return FALSE;
}

BOOL Util::CreateDirs(CString strFilename)
{

	int nStart = 3;
	int nEnd = 0;
	
	CString validated = strFilename.Left(3);
	_tchdir(validated);
	
	while(TRUE){

		nEnd = strFilename.Find('\\', nStart);
		if(nEnd == -1){

			return TRUE;
		}

		if(_tchdir(validated + "\\" + strFilename.Mid(nStart, nEnd - nStart)) != 0){

			_tchdir(validated);
			if(_tmkdir(strFilename.Mid(nStart, nEnd - nStart)) != 0){

				return FALSE;
			}
			
		}
		validated += "\\" + strFilename.Mid(nStart, nEnd - nStart);
		nStart = nEnd + 1;		
	}

	return FALSE;
}

CString Util::MakeValidFilename(CString strName)
{

	strName.Remove('?');
	strName.Remove(':');
	strName.Remove(',');
	strName.Remove('\\');
	strName.Remove('/');
	strName.Remove('<');
	strName.Remove('>');
	strName.Remove('\"');
	strName.Remove('*');
	strName.Remove('|');
	strName.Replace(1, '-');
	
	return strName;
}

CString Util::GetIPFromRoom(CString strRoom)
{
	
	int nIndex = strRoom.ReverseFind('_')+1;

	if(nIndex <= 0) return "";

	CString strTmp = strRoom.Mid(nIndex, 8);
	
	int nA = 0, nB = 0, nC = 0, nD = 0;
	
	nA = Util::axtoi((LPSTR)(LPCSTR)strTmp.Mid(0,2), 2);
	nB = Util::axtoi((LPSTR)(LPCSTR)strTmp.Mid(2,2), 2);
	nC = Util::axtoi((LPSTR)(LPCSTR)strTmp.Mid(4,2), 2);
	nD = Util::axtoi((LPSTR)(LPCSTR)strTmp.Mid(6,2), 2);

	strTmp.Format("%d.%d.%d.%d", nD, nC, nB, nA);
	return strTmp;
}


LPCTSTR _tcsistr(LPCTSTR pszString, LPCTSTR pszPattern)
{
	LPCTSTR pptr, sptr, start;
	//DWORD slen, plen;
	size_t slen, plen;

	for (	start	= pszString,
			pptr	= pszPattern,
			slen	= _tcslen( pszString ),
			plen	= _tcslen( pszPattern ) ;
			slen >= plen ; start++, slen-- )
	{
		while ( toupper( *start ) != toupper( *pszPattern ) )
		{
			start++;
			slen--;

			if ( slen < plen ) return NULL;
		}

		sptr = start;
		pptr = pszPattern;

		while ( toupper( *sptr ) == toupper( *pptr ) )
		{
			sptr++;
			pptr++;

			if ( ! *pptr) return start;
		}
	}

	return NULL;
}
		

CString Util::LoadString(UINT nID)
{

	CString strReturn;
	strReturn.LoadString(nID);
	return strReturn;
}

BOOL Util::WildcardMatch(CString strString, CString strSearch, BOOL bStartWild, BOOL bCase)
{

	if(strSearch.IsEmpty()) return TRUE; // this is always true ;)
	if(strSearch.Right(1) != "*") strSearch += "*";
	if(bStartWild && strSearch[0] != '*') strSearch.Insert(0, '*');

	if(!bCase){

		strString.MakeLower();
		strSearch.MakeLower();
	}

	char* string = (char*)(const char*)strString;
	char* wild   = (char*)(const char*)strSearch;

	char *cp, *mp;
	
	while((*string) && (*wild != '*')){

		if((*wild != *string) && (*wild != '?')){

			return 0;
		}
		wild++;
		string++;
	}
		
	while(*string){

		if(*wild == '*'){

			if(!*++wild){

				return 1;
			}
			mp = wild;
			cp = string+1;
		}
		else if((*wild == *string) || (*wild == '?')){

			wild++;
			string++;
		}
		else{

			wild = mp;
			string = cp++;
		}
	}
		
	while(*wild == '*'){

		wild++;
	}
	return !*wild;	
}



CString Util::FormatFloat(float fNum)
{

	CString strReturn;
	strReturn.Format("%.2f", fNum);
	return strReturn;
}

