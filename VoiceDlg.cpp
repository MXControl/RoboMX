// VoiceDlg.cpp : implementation file
//

#include "stdafx.h"
#ifdef _ROBO_READER

#include "RoboMX.h"
#include "VoiceDlg.h"
#include ".\voicedlg.h"
#include "Settings.h"
#include "VoiceSetup.h"
#define WM_RECOEVENT	WM_USER+101	


extern CSettings g_sSettings;

// CVoiceDlg dialog
/////////////////////////////////////////////////////////////////////////////
// Mouth Mapping Array(from Microsoft's TTSApp Example)

const int g_iMapVisemeToImage[22] = {
	0,  // SP_VISEME_0 = 0,    // Silence
    11, // SP_VISEME_1,        // AE, AX, AH
    11, // SP_VISEME_2,        // AA
    11, // SP_VISEME_3,        // AO
    10, // SP_VISEME_4,        // EY, EH, UH
    11, // SP_VISEME_5,        // ER
    9,  // SP_VISEME_6,        // y, IY, IH, IX
    2,  // SP_VISEME_7,        // w, UW
    13, // SP_VISEME_8,        // OW
    9,  // SP_VISEME_9,        // AW
    12, // SP_VISEME_10,       // OY
    11, // SP_VISEME_11,       // AY
    9,  // SP_VISEME_12,       // h
    3,  // SP_VISEME_13,       // r
    6,  // SP_VISEME_14,       // l
    7,  // SP_VISEME_15,       // s, z
    8,  // SP_VISEME_16,       // SH, CH, JH, ZH
    5,  // SP_VISEME_17,       // TH, DH
    4,  // SP_VISEME_18,       // f, v
    7,  // SP_VISEME_19,       // d, t, n
    9,  // SP_VISEME_20,       // k, g, NG
    1	// SP_VISEME_21,       // p, b, m
};

IMPLEMENT_DYNAMIC(CVoiceDlg, CDialog)
CVoiceDlg::CVoiceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVoiceDlg::IDD, pParent)
{
	m_iMouthBmp = 0;
	m_bCurrentMale = TRUE;
}

CVoiceDlg::~CVoiceDlg()
{
}

void CVoiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MOUTH_IMG, m_cMouth);
}


BEGIN_MESSAGE_MAP(CVoiceDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_MESSAGE(WM_RECOEVENT, OnMouthEvent)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_STN_CLICKED(IDC_MOUTH_IMG, OnStnClickedMouthImg)
	ON_BN_CLICKED(IDC_SETUP, OnBnClickedSetup)
END_MESSAGE_MAP()


// CVoiceDlg message handlers
BOOL CVoiceDlg::OnInitDialog(void)
{

	CDialog::OnInitDialog();

	SetWindowPos(&wndTopMost, 0, 0, 0, 0,SWP_NOSIZE|SWP_NOMOVE);


	if(!InitializationSAPI()){
	
		AfxMessageBox(m_strError, MB_ICONSTOP);		
		DestroySAPI();
	}

	InitMouthImageList();
	return 0;
}

void CVoiceDlg::OnPaint() 
{
	
	if(!IsIconic())
	{

		CPaintDC dc(this);
		CDialog::OnPaint();
		// Draw into memory DC
		m_cMaleList.Draw(&dc, 0, m_cMouthRect.TopLeft(), INDEXTOOVERLAYMASK(m_iMouthBmp)); 
		
		if(m_iMouthBmp % 6 == 2) {
			m_cMaleList.Draw(&dc, WEYESNAR, m_cMouthRect.TopLeft(), 0); 
		} else if(m_iMouthBmp % 6 == 5) {
			m_cMaleList.Draw(&dc, WEYESCLO, m_cMouthRect.TopLeft(), 0); 
		}
	}
}

void CVoiceDlg::OnDestroy() 
{
	
	DestroySAPI();
	CDialog::OnDestroy();
		
}

BOOL CVoiceDlg::InitializationSAPI()
{
	if(FAILED(CoInitialize(NULL))) {
		
		m_strError=_T("Error intialization COM");
		return FALSE;
	}

    HRESULT hRes;
	
	hRes = IpVoice.CoCreateInstance(CLSID_SpVoice);
	
	if(FAILED(hRes)) {
		m_strError=_T("Error creating voice");
		return FALSE;
	}
	
	hRes = IpVoice->SetInterest(SPFEI(SPEI_VISEME), SPFEI(SPEI_VISEME));	

	if(FAILED(hRes)) {
		m_strError=_T("Error creating interest.");
		return FALSE;
	}
	
	hRes = IpVoice->SetNotifyWindowMessage(m_hWnd, WM_RECOEVENT, 0, 0);
	
	if(FAILED(hRes)) {
		m_strError=_T("Error setting notification window");
		return FALSE;
	}


	long lRate = 0;
	hRes = IpVoice->GetRate(&lRate);
	
	if(FAILED(hRes)) {
		m_strError=_T("Error applying voice settings");
		return FALSE;
	}

	lRate +=2;
	
	hRes = IpVoice->SetRate(lRate);
	
	if(FAILED(hRes)) {
		m_strError=_T("Error applying voice settings");
		return FALSE;
	}



	return TRUE;
}

BOOL CVoiceDlg::DestroySAPI()
{
	if(IpVoice) {
		IpVoice.Release();
	}
	return TRUE;
}

BOOL CVoiceDlg::InitMouthImageList()
{

	m_cMouth.GetClientRect(&m_cMouthRect);
	m_cMouth.ClientToScreen(&m_cMouthRect);
	ScreenToClient(&m_cMouthRect);

	CString strFile = g_sSettings.GetMale();

	HBITMAP hBmp = (HBITMAP)::LoadImage(NULL, strFile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE|LR_CREATEDIBSECTION);

	if(hBmp == NULL) return FALSE;

	CBitmap bmp;
	bmp.Attach(hBmp);

	m_cMaleList.Create(128, 128, ILC_COLORDDB|ILC_MASK, 15, 1);
	m_cMaleList.Add(&bmp, RGB(255,0,255));
	bmp.Detach();
		
	m_cMaleList.SetOverlayImage(1, 1);
	m_cMaleList.SetOverlayImage(2, 2);
	m_cMaleList.SetOverlayImage(3, 3);
	m_cMaleList.SetOverlayImage(4, 4);
	m_cMaleList.SetOverlayImage(5, 5);
	m_cMaleList.SetOverlayImage(6, 6);
	m_cMaleList.SetOverlayImage(7, 7);
	m_cMaleList.SetOverlayImage(8, 8);
	m_cMaleList.SetOverlayImage(9, 9);
	m_cMaleList.SetOverlayImage(10, 10);
	m_cMaleList.SetOverlayImage(11, 11);
	m_cMaleList.SetOverlayImage(12, 12);
	m_cMaleList.SetOverlayImage(13, 13);
	m_cMaleList.SetOverlayImage(14, WEYESNAR);
	m_cMaleList.SetOverlayImage(15, WEYESCLO);
	
	return TRUE;  
}


void CVoiceDlg::OnLButtonDown(UINT nFlags, CPoint point)
{

	if(m_cMouthRect.PtInRect(point)){

		OnStnClickedMouthImg();
	}   
}

LRESULT CVoiceDlg::OnMouthEvent(WPARAM wParam, LPARAM lParam) 
{

	CSpEvent event;  
	
    while(event.GetFrom(IpVoice) == S_OK) {
        switch(event.eEventId) {
			case SPEI_VISEME:
				m_iMouthBmp = g_iMapVisemeToImage[event.Viseme()];
				InvalidateRect(m_cMouthRect, false);
				break;
        }
    }

	return 0;
}

void CVoiceDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//OnCancel();
	//USES_CONVERSION;
	CString strText = "OK, OK, I am quiet now.";
	IpVoice->Speak(strText.AllocSysString(), SPF_PURGEBEFORESPEAK|SPF_ASYNC, NULL);	
	ShowWindow(SW_HIDE);
}


