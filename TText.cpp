#include "TText.h"

TText::TText(TLink *pointer)
{
	if (pointer == NULL)
		pFirst = new TLink;
	else
		pFirst = pointer;
}

TLink* TText::ReadSection(ifstream &_file)
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
			pTmp->pDown = ReadSection(_file);
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

void TText::PrintSection(TLink *pTmp)
{
	if (pTmp == NULL)
		throw "NULL Pointer pTmp";
	cout << pTmp->str << endl;
	if (pTmp->pDown != NULL)
		PrintSection(pTmp->pDown);
	if (pTmp->pNext != NULL)
		PrintSection(pTmp->pNext);
}

void TText::SaveSection(TLink *pTmp, ofstream &_file)
{
	if (pTmp == NULL)
		throw "NULL Pointer pTmp";
	_file << pTmp->str << endl;
	PrintSection(pTmp->pDown);
	PrintSection(pTmp->pNext);
}

void TText::Reset()
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

void TText::GoNext()
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

bool TText::IsEnd()
{
	return path.IsEmpty();
}

int TText::GoNextLink()
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

int TText::GoDownLink()
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

int TText::GoPrevLink()
{
	if (!path.IsEmpty())
		pCurrent = path.Pop();
	return 0;
}

int TText::GoFirstLink()
{
	while (!path.IsEmpty())
		pCurrent = path.Pop();
	return 0;
}

int TText::InsNextLine(char *_str)
{
	if (pCurrent == NULL)
		throw "NULL Pointer pCurrent";
	TLink *tmp = new TLink(_str);
	tmp->pNext = pCurrent->pNext;
	pCurrent->pNext = tmp;
	return 0;
}

int TText::InsNextSection(char *_str)
{
	if (pCurrent == NULL)
		throw "NULL Pointer pCurrent";
	TLink *tmp = new TLink(_str);
	tmp->pDown = pCurrent->pNext;
	pCurrent->pNext = tmp;
	return 0;
}

void TText::InsDownLine(char *_str)
{
	if (pCurrent == NULL)
		throw "NULL Pointer pCurrent";
	TLink *tmp = new TLink(_str);
	tmp->pNext = pCurrent->pDown;
	pCurrent->pDown = tmp;
}

void TText::InsDownSection(char *_str)
{
	if (pCurrent == NULL)
		throw "NULL Pointer pCurrent";
	TLink *tmp = new TLink(_str);
	tmp->pDown = pCurrent->pDown;
	pCurrent->pDown = tmp;
}

void TText::DelNextLine()
{
	if (pCurrent == NULL)
		throw "NULL Pointer pCurrent";
	if (pCurrent->pNext == NULL)
		throw "NULL Pointer pCurrent->pNext";
	TLink* tmp = pCurrent->pNext;
	pCurrent->pNext = pCurrent->pNext->pNext;
	delete tmp;
}

void TText::DelDownLine()
{
	if (pCurrent == NULL)
		throw "NULL Pointer pCurrent";
	if (pCurrent->pDown == NULL)
		throw "NULL Pointer pCurrent->pDown";
	TLink* tmp = pCurrent->pDown;
	pCurrent->pDown = pCurrent->pDown->pNext;
	delete tmp;
}

char *TText::GetLine()
{
	if (pCurrent == NULL)
		throw "NULL Pointer pCurrent";
	return pCurrent->str;
}

void TText::SetLine(char *_str)
{
	if (pCurrent == NULL)
		throw "NULL Pointer pCurrent";
	if (strlen(_str) < MaxLen)
		strcpy_s(pCurrent->str, _str);
}

void TText::Load(char *_str)
{
	ifstream _file(_str);
	pFirst = ReadSection(_file);
}

void TText::Print()
{
	PrintSection(pFirst);
}
