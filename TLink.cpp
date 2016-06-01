#include "TLink.h"

TLink::TLink(const char *_str, TLink *_pNext, TLink *_pDown)
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
	if (TextMem.pFree != NULL)
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
	TextMem.pLast = TextMem.pFirst + (size - 1);
	TLink *pCurrent = TextMem.pFirst;
	for (unsigned int i = 0; i < size - 1; i++)
	{
		pCurrent->str[0] = '\0';
		pCurrent->pNext = pCurrent + 1;
		pCurrent = pCurrent + 1;
	}
	pCurrent->str[0] = '\0';
	pCurrent->pNext = NULL;
}

void TLink::MemClean(TText &txt)
{
	for (txt.Reset(); !txt.IsEnd(); txt.GoNext())
	{
		string tmp = "+";
		tmp += txt.GetLine();
		txt.SetLine(tmp.c_str());
	}
	string tmp_str = "+";
	tmp_str += txt.GetLine();
	txt.SetLine(tmp_str.c_str());
	TLink *tmp = TextMem.pFree;
	while (tmp != NULL)
	{
		tmp->str[0] = '+';
		tmp->str[1] = '\0';
		tmp = tmp->pNext;
	}
	tmp = TextMem.pFirst;
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
		int c = 0;
		if (tmp->str[0] != '\0')
			cout << tmp->str << endl;
		tmp = tmp->pNext;
		c++;
	}
}
