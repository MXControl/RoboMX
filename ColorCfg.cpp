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
#include "ColorCfg.h"
#include "Settings.h"
#include ".\colorcfg.h"
#include "Util.h"
extern CSettings g_sSettings;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCfgColor dialog


CCfgColor::CCfgColor(CWnd* pParent /*=NULL*/)
	: CCfgDlg(CCfgColor::IDD, pParent)
	, m_bRawname(FALSE)
{
	//{{AFX_DATA_INIT(CCfgColor)
	m_strActionEnd = _T("");
	m_strActionFront = _T("");
	m_strMsgEnd = _T("");
	m_strMsgFront = _T("");
	m_bFocus = FALSE;
	m_bIsExpert = FALSE;
	//}}AFX_DATA_INIT
}


void CCfgColor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_OPMSG, m_stOpMsg);
	DDX_Control(pDX, IDC_STATIC_FOCUS, m_stFocus);
	DDX_Control(pDX, IDC_STATIC_PENDING, m_stPend);
	DDX_Control(pDX, IDC_STATIC_SUCCESS, m_stOK);
	DDX_Control(pDX, IDC_STATIC_ERROR, m_stErr);
	DDX_Control(pDX, IDC_STATIC_LINE, m_stLine);
	DDX_Control(pDX, IDC_STATIC_FILES, m_stFiles);
	DDX_Control(pDX, IDC_STATIC_NODEIP, m_stIP);
	DDX_Control(pDX, IDC_STATIC_NODEPORT, m_stPort);
	DDX_Control(pDX, IDC_STATIC_BG, m_stBg);
	DDX_Control(pDX, IDC_STATIC_COLOR_MSG_PREV, m_stMsg);
	DDX_Control(pDX, IDC_STATIC_COLOR_ENC_MSG_PREV, m_stMsgEnc);
	DDX_Control(pDX, IDC_STATIC_JOIN, m_stJoin);
	DDX_Control(pDX, IDC_STATIC_MOTD, m_stMotd);
	DDX_Control(pDX, IDC_STATIC_PART, m_stPart);
	DDX_Control(pDX, IDC_STATIC_TOPIC, m_stTopic);
	DDX_Control(pDX, IDC_STATIC_DOCHI, m_stDocHi);
	DDX_Control(pDX, IDC_STATIC_COLOR_ENG_ACTION_PREV, m_stActEnc);
	DDX_Control(pDX, IDC_STATIC_COLOR_ACTION_PREV, m_stAction);
	DDX_Text(pDX, IDC_ACTION_END, m_strActionEnd);
	DDX_Text(pDX, IDC_ACTION_FRONT, m_strActionFront);
	DDX_Text(pDX, IDC_MSG_END, m_strMsgEnd);
	DDX_Text(pDX, IDC_MSG_FRONT, m_strMsgFront);
	DDX_Check(pDX, IDC_FOCUSCLR, m_bFocus);
	DDX_Check(pDX, IDC_BLOCKMCMA, m_bBlockMCMA);
	DDX_Control(pDX, IDC_STATIC_COLOR_NAME_PREV, m_stName);
	DDX_Control(pDX, IDC_STATIC_TIME, m_stTime);
	DDX_Control(pDX, IDC_STATIC_HILITE, m_stHiLite);
	DDX_Control(pDX, IDC_STATIC_HILITE_TXT, m_stHiLiteTxt);
	DDX_Control(pDX, IDC_STATIC_GRID_BG, m_stGrid);
	DDX_Control(pDX, IDC_STATIC_GRID_TXT, m_stGridTxt);
	DDX_Control(pDX, IDC_STATIC_LINK, m_stLink);
	DDX_Control(pDX, IDC_MCMARAW, m_btRawname);
	DDX_Check(pDX, IDC_MCMARAW, m_bRawname);
}


