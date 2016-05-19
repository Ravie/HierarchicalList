#pragma once
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cstring>
#include "Stack.h"

using namespace std;

class TText;
class TLink;
struct TTextMem { TLink *pFirst, *pLast, *pFree; };

const int MaxLen = 81;

class TLink
{
public:
	char str[MaxLen];
	TLink *pNext, *pDown;
	static TTextMem TextMem;

	void *operator new(size_t size)
	{
		TLink *tmp = TextMem.pFree;
		if (TextMem.pFree == NULL)
			throw "NULL Pointer pFree";
		TextMem.pFree = TextMem.pFree->pNext;
		return tmp;
	}

	void operator delete(void *pointer)
	{
		TLink *tmp = (TLink *)pointer;
		tmp->pNext = TextMem.pFree;
		TextMem.pFree = tmp;
	}

	static void InitMem(size_t size)
	{
		TextMem.pFirst = (TLink *)new char[size * sizeof(TLink)];
		TextMem.pFree = TextMem.pFirst;
		TextMem.pLast = TextMem.pFirst + size - 1;
		TLink *pCurrent = TextMem.pFirst;
		for (unsigned int i = 0; i < size - 1; i++)
		{
			pCurrent->pNext = pCurrent + 1;
			pCurrent = pCurrent->pNext;
		}
		pCurrent->pNext = NULL;
	}

	static void MemClean(TText &txt)
	{
		for (txt.Reset(); !txt.IsEnd(); txt.GoNext())
		{
			char tmp[MaxLen] = "+";
			strcpy_s(tmp, txt.GetLine());
			txt.SetLine(tmp);
		}
		TLink *tmp = TextMem.pFree;
		while (tmp != NULL)
		{
			tmp->str[0] = '+';
			tmp->str[1] = '\0';
			tmp = tmp->pNext;
		}
		tmp = TextMem.pFirst;
		for (int i = 0; i < MaxLen; i++)
		{
			if (tmp->str[0] != '+')
				delete tmp;
			else
				strcpy_s(tmp->str, tmp->str + 1);
		}
	}

	static void PrintFree()
	{
		TLink *tmp = TextMem.pFree;
		while (tmp != NULL)
		{
			cout << tmp << " ";
			tmp = tmp->pNext;
		}
	}

	TLink(char *_str = NULL, TLink *_pNext = NULL, TLink *_pDown = NULL)
	{
		pNext = _pNext;
		pDown = _pDown;
		if (_str != NULL)
			strcpy_s(str, _str);
		else
			str[0] = '\0';
	}

	~TLink() {}
};

class TText
{
private:
	TStack<TLink *> path;

	TLink *pFirst, *pCurrent;

	TLink* ReadSection(ifstream &_file)
	{
		char _str[MaxLen];
		TLink *pFirstInSection, *pTmp;
		pFirstInSection = new TLink;
		pTmp = pFirstInSection;
		while (!_file.eof())
		{
			_file.getline(_str, MaxLen);
			if (strchr(_str, '}'))
				break;
			if (strchr(_str, '{'))
				pFirstInSection->pDown = ReadSection(_file);
			else
			{
				TLink *NewLink = new TLink(_str);
				pTmp->pNext = NewLink;
				pTmp = NewLink;
			}
		}
		if (pFirstInSection->pDown == NULL)
		{
			pTmp = pFirstInSection->pNext;
			delete pFirstInSection;
			pFirstInSection = pTmp;
		}
		return pFirstInSection;
	}

	void PrintSection(TLink *pTmp)
	{
		if (pTmp == NULL)
			throw "NULL Pointer pTmp";
		cout << pTmp->str << endl;
		PrintSection(pTmp->pDown);
		PrintSection(pTmp->pNext);
	}

	void SaveSection(TLink *pTmp, ofstream &_file)
	{
		if (pTmp == NULL)
			throw "NULL Pointer pTmp";
		_file << pTmp->str << endl;
		PrintSection(pTmp->pDown);
		PrintSection(pTmp->pNext);
	}
public:
	void Reset()
	{
		path.ClearStack();
		pCurrent = pFirst;
		if (pCurrent != NULL)
		{
			if (pCurrent->pNext != NULL)
				path.PushElem(pCurrent->pNext);
			if (pCurrent->pDown != NULL)
				path.PushElem(pCurrent->pDown);
		}
	}

