#pragma once

#include "cNode.h"

class cMainGame
{
private:
	std::vector<cNode*> m_vecNode;
	std::set<cNode*>	m_setOpenList;

	enum
	{
		E_LEFT	= 1 << 0,
		E_RIGHT = 1 << 1,
		E_UP	= 1 << 2,
		E_DOWN	= 1 << 3,
	};

	std::vector<int>	m_vecDirection;
	std::map<int, float>		m_mapDistance;
public:
	cMainGame();
	~cMainGame();

	void Setup();
	void FindPath();
	void Render(HDC hdc);

	cNode* FindNode(cNode::eNodeType e);
	void MoveToOpenList(cNode* pNode);
	void MoveToCloseList(cNode* pNode);
	float CalcHeuristic(cNode* pNode1, cNode* pNode2);
	cNode* FindMinFNodeAtOpenList();
	void Extend(cNode* pExtNode, cNode* pDestNode);
	cNode* GetAdjNode(cNode* pExtNode, int nDirection);
	void MarkNodeType(cNode* pDestNode);
};

