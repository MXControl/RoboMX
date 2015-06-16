// CfgSelector.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "CfgSelector.h"


// CCfgSelector

IMPLEMENT_DYNAMIC(CCfgSelector, CListCtrl)
CCfgSelector::CCfgSelector()
{
}

CCfgSelector::~CCfgSelector()
{
}


BEGIN_MESSAGE_MAP(CCfgSelector, CListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdrawList)
END_MESSAGE_MAP()



// CCfgSelector message handlers
void CCfgSelector::OnCustomdrawList (NMHDR* pNMHDR, LRESULT* pResult)
{

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	*pResult = 0;

	if(CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage){

		//request notifications for each item.
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage){

		// request to be notified during the post-paint stage.
		*pResult = CDRF_NOTIFYPOSTPAINT;
	}
	else if (CDDS_ITEMPOSTPAINT == pLVCD->nmcd.dwDrawStage){

		// If this item is selected, re-draw the icon in its normal
		// color (not blended with the highlight color).
		LVITEM rItem;
		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);

		// Get the image index and state of this item.  Note that we need to
		// check the selected state manually.  The docs _say_ that the
		// item's state is in pLVCD->nmcd.uItemState, but during my testing
		// it was always equal to 0x0201, which doesn't make sense, since
		// the max CDIS_* constant in commctrl.h is 0x0100.
		// /me makes a rude gesture
		ZeroMemory(&rItem, sizeof(LVITEM));
		rItem.mask  = LVIF_IMAGE | LVIF_STATE;
		rItem.iItem = nItem;
		rItem.stateMask = LVIS_SELECTED;
		GetItem (&rItem);

		// If this item is selected, redraw the icon with its normal colors.
		if (rItem.state & LVIS_SELECTED){

			CDC* pDC = CDC::FromHandle (pLVCD->nmcd.hdc);
			CRect rcIcon;

			GetItemRect(nItem, &rcIcon, LVIR_ICON);
			GetImageList(LVSIL_SMALL)->Draw(pDC, rItem.iImage, rcIcon.TopLeft(), ILD_TRANSPARENT);

			*pResult = CDRF_SKIPDEFAULT;
		}
	}
}
