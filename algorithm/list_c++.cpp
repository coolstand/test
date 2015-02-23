/*************************************************************************
	> File Name: list.cpp
	> Author: junkizou
	> Mail: junkizou@tencent.com 
	> Created Time: 日  2/22 20:16:16 2015
 ************************************************************************/

#include "list_c++.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;


void InitList(List* list)
{
	list->pHead = NULL;
	list->pTail = NULL;
	list->size = 0;
	return;
}

void CreateList(List* list, int data)
{
	if(GetSize(list) != 0)
	{
		return;
	}

	ListElmt *pNew = AddElmt(data);
	list->pHead = pNew;
	list->pTail = pNew;
	pNew->pNext = NULL;
	list->size = 1;

	return;
}

void AddNode(List *list, int data)
{
	if(list->pHead == NULL && list->pTail == NULL)
	{
		CreateList(list, data);
		return;
	}
	ListElmt *pNew = AddElmt(data);
	list->pTail->pNext = pNew;
	list->pTail = pNew;
	pNew->pNext = NULL;
	list->size++;

	return;
}

ListElmt* GetPos(List *list, int data)
{
	if(list->size == 0)
	{
		return NULL;
	}
	ListElmt* pCur = list->pHead;
	while(pCur != NULL)
	{
		if(pCur->data == data)
		{
			return pCur;
		}
		pCur = pCur->pNext;
	}

	return NULL;
}

bool DeleteNode(List* list, int data)
{
	if(list->size == 0)
	{
		return false;
	}
	//pre用于保存cur的上一个节点
	ListElmt *pCur = list->pHead, *pPre;
	while(pCur != NULL)
	{
		if(pCur->data == data)
		{
			break;
		}
		pPre = pCur;
		pCur = pCur->pNext;
	}
	if(pCur == list->pHead && pCur == list->pTail)
	{
		list->pHead = NULL;
		list->pTail = NULL;
	}
	else if(pCur == list->pHead)
	{
		list->pHead = list->pHead->pNext;
	}
	else
	{
		pPre->pNext = pCur->pNext;
	}

	pCur = NULL;
	list->size--;

	return true;
}

bool DeleteNodeOrder(List *list)
{
	if(list->size == 0)
	{
		return false;
	}

	ListElmt *pCur = list->pHead;

	list->pHead = list->pHead->pNext;

	pCur = NULL;
	list->size--;

	return true;
}

void PrintList(List *list)
{
	if(list->size == 0)
	{
		return;
	}

	ListElmt *pCur = list->pHead;
	while(pCur != NULL)
	{
		printf("%d\t", pCur->data);
		pCur = pCur->pNext;
	}
	printf("\n");
	return;
}

bool ClearList(List *list)
{
	int size = GetSize(list);
	if(size == 0)
	{
		return true;
	}

	while(GetSize(list)>0)
	{
		DeleteNodeOrder(list);
		PrintList(list);
	}


	printf("now List size is: %d\n", list->size);

	return true;
}

void ReverseList(List* list)
{
	if(list->size == 0)
	{
		return;
	}

	ListElmt *Pre = list->pHead;
	ListElmt *Cur = list->pHead->pNext;
	ListElmt *Next;
	while(Cur != NULL)
	{
		Next = Cur->pNext;
		Cur->pNext = Pre;
		Pre = Cur;
		Cur = Next;
	}
	list->pHead->pNext = NULL;
	list->pHead = Pre;

	PrintList(list);

	return;
}

int main(int argc, char* argv[])
{
	int nRet = 0;

	List *list = new List;
	InitList(list);
	printf("InitList passed!\n");
	CreateList(list, 1);
	printf("CreateList passed!\n");

	for(int i=2; i<= 9 ;i++)
	{
		AddNode(list, i);
	}
	printf("AddNode passed!\n");

	PrintList(list);
	printf("PrintList passed!\n");

	DeleteNode(list, 4);
	printf("DeleteNode passed!\n");

	PrintList(list);
	printf("PrintList passed!\n");

	DeleteNode(list, 9);
	printf("DeleteNode passed!\n");

	PrintList(list);
	printf("PrintList passed!\n");

	DeleteNode(list, 1);
	printf("DeleteNode passed!\n");

	PrintList(list);
	printf("PrintList passed!\n");

	//ClearList(list);
	//printf("ClearList passed!\n");

	ReverseList(list);
	printf("ReverseList passed!\n");
	return 0;
}