	void GoNext()
	{
		if (!path.IsEmpty())
		{
			pCurrent = path.Pop();
			if (pCurrent->pNext != NULL)
				path.PushElem(pCurrent->pNext);
			if (pCurrent->pDown != NULL)
				path.PushElem(pCurrent->pDown);
		}
	}

	bool IsEnd()
	{
		return path.IsEmpty();
	}

	TText(TLink *pointer = NULL)
	{
		if (pointer == NULL)
			pFirst = new TLink;
		else
			pFirst = pointer;
	}

	~TText() {}

	int GoNextLink()
	{
		if (pCurrent == NULL)
		{
			if (pCurrent->pNext == NULL)
				throw "NULL Pointer pCurrent->pNext";
			throw "NULL Pointer pCurrent";
		}
		path.PushElem(pCurrent);
		pCurrent = pCurrent->pNext;
		return 0;
	}

	int GoDownLink()
	{
		if (pCurrent == NULL)
		{
			if (pCurrent->pDown == NULL)
				throw "NULL Pointer pCurrent->pDown";
			throw "NULL Pointer pCurrent";
		}
		path.PushElem(pCurrent);
		pCurrent = pCurrent->pDown;
		return 0;
	}

	int GoPrevLink()
	{
		if (!path.IsEmpty())
			pCurrent = path.Pop();
		return 0;
	}

	int GoFirstLink()
	{
		while (!path.IsEmpty())
			pCurrent = path.Pop();
		return 0;
	}

	int InsNextLine(char *_str)
	{
		if (pCurrent == NULL)
			throw "NULL Pointer pCurrent";
		TLink *tmp = new TLink(_str);
		tmp->pNext = pCurrent->pNext;
		pCurrent->pNext = tmp;
		return 0;
	}

	int InsNextSection(char *_str)
	{
		if (pCurrent == NULL)
			throw "NULL Pointer pCurrent";
		TLink *tmp = new TLink(_str);
		tmp->pDown = pCurrent->pNext;
		pCurrent->pNext = tmp;
		return 0;
	}

	void InsDownLine(char *_str)
	{
		if (pCurrent == NULL)
			throw "NULL Pointer pCurrent";
		TLink *tmp = new TLink(_str);
		tmp->pNext = pCurrent->pDown;
		pCurrent->pDown = tmp;
	}

	void InsDownSection(char *_str)
	{
		if (pCurrent == NULL)
			throw "NULL Pointer pCurrent";
		TLink *tmp = new TLink(_str);
		tmp->pDown = pCurrent->pDown;
		pCurrent->pDown = tmp;
	}

	void DelNextLine()
	{
		if (pCurrent == NULL)
			throw "NULL Pointer pCurrent";
		if (pCurrent->pNext == NULL)
			throw "NULL Pointer pCurrent->pNext";
		TLink* tmp = pCurrent->pNext;
		pCurrent->pNext = pCurrent->pNext->pNext;
		delete tmp;
	}

	void DelDownLine()
	{
		if (pCurrent == NULL)
			throw "NULL Pointer pCurrent";
		if (pCurrent->pDown == NULL)
			throw "NULL Pointer pCurrent->pDown";
		TLink* tmp = pCurrent->pDown;
		pCurrent->pDown = pCurrent->pDown->pNext;
		delete tmp;
	}

	char *GetLine()
	{
		if (pCurrent == NULL)
			throw "NULL Pointer pCurrent";
		return pCurrent->str;
	}

	void SetLine(char *_str)
	{
		if (pCurrent == NULL)
			throw "NULL Pointer pCurrent";
		if (strlen(_str) < MaxLen)
			strcpy_s(pCurrent->str, _str);
	}

	void Load(char *_str)
	{
		ifstream _file(_str);
		pFirst = ReadSection(_file);
	}

	void Print()
	{
		PrintSection(pFirst);
	}
};

