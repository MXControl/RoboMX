#pragma once


// CCfgSelector

class CCfgSelector : public CListCtrl
{
	DECLARE_DYNAMIC(CCfgSelector)

public:
	CCfgSelector();
	virtual ~CCfgSelector();

protected:
	afx_msg void OnCustomdrawList (NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};


