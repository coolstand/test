/*************************************************************************
	> File Name: /Users/junkizou/workspace/cplusplus/test/list.h
	> Author: junkizou
	> Mail: junkizou@tencent.com 
	> Created Time: 日  2/22 20:16:51 2015
 ************************************************************************/
#ifndef _LIST_H
#define _LIST_H


#include <iostream>
using namespace std;

typedef struct ListElmt_
{
	int data;
	struct ListElmt_ *pNext;
} ListElmt;

typedef struct List_
{
	int size;
	ListElmt* pHead;
	ListElmt* pTail;
} List;

void InitList(List* list);
void CreateList(List* list, int data);
inline int GetSize(List* list)
{
	return list->size;
}
inline ListElmt* AddElmt(int data)
{
	ListElmt *pNew = new ListElmt;
	if(pNew != NULL)
	{
		pNew->data = data;
		return pNew;
	}
	return NULL;
}
void AddNode(List *list, int data);
ListElmt* GetPos(List *list, int data);
bool DeleteNode(List *list, int data);
bool DeleteNodeOrder(List *list);
bool ClearList(List *list);
void ReverseList(List *list);

#endif
