// AddCache.cpp : implementation file
//

#include "stdafx.h"
#include "RoboMX.h"
#include "AddCache.h"


// CAddCache dialog

IMPLEMENT_DYNAMIC(CAddCache, CDialog)
CAddCache::CAddCache(CWnd* pParent /*=NULL*/)
	: CDialog(CAddCache::IDD, pParent)
	, m_strLogin(_T(""))
{
}

CAddCache::~CAddCache()
{
}

void CAddCache::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ROOM, m_strRoom);
	DDX_Text(pDX, IDC_TOPIC, m_strTopic);
	DDX_Text(pDX, IDC_USERS, m_strUsers);
	DDX_Text(pDX, IDC_LIMIT, m_strLimit);
	DDX_Text(pDX, IDC_LOGIN, m_strLogin);
}


BEGIN_MESSAGE_MAP(CAddCache, CDialog)
END_MESSAGE_MAP()


// CAddCache message handlers
