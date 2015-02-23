/*************************************************************************
	> File Name: list.cpp
	> Author: junkizou
	> Mail: junkizou@tencent.com 
	> Created Time: 日  2/22 20:16:16 2015
 ************************************************************************/

#include "dlist_c++.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

CListManage::CListManage(int nNum)
{
	if(m_nListSize == 0)
	{
		CreateList(nNum);
	}
}

CListManage::~CListManage(void)
{
	if(m_nListSize != 0)
	{
		ClearList();
	}
}

int CListManage::CreateList(int nNum)
{
	if( 0 == m_nListSize ) 
	{
		//创建头节点
		bool bRes = AddData(nNum);
		if(!bRes)
		{
			return -1;
		}
		return 0;
	}

	return m_nListSize;
}

bool CListManage::AddData(int nNum)
{
	CListData* pData = CreateData(nNum);
	if(NULL == pData)
	{
		return false;
	}
	if(m_pHead == NULL && m_pEnd == NULL)
	{
		pData->pPre = NULL;
		m_pHead = pData;
	}
	else
	{
		m_pEnd->pNext = pData;
		pData->pPre = m_pEnd;
	}

	m_pEnd = pData;
	pData->pNext = NULL;
	m_nListSize++;

	return true;
}

int CListManage::InsertData(int nNum, int nPos, TYPE_INSERT typeInsert)
{
	if(0 == m_nListSize)
	{
		return -1;
	}

	CListData *pData = CreateData(nNum);
	if(NULL == pData)
	{
		return -1;
	}

	CListData *pCur = GetData(nPos);
	if(NULL == pCur)
	{
		return -1;
	}

	CListData *pFrontOrBack = NULL;
	switch(typeInsert)
	{
		case FRONT:
			{
				if(pCur == m_pHead)
				{
					pData->pPre = NULL;
					pData->pNext = m_pHead;
					m_pHead->pPre = pData;
					m_pHead = pData;
				}
				else
				{
					pFrontOrBack = pCur->pPre;
					pFrontOrBack->pNext = pData;
					pData->pPre = pFrontOrBack;
					pData->pNext = pCur;
					pCur->pPre = pData;
				}

			}
			break;
		case BACK:
			{
				if(pCur == m_pEnd)
				{
					pData->pNext = NULL;
					pData->pPre = pCur;
					pCur->pNext = pData;
					m_pEnd = pData;
				}
				else
				{
					pFrontOrBack = pCur->pNext;
					pData->pNext = pFrontOrBack;
					pData->pPre = pCur;
					pCur->pNext = pData;
					pFrontOrBack->pPre = pData;
				}

			}
			break;
		default:
			if(NULL != pData)
			{
				delete pData;
				pData = NULL;
			}
			return -2;
	}

	m_nListSize++;
	return 0;
}

bool CListManage::DeleteData(int nPos)
{
	if(nPos > m_nListSize && nPos != 0)
	{
		return false;
	}

	CListData* pCur = GetData(nPos);
	if(NULL == pCur)
	{
		return false;
	}

	CListData *pFront = NULL, *pBack = NULL;
	if(pCur == m_pHead && pCur == m_pEnd)
	{
		m_pHead = NULL;
		m_pEnd = NULL;
	}
	else if(pCur == m_pHead)
	{
		pBack = m_pHead->pNext;
		pBack->pPre = NULL;
		m_pHead = pBack;
	}
	else if(pCur == m_pEnd)
	{
		pFront = m_pEnd->pPre;
		m_pEnd = pFront;
		pFront->pNext = NULL;
	}
	else
	{
		pFront = pCur->pPre;
		pBack = pCur->pNext;
		pFront->pNext = pBack;
		pBack->pPre = pFront;
	}

	delete pCur;
	pCur = NULL;
	m_nListSize--;

	return true;
}

CListData* CListManage::GetData(int nPos)
{
	if(nPos > m_nListSize && nPos != 0)
	{
		return NULL;
	}
	CListData *pData = m_pHead;
	for(int i = 0; i< m_nListSize; i++)
	{
		if(i == nPos - 1)
		{
			return pData;
		}
		pData = pData->pNext;
	}
	return NULL;
}


void CListManage::PrintList(void)
{
	if(m_nListSize == 0)
	{
		return;
	}

	CListData *pCur = m_pHead;
	while(pCur != NULL)
	{
		printf("%d\t", pCur->data);
		pCur = pCur->pNext;
	}
	printf("\n");
	return;
}

bool CListManage::ClearList(void)
{
	int size = GetListSize();
	if(size == 0)
	{
		return true;
	}

	CListData *pData = m_pEnd->pPre;
	int nCount = size;
	for(int i = 0;i< nCount;i++)
	{
		if(NULL != m_pEnd)
		{
			delete m_pEnd;
			m_pEnd = NULL;
		}
		m_pEnd = pData;
		if(NULL != m_pEnd && NULL != m_pEnd->pPre)
		{
			pData = m_pEnd->pPre;
		}
		m_nListSize--;
	}
	return true;
}

bool CListManage::ShowData(int nPos, int& nNum)
{
	CListData *pData = GetData(nPos);
	if(NULL == pData)
	{
		return false;
	}

	nNum = pData->data;

	return true;
}

int main(int argc, char* argv[])
{
	int nRet = 0;

	return 0;
}

