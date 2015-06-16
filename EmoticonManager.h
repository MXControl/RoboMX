/*
** Copyright (c) Shareaza Development Team, 2002-2004.
** This file is part of SHAREAZA (www.shareaza.com)
**
** Modifications Copyright (C) 2004-2005 Bender
**  
** RoboMX is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** RoboMX is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS forA PARTICULAR PURPOSE.  See the
** GNU General Public License formore details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; ifnot, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#pragma once

class CRichDocument;

class CEmoticon
{
public:
	CEmoticon(){ hBitmap = 0; hMenuBitmap = 0;}
	~CEmoticon(){ if(hBitmap) DeleteObject(hBitmap); if(hMenuBitmap) DeleteObject(hMenuBitmap); }

	HBITMAP hBitmap;
	HBITMAP hMenuBitmap;
	char szFileName[1024];
	char szActivationText[64];
};

class CEmoticonManager
{

public:
	CEmoticonManager(void);
	~CEmoticonManager(void);

public:
	BOOL	Load(void);
	BOOL	Free(void);
	LPCTSTR	FindNext(LPCTSTR pszText, int* pnIndex);
	int		Lookup(LPCTSTR pszText, int nLen = -1) const;
	LPCTSTR	GetText(int nIndex) const;
	void	BuildTokens();
	void	FormatText(CRichDocument* pDocument, LPCTSTR pszBody, BOOL bNewlines = FALSE, BOOL bBold = TRUE, BOOL bItalic = FALSE, BOOL bUnderLine = FALSE, COLORREF cr = 0, COLORREF crBg = 0xFFFFFF, BOOL bName = FALSE, BOOL bExt = FALSE);
	static	HBITMAP ReplaceColor(HBITMAP hBmp,COLORREF cOldColor,COLORREF cNewColor,HDC hBmpDC);

public:
	CArray<CEmoticon*, CEmoticon*> m_aEmoticons;
	LPTSTR		m_pTokens;

protected:
	void AddEmoticon(char* szFileName, char* szActivationText);
};
