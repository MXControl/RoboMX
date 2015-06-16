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
#include "MyListCtrl.h"
#include <Winsock2.h>
#include "settings.h"
#include ".\mylistctrl.h"
#include "util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl
LPCTSTR g_pszSection = _T("ListCtrls");

extern CSettings g_sSettings;

struct ItemData
{
public:
	ItemData() : arrpsz(NULL), dwData(NULL), wMode(NULL), bCached(FALSE) {}

	LPTSTR* arrpsz;
	DWORD dwData;
	WORD  wMode;
	BOOL  bCached;

private:
	// ban copying.
	ItemData(const ItemData&);
	ItemData& operator=(const ItemData&);
};

CMyListCtrl::CMyListCtrl()
	: m_nNumColumns(0)
    , m_nSortColumn(-1)
	, m_bSortAscending(TRUE)
	, m_bInited(false)
	, m_bUserList(FALSE)
	, m_bChannelList(FALSE)
	, m_cr1(RGB(0,0,0))
	, m_cr2(RGB(200,0,0))
	, m_cr3(RGB(0,150,0))
	, m_cr4(RGB(0,0,150))
	, m_cr5(RGB(0,0,180))
	, m_crHiLite(RGB(255,128, 128))
	, m_crHiLiteTxt(RGB(255,255,255))
	, m_bHiLite(FALSE)
	, m_crGrid(RGB(231, 245, 255))
	, m_crGridTxt(RGB(0,0,0))
{

}


CMyListCtrl::~CMyListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CMyListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	ON_NOTIFY_REFLECT (NM_CUSTOMDRAW, OnCustomdrawMyList)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyListCtrl message handlers

void CMyListCtrl::OnCustomdrawMyList(NMHDR* pNMHDR, LRESULT* pResult)
{

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
    // Take the default processing unless we set this to something else below.
    *pResult = CDRF_DODEFAULT;

    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.

    if(pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT){

        *pResult = CDRF_NOTIFYITEMDRAW;
    }
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.

		if((GetStyle()&LVS_LIST)==LVS_LIST){

			pLVCD->clrTextBk = m_crBg;
			//COLORREF crText  = m_cr1;
			// Store the color back in the NMLVCUSTOMDRAW struct.
			pLVCD->clrText =  m_cr1;

			// Tell Windows to paint the control itself.
			*pResult = CDRF_DODEFAULT;
		}
		else{

			*pResult = CDRF_NOTIFYSUBITEMDRAW;
		}
	}
    else if(pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM)){

        // This is the prepaint stage for an item. Here's where we set the
        // item's text color. Our return value will tell Windows to draw the
        // item itself, but it will use the new color we set here.
        // We'll cycle the colors through red, green, and light blue.

		pLVCD->clrTextBk = m_crBg;
		COLORREF crText  = m_cr1;

		if((GetExtendedStyle() & LVS_EX_GRIDLINES) != LVS_EX_GRIDLINES){

			if(pLVCD->nmcd.dwItemSpec%2 == 0){

				// Draw every other line in the grid-color
				pLVCD->clrTextBk = m_crGrid;
				crText   = m_crGridTxt;
			}
		}
		if(m_bHiLite){
	
			// Check if the item's text is to be highlighted or not
			for(int i = 0; i < g_sSettings.m_aHilite.GetSize(); i++){

				if(
					Util::WildcardMatch(GetItemText(pLVCD->nmcd.dwItemSpec, pLVCD->iSubItem),
										g_sSettings.m_aHilite[i], TRUE, FALSE)
					){
				
					pLVCD->clrTextBk = m_crHiLite;
					crText   = m_crHiLiteTxt;  
					
					break;
				}
			}
		}
		
		if(pLVCD->iSubItem == m_nSortColumn){

			DWORD dwColNormalColor = pLVCD->clrTextBk;

			// emulate sort column coloring of Windows XP explorer
			UINT nRed   = GetRValue(dwColNormalColor);
			UINT nGreen = GetGValue(dwColNormalColor);
			UINT nBlue  = GetBValue(dwColNormalColor);

			if (nRed > 240 && nGreen > 240 && nBlue > 240)
			{
				nRed   -= 8;
				nGreen -= 8;
				nBlue  -= 8;
			}
			else
			{
				if (nRed   < 232) nRed   += nRed   / 10; else nRed   = 255;
				if (nGreen < 232) nGreen += nGreen / 10; else nGreen = 255;
				if (nBlue  < 232) nBlue  += nBlue  / 10; else nBlue  = 255;
			}
			pLVCD->clrTextBk = RGB(nRed, nGreen, nBlue);
		}
        if(pLVCD->iSubItem == 1){ // files

            crText = m_cr2;
		}
        else if(pLVCD->iSubItem == 2){ // line

            crText = m_cr3;
		}
        else if(pLVCD->iSubItem == 3){ // ip

            crText = m_cr4;
		}
        else if(pLVCD->iSubItem == 4){ // port

            crText = m_cr5;
		}

		if(m_bChannelList){

			if(GetItemCached(pLVCD->nmcd.dwItemSpec)){
			
				DWORD dwColNormalColor = pLVCD->clrTextBk;

				// emulate sort column coloring of Windows XP explorer
				UINT nRed   = GetRValue(dwColNormalColor);
				UINT nGreen = GetGValue(dwColNormalColor);
				UINT nBlue  = GetBValue(dwColNormalColor);

				if (nRed > 200 && nGreen > 200 && nBlue > 200)
					crText = RGB(120, 120, 120);
				else
					crText = RGB(180, 180, 180);
			}
		}
        // Store the color back in the NMLVCUSTOMDRAW struct.
        pLVCD->clrText = crText;

        // Tell Windows to paint the control itself.
        *pResult = CDRF_DODEFAULT;
	}
}


int CMyListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CMyListCtrl::SetColors(COLORREF cr1, COLORREF cr2, COLORREF cr3, COLORREF cr4, COLORREF cr5)
{

	m_cr1 = cr1;
	m_cr2 = cr2;
	m_cr3 = cr3;
	m_cr4 = cr4;
	m_cr5 = cr5;
}

void CMyListCtrl::SetBkColor(COLORREF cr)
{

	CListCtrl::SetBkColor(cr);
	
	m_crBg			= cr;
	m_crHiLite		= g_sSettings.GetRGBHiLite();
	m_crHiLiteTxt	= g_sSettings.GetRGBHiLiteTxt();
	m_crGrid		= g_sSettings.GetRGBGrid();
	m_crGridTxt		= g_sSettings.GetRGBGridTxt();
}


void CMyListCtrl::PreSubclassWindow()
{

	// the list control must have the report style.
	ASSERT(GetStyle() & LVS_REPORT);

	CListCtrl::PreSubclassWindow();
}


BOOL CMyListCtrl::SetHeadings(UINT uiStringID)
{

	CString strHeadings;
	VERIFY(strHeadings.LoadString(uiStringID));
	return SetHeadings(strHeadings);
}


// the heading text is in the format column 1 text,column 1 width;column 2 text,column 3 width;etc.
BOOL CMyListCtrl::SetHeadings(const CString& strHeadings)
{

	int nStart = 0;

	for(;;)
	{
		const int nComma = strHeadings.Find(_T(','), nStart);

		if(nComma == -1)
			break;

		const CString strHeading = strHeadings.Mid(nStart, nComma - nStart);

		nStart = nComma + 1;

		int nSemiColon = strHeadings.Find(_T(';'), nStart);

		if(nSemiColon == -1)
			nSemiColon = strHeadings.GetLength();

		const int iWidth = atoi(strHeadings.Mid(nStart, nSemiColon - nStart));
		
		nStart = nSemiColon + 1;

		if(InsertColumn(m_nNumColumns++, strHeading, LVCFMT_LEFT, iWidth) == -1)
			return FALSE;
	}

	return TRUE;
}


