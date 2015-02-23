/*************************************************************************
	> File Name: /Users/junkizou/workspace/cplusplus/test/list.h
	> Author: junkizou
	> Mail: junkizou@tencent.com 
	> Created Time: 日  2/22 20:16:51 2015
 ************************************************************************/
#ifndef _DOUBLE_LIST_H
#define _DOUBLE_LIST_H

#include <iostream>
using namespace std;

typedef enum{
	FRONT =1,
	BACK
} TYPE_INSERT;

class CListData
{
	public:
		int data;
		CListData *pNext;
		CListData *pPre;
};

class CListManage
{
	public:
		CListManage(void);
		CListManage(int nNum);
		~CListManage(void);

		inline int GetListSize(void) const
		{
			return m_nListSize;
		}
		int CreateList(int nNum);
		bool AddData(int nNum);
		int InsertData(int nNum, int nPos, TYPE_INSERT typeInsert = FRONT);
		CListData* GetData(int nPos);
		bool ClearList();
		bool DeleteData(int nPos);
		inline bool IsEmpty(void)
		{
			if( 0 == m_nListSize)
			{
				return true;
			}
			return false;
		}
		bool ShowData(int nPos, int& nNum);
		void PrintList(void);

	private:
		int m_nListSize;
		CListData* m_pHead;
		CListData* m_pEnd;
		inline CListData* CreateData(int nNum)
		{
			CListData *pNew = new CListData;
			if(pNew != NULL)
			{
				pNew->data = nNum;
				return pNew;
			}
			return NULL;
		}
};


#endif
