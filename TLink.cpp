#include "TLink.h"

TLink::TLink(char *_str, TLink *_pNext, TLink *_pDown)
{
	pNext = _pNext;
	pDown = _pDown;
	if (_str != NULL)
		strcpy_s(str, _str);
	else
		str[0] = '\0';
}

void *TLink::operator new(size_t size)
{
	TLink *tmp = TextMem.pFree;
	if (TextMem.pFree == NULL)
		throw "NULL Pointer pFree";
	TextMem.pFree = TextMem.pFree->pNext;
	return tmp;
}

void TLink::operator delete(void *pointer)
{
	TLink *tmp = (TLink *)pointer;
	tmp->pNext = TextMem.pFree;
	TextMem.pFree = tmp;
}

void TLink::InitMem(size_t size)
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

void TLink::MemClean(TText &txt)
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

void TLink::PrintFree()
{
	TLink *tmp = TextMem.pFree;
	while (tmp != NULL)
	{
		cout << tmp << " ";
		tmp = tmp->pNext;
	}
}
