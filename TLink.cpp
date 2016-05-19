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
	tmp->pDown = NULL;
}

void TLink::InitMem(size_t size)
{
	TextMem.pFirst = (TLink *)new char[size * sizeof(TLink)];
	TextMem.pFree = TextMem.pFirst;
	TextMem.pLast = TextMem.pFirst + size - 1;
	TLink *pCurrent = TextMem.pFirst;
	for (unsigned int i = 0; i < size - 1; i++)
	{
		pCurrent->str[0] = '\0';
		pCurrent->pNext = pCurrent + 1;
		pCurrent = pCurrent->pNext;
	}
	pCurrent->str[0] = '\0';
	pCurrent->pNext = NULL;
}

void TLink::MemClean(TText &txt)
{
	for (txt.Reset(); !txt.IsEnd(); txt.GoNext())
	{
		char tmp[MaxLen] = "+";
		strcat_s(tmp, MaxLen, txt.GetLine());
		txt.SetLine(tmp);
	}
	TLink *tmp = TextMem.pFree;
	while (tmp != NULL)
	{
		tmp->str[0] = '+';
		tmp->str[1] = '\0';
		tmp = tmp->pNext;
	}
	for (tmp = TextMem.pFirst; tmp < TextMem.pLast; tmp++)
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
		if (tmp->str[0] != '\0')
			cout << tmp->str << endl;
		tmp = tmp->pNext;
	}
}