int CMyListCtrl::AddItem(int nIcon, LPCTSTR pszText, ...)
{

	const int nIndex = InsertItem(GetItemCount(), pszText, nIcon);

	WORD wMode = (WORD)nIcon;
	//if(m_bChannelList) nIcon = 0;

	LPTSTR* arrpsz = new LPTSTR[ m_nNumColumns ];
	arrpsz[ 0 ] = new TCHAR[ lstrlen(pszText) + 1 ];
	(void)lstrcpy(arrpsz[ 0 ], pszText);

 	va_list list;
	va_start(list, pszText);

	for(int nColumn = 1; nColumn < m_nNumColumns; nColumn++){

		pszText = va_arg(list, LPCTSTR);
		ASSERT_VALID_STRING(pszText);
		VERIFY(CListCtrl::SetItem(nIndex, nColumn, LVIF_TEXT|LVIF_IMAGE, pszText, nIcon, 0, 0, 0));

		arrpsz[ nColumn ] = new TCHAR[ lstrlen(pszText) + 1 ];
		(void)lstrcpy(arrpsz[ nColumn ], pszText);
	}

	va_end(list);

	VERIFY(SetTextArray(nIndex, arrpsz, wMode));

	return nIndex;
}


void CMyListCtrl::FreeItemMemory(const int nItem)
{

	ItemData* pid = reinterpret_cast<ItemData*>(CListCtrl::GetItemData(nItem));

	LPTSTR* arrpsz = pid->arrpsz;

	for(int i = 0; i < m_nNumColumns; i++){

		delete[] arrpsz[ i ];
	}

	delete[] arrpsz;
	delete pid;

	VERIFY(CListCtrl::SetItemData(nItem, NULL));
}


BOOL CMyListCtrl::DeleteItem(int nItem)
{

	FreeItemMemory(nItem);
	return CListCtrl::DeleteItem(nItem);
}


BOOL CMyListCtrl::DeleteAllItems()
{

	for(int nItem = 0; nItem < GetItemCount(); nItem ++){
		
		FreeItemMemory(nItem);
	}

	return CListCtrl::DeleteAllItems();
}


bool IsTime(LPCTSTR pszText)
{

	ASSERT_VALID_STRING(pszText);

	BOOL bTime = FALSE;	// time string may have one : and only numbers

	for(int i = 0; i < lstrlen(pszText); i++){

		if(!_istdigit(pszText[i])){

			if((pszText[i] == ':') && !bTime){

				bTime = TRUE;
			}
			else{

				return false;   // string has char or more then one : thus is no time
			}
		}
	}

	return true;
}

int TimeCompare(const CString&  strTime1, const CString& strTime2)
{

	int nSeconds1 = 0, nSeconds2 = 0;

	nSeconds1  = atoi(strTime1.Mid(strTime1.GetLength()-2));	
	nSeconds2  = atoi(strTime2.Mid(strTime2.GetLength()-2));
	nSeconds1 += atoi(strTime1.Left(strTime1.GetLength()-3)) * 60;
	nSeconds2 += atoi(strTime2.Left(strTime2.GetLength()-3)) * 60;

	if(nSeconds1 < nSeconds2){

		return -1;
	}
	if(nSeconds1 > nSeconds2){

		return 1;
	}

	return 0;
}

bool IsNumber(LPCTSTR pszText)
{

	ASSERT_VALID_STRING(pszText);

	BOOL bDecDel = FALSE;

	for(int i = 0; i < lstrlen(pszText); i++){

		if(!_istdigit(pszText[i])){

			if((pszText[i] == '.') && !bDecDel){

				bDecDel = TRUE;
			}
			else{

				return false;
			}
		}
	}

	return true;
}


int NumberCompare(LPCTSTR pszNumber1, LPCTSTR pszNumber2)
{

	ASSERT_VALID_STRING(pszNumber1);
	ASSERT_VALID_STRING(pszNumber2);

	const float fNumber1 = (float)atof(pszNumber1);
	const float fNumber2 = (float)atof(pszNumber2);

	if(fNumber1 < fNumber2){

		return -1;
	}
	
	if(fNumber1 > fNumber2){

		return 1;
	}

	return 0;
}

bool IsIPAddr(LPCSTR pszText)
{
    ASSERT_VALID_STRING(pszText);
    
	if(inet_addr(pszText) != INADDR_NONE){

        return true;
	}
	else{

        return false;
	}
}

int IPAddrCompare(LPCTSTR pszIP1, LPCTSTR pszIP2)
{

    ASSERT_VALID_STRING(pszIP1);
    ASSERT_VALID_STRING(pszIP2);

    long lIP1 = inet_addr(pszIP1);
    long lIP2 = inet_addr(pszIP2);

	if( lIP1 < lIP2 ){

        return -1;
	}
	if( lIP1 > lIP2 ){

        return 1;    
	}
    
    return 0;
}