void CVoiceDlg::SpeakOut(const CString strText)
{

	if(!m_strError.IsEmpty() || !IsWindowVisible()) return;

	TRACE("Speak %s\n", strText);

	IpVoice->Speak(strText.AllocSysString(), SPF_ASYNC, NULL);		
}

void CVoiceDlg::OnStnClickedMouthImg()
{

	if(!m_strError.IsEmpty() || !IsWindowVisible()) return;

	SpeakOut("Stop clicking on me, I am ticklish!");
}

void CVoiceDlg::SpeakChat(CString strName, CString strText, BOOL bAction)
{

	if(!m_strError.IsEmpty() || !IsWindowVisible()) return;

	USES_CONVERSION;
	CString strOut;
	FixName(strName);
	FixString(strText);

	if(bAction){

		strOut = strName + " " + strText;
	}
	else{

		strOut = strName + " says: " + strText;
	}
	
	SpeakOut(strOut);
	//m_aTexts.Add(strOut);
}

void CVoiceDlg::FixString(CString& strText)
{

	strText.Replace(":-)", "happy smiley");
	strText.Replace(":)", "happy smiley");
	strText.Replace(":o)", "happy smiley");
	strText.Replace(":-(", "sad smiley");
	strText.Replace(":(", "sad smiley");
	strText.Replace(":o(", "sad smiley");
	strText.Replace(":-O", "gasping smiley");
	strText.Replace(":O", "gasping smiley");
	strText.Replace(":-P", "smiley sticking tongue out");
	strText.Replace(":P", "smiley sticking tongue out");
	strText.Replace("brb", "Be right back");
	strText.Replace("lol", "laughing out loud");
	strText.Replace("rofl", "rolling on the floor laughing");
	strText.Replace("&", "and");
}

void CVoiceDlg::FixName(CString& strName)
{

	strName.Replace(".", " ");
	strName.Replace("-", " ");
	strName.Replace("_", " ");
	strName.Replace("(", " ");
	strName.Replace(")", " ");
	strName.Remove('0');
	strName.Remove('1');
	strName.Remove('2');
	strName.Remove('3');
	strName.Remove('4');
	strName.Remove('5');
	strName.Remove('6');
	strName.Remove('7');
	strName.Remove('8');
	strName.Remove('9');
	strName.Replace("&", "and");
}

void CVoiceDlg::OnBnClickedSetup()
{

	CVoiceSetup dlg;
	if(dlg.DoModal() == IDOK){

		AfxMessageBox("You have to restart RoboMX to take your changes into effect", MB_ICONINFORMATION);
	}
}

#endif
