#include "stdafx.h"
#include "cMainGame.h"

cMainGame::cMainGame()
{
}


cMainGame::~cMainGame()
{
}

void cMainGame::Setup()
{
	std::map<char, cNode::eNodeType> mapNodeType;
	mapNodeType['E'] = cNode::E_EMPTY;
	mapNodeType['W'] = cNode::E_WALL;
	mapNodeType['S'] = cNode::E_START;
	mapNodeType['D'] = cNode::E_DEST;


	m_vecDirection.push_back(E_LEFT);
	m_vecDirection.push_back(E_RIGHT);
	m_vecDirection.push_back(E_UP);
	m_vecDirection.push_back(E_DOWN);
	m_vecDirection.push_back(E_LEFT | E_UP);
	m_vecDirection.push_back(E_LEFT | E_DOWN);
	m_vecDirection.push_back(E_RIGHT | E_UP);
	m_vecDirection.push_back(E_RIGHT | E_DOWN);


	m_mapDistance[E_LEFT] = 1;
	m_mapDistance[E_RIGHT] = 1;
	m_mapDistance[E_UP] = 1;
	m_mapDistance[E_DOWN] = 1;
	m_mapDistance[E_LEFT | E_UP] = sqrt(2.0f);
	m_mapDistance[E_LEFT | E_DOWN] = sqrt(2.0f);
	m_mapDistance[E_RIGHT | E_UP] = sqrt(2.0f);
	m_mapDistance[E_RIGHT | E_DOWN] = sqrt(2.0f);

	FILE* fp = NULL;
	fopen_s(&fp, "data.txt", "r");
	while (!feof(fp))
	{
		char c = fgetc(fp);
		if (c < 33) continue;
		int nIndex = m_vecNode.size();
		cNode* pNode = new cNode;
		pNode->Setup(nIndex, mapNodeType[c]);
		m_vecNode.push_back(pNode);
	}
	fclose(fp);

	FindPath();
}

void cMainGame::FindPath()
{
	cNode* pStartNode = FindNode(cNode::E_START);
	cNode* pDestNode = FindNode(cNode::E_DEST);

	pStartNode->G = 0.0f;
	pStartNode->H = CalcHeuristic(pStartNode, pDestNode);
	pStartNode->F = pStartNode->G + pStartNode->H;

	MoveToOpenList(pStartNode);

	while (true)
	{
		cNode* pMinFNode = FindMinFNodeAtOpenList();
		if (pMinFNode == NULL)
		{
			// 길 못찾음
			MarkNodeType(pDestNode);
			break;
		}

		MoveToCloseList(pMinFNode);

		Extend(pMinFNode, pDestNode);

		if (pMinFNode == pDestNode)
		{
			// 길 찾음
			MarkNodeType(pDestNode);
			break;
		}
	}
}

void cMainGame::Render(HDC hdc)
{
	for each(auto p in m_vecNode)
	{
		p->Render(hdc);
	}
}

cNode * cMainGame::FindNode(cNode::eNodeType e)
{
	for each(auto p in m_vecNode)
	{
		if (p->m_eNodeType == e)
			return p;
	}
	return nullptr;
}

void cMainGame::MoveToOpenList(cNode * pNode)
{
	pNode->m_isOpen = true;
	m_setOpenList.insert(pNode);
}

void cMainGame::MoveToCloseList(cNode * pNode)
{
	pNode->m_isOpen = false;
	pNode->m_isClose = true;
	m_setOpenList.erase(pNode);
}

float cMainGame::CalcHeuristic(cNode * pNode1, cNode * pNode2)
{
	return abs(pNode1->m_nRow - pNode2->m_nRow) + 
		abs(pNode1->m_nCol - pNode2->m_nCol);
}

cNode * cMainGame::FindMinFNodeAtOpenList()
{
	cNode* pMinFNode = NULL;
	float fMinF = FLT_MAX;
	for each(auto p in m_setOpenList)
	{
		if (p->F < fMinF)
		{
			pMinFNode = p;
			fMinF = p->F;
		}
	}
	return pMinFNode;
}

void cMainGame::Extend(cNode * pExtNode, cNode * pDestNode)
{
	for each(int d in m_vecDirection)
	{
		cNode* pAdjNode = GetAdjNode(pExtNode, d);
		if (pAdjNode == NULL) continue;
		if (pAdjNode->m_isOpen)
		{
			if (pAdjNode->G > pExtNode->G + m_mapDistance[d])
			{
				pAdjNode->G = pExtNode->G + m_mapDistance[d];
				//pAdjNode->H = CalcHeuristic(pAdjNode, pDestNode);
				pAdjNode->F = pAdjNode->G + pAdjNode->H;
				pAdjNode->m_pParent = pExtNode;
			}
		}
		else
		{
			pAdjNode->G = pExtNode->G + m_mapDistance[d];
			pAdjNode->H = CalcHeuristic(pAdjNode, pDestNode);
			pAdjNode->F = pAdjNode->G + pAdjNode->H;
			pAdjNode->m_pParent = pExtNode;
			MoveToOpenList(pAdjNode);
		}
	}
}

cNode * cMainGame::GetAdjNode(cNode * pExtNode, int nDirection)
{
	int nAdjRow = pExtNode->m_nRow;
	int nAdjCol = pExtNode->m_nCol;

	if (nDirection & E_UP)
	{
		if (nAdjRow == 0) return NULL;
		int nIndex = (pExtNode->m_nRow - 1) * TILE_N + pExtNode->m_nCol;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL) return NULL;
		nAdjRow -= 1;
	}
	else if (nDirection & E_DOWN)
	{
		if (nAdjRow == TILE_N - 1) return NULL;
		int nIndex = (pExtNode->m_nRow + 1) * TILE_N + pExtNode->m_nCol;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL) return NULL;
		nAdjRow += 1;
	}

	if (nDirection & E_LEFT)
	{
		if (nAdjCol == 0) return NULL;
		int nIndex = (pExtNode->m_nRow) * TILE_N + pExtNode->m_nCol - 1;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL) return NULL;
		nAdjCol -= 1;
	}
	else if (nDirection & E_RIGHT)
	{
		if (nAdjCol == TILE_N - 1) return NULL;
		int nIndex = (pExtNode->m_nRow) * TILE_N + pExtNode->m_nCol + 1;
		if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL) return NULL;
		nAdjCol += 1;
	}

	int nIndex = nAdjRow * TILE_N + nAdjCol;
	if (m_vecNode[nIndex]->m_isClose) return NULL;
	if (m_vecNode[nIndex]->m_eNodeType == cNode::E_WALL) return NULL;
	
	return m_vecNode[nIndex];
}

void cMainGame::MarkNodeType(cNode * pDestNode)
{
	for each(auto p in m_vecNode)
	{
		if (p->m_isOpen) p->m_eNodeType = cNode::E_OPEN;
		if (p->m_isClose) p->m_eNodeType = cNode::E_CLOSE;
	}

	cNode* pTemp = pDestNode;
	while (pTemp)
	{
		pTemp->m_eNodeType = cNode::E_PATH;
		pTemp = pTemp->m_pParent;
	}
}