int CALLBACK CMyListCtrl::CompareFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamData)
{

	CMyListCtrl* pListCtrl = reinterpret_cast<CMyListCtrl*>(lParamData);
	ASSERT(pListCtrl->IsKindOf(RUNTIME_CLASS(CListCtrl)));

	ItemData* pid1 = reinterpret_cast<ItemData*>(lParam1);
	ItemData* pid2 = reinterpret_cast<ItemData*>(lParam2);

	ASSERT(pid1);
	ASSERT(pid2);

	LPCTSTR pszText1 = pid1->arrpsz[ pListCtrl->m_nSortColumn ];
	LPCTSTR pszText2 = pid2->arrpsz[ pListCtrl->m_nSortColumn ];

	ASSERT_VALID_STRING(pszText1);
	ASSERT_VALID_STRING(pszText2);

	if((pid1->wMode != pid2->wMode) && g_sSettings.GetOpsTop() && pListCtrl->m_bUserList){

		//-1 is p1 preceed p2;
		//1 is p1 follow p2
		int nResult = 0;
		if(pid1->wMode == 3){

			return -1;
		}
		else if(pid2->wMode == 3){

			return 1;
		}
		else if((pid1->wMode == 2) && (pid2->wMode == 1)){

			nResult = 1;
		}
		else if(pid1->wMode == 0){

			nResult = 1;
		}
		else if(pid2->wMode == 0){

			nResult = -1;
		}
		return nResult;
	}
	if((pid1->wMode != pid2->wMode) && pListCtrl->m_bChannelList && g_sSettings.GetHiliteTop()){

		//-1 is p1 preceed p2;
		//1 is p1 follow p2
		return pid1->wMode > pid2->wMode ? -1 : 1;
	}
	else{

		if(IsNumber(pszText1)){

			return pListCtrl->m_bSortAscending ? NumberCompare(pszText1, pszText2) : NumberCompare(pszText2, pszText1);
		}
		else if(IsTime(pszText1)){

			return pListCtrl->m_bSortAscending ? TimeCompare(pszText1, pszText2) :  TimeCompare(pszText2, pszText1);
		}
		else if (IsIPAddr(pszText1)){

			return pListCtrl->m_bSortAscending ? IPAddrCompare(pszText1, pszText2) : IPAddrCompare(pszText2, pszText1);
		}
		else{

			// text.
			return pListCtrl->m_bSortAscending ? lstrcmpi(pszText1, pszText2) : lstrcmpi(pszText2, pszText1);
		}

	}
}


void CMyListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	const int nColumn = pNMListView->iSubItem;

	// if it's a second click on the same column then reverse the sort order,
	// otherwise sort the new column in ascending order.
	Sort(nColumn, nColumn == m_nSortColumn ? !m_bSortAscending : TRUE);

	*pResult = 0;
}


void CMyListCtrl::Sort(int nColumn, BOOL bAscending)
{

	m_nSortColumn = nColumn;
	m_bSortAscending = bAscending;

	VERIFY(SortItems(CompareFunction, reinterpret_cast<DWORD>(this)));
}


void CMyListCtrl::Sort(void)
{

	if(m_nSortColumn == -1){

		m_nSortColumn = 0;
	}
	Sort(m_nSortColumn, m_bSortAscending);
}

void CMyListCtrl::LoadColumnInfo()
{

	m_bInited = true;
}


void CMyListCtrl::SaveColumnInfo()
{

	if(!m_bInited) return;
}


void CMyListCtrl::OnDestroy() 
{

	for(int nItem = 0; nItem < GetItemCount(); nItem ++){

		FreeItemMemory(nItem);
	}

	CListCtrl::OnDestroy();
}


BOOL CMyListCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{

	if(!CListCtrl::SetItemText(nItem, nSubItem, lpszText)){

		return FALSE;
	}

	LPTSTR* arrpsz = GetTextArray(nItem);
	LPTSTR pszText = arrpsz[ nSubItem ];
	delete[] pszText;
	pszText = new TCHAR[ lstrlen(lpszText) + 1 ];
	(void)lstrcpy(pszText, lpszText);
	arrpsz[ nSubItem ] = pszText;

	return TRUE;
}


