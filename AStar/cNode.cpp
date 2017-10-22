#include "stdafx.h"
#include "cNode.h"


cNode::cNode()
{
}


cNode::~cNode()
{
}

void cNode::Setup(int nIndex, eNodeType e)
{
	F = 0.0f;
	G = 0.0f;
	H = 0.0f;
	m_pParent = NULL;
	m_eNodeType = e;
	m_nRow = nIndex / TILE_N;
	m_nCol = nIndex % TILE_N;
	SetRect(&m_rc,
		m_nCol * TILE_SIZE,
		m_nRow * TILE_SIZE,
		(m_nCol + 1) * TILE_SIZE,
		(m_nRow + 1) * TILE_SIZE);

	m_isOpen = false;
	m_isClose = false;
}

void cNode::Render(HDC hdc)
{
	COLORREF Color;
	switch (m_eNodeType)
	{
	case E_EMPTY:
		Color = RGB(255, 255, 255);
		break;
	case E_WALL:
		Color = RGB(128, 128, 128);
		break;
	case E_PATH:
		Color = RGB(255, 255, 0);
		break;
	case E_CLOSE:
		Color = RGB(255, 0, 0);
		break;
	case E_OPEN:
		Color = RGB(0, 255, 0);
		break;
	default:
		Color = RGB(255, 255, 255);
		break;
	}
	HBRUSH hBrush = CreateSolidBrush(Color);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);

	Rectangle(hdc, m_rc.left, m_rc.top, m_rc.right, m_rc.bottom);

	SelectObject(hdc, hOldBrush);
	DeleteObject(hBrush);
}
