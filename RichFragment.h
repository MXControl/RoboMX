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

class CRichElement;
class CRichEditExCtrl;

class CRichFragment
{
public:
	CRichFragment(CRichElement* pElement, int nOffset, int nLength, CPoint* pPoint, CSize* pSize);
	CRichFragment(CRichElement* pElement, CPoint* pPoint);
	virtual ~CRichFragment(void);

	// Attributes
public:
	CRichElement*	m_pElement;
	CPoint			m_pt;
	CSize			m_sz;
	WORD			m_nOffset;
	WORD			m_nLength;

// Operations
public:
	void	Add(int nLength, CSize* pSize);
	void	Paint(CDC* pDC, CRichEditExCtrl* pCtrl, int nFragment);
};
