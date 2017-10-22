#pragma once
class cNode
{
public:
	cNode();
	~cNode();

	enum eNodeType
	{
		E_START,
		E_DEST,
		E_EMPTY,
		E_WALL,
		E_PATH,
		E_OPEN,
		E_CLOSE
	};

	float		F;
	float		G;
	float		H;
	cNode*		m_pParent;
	eNodeType	m_eNodeType;
	RECT		m_rc;
	bool		m_isOpen;
	bool		m_isClose;
	int			m_nRow;
	int			m_nCol;
	void Setup(int nIndex, eNodeType e);
	void Render(HDC hdc);
};

