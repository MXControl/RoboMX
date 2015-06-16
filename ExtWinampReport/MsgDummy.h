#pragma once

#include "memmap.h"

// CMsgDummy dialog

class CMsgDummy : public CDialog
{
	DECLARE_DYNAMIC(CMsgDummy)

public:
	CMsgDummy(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMsgDummy();

// Dialog Data
	enum { IDD = IDD_MSGDUMMY };
	void OnCancel(){}
	volatile BOOL m_bIsUpdating;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
 	void SetSongInfo(const CString strFile, const CString strArtist, const CString strSong, const CString strAlbum, const CString strYear, const CString strGenre, const CString strTrack, const CString strComment);
	void OpenMemFile(void);
	CMemMapFile m_mmf;

   
	afx_msg LRESULT OnUpdateInfo(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