BEGIN_MESSAGE_MAP(CCfgColor, CCfgDlg)
	ON_BN_CLICKED(IDC_ENC_MSG_COLOR, OnEncMsgColor)
	ON_BN_CLICKED(IDC_COLOR_MESSAGE, OnColorMessage)
	ON_BN_CLICKED(IDC_ENC_ACTION_COLOR, OnEncActionColor)
	ON_BN_CLICKED(IDC_COLOR_ACTION, OnColorAction)
	ON_BN_CLICKED(IDC_TOPIC, OnTopic)
	ON_BN_CLICKED(IDC_MOTD, OnMotd)
	ON_BN_CLICKED(IDC_JOIN, OnJoin)
	ON_BN_CLICKED(IDC_PART, OnPart)
	ON_BN_CLICKED(IDC_BACKG, OnBackg)
	ON_BN_CLICKED(IDC_LINE, OnLine)
	ON_BN_CLICKED(IDC_FILES, OnFiles)
	ON_BN_CLICKED(IDC_NODEIP, OnNodeip)
	ON_BN_CLICKED(IDC_PORT, OnPort)
	ON_BN_CLICKED(IDC_PENDING, OnPending)
	ON_BN_CLICKED(IDC_SUCCESS, OnSuccess)
	ON_BN_CLICKED(IDC_ERROR, OnError)
	ON_BN_CLICKED(IDC_COLORFOCUS, OnColorfocus)
	ON_BN_CLICKED(IDC_COLOR_NAME, OnBnClickedColorName)
	ON_BN_CLICKED(IDC_TIME, OnBnClickedTime)
	ON_BN_CLICKED(IDC_HILITE, OnBnClickedHilite)
	ON_BN_CLICKED(IDC_EDITHILITE, OnBnClickedEdithilite)
	ON_BN_CLICKED(IDC_DOCHI, OnBnClickedDochi)
	ON_BN_CLICKED(IDC_OPMSG, OnBnClickedOpmsg)
	ON_BN_CLICKED(IDC_LIGHT_DARK, OnBnClickedLightDark)
	ON_BN_CLICKED(IDC_DARK_LIGHT, OnBnClickedDarkLight)
	ON_BN_CLICKED(IDC_WINMX_LIKE, OnBnClickedWinmxLike)
	ON_BN_CLICKED(IDC_HILITE_TXT, OnBnClickedHiliteTxt)
	ON_BN_CLICKED(IDC_GRID_BG, OnBnClickedGridBg)
	ON_BN_CLICKED(IDC_GRID_TXT, OnBnClickedGridTxt)
	ON_BN_CLICKED(IDC_LINK, OnBnClickedLink)
	ON_BN_CLICKED(IDC_BLUE, OnBnClickedBlue)
	ON_BN_CLICKED(IDC_IMPORT, OnBnClickedImport)
	ON_BN_CLICKED(IDC_EXPORT, OnBnClickedExport)
	ON_BN_CLICKED(IDC_BLOCKMCMA, OnBnClickedBlockmcma)
	ON_BN_CLICKED(IDC_MCMARAW, OnBnClickedMcmaraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCfgColor message handlers

BOOL CCfgColor::OnInitDialog() 
{

	CCfgDlg::OnInitDialog();
	


	m_stJoin.SetBkColor(g_sSettings.GetRGBJoin());
	m_stMotd.SetBkColor(g_sSettings.GetRGBMotd());
	m_stMsg.SetBkColor(g_sSettings.GetRGBNormalMsg());
	m_stName.SetBkColor(g_sSettings.GetRGBNormalName());
	m_stMsgEnc.SetBkColor(g_sSettings.GetRGBBrMessage());
	m_stPart.SetBkColor(g_sSettings.GetRGBPart());
    m_stAction.SetBkColor(g_sSettings.GetRGBActionMsg());
	m_stActEnc.SetBkColor(g_sSettings.GetRGBBrAction());
	m_stTopic.SetBkColor(g_sSettings.GetRGBTopic());
	m_stBg.SetBkColor(g_sSettings.GetRGBBg());
	m_stFiles.SetBkColor(g_sSettings.GetRGBFiles());
	m_stLine.SetBkColor(g_sSettings.GetRGBLine());
	m_stIP.SetBkColor(g_sSettings.GetRGBIP());
	m_stPort.SetBkColor(g_sSettings.GetRGBPort());
	m_stOK.SetBkColor(g_sSettings.GetRGBOK());
	m_stPend.SetBkColor(g_sSettings.GetRGBPend());
	m_stErr.SetBkColor(g_sSettings.GetRGBErr());
	m_stFocus.SetBkColor(g_sSettings.GetRGBFocus());
	m_stTime.SetBkColor(g_sSettings.GetRGBTime());
	m_stHiLite.SetBkColor(g_sSettings.GetRGBHiLite());
	m_stHiLiteTxt.SetBkColor(g_sSettings.GetRGBHiLiteTxt());
	m_stGrid.SetBkColor(g_sSettings.GetRGBGrid());
	m_stGridTxt.SetBkColor(g_sSettings.GetRGBGridTxt());
	m_stDocHi.SetBkColor(g_sSettings.GetRGBDocHiLite());
	m_stOpMsg.SetBkColor(g_sSettings.GetRGBOp());
	m_stLink.SetBkColor(g_sSettings.GetRGBLink());

	m_strMsgFront		= g_sSettings.GetBrMsgFront();
	m_strMsgEnd			= g_sSettings.GetBrMsgEnd();
	m_strActionFront	= g_sSettings.GetBrActionFront();
	m_strActionEnd		= g_sSettings.GetBrActionEnd();
	m_bFocus			= g_sSettings.GetFocus();
	m_bBlockMCMA		= g_sSettings.GetBlockMCMA();
	m_bRawname			= g_sSettings.GetMCMARawname();
	m_btRawname.EnableWindow(m_bBlockMCMA);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCfgColor::Apply()
{

	UpdateData(TRUE);
	
	g_sSettings.SetRGBJoin(m_stJoin.GetBkColor());
	g_sSettings.SetRGBMotd(m_stMotd.GetBkColor());
	g_sSettings.SetRGBNormalName(m_stName.GetBkColor());
	g_sSettings.SetRGBNormalMsg(m_stMsg.GetBkColor());
	g_sSettings.SetRGBBrMessage(m_stMsgEnc.GetBkColor());
	g_sSettings.SetRGBPart(m_stPart.GetBkColor());
    g_sSettings.SetRGBActionMsg(m_stAction.GetBkColor());
	g_sSettings.SetRGBBrAction(m_stActEnc.GetBkColor());
	g_sSettings.SetRGBTopic(m_stTopic.GetBkColor());
	g_sSettings.SetRGBBg(m_stBg.GetBkColor());
	g_sSettings.SetRGBFiles(m_stFiles.GetBkColor());
	g_sSettings.SetRGBLine(m_stLine.GetBkColor());
	g_sSettings.SetRGBIP(m_stIP.GetBkColor());
	g_sSettings.SetRGBPort(m_stPort.GetBkColor());
	g_sSettings.SetRGBOK(m_stOK.GetBkColor());
	g_sSettings.SetRGBPend(m_stPend.GetBkColor());
	g_sSettings.SetRGBErr(m_stErr.GetBkColor());
	g_sSettings.SetRGBFocus(m_stFocus.GetBkColor());
	g_sSettings.SetRGBTime(m_stTime.GetBkColor());
	g_sSettings.SetRGBHiLite(m_stHiLite.GetBkColor());
	g_sSettings.SetRGBHiLiteTxt(m_stHiLiteTxt.GetBkColor());
	g_sSettings.SetRGBGrid(m_stGrid.GetBkColor());
	g_sSettings.SetRGBGridTxt(m_stGridTxt.GetBkColor());
	g_sSettings.SetRGBDocHiLite(m_stDocHi.GetBkColor());
	g_sSettings.SetRGBOp(m_stOpMsg.GetBkColor());
	g_sSettings.SetRGBLink(m_stLink.GetBkColor());

	g_sSettings.SetBrMsgFront(m_strMsgFront);
	g_sSettings.SetBrMsgEnd(m_strMsgEnd);
	g_sSettings.SetBrActionFront(m_strActionFront);
	g_sSettings.SetBrActionEnd(m_strActionEnd);
	g_sSettings.SetFocus(m_bFocus);
	g_sSettings.SetBlockMCMA(m_bBlockMCMA);
	g_sSettings.SetMCMARawname(m_bRawname);

}

void CCfgColor::OnBnClickedColorName()
{

	CColorDialog cDlg(m_stName.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stName.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnEncMsgColor() 
{
	
	CColorDialog cDlg(m_stMsgEnc.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stMsgEnc.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnColorMessage() 
{
	
	CColorDialog cDlg(m_stMsg.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stMsg.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnEncActionColor() 
{
	
	CColorDialog cDlg(m_stActEnc.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stActEnc.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnColorAction() 
{
	
	CColorDialog cDlg(m_stAction.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stAction.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnTopic() 
{
	
	CColorDialog cDlg(m_stTopic.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stTopic.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnMotd() 
{
	
	CColorDialog cDlg(m_stMotd.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stMotd.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnJoin() 
{
	
	CColorDialog cDlg(m_stJoin.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stJoin.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnPart() 
{
	
	CColorDialog cDlg(m_stPart.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stPart.SetBkColor(cDlg.GetColor());
	}
}


void CCfgColor::OnLine() 
{
	
	CColorDialog cDlg(m_stLine.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stLine.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnFiles() 
{
	
	CColorDialog cDlg(m_stFiles.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stFiles.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnNodeip() 
{
	
	CColorDialog cDlg(m_stIP.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stIP.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnPort() 
{
	
	CColorDialog cDlg(m_stPort.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stPort.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnPending() 
{
	
	CColorDialog cDlg(m_stPend.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stPend.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnSuccess() 
{
	
	CColorDialog cDlg(m_stOK.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stOK.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnError() 
{
	
	CColorDialog cDlg(m_stErr.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stErr.SetBkColor(cDlg.GetColor());
	}
}


void CCfgColor::OnBnClickedOpmsg()
{

	CColorDialog cDlg(m_stOpMsg.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stOpMsg.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnColorfocus() 
{

	CColorDialog cDlg(m_stFocus.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stFocus.SetBkColor(cDlg.GetColor());
	}
}


void CCfgColor::OnBnClickedTime()
{

	CColorDialog cDlg(m_stTime.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stTime.SetBkColor(cDlg.GetColor());
	}
}


void CCfgColor::OnBnClickedHilite()
{

	CColorDialog cDlg(m_stHiLite.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stHiLite.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnBnClickedHiliteTxt()
{

	CColorDialog cDlg(m_stHiLiteTxt.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stHiLiteTxt.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnBnClickedGridBg()
{

	CColorDialog cDlg(m_stGrid.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stGrid.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnBnClickedGridTxt()
{

	CColorDialog cDlg(m_stGridTxt.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stGridTxt.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnBnClickedDochi()
{

	CColorDialog cDlg(m_stDocHi.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stDocHi.SetBkColor(cDlg.GetColor());
	}
}

void CCfgColor::OnBackg() 
{
	
	CColorDialog cDlg(m_stBg.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stBg.SetBkColor(cDlg.GetColor());
	}
}


void CCfgColor::OnBnClickedLink()
{

	CColorDialog cDlg(m_stLink.GetBkColor());
	
	if(cDlg.DoModal() == IDOK){

		m_stLink.SetBkColor(cDlg.GetColor());
	}
}


void CCfgColor::OnBnClickedEdithilite()
{

	char buffer[MAX_PATH+1];
	GetWindowsDirectory((LPSTR)&buffer, MAX_PATH+1);

	CString strExec, strParam;
	strExec.Format("%s\\notepad.exe", buffer);
	strParam.Format("%s\\hilite.ini", g_sSettings.GetWorkingDir());
	Util::ShellExecuteWait(strExec, strParam);
	g_sSettings.LoadHiLite();
	AfxMessageBox(IDS_HILITE_LOADED, MB_ICONINFORMATION);
}

void CCfgColor::OnBnClickedLightDark()	 // default RoboMX scheme :-P
{


	m_stLink.SetBkColor(RGB(0, 0, 255));
	m_stJoin.SetBkColor(RGB(255,150,0));
	m_stMotd.SetBkColor(RGB(0,0,180));
	m_stMsg.SetBkColor(RGB(0,0,0));
	m_stName.SetBkColor(RGB(255,0,0));
	m_stMsgEnc.SetBkColor(RGB(255,0,0));
	m_stPart.SetBkColor(RGB(120,0,0));
    m_stAction.SetBkColor(RGB(0,91,183));
	m_stActEnc.SetBkColor(RGB(0,91,183));
	m_stTopic.SetBkColor(RGB(0,0,120));
	m_stBg.SetBkColor(RGB(255,255,255));
	m_stFiles.SetBkColor(RGB(200,0,0));
	m_stLine.SetBkColor(RGB(0,150,0));
	m_stIP.SetBkColor(RGB(0,0,150));
	m_stPort.SetBkColor(RGB(0,0,180));
	m_stOK.SetBkColor(RGB(0,150,0));
	m_stPend.SetBkColor(RGB(254,128,64));
	m_stErr.SetBkColor(RGB(150,0,0));
	m_stFocus.SetBkColor(RGB(255,245,210));
	m_stTime.SetBkColor(RGB(0,128,128));
	m_stHiLite.SetBkColor(RGB(255,128,128));
	m_stHiLiteTxt.SetBkColor(RGB(255,255,255));
	m_stGrid.SetBkColor(RGB(231,245,255));
	m_stGridTxt.SetBkColor(RGB(0,0,0));
	m_stDocHi.SetBkColor(RGB(0,0,255));
	m_stOpMsg.SetBkColor(RGB(55,170,100));

	m_strMsgFront = "";
	m_strMsgEnd   = " :";
	m_strActionFront = "*  ";
	m_strActionEnd = "";
	m_bFocus = TRUE;
	UpdateData(FALSE);
}

void CCfgColor::OnBnClickedDarkLight()
{

	m_stLink.SetBkColor(RGB(0, 0, 255));
	m_stJoin.SetBkColor(RGB(180,230,125));
	m_stMotd.SetBkColor(RGB(0,0,250));
	m_stMsg.SetBkColor(RGB(255,255,255));
	m_stName.SetBkColor(RGB(134,194,164));
	m_stMsgEnc.SetBkColor(RGB(255,183,0));
	m_stPart.SetBkColor(RGB(255,156,156));
    m_stAction.SetBkColor(RGB(134,194,164));
	m_stActEnc.SetBkColor(RGB(255,183,0));
	m_stTopic.SetBkColor(RGB(0,0,230));
	m_stBg.SetBkColor(RGB(0,0,0));
	m_stFiles.SetBkColor(RGB(250,0,0));
	m_stLine.SetBkColor(RGB(0,255,0));
	m_stIP.SetBkColor(RGB(255,245,210));
	m_stPort.SetBkColor(RGB(255,245,210));
	m_stOK.SetBkColor(RGB(0,225,0));
	m_stPend.SetBkColor(RGB(254,128,64));
	m_stErr.SetBkColor(RGB(255,0,0));
	m_stFocus.SetBkColor(RGB(74,74,74));
	m_stTime.SetBkColor(RGB(255,183,0));
	m_stHiLite.SetBkColor(RGB(255,128,128));
	m_stHiLiteTxt.SetBkColor(RGB(255,255,255));
	m_stGrid.SetBkColor(RGB(45,45,45));
	m_stGridTxt.SetBkColor(RGB(255,255,255));
	m_stDocHi.SetBkColor(RGB(0,0,255));
	m_stOpMsg.SetBkColor(RGB(55,170,100));

	m_strMsgFront = "<";
	m_strMsgEnd   = ">";
	m_strActionFront = ">  ";
	m_strActionEnd = "";
	m_bFocus = TRUE;
	UpdateData(FALSE);
}

void CCfgColor::OnBnClickedWinmxLike()
{

	m_stLink.SetBkColor(RGB(0, 0, 255));
	m_stJoin.SetBkColor(RGB(128,255,0));
	m_stMotd.SetBkColor(RGB(128,255,250));
	m_stMsg.SetBkColor(RGB(255,255,255));
	m_stName.SetBkColor(RGB(255,255,0));
	m_stMsgEnc.SetBkColor(RGB(255,255,0));
	m_stPart.SetBkColor(RGB(255,128,128));
    m_stAction.SetBkColor(RGB(255,0,255));
	m_stActEnc.SetBkColor(RGB(255,0,255));
	m_stTopic.SetBkColor(RGB(0,0,255));
	m_stBg.SetBkColor(RGB(0,0,0));
	m_stFiles.SetBkColor(RGB(250,0,0));
	m_stLine.SetBkColor(RGB(255,255,0));
	m_stIP.SetBkColor(RGB(255,245,210));
	m_stPort.SetBkColor(RGB(255,245,210));
	m_stOK.SetBkColor(RGB(0,225,0));
	m_stPend.SetBkColor(RGB(254,128,64));
	m_stErr.SetBkColor(RGB(255,0,0));
	m_stFocus.SetBkColor(RGB(74,74,74));
	m_stTime.SetBkColor(RGB(0,0,0));
	m_stHiLite.SetBkColor(RGB(255,128,128));
	m_stHiLiteTxt.SetBkColor(RGB(255,255,255));
	m_stGrid.SetBkColor(RGB(0,0,0));
	m_stGridTxt.SetBkColor(RGB(255,255,255));
	m_stDocHi.SetBkColor(RGB(0,0,255));
	m_stOpMsg.SetBkColor(RGB(255,0,0));

	m_strMsgFront = "<";
	m_strMsgEnd   = ">";
	m_strActionFront = "";
	m_strActionEnd = "";
	m_bFocus = TRUE;
	UpdateData(FALSE);
}

void CCfgColor::OnBnClickedBlue()
{

	m_stLink.SetBkColor(RGB(0, 128, 255));
	m_stJoin.SetBkColor(RGB(128,255,0));
	m_stMotd.SetBkColor(RGB(128,255,250));
	m_stMsg.SetBkColor(RGB(255,255,255));
	m_stName.SetBkColor(RGB(255,255,0));
	m_stMsgEnc.SetBkColor(RGB(255,255,0));
	m_stPart.SetBkColor(RGB(255,128,128));
    m_stAction.SetBkColor(RGB(255,255,0));
	m_stActEnc.SetBkColor(RGB(255,255,0));
	m_stTopic.SetBkColor(RGB(0,0,255));
	m_stBg.SetBkColor(RGB(0,0,128));
	m_stFiles.SetBkColor(RGB(250,0,0));
	m_stLine.SetBkColor(RGB(255,255,0));
	m_stIP.SetBkColor(RGB(255,245,210));
	m_stPort.SetBkColor(RGB(255,245,210));
	m_stOK.SetBkColor(RGB(0,225,0));
	m_stPend.SetBkColor(RGB(254,128,64));
	m_stErr.SetBkColor(RGB(255,0,0));
	m_stFocus.SetBkColor(RGB(0,0,166));
	m_stTime.SetBkColor(RGB(0,0,128));
	m_stHiLite.SetBkColor(RGB(255,217,128));
	m_stHiLiteTxt.SetBkColor(RGB(0,0,128));
	m_stGrid.SetBkColor(RGB(0,0,85));
	m_stGridTxt.SetBkColor(RGB(255,255,255));
	m_stDocHi.SetBkColor(RGB(0,0,255));
	m_stOpMsg.SetBkColor(RGB(255,0,0));

	m_strMsgFront = "<";
	m_strMsgEnd   = ">";
	m_strActionFront = "";
	m_strActionEnd = "";
	m_bFocus = TRUE;
	UpdateData(FALSE);
}

void CCfgColor::OnBnClickedImport()
{

	CString strFilter;
	strFilter.LoadString(IDS_INI_FILTER);

	CFileDialog dlg(TRUE, ".ini", "", OFN_FILEMUSTEXIST, strFilter, this);

	if(dlg.DoModal() == IDOK){

		g_sSettings.LoadColorsFromFile(dlg.GetPathName());
		m_stJoin.SetBkColor(g_sSettings.GetRGBJoin());
		m_stMotd.SetBkColor(g_sSettings.GetRGBMotd());
		m_stMsg.SetBkColor(g_sSettings.GetRGBNormalMsg());
		m_stName.SetBkColor(g_sSettings.GetRGBNormalName());
		m_stMsgEnc.SetBkColor(g_sSettings.GetRGBBrMessage());
		m_stPart.SetBkColor(g_sSettings.GetRGBPart());
		m_stAction.SetBkColor(g_sSettings.GetRGBActionMsg());
		m_stActEnc.SetBkColor(g_sSettings.GetRGBBrAction());
		m_stTopic.SetBkColor(g_sSettings.GetRGBTopic());
		m_stBg.SetBkColor(g_sSettings.GetRGBBg());
		m_stFiles.SetBkColor(g_sSettings.GetRGBFiles());
		m_stLine.SetBkColor(g_sSettings.GetRGBLine());
		m_stIP.SetBkColor(g_sSettings.GetRGBIP());
		m_stPort.SetBkColor(g_sSettings.GetRGBPort());
		m_stOK.SetBkColor(g_sSettings.GetRGBOK());
		m_stPend.SetBkColor(g_sSettings.GetRGBPend());
		m_stErr.SetBkColor(g_sSettings.GetRGBErr());
		m_stFocus.SetBkColor(g_sSettings.GetRGBFocus());
		m_stTime.SetBkColor(g_sSettings.GetRGBTime());
		m_stHiLite.SetBkColor(g_sSettings.GetRGBHiLite());
		m_stHiLiteTxt.SetBkColor(g_sSettings.GetRGBHiLiteTxt());
		m_stGrid.SetBkColor(g_sSettings.GetRGBGrid());
		m_stGridTxt.SetBkColor(g_sSettings.GetRGBGridTxt());
		m_stDocHi.SetBkColor(g_sSettings.GetRGBDocHiLite());
		m_stOpMsg.SetBkColor(g_sSettings.GetRGBOp());
		m_stLink.SetBkColor(g_sSettings.GetRGBLink());
	}
}

void CCfgColor::OnBnClickedExport()
{

	CString strFilter;
	strFilter.LoadString(IDS_INI_FILTER);

	CFileDialog dlg(FALSE, ".ini", "", OFN_HIDEREADONLY, strFilter, this);

	if(dlg.DoModal() == IDOK){

		Apply();
		g_sSettings.SaveColorsToFile(dlg.GetPathName());
	}
}

void CCfgColor::OnBnClickedBlockmcma()
{

	if(UpdateData(TRUE)){

		m_btRawname.EnableWindow(m_bBlockMCMA);
	}

}

void CCfgColor::OnBnClickedMcmaraw()
{

	AfxMessageBox(IDS_MCMARAWNAMEWARNING, MB_OK);
}
