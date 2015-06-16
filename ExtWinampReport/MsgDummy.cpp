// MsgDummy.cpp : implementation file
//

#include "stdafx.h"
#include "ExtWinampReport.h"
#include "MsgDummy.h"


// CMsgDummy dialog
#define WM_UPDATE_INFO		WM_USER+1
#define MAX_LENGTH 2048

extern winampGeneralPurposePlugin plugin;

IMPLEMENT_DYNAMIC(CMsgDummy, CDialog)
CMsgDummy::CMsgDummy(CWnd* pParent /*=NULL*/)
	: CDialog(CMsgDummy::IDD, pParent)
{
	m_bIsUpdating = FALSE;
	OpenMemFile();
}

CMsgDummy::~CMsgDummy()
{
}

void CMsgDummy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMsgDummy, CDialog)
	ON_MESSAGE(WM_UPDATE_INFO, OnUpdateInfo)
END_MESSAGE_MAP()


// CMsgDummy message handlers
LRESULT CMsgDummy::OnUpdateInfo(WPARAM wParam, LPARAM lParam)
{

	m_bIsUpdating = TRUE;

	// set up structure for file info query
	// over the winamp IPC api
	extendedFileInfoStruct fileInfo;
	fileInfo.ret      = new char[1024];
	fileInfo.retlen   = 1024;
	ZeroMemory(fileInfo.ret, fileInfo.retlen);

	CString strCurrent, strPrev, strAlbum, strArtist, strSong, strYear, strGenre, strTrack, strComment;
	
	int nCurrent = -1, nLength = 0;
	
	// get length of the play list
	// if the playlist is empty, we need to 
	// abort or we will crash winamp
	nLength = (int)::SendMessage(plugin.hwndParent, WM_WA_IPC, 0, IPC_GETLISTLENGTH);
	if(nLength <= 0){ 
		
		m_bIsUpdating = FALSE; 
		return 0;
	}

	// ask winamp to refresh the playlist cache
	::SendMessage(plugin.hwndParent,WM_WA_IPC,0,IPC_REFRESHPLCACHE);
	// ask winamp to update the info of the current title
	::SendMessage(plugin.hwndParent, WM_WA_IPC, 0, IPC_UPDTITLE);

	// get the position of the track in the playlist
	nCurrent = (int)::SendMessage(plugin.hwndParent, WM_WA_IPC, 0, IPC_GETLISTPOS);
	if(nCurrent >= 0){

		// if the file is in the list we can get the filename
		// that is assigned to the playlist position
		fileInfo.filename = (LPSTR)::SendMessage(plugin.hwndParent, WM_WA_IPC, nCurrent, IPC_GETPLAYLISTFILE);
		strCurrent = fileInfo.filename;
	}
	else{
	
		// otherwise we need to abort
		// or we will crash empty (playlist seems empty then)
		m_bIsUpdating = FALSE;
		return 0;
	}

	// Just to be on the safe side in case the file 
	// does not have a tag, we get the Artist and Song
	// the old fashioned way first
	TCHAR *buff = new TCHAR[250];
	::GetWindowText(plugin.hwndParent, buff, 250);
	CString strTmp = buff;
	strTmp = strTmp.Mid(strTmp.Find(" ", 0)+1, strTmp.Find(" - Winamp") - strTmp.Find(" ", 0)-1);
    int nIndex = strTmp.Find("-", 0);
	if(nIndex > 0){

		if(strArtist.IsEmpty()){

			strArtist	= strTmp.Left(nIndex);
			strArtist.TrimRight();
			strArtist.TrimLeft();
		}
		if(strSong.IsEmpty()){

			strSong		= strTmp.Mid(nIndex+1);
			strSong.TrimLeft();
			strSong.TrimRight();
		}
	}
	// clean up
	delete buff;
	buff = 0;

	// Now we use the extended query api of winamp 
	// to get all interesting fields of the ID3-/Ogg-Tag
	fileInfo.metadata = "Title";
	if(::SendMessage(plugin.hwndParent, WM_WA_IPC, (WPARAM)&fileInfo, IPC_GET_EXTENDED_FILE_INFO)){

		if(strlen(fileInfo.ret) > 0)
			strSong = fileInfo.ret; // only assign if we got a string back
	}
	fileInfo.metadata = "Artist";
	if(::SendMessage(plugin.hwndParent, WM_WA_IPC, (WPARAM)&fileInfo, IPC_GET_EXTENDED_FILE_INFO)){

		if(strlen(fileInfo.ret) > 0)
			strArtist = fileInfo.ret; // only assign if we got a string back
	}


	fileInfo.metadata = "Album";
	if(::SendMessage(plugin.hwndParent, WM_WA_IPC, (WPARAM)&fileInfo, IPC_GET_EXTENDED_FILE_INFO)){

		strAlbum = fileInfo.ret;
	}
	else{ 

		strAlbum	= "Unknown Album"; 
	}

	fileInfo.metadata = "Year";
	if(::SendMessage(plugin.hwndParent, WM_WA_IPC, (WPARAM)&fileInfo, IPC_GET_EXTENDED_FILE_INFO)){

		strYear = fileInfo.ret;
	}
	else{
		
		strYear		= "Unknown Year"; 
	}

	fileInfo.metadata = "Genre";
	if(::SendMessage(plugin.hwndParent, WM_WA_IPC, (WPARAM)&fileInfo, IPC_GET_EXTENDED_FILE_INFO)){

		strGenre = fileInfo.ret;
	}
	else{

		strGenre	= "Other"; 
	}

	fileInfo.metadata = "Track";
	if(::SendMessage(plugin.hwndParent, WM_WA_IPC, (WPARAM)&fileInfo, IPC_GET_EXTENDED_FILE_INFO)){

		strTrack = fileInfo.ret;
	}
	else{

		strTrack	= "0"; 
	}

	fileInfo.metadata = "Comment";
	if(::SendMessage(plugin.hwndParent, WM_WA_IPC, (WPARAM)&fileInfo, IPC_GET_EXTENDED_FILE_INFO)){

		strComment = fileInfo.ret;
		if(strComment.GetLength() > 255) 
			strComment.Truncate(255);
	}
	else{

		strComment	= "";
	}

	strPrev	= strCurrent;
	SetSongInfo(strCurrent, strArtist, strSong, strAlbum, strYear, strGenre, strTrack, strComment);

	// clean up
	delete fileInfo.ret;
	fileInfo.ret = NULL;

	m_bIsUpdating = FALSE;
	return 3; // return 3 to indicate everything went well :-)
}

void CMsgDummy::SetSongInfo(const CString strFile, const CString strArtist, const CString strSong, 
									  const CString strAlbum, const CString strYear, 
									  const CString strGenre, const CString strTrack, 
									  const CString strComment)
{

	CString strData;
	strData.Format("%s \n%s \n%s \n%s \n%s \n%s \n%s \n%s ", strArtist, strSong, strAlbum, strYear, strGenre, strTrack, strFile, strComment);

	LPVOID lpData = m_mmf.Open();
	if(lpData){

		_tcscpy((TCHAR*)lpData, strData);
	}
	m_mmf.Close();
}

void CMsgDummy::OpenMemFile(void)
{
	
	if(!m_mmf.MapExistingMemory(_T("ROBOMXREPORT"), (MAX_LENGTH+1)*sizeof(TCHAR))){

		if(!m_mmf.MapMemory(_T("ROBOMXREPORT"), (MAX_LENGTH+1)*sizeof(TCHAR)))
		{
	
			::MessageBox(plugin.hwndParent, "Error: could not open memory file!", "RoboMX Winamp Report plugin", MB_ICONEXCLAMATION+MB_OK);
		}
  }
}