BOOL CMyListCtrl::SetItemData(int nItem, DWORD dwData)
{

	if(nItem >= GetItemCount()){

		return FALSE;
	}

	ItemData* pid = reinterpret_cast<ItemData*>(CListCtrl::GetItemData(nItem));
	ASSERT(pid);
	pid->dwData = dwData;

	return TRUE;
}


DWORD CMyListCtrl::GetItemData(int nItem) const
{

	ASSERT(nItem < GetItemCount());

	ItemData* pid = reinterpret_cast<ItemData*>(CListCtrl::GetItemData(nItem));
	ASSERT(pid);
	return pid->dwData;
}

BOOL CMyListCtrl::SetItemCached(int nItem, BOOL bCached)
{

	if(nItem >= GetItemCount()){

		return FALSE;
	}

	ItemData* pid = reinterpret_cast<ItemData*>(CListCtrl::GetItemData(nItem));
	ASSERT(pid);
	pid->bCached = bCached;

	return TRUE;
}

BOOL CMyListCtrl::GetItemCached(int nItem) const
{

	ASSERT(nItem < GetItemCount());

	ItemData* pid = reinterpret_cast<ItemData*>(CListCtrl::GetItemData(nItem));
	ASSERT(pid);
	return pid->bCached;
}

BOOL CMyListCtrl::SetItemMode(int nItem, WORD wMode)
{

	if(nItem >= GetItemCount()){

		return FALSE;
	}

	ItemData* pid = reinterpret_cast<ItemData*>(CListCtrl::GetItemData(nItem));
	ASSERT(pid);
	pid->wMode = wMode;

	return TRUE;
}

WORD CMyListCtrl::GetItemMode(int nItem) const
{

	ASSERT(nItem < GetItemCount());

	ItemData* pid = reinterpret_cast<ItemData*>(CListCtrl::GetItemData(nItem));
	ASSERT(pid);
	return pid->wMode;
}

BOOL CMyListCtrl::SetTextArray(int nItem, LPTSTR* arrpsz, WORD wMode)
{

	ASSERT(CListCtrl::GetItemData(nItem) == NULL);
	ItemData* pid = new ItemData;
	pid->arrpsz = arrpsz;
	pid->wMode  = wMode;
	return CListCtrl::SetItemData(nItem, reinterpret_cast<DWORD>(pid));
}


LPTSTR* CMyListCtrl::GetTextArray(int nItem) const
{

	ASSERT(nItem < GetItemCount());

	ItemData* pid = reinterpret_cast<ItemData*>(CListCtrl::GetItemData(nItem));
	return pid->arrpsz;
}

void CMyListCtrl::SetHiLite(BOOL bHiLite)
{

	m_bHiLite = bHiLite;
}

int CMyListCtrl::SafeFind(LPCTSTR lpszItem, int nStart)
{

	if(nStart >= GetItemCount()) nStart = 0;

	for(int i = nStart; i < GetItemCount(); i++){

		if(GetItemText(i, 0).Compare(lpszItem) == 0){

			return i;
		}
	}

	return -1;
}

int CMyListCtrl::SafeSearch(CString strItem, int nStart, BOOL bExactStart, BOOL bWildCards)
{

	if(nStart >= GetItemCount()) nStart = 0;

	strItem.MakeLower();
	CString strText;
	for(int i = nStart; i < GetItemCount(); i++){

		strText = GetItemText(i, 0);
		strText.MakeLower();
		if(bExactStart && !bWildCards){	// no wildcards, start force start
			if(strText.Find(strItem, 0) == 0)
				return i;
		}
		else if(!bExactStart && !bWildCards){   // no wildcards,normal search
			if(strText.Find(strItem, 0) >= 0)
            	return i;
		}
		else if(bExactStart && bWildCards){ // wildcards, but force start
			if(Util::WildcardMatch(strText, strItem, FALSE, FALSE))
            	return i;
		}
		else if(!bExactStart && bWildCards){ // wildcard search
			if(Util::WildcardMatch(strText, strItem, TRUE, FALSE))
            	return i;
		}
	}

	return -1;
}


