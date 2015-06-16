#pragma once
#ifdef _ROBO_READER


#include "sapi.h"
#include <sphelper.h>
#include "roboex.h"
// CONTANTS OF MOUTH
#define CHARACTER_WIDTH         128
#define CHARACTER_HEIGHT        128
#define WEYESNAR                14              // eye positions
#define WEYESCLO                15

class CVoiceDlg : public CDialog
{
	DECLARE_DYNAMIC(CVoiceDlg)

public:
	CVoiceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CVoiceDlg();
// Dialog Data
	enum { IDD = IDD_VOICE };

	CStatic		m_cMouth;
	
	CComPtr<ISpVoice> IpVoice;
	BOOL		m_bCurrentMale;
	CImageList	m_cMouthList;
	CImageList	m_cMaleList;
	int			m_iMouthBmp;
	CRect		m_cMouthRect;
	CString		m_strError;
 	
	BOOL InitMouthImageList();
	BOOL DestroySAPI();
	BOOL InitializationSAPI();
public:
	void SpeakOut(const CString strText);
	void SpeakChat(CString strName, CString strText, BOOL bAction);
	static void FixString(CString& strText);
	static void FixName(CString& strName);

protected:
	virtual BOOL OnInitDialog(void);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnMouthEvent(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnStnClickedMouthImg();
 	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSetup();
};

#endif