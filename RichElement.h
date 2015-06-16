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
class CRichFragment;
class CRichEditExCtrl;

class CRichElement
{
public:
	CRichElement(int nType = 0, LPCTSTR pszText = NULL, LPCTSTR pszLink = NULL, DWORD nFlags = 0, int nGroup = 0);
	virtual ~CRichElement();
	
// Attributes
public:
	CRichDocument*	m_pDocument;
	int				m_nType;
	int				m_nGroup;
	DWORD			m_nFlags;
	CString			m_sText;
	CString			m_sLink;
	DWORD			m_hImage;
	COLORREF		m_cColor;
	COLORREF		m_cBgColor;

// Operations
public:
	void	Show(BOOL bShow = TRUE);
	void	SetText(LPCTSTR pszText);
	void	SetFlags(DWORD nFlags, DWORD nMask = 0xFFFFFFFF);
	void	Delete();
protected:
	void	PrePaint(CDC* pDC, BOOL bHover);
	void	PrePaintBitmap(CDC* pDC);
	CSize	GetSize();
	
	friend class CRichFragment;
	friend class CRichEditExCtrl;
};

enum TAG_RICHFRAGMENTS
{
	retNull, retNewline, retGap, retAlign,
	retBitmap, retAnchor, retCmdIcon,
	retText, retLink, retName, retHeading
};

enum TAG_RICHFORMATS
{
	retfNull		= 0x00,
	retfBold		= 0x01,
	retfItalic		= 0x02,
	retfUnderline	= 0x04,
	retfHeading		= 0x08,
	retfMiddle		= 0x10,
	retfColor		= 0x20,
	retfHidden		= 0x80,
};

enum TAG_RICHALIGNS
{
	reaLeft, reaCenter, reaRight
};
